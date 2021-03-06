/*!
   \name testSunPosition
   \brief show current sun rise and sun set at Bologna and Madrid
   \brief using solarRadiation library (sunPostion module)
 */


#include <QCoreApplication>
#include <QDate>
#include <QTime>
#include <iostream>

#include "commonConstants.h"
#include "sunPosition.h"
#include "radiationDefinitions.h"


void printSunriseSunset(int timeMinutes, QString rise_set)
{
    int h = timeMinutes / 60;
    int m = timeMinutes - (h*60);
    std::cout << "Sun " << rise_set.toStdString() << " at (solar time) " << h << ":" << m << std::endl;
}


bool testSunPosition(float lat, float lon, int timeZone, QDate myDate, QTime myTime)
{
    int chk;
    float etrTilt;                  // Extraterrestrial (top-of-atmosphere) global irradiance on a tilted surface (W m-2)
    float cosZen;                   // Cosine of refraction corrected solar zenith angle
    float sbcf;                     // Shadow-band correction factor
    float prime;                    // Factor that normalizes Kt, Kn, etc.
    float unPrime;                  // Factor that denormalizes Kt', Kn', etc.
    float zenRef;                   // Solar zenith angle, deg. from zenith, refracted
    float sunCosIncidenceCompl;     // cosine of (90 - incidence)
    float sunRiseMinutes;           // sun rise time [minutes from midnight]
    float sunSetMinutes;            // sun set time [minutes from midnight]

    TsunPosition *mySunPosition;
    mySunPosition = new TsunPosition();

    chk = RSUN_compute_solar_position(lon, lat, timeZone, myDate.year(), myDate.month(), myDate.day(),
                                      myTime.hour(), myTime.minute(), myTime.second(),
                                      TEMPERATURE_DEFAULT, PRESSURE_SEALEVEL,
                                      180, 0, SBWID, SBRAD, SBSKY);
    if (chk > 0)
    {
        std::cout << "Error in sun position!" << std::endl;
        return false;
    }

    RSUN_get_results(&((*mySunPosition).relOptAirMass), &((*mySunPosition).relOptAirMassCorr),
                     &((*mySunPosition).azimuth), &sunCosIncidenceCompl,
                     &cosZen, &((*mySunPosition).elevation),
                     &((*mySunPosition).elevationRefr),
                     &((*mySunPosition).extraIrradianceHorizontal),
                     &((*mySunPosition).extraIrradianceNormal),
                     &etrTilt, &prime, &sbcf, &sunRiseMinutes, &sunSetMinutes, &unPrime, &zenRef);

    printSunriseSunset(int(sunRiseMinutes), "rise");
    printSunriseSunset(int(sunSetMinutes), "set");
    std::cout << "\nValues at solar time: " << myTime.toString().toStdString() << std::endl;
    std::cout << "azimuth [deg.] " << (*mySunPosition).azimuth << std::endl;
    std::cout << "elevation angle refracted [deg. from horizon] " << (*mySunPosition).elevationRefr << std::endl;
    std::cout << "extr. global irradiance horizontal [W/m2] " << (*mySunPosition).extraIrradianceHorizontal << std::endl;

    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDate myDate = QDate::currentDate();
    QTime myTime = QTime(12, 0, 0);
    int timeZone = 1;


    std::cout << "Sun position test\n" << std::endl;

    std::cout << "Today is " << myDate.toString().toStdString() << std::endl;

    std::cout << "\nBologna" << std::endl;
    testSunPosition(44.5f, 11.35f, timeZone, myDate, myTime);

    std::cout << "\nMadrid" << std::endl;
    testSunPosition(40.6f, -3.67f, timeZone, myDate, myTime);

    return a.exec();
}

