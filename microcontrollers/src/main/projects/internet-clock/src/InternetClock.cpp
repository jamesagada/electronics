
#include "InternetClock.h"

const long utcOffsetInSeconds = 3600 * -5;

// Define NTP Client to get time
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org", utcOffsetInSeconds);

InternetClock::InternetClock()
{
    timeClient.begin();
}

void InternetClock::oneLoop()
{
    Serial.println();
    
    timeClient.update();

    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print(", ");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.println(timeClient.getSeconds());
    
    Serial.println(timeClient.getFormattedTime());

    unsigned long epoch = timeClient.getEpochTime();

    Serial.print("year: ");
    Serial.print( year(epoch) );

    Serial.print(" month: ");
    Serial.print( month(epoch) );

    // it seems this is off by one with the current configuration (US Central Time is intended)
    Serial.print(" day: ");
    Serial.println( day(epoch) );
}
