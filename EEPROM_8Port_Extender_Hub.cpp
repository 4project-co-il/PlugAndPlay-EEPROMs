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

	// Embedded HUB EEPROM programming code for SAMD21 Development board
	PnP_DeviceInfo deviceInfo;
	uint8_t interruptMapping[8*2];		// 8 ports, 2 interrupts per port
	memset(&deviceInfo, 0, sizeof(PnP_DeviceInfo));

	deviceInfo.headerId = 0x506E502A;	// "PnP*"
	deviceInfo.version = 1;
	deviceInfo.deviceIDs[0] = PNP_ID_EXTENDER_HUB;
	deviceInfo.numberOfPorts = 8;
	deviceInfo.numberOfEndpoints = 2;

	deviceInfo.numberOfInterrupts = 1;
	deviceInfo.interrupt1Mode = PnP_InterruptMode::PNP_INTERRUPT_FALLING;
	deviceInfo.interrupt1Endpoint = 1;

	deviceInfo.endpointData[0].endpointId = 0;
	deviceInfo.endpointData[0].i2cAddress = 0x70;		// TCA9548A I2C mux address

	deviceInfo.endpointData[1].endpointId = 1;
	deviceInfo.endpointData[1].i2cAddress = 0x74;		// TCAL9539 I2C 16bit I/O expander

	deviceInfo.paramsLength = 16;

	// Should be sequential for the extender HUB. Processing code relies on that order!
	interruptMapping[0*2 + 0] = 0;
	interruptMapping[0*2 + 1] = 1;
	interruptMapping[1*2 + 0] = 2;
	interruptMapping[1*2 + 1] = 3;
	interruptMapping[2*2 + 0] = 4;
	interruptMapping[2*2 + 1] = 5;
	interruptMapping[3*2 + 0] = 6;
	interruptMapping[3*2 + 1] = 7;
	interruptMapping[4*2 + 0] = 8;
	interruptMapping[4*2 + 1] = 9;
	interruptMapping[5*2 + 0] = 10;
	interruptMapping[5*2 + 1] = 11;
	interruptMapping[6*2 + 0] = 12;
	interruptMapping[6*2 + 1] = 13;
	interruptMapping[7*2 + 0] = 14;
	interruptMapping[7*2 + 1] = 15;

	rc = PnP_PlugAndPlayManager::WriteDeviceEEPROM(0x50 + PnP_PlugAndPlayManager::PNP_EEPROM_EXTENDER_HUB, deviceInfo, interruptMapping, sizeof(interruptMapping));
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
