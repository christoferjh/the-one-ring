#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "secrets.h"
#include "webpage.h"  // Include the header file

#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <FastLED.h>

#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define UPDATES_PER_SECOND 100
#define ANIMATOR_MS_PER_FRAME 10

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;

unsigned long aniframe = 0;
unsigned long lastAnimation = 0;

enum AnimationType {
    COLOR_ROLL,
    FULL_ON,
    COLOR_FOLLOW,
    GLOW, 
    CUSTOM_LEDS,
    LAVA_FLOW
};


const int NUM_ANIMATIONS = 6;
AnimationType animations[NUM_ANIMATIONS] = {COLOR_ROLL, FULL_ON, COLOR_FOLLOW, GLOW, CUSTOM_LEDS, LAVA_FLOW};

template<int DATA_PIN, int NUM_LEDS>
class LEDController {
private:
    CRGB leds[NUM_LEDS];
    int active_led = 0;
    unsigned long aniloop = 0;
    CRGB default_color;
    int ani_mod_1 = 5;
    int current_animation = 0;
    bool isOn = true;

public:
    LEDController() : default_color(255, 0, 0) {
        FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    }

    void setBrightness(uint8_t brightness) {
        FastLED.setBrightness(brightness);
    }

    void fillSolid(const CRGB& color) {
        fill_solid(leds, NUM_LEDS, color);
    }

    void setLED(int index, const CRGB& color) {
        if (index >= 0 && index < NUM_LEDS) {
            leds[index] = color;
        }
    }

    void show() {
        FastLED.show();
    }

