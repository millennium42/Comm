#ifndef NRF24_HAL_H_
#define NRF24_HAL_H_


#include "stm32f4xx_hal.h"
#include <stdint.h>


#define NRF24_CE_PORT   GPIOB
#define NRF24_CE_PIN    GPIO_PIN_1

#define NRF24_CSN_PORT  GPIOB
#define NRF24_CSN_PIN   GPIO_PIN_0

extern SPI_HandleTypeDef hspi1;
#define NRF24_SPI_HANDLE   (&hspi1)

static inline void NRF24_CE_High (void) { HAL_GPIO_WritePin(NRF24_CE_PORT,  NRF24_CE_PIN,  GPIO_PIN_SET);   }
static inline void NRF24_CE_Low  (void) { HAL_GPIO_WritePin(NRF24_CE_PORT,  NRF24_CE_PIN,  GPIO_PIN_RESET); }

static inline void NRF24_CSN_High(void) { HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);   }
static inline void NRF24_CSN_Low (void) { HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET); }


void NRF24_SPI_TxRx(uint8_t *tx, uint8_t *rx, uint16_t len);
void NRF24_DelayUs(uint32_t us);

#endif /* NRF24_HAL_H_ */

