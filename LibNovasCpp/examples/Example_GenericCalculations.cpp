/*
  NOVAS 3.1 Example Calculations

  Written for use with solsys version 3.

  Refactorized for C++ compilers.
 */

#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <chrono>

#include "LibNovasCpp/novas.h"

int main()
{
    // Example initial log.
    std::cout<<"-----------------------------------------------------------"<<std::endl;
    std::cout<<"  NOVAS Example: Calculate star local coordinates"<<std::endl;
    std::cout<<"-----------------------------------------------------------"<<std::endl;

    // Star configuration.
    const std::string star_name = "Vega";
    const std::string catalog_name = "FK5";
    const short int num = 699;
    const double ra = 18.615648986;
    const double dec = 38.78368896;
    const double pm_ra = 200.94;
    const double pm_dec = 287.78;
    const double parallax = 130.23;
    const double rad_vel = 20.0;

    // Datetime configuration.
    const unsigned year = 2023;
    const short int month = 10;
    const short int day = 18;
    const short int hour = 22;
    const short int min = 15;
    const double sec = 30.5;

    // Location (ITRS -> WGS84) and meteo configuration.
    const double latitude = 36.465257734376407939;
    const double longitude = -6.20530535896;
    const double height = 98.2496715541929;
    const double temperature = 25.8;
    const double pressure = 1024.1;

    // Time adjust and pole configuration.
    // Note: For most purposes setting the CIO coordinates to zero
    //       is ok when sub-arcsecond accuracy is unnecessary.
    const double x_pole = 0;
    const double y_pole = 0;
    const short int leap_secs = 37;  // Difference between UTC and TAI (from 2017-01-01).
    const double ut1_utc = 0.013616; // UT1 - UTC (2023-10-19)

    // Accuracy configuration.
    // Note: Reduced-accuracy mode (1) using NOVAS 2000K nutation series (488 terms) can
    //       be used when the requirements are not better than 0.1 milliarcsecond for stars
    //       or 3.5 milliarcseconds for solar system bodies. The computation time for these
    //       calculationsis thereby reduced by about two-thirds.
    const short int accuracy = 1;

    // Auxiliar variables.
    short int error = 0;
    double jd_utc, jd_tt, jd_ut1, delta_t, rat, dect,  zd, rar, decr, az, el;

    // NOVAS data structs.
    on_surface geo_loc;
    cat_entry star;

    // Start measuring time
    auto start_meas = std::chrono::steady_clock::now();

    // Establish time arguments.
    double fractional_part = (static_cast<double>(min)/1440.0) + (sec / 86400.0);
    jd_utc = julian_date(year,month,day,hour) + fractional_part;
    jd_tt = jd_utc + (static_cast<double>(leap_secs) + 32.184) / 86400.0; // TT = UTC + incrementAT + 32.184
    jd_ut1 = jd_utc + ut1_utc / 86400.0;
    delta_t = 32.184 + leap_secs - ut1_utc; // TT - UT1 in seconds.

    // Make the observer (on_surface) structure.
    make_on_surface (latitude,longitude,height,temperature,pressure, &geo_loc);

    // Make the star catalog entry containing the ICRS position and motion of the star.
    std::vector<char> name_ptr(star_name.c_str(), star_name.c_str() + star_name.size() + 1);
    std::vector<char> cat_ptr(catalog_name.c_str(), catalog_name.c_str() + catalog_name.size() + 1);
    make_cat_entry(name_ptr.data(), cat_ptr.data(), num, ra, dec, pm_ra, pm_dec, parallax, rad_vel, &star);

    // Apparent and topocentric place of star.
    if ((error = topo_star(jd_tt,delta_t,&star,&geo_loc, accuracy, &rat, &dect)) != 0)
    {
        std::cout<<"Error " << error << " from topo_star." <<std::endl;
        return (error);
    }

    // Transform the coordinates to az and zenith distance.
    equ2hor(jd_ut1, delta_t, accuracy, x_pole, y_pole, &geo_loc, rat, dect, 2, &zd, &az, &rar, &decr);

    // Get the elevation.
    el = 90.0 - zd;

    // Stop measuring time
    auto end_meas = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_meas - start_meas);

    // Log the data.
    std::cout << "------------------------------------" << std::endl;
    std::cout << "NEW TOPOCENTRIC POSITION:" << std::endl;
    std::cout << "ELAPSED CALC: " << elapsed.count() << "us" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "CATALOG: " << catalog_name << std::endl;
    std::cout << "STAR NAME: " << star_name << std::endl;
    std::cout << "STAR NUM: " << num << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "JULIAN UTC: " << std::fixed << std::setprecision(10) << jd_utc << std::endl;
    std::cout << "JULIAN TT: " << std::fixed << std::setprecision(10) << jd_tt << std::endl;
    std::cout << "JULIAN UT1: " << std::fixed << std::setprecision(10) << jd_ut1 << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "LOC LAT: " << std::fixed << std::setprecision(10) << latitude << std::endl;
    std::cout << "LOC LON: " << std::fixed << std::setprecision(10) << longitude << std::endl;
    std::cout << "LOC ALT: " << std::fixed << std::setprecision(10) << height << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "RA: " << std::fixed << std::setprecision(10) << rar << std::endl;
    std::cout << "DEC: " << std::fixed << std::setprecision(10) << decr << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "AZ: " << std::fixed << std::setprecision(10) << az << std::endl;
    std::cout << "EL: " << std::fixed << std::setprecision(10) << el << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // Final wait.
    std::cout << "Example finished. Press Enter to exit..." << std::endl;
    std::cout<<std::flush;
    std::cin.get();

    // All ok.
    return (0);
}

// ASTROPY OUTPUT:
/*
==============================================
Position of : vega
Time: 2023-10-18 22:15:30.005000
----------------------------------------------
279.23473476927495
38.783688950095936
----------------------------------------------
Az (deg): 297.17540785918834
El (deg): 32.07416454200478
----------------------------------------------
Az (rad): 5.18668932309987
El (rad): 0.5597997760844025
==============================================
*/

// NOVAS OUTPUT:
/*
-----------------------------------------------------------
  NOVAS Example: Calculate star local coordinates
-----------------------------------------------------------
------------------------------------
NEW TOPOCENTRIC POSITION:
ELAPSED CALC: 235us
------------------------------------
CATALOG: FK5
STAR NAME: Vega
STAR NUM: 699
------------------------------------
JULIAN UTC: 2460236.4274363425
JULIAN TT: 2460236.4282370834
JULIAN UT1: 2460236.4274364999
------------------------------------
LOC LAT: 36.4652577344
LOC LON: -6.2053053590
LOC ALT: 98.2496715542
------------------------------------
RA: 18.6308090278
DEC: 38.8194063721
------------------------------------
AZ: 297.1777162374
EL: 32.0753535966
------------------------------------
*/
