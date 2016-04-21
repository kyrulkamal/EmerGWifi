/*
 * twi_megarf.h
 *
 * Created: 1/9/2015 5:28:02 PM
 *  Author: tay
 */ 


#ifndef TWI_MEGARF_H_
#define TWI_MEGARF_H_


#include "sysTypes.h"
#include "status_codes.h"
#include "config.h"
#include <stdbool.h>

/**
 * \defgroup group_megarf_drivers_twi TWI - Two-Wire Interface
 *
 * See \ref megarf_twi_quickstart
 *
 * Driver for the Two-Wire Interface (TWI).
 * Provides functions for configuring and using the TWI in both master and
 * slave mode.
 *
 * \section megarf_twi_quickstart_guide Quick start guide
 * See \ref megarf_twi_quickstart
 *
 * \{
 */

/**
 * \brief Input parameters when initializing the twi master mode
 */
typedef struct {
	/* ! The baudrate of the TWI bus. */
	unsigned long speed;
	/* ! The baudrate register value of the TWI bus. */
	uint8_t baud_reg;
	/* ! The desired address. */
	char chip;
} twi_master_options_t;

/**
 * \brief TWI Slave data buffers
 */
typedef struct {
	uint8_t *rx_buffer;
	uint8_t *tx_buffer;
} slave_data_buffer_t;

/**
 * \brief Information concerning the data transmission
 */
typedef struct {
	/* ! TWI chip address to communicate with. */
	char chip;
	/* ! TWI address/commands to issue to the other chip (node). */
	uint8_t addr[3];
	/* ! Length of the TWI data address segment (1-3 bytes). */
	int addr_length;
	/* ! Where to find the data to be written. */
	uint8_t *buffer;
	/* ! How many bytes do we want to write. */
	unsigned int length;
} twi_package_t;

/**
 * \internal
 * \brief internal address size
 */

typedef enum {
	TWI_SLAVE_NO_INTERNAL_ADDRESS,
	TWI_SLAVE_ONE_BYTE_SIZE,
	TWI_SLAVE_TWO_BYTE_SIZE,
	TWI_SLAVE_THREE_BYTE_SIZE
} twiInternalAddrSize_t;

/** \brief TWI status codes. */
enum {
	TWS_BUSERROR                   = 0x00, /*Bus error due to illegal START or STOP
	                                        * condition.*/
	TWS_START                      = 0x08, /*A START condition has been transmitted.*/
	TWS_RSTART                     = 0x10, /*A repeated START condition has been
	                                        *transmitted.*/
	TWS_MT_SLA_ACK                 = 0x18, /*SLA+W has been transmitted; ACK has
	                                        *been received.*/
	TWS_MT_SLA_NACK                = 0x20, /*SLA+W has been transmitted; NOT ACK has
	                                        *been received.*/
	TWS_MT_DATA_ACK                = 0x28, /*Data byte has been transmitted; ACK has
	                                        *been received.*/
	TWS_MT_DATA_NACK               = 0x30, /*Data byte has been transmitted; NOT ACK
	                                        *has been received.*/
	TWS_M_ARB_LOST                 = 0x38, /*Arbitration lost in SLA+W or data bytes
	                                       *(Transmitter); Arbitration lost in SLA+R or
	                                       *NOT ACK bit (Receiver).*/
	TWS_MR_SLA_ACK                 = 0x40, /*SLA+R has been transmitted; ACK has been
	                                       *received.*/
	TWS_MR_SLA_NACK                = 0x48, /*SLA+R has been transmitted; NOT ACK has
	                                       *been received.*/
	TWS_MR_DATA_ACK                = 0x50, /*Data byte has been received; ACK has been
	                                       *returned.*/
	TWS_MR_DATA_NACK               = 0x58, /*Data byte has been received; NOT ACK has
	                                       *been returned.*/
	TWS_ST_SLA_ACK                 = 0xA8, /* ! Own SLA+R has been received;
	                                        * ACK has been returned */
	TWS_ST_SLA_ACK_M_ARB_LOST      = 0xB0, /* ! Arbitration lost in SLA+R/W as Master;
	                                        *own SLA+R has been received;
	                                        *ACK has been returned */
	TWS_ST_DATA_ACK                = 0xB8, /* ! Data byte in TWDR has been transmitted; 
	                                        *ACK has been received */
	TWS_ST_DATA_NACK               = 0xC0, /* ! Data byte in TWDR has been transmitted; 
	                                        * NOT ACK has been received */                                      
	TWS_ST_DATA_ACK_LAST_BYTE      = 0xC8, /* ! Last data byte in TWDR has been transmitted
	                                        * (TWEA = “0”); ACK has been received */	                                       
	TWS_SR_SLA_ACK                 = 0x60, /* ! Own SLA+W has been received 
	                                        *ACK has been returned */
	TWS_SR_SLA_ACK_M_ARB_LOST      = 0x68, /* ! Arbitration lost in  SLA+R/W as Master;
	                                        * own SLA+W has been received; 
	                                        * ACK has been returned */
	TWS_SR_GEN_ACK                 = 0x70, /* ! General call address has been received; 
											ACK has been returned */                                     
	TWS_SR_GEN_ACK_M_ARB_LOST      = 0x78, /* ! Arbitration lost in SLA+R/W as Master; 
	                                        * General call address has been received;
	                                        *  ACK has been returned */                                      
	TWS_SR_SLA_DATA_ACK            = 0x80, /* ! Previously addressed with own SLA+W;
	                                        * data has been received;  ACK
	                                        * has been returned */                                      
	TWS_SR_SLA_DATA_NACK           = 0x88, /* ! Previously addressed with own SLA+W; 
	                                        * data has been received;NOT ACK has been
	                                        * returned */
	TWS_SR_GEN_DATA_ACK            = 0x90, /* ! Previously addressed with general call;
	                                        * data has been received; ACK
	                                        * has been returned */
	TWS_SR_GEN_DATA_NACK           = 0x98, /* ! Previously addressed with general call;
	                                        *  data has been received;
	                                        *  NOT ACK has been returned */	                                       
	TWS_SR_STOP_RESTART            = 0xA0  /* ! A STOP condition or repeated START condition
	                                        *  has been received while still addressed
	                                        *  as Slave */	                                       
};

