
#include <ESP32Servo.h>

// create servo object
Servo servo1;



int servo1Pin = 27;

int pos = 0;      // position in degrees

void setup()
{
	Serial.begin(115200);
	servo1.attach(servo1Pin);
}

void loop()
{
	for (pos = 0; pos <= 180; pos += 1) // sweep from 0 degrees to 180 degrees
	{
		servo1.write(pos);
		delay(20);             // waits 20ms for the servo to reach the position
	}

}

