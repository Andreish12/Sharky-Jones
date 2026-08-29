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
  PS4.begin("04:33:F7:64:1D:44");  // Replace with your ESP32's MAC address
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
    int l2 = PS4.L2Value();  // 0–255
    int r2 = PS4.R2Value();  // 0–255

    int pwmL = 0;
    int pwmR = 0;

    int LstickY = PS4.LStickY();
    int RstickY = PS4.RStickY();

    bool boost = PS4.R1();
    bool reverse = PS4.Circle();
    


    Serial.printf("pwm_left=%d pwm_right=%d\n", LstickY, RstickY);

    // convert to deal with not enough current to spin motor
    pwmL = map(abs(LstickY),0, 127, 50, 127);
    if (LstickY < 0) 
      pwmL = -pwmL;

    pwmR = map(abs(RstickY),0, 127, 50, 127);
    if (RstickY < 0) 
      pwmR = -pwmR;




    if (abs(LstickY) < 15)
      pwmL = 0;


    if (abs(RstickY) < 15) 
      pwmR = 0;


    
  

    spin(pwmL, pwmR, boost);


    
    delay(50);
  }
}








void spin(int pwm_left, int pwm_right, bool boost)
{
  // left motor
  if (pwm_left >= 0) 
  {
    digitalWrite(in1PinL, LOW);   // forward
    digitalWrite(in2PinL, HIGH);
  } 
  else 
  {
    digitalWrite(in1PinL, HIGH);  // reverse
    digitalWrite(in2PinL, LOW);
  }

  // Right motor
  if (pwm_right >= 0) 
  {
    digitalWrite(in3PinR, LOW);   // forward
    digitalWrite(in4PinR, HIGH);
  } 
  else 
  {
    digitalWrite(in3PinR, HIGH);  // reverse
    digitalWrite(in4PinR, LOW);
  }

  if(boost)
  {
    ledcWrite(enablePinL, pwm_left * 2);
    ledcWrite(enablePinR, pwm_right* 2);
  }
  else
  {
    ledcWrite(enablePinL, abs(pwm_left));
    ledcWrite(enablePinR, abs(pwm_right));
  }

  Serial.printf("pwm_left=%d pwm_right=%d boost=%d reverse=%d\n", pwm_left, pwm_right, boost);
}
