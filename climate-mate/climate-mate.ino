
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DHT related
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// SSD1306 related
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//application related
#define HUMVIEWPORTW 100
#define HUMVIEWPORTH 30
#define HUMVIEWPORTPOSX (128 - HUMVIEWPORTW)/2
#define HUMVIEWPORTPOSY 0
#define TEMVIEWPORTW HUMVIEWPORTW
#define TEMVIEWPORTH 30
#define TEMVIEWPORTPOSX (128 - TEMVIEWPORTW)/2
#define TEMVIEWPORTPOSY 33
#define TMIN 15
#define TMAX 35
#define HMIN 20
#define HMAX 70

#define UPDATE_PLOT_INTERVAL 5*60 // expressed in seconds

byte i = 0;
byte ah[HUMVIEWPORTW];
byte at[TEMVIEWPORTW];
byte h;
byte t;
byte h_coord;
byte t_coord;

void setup() {
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(13, OUTPUT);
}

void loop() {

  for (byte m = 0; m < UPDATE_PLOT_INTERVAL; m++) {

    // measurements
    t = dht.readTemperature();
    h = dht.readHumidity();
    // calculations of coordinates on plot
    t_coord = map(constrain(t, TMIN, TMAX), TMIN, TMAX, TEMVIEWPORTPOSY + TEMVIEWPORTH, TEMVIEWPORTPOSY);
    h_coord = map(constrain(h, HMIN, HMAX), HMIN, HMAX, HUMVIEWPORTPOSY + HUMVIEWPORTH, HUMVIEWPORTPOSY);
    // storage
    at[i] = t_coord;
    ah[i] = h_coord;
  
    display.clearDisplay();
  
    drawAxes();
    writeValues();
      
    for (byte j = 0; j < HUMVIEWPORTW; j++) {
      display.drawPixel(HUMVIEWPORTPOSX + j, ah[(i + j) % HUMVIEWPORTW], WHITE);
    }
    for (byte j = 0; j < TEMVIEWPORTW; j++) {
      display.drawPixel(TEMVIEWPORTPOSX + j, at[(i + j) % TEMVIEWPORTW], WHITE);
    }
  
    display.display();

    // sleep a second with short beat
    delay(900);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    
  }

  digitalWrite(13, HIGH); // long beat to signalize plot cycle

  if (i < HUMVIEWPORTW - 1) {
    i++; // increment the offset
  } else {
    i = 0; // reset the offset
  }

}

// functions

void drawAxes() {
    //HUM axes etc
    display.drawRect(HUMVIEWPORTPOSX, HUMVIEWPORTPOSY, HUMVIEWPORTW, HUMVIEWPORTH, WHITE);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, HUMVIEWPORTPOSY);
    display.println(HMAX);
    display.setCursor(0, HUMVIEWPORTPOSY + HUMVIEWPORTH - 8);
    display.println(HMIN);
  
    //TEM axes etc
    display.drawRect(TEMVIEWPORTPOSX, TEMVIEWPORTPOSY, TEMVIEWPORTW, TEMVIEWPORTH, WHITE);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, TEMVIEWPORTPOSY);
    display.println(TMAX);
    display.setCursor(0, TEMVIEWPORTPOSY + TEMVIEWPORTH - 8);
    display.println(TMIN);
}

void writeValues() {
    //values on the right
    display.setTextSize(1);
    display.setCursor(HUMVIEWPORTPOSX + HUMVIEWPORTW + 2, h_coord - 4);
    display.println(h);
    display.setCursor(TEMVIEWPORTPOSX + TEMVIEWPORTW + 2, t_coord - 4);
    display.println(t);
}
