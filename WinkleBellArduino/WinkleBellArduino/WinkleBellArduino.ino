#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NeoPixelPin       9
#define NeoPexelInterval  12
#define BellNum           8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel((BellNum*NeoPexelInterval), NeoPixelPin, NEO_GRB + NEO_KHZ800);

//Bell RGB
int *SetledR;
int *SetledG;
int *SetledB;
int *ledR;
int *ledG;
int *ledB;


//Seramic Sensor
int *Sensor;
int *ExSensor;

unsigned long ExMillis;
unsigned long PreMillis;
unsigned long ExSensorMillis;

void CheckSensor();
void ResetSensor();
void SendMessage();
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
	Serial.begin(115200);
}

void loop()
{
	PreMillis = millis();
	CheckSensor();
	SendMessage();
	BrightLED();
	ResetSensor();
}

void CheckSensor()
{
	if (PreMillis - ExSensorMillis >= 20)
	{
		for (int i = 0; i < BellNum; i++)
		{
			int Temp = (int)(analogRead(i));
			if (Temp - ExSensor[i] >15)
			{
				Sensor[i] = Temp - ExSensor[i];
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

void SendMessage()
{

	for (int i = 0; i < BellNum; i++)
	{
		if (Sensor[i] > 0)
		{
			{
				Serial.println(i);
			}
		}
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
