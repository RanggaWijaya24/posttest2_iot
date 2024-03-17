#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Universitas Mulawarman";    // Nama SSID AP/Hotspot
const char* password = "";  // Password Wifi

const int BUZZER_PIN = D1;
const int LED_PIN = D2;
const int BtnUp = D5;
const int BtnDown = D6;
const int btnONOFF = D7;

bool on = true;
int LED = 0;
int BUZZER = 100;
int BtnUpState = 0;
int BtnDownState = 0;
int btnONOFFState = 0;

ESP8266WebServer server(80);  // Port untuk web server

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BtnUp, INPUT_PULLUP);
  pinMode(BtnDown, INPUT_PULLUP);
  pinMode(btnONOFF, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.println(WiFi.localIP());

  // Membuat file webpage
  server.on("/", handleRoot);

  server.begin();
  Serial.println("Server dijalankan");
}

void loop() {
  server.handleClient();  // Menangani permintaan dari klien
  BtnUpState = digitalRead(BtnUp);
  BtnDownState = digitalRead(BtnDown);
  btnONOFFState = digitalRead(btnONOFF);
  if (BtnUpState == LOW) { // pola jika led di nyalakan menggunakan btn up
    if (LED < 245) {
      LED += 3;
    } 
    if (BUZZER < 2500) {
      BUZZER += 10;
    }
    delay(100);
  }
  if (BtnDownState == LOW) {   // pola jika led di matikan menggunakan btn down
    if (LED > 0){
      LED -= 3;
    }
    if (BUZZER > 100){
      BUZZER -= 10;
    }
    delay(100);
  }
  if (btnONOFFState == LOW) { // tombol on/off 
    on = !on;
    delay(300);
  }

  if (on){
    analogWrite(LED_PIN, LED);
    tone(BUZZER_PIN, BUZZER);
  } else {
    analogWrite(LED_PIN, 0);
    noTone(BUZZER_PIN);
  }

}

void handleRoot() {
  String content = + String(LED) + "</p><br>" + "Tingkat Tone Frekuensi Buzzer = " + String(BUZZER);
  server.send(200, "text/html", content);
}