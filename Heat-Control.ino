#include <Arduino.h>
#include <U8g2lib.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

class Button {
  public:
    int pin;
    int state;
    int pressed = 0;

    Button(int pin, int defaultState){
      this->pin = pin;
      this->state = defaultState;
      pinMode(pin, INPUT);
    }
  
    void update() {
      int newState = digitalRead(pin);
      
      this->pressed = state == LOW && newState == HIGH;
      this->state = newState;
    }
    int isPressed(){
      return this->pressed;
    }
};

Button downButton = Button(8, 0);
Button upButton = Button(13, 0);
long lastRead = 0;
float curTemp = 22;

void setup() {
  Serial.begin(9600);
  delay(3000);  // Give serial monitor time to connect
  
  Serial.println(F("==================================="));
  Serial.println(F("Let's control Garance's heating!"));
  Serial.println(F("==================================="));

  u8g2.begin();
  dht.begin();
}

void loop() {
  delay(2000);
  
  if (millis() - lastRead >= 2000) {
    Serial.println("Temp: ");
    //float humidity = dht.readHumidity();
    float newTemp = dht.readTemperature();
    if (isnan(newTemp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.print(newTemp);
    curTemp = newTemp;
    lastRead = millis();
  }
  
  draw(curTemp);

}

void draw(float temp) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_helvB18_tf);
    u8g2.setCursor(25, 48);
    u8g2.print(temp, 1);
    u8g2.print(" \xB0""C");
  } while (u8g2.nextPage());
}

