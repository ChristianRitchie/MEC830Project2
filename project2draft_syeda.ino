#include <Servo.h>

//initializing servo
const int my_servo = 6;

//initializing joystick 
const int val_x = 0;
const int val_y = 1;
const int val_SW = 2;

//initializing ultrasonic sensor
const int TRIG = 7;
const int ECHO = 8;

//initializing dcmotor
const int Enable12 = 5; //PWM pin to L239D's En12 (pin 1)
const int Driver1A = 4; //To L239D's 1A (pin 2)
const int Driver2A = 3; //To L239D's 2A (pin 7)

//defining car variables
int mode = 0;

const int neutral = 105; //neutral angle can be tuned to make car go straight
const int range = 25; //ideal range = 35, gives a 15-25cm turning radius, tune accordingly

const int right = neutral + range;
const int left = neutral - range;

float ultrasnoic_sensor;
float distance;

int buttonState;
int previousButtonState;

int speed = 255; //120 is the lowest tested PWM for going straight, use 255-255 for going straight
bool obstacle = false;


Servo servo;

void setup() {
  Serial.begin(9600);

  //servo
  servo.attach(my_servo);
  
  //ultrasonic sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  //joystick
  pinMode(val_SW, INPUT_PULLUP);
  
  //dcmotor
  pinMode(Enable12, OUTPUT);
  pinMode(Driver1A, OUTPUT);
  pinMode(Driver2A, OUTPUT);

  servo.write(neutral);
  
}

void loop() {

 //Task1 (mode 0)

 while (mode ==0)
 {
    Serial.println("Mode 0 initiated");

    if (analogRead(val_y) > 450 && analogRead(val_y) < 550)
      {
        servo.write(neutral);
      } 
    else if (analogRead(val_y) > 900)
      {
        servo.write(left-20); //turn left
      }
    else if (analogRead(val_y) > 900)
      {
        servo.write(right+20); //turn right
      }

    if (analogRead(val_x) < 100)
      {
        motorCtrl (255, false);
      }
    else if (analogRead(val_y) > 900)
      {
        motorCtrl (255, true);
      }
    else
      {
        motorCtrl (0, false);
      }

 }
  //Task2 (mode 1)

 while (mode ==1)
 {
    servo.write(neutral);
    motorCtrl (0, false);
    
    Serial.println("Mode 1 initiated");
    delay(2000);

    motorCtrl (255, false);
    delay(1000);

    servo.write(right);
    delay(1000);

    servo.write(left);
    delay(1000);

    servo.write(neutral);
    delay(50);
    motorCtrl (0,false); 
 }
  //Task 3 (mode 2)
 if (mode == 2)
 {
    Serial.println("Mode 2 initiated");

    distance = getDistance();
    servo.write(neutral);
    motorCtrl (255,false);
    delay((distance+38-18)/0.16);
    motorCtrl (0, false);
    delay(2000);

    servo.write(right+20);
    motorCtrl (255, false);
    delay(1200);

    servo.write(neutral);
    delay(450);

    servo.write(left-20);
    delay(800);

    servo.write(right+20);
    delay(500);

    servo.write(neutral);
    motorCtrl (0, false);
    delay(1000);

    motorCtrl(255, false);
    distance = 168 - (distance+40);
    delay((distance+38)/0.16);
    motorCtrl(0, false);
 }   
             }
void motorCtrl(byte speed, bool dir){
  
  //speed range 0-255
  analogWrite(Enable12,speed);  // PWM
  
  if (dir){
    digitalWrite(Driver1A,LOW);   // Boolean
    digitalWrite(Driver2A,HIGH);   // Boolean 
  } else {
    digitalWrite(Driver1A,HIGH);   // Boolean
    digitalWrite(Driver2A,LOW);   // Boolean 
  }
}

float getDistance()
{
    float pulse = 0;
    int sample = 10;
    for (int i=0; i<sample; i++)
    {
        digitalWrite(TRIG,LOW);
        delay(5);
        digitalWrite(TRIG,HIGH);
        delay(10);
        digitalWrite(TRIG, LOW);
        pulse += pulseIn(ECHO, HIGH);
        delay(10);
    }
    return (pulse/sample)*0.01848484288 + 0.421;
}
