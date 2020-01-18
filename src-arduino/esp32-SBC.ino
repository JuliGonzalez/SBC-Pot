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
const int relePin = 13;

const int ledRojo = 4;
const int ledVerde = 17;
const int ledAzul = 16;


const char* ssid = "MOVISTAR_1B78";
// const  char* ssid = "iPhuli"
// const char* password = "holaquetal"
const char* password =  "PR7UnUjliPmP99wupwpp";
const char* token = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJmamdhcmNpYS5hbHZhcmV6QGhvdG1haWwuY29tIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJkNjA2YWU1MC1lYWQwLTExZTktYmM3Mi1lOWQyMzA2NTUzOTYiLCJmaXJzdE5hbWUiOiJGcmFuY2lzY28gSmF2aWVyIiwibGFzdE5hbWUiOiJHYXJjaWEgQWx2YXJleiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImQ1ODA4ZTYwLWVhZDAtMTFlOS1iYzcyLWU5ZDIzMDY1NTM5NiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAiLCJpc3MiOiJ0aGluZ3Nib2FyZC5pbyIsImlhdCI6MTU3ODY1NDMyOCwiZXhwIjoxNTgwNDU0MzI4fQ.kTEpGzZbAMWk3YR3ItAEg8i6vIwajJi0tPCDmrH5SvMdO_1G3NUOhTiFpc_dsQtPhnR7OiIhdq2td-Q6-HuxPg";

int co2OK = 1;
int humedadOK = 1;

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
  return valor_peso;
}

int calcularHumedadSueloINT() {
  int valor_humedad = 0;
  valor_humedad = analogRead(humedadPinINT);
  valor_humedad = 1024 - (valor_humedad / 4);
  return valor_humedad;
}

int calcularHumedadSueloEXT() {
  int valor_humedad = 0;
  valor_humedad = analogRead(humedadPinINT);
  valor_humedad = 1024 - (valor_humedad / 4);
  return valor_humedad;
}

int comprobarAgua() {
  int valor_agua = 0;
  valor_agua = analogRead(aguaPin);
  if (valor_agua > 3500) valor_agua = 0;
  return valor_agua;
}

float read_lux() {
  float luxis;
  als.getALSLux(luxis);
  return luxis;
}

int read_co2() {
  uint16_t eco2;
  uint16_t stat;
  uint32_t resist;
  uint16_t etvoc;
  iaqcore.read(&eco2,&stat,&resist,&etvoc);
  if (eco2 > 30000){
    eco2 = 449;
  }
  return eco2;
}

float read_temperature() {
  float temperature;
  if (sht.readSample()) {
      temperature = sht.getTemperature();
  } else {
      temperature = 0;
  }
  return temperature;
}

float read_humidity() {
  float humidity;
  if (sht.readSample()) {
      humidity = sht.getHumidity();
  } else {
      humidity = 0;
  }
  return humidity;
}

void activarRele() {
  digitalWrite(relePin, HIGH);
}

void desactivarRele() {
  digitalWrite(relePin, LOW);
}

int comprobarSistema() {
  int rele, agua_detectada;
  int humedad_tierra_int, humedad_tierra_ext, humedad_tierra_total;
  int co2;
  co2 = read_co2();
  if (co2 > 1000){
    led_parpadeo(ledRojo);
    co2OK = 0;
  }else if (co2 < 1500 and co2OK == 0){
    led_parpadeo(ledVerde);
    co2OK = 1;
  }

  humedad_tierra_int = calcularHumedadSueloINT();
  humedad_tierra_ext = calcularHumedadSueloEXT();
  humedad_tierra_total = (humedad_tierra_int + humedad_tierra_ext) / 2;
  agua_detectada = comprobarAgua();


  if (humedad_tierra_total < 420) {
    activarRele();
    led_parpadeo(ledAzul);
    rele = 1;
    humedadOK = 0;
    return rele;
  }
  else if ((agua_detectada > 150 or humedad_tierra_total > 420) and humedadOK == 0) {
    desactivarRele();
    led_parpadeo(ledVerde);
    humedadOK = 1;
    rele = 0;
    return rele;
  }
  return rele;
}

int send_data(String name_value, float value){
  if(WiFi.status()== WL_CONNECTED){
   HTTPClient http;

   http.begin("https://demo.thingsboard.io/api/v1/r7Sew3dL20m8iKCpzWIf/telemetry");
   http.addHeader("Content-Type", "application/json");
   http.addHeader("X-Authorization", token);
   int httpResponseCode = http.POST("{" + name_value + ":"+ value + "}");  //Send the actual POST request


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
  int rele, peso, humedad_int, humedad_ext, co2, agua;
  float luminosidad, temperatura, humedad;
  rele = comprobarSistema();

  peso = calcularPeso();
  Serial.print("Peso: "); Serial.println(peso);
  send_data("peso", peso);

  humedad_int = calcularHumedadSueloINT();
  Serial.print("Valor humedad INT: "); Serial.println(humedad_int);
  send_data("humedad_suelo_INT", humedad_int);

  humedad_ext = calcularHumedadSueloEXT();
  Serial.print("Valor humedad EXT: "); Serial.println(humedad_ext);
  send_data("humedad_suelo_EXT", humedad_ext);

  humedad = read_humidity();
  Serial.print("humidity: "); Serial.println(humedad);
  send_data("humedad_aire", humedad);

  co2 = read_co2();
  Serial.print("co2: "); Serial.println(co2);
  send_data("co2", co2);

  luminosidad = read_lux();
  Serial.print("luminosidad: "); Serial.println(luminosidad);
  send_data("luminosidad", luminosidad);

  temperatura = read_temperature();
  Serial.print("temperature: "); Serial.println(temperatura);
  send_data("temperatura", temperatura);

  agua = comprobarAgua();
  Serial.print("Agua detectada: "); Serial.println(agua);
  send_data("agua_detectada", agua);

  Serial.print("Rele: "); Serial.println(rele);
  send_data("rele", rele);

  //delay of 5s for each bucket of data readed.
  delay(5000);
}