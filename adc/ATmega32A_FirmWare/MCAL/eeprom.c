/*
 * eeprom.c
 *
 * Created: 5/3/2020 9:12:17 PM
 *  Author: ahmed
 */ 

 #include <eeprom.h>

  void eeprom_write ( uint16_t address, uint8_t byte ) {  //write a byte to the AVR EEPROM
	  while (EECR &  (1<<EEWE)) ;                 //wait until any write operation is complete
	  EEAR = address;                            //set the desired address
	  EEDR = byte;                               //set the desired byte
	  EECR |= (1<<EEMWE);
	  /* Start eeprom write by setting EEWE */
	  EECR |= (1<<EEWE);                    //start the write operation
  }

  uint8_t eeprom_read( uint16_t address ) 
  {     //read a byte from the AVR EEPROM
	  while(EECR & (1<<EEWE));	  EEAR = address;                            //set the desired address
	  EECR |= ( 1 << EERE );                     //trigger a read operation
	  return EEDR;                               //return the data
  }