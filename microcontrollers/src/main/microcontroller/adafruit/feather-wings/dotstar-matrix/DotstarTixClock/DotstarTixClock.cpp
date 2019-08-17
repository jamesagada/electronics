
#include <cstdlib> // for std::srand()

#include <algorithm> // for std::random_shuffle()

#include "DotstarTixClock.h"

void DotstarTixClock::tixClockPositionsVerification()
{
    int displayDelay = 250;

    uint16_t hourTensColor = dotstarMatrix->Color(5, 5, 155);
    
    for(int i = 0; i<ROWS; i++)
    {
        int x = hourTensLocations[i][0];

        int y = hourTensLocations[i][1];
        
        Serial.print(x);        
        Serial.print(",");        
        Serial.println(y);
        
        dotstarMatrix->drawPixel(x,y, hourTensColor);       
        
        dotstarMatrix->show();
        
        delay(displayDelay);
    }
    
//    uint16_t hourOnesColor = dotstarMatrix->Color(5, 175, 5);

    for(int i = 0; i<ROWS*3; i++)
    {
        int x = hourOnesLocations[i][0];

        int y = hourOnesLocations[i][1];
        
        Serial.print(x);        
        Serial.print(",");        
        Serial.println(y);
        
        dotstarMatrix->drawPixel(x,y, hourOnesColor);
        
        dotstarMatrix->show();
        
        delay(displayDelay);
    }
    
    uint16_t minuteTensColor = dotstarMatrix->Color(185, 5, 5);

    for(int i = 0; i<ROWS*2; i++)
    {
        int x = minuteTensLocations[i][0];

        int y = minuteTensLocations[i][1];
        
        Serial.print(x);        
        Serial.print(",");        
        Serial.println(y);
        
        dotstarMatrix->drawPixel(x,y, minuteTensColor);       
        
        dotstarMatrix->show();
        
        delay(displayDelay);
    }

    uint16_t minuteOnesColor = dotstarMatrix->Color(151, 48, 177);
    
    for(int i = 0; i<ROWS*3; i++)
    {
        int x = minuteOnesLocations[i][0];

        int y = minuteOnesLocations[i][1];
        
        Serial.print(x);        
        Serial.print(",");        
        Serial.println(y);
        
        dotstarMatrix->drawPixel(x,y, minuteOnesColor);       
        
        dotstarMatrix->show();
        
        delay(displayDelay);
    }    
    
    Serial.println("ploop tix");
}

void DotstarTixClock::updateTime(int hour, int minute)
{
    updateHourOnes(hour);
    
    updateHourTens(hour);
}

void DotstarTixClock::updateHourOnes(int hours)
{
    int hourOnes = hours % 10;
//    int hourOnes = 9;
    
    Serial.print("hourOnes -> ");
    Serial.println(hourOnes);

// I haven't figured out how to shuffle the array insdie the call to updatTimePanel()    
    std::random_shuffle(threeRowIndicies, threeRowIndicies + (sizeof(threeRowIndicies) / sizeof(*threeRowIndicies)));
    updateTimePanel(hourOnes, threeRowIndicies, hourOnesLocations, hourOnesColor);
//    updateTimePanel(hourOnes, threeRowIndicies, &(hourOnesLocations[0][0]), hourOnesColor);    
}

void DotstarTixClock::updateHourTens(int hours)
{
    
}

void DotstarTixClock::updateTimePanel(int count, int rowIndicies[], int locations[][2], uint16_t color)
//void DotstarTixClock::updateTimePanel(int count, int* rowIndicies, int** locations, uint16_t color)
{
    // shuffle the indicies
//    std::random_shuffle(rowIndicies, rowIndicies + (sizeof(rowIndicies) / sizeof(*rowIndicies)));
    
    for(int ho = 0; ho < count; ho++)
    {
        int i = rowIndicies[ho];
        
        int x = locations[i][0];
        
        int y = locations[i][1];
        
        dotstarMatrix->drawPixel(x,y, color);
    }    
}