#include <Servo.h>
#include <math.h>

#define angleMinServo 0
#define angleMaxServo 180

#define instaMinFollowers 0
#define instaMaxFollowers 10000

#define waitonesecond 1000

Servo myservo;  // create servo object to control a servo


void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  servoEffect();
  myservo.read();

}

void loop() {
  Serial.println(FollowerstoAngle(32));
  delay(waitonesecond);
  myservo.write(FollowerstoAngle(32));
  myservo.read();

}

int FollowerstoAngle(int nbFollowers){
  int angle;
  if (nbFollowers > instaMaxFollowers){
    angle = angleMaxServo;
  }
  else{
    angle = maplogarithmic(nbFollowers, instaMinFollowers, instaMaxFollowers, angleMinServo, angleMaxServo);
  }
  return angle;
}

long maplogarithmic(long x, long in_min, long in_max, long out_min, long out_max) {
  return log10((x - in_min) + 1) / log10(in_max) * out_max;
}

void servoEffect(){
    for (int position = 0; position <= 180; position++) {
    myservo.write(position);
    delay(15);
  }

  delay(waitonesecond);
  // Fait bouger le bras de 180° à 10°
  for (int position = 180; position >= 0; position--) {
    myservo.write(position);
    delay(15);
  } 
}
