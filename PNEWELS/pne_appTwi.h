/*
 * pne_appTwi.h
 *
 * Created: 2/10/2015 1:53:42 PM
 *  Author: tay
 */ 


#ifndef PNE_APPTWI_H_
#define PNE_APPTWI_H_


//Definition
#define TWI_EXAMPLE  &TWBR


//Variables
uint8_t current_state_emergency_light;
uint16_t memory_address_incr;


//Types
typedef enum
{
	charge_start,
	charge_end,
	discharge_start,
	discharge_end,
	emergency_start,
	emergency_end,
	battery_charged,
	battery_discharged,
	normal
}store_seq_t;

typedef enum
{
	APP_PNEWELS_TWI_WRITE_SUCCESS,
	APP_PNEWELS_TWI_WRITE_FAIL,
	APP_PNEWELS_TWI_READ_SUCCESS,
	APP_PNEWELS_TWI_READ_FAIL
}AppPNEWELS_TWIStatus_t;


//Prototypes
void pne_APP_TwiInit(uint8_t slave_bus_address);
AppPNEWELS_TWIStatus_t pne_APP_TwiSend(uint8_t *slave_mem_addr, uint8_t slave_mem_addr_length, uint8_t slave_bus_address, uint8_t *test_pattern, uint8_t test_pattern_length);
AppPNEWELS_TWIStatus_t pne_APP_TwiReceive(uint8_t *slave_mem_addr, uint8_t slave_mem_addr_length, uint8_t slave_bus_address, uint8_t *data_received, uint8_t data_received_length);
uint8_t PNEWELSTemperatureRead(void);
uint8_t PNEWELSE2promRead(uint8_t address, char in_ex_type, uint8_t memory_loc_1, uint8_t memory_loc_2);
void PNEWELSE2promWrite(uint8_t address, char in_ex_type, uint8_t memory_loc_1, uint8_t memory_loc_2, uint8_t memory_data);
void store(store_seq_t seq);
void clre2prom();
void memoryCounter_init();
void memoryCounter_save(uint16_t log_counter);


#endif /* PNE_APPTWI_H_ */