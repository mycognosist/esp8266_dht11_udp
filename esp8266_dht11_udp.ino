/* 
 * ESP8266 NodeMCU DHT11 - Humidity Temperature Sensor Example
 * 
 * References
 * https://circuits4you.com/2017/12/31/nodemcu-pinout/
 * https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
 *
 */
        
#include "DHTesp.h"
#include "ESP8266WiFi.h"

#define DHTpin 2    //D4 of NodeMCU is GPIO2

DHTesp dht;

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Initializing DHT11 sensor on D4 (GPIO2)");       
    dht.setup(DHTpin, DHTesp::DHT11);

    WiFi.begin("Home", "0217890132");

    Serial.print("Connecting to Home WiFi AP");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
}

void loop()
{
    delay(dht.getMinimumSamplingPeriod());

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();

    Serial.print(dht.getStatusString());
    Serial.print("\t");
    Serial.print(humidity, 1);
    Serial.print("\t\t");
    Serial.print(temperature, 1);
    Serial.print("\t\t");
    Serial.print(dht.toFahrenheit(temperature), 1);
    Serial.print("\t\t");
    Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
    Serial.print("\t\t");
    Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
}
