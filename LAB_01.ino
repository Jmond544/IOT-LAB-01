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

const int screenWidth = 240;
const int screenHeight = 320;

// Define colors
#define BLACK   0x0000
#define GRAY    0x1111
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

  // Definir las coordenadas para dibujar
  uint16_t x = 0, y = 100;
  
  // Actualizar solo la parte de la pantalla correspondiente
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);

  switch (buttonPressed) {
    case 1: // Calidad de aire
      metricasMq135(x, y);
      break;
    case 2: // Humedad y temperatura
      metricasDHT11(x, y);
      break;
    default: // Bienvenida
      bienvenida(x, y);
  }
}

void metricasMq135 (uint16_t x, uint16_t y) {
  // Leer el sensor MQ135
  int sensorMqValue = analogRead(PIN_MQ135);

  // Imprimir Calidad de aire
  String calidadStr = " Calidad de aire:";
  int16_t x1, y1;
  uint16_t wi, he;
  tft.setTextSize(2);
  tft.setTextColor(BLACK, CYAN);
  tft.getTextBounds(calidadStr, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 - 80);
  tft.println(calidadStr);

  // Imprimir valor calidad de aire
  tft.setTextColor(CYAN, BLACK);
  tft.setTextSize(4);
  String calidadVal = String(sensorMqValue) + " ppm";
  tft.getTextBounds(calidadVal, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 - 50);
  tft.println(calidadVal);

  tft.setTextSize(3);
  String statusCalidad;
  if (sensorMqValue <= 350) {
    tft.setTextColor(BLACK, GREEN);
    statusCalidad = "Calidad alta";
  } else if (sensorMqValue > 350 && sensorMqValue <= 500) {
    tft.setTextColor(BLACK, GREEN);
    statusCalidad = "Calidad buena";
  } else if (sensorMqValue > 500 && sensorMqValue <= 800) {
    tft.setTextColor(BLACK, YELLOW);
    statusCalidad = "Calidad moderada";
  } else if (sensorMqValue > 800 && sensorMqValue <= 1200) {
    tft.setTextColor(BLACK, RED);
    statusCalidad = "Calidad baja";
  } else {
    tft.setTextColor(BLACK, RED);
    statusCalidad = "Calidad mala";
  }

  tft.getTextBounds(statusCalidad, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 + 40);
  tft.println(statusCalidad);
}

void metricasDHT11 (uint16_t x, uint16_t y) {
  // Leer el sensor DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Imprimir humedad
  tft.setTextSize(3);
  tft.setTextColor(BLACK, CYAN);
  tft.setCursor(x, y-40);
  String humedadStr = " Humedad:";
  int16_t x1, y1;
  uint16_t wi, he;
  tft.getTextBounds(humedadStr, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 - 80);
  tft.println(humedadStr);

  // Imprimmir valor humedad
  tft.setTextSize(4);
  tft.setTextColor(CYAN, BLACK);
  String humedadVal = String(h) + " %";
  tft.getTextBounds(humedadVal, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 - 50);
  tft.print(humedadVal);
  tft.println("");

  // Imprimir temperatura
  tft.setTextSize(3);
  tft.setTextColor(BLACK, YELLOW);
  String tempStr = " Temperatura:";
  tft.getTextBounds(tempStr, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 - 10);
  tft.println(tempStr);


  // Imprimir valor de temperatura
  tft.setTextSize(4);
  tft.setTextColor(YELLOW, BLACK);
  String tempVal = String(t) + " C";
  tft.getTextBounds(tempVal, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 + 20);
  tft.print(tempVal);

  int rectX = x;
  int rectY = y + 47;
  int rectWidth = 216; // Ajusta el ancho del rectángulo según sea necesario
  int rectHeight = 60; // Ajusta la altura del rectángulo según sea necesario
  
  //tft.drawRect(rectX, rectY, rectWidth, rectHeight, YELLOW);

  String statusTemp;
  
  if(t>20){
    tft.setTextColor(RED, BLACK);
    statusTemp = "CALOR";
  }else if (t > 12){
    tft.setTextColor(YELLOW, BLACK);
    statusTemp = "TEMPLADO";
  }else {
    tft.setTextColor(GREEN, BLACK);
    statusTemp = "FRIO";
  }
  tft.getTextBounds(statusTemp, 0, 0, &x1, &y1, &wi, &he);
  tft.setCursor((screenWidth - wi) / 2, screenHeight / 2 + 70);
  tft.println(statusTemp);
}

void bienvenida(uint16_t x, uint16_t y) {
  drawSun();
  tft.setCursor(30, y);
  tft.setTextColor(YELLOW, BLACK);
  tft.println("= BIENVENIDOS =");
  tft.setCursor(25, y+40);
  tft.setTextColor(RED, BLACK);
  tft.println("--- GRUPO 11 ---");
  tft.println("");
  tft.setTextColor(GRAY, WHITE);
  tft.println("Presione un boton...");
  tft.println("");
  tft.setTextColor(ORANGE, BLACK);
  tft.println("Integrantes:");
  tft.setTextColor(WHITE, BLACK);
  tft.println("- Castillon Gabriel");
  tft.println("- Mondalgo Tapia");
  tft.println("- Pineda Silupu");
  tft.println("- Gutierrez Mudarra");
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