#ifndef NRF24_CORE_H_
#define NRF24_CORE_H_


#include <stdint.h>
#include "NRF24_HAL.h"
#include "NRF24_DEF.h"


void nrf24_WriteReg(uint8_t reg, uint8_t value);
void nrf24_WriteRegMulti(uint8_t reg, const uint8_t *buf, uint8_t len);
uint8_t nrf24_ReadReg(uint8_t reg);
void nrf24_ReadRegMulti(uint8_t reg, uint8_t *buf, uint8_t len);
void nrf24_SendCmd(uint8_t cmd);
void nrf24_Reset(uint8_t reg);

#endif /* NRF24_CORE_H_ */
