/*
 Name:		Clock_Speed.ino
 Created:	7/19/2021 3:53:25 AM
 Author:	rehsd
*/

#include<Wire.h>

unsigned int pulses = 0;
unsigned long lastMicros = 0;
volatile float currentClockSpeed = 0.0;


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(2, INPUT);	//clock
	attachInterrupt(digitalPinToInterrupt(2), onClock, RISING);
	Wire.begin();
}

// the loop function runs over and over again until power down or reset
void loop() {
	Wire.beginTransmission(9);
	//Serial.println((int)currentClockSpeed);
	int cClock = (int)currentClockSpeed;
	Serial.println(cClock);
	byte myArray[2];
	myArray[0] = (cClock >> 8) & 0xFF;
	myArray[1] = cClock & 0xFF;

	Wire.write(myArray, 2);
	Wire.endTransmission();
	delay(1000);
}

void onClock() {
	//Serial.println("clock...");
	pulses++;
	unsigned long currentMicros = micros();
	float timeLapsed = (currentMicros - lastMicros) / 1000000.0;
	//Serial.println(timeLapsed);
	if (timeLapsed > .01)
	{
		currentClockSpeed = pulses / timeLapsed;
		//Serial.println(currentClockSpeed);
		pulses = 0;
		lastMicros = currentMicros;
	}
}