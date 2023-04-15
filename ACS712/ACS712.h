#ifndef _ACS712_H_
#define _ACS712_H_

#include "../LibraryConfig.h"
#ifdef CONFIG_USE_ACS712

#include <stdio.h>
#include <stdint.h>

#define ADC_SCALE 1023.0
#define VREF 5.0
#define DEFAULT_FREQUENCY 50

#define zero pACS->zero
#define sensitivity pACS->sensitivity
#define pin pACS->pin
#define ADC_READ(X) adc1_get_raw(X)
enum ACS712_type {ACS712_05B, ACS712_20A, ACS712_30A};

typedef struct ACS712_t{
	int zero;
	float sensitivity;
	uint8_t pin;
	uint32_t raw_value;
}ACS712_t;


/**
 * @brief Init ACS712
 * @param ACS712 pointer pointed to object of ACS712
 * @param type enum type: ACS712_05B, ACS712_20A, ACS712_30A
 */

void ACS712_init(ACS712, enum ACS712_type type);
void ACS712_setZeroPoint(acs712_t *pACS, int _zero);
void ACS712_setSensitivity(acs712_t *pACS, float sens);
float ACS712_getCurrentDC(acs712_t *pACS);
float ACS712_getCurrentAC(acs712_t *pACS, uint16_t frequency);

#endif
#endif