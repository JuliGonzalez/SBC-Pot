#include "WiFi.h"
#include "HTTPClient.h"


const int humedadPin = 36;
const int pesoPin = 37;
const int aguaPin = 38;
const int relePin = 22;

const char* ssid = "MOVISTAR_1B78";
const char* password =  "PR7UnUjliPmP99wupwpp";



void setup() {
  Serial.begin(115200);
  pinMode(humedadPin, INPUT);
  pinMode(pesoPin, INPUT);
  pinMode(aguaPin, INPUT);
  pinMode(relePin, OUTPUT);


  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
}

int calcularHumedadSuelo() {
  int valor_humedad = 0;
  valor_humedad = analogRead(humedadPin);
  // Serial.println(valor_humedad);
  return valor_humedad;
}

int calcularPeso() {
  float valor_peso = 0;
  valor_peso = analogRead(pesoPin);
  valor_peso = valor_peso * 18.41;
  // Serial.println(pesoPin);
  return valor_peso;
}

int comprobarAgua() {
  int valor_agua = 0;
  valor_agua = analogRead(aguaPin);
  return valor_agua;
}

void activarRele() {
  digitalWrite(relePin, LOW);
}

void desactivarRele() {
  digitalWrite(relePin, HIGH);
}

int send_data(String name_value, int value){
  if(WiFi.status()== WL_CONNECTED){
   HTTPClient http;   
 
   http.begin("https://demo.thingsboard.io/api/v1/r7Sew3dL20m8iKCpzWIf/telemetry");
   http.addHeader("Content-Type", "application/json");   //Specify content-type header
   //int httpResponseCode = http.POST("{\"value\" : \"40\"}");   //Send the actual POST request
   int httpResponseCode = http.POST("{" + name_value + ":"+ value + "}");   //Send the actual POST request


   if(httpResponseCode>0){
    String response = http.getString();   
    Serial.println(httpResponseCode);
    Serial.println(response);          
   }else{
    Serial.print("Error on sending PUT Request: ");
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
  Serial.print("Humedad suelo: ");
  Serial.println(calcularHumedadSuelo());
  Serial.print("Peso: ");
  Serial.println(calcularPeso());
  send_data("peso", calcularPeso()); 
  Serial.print("Agua detectada: ");
  Serial.println(comprobarAgua());
  activarRele();
  delay(4000);
  desactivarRele();
  delay(1000);
}
