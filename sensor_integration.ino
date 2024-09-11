#include <WiFiNINA.h>
#include <dht.h>
#include <SPI.h>      
#include <MFRC522.h> 
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define DHT22_PIN 3 //humidity and temperatrue sensor
#define DHTTYPE DHT22 //humidity and temperatrue sensor type
#define VIB_PIN 4 //vibration sensor
#define RST_PIN 5 //RFID reader reset pin
#define SS_PIN 53 //RFID reader slave select pin
#define FAN_RELAY 6 //relay that controls the exuhast fan
#define AC_RELAY 7 //relay that controls the AC

MFRC522 reader(SS_PIN, RST_PIN);
SoftwareSerial gpsSerial(1, 2); // TX, RX pins for GPS module


char ssid[] = ""; //connecting to the router
char pass[] = "";
char server[] = ""; 
int status = WL_IDLE_STATUS;
const int INHIBIT_MUX = 7;
const int MUX_PINS[] = {8, 9, 10};
String postData;

TinyGPSPlus gps;
WiFiClient client;

void setup() {

  Serial.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();
  SPI.begin();
  reader.PCD_Init();
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(AC_RELAY, OUTPUT)
  pinMode(INHIBIT_MUX, OUTPUT);
  digitalWrite(INHIBIT_MUX, LOW);

  for (int pin : MUX_PINS) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  //get current location and time 
  gps();
   
  //read RFID tag data
  muxedRFID();
  recommended_temp = xx
  recommended_humidity = xx

  //temperature and humidity sensing and contorl
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) { //sensor is malfunctioning
 

    return;
  }
    
  if (temperature > recommended_temperature) {
    digitalWrite(AC_RELAY, HIGH); // Turn on fan
  } else {
    digitalWrite(AC_RELAY, LOW); // Turn off fan
  }

  if (humidity > recommended_humidity) {
    digitalWrite(FAN_RELAY, HIGH); // Turn on AC
  } else {
    digitalWrite(FAN_RELAY, LOW); // Turn off AC
  }


  postData = xx ; //data thats getting uploaded to the database

  //upload status and other information to the database
  if (client.connect(server, 80)) {
    client.println("POST ");
    client.println("Host: ");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
  }

  if (client.connected()) {
    client.stop();
  }
  Serial.println(postData);

  delay(3000);
}

void muxedRFID() {
  for (int i =0; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      digitalWrite(MUX_PINS[j], (i >> j) & 1);
    }
    rfid_read();
    delay(wait);
  }

  digitalWrite(INHIBIT_MUX, HIGH);
  delay(wait);
  digitalWrite(INHIBIT_MUX, LOW);
}

void rfid_read() {
  if (reader.PICC_IsNewCardPresent()) {
    if (reader.PICC_ReadCardSerial()) {
      Serial.print("UID: ");
      for (byte i = 0; i < reader.uid.size; i++) {
        Serial.print(reader.uid.uidByte[i], HEX);
      }
      Serial.println();
      reader.PICC_HaltA();
      reader.PCD_StopCrypto();
    }
  }

void gps()
{
    while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gps.encode(c);

    if (gps.location.isUpdated() && gps.date.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      date = gps.date();
      time = gps.time();
    }
  }
}