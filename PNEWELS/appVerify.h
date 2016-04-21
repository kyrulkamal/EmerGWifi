/*
 * appVerify.h
 *
 * Created: 12/2/2015 09:05:44
 *  Author: Zulhilmi
 */ 


#ifndef APPVERIFY_H_
#define APPVERIFY_H_

uint8_t signatureVerify;

uint8_t pneVerify_received(uint8_t *data, uint8_t size);
uint16_t pneVerify_send(uint8_t *data, uint8_t size);

#endif /* APPVERIFY_H_ */