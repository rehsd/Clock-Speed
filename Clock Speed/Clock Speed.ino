/*
 Name:		Clock_Speed.ino
 Updated:	23 July 2021
 Author:	rehsd
*/

#include<Wire.h>

volatile unsigned int pulses = 0;
volatile unsigned long lastMicros = 0;
volatile float currentClockSpeed = 0.0;


// ******* OLED for clock display *******************************************************************
// Screen used: I2C SSD1306, https://www.amazon.com/gp/product/B079BN2J8V
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
#define OLED_RESET     -1 //4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// **************************************************************************************************

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(2, INPUT);	//clock
	attachInterrupt(digitalPinToInterrupt(2), onClock, RISING);
	Wire.setClock(10000);
	Wire.setWireTimeout(50000, false);
	Wire.setTimeout(50000);
	Wire.begin();
	Serial.begin(115200);

	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;); // Don't proceed, loop forever
	}
	// Clear the buffer
	display.clearDisplay();
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
	
	display.clearDisplay();
	display.setTextSize(4);						// Normal 1:1 pixel scale
	display.setTextColor(SSD1306_WHITE);        // Draw white text
	display.setCursor(0, 0);					// Start at top-left corner
	display.print(currentClockSpeed);
	display.display();

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