/* 
 * ESP8266 NodeMCU DHT11 UDP Server
 * 
 * References
 * https://circuits4you.com/2017/12/31/nodemcu-pinout/
 * https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
 *
 */

#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DHTpin 2    //D4 of NodeMCU is GPIO2

DHTesp dht;

WiFiUDP Udp;
unsigned int localUdpPort = 3210;
char incomingPacket[256];

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Initializing DHT11 sensor on D4 (GPIO2)");       
    dht.setup(DHTpin, DHTesp::DHT11);

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
}

void loop()
{
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

        delay(dht.getMinimumSamplingPeriod());

        float humidity = dht.getHumidity();
        float temperature = dht.getTemperature();

        char temperature_str[16];
        dtostrf(temperature, 3, 1, temperature_str);
        
        char humidity_str[16];
        dtostrf(humidity, 4, 2, humidity_str);
        
        char replyPacket[32];
        
        sprintf(replyPacket, "%s %s", temperature_str, humidity_str);

        Serial.print("Sending UDP response with temperature and humidity: ");
        Serial.println(replyPacket);
        
        Udp.beginPacket(Udp.remoteIP(), 3210);
        Udp.write(replyPacket);
        Udp.endPacket();
    }
}
