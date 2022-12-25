#include "ACS712.h"
#include "esp_adc_library.h"
#include "math.h"

#define zero pACS->zero
#define sensitivity pACS->sensitivity
#define pin pACS->pin

int ACS712_calib(acs712_t *pACS) {
	uint16_t acc = 0;
	for (int i = 0; i < 10; i++) {
		acc += ADC_READ(pin);
	}
	zero = acc / 10;
	return zero;
}

void ACS712_init(acs712_t *pACS, enum ACS712_type type) {
	// Different models of the sensor have their sensitivity:
	zero = 512;
	switch (type) {
		case ACS712_05B:
			sensitivity = 0.185;
			break;
		case ACS712_20A:
			sensitivity = 0.100;
			break;
		case ACS712_30A:
			sensitivity = 0.066;
			break;
	}
}

float ACS712_getCurrentAC(acs712_t *pACS, uint16_t frequency) {
	uint32_t period = 1000000 / frequency;
	uint32_t t_start = micros();

	uint32_t Isum = 0, measurements_count = 0;
	int32_t Inow;

	while (micros() - t_start < period) {
		Inow = ADC_READ(pin) - zero;
		Isum += Inow*Inow;
		measurements_count++;
	}

	float Irms = sqrt(Isum / measurements_count) / ADC_SCALE * VREF / sensitivity;
	return Irms;
}

float ACS712_getCurrentDC(acs712_t *pACS) {
	int16_t acc = 0;
	for (int i = 0; i < 10; i++) {
		acc += ADC_READ(pin) - zero;
	}
	float I = (float)acc / 10.0 / ADC_SCALE * VREF / sensitivity;
	return I;
}

void ACS712_setZeroPoint(acs712_t *pACS, int _zero) {
	zero = _zero;
}

void ACS712_setSensitivity(acs712_t *pACS, float sens) {
	sensitivity = sens;
}