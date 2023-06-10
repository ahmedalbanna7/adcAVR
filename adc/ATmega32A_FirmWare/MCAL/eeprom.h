/*
 * eeprom.h
 *
 * Created: 5/3/2020 9:12:35 PM
 *  Author: ahmed
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


#include <avr/eeprom.h>
#include <util/delay.h>

  void eeprom_write ( uint16_t address, uint8_t byte );
  uint8_t eeprom_read( uint16_t address );


#endif /* EEPROM_H_ */