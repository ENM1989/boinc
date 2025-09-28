#include <cmath>
#include "sched_sun.h"

#define PI 3.14159265358979323846
#define TPI (2 * PI)
#define DEGS (180.0 / PI)
#define RADS (PI / 180.0)

// The following code is adapted from the C++ program for calculating sunrise and sunset
// by Jarmo Lammi, found at: https://www.scribd.com/document/307500514/Cpp-Code-Sun-Position

// get the days to J2000
// h is UT in decimal hours
// FNday only works between 1901 to 2099 - see Meeus chapter 7
double FNday(int y, int m, int d, float h) {
    int luku = -7 * (y + (m + 9) / 12) / 4 + 275 * m / 9 + d;
    luku += (long int)y * 367;
    return (double)luku - 730531.5 + h / 24.0;
};

// the function below returns an angle in the range 0 to 2*pi
double FNrange(double x) {
    double b = x / TPI;
    double a = TPI * (b - (long)(b));
    if (a < 0)
        a = TPI + a;
    return a;
};

// Calculating the hourangle
double f0(double lat, double declin) {
    double fo, dfo;
    double SunDia = 0.53; // Sunradius degrees
    double AirRefr = 34.0 / 60.0; // athmospheric refraction degrees
    dfo = RADS * (0.5 * SunDia + AirRefr);
    if (lat < 0.0)
        dfo = -dfo;
    fo = tan(declin + dfo) * tan(lat * RADS);
    if (fo > 0.99999)
        fo = 1.0; // to avoid overflow
    fo = asin(fo) + PI / 2.0;
    return fo;
};

// Find the ecliptic longitude of the Sun
double FNsun(double d) {
    // mean longitude of the Sun
    double L = FNrange(280.461 * RADS + .9856474 * RADS * d);
    // mean anomaly of the Sun
    double g = FNrange(357.528 * RADS + .9856003 * RADS * d);
    // Ecliptic longitude of the Sun
    return FNrange(L + 1.915 * RADS * sin(g) + .02 * RADS * sin(2 * g));
};

void sun_rise_set(int year, int month, int day, double lat, double lon, double& sunrise, double& sunset) {
    double d = FNday(year, month, day, 12);

    // Use FNsun to find the ecliptic longitude of the Sun
    double lambda = FNsun(d);

    // Obliquity of the ecliptic
    double obliq = 23.439 * RADS - .0000004 * RADS * d;

    // Find the RA and DEC of the Sun
    double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
    double delta = asin(sin(obliq) * sin(lambda));

    // Find the Equation of Time in minutes
    double L = FNrange(280.461 * RADS + .9856474 * RADS * d);
    double LL = L - alpha;
    if (L < PI)
        LL += TPI;
    double equation = 1440.0 * (1.0 - LL / TPI);

    double ha = f0(lat, delta);
    double daylen = DEGS * ha / 7.5;
    if (daylen < 0.0001) {
        daylen = 0.0;
    }

    sunrise = 12.0 - 12.0 * ha / PI - lon / 15.0 + equation / 60.0;
    sunset = 12.0 + 12.0 * ha / PI - lon / 15.0 + equation / 60.0;

    if (sunrise > 24.0)
        sunrise -= 24.0;
    if (sunset > 24.0)
        sunset -= 24.0;
    if (sunrise < 0.0)
        sunrise += 24.0;
    if (sunset < 0.0)
        sunset += 24.0;
}