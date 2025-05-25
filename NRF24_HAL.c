#include "NRF24_HAL.h"


static uint8_t nrf24_dummy_tx = 0xFF;
static uint8_t nrf24_dummy_rx;


void NRF24_SPI_TxRx(uint8_t *tx, uint8_t *rx, uint16_t len)
{

    uint8_t *ptx = tx ? tx : &nrf24_dummy_tx;
    uint8_t *prx = rx ? rx : &nrf24_dummy_rx;

    HAL_SPI_TransmitReceive(NRF24_SPI_HANDLE, ptx, prx, len, 1000);
}


void NRF24_DelayUs(uint32_t us)
{
    uint32_t start = HAL_GetTick();
    uint32_t wait_ms = (us + 999) / 1000;
    while ((HAL_GetTick() - start) < wait_ms) { __NOP(); }
}
