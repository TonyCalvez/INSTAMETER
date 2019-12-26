#include <Servo.h>
#include <math.h>

#define angleMinServo 0
#define angleMinOffsetServo 20
#define angleMaxServo 180

#define instaMinFollowers 0
#define instaMaxFollowers 1000000

#define waitonesecond 1000

Servo myservo;  // create servo object to control a servo

unsigned int angle = 0;
unsigned int angleold = 0;
unsigned long nbFollowers = 1000000;


void setup() {
  Serial.begin(9600);
}

void loop() {
  angle = FollowerstoAngle(nbFollowers);
  Serial.println((180-angle)%160);

  if (angleold != angle){
      Serial.println("The number of Followers have been changed:");
      Serial.println(angle);

      servoMouvement(angleold, angle);
      angleold = angle;
  }
  
//  Serial.println(angle);
  delay(waitonesecond);
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
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object
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
