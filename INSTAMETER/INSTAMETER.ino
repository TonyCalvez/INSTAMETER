
#include "InstagramStats.h"

 // ----------------------------
 // Standard Libraries - Already Installed if you have ESP32 set up
 // ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
#include <math.h>
#include <ESP32Servo.h>

// ----------------------------

#include "JsonStreamingParser.h"
// Used to parse the Json code within the library
// Available on the library manager (Search for "Json Streamer Parser")
// https://github.com/squix78/json-streaming-parser

#define angleMinServo 0
#define angleMinOffsetServo 20
#define angleMaxServo 180

#define instaMinFollowers 0
#define instaMaxFollowers 1000000

#define waitonesecond 1000

#define servoPin 14

Servo myservo;  // create servo object to control a servo

//------- Replace the following! ------

unsigned int angle = 0;
unsigned int angleold = 0;
unsigned long nbFollowers = 1000000;

char ssid[] = "netbook";       // your network SSID (name)
char password[] = "netbook1";  // your network key

WiFiClientSecure client;
InstagramStats instaStats(client);

unsigned long delayBetweenChecks = 60000; //mean time between api requests
unsigned long whenDueToCheck = 0;

//Inputs
String userName = "MyFrenchFox"; // from their instagram url https://www.instagram.com/MyFrenchFox/


void setup() {

  Serial.begin(115200);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  servoInit();
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

    whenDueToCheck = timeNow + delayBetweenChecks;
  }

//  INITIALISATION




  angle = FollowerstoAngle(nbFollowers);
  Serial.println((180-angle)%160);
  Serial.print(" °");


  if (angleold != angle){
      Serial.println("The number of Followers have been changed!");
      servoMouvement(angleold, angle);
      angleold = angle;
  }
  
  
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
   myservo.attach(servoPin, 500, 2400);   // attaches the servo on pin 14 to the servo object
    if (valuenew < valueold){
      for (unsigned int position = valueold; position > valuenew; position--) {
        myservo.write(position);
        delay(25);
      }
    }
    else if (valuenew > valueold){
      for (unsigned int position = valueold; position < valuenew; position++) {
        myservo.write(position);
        delay(25);
      }
    }
    myservo.detach();  // attaches the servo on pin 9 to the servo object
}

void servoInit(){
  myservo.attach(servoPin, 500, 2400);   // attaches the servo on pin 14 to the servo object

  myservo.write(angleMinServo);
  delay(angleMinServo);
  myservo.write(angleMaxServo);

  myservo.detach();  // attaches the servo on pin 9 to the servo object

}



void getInstagramStatsForUser() {
  Serial.println("Getting instagram user stats for " + userName );
  InstagramUserStats response = instaStats.getUserStats(userName);
  Serial.println("Response:");
  Serial.print("Number of followers: ");
  Serial.println(response.followedByCount);
  nbFollowers = response.followedByCount;

}
