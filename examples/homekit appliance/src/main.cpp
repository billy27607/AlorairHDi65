/*
	This is a homekit appliance which allos Apple home to control an Alorair HDi65 dehumidifier that
	I bought to reduce the humidity in a vacation home. It is based on the wonderful library HomeSpan
	and I have it running on a xiao esp32c3. The humidifier does all the work of trying to hold to a target
	humidity. this is just a homekit control for it.

*/
#include <AlorairHDi65.h>
#include "HomeSpan.h"

// instantiate the HDi65

AlorairHDi65 alorair(D7);

// Dehumidifier structure

struct HumidifierDehumidifier : Service::HumidifierDehumidifier
{

	// Create characteristics, set initial values, and set storage in NVS to true

	Characteristic::Active active{0, true};
	Characteristic::CurrentRelativeHumidity humidity{70, true};
	Characteristic::CurrentHumidifierDehumidifierState currentState{0, true};
	Characteristic::TargetHumidifierDehumidifierState targetState{0, true};
	Characteristic::RelativeHumidityHumidifierThreshold humidThreshold{40, true};
	Characteristic::RelativeHumidityDehumidifierThreshold dehumidThreshold{80, true};

	HumidifierDehumidifier() : Service::HumidifierDehumidifier()
	{
		targetState.setValidValues(1, 2); // on allow Dehumidifier State
	}

	boolean update() override
	{

		if (active.updated())
		{
			alorair.setPower(active.getNewVal());
		}

		if (dehumidThreshold.updated() && dehumidThreshold.getNewVal<float>() != dehumidThreshold.getVal<float>())
			alorair.setTargetHumidity(dehumidThreshold.getNewVal());

		return (true);
	}

	void loop() override
	{
		//  if the current state has not been update in the last 5 seconds we want to get fresh data from
		//  the dehumidifier
		if (currentState.timeVal() > 5000)
		{
			alorair.status();
			if (alorair.currentStatus == true)
				currentState.setVal(3);
			else
				currentState.setVal(1);
		}

		if (humidity.timeVal() > 5000 && fabs(humidity.getVal<float>() - alorair.currentHumidity) > 0.25)
		{ // if it's been more than 5 seconds since last update, and humidity has changed
			humidity.setVal(alorair.currentHumidity);
		}

		if (active.getVal() == 0)
		{ // power is OFF
			if (currentState.getVal() != 0)
			{																// if current state is NOT Inactive
				Serial.printf("Humidifier/DeHumidifier State: INACTIVE\n"); // set to Inactive
				currentState.setVal(0);
			}
			return; // return since there is nothing more to check when device if OFF
		}
	}
};

// temperature sensor structure
struct TempSensor : Service::TemperatureSensor
{
	SpanCharacteristic *temp;

	TempSensor() : Service::TemperatureSensor()
	{
		temp = new Characteristic::CurrentTemperature(0);
		temp->setRange(0, 110);
	}

	void loop() override 
	{
		temp->setVal(alorair.currentTemperature);
	}
};

// humidity sensor structure
struct HumiditySensor : Service::HumiditySensor
{
	SpanCharacteristic *humidity;

	HumiditySensor() : Service::HumiditySensor()
	{
		humidity = new Characteristic::CurrentRelativeHumidity(0);
		humidity->setRange(0, 100);
	}

	void loop() override 
	{
		humidity->setVal(alorair.currentHumidity);
	}
};

////////////////////////////////////////////////////////////////////////

void setup()
{

	Serial.begin(115200);

	homeSpan.enableOTA();
	homeSpan.begin(Category::Humidifiers, "HomeSpan Humidifier");

	String mac = WiFi.macAddress();

	new SpanAccessory();
	new Service::AccessoryInformation();
		new Characteristic::Identify();
		new Characteristic::Name("AlorAirHDi65");
		new Characteristic::FirmwareRevision("0.9.0");
		new Characteristic::Manufacturer("AlorAir");
		new Characteristic::SerialNumber(mac);


	new HumidifierDehumidifier();
	new TempSensor();
	new HumiditySensor();

	 if (alorair.begin())
	 	{
			LOG1("Can connected...");
		}
		else 
		{
			LOG1("Can connect failed...");
		}
}

////////////////////////////////////////////////////////////////////////

void loop()
{
	homeSpan.poll();
}

////////////////////////////////////////////////////////////////////////