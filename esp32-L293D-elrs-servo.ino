#include <AlfredoCRSF.h>
#include <HardwareSerial.h>
#include <Arduino.h>
#include <ESP32Servo.h>



#define PIN_RX 16
#define PIN_TX 17



// Set up a new Serial object
HardwareSerial crsfSerial(1);
AlfredoCRSF crsf;


// Pins (your setup)
const int enablePinL = 25; // PWM pin -> L293D Pin 1 (Enable)
const int in1PinL    = 32; // L293D Pin 2
const int in2PinL    = 33; // L293D Pin 7

const int enablePinR = 18; // PWM pin -> L293D Pin 9 (Enable)
const int in3PinR    = 19; // L293D Pin 10
const int in4PinR    = 21; // L293D Pin 15

//servo pin
static const int servoPin = 27;
Servo servo1;

// PWM params
const uint32_t freq = 1000;    // 1 kHz
const uint8_t  resolution = 8; // bits (0..255)
const uint32_t maxDuty = (1UL << resolution) - 1; // 255 for 8-bit



void setup()
{
  Serial.begin(115200);
  
  crsfSerial.begin(CRSF_BAUDRATE, SERIAL_8N1, PIN_RX, PIN_TX);
  if (!crsfSerial) while (1) Serial.println("Invalid crsfSerial configuration");

  crsf.begin(crsfSerial);


  // motor left
  pinMode(in1PinL, OUTPUT);
  pinMode(in2PinL, OUTPUT);

  // motor right
  pinMode(in3PinR, OUTPUT);
  pinMode(in4PinR, OUTPUT);


  // Set left motor direction: forward
  digitalWrite(in1PinL, LOW);
  digitalWrite(in2PinL, HIGH);

  // Set left motor direction: forward
  digitalWrite(in3PinR, LOW);
  digitalWrite(in4PinR, HIGH);

 // Attach PWM to pin (new API). Returns true on success.
  if (!ledcAttachChannel(enablePinL, freq, resolution, 8)) {
  Serial.println("ledcAttachChannel L failed!");
  while (1) delay(1000);
}

if (!ledcAttachChannel(enablePinR, freq, resolution, 9)) {
  Serial.println("ledcAttachChannel R failed!");
  while (1) delay(1000);
}

  servo1.attach(servoPin);


}

void loop()
{
  // raw values from remote
  int move = 0;
  int steering = 0;
  int servo = 0;

  // cleaned values 
  int pwm_forward = 0;
  int pwm_backward = 0;
  int pwm_steer = 0;

  bool boost = 0;

  // servo
  int servo_pos = 0;

  // Must call crsf.update() in loop() to process data
  crsf.update();

  move = crsf.getChannel(2);
  steering = crsf.getChannel(1);

  servo_pos = map(crsf.getChannel(11), 1000, 2000, 0, 180);
  Serial.println(servo_pos);
  if(move > 1510)
    pwm_forward = map(move, 1511, 2011, 60, 170);
  
  if(move < 1490)
    pwm_backward = map(move, 1489, 988, 60, 170);

  if(steering > 1510)
    pwm_steer = map(steering, 1511, 2011, 0, 120);
  
  if(steering < 1485)
    pwm_steer = map(steering, 1484, 990,  0, -120);

  if(crsf.getChannel(8) > 1500)
    boost = 1;


  if(crsf.isLinkUp())
  {
    servo1.write(servo_pos);
    spin(pwm_forward, pwm_backward, pwm_steer, boost);
  }
}





void spin(int pwm_forward, int pwm_backward, int pwm_steer, bool boost)
{
  int pwm = 0;
  int pwm_steer_right = 0;
  int pwm_steer_left  = 0;
  if (pwm_forward > pwm_backward) 
  {
    pwm = pwm_forward;
    digitalWrite(in1PinL, LOW);   // forward
    digitalWrite(in2PinL, HIGH);

    digitalWrite(in3PinR, LOW);   // forward
    digitalWrite(in4PinR, HIGH);
  } 
  else 
  {
    pwm = pwm_backward;
    digitalWrite(in1PinL, HIGH);  // reverse
    digitalWrite(in2PinL, LOW);

    digitalWrite(in3PinR, HIGH);  // reverse
    digitalWrite(in4PinR, LOW);
  }

  if(boost)
    pwm *= 2;
 
  if(pwm !=0)
  {
    pwm_steer_right = pwm - pwm_steer;
    pwm_steer_left  = pwm + pwm_steer;
  }



  if(pwm_steer > 0)
  {
    ledcWrite(enablePinR, pwm_steer_right);
    ledcWrite(enablePinL, pwm);
  }
  else
  {
    ledcWrite(enablePinR, pwm);
    ledcWrite(enablePinL, pwm_steer_left);
  }

  //Serial.printf("pwm=%d pwm_forward=%d pwm_backward=%d pwm_steer=%d boost=%d\n", pwm, pwm_forward, pwm_backward, pwm_steer, boost);
  Serial.printf("pwm=%d pwm_steer_right=%d pwm_steer_left=%d pwm_steer=%d\n", pwm, pwm_steer_right, pwm_steer_left, pwm_steer);
}






//Use crsf.getChannel(x) to get us channel values (1-16).
void printChannels()
{
  for (int ChannelNum = 1; ChannelNum <= 12; ChannelNum++)
  {
    Serial.print(crsf.getChannel(ChannelNum));
    Serial.print(", ");
  }
  Serial.println(" ");
}