/**
 * \internal
 * \def TWI_STATE
 * \brief different states of TWI process
 */
/* states of the i2c transaction */
#define TWI_IDLE                     1
#define TWI_WRITE_IADDR_WRITE_DATA   2
#define TWI_WRITE_IADDR_READ_DATA    3
#define TWI_WRITE_DATA               4
#define TWI_READ_DATA                5
#define TWI_TRANSAC_SUCCESS          6
#define TWI_TRANSAC_FAIL             7
#define TWI_PROCESS                  8

/**
 * \def TWI_TWSR_PRESCALE
 * \brief TWI_TWSR_PRESCALE register bit masking for SCL baud Values
 */
#define TWI_TWSR_PRESCALE0      0x00  /* TWI Prescale=1. */
#define TWI_TWSR_PRESCALE1      0x01  /* TWI Prescale=4. */
#define TWI_TWSR_PRESCALE2      0x02  /* TWI Prescale=16. */
#define TWI_TWSR_PRESCALE3      0x03  /* TWI Prescale=64. */

/**
 * \internal
 * \def TWI_PRESCALE_VALUE
 * \brief TWI prescale value from register setting
 */
/** TWI prescale register value */
#define TWI_PRESCALE_REG   TWI_TWSR_PRESCALE0

#if TWI_PRESCALE_REG == TWI_TWSR_PRESCALE0
#define TWI_PRESCALER  1
#elif TWI_PRESCALE_REG == TWI_TWSR_PRESCALE1
#define TWI_PRESCALER  4
#elif TWI_PRESCALE_REG == TWI_TWSR_PRESCALE2
#define TWI_PRESCALER  16
#elif TWI_PRESCALE_REG == TWI_TWSR_PRESCALE3
#define TWI_PRESCALER  64
#endif

/**
 * \internal
 * \def TWI_CLOCK_RATE
 * \brief Baud rate register setting calculation
 */

/** Baud register setting calculation. Formula described in datasheet. */
#define TWI_CLOCK_RATE(F_CPU, \
TWI_SPEED) (((F_CPU / \
TWI_SPEED) - 16) / (2 * TWI_PRESCALER))
	
/**
 * \def TWI_STATUS
 * \brief Define Status of the twi transaction
 */
