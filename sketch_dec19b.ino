
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ArduinoJson.h>
// Set these to run example.
#define FIREBASE_HOST "iotproject1-8da9f.firebaseio.com"
#define FIREBASE_AUTH "b6yQBZc00Q4nzryuBKyn7OtOtskbi69hD0ICbQlz"
#define WIFI_SSID "SP"
#define WIFI_PASSWORD "SPagdmgv2017"

int trigPin1=4;
int echoPin1=0;

int trigPin2=2;
int echoPin2=14;

int n;

int timezone = 3 * 3600;
int dst = 3600;


void setup() {
    Serial.begin(9600);

    

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");   
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
   pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

}


void loop() {

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);


  String gun = String(p_tm->tm_mday);
  String ay = String(p_tm->tm_mon + 1);
  String yil = String(p_tm->tm_year + 1900);
  String saat = String(p_tm->tm_hour)+":"+String(p_tm->tm_min)+":"+String(p_tm->tm_sec);

  
  delay(1000);

   StaticJsonBuffer<200> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["gun"] = gun;
        root["ay"] = ay;
        root["yil"] = yil;
        root["saat"] =saat;
        
  
  long duration1, distance1;
  digitalWrite(trigPin1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;

   if (distance1 <= 150 && distance1 >= 0){
    Serial.println("WELCOME");
   
 Firebase.push("musterisayi", root);

  delay(3000);
 
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  }
  else {
    Serial.print ( "Sensor1  ");
    Serial.print ( distance1);
    Serial.println("cm");
  }
  
  delay(1000);
long duration2, distance2;
  digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2= (duration2/2) / 29.1;

  
  
   if (distance2 <= 150 && distance2 >= 0){
    Serial.println("GOOD BYE");
  }
  else {
    Serial.print("Sensor2  ");
    Serial.print(distance2);
    Serial.println("cm");
  }
  
  delay(1000);
 

}
