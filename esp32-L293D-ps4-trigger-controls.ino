#include <Arduino.h>
#include <PS4Controller.h>

// Pins (your setup)
const int enablePinL = 25; // PWM pin -> L293D Pin 1 (Enable)
const int in1PinL    = 32; // L293D Pin 2
const int in2PinL    = 33; // L293D Pin 7

const int enablePinR = 18; // PWM pin -> L293D Pin 9 (Enable)
const int in3PinR    = 19; // L293D Pin 10
const int in4PinR    = 21; // L293D Pin 15

// ps4 buttons 


// PWM params
const uint32_t freq = 1000;    // 1 kHz
const uint8_t  resolution = 8; // bits (0..255)
const uint32_t maxDuty = (1UL << resolution) - 1; // 255 for 8-bit

void setup() {
  Serial.begin(9600);

  // motor left
  pinMode(in1PinL, OUTPUT);
  pinMode(in2PinL, OUTPUT);

  // motor right
  pinMode(in3PinR, OUTPUT);
  pinMode(in4PinR, OUTPUT);

  // ps controller

  PS4.begin("AC:FD:93:1F:E3:B0");  // Replace with your ESP32's MAC address
  Serial.println("Ready to connect PS4 controller");


  // Set left motor direction: forward
  digitalWrite(in1PinL, LOW);
  digitalWrite(in2PinL, HIGH);

  // Set left motor direction: forward
  digitalWrite(in3PinR, LOW);
  digitalWrite(in4PinR, HIGH);

  // Attach PWM to pin (new API). Returns true on success.
  if (!ledcAttach(enablePinL, freq, resolution)) {
    Serial.println("ledcAttach() failed!");
    while (1) delay(1000);
  }

  if (!ledcAttach(enablePinR, freq, resolution)) {
    Serial.println("ledcAttach() failed!");
    while (1) delay(1000);
  }
}





void loop() 
{
  if (PS4.isConnected()) 
  {

    int LstickX = PS4.LStickX();
    int l2 = PS4.L2Value();  // 0–255
    int r2 = PS4.R2Value();  // 0–255    

    bool boost = PS4.Cross();
    bool reverse = PS4.Circle();
  

  



    //Serial.printf("R2=%d L2=%d LstickX=%d boost=%d\n", r2, l2, LstickX, boost);



    // convert to deal with not enough current to spin motor
    // map(value, fromLow, fromHigh, toLow, toHigh)
    
    int pwm_forward  = map(r2, 0, 255, 40, 127);
    int pwm_backward = map(l2, 0, 255, 40, 127);
    
    
    int pwm_steer = LstickX;
    
    // delete small gitter 
    if(abs(LstickX) < 10)
      pwm_steer = 0;

    if (r2 < 15)
      pwm_forward = 0;

    if (l2 < 15) 
      pwm_backward = 0;


    
  

    spin(pwm_forward, pwm_backward, pwm_steer/2, boost);


    
    delay(50);
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





















































