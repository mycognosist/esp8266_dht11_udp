# esp8266_dht11_udp

Connect to local WiFi network with an ESP8266 and respond to UDP requests with temperature and humidity readings from attached DHT11 sensor.

 - Upload `esp8266_dht11_udp.ino` to your ESP8266  
 - Open the serial terminal: `cu -l /dev/ttyUSB0 -s 115200`
 - Send UDP packet with contents `GET` to ESP8266 IP and port (3210)
 - Terminal output:

     ```
     Connected.
     Received 3 bytes from 192.168.0.165, port 3210
     UDP packet contents: GET
     Sending UDP response with temperature and humidity: 17.0 60.00
     ```

Be sure to set your WiFi AP credentials and UDP response IP and port in the code.

Temperature and humidity are displayed on an attached DFRobot I2C 16x2 Arduino LCD with RGB Backlight Display module.
