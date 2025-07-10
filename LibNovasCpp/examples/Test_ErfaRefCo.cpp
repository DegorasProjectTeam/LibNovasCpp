/*
  NOVAS 3.1 Example Calculations

  Written for use with solsys version 3.

  Refactorized for C++ compilers.
 */

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

/**
 * @brief Compute atmospheric refraction correction for a given zenith distance.
 *
 * Calculates the correction dZ in radians to be added to the observed zenith distance,
 * based on pressure, temperature, relative humidity and wavelength.
 * Uses the ERFA model: dZ = A * tan(Z) + B * tan^3(Z).
 *
 * @param phpa     Pressure at observer in hPa
 * @param tc       Ambient temperature in degrees Celsius
 * @param rh       Relative humidity (0.0 to 1.0)
 * @param wl       Wavelength in micrometers (e.g. 0.532 for 532 nm)
 * @param zenith   Observed zenith angle in radians (i.e., 90° - elevation)
 * @return double  Correction dZ in radians to be added to zenith
 */
double iauRefractionCorrection(double phpa, double tc, double rh, double wl, double zenith)
{
    int optic;
    double p, t, r, w, ps, pw, tk, wlsq, gamma, beta;

    /* Decide whether optical/IR or radio case:  switch at 100 microns. */
    optic = ( wl <= 100.0 );

    /* Restrict parameters to safe values. */
    t = std::clamp(tc, -150.0, 200.0);
    p = std::clamp(phpa, 0.0, 10000.0);
    r = std::clamp(rh, 0.0, 1.0);
    w = std::clamp(wl, 0.1, 1e6);

    /* Water vapour pressure at the observer. */
    if ( p > 0.0 )
    {
        ps = std::pow ( 10.0, ( 0.7859 + 0.03477*t ) /
                           ( 1.0 + 0.00412*t ) ) *
             ( 1.0 + p * ( 4.5e-6 + 6e-10*t*t )  );
        pw = r * ps / ( 1.0 - (1.0-r)*ps/p );
    }
    else
    {
        pw = 0.0;
    }

    /* Refractive index minus 1 at the observer. */
    tk = t + 273.15;
    if ( optic )
    {
        wlsq = w * w;
        gamma = ( ( 77.53484e-6 +
                  ( 4.39108e-7 + 3.666e-9/wlsq ) / wlsq ) * p
                 - 11.2684e-6*pw ) / tk;
    }
    else
    {
        gamma = ( 77.6890e-6*p - ( 6.3938e-6 - 0.375463/tk ) * pw ) / tk;
    }

    /* Formula for beta from Stone, with empirical adjustments. */
    beta = 4.4474e-6 * tk;
    if ( ! optic )
        beta -= 0.0074 * pw * beta;

    // Compute the refraction constants.
    double refa = gamma * (1.0 - beta);
    double refb = -gamma * (beta - gamma / 2.0);
    double tanz = std::tan(zenith);

    // Return the zenith correction.
    return refa * tanz + refb * tanz * tanz * tanz;
}


int main()
{

    // const double temperature_degc = 7.0;
    // const double pressure_hpa= 1005.0;
    // const double obs_wl_um = 0.574;
    // const double rel_hum = 0.8;
    // const double elevation_deg = 30.0;

    const double temperature_degc = 25.0;
    const double pressure_hpa= 1017.0;
    const double obs_wl_um = 0.532;
    const double rel_hum = 0.45;
    const double elevation_deg = 29.041298191349995;

    // Conversions.
    const double deg2rad = M_PI / 180.0;
    const double rad2arcsec = 180.0 * 3600.0 / M_PI;
    const double zenith_rad = (90.0 - elevation_deg) * deg2rad;

    // Compute refraction correction
    const double dz_rad = iauRefractionCorrection(pressure_hpa, temperature_degc, rel_hum, obs_wl_um, zenith_rad);
    const double dz_arcsec = dz_rad * rad2arcsec;
    const double dz_deg = dz_rad * (180.0 / M_PI);
    const double zenith_deg = zenith_rad * (180.0 / M_PI);
    const double corrected_zenith_deg = zenith_deg - dz_deg;
    const double corrected_elevation_deg = 90.0 - corrected_zenith_deg;

    // Output the result
    std::cout << "------------------------------------" << std::endl;
    std::cout << "REFRACTION CORRECTION CALCULATION:" << std::endl;
    std::cout << "Elevation: " << elevation_deg << " deg" << std::endl;
    std::cout << "Zenith angle: " << zenith_rad << " rad (" << zenith_deg << " deg)" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Temperature: " << temperature_degc << " °C" << std::endl;
    std::cout << "Pressure: " << pressure_hpa << " hPa" << std::endl;
    std::cout << "Humidity: " << rel_hum * 100.0 << " %" << std::endl;
    std::cout << "Wavelength: " << obs_wl_um << " um" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "dZ (radians): " << std::scientific << dz_rad << std::endl;
    std::cout << "dZ (arcseconds): " << std::fixed << std::setprecision(3) << dz_arcsec << std::endl;
    std::cout << "dZ (degrees): " << std::fixed << std::setprecision(6) << dz_deg << " deg" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Corrected zenith angle: " << corrected_zenith_deg << " deg" << std::endl;
    std::cout << "Corrected elevation:    " << corrected_elevation_deg << " deg" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // TEST

    std::vector<double> zd_deg_list = {10, 20, 30, 40, 45, 50, 55, 60, 65, 70, 72, 74, 76, 78, 80};
    std::vector<double> expected_arcsec = {10.27, 21.20, 33.61, 48.83, 58.18, 69.30, 82.99, 100.54,
                                           124.26, 158.68, 177.37, 200.38, 229.43, 267.29, 318.55};

    std::cout << "--------------------------------------------------\n";
    std::cout << " CHECKING REFRACTION CORRECTION TABLE:\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << " ZD (deg) | iauRefco (arcsec) | Expected | Diff\n";
    std::cout << "----------|-------------------|----------|--------\n";

    for (size_t i = 0; i < zd_deg_list.size(); ++i)
    {
        double zd_deg = zd_deg_list[i];
        double zd_rad = zd_deg * (M_PI / 180.0);
        double dz_rad = iauRefractionCorrection(pressure_hpa, temperature_degc, rel_hum, obs_wl_um, zd_rad);
        double dz_arcsec = dz_rad * (180.0 * 3600.0 / M_PI);
        double expected = expected_arcsec[i];
        double diff = dz_arcsec - expected;

        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(9) << zd_deg << " | "
                  << std::setw(17) << dz_arcsec << " | "
                  << std::setw(8) << expected << " | "
                  << std::showpos << std::setw(6) << std::setprecision(2) << diff << std::noshowpos << "\n";
    }

    std::cout << "--------------------------------------------------\n";

    // Final wait.
    std::cout << "Example finished. Press Enter to exit..." << std::endl;
    std::cout<<std::flush;
    std::cin.get();


    // All ok.
    return 0;
}


