#include <Arduino.h>
// LCD Library
#include<LiquidCrystal.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <HTTPClient.h>

#include <ESPmDNS.h>
#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>


#define DHTPIN 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);


char ssid[] = "RedmiK50i";  // type your wifi name
char pass[] = "bocho1234";  // type your wifi password

//IFTTT Variables
String ifttt_server = "http://maker.ifttt.com";
String eventName = "temp_data";
String IFTTT_Key = "glkEyPRHsZIwc5ashYk5Ogke1LpRnmWjhKmiRBMLkZU";
String IFTTTUrl = "https://maker.ifttt.com/trigger/temp_data/with/key/glkEyPRHsZIwc5ashYk5Ogke1LpRnmWjhKmiRBMLkZU";

int value1;
int value2;

//LCD
LiquidCrystal lcd(22,21,5,18,23,19);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "NEBULA"
#define WIFI_PASSWORD "Varun2325"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCnfS_Y9g7MUjO4rs45Vmv94olJoS0PpNI"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://innovolt-7c863-default-rtdb.asia-southeast1.firebasedatabase.app/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
int intValue;
float floatValue;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);



  

  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");


   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

   // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

    if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
 
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  

  /* Assign the api key (required) */
  config.api_key = "AIzaSyCnfS_Y9g7MUjO4rs45Vmv94olJoS0PpNI";

  /* Assign the RTDB URL (required) */
  config.database_url = "https://innovolt-7c863-default-rtdb.asia-southeast1.firebasedatabase.app/";


  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  
}

void handleRoot(){
  char msg[1500];

  snprintf(msg, 1500, 
  "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>ESP32 DHT Server!</h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}


void sendSensor()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}


float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(temperature);
    return temperature;
  }
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(humidity);
    return humidity;
  }
}

void loop(){

  value1 = dht.readHumidity();
  value2 = dht.readTemperature();
  Serial.println("Value 1:");
  Serial.println(value1);
  Serial.println("Value 2:"); 
  delay(300);
//    sendDataToEmail();
  sendDataToSheet();

  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
  lcdDisplay();
  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    /* TEST INT IS THE INTEGER WHICH IS SAVED*/
    if (Firebase.RTDB.setInt(&fbdo, "Station/Temperature", readDHTTemperature())){  /* COUNT IS THE VARIABLE , THE VALUE STORES IN THIS IS GETTING PASSED IN THE DATABASE PATH*/
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setInt(&fbdo, "Station/Humidity", readDHTHumidity())){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}

void sendDataToSheet(void){
  String url = ifttt_server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((int)value1) + "&value2=" + String((int)value2);
  Serial.print(url);

  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url);
  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();
  if(httpCode > 0){
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(payload);
    }
  }else{
    Serial.printf("[HTTP] GET... failed, %s", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void lcdDisplay(void){
lcd.begin(16,2);
lcd.clear();
delay(500);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());

  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(dht.readHumidity());
}


void sendDataToEmail(void){
  String url2 = ifttt_server + "/triger/email_send" + "/with/key/" + IFTTT_Key + "?value1=" + String(value1) + "&value2=" + String(value2);
  Serial.print(url2);
  delay(1000);

  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url2);
  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();
  if(httpCode > 0){
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(payload);
    }
  }else{
    Serial.printf("[HTTP] GET... failed, %s", http.errorToString(httpCode).c_str());
  }
  http.end();
}
