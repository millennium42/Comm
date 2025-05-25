#ifndef NRF24_H_
#define NRF24_H_

#include <stdint.h>


#define NRF24_MAX_PAYLOAD     32U


void    NRF24_Init            (uint8_t payload_len);
void    NRF24_TxMode          (const uint8_t *addr, uint8_t channel);
void    NRF24_RxMode          (const uint8_t *addr, uint8_t channel);

uint8_t NRF24_Transmit        (const uint8_t *data, uint8_t len);
uint8_t NRF24_IsDataAvailable (void);
void    NRF24_Receive         (uint8_t *buf);

void    NRF24_ReadAllRegs     (uint8_t *dst38);

#endif /* NRF24_H_ */
