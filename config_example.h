/**
 * This is a sample configuration file for the Arduino's MQTT client
 *
 * Please rename this file into "config.h" and update it's values
 * according to your needs.
 */

#ifndef ARDUINOMQTTCLIENT_CONFIG_H
#define ARDUINOMQTTCLIENT_CONFIG_H

/**
 * Uncomment this line to activate verbose debug information sent to serial port
 */
//#define DEBUG           1

/**
 * Configure your Wifi access point
 */
#define WLAN_SSID       "WLAN_SSID"
#define WLAN_PASS       "WLAN_PASS"

#define WLAN_SECURITY   WLAN_SEC_WPA2

/**
 * Configure your MQTT server
 *
 * Ip of the MQTT server is defined in a reverse way.
 * Means that loopback 127.0.0.1 will be defined as {1, 0, 0, 127}
 */
ArrayToIp server = {80,1,168,192};
#define MQTT_PORT              1883

/**
 * Configure DHT sensor
 */
#define DHTPIN            2
#define DHTTYPE           DHT11

/**
 * Refresh delay in miliseconds
 */
#define DELAY             1000

#endif