#define TWI_SUCCESS                  0
#define TWI_STATUS_NO_STATE          1
#define TWI_STATUS_TX_COMPLETE       2
#define TWI_STATUS_RX_COMPLETE       3
#define TWI_STATUS_IO_ERROR         -1
#define TWI_STATUS_PROTOCOL_ERROR   -2

/**
 * \def TWI_TWSR_STATUS_MASK
 * \brief TWSR status mask
 */
#define TWI_TWSR_STATUS_MASK    (TWSR & 0xF8)

/**
 * \def TWI_WRITE_ENABLE
 * \brief TWI Write Enable
 */
#define TWI_WRITE_ENABLE(SDA)   (SDA & 0xFE)

/**
 * \def TWI_READ_ENABLE
 * \brief TWI Read Enable
 */
#define TWI_READ_ENABLE(SDA)    ((SDA & 0xFE) | 0x01)

/**
 * \brief Enables interrupt on TWI.
 *
 * \param none.
 */
static inline void twi_interrupt_enable(void)
{
	TWCR |= (1 << TWIE);
}

/**
 * \brief Disables interrupt on TWI.
 *
 * \param none.
 */
static inline void twi_interrupt_disable(void)
{
	TWCR &= (~(1 << TWIE));
}

/**
 * \brief Resets the TWI.
 *
 * \param none.
 */
static inline void twi_reset(void)
{
	TWCR = ((1 << TWSTO) | (1 << TWINT));
}

/**
 * \brief reads the data
 * \param none.
 * \return byte that was read from the TWI.
 */
static inline uint8_t twi_read_byte(void)
{
	return TWDR;
}

/**
 * \brief writes the data
 * \param data - an byte for sending.
 */
static inline void twi_write_byte(uint8_t data)
{
	TWDR =  data;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

/**
 * \brief Enables the ACK/NACK to send for the received data
 */
static inline void twi_send_ack(bool ack)
{
	if (ack) {
		TWCR |= (1 << TWEA);
	} else {
		TWCR &= ~(1 << TWEA);
	}

	TWCR |= ((1 << TWINT) | (1 << TWIE) | (1 << TWEN)); /* Trigger the TWI */
}

/**
 * \brief Directs TWI to send stop condition.
 */
static inline void twi_send_stop(void)
{
	TWCR = ((1 << TWSTO) | (1 << TWINT) | (1 << TWEN));
}

/**
 * \brief Directs the TWI to send start condition.
 */
static inline void twi_send_start(void)
{
	TWCR = ((1 << TWSTA) | (1 << TWINT) | (1 << TWEN) | (1 << TWIE));
}

/**
 * \brief Perform a TWI master write transfer.
 *
 * This function is a TWI Master write transaction.
 *
 * \param package -  Package information and data
 *                  (see \ref twi_package_t)
 */
status_code_t twi_master_write(volatile void *twi,const twi_package_t *package);

/**
 * \brief Reads the series of bytes from the TWI bus
 * \param package -  Package information and data
 *                  (see \ref twi_package_t)
 */
status_code_t twi_master_read(volatile void *twi,const twi_package_t *package);

/**
 * \brief returns the status of TWI bus
 *
 * \param none
 */
status_code_t twi_master_get_status(void);

/**
 * \brief Inits TWI module as master
 *
 * This function is a TWI Master initialisation.
 *
 * \param opt twi setting options
 *                  (see \ref twi_master_options_t)
 */
status_code_t twi_master_init(volatile void *twi, twi_master_options_t *opt);

/**
 * \brief Get the Status of TWI transceiver
 *
 * \return int - status information
 */
int twi_slave_status_get(void);

/**
 * \brief Get the state of TWI transceiver
 *
 * \return int - state information
 */
int twi_slave_state_get(void);

/**
 * \brief Initialize TWI as Slave
 *
 * \param TWI_ownAddress - contains Slave own Address
 */
status_code_t twi_slave_init(uint8_t twi_slave_ownadd);

/**
 * \brief Start the slave Transceiver
 *
 * \return status_code_t - status of twi slave
 */
status_code_t twi_slave_start(slave_data_buffer_t *package);

/**
 * \brief Resets the slave state and status to initial for next
 * transmission/reception
 */
void twi_slave_status_reset(void);


#endif /* TWI_MEGARF_H_ */