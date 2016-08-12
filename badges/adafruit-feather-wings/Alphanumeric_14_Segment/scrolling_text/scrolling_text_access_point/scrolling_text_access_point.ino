
/**
 * 
 * The default IP of the access point is 
 * 
 *      192.168.4.1
 * 
 *      Here is some documentation on changing the default IP address of the 
 *      access point.
 * 
 * This ESP8266 specific source code was originally from Sparkfun's example here
 *
 *        https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server
 * 
 */

#include <ESP8266WiFi.h>

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include "scrolling_text.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "lovelovelove";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 5; // Thing's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the Thing
const int DIGITAL_PIN = 12; // Digital pin to be read

WiFiServer server(80);

String localhostIp;

int indecies[] = {0,1,2,3};

String message = "  Go Spurs Go   *   Timmy Rules!   * Bruce Bruce!!!    *";

int messageLength = message.length();

/**
  * This array holds the current values of the 4 alphanumeric segments.
  */
char displaybuffer[] = {' ', ' ', ' ', ' '};

unsigned long lightPreviousMillis = 0;

unsigned long currentMillis;

/**
 * This is the delay in milliseconds for the scrolling text.
 */
long scrollDelay = 350;

void setup() 
{
    w = 3;
    
    Serial.begin(115200);
    
    initHardware();
    setupWiFi();
  
    // start the server
    server.begin();
    Serial.println("Server started");

    localhostIp = WiFi.localIP().toString();

    // Print the IP address
    Serial.println();
    Serial.print("local IP: ");
    Serial.println(localhostIp);
    Serial.println();
}

void loop() 
{
    currentMillis = millis();
    
    if(currentMillis - lightPreviousMillis >= scrollDelay) 
    {
        // save the last time you blinked the LED
        lightPreviousMillis = currentMillis;

        updateDisplay();
    }     
    
    
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/led/1") != -1)
    val = 1; // Will write LED high
  else if (req.indexOf("/read") != -1)
    val = -2; // Will print pin reads
  // Otherwise request will be invalid. We'll say as much in HTML

  // Set GPIO5 according to the request
  if (val >= 0)
    digitalWrite(LED_PIN, val);

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val >= 0)
  {
    s += "LED is now ";
    s += (val)?"on":"off";
  }
  else if (val == -2)
  { 
    // If we're reading pins, print out those values:
    s += "Analog Pin = ";
    s += String(analogRead(ANALOG_PIN));
    s += "<br>"; // Go to the next line.
    s += "Digital Pin 12 = ";
    s += String(digitalRead(DIGITAL_PIN));
  }
  else
  {
    s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void setupWiFi()
{
    WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Scroller " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.

    initQuadAlpha();
}

void initQuadAlpha()
{
    // pass in the address
    alpha4.begin(0x70);  

  
    alpha4.clear();

    alpha4.writeDigitAscii(0, 'N');
    alpha4.writeDigitAscii(1, 'I');
    alpha4.writeDigitAscii(2, 'C');
    alpha4.writeDigitAscii(3, 'E');

    alpha4.writeDisplay();
   
    Serial.println("The Adafruit_AlphaNum4 setup is complete.");
}

void updateDisplay()
{
    // shift the index into the message for each segment 
    indecies[0] += 1;
    indecies[1] += 1;
    indecies[2] += 1;
    indecies[3] += 1;

    if(indecies[0] == messageLength)
    {
        indecies[0] = 0;
    }

    if(indecies[1] == messageLength)
    {
        indecies[1] = 0;
    }

    if(indecies[2] == messageLength)
    {
        indecies[2] = 0;
    }

    if(indecies[3] == messageLength)
    {
        indecies[3] = 0;
    }
  
  // scroll down display
  displaybuffer[0] = message[indecies[0]];
  displaybuffer[1] = message[indecies[1]];
  displaybuffer[2] = message[indecies[2]];
  displaybuffer[3] = message[indecies[3]];
 
  // set every digit to the buffer
  alpha4.writeDigitAscii(0, displaybuffer[0]);
  alpha4.writeDigitAscii(1, displaybuffer[1]);
  alpha4.writeDigitAscii(2, displaybuffer[2]);
  alpha4.writeDigitAscii(3, displaybuffer[3]);
 
  // write it out!
  alpha4.writeDisplay();    
}