// -----------------------------------------------------------------------------------------------
// 💑 PROJECT INSTAMETER
//  InstaMeter is a follower counter in Arduino. Project released for "Elles Bougent" Organization.
//  DIY with Tony CALVEZ and Nicolas TB
//  https://github.com/TonyCalvez/INSTAMETER
// -----------------------------------------------------------------------------------------------

// ---- Libraries ----
#include "InstagramStats.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <math.h>
#include <ESP32Servo.h>
#include "JsonStreamingParser.h"

#define onesecond 1000
#define oneminute 60000
#define fiveseconds 5000

// ---- Define Servo Parameters ----
#define angleMinServo 0
#define angleMinOffsetServo 10
#define angleMaxServo 180

// ---- Define the Scale ----
#define instaMinFollowers 0
#define instaMaxFollowers 1000000

// ---- Define the Servo's Pin ----
#define servoPin 14



// ---- WiFi Connection ----
char ssid[] = "netbook";
char password[] = "netbook1";

// ---- Instagram Username ----
String userName = "MyFrenchFox"; // from their instagram url https://www.instagram.com/MyFrenchFox/

// ----------------------
unsigned int angle = 0;
unsigned int angleold = 0;
unsigned long nbFollowers = 1000000;
unsigned long whenDueToCheck = 0;

// ---- Declare Servo Object ----
Servo myservo;  

// ---- Declare WiFi Object ----
WiFiClientSecure client;
InstagramStats instaStats(client);


void setup() {
  Serial.begin(115200);
  
  // Initialisation Servo
  servoInit();


  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > whenDueToCheck))  {
    InstagramUserStats response = instaStats.getUserStats(userName);
    nbFollowers = response.followedByCount;
    
    Serial.print(userName);
    Serial.print(" counts ");
    Serial.print(nbFollowers);
    Serial.println(" Followers");

    whenDueToCheck = timeNow + oneminute;
  }

  angle = FollowerstoAngle(nbFollowers);

  if (angleold != angle){
    
      Serial.println("The number of Followers have been changed!");
      
      servoMouvement(angleold, angle);
      angleold = angle;
  }
  
  delay(oneminute);
  }

void servoInit(){
  myservo.attach(servoPin, 500, 2400);
  
  myservo.write(angleMinServo);
  delay(fiveseconds);
  myservo.write(angleMaxServo - angleMinOffsetServo);
  delay(fiveseconds);
  myservo.write(angleMinServo);
  
  myservo.detach();
}

unsigned int FollowerstoAngle(unsigned long nb){
  unsigned int anglegenerated;
  if (nb > instaMaxFollowers){
    anglegenerated = angleMaxServo;
  }
  else{
    anglegenerated = maplogarithmic(nbFollowers, instaMinFollowers, instaMaxFollowers, angleMinServo, angleMaxServo-angleMinOffsetServo);
  }
  return (180-anglegenerated)%180;
}

long maplogarithmic(unsigned long x, unsigned long in_min, unsigned long in_max, unsigned long out_min, unsigned long out_max) {
  return log10((x - in_min) + 1) / log10(in_max) * out_max;
}


void servoMouvement(unsigned int valueold, unsigned int valuenew){
   myservo.attach(servoPin, 500, 2400);
   
    if (valuenew < valueold){
      for (unsigned int position = valueold; position > valuenew; position--) {
        myservo.write(position);
        delay(75);
      }
    }
    
    else if (valuenew > valueold){
      for (unsigned int position = valueold; position < valuenew; position++) {
        myservo.write(position);
        delay(75);
      }
    }
    
    myservo.detach();
}
