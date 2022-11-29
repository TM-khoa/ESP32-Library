#ifndef _ACS712_H_
#define _ACS712_H_

#include <stdio.h>
#include <stdint.h>

#define ADC_SCALE 1023.0
#define VREF 5.0
#define DEFAULT_FREQUENCY 50

enum ACS712_type {ACS712_05B, ACS712_20A, ACS712_30A};

struct acs712_t{
	enum ACS712_type type;
	int zero;
	float sensitivity;
	uint8_t pin;
};
typedef struct acs712_t acs712_t;
#define ACS712 acs712_t *pACS


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




// class ACS712 {
// public:
	// ACS712(ACtS712_type type, uint8_t _pin);
// 	int calibrate();
// 	void setZeroPoint(int _zero);
// 	void setSensitivity(float sens);
// 	float getCurrentDC();
// 	float getCurrentAC(uint16_t frequency = 50);

// private:
// 	int zero = 512;
// 	float sensitivity;
// 	uint8_t pin;
// };