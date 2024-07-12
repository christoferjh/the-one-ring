# the-one-ring

Animated mega sized the one ring


![gif4](https://github.com/christoferjh/the-one-ring/assets/18284010/54876285-31c4-4231-8025-8f2eeb2e241b)

## And another animation example


![gif2croped](https://github.com/christoferjh/the-one-ring/assets/18284010/1ff47728-7d2a-474f-bfb9-f73cf370d8e4)


### The One Ring Notification Device

#### Overview

The One Ring Notification Device is an innovative and visually appealing notification system integrated into a smart home environment. Utilizing an array of individually addressable LEDs, this device is designed to provide real-time notifications by displaying various animations and colors, all controlled via Home Assistant and Node-RED.

#### Features

1. **Customizable LED Animations:**
   - **Color Roll:** A rotating color effect cycling through red, green, and blue.
   - **Full On:** All LEDs lit with a single, user-defined color.
   - **Color Follow:** A moving light effect where a specified number of LEDs follow each other.
   - **Glow:** A pulsing glow effect with adjustable brightness.
   - **Lava Flow:** A trickling lava animation with independent streams, creating a captivating visual effect.

2. **Integration with Home Assistant via node-red:**
   - Seamlessly connects with Home Assistant, allowing the LED ring to serve as a visual notification hub for various home automation events.
   - Node-RED flows manage the LED animations and color changes based on triggers from Home Assistant.
   - Provides a user-friendly interface for creating complex automation scenarios without extensive coding.

4. **Web Interface:**
   - A simple HTML interface to control the LED ring’s brightness, animation type, and color.
   - Accessible via any web browser on the local network.

#### Technical Specifications

- **Microcontroller:** ESP8266, providing Wi-Fi connectivity.
- **LED Type:** WS2812B, individually addressable LEDs.
- **Power Supply:** 5V DC, adequate for powering the ESP8266 and LEDs.
- **Communication:** Initially attempted MQTT, but switched to Node-RED for enhanced stability and reliability.

#### Setup and Configuration

1. **Hardware Setup:**
   - Connect the WS2812B LED ring to the ESP8266 microcontroller, ensuring proper power and data connections.
   - Power the setup using a stable 5V power supply.

2. **Software Setup:**
   - Flash the ESP8266 with the provided Arduino code, ensuring the inclusion of necessary libraries such as `ESP8266WiFi`, `FastLED`, and `ESP8266WebServer`.
   - Configure the device to connect to your home Wi-Fi network using credentials stored in a `secrets.h` file.

3. **Home Assistant Integration:**
   - Set up Node-RED within Home Assistant to communicate with the ESP8266 device.
   - Create Node-RED flows to handle various home automation events and control the LED ring accordingly.

4. **Web Interface:**
   - Access the web interface through the ESP8266’s IP address to manually control the LED ring’s settings.
   - Adjust brightness, set default colors, change animation types, and set individual LED colors using the provided web controls.

#### Use Cases

- **Notification Alerts:**
   - Use the LED ring to visually notify you of important events such as incoming emails, security alerts, or weather warnings.
- **Ambiance Lighting:**
   - Set the LED ring to a calming glow or a dynamic color roll to enhance the ambiance of your living space.
- **Status Indicators:**
   - Use different colors and animations to indicate the status of various home automation devices, such as door locks, thermostat settings, or light statuses.

#### Troubleshooting and Tips

- **Power Supply:**
   - Ensure a stable power supply to prevent flickering or unexpected behavior of the LEDs.
- **Wi-Fi Connectivity:**
   - Place the ESP8266 within a strong Wi-Fi signal range to ensure consistent communication with Home Assistant.

By leveraging the flexibility of the ESP8266, the versatility of WS2812B LEDs, and the powerful automation capabilities of Home Assistant and Node-RED, the LED Ring Notification Device serves as a sophisticated and customizable solution for enhancing your smart home experience.

## Misc
Wemos d1 mini

about 73cm led sling

Model and some description here: 
https://makerworld.com/en/models/533264#profileId-450094
![20240709_155758](https://github.com/christoferjh/the-one-ring/assets/18284010/37c7ac63-a2a1-4108-9773-2521c9170fe5)

If you open the ipadress of the controller there is a simple contollpage
![image](https://github.com/christoferjh/the-one-ring/assets/18284010/0582cd41-96f0-405f-a511-105f65163e06)
