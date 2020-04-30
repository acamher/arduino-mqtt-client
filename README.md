# Arduino MQTT Client #

Arduino sends data via MQTT to Raspberry

## Requirements ##

As this sample use the CC3000 WiFi shield, it require some additionnal library :

- [Adafruit CC3000 Library](https://github.com/adafruit/Adafruit_CC3000_Library)
- [CC3000 MQTT PubSub library](https://github.com/nathanchantrell/pubsubclient)
- DHT library
- Adafruit Generic Sensor Library

There is also an ethernet client which can be usefull :

- [MQTT Ethernet PubSubClient](https://github.com/knolleary/pubsubclient)

## How to install ##

After cloning the repository into your arduino sketch directory, rename (or copy) the `config-sample.h` file into `config.h` and update it's configuration values according to your needs.
