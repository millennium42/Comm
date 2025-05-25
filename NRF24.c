#include "NRF24.h"
#include "NRF24_CORE.h"
#include "NRF24_HAL.h"

#include <string.h>

static uint8_t g_payload_len = NRF24_MAX_PAYLOAD;


static void _write_payload(const uint8_t *buf, uint8_t len)
{
    NRF24_CSN_Low();
    uint8_t cmd = W_TX_PAYLOAD;
    NRF24_SPI_TxRx(&cmd, NULL, 1);
    NRF24_SPI_TxRx((uint8_t*)buf, NULL, len);
    NRF24_CSN_High();
}

static void _read_payload(uint8_t *buf, uint8_t len)
{
    NRF24_CSN_Low();
    uint8_t cmd = R_RX_PAYLOAD;
    NRF24_SPI_TxRx(&cmd, NULL, 1);
    NRF24_SPI_TxRx(NULL, buf, len);
    NRF24_CSN_High();
}


void NRF24_Init(uint8_t payload_len)
{
    if (payload_len == 0 || payload_len > NRF24_MAX_PAYLOAD)
        payload_len = NRF24_MAX_PAYLOAD;

    g_payload_len = payload_len;

    NRF24_CE_Low();
    nrf24_Reset(0);

    /* CRC 16-bit + PowerDown */
    nrf24_WriteReg(CONFIG, CONFIG_EN_CRC | CONFIG_CRCO);

    /* Auto-ACK em todos os pipes                      */
    nrf24_WriteReg(EN_AA, 0x3F);

    /* 250 kbps, 0 dBm                                  */
    nrf24_WriteReg(RF_SETUP, 0x26);

    /* 15 retransmissões, 4000 µs                       */
    nrf24_WriteReg(SETUP_RETR, 0x4F);

    /* Payload dinâmico OFF – usamos tamanho fixo       */
    nrf24_WriteReg(FEATURE, 0x00);
    nrf24_WriteReg(DYNPD,   0x00);

    /* Set payload length em todos os pipes             */
    for (uint8_t p = RX_PW_P0; p <= RX_PW_P5; ++p)
        nrf24_WriteReg(p, g_payload_len);

    NRF24_CE_High();
}


void NRF24_TxMode(const uint8_t *addr, uint8_t ch)
{
    NRF24_CE_Low();

    nrf24_WriteReg(RF_CH, ch);
    nrf24_WriteRegMulti(TX_ADDR,     addr, 5);
    nrf24_WriteRegMulti(RX_ADDR_P0,  addr, 5);

    uint8_t cfg = nrf24_ReadReg(CONFIG);
    cfg |= CONFIG_PWR_UP;
    cfg &= ~CONFIG_PRIM_RX;
    nrf24_WriteReg(CONFIG, cfg);

    NRF24_DelayUs(1500);
    NRF24_CE_High();
}


void NRF24_RxMode(const uint8_t *addr, uint8_t ch)
{
    NRF24_CE_Low();

    nrf24_WriteReg(RF_CH, ch);
    nrf24_WriteRegMulti(RX_ADDR_P0, addr, 5);
    nrf24_WriteReg(EN_RXADDR, 0x01);

    uint8_t cfg = nrf24_ReadReg(CONFIG);
    cfg |=  CONFIG_PWR_UP | CONFIG_PRIM_RX;
    nrf24_WriteReg(CONFIG, cfg);
    nrf24_SendCmd(FLUSH_RX);
    nrf24_WriteReg(STATUS, STATUS_RX_DR | STATUS_MAX_RT | STATUS_TX_DS);

    NRF24_DelayUs(150);
    NRF24_CE_High();
}


uint8_t NRF24_Transmit(const uint8_t *data, uint8_t len)
{

    uint8_t buf[NRF24_MAX_PAYLOAD] = {0};
    if (len > g_payload_len) len = g_payload_len;
    memcpy(buf, data, len);

    _write_payload(buf, g_payload_len);

    uint32_t tick = HAL_GetTick();
    uint8_t  status;
    do {
        status = nrf24_ReadReg(STATUS);
        if (status & (STATUS_TX_DS | STATUS_MAX_RT)) break;
    } while ((HAL_GetTick() - tick) < 10);


    nrf24_WriteReg(STATUS, STATUS_TX_DS | STATUS_MAX_RT);


    if (status & STATUS_TX_DS)  return 1;
    if (status & STATUS_MAX_RT) { nrf24_SendCmd(FLUSH_TX); }
    return 0;
}


uint8_t NRF24_IsDataAvailable(void)
{
    uint8_t status = nrf24_ReadReg(STATUS);
    if (status & STATUS_RX_DR)
    {
        uint8_t pipe = (status & STATUS_RX_P_NO_MASK) >> 1;
        nrf24_WriteReg(STATUS, STATUS_RX_DR);
        return pipe;
    }
    return 0xFF;
}


void NRF24_Receive(uint8_t *buf)
{
    _read_payload(buf, g_payload_len);
    nrf24_SendCmd(FLUSH_RX);
}


void NRF24_ReadAllRegs(uint8_t *dst)
{
    for (uint8_t r = 0; r <= 0x09; ++r)
        dst[r] = nrf24_ReadReg(r);

    nrf24_ReadRegMulti(RX_ADDR_P0, dst+10, 5);
    nrf24_ReadRegMulti(RX_ADDR_P1, dst+15, 5);

    dst[20] = nrf24_ReadReg(RX_ADDR_P2);
    dst[21] = nrf24_ReadReg(RX_ADDR_P3);
    dst[22] = nrf24_ReadReg(RX_ADDR_P4);
    dst[23] = nrf24_ReadReg(RX_ADDR_P5);

    nrf24_ReadRegMulti(TX_ADDR, dst+24, 5);

    for (uint8_t i = 29; i < 38; ++i)
        dst[i] = nrf24_ReadReg(i - 12);
}
