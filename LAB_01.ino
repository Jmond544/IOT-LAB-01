#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include "DHT.h"
#define DHTPIN 10     // Digital pin connected to the DHT sensor
MCUFRIEND_kbv tft;

#define PIN_MQ135 A6  // Analog pin connected to the MQ135
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int buttonPressed = 3; // 1, 2, o 3

float temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
float humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22

const int buttonPin1 = 11;
const int buttonPin2 = 12;
const int buttonPin3 = 13;


// Define colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFD20

void setup()
{
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(buttonPin3, INPUT_PULLUP);
    // put your setup code here, to run once:
    Serial.begin(9600);
    tft.reset();
    uint16_t identifier = tft.readID();
    Serial.print("ID = 0x");
    Serial.println(identifier, HEX);
    if (identifier == 0xEFEF) identifier = 0x9486;
    tft.begin(identifier);
    dht.begin();
    tft.fillScreen(0x0000); // Limpiar la pantalla solo una vez al inicio
}

char *msg[] = { "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV" };
uint8_t aspect;

void loop()
{ 
  // Leer estado de los botones
  if (digitalRead(buttonPin1) == HIGH) {
    uint16_t x = 0, y = 100;
    tft.fillScreen(0x0000);
    buttonPressed = 1;
  } else if (digitalRead(buttonPin2) == HIGH) {
    uint16_t x = 0, y = 100;
    tft.fillScreen(0x0000);
    buttonPressed = 2;
  } else if (digitalRead(buttonPin3) == HIGH) {
    uint16_t x = 0, y = 100;
    tft.fillScreen(0x0000);
    buttonPressed = 3;
  }

  Serial.println(buttonPressed);

  // Leer el sensor MQ135
  int sensorMqValue = analogRead(PIN_MQ135);
  float voltage = sensorMqValue * (5.0 / 1023.0);
  float airQuality = voltage * 100; // Escala el valor a un rango de 0 a 100

  // Leer el sensor DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Definir las coordenadas para dibujar
  uint16_t x = 0, y = 100;
  
  // Actualizar solo la parte de la pantalla correspondiente
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);

  switch (buttonPressed) {
    case 1: // Calidad de aire
      tft.fillRect(x, y, 200, 50, BLACK); // Limpiar área específica
      tft.setCursor(x, y);
      tft.println("* Calidad de aire:");
      tft.println(airQuality);
      break;
    case 2: // Humedad y temperatura
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      tft.fillRect(x, y, 200, 50, BLACK); // Limpiar área específica
      tft.setCursor(x, y);
      tft.println("* Humedad:");
      tft.println(h);
      tft.println("* Temperatura:");
      tft.println(t);
      break;
    default: // Bienvenida
      drawSun();
      tft.setCursor(30, y);
      tft.setTextColor(YELLOW, BLACK);
      tft.println("= BIENVENIDOS =");
      tft.setCursor(25, y+40);
      tft.setTextColor(RED, BLACK);
      tft.println("--- GRUPO 11 ---");
      tft.println("");
      tft.setTextColor(WHITE, BLACK);
      tft.println("Presione un boton...");
      tft.println("");
      tft.println("Integrantes:");
      tft.println("- Castillon Gabriel");
      tft.println("- Mondalgo Tapia");
      tft.println("- Pineda Silupu");
      tft.println("- Gutierrez Mudarra");
  }
}

void drawSun() {
  // Dimensiones de la pantalla
  int16_t screenWidth = tft.width();
  int16_t screenHeight = tft.height();

  // Dibujar un sol en el centro de la pantalla
  int16_t sunCenterX = screenWidth / 2;
  int16_t sunCenterY = screenHeight / 6;
  int16_t sunRadius = 20;
  
  // Dibujar el círculo del sol
  tft.fillCircle(sunCenterX, sunCenterY, sunRadius, YELLOW);
  
  // Dibujar los rayos del sol
  for (int i = 0; i < 360; i += 15) {
    float angle = i * 3.14 / 180;
    int16_t rayX = sunCenterX + cos(angle) * (sunRadius + 20);
    int16_t rayY = sunCenterY + sin(angle) * (sunRadius + 20);
    tft.drawLine(sunCenterX, sunCenterY, rayX, rayY, ORANGE);
  }
}