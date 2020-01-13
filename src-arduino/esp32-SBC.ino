#include <SHTSensor.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include <Wire.h>
#include <VEML7700.h>
#include "iAQcore.h"

//inicializacion de los objetos de i2c
VEML7700 als;
//Adafruit_VEML7700 als;
iAQcore iaqcore;
SHTSensor sht;

const int humedadPinINT = 36;
const int humedadPinEXT = 39;
const int pesoPin = 37;
const int aguaPin = 38;
const int relePin = 22;

const int ledRojo = 4;
const int ledVerde = 17;
const int ledAzul = 16;


const char* ssid = "MOVISTAR_1B78";
const char* password =  "PR7UnUjliPmP99wupwpp";
const char* token = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJmamdhcmNpYS5hbHZhcmV6QGhvdG1haWwuY29tIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJkNjA2YWU1MC1lYWQwLTExZTktYmM3Mi1lOWQyMzA2NTUzOTYiLCJmaXJzdE5hbWUiOiJGcmFuY2lzY28gSmF2aWVyIiwibGFzdE5hbWUiOiJHYXJjaWEgQWx2YXJleiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImQ1ODA4ZTYwLWVhZDAtMTFlOS1iYzcyLWU5ZDIzMDY1NTM5NiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAiLCJpc3MiOiJ0aGluZ3Nib2FyZC5pbyIsImlhdCI6MTU3ODY1NDMyOCwiZXhwIjoxNTgwNDU0MzI4fQ.kTEpGzZbAMWk3YR3ItAEg8i6vIwajJi0tPCDmrH5SvMdO_1G3NUOhTiFpc_dsQtPhnR7OiIhdq2td-Q6-HuxPg";



void setup() {
  Serial.begin(115200);
  pinMode(humedadPinINT, INPUT);
  pinMode(humedadPinEXT, INPUT);
  pinMode(pesoPin, INPUT);
  pinMode(aguaPin, INPUT);
  pinMode(relePin, OUTPUT);

  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);


  Wire.begin(21, 22);
  als.begin();

  if (sht.init()) {
      Serial.print("init(): success\n");
  } else {
      Serial.print("init(): failed\n");
  }

  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void led_parpadeo(int color){
  //digitalWrite(ledRojo, LOW);
  //digitalWrite(ledVerde, LOW);
  for (int i = 0; i < 10; i++){
    digitalWrite(color, HIGH);
    delay(500);
    digitalWrite(color, LOW);
    Serial.println(i);
    delay(500);
  }
}

int calcularPeso() {
  float valor_peso = 0;
  valor_peso = analogRead(pesoPin);
  valor_peso = valor_peso * 18.41;
  Serial.print("Peso: "); Serial.println(valor_peso);
  return valor_peso;
}

int calcularHumedadSueloINT() {
  int valor_humedad = 0;
  valor_humedad = analogRead(humedadPinINT);
  valor_humedad = 1024 - (valor_humedad / 4);
  Serial.print("Valor humedad INT: "); Serial.println(valor_humedad);
  return valor_humedad;
}

int calcularHumedadSueloEXT() {
  int valor_humedad = 0;
  valor_humedad = analogRead(humedadPinINT);
  valor_humedad = 1024 - (valor_humedad / 4);
  Serial.print("Valor humedad EXT: "); Serial.println(valor_humedad);
  return valor_humedad;
}

int comprobarAgua() {
  int valor_agua = 0;
  valor_agua = analogRead(aguaPin);
  if (valor_agua == 4095) valor_agua = 0;
  Serial.print("Agua detectada: "); Serial.println(valor_agua);
  return valor_agua;
}

float read_lux() {
  float luxis;
  als.getALSLux(luxis);
  Serial.print("Luminosidad: "); Serial.println(luxis);
  return luxis;
}

int read_co2() {
  uint16_t eco2;
  uint16_t stat;
  uint32_t resist;
  uint16_t etvoc;
  iaqcore.read(&eco2,&stat,&resist,&etvoc);
  Serial.print("eco2: "); Serial.println(eco2);
  return eco2;
}

float read_temperature() {
  float temperature;
  if (sht.readSample()) {
      temperature = sht.getTemperature();
  } else {
      temperature = 0;
  }
  Serial.print("temperature: "); Serial.println(temperature);
  return temperature;
}

float read_humidity() {
  float humidity;
  if (sht.readSample()) {
      humidity = sht.getHumidity();
  } else {
      humidity = 0;
  }
  Serial.print("humidity: "); Serial.println(humidity);
  return humidity;
}

void activarRele() {
  digitalWrite(relePin, LOW);
}

void desactivarRele() {
  digitalWrite(relePin, HIGH);
}

int comprobarSistema() {
  int rele;
  int humedad_tierra_int, humedad_tierra_ext, humedad_tierra_total;
  humedad_tierra_int = calcularHumedadSueloINT();
  humedad_tierra_ext = calcularHumedadSueloEXT();
  humedad_tierra_total = (humedad_tierra_int + humedad_tierra_ext) / 2;

  if (humedad_tierra_total < 350) {
    activarRele();
    led_parpadeo(ledAzul);
    rele = 1;
    return rele;
  }
  else{
    desactivarRele();
    rele = 0;
    return rele;
  }
}

int send_data(String name_value, float value){
  if(WiFi.status()== WL_CONNECTED){
   HTTPClient http;

   http.begin("https://demo.thingsboard.io/api/v1/r7Sew3dL20m8iKCpzWIf/telemetry");
   http.addHeader("Content-Type", "application/json");
   http.addHeader("X-Authorization", token);
   int httpResponseCode = http.POST("{" + name_value + ":"+ value + "}");   //Send the actual POST request


   if(httpResponseCode>0){
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
   }else{
    Serial.print("Error on sending POST Request: ");
    Serial.println(httpResponseCode);
   }
   http.end();
   return httpResponseCode;
 }else{
    Serial.println("Error in WiFi connection");
    return 0;
 }
}

void loop() {
  int rele;
  rele = comprobarSistema();
  send_data("peso", calcularPeso());
  send_data("humedad_suelo_INT", calcularHumedadSueloINT());
  send_data("humedad_suelo_EXT", calcularHumedadSueloEXT());
  send_data("humedad_aire", read_humidity());
  send_data("co2", read_co2());
  send_data("luminosidad", read_lux());
  send_data("temperatura", read_temperature());
  send_data("agua_detectada", comprobarAgua());
  send_data("rele", rele);
  delay(5000);
}