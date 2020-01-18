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

int sistemaOK = 1;

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