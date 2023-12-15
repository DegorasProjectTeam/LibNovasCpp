/*
 * This util was created originally in C and is not ported to C++ yet.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "LibNovasCpp/novas.h"

#define MAX_STARS 256
#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 256
#define MAX_SECTION_LENGTH 256
#define MAX_VALUE_LENGTH 256
#define MAX_POSITIONS_LENGTH 28800

struct Location
{
    double latitude;
    double longitude;
    double height;
};

struct Adjust
{
    double x_pole;
    double y_pole;
    double leap_secs;
    double ut1_utc;
};

struct Meteo
{
    double temperature;
    double pressure;
    double humidity;
};

struct Observation
{

    short int start_year;
    short int start_month;
    short int start_day;
    int start_hour;
    int start_min;
    double start_sec;
    short int end_year;
    short int end_month;
    short int end_day;
    int end_hour;
    int end_min;
    double end_sec;
    int limit_el_min;
    int limit_el_max;
    double inc;
};

struct Star
{
    char name[MAX_KEY_LENGTH];
    char catalog[MAX_KEY_LENGTH];
    int star_num;
    double ra;
    double dec;
    double pm_ra;
    double pm_dec;
    double parallax;
    double rad_vel;
};

struct StarPosition
{
    double jd_utc;
    double az;
    double el;
    double t_ra;
    double t_dec;
};

bool isWhitespaceLine(const char* line)
{
    while (*line)
    {
        if (!isspace((unsigned char)*line))
            return false;
        line++;
    }
    return true;
}

bool extractSection(const char* input, char* section)
{
    // Find the first '['
    const char* start = strchr(input, '[');
    if (start == NULL)
        return false;

    // Find the first ']' after the '['
    const char* end = strchr(start, ']');
    if (end == NULL)
        return false;

    // Move to the character after '['
    start++;

    // Calculate the length of the key
    size_t length = (size_t)(end - start);

    // Check if the key is too long for the provided buffer
    if (length >= MAX_SECTION_LENGTH)
        return false;

    // Copy the key to the extractedKey buffer
    strncpy(section, start, length);
    section[length] = '\0';

    // All ok.
    return true;
}

bool extractKeyValuePair(const char* input, char* key, char* value)
{
    // Find the first '=' character in the input
    const char* aux_key = input;
    size_t key_length = 0;

    // Calculate the length of the key.
    while (*aux_key && *aux_key != '=')
    {
        aux_key++;
        key_length++;
    }

    // Check if the key is too long for the provided buffer
    if (key_length >= MAX_KEY_LENGTH)
        return false;

    // Remove double quotes from the key (if they exist)
    char source[MAX_KEY_LENGTH];
    char destination[MAX_KEY_LENGTH];
    char* idx_src = source;
    char* idx_dst = destination;

    strncpy(source, input, key_length);
    source[key_length] = '\0';

    while(*idx_src != '\0')
    {
        if (*idx_src != '"')
        {
            *idx_dst = *idx_src;
            idx_dst++;
        }
        else
            key_length--;
        idx_src++;
    }
    *idx_dst = '\0';
    strncpy(key, destination, key_length);
    key[key_length] = '\0';

    // Move to the character after '='
    aux_key++;

    // Skip leading whitespace
    while (*aux_key && isspace((unsigned char)*aux_key))
        aux_key++;

    // Check if the value is enclosed in double quotes
    if (*aux_key == '"')
    {
        // Move past the opening double quote
        aux_key++;

        // Find the closing double quote
        const char* closing_quote = strchr(aux_key, '"');
        if (closing_quote == NULL)
            return false;

        // Calculate the length of the value
        size_t val_length = (size_t)(closing_quote - aux_key);

        // Check if the value is too long for the provided buffer
        if (val_length >= MAX_KEY_LENGTH)
            return false;

        // Copy the value to the 'value' buffer without the double quotes
        strncpy(value, aux_key, val_length);
        value[val_length] = '\0';
    }
    else
    {
        // Find the end of the value (until the next whitespace or end of string)
        const char* end = aux_key;
        while (*end && !isspace((unsigned char)*end)) {
            end++;
        }

        // Calculate the length of the value
        size_t valueLength = (size_t)(end - aux_key);

        // Check if the value is too long for the provided buffer
        if (valueLength >= MAX_KEY_LENGTH) {
            return false;
        }

        // Copy the value to the 'value' buffer
        strncpy(value, aux_key, valueLength);
        value[valueLength] = '\0';
    }

    return true;
}

bool extractStarValues(char* input, struct Star* star)
{
    // Use strtok to split the input string by commas
    char* token = strtok(input, ",");
    if (token == NULL)
        return false;

    const char* aux_token = token;
    size_t cat_length = 0;

    // Calculate the length of the key.
    while (*aux_token && *aux_token != '=')
    {
        aux_token++;
        cat_length++;
    }

    // Copy the catalog to the struct
    strncpy(star->catalog, token, cat_length);
    star->catalog[cat_length] = '\0';

    // Parse the remaining values
    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%d", &star->star_num) != 1)
        return false;

    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%lf", &star->ra) != 1)
        return false;

    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%lf", &star->dec) != 1)
        return false;

    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%lf", &star->pm_ra) != 1)
        return false;

    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%lf", &star->pm_dec) != 1)
        return false;

    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%lf", &star->parallax) != 1)
        return false;

    token = strtok(NULL, ",");
    if (token == NULL || sscanf(token, "%lf", &star->rad_vel) != 1)
        return false;

    return true;
}

bool parseStarAzElData(const char* filename,
                      struct Location* location,
                      struct Adjust* adjust,
                      struct Meteo* meteo,
                      struct Observation* observation,
                      struct Star stars[],
                      size_t* num_star)
{
    // Containers.
    char line[MAX_LINE_LENGTH];
    int next_section = 0;
    *num_star = 0;

    // Open the file.
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return false;

    // Extract the data.
    while (fgets(line, sizeof(line), file))
    {
        // Auxiliar variables.
        char section[MAX_SECTION_LENGTH];
        char key[MAX_KEY_LENGTH];
        char value[MAX_VALUE_LENGTH];

        // Skip comments.
        if(line[0]=='#' || isWhitespaceLine(line))
            continue;

        // 1 - Get the section.
        // 2 - Get the key and value.
        if(extractSection(line, section))
        {
            if (strcmp(section, "location") == 0)
                next_section = 1;
            else if (strcmp(section, "adjust") == 0)
                next_section = 2;
            else if (strcmp(section, "meteo") == 0)
                next_section = 3;
            else if (strcmp(section, "observation") == 0)
                next_section = 4;
            else if (strcmp(section, "stars") == 0)
                next_section = 5;
        }
        else if(extractKeyValuePair(line, key, value))
        {
            switch (next_section)
            {
            case 1:
                if (strcmp(key, "latitude") == 0)
                    location->latitude = atof(value);
                else if (strcmp(key, "longitude") == 0)
                    location->longitude = atof(value);
                else if (strcmp(key, "height") == 0)
                    location->height = atof(value);
                break;
            case 2:
                if (strcmp(key, "x_pole") == 0)
                    adjust->x_pole = atof(value);
                else if (strcmp(key, "y_pole") == 0)
                    adjust->y_pole = atof(value);
                else if (strcmp(key, "leap_secs") == 0)
                    adjust->leap_secs = atof(value);
                else if (strcmp(key, "ut1_utc") == 0)
                    adjust->ut1_utc = atof(value);
                break;
            case 3:
                if (strcmp(key, "temperature") == 0)
                    meteo->temperature = atof(value);
                else if (strcmp(key, "pressure") == 0)
                    meteo->pressure = atof(value);
                else if (strcmp(key, "humidity") == 0)
                    meteo->humidity = atof(value);
                break;
            case 4:
                if (strcmp(key, "start") == 0)
                {
                    short int year, month, day, hour, min;
                    double sec;
                    if (sscanf(value, "%hd-%hd-%hdT%hd:%hd:%lf", &year, &month, &day, &hour, &min, &sec) == 6)
                    {
                        observation->start_year = year;
                        observation->start_month = month;
                        observation->start_day = day;
                        observation->start_hour = hour;
                        observation->start_min = min;
                        observation->start_sec = sec;
                    }
                }
                else if (strcmp(key, "end") == 0)
                {
                    short int year, month, day, hour, min;
                    double sec;
                    if (sscanf(value, "%hd-%hd-%hdT%hd:%hd:%lf", &year, &month, &day, &hour, &min, &sec) == 6)
                    {
                        observation->end_year = year;
                        observation->end_month = month;
                        observation->end_day = day;
                        observation->end_hour = hour;
                        observation->end_min = min;
                        observation->end_sec = sec;
                    }
                }
                else if (strcmp(key, "limit_el_min") == 0)
                    observation->limit_el_min = atoi(value);
                else if (strcmp(key, "limit_el_max") == 0)
                    observation->limit_el_max = atoi(value);
                else if (strcmp(key, "increment") == 0)
                    observation->inc = atof(value);
                break;
            case 5:
                if (*num_star < MAX_STARS)
                {
                    if(!extractStarValues(value, &stars[*num_star]))
                        return false;
                    strcpy(stars[*num_star].name, key);
                    (*num_star)++;
                }
                break;
            }
        }
    }

    fclose(file);
    return true; // Success
}

void generateStarPositionFile(const char* path,
                              const struct Star* star,
                              const struct Observation* obs,
                              const struct StarPosition positions[],
                              size_t num_pos)
{
    // Generate the filename.
    char file_name[256];
    char full_filepath[256];

    snprintf(file_name, sizeof(file_name),
             "%s_%d_[%s]_%d%02d%02d_%02d%02d.starpos",
             star->catalog, star->star_num, star->name,
             obs->start_year, obs->start_month, obs->start_day, obs->start_hour, obs->start_min);

    snprintf(full_filepath, sizeof(full_filepath), "%s/%s", path, file_name);

    // Open the file for writing
    FILE* file = fopen(full_filepath, "w");
    if (file == NULL)
    {
        printf("Error opening file %s for writing\n", full_filepath);
        return;
    }

    // Write the header section
    fprintf(file, "[HEADER]\n");
    fprintf(file, "star_name=\"%s\"\n", star->name);
    fprintf(file, "catalog=%s\n", star->catalog);
    fprintf(file, "catalog_num=%d\n", star->star_num);
    fprintf(file, "start_iso8601=%d-%02d-%02dT%02d:%02d:%06.3f\n",
            obs->start_year, obs->start_month, obs->start_day,
            obs->start_hour, obs->start_min, obs->start_sec);
    fprintf(file, "end_iso8601=%d-%02d-%02dT%02d:%02d:%06.3f\n",
            obs->end_year, obs->end_month, obs->end_day,
            obs->end_hour, obs->end_min, obs->end_sec);
    fprintf(file, "start_jd=%6f\n", positions[0].jd_utc);
    fprintf(file, "end_jd=%6f\n", positions[num_pos-1].jd_utc);
    fprintf(file, "\n");

    // Write the positions section
    fprintf(file, "[POSITIONS]\n");
    fprintf(file, "# JD_UTC;az;el;topo_ra;topo_dec\n");
    for (size_t i = 0; i < num_pos; i++)
    {
        fprintf(file, "%.6f;%.10f;%.10f;%.10f;%.10f\n",
                positions[i].jd_utc, positions[i].az, positions[i].el,
                positions[i].t_ra, positions[i].t_dec);
    }
    fprintf(file, "\n");

    // Write the END section
    fprintf(file, "[END]\n");

    // Close the file
    fclose(file);

    printf("File generated: %s\n", full_filepath);
}
int main(int argc, char *argv[])
{
    // Star log.
    printf ("-------------------------------\n");
    printf ("NOVAS Util: Generate Star Az El\n");
    printf ("-------------------------------\n");
    printf ("\n\n");

    // Low accuracy mode with NOVAS 2000K nutation series (488 terms).
    // The requirements are no better than about 0.05 arcsecond so you can use
    // this alternative version of two NOVAS subroutines. In this way, the
    // computational load and calculation time will decrease considerably, and
    // these routines can be used to control astronomical mount guidance systems.
    const short int accuracy = 1;

    // Auxiliar variables.
    short int error;
    double jd_utc_start, jd_utc_end, jd_utc, delta_t;
    double jd_tt, jd_ut1, fractional_t, dt_inc;
    double t_ra, t_dec;
    double az, zd;
    size_t num_positions;
    bool stop_flag = false;

    // Configuration file.
    const char* filename = "../../../LibNovasCpp/utilities/GenerateStarAzEl/StarList.ini";
    const char* output_path = "../../../LibNovasCpp/utilities/GenerateStarAzEl/auxiliar";

    // Prepare the configuration structs and variables.
    struct Star stars[MAX_STARS];
    struct Location loc;
    struct Adjust adj;
    struct Meteo met;
    struct Observation obs;
    struct StarPosition pos;
    size_t num_stars;

    // Novas structs.
    on_surface geo_loc;
    cat_entry cat_star;

    // Parse de configuration file.
    if(!parseStarAzElData(filename, &loc, &adj, &met, &obs, stars, &num_stars))
        return 1;

    // Establish start time.
    fractional_t = ((double)obs.start_min / 1440.0) + (obs.start_sec / 86400.0);
    jd_utc_start = julian_date(obs.start_year,obs.start_month,obs.start_day,obs.start_hour) + fractional_t;

    // Establish end time.
    fractional_t = ((double)obs.end_min / 1440.0) + (obs.end_sec / 86400.0);
    jd_utc_end = julian_date(obs.end_year,obs.end_month,obs.end_day,obs.end_hour) + fractional_t;

    // Get delta (Delta = TT - UT1 in seconds) and increment.
    delta_t = 32.184 + adj.leap_secs - adj.ut1_utc;
    dt_inc = obs.inc/86400.0;

    // Log time values.
    printf ("----------------------------\n");
    printf ("TIME VALUES:\n");
    printf ("----------------------------\n");
    printf ("START JULIAN UTC: %15.10f\n", jd_utc_start);
    printf ("END JULIAN UTC: %15.10f\n", jd_utc_end);
    printf ("----------------------------\n\n");

    // Process each star.
    for(size_t i = 0; i < num_stars; i++)
    {
        // Get the star and prepare the positions.
        struct Star star = stars[i];
        struct StarPosition positions[MAX_POSITIONS_LENGTH];
        struct StarPosition* current_pos = positions;

        // Reset the time and flags.
        jd_utc = jd_utc_start;
        stop_flag = false;
        num_positions = 0;

        printf ("------------------------------------\n");
        printf ("COMPUTING STAR:\n");
        printf ("------------------------------------\n");
        printf ("STAR NAME: %s\n", star.name);
        printf ("STAR NUM: %d\n", star.star_num);
        printf ("------------------------------------\n");

        // Generate the positions.
        while((jd_utc < jd_utc_end) && !stop_flag)
        {
            // Get the new times.
            jd_tt = jd_utc + (adj.leap_secs + 32.184) / 86400.0;
            jd_ut1 = jd_utc + adj.ut1_utc / 86400.0;

            // Make the surface structure.
            make_on_surface(loc.latitude,loc.longitude,loc.height,met.temperature,met.pressure, &geo_loc);

            // Make the catalog entry (ICRS position and motion).
            make_cat_entry(star.name,star.catalog,star.star_num, star.ra, star.dec, star.pm_ra,
                            star.pm_dec, star.parallax, star.rad_vel, &cat_star);

            // Calculates the apparent and topocentric place of star.
            if ((error = topo_star(jd_tt,delta_t,&cat_star,&geo_loc, accuracy, &t_ra, &t_dec)) != 0)
            {
               printf ("Error %d from topo_star.\n", error);
               return (error);
            }

            // Do the transformation to get the az and elevation (degrees).
            equ2hor(jd_ut1, delta_t, accuracy, adj.x_pole, adj.y_pole, &geo_loc, t_ra, t_dec, 2,
                    &zd, &az, &t_ra, &t_dec);

            // Store the position.

            current_pos->jd_utc = jd_utc;
            current_pos->t_ra = t_ra;
            current_pos->t_dec = t_dec;
            current_pos->az = az;
            current_pos->el = (90.0 - zd);

            struct StarPosition pos = *current_pos;

            // Update the UTC julian.
            jd_utc = jd_utc + dt_inc;

            // Check the elevation:
            if((pos.el < obs.limit_el_min || pos.el > obs.limit_el_max))
            {
               if(false)
               {
                   printf ("------------------------------------\n");
                   printf ("POSITION DISCARTED DUE TO ELEVATION:\n");
                   printf ("------------------------------------\n");
                   printf ("STAR NAME: %s\n", star.name);
                   printf ("STAR NUM: %d\n", star.star_num);
                   printf ("------------------------------------\n");
               }
            }
            else
            {
                // New position.
                num_positions++;
                current_pos++;

                // Log each new position.
                if(false)
                {
                    printf ("------------------------------------\n");
                    printf ("NEW TOPOCENTRIC POSITION:\n");
                    printf ("------------------------------------\n");
                    printf ("STAR NAME: %s\n", star.name);
                    printf ("STAR NUM: %d\n", star.star_num);
                    printf ("------------------------------------\n");
                    printf ("JULIAN UTC: %15.10f\n", pos.jd_utc);
                    printf ("JULIAN TT: %15.10f\n", jd_tt);
                    printf ("JULIAN UT1: %15.10f\n", jd_ut1);
                    printf ("------------------------------------\n");
                    printf ("RA: %15.10f\n", pos.t_ra);
                    printf ("DEC: %15.10f\n", pos.t_dec);
                    printf ("------------------------------------\n");
                    printf ("AZ: %15.10f\n", pos.az);
                    printf ("EL: %15.10f\n", pos.el);
                    printf ("------------------------------------\n");
                }
            }
        }

        // Generate the file.
        if(!stop_flag)
        {
            if(num_positions > 0)
            {
                printf ("STORNIG POSITIONS...\n\n");
                generateStarPositionFile(output_path, &star, &obs, positions, num_positions);
            }
            else
            {
                printf ("------------------------------------\n");
                printf ("STAR DISCARTED DUE TO ELEVATION:\n");
                printf ("------------------------------------\n");
                printf ("STAR NAME: %s\n", star.name);
                printf ("STAR NUM: %d\n", star.star_num);
                printf ("------------------------------------\n");
            }
        }
    }

    return (0);
}
