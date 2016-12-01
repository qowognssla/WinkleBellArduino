#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NeoPixelPin       9
#define NeoPexelInterval  12
#define BellNum           8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel((BellNum*NeoPexelInterval), NeoPixelPin, NEO_GRB + NEO_KHZ800);

#define StartPin		  22
#define BeatPin0		  23
#define BeatPin1		  24
#define BeatPin2		  25


//Bell RGB
int *SetledR;
int *SetledG;
int *SetledB;
int *ledR;
int *ledG;
int *ledB;

//Button State
int ExStartBtnState;
int CurrentStartBtnState;

int ExBeatBtnState0;
int CurrentBeatBtnState0;
int ExBeatBtnState1;
int CurrentBeatBtnState1;
int ExBeatBtnState2;
int CurrentBeatBtnState2;


//Seramic Sensor
int *Sensor;
int *ExSensor;

unsigned long ExMillis;
unsigned long PreMillis;
unsigned long ExSensorMillis;
unsigned long ExBtnMillis;

void CheckButtonState();
void CheckSensor();
void ResetSensor();
void BrightLED();

void setup() {

#if defined (__AVR_ATtiny85__)
	if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
	pixels.begin();
	ExMillis = millis();

	ledR = new int[BellNum];
	ledG = new int[BellNum];
	ledB = new int[BellNum];
	SetledR = new int[BellNum];
	SetledG = new int[BellNum];
	SetledB = new int[BellNum];

	Sensor = new int[BellNum];
	ExSensor = new int[BellNum];

	for (int i = 0; i < BellNum; i++)
	{
		ledR[i] = 0;
		ledG[i] = 0;
		ledB[i] = 0;
		SetledR[i] = 0;
		SetledG[i] = 0;
		SetledB[i] = 0;
		Sensor[i] = 0;
		ExSensor[i] = 0;
	}

	//Test
	for (int i = 0; i < BellNum; i++)
	{
		SetledR[i] = 255;
		SetledG[i] = 255;
		SetledB[i] = 255;
	}

	//StartBtn Initialize
	pinMode(StartPin, INPUT);
	int Temp = digitalRead(StartPin);
	ExStartBtnState = Temp;
	CurrentStartBtnState = Temp;

	//BeatBtn Initialize
	pinMode(BeatPin0, INPUT);
	int Temp = digitalRead(BeatPin0);
	ExBeatBtnState0 = Temp;
	CurrentBeatBtnState0 = Temp;
	pinMode(BeatPin1, INPUT);
	int Temp = digitalRead(BeatPin1);
	ExBeatBtnState1 = Temp;
	CurrentBeatBtnState1 = Temp;
	pinMode(BeatPin2, INPUT);
	int Temp = digitalRead(BeatPin2);
	ExBeatBtnState2 = Temp;
	CurrentBeatBtnState2 = Temp;
	
	Serial.begin(115200);
}

void loop()
{
	PreMillis = millis();
	CheckButtonState();
	CheckSensor();
	BrightLED();
	ResetSensor();
}

void CheckButtonState()
{
	if (PreMillis - ExBtnMillis >= 20)
	{
		CurrentStartBtnState = digitalRead(StartPin);
		CurrentBeatBtnState0 = digitalRead(CurrentBeatBtnState0);
		CurrentBeatBtnState1 = digitalRead(CurrentBeatBtnState1);
		CurrentBeatBtnState2 = digitalRead(CurrentBeatBtnState2);
		
		if (CurrentStartBtnState > ExStartBtnState)
			Serial.print("s");
		ExStartBtnState = CurrentStartBtnState;

		if (CurrentBeatBtnState0 > ExBeatBtnState0)
			Serial.print("b0");
		ExBeatBtnState0 = CurrentBeatBtnState0;

		if (CurrentBeatBtnState1 > ExBeatBtnState1)
			Serial.print("b1");
		ExBeatBtnState1 = CurrentBeatBtnState1;

		if (CurrentBeatBtnState2 > ExBeatBtnState2)
			Serial.print("b2");
		ExBeatBtnState2 = CurrentBeatBtnState2;
		ExBtnMillis = PreMillis;
	}
}



void CheckSensor()
{
	if (PreMillis - ExSensorMillis >= 20)
	{
		for (int i = 0; i < BellNum; i++)
		{
			int Temp = (int)(analogRead(i));
			if (Temp - ExSensor[i] > 10)
			{
				Sensor[i] = Temp - ExSensor[i];
				Serial.println(i);
			}
			ExSensor[i] = Temp;
		}
		ExSensorMillis = PreMillis;
	}
}

void ResetSensor()
{
	for (int i = 0; i < BellNum; i++)
	{
		Sensor[i] = 0;
	}
}

void BrightLED()
{

	for (int i = 0; i < BellNum; i++)
	{
		if (Sensor[i] > 0)
		{
			ledR[i] = SetledR[i];
			ledG[i] = SetledG[i];
			ledB[i] = SetledB[i];
		}
	}

	if (PreMillis - ExMillis >= 7)
	{
		for (int i = 0; i < BellNum; i++)
		{
			if (ledR[i] > 0)
				ledR[i] = ledR[i] - 1;
			if (ledG[i] > 0)
				ledG[i] = ledG[i] - 1;
			if (ledB[i] > 0)
				ledB[i] = ledB[i] - 1;
			for (int j = i*NeoPexelInterval; j < (i + 1)*NeoPexelInterval; j++)
			{
				pixels.setPixelColor(j, pixels.Color(ledR[i], ledG[i], ledB[i]));
				pixels.show();
			}
		}
		ExMillis = PreMillis;
	}
}
