/*************************************************************************
* Title:    I2C master library using hardware TWI interface
* Author:   Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
* File:     $Id: twimaster.c,v 1.3 2005/07/02 11:14:21 Peter Exp $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device with hardware TWI 
* Usage:    API compatible with I2C Software Library i2cmaster.h
**************************************************************************/
#include QMK_KEYBOARD_H
#include <inttypes.h>
#include <compat/twi.h>

#include "i2cmaster.h"


/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* I2C clock in Hz */
#define SCL_CLOCK  100000L


/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void i2c_init(void)
{
  /* initialize TWI clock
   * minimal values in Bit Rate Register (TWBR) and minimal Prescaler
   * bits in the TWI Status Register should give us maximal possible
   * I2C bus speed - about 444 kHz
   *
   * for more details, see 20.5.2 in ATmega16/32 secification
   */
  print("Waiting before i2c init\n");

  _delay_ms(10000);
  print("Doing i2c init\n");

  // Enable pull-up resistors on PD0 + PD1
  DDRD |= 0x3;
  PORTD ^= ~0x3;

  print("Sleeping with SDA/SCL pulled low");
  _delay_ms(10000);
  
  TWCR = 1<<TWEN;

  TWSR = 0;
  TWBR = (F_CPU/SCL_CLOCK - 16) / 2;

  xprintf("Done i2c init (%b)\n", TWCR);

}/* i2c_init */

/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return 0 = device accessible, 1= failed to access device
*************************************************************************/
unsigned char i2c_start(unsigned char address)
{
    uint8_t   twst;

    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    print("begin i2c and wait\n");
    xprintf("TWCR: %08b\n", TWCR);
    xprintf("TWSR: %08b\n", TWSR);
	// send START condition
	// wait until transmission completed
	while(!(TWCR & (1<<TWINT))) {
    xprintf("TWCR: %08b\n", TWCR);
    xprintf("TWSR: %08b\n", TWSR);
    _delay_ms(100);
  }

    print("done waiting\n");
	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

    print("begin tx address and wait\n");
	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

    print("done waiting\n");
	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;

}/* i2c_start */


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 
 Input:   address and transfer direction of I2C device
*************************************************************************/
void i2c_start_wait(unsigned char address)
{
    /* uint8_t   twst; */


    /* while ( 1 ) */
    /* { */
	    /* // send START condition */
	    /* TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); */
    
    /* 	// wait until transmission completed */
    /* 	while(!(TWCR & (1<<TWINT))); */
    
    /* 	// check value of TWI Status Register. Mask prescaler bits. */
    /* 	twst = TW_STATUS & 0xF8; */
    /* 	if ( (twst != TW_START) && (twst != TW_REP_START)) continue; */
    
    /* 	// send device address */
    /* 	TWDR = address; */
    /* 	TWCR = (1<<TWINT) | (1<<TWEN); */
    
    /* 	// wail until transmission completed */
    /* 	while(!(TWCR & (1<<TWINT))); */
    
    /* 	// check value of TWI Status Register. Mask prescaler bits. */
    /* 	twst = TW_STATUS & 0xF8; */
    /* 	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) */ 
    /* 	{ */    	    
    /* 	    /1* device busy, send stop condition to terminate write operation *1/ */
	        /* TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); */
	        
	        /* // wait until stop condition is executed and bus released */
	        /* while(TWCR & (1<<TWSTO)); */
	        
    /* 	    continue; */
    /* 	} */
    /* 	//if( twst != TW_MT_SLA_ACK) return 1; */
    /* 	break; */
    /*  } */

}/* i2c_start_wait */


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction 

 Input:   address and transfer direction of I2C device
 
 Return:  0 device accessible
          1 failed to access device
*************************************************************************/
unsigned char i2c_rep_start(unsigned char address)
{
    return i2c_start( address );

}/* i2c_rep_start */


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
void i2c_stop(void)
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}/* i2c_stop */


/*************************************************************************
  Send one byte to I2C device
  
  Input:    byte to be transfered
  Return:   0 write successful 
            1 write failed
*************************************************************************/
unsigned char i2c_write( unsigned char data )
{	
    uint8_t   twst;
    
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}/* i2c_write */


/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;

}/* i2c_readAck */


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;

}/* i2c_readNak */