    void setDefaultColor(const CRGB& color) {
        default_color = color;
    }
  int getCurrentAnimation(){
    return current_animation;
  }
  void setAnimationId(int animation) {
        current_animation = animation;
    }
    void setAniMod(int mod) {
        ani_mod_1 = mod;
    }
    void turnOn(){
      isOn=true;
    }    
    void turnOff(){
      isOn=false;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
    void animate(){
      if (isOn) {
        animate(animations[current_animation]);
      }
    }
    void animate(AnimationType type) {
        switch (type) {
            case COLOR_ROLL:
                animateColorRoll();
                break;
            case FULL_ON:
                fillSolid(default_color);
                break;
            case COLOR_FOLLOW:
                animateColorFollow();
                break;
            case GLOW:
                animateGlow();
                break;
            case CUSTOM_LEDS:
                // Do nothing, as custom LEDs are set directly
                break;

            case LAVA_FLOW:
                animatelavaflow();
                break;
        }
    }

private:
    void animateColorRoll() {
      if (aniframe % 10 != 0) {
          return; //just run each 10 frames
        }
        active_led++;
        if (active_led >= NUM_LEDS) {
            active_led = 0;
            aniloop++;
        }

        if (aniloop % 3 == 0) {
            setLED(active_led, CRGB::Red);
        } else if (aniloop % 3 == 1) {
            setLED(active_led, CRGB::Green);
        } else {
            setLED(active_led, CRGB::Blue);
        }
    }

    void animateColorFollow() {
      if (aniframe % 10 != 0) {
          return; //just run each 10 frames
        }
        active_led++;
        if (active_led >= NUM_LEDS) {
            active_led = 0;
            aniloop++;
        }

        fillSolid(CRGB::Black);
        setLED(active_led, default_color);

        if (ani_mod_1 > 0) {
            for (int i = 1; i <= ani_mod_1; i++) {
                int offsetLed = (active_led + i) % NUM_LEDS;
                setLED(offsetLed, default_color);
            }
        }
    }

    void animateGlow() {
      if (aniframe % 10 != 0) {
          return; //just run each 10 frames
        }

        static unsigned int glowval = 0;
        static int glowup = 1;

        glowval += 10 * glowup;

        if (glowval >= 254) {
            glowup = -1;
            glowval = 255;
        } else if (glowval <= 0) {
            glowup = 1;
            glowval = 0;
        }

        fillSolid(CRGB(glowval, 0, 0));
    }
    // Helper function to update the lava stream positions
    void updateLavaStream(const unsigned int stream[], unsigned int size,  unsigned int position) {
        unsigned int index = position % size;
        setLED(stream[index], default_color);
    }

    CRGB getLavaHalfColor(){
      
      return CRGB(default_color.red/3 , default_color.green/3, default_color.blue/3);
    }
        // Helper function to move the lava positions and turn off the previous LEDs
    void moveLavaPosition(const unsigned int stream[], unsigned int size, unsigned int &position) {
        unsigned int prev_index = (position - 1 + size) % size;

        //setLED(stream[prev_index], CRGB::Black);
        setLED(stream[prev_index], getLavaHalfColor());
        
        position++;
        if (position >= size) {
            position = 0;
        }
    }
  void animatelavaflow() {
        /* if (aniframe % 10 != 0) {
          return; //just run each 10 frames
        } */

        static unsigned int lava_position_1 = 0;
        static unsigned int lava_position_2 = 0;
        static unsigned int lava_position_3 = 0;
        static unsigned int lava_position_4 = 0;

        // Define the lava stream order
        const unsigned int stream_1[] = {0, 1, 2};
        const unsigned int stream_2[] = {4, 3};
        const unsigned int stream_3[] = {5, 6};
        const unsigned int stream_4[] = {8, 9};

        // Update the lava positions in the streams
        if (aniframe % 20 == 0) {
          updateLavaStream(stream_1, sizeof(stream_1) / sizeof(stream_1[0]), lava_position_1);
          moveLavaPosition(stream_1, sizeof(stream_1) / sizeof(stream_1[0]), lava_position_1);
        }
        
        if (aniframe % 21 == 0) {
        updateLavaStream(stream_2, sizeof(stream_2) / sizeof(stream_2[0]), lava_position_2);
        moveLavaPosition(stream_2, sizeof(stream_2) / sizeof(stream_2[0]), lava_position_2);
        }
        if (aniframe % 31 == 0) {
        updateLavaStream(stream_3, sizeof(stream_3) / sizeof(stream_3[0]), lava_position_3);
        moveLavaPosition(stream_3, sizeof(stream_3) / sizeof(stream_3[0]), lava_position_3);
        }
        if (aniframe % 43 == 0) {
        updateLavaStream(stream_4, sizeof(stream_4) / sizeof(stream_4[0]), lava_position_4);
        moveLavaPosition(stream_4, sizeof(stream_4) / sizeof(stream_4[0]), lava_position_4);
        }
        
        
        
        


/*         // Move the lava positions downwards
        lava_position_1++;
        lava_position_2++;
        lava_position_3++;
        lava_position_4++;

        // Loop positions if they exceed the stream length
        if (lava_position_1 >= sizeof(stream_1) / sizeof(stream_1[0])) {
            lava_position_1 = 0;
        }
        if (lava_position_2 >= sizeof(stream_2) / sizeof(stream_2[0])) {
            lava_position_2 = 0;
        }
        if (lava_position_3 >= sizeof(stream_3) / sizeof(stream_3[0])) {
            lava_position_3 = 0;
        }
        if (lava_position_4 >= sizeof(stream_4) / sizeof(stream_4[0])) {
            lava_position_4 = 0;
        } */
  }

};

LEDController<D2, 33> mainStrip;
LEDController<D4, 10> secondaryStrip;


//int current_animation = 0;



void handleRoot() {
    digitalWrite(led, 1);
    server.send(200, "text/html", webpage);  // Use the webpage string from the header file
    digitalWrite(led, 0);
}

void handleNotFound() {
    digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: " + server.uri() + "\n";
    message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
    message += "Arguments: " + String(server.args()) + "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(led, 0);
}

void setup() {
    pinMode(led, OUTPUT);
    digitalWrite(led, 1);

    mainStrip.setBrightness(BRIGHTNESS);
    mainStrip.setDefaultColor(CRGB(215,250,55));
    secondaryStrip.setBrightness(BRIGHTNESS);
    secondaryStrip.setDefaultColor(CRGB(120,1,0));
    mainStrip.fillSolid(CRGB::Black);
    secondaryStrip.fillSolid(CRGB::Black);

    //Defaults
    mainStrip.setAnimationId(COLOR_FOLLOW);
    mainStrip.setAniMod(14);
    secondaryStrip.setAnimationId(LAVA_FLOW);

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/inline", []() {
        String res = "aniframe timer\n" + String(aniframe);
        server.send(200, "text/plain", res);
    });
    server.on("/getanimation", []() {
        String res = "current_animation Strip 1: " + String(mainStrip.getCurrentAnimation());
        res += "\ncurrent_animation Strip 2: " + String(secondaryStrip.getCurrentAnimation());
        server.send(200, "text/plain", res);
    });
    server.on("/setbrightness", []() {
        int newval = server.arg("val").toInt();
        int strip = -1;if (server.hasArg("strip")){ strip = server.arg("strip").toInt();}
        if (newval >= 0 && newval <= 255) {
            if (strip == 0) {
                mainStrip.setBrightness(newval);
            } else if (strip == 1) {
                secondaryStrip.setBrightness(newval);
            } else {
                mainStrip.setBrightness(newval);
                secondaryStrip.setBrightness(newval);
            }
            server.send(200, "text/plain", "ok");
        } else {
            server.send(400, "text/plain", "bad val");
        }
    });
    server.on("/on", []() {
        int strip = -1;if (server.hasArg("strip")){  strip = server.arg("strip").toInt();}
        
            if (strip == 0) {
                mainStrip.turnOn();
            } else if (strip == 1) {
                secondaryStrip.turnOn();
            } else {
                mainStrip.turnOn();
                secondaryStrip.turnOn();
            }
            server.send(200, "text/plain", "OK");
    });
    server.on("/reboot", []() {
      server.send(200, "text/plain", "OK");
      ESP.restart();
    });
    server.on("/off", []() {
        int strip = -1;if (server.hasArg("strip")){  strip = server.arg("strip").toInt();}
        
            if (strip == 0) {
                mainStrip.turnOff();
            } else if (strip == 1) {
                secondaryStrip.turnOff();
            } else {
                mainStrip.turnOff();
                secondaryStrip.turnOff();
            }
            server.send(200, "text/plain", "OK");
    });

    server.on("/setanimod", []() {
        int newval1 = server.arg("val1").toInt();
        int strip = -1;if (server.hasArg("strip")){  strip = server.arg("strip").toInt();}
        if (newval1 >= 0 && newval1 <= 255) {
            if (strip == 0) {
                mainStrip.setAniMod(newval1);
            } else if (strip == 1) {
                secondaryStrip.setAniMod(newval1);
            } else {
                mainStrip.setAniMod(newval1);
                secondaryStrip.setAniMod(newval1);
            }
            server.send(200, "text/plain", "OK");
        } else {
            server.send(400, "text/plain", "BAD INPUT");
        }
    });
    server.on("/setcolor", []() {
        int newval_R = server.arg("R").toInt();
        int newval_G = server.arg("G").toInt();
        int newval_B = server.arg("B").toInt();
        int strip = -1;if (server.hasArg("strip")){  strip = server.arg("strip").toInt();}
        if (newval_R >= 0 && newval_R <= 255 && newval_G >= 0 && newval_G <= 255 && newval_B >= 0 && newval_B <= 255) {
            CRGB newColor(newval_R, newval_G, newval_B);
            if (strip == 0) {
                mainStrip.setDefaultColor(newColor);
            } else if (strip == 1) {
                secondaryStrip.setDefaultColor(newColor);
            } else {
                mainStrip.setDefaultColor(newColor);
                secondaryStrip.setDefaultColor(newColor);
            }
            server.send(200, "text/plain", "OK");
        } else {
            server.send(400, "text/plain", "BAD INPUT");
        }
    });
    server.on("/setanimation", []() {
        int newani = server.arg("id").toInt();
        int strip = -1;if (server.hasArg("strip")){  strip = server.arg("strip").toInt();}
        if (newani >= 0 && newani < NUM_ANIMATIONS) {
            

            if (strip == 0) {
                mainStrip.setAnimationId(newani);
            } else if (strip == 1) {
                secondaryStrip.setAnimationId(newani);
            } else {
                mainStrip.setAnimationId(newani);
                secondaryStrip.setAnimationId(newani);
            }

            server.send(200, "text/plain", "ok");
        } else {
            server.send(400, "text/plain", "unknown animationId");
        }
    });
      server.on("/setled", []() {
        int strip = -1;if (server.hasArg("strip")){ strip = server.arg("strip").toInt();}
        int led_index = server.arg("led").toInt();
        int r = server.arg("R").toInt();
        int g = server.arg("G").toInt();
        int b = server.arg("B").toInt();

         
        if (strip >= 0 && strip < 2 &&
            led_index >= 0 &&
            r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
            
            if (strip == 0) {
              mainStrip.setAnimationId(CUSTOM_LEDS);
              mainStrip.setLED(led_index, CRGB(r, g, b));
                
            } else if (strip == 1) {
                secondaryStrip.setAnimationId(CUSTOM_LEDS);
                secondaryStrip.setLED(led_index, CRGB(r, g, b));
            } else {
              mainStrip.setAnimationId(CUSTOM_LEDS);
              secondaryStrip.setAnimationId(CUSTOM_LEDS);
                mainStrip.setLED(led_index, CRGB(r, g, b));
                secondaryStrip.setLED(led_index, CRGB(r, g, b));
            }
            


            server.send(200, "text/plain", "OK");
        } else {
            server.send(400, "text/plain", "BAD INPUT");
        }
    });
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void animator() {
    unsigned long now = millis();
    if (now - lastAnimation > ANIMATOR_MS_PER_FRAME) {
        lastAnimation = now;
        aniframe++;
        
        mainStrip.animate();
        secondaryStrip.animate();

        // Move to the next animation every 100 frames (10 seconds)
       // if (aniframe % 100 == 0) {
        //    current_animation = (current_animation + 1) % NUM_ANIMATIONS;
        //}
    }
}

void loop() {
    server.handleClient();
    MDNS.update();
    animator();
    mainStrip.show();
    secondaryStrip.show();
}