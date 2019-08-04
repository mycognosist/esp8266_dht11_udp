/* 
 * ESP8266 NodeMCU DHT11 UDP Server
 * 
 * References
 * https://circuits4you.com/2017/12/31/nodemcu-pinout/
 * https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
 * https://wiki.dfrobot.com/Gravity__I2C_16x2_Arduino_LCD_with_RGB_Backlight_Display_SKU__DFR0464
 */

#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <DFRobot_RGBLCD.h>

#define DHTpin 2    //D4 of NodeMCU is GPIO2

DHTesp dht;

WiFiUDP Udp;
unsigned int localUdpPort = 3210;
char incomingPacket[256];

DFRobot_RGBLCD lcd(16,2);  //16 characters and 2 lines of show

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Initializing DHT11 sensor on D4 (GPIO2)");       
    dht.setup(DHTpin, DHTesp::DHT11);

    Serial.println("Initializing LCD display on D1 & D2 (SCL & SDA)");       
    lcd.init();
    
    WiFi.begin("Home", "an_actual_password");

    Serial.print("Connecting to Home WiFi AP");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("Activating UDP server");
    Udp.begin(localUdpPort);
    Serial.printf("UDP server active, port %d\n", localUdpPort);
    Serial.println();

    lcd.setRGB(114, 222, 194);
    lcd.setCursor(0,0);
    lcd.print("glyph");
    lcd.setCursor(0,1);
    lcd.print("sunday hacks");
    delay(3);
}

void loop()
{    
    delay(dht.getMinimumSamplingPeriod());

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    
    char temperature_str[16];
    dtostrf(temperature, 3, 1, temperature_str);
    char t[16];
    sprintf(t, "Temp: %s", temperature_str);
    
    lcd.setCursor(0,0);
    lcd.print(t);
    
    char humidity_str[16];
    dtostrf(humidity, 2, 0, humidity_str);
    char h[16];
    sprintf(h, "Humidity: %s", humidity_str);

    lcd.setCursor(0,1);
    lcd.print(h);
    
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        Serial.printf("Received %d bytes from %s, port %d", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        int len = Udp.read(incomingPacket, 255);
        
        if (len > 0)
        {
            incomingPacket[len] = 0;
        }
        Serial.println();
        Serial.printf("UDP packet contents: %s", incomingPacket);
        Serial.println();
    
        char replyPacket[32];
        sprintf(replyPacket, "%s %s", temperature_str, humidity_str);

        Serial.print("Sending UDP response with temperature and humidity: ");
        Serial.println(replyPacket);
        
        Udp.beginPacket(Udp.remoteIP(), 3210);
        Udp.write(replyPacket);
        Udp.endPacket();
    }
}
