#include <Servo.h>
#include <math.h>

#define angleMinServo 0
#define angleMinOffsetServo 20
#define angleMaxServo 180

#define instaMinFollowers 0
#define instaMaxFollowers 10000

#define waitonesecond 1000

Servo myservo;  // create servo object to control a servo

unsigned int angle = 0;
unsigned int angleold = 0;
unsigned long nbFollowers = 1000;


void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
//  servoInitialisation();
}

void loop() {
  angle = FollowerstoAngle(nbFollowers);
  Serial.println((180-angle)%180);
  
  if (angleold != angle){
      Serial.println("The number of Followers have been changed");
      servoMouvement(angleold, angle);

      angleold = angle;
  }
  
//  Serial.println(angle);
  delay(waitonesecond);
}


int FollowerstoAngle(int nbFollowers){
  int anglegenerated;
  if (nbFollowers >= instaMaxFollowers){
    anglegenerated = angleMaxServo;
  }
  else{
    anglegenerated = maplogarithmic(nbFollowers, instaMinFollowers, instaMaxFollowers, angleMinServo, angleMaxServo-angleMinOffsetServo);
    anglegenerated = (180-anglegenerated)%180;
  }
  return anglegenerated;
}

long maplogarithmic(long x, long in_min, long in_max, long out_min, long out_max) {
  return log10((x - in_min) + 1) / log10(in_max) * out_max;
}

void servoInitialisation(){
  for (unsigned int position = angleMaxServo; position > angleMinOffsetServo; position--) {
    myservo.write(position);
    delay(15);
  }
  
  delay(waitonesecond);

  for (unsigned int position = angleMinOffsetServo; position < angleMaxServo; position++) {
    myservo.write(position);
    delay(15);
  }
}

void servoMouvement(unsigned int valueold, unsigned int valuenew){
//    if (valuenew < valueold){
//      for (unsigned int position = valueold; position > valuenew; position--) {
//        myservo.write(position);
//        delay(15);
//      }
//    }
//    else{
//      for (unsigned int position = valueold; position < valuenew; position++) {
//        myservo.write(position);
//        delay(15);
//      }
//    }
  myservo.write(valuenew);
}
