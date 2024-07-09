/**
 * WARNING.
 * This code... Its something else...
 * If quick and dirty had a poste child, this is it!
 * 
 * use or read on your own risk.
 * 
 * Pinout defined in the code. Got some 220 Ohm resitors on the data pins.
 * Using Wemos D1 mini.
 * 
 * 
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "secrets.h"
//**********  OBS! Create a secrets.h file and copy this in with your values, or change here but don't commit the code then. ********** 
#ifndef STASSID
#define STASSID "xxx"
#define STAPSK  "xxx"
#endif



#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0



#include <FastLED.h>

#define DATA_PIN    D2
#define DATA_PIN2    D4
#define NUM_LEDS2    10
#define NUM_LEDS    33
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

#define UPDATES_PER_SECOND 100

#define ANIMATOR_MS_PER_FRAME 100

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;


//animations
#define ANIMATION_COLOR_ROLL    0
#define ANIMATION_FULL_ON    1
#define ANIMATION_COLOR_FOLLOW    2
int active_animation = ANIMATION_COLOR_ROLL;


void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}


void setactiveanimation(int i){
  active_animation = i;
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  fill_solid( leds2, NUM_LEDS2, CRGB::Black);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

unsigned long aniframe = 0;
unsigned long aniloop = 0;
int color = 0;

int def_c_R=255;
int def_c_G=0;
int def_c_B=0;

int ani_mod_1=1;

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds2, NUM_LEDS2);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  fill_solid( leds2, NUM_LEDS2, CRGB::Black);


  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
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
    String res = "aniframe timer\n";
    res+=aniframe;
    server.send(200, "text/plain", res);
  });

server.on("/getanimation", []() {
    String res = "active_animation\n";
    res+=active_animation;
    server.send(200, "text/plain", res);
  });

 server.on("/setbrightness", []() {
    int newval = server.arg(String("val")).toInt();
    String res = "ok";
    if (newval<0 || newval>255){
      res = "bad val";
    }else{
      FastLED.setBrightness(newval);
    }
    server.send(200, "text/plain", res);
  });

  server.on("/setanimod", []() {
    int newval1 = server.arg(String("val1")).toInt();
    
    if (newval1<0 || newval1>255){
      
      server.send(400, "text/plain", "BAD INPUT");
    }else{
      ani_mod_1=newval1;
      server.send(200, "text/plain", "OK");
    }
    
  });

server.on("/setcolor", []() {
    int newval_R = server.arg(String("R")).toInt();
    int newval_G = server.arg(String("G")).toInt();
    int newval_B = server.arg(String("B")).toInt();
    
    if (newval_R>=0 && newval_R<=255 && newval_G>=0 && newval_G<=255 && newval_B>=0 && newval_B<=255){
    def_c_R = newval_R;
    def_c_G = newval_G;
    def_c_B = newval_B;
      server.send(200, "text/plain", "OK");
    }else {
      server.send(400, "text/plain", "BAD INPUT");
    }

    
  });

  server.on("/setanimation", []() {
    int newani = server.arg(String("id")).toInt();
    String res = "ok";
    if (newani>2 || newani<0){
      res = "unkown ani";
    }else{
      setactiveanimation(newani);
    }
    server.send(200, "text/plain", res);
  });



  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");




}




void animateledsOld(){
  if (aniframe%10!=0){
    return; //run this only each 10 frames (1 sec tot)
  }
 // Turn the LED on
  //leds[0] = CRGB::Red;
  //leds[1] = CRGB::Green;
  //leds[9] = CRGB::Blue;
  //FastLED.show();
  //delay(10);
  //leds[i].setRGB( 255, 68, 221);
  for (int i = 0; i<= NUM_LEDS -1;i++){
    if (color%3==0){
      leds[i] = CRGB::Red;
      //leds2[i] = CRGB::Green;
      }else if (color%3==1){
      leds[i] = CRGB::Green;
      //leds2[i] = CRGB::Red;
      }else{
        leds[i] = CRGB::Blue;
        //leds2[i] = CRGB::Blue;
        }
    //leds2[i+10] = CRGB::Red;
    //leds2[i+20] = CRGB::Red;
    //leds2[32] = CRGB::Green;
    FastLED.show();
    //FastLED.delay(200);
    //delay(50);
    leds[i].setRGB( 0, 0, 0);
    //leds2[i].setRGB( 0, 0, 0);
    //leds2[i+10] = CRGB::Black;
    //leds2[i+20] = CRGB::Black;
  }
  color++;

}
int active_led =0;
void animation_COLOR_ROLL(){
  if (aniframe%10!=0){
    return; //run this only each 10 frames (1 sec tot)
  }
  Serial.println("animation_COLOR_ROLL");
  active_led++;
  if (active_led>=NUM_LEDS){
    active_led=0;
    aniloop++;
  }

  Serial.println(active_led);

   if (aniloop%3==0){
      leds[active_led] = CRGB::Red;
      //leds2[i] = CRGB::Green;
      }else if (aniloop%3==1){
      leds[active_led] = CRGB::Green;
      //leds2[i] = CRGB::Red;
      }else{
        leds[active_led] = CRGB::Blue;
        //leds2[i] = CRGB::Blue;
        }
  FastLED.show();
  //leds[active_led].setRGB( 0, 0, 0);
}

void animation_COLOR_FULL_ON(){
  //if (aniframe%10!=0){
  //  return; //run this only each 10 frames (1 sec tot)
  //}
  fill_solid( leds, NUM_LEDS, CRGB(def_c_R, def_c_G, def_c_B));
}

int getOffsetLedId(int led,int offsett,int maxleds){
  int offled = led+offsett;
  if (offled<0){
    return maxleds+offled;
  }else if (offled>=maxleds){
    return offled-maxleds;
  }
  return offled;
}
void animation_COLOR_FOLLOW(){
  if (aniframe%1!=0){
    return; //run this only each 1 frames (0.2 sec tot)
  }
  Serial.println("animation_COLOR_FOLLOW");
  active_led++;
  if (active_led>=NUM_LEDS){
    active_led=0;
    aniloop++;
  }

  Serial.println(active_led);
  //clear
  fill_solid( leds, NUM_LEDS, CRGB::Black);
   leds[active_led] = CRGB(def_c_R, def_c_G, def_c_B);

  if (ani_mod_1>0){
    for(int i = 1;i<=ani_mod_1;i++){
      leds[getOffsetLedId(active_led,i,NUM_LEDS)] = CRGB(def_c_R, def_c_G, def_c_B);
    }

  }

  FastLED.show();
  //leds[active_led].setRGB( 0, 0, 0);
}

unsigned int glowval=0;
int glowup=1;
void animateleds2(){
  if (aniframe%1!=0){
    return; //run this only each 1 frames (0.2 sec tot)
  }
  
  glowval+=10*glowup;

  if (glowval>=254){
    glowup=-1;
    glowval=255;
  }else if (glowval<=0){
    glowup=1;
    glowval=0;
  }
  //yes, i know the flash bug. But it looks good. Keep for now.
  fill_solid( leds2, NUM_LEDS2, CRGB(glowval, 0, 0));
}

/// ANIMATION_COLOR_FOLLOW
// TODO: Make every animation function take the led array as input, so it can be reused for both led strips. This is just stupid. Later med problem.
void animateleds(){

  switch (active_animation)
  {
  case ANIMATION_COLOR_ROLL:
    animation_COLOR_ROLL();
    break;
  case ANIMATION_FULL_ON:
    animation_COLOR_FULL_ON();
    break;
  case ANIMATION_COLOR_FOLLOW:
    animation_COLOR_FOLLOW();
    break;
  default:
    animation_COLOR_ROLL();
    break;
  }

}

unsigned long lastAnimation = 0;
unsigned long now = 0;
void animator(){
  now = millis();
  if (now-lastAnimation > ANIMATOR_MS_PER_FRAME){
    lastAnimation = now;
    aniframe++;
    animateleds();
    animateleds2();
  }
}


void loop(void) {
  server.handleClient();
  MDNS.update();
  animator();
  FastLED.show();
}
