#ifndef NRF24_DEFS_H_
#define NRF24_DEFS_H_

/* =========================================================================
 *  1. Endereços de registradores nRF24L01+
 * -------------------------------------------------------------------------*/
#define CONFIG        0x00
#define EN_AA         0x01
#define EN_RXADDR     0x02
#define SETUP_AW      0x03
#define SETUP_RETR    0x04
#define RF_CH         0x05
#define RF_SETUP      0x06
#define STATUS        0x07
#define OBSERVE_TX    0x08
#define CD            0x09

#define RX_ADDR_P0    0x0A
#define RX_ADDR_P1    0x0B
#define RX_ADDR_P2    0x0C
#define RX_ADDR_P3    0x0D
#define RX_ADDR_P4    0x0E
#define RX_ADDR_P5    0x0F
#define TX_ADDR       0x10

#define RX_PW_P0      0x11
#define RX_PW_P1      0x12
#define RX_PW_P2      0x13
#define RX_PW_P3      0x14
#define RX_PW_P4      0x15
#define RX_PW_P5      0x16
#define FIFO_STATUS   0x17

#define DYNPD         0x1C
#define FEATURE       0x1D

/* =========================================================================
 *  2. Comandos SPI
 * -------------------------------------------------------------------------*/
#define R_REGISTER            0x00        /* | reg(4:0) */
#define W_REGISTER            0x20        /* | reg(4:0) */
#define REGISTER_MASK         0x1F

#define R_RX_PAYLOAD          0x61
#define W_TX_PAYLOAD          0xA0
#define FLUSH_TX              0xE1
#define FLUSH_RX              0xE2
#define REUSE_TX_PL           0xE3
#define R_RX_PL_WID           0x60
#define W_ACK_PAYLOAD         0xA8        /* | pipe(2:0) */
#define W_TX_PAYLOAD_NOACK    0xB0
#define NOP_CMD               0xFF

/* =========================================================================
 *  3. Bits úteis (máscaras)
 * -------------------------------------------------------------------------*/
/* CONFIG */
#define CONFIG_MASK_RX_DR   (1u << 6)
#define CONFIG_MASK_TX_DS   (1u << 5)
#define CONFIG_MASK_MAX_RT  (1u << 4)
#define CONFIG_EN_CRC       (1u << 3)
#define CONFIG_CRCO         (1u << 2)
#define CONFIG_PWR_UP       (1u << 1)
#define CONFIG_PRIM_RX      (1u << 0)

/* STATUS */
#define STATUS_RX_DR        (1u << 6)
#define STATUS_TX_DS        (1u << 5)
#define STATUS_MAX_RT       (1u << 4)
#define STATUS_RX_P_NO_MASK (7u << 1)
#define STATUS_TX_FULL      (1u << 0)

/* FIFO_STATUS */
#define FIFO_TX_REUSE       (1u << 6)
#define FIFO_TX_FULL        (1u << 5)
#define FIFO_TX_EMPTY       (1u << 4)
#define FIFO_RX_FULL        (1u << 1)
#define FIFO_RX_EMPTY       (1u << 0)

#endif /* NRF24_DEFS_H_ */
