/*
 Name:		Clock_Speed.ino
 Created:	7/19/2021 3:53:25 AM
 Author:	rehsd
*/

#include<Wire.h>

volatile unsigned int pulses = 0;
volatile unsigned long lastMicros = 0;
volatile float currentClockSpeed = 0.0;


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(2, INPUT);	//clock
	attachInterrupt(digitalPinToInterrupt(2), onClock, RISING);
	Wire.setClock(10000);
	Wire.setWireTimeout(50000, false);
	Wire.setTimeout(50000);
	Wire.begin();
	Serial.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Serial.println((int)currentClockSpeed);
	int cClock = (int)currentClockSpeed;
	Serial.print(cClock);
	Serial.print(".");
	byte myArray[2];
	myArray[0] = (cClock >> 8) & 0xFF;
	myArray[1] = cClock & 0xFF;
	
	
	Wire.beginTransmission(9);
	Wire.write(myArray, 2);
	Wire.endTransmission();
	
	//increase loop count for additional delay between clock info sends
	for (int i =0; i < 4; i++)
	{
		delay(500);
		Serial.print(".");
	}
}

void onClock() {
	//Serial.println("clock...");
	pulses++;
	unsigned long currentMicros = micros();
	float timeLapsed = (currentMicros - lastMicros) / 1000000.0;
	//Serial.println(timeLapsed);
	if (timeLapsed > .1)
	{
		currentClockSpeed = pulses / timeLapsed;
		//Serial.println(currentClockSpeed);
		pulses = 0;
		lastMicros = currentMicros;
	}
}