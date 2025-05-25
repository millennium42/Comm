#include "NRF24_CORE.h"
#include <string.h>   /* memcpy */


static inline uint8_t _W(uint8_t reg) { return (uint8_t)(reg | 0x20u); }


void nrf24_WriteReg(uint8_t reg, uint8_t value)
{
    uint8_t tx[2] = { _W(reg), value };
    NRF24_CSN_Low();
    NRF24_SPI_TxRx(tx, NULL, 2);
    NRF24_CSN_High();
}

void nrf24_WriteRegMulti(uint8_t reg, const uint8_t *buf, uint8_t len)
{
    uint8_t cmd = _W(reg);
    NRF24_CSN_Low();
    NRF24_SPI_TxRx(&cmd, NULL, 1);
    NRF24_SPI_TxRx((uint8_t *)buf, NULL, len);
    NRF24_CSN_High();
}

uint8_t nrf24_ReadReg(uint8_t reg)
{
    uint8_t rx;
    NRF24_CSN_Low();
    NRF24_SPI_TxRx(&reg, NULL, 1);
    NRF24_SPI_TxRx(NULL, &rx, 1);
    NRF24_CSN_High();
    return rx;
}

void nrf24_ReadRegMulti(uint8_t reg, uint8_t *buf, uint8_t len)
{
    NRF24_CSN_Low();
    NRF24_SPI_TxRx(&reg, NULL, 1);
    NRF24_SPI_TxRx(NULL, buf, len);
    NRF24_CSN_High();
}

void nrf24_SendCmd(uint8_t cmd)
{
    NRF24_CSN_Low();
    NRF24_SPI_TxRx(&cmd, NULL, 1);
    NRF24_CSN_High();
}

void nrf24_Reset(uint8_t reg)
{
    if (reg == STATUS)
    {
        /* limpa RX_DR, TX_DS, MAX_RT */
        nrf24_WriteReg(STATUS, (1u << 6) | (1u << 5) | (1u << 4));
    }
    else if (reg == FIFO_STATUS)
    {
        /* FIFO_STATUS padrão */
        nrf24_WriteReg(FIFO_STATUS, 0x11);
    }
    else
    {

        nrf24_WriteReg(CONFIG,       0x08);
        nrf24_WriteReg(EN_AA,        0x3F);
        nrf24_WriteReg(EN_RXADDR,    0x03);
        nrf24_WriteReg(SETUP_AW,     0x03);
        nrf24_WriteReg(SETUP_RETR,   0x03);
        nrf24_WriteReg(RF_CH,        0x02);
        nrf24_WriteReg(RF_SETUP,     0x0E);
        nrf24_WriteReg(STATUS,       0x00);
        nrf24_WriteReg(OBSERVE_TX,   0x00);
        nrf24_WriteReg(CD,           0x00);

        const uint8_t addr_p0[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
        const uint8_t addr_p1[5] = {0xC2,0xC2,0xC2,0xC2,0xC2};
        const uint8_t tx_addr[5]  = {0xE7,0xE7,0xE7,0xE7,0xE7};

        nrf24_WriteRegMulti(RX_ADDR_P0, addr_p0, 5);
        nrf24_WriteRegMulti(RX_ADDR_P1, addr_p1, 5);
        nrf24_WriteReg(      RX_ADDR_P2, 0xC3);
        nrf24_WriteReg(      RX_ADDR_P3, 0xC4);
        nrf24_WriteReg(      RX_ADDR_P4, 0xC5);
        nrf24_WriteReg(      RX_ADDR_P5, 0xC6);
        nrf24_WriteRegMulti(TX_ADDR,     tx_addr, 5);

        for (uint8_t p = RX_PW_P0; p <= RX_PW_P5; ++p)
            nrf24_WriteReg(p, 0x00);

        nrf24_WriteReg(FIFO_STATUS, 0x11);
        nrf24_WriteReg(DYNPD,       0x00);
        nrf24_WriteReg(FEATURE,     0x00);
    }
}
