#include <Arduino.h>
#include "EBF.h"
#include "PlugAndPlay.h"
#include "PnP_PlugAndPlayManager.h"

// EBF objects creation, should be global
EBF_Core EBF;
PnP_StatusLed led;

void setup()
{
	uint8_t rc;

	// EBF is the first thing that should be initialized, with the maximum timers to be used
	EBF.Init();

	// Status LED initialization
	led.Init();

	// Device EEPROM programming code
	PnP_DeviceInfo deviceInfo;
	uint16_t pointCurrent[3];		// max current tuning for the RGB colors of the led, value multiplied by 10

	memset(&deviceInfo, 0, sizeof(PnP_DeviceInfo));

	deviceInfo.headerId = 0x506E502A;	// "PnP*"
	deviceInfo.version = 1;
	deviceInfo.deviceIDs[0] = PNP_ID_1_RGB_LED_LP5815;

	deviceInfo.numberOfEndpoints = 1;
	deviceInfo.endpointData[0].endpointId = 1;
	deviceInfo.endpointData[0].i2cAddress = 0x2D;		// LP5815 address

	deviceInfo.numberOfInterrupts = 1;
	deviceInfo.interrupt1Mode = PnP_InterruptMode::PNP_DIGITAL_OUTPUT;

	deviceInfo.paramsLength = sizeof(pointCurrent);

	pointCurrent[0] = (uint16_t)(13.5 * 10);		// RED LED max current
	pointCurrent[1] = (uint16_t)(10 * 10);		// GREEN LED max current
	pointCurrent[2] = (uint16_t)(4.8 * 10);		// BLUE LED max current

	rc = PnP_PlugAndPlayManager::WriteDeviceEEPROM(0x50 + PnP_PlugAndPlayManager::PNP_EEPROM_DEVICE, deviceInfo, (uint8_t*)pointCurrent, sizeof(pointCurrent));
	if (rc == EBF_OK) {
		led.Blink(100, 900);
	} else {
		led.Blink(100, 100);
	}
}

void loop()
{
	// Let EBF to do all the processing
	// Your logic should be done in the callback functions
	EBF.Process();
}
