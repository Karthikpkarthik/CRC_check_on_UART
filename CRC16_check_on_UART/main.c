/*
 * CRC16_check_on_UART.c
 *
 * Created: 12/1/2023 11:40:42 AM
 * Author : karthi
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

// UART initialization function
void initUART() {
	// Set baud rate to 9600
	UBRR0 = F_CPU / (16 * 9600) - 1;
	// Enable transmitter and receiver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// CRC16 calculation function
uint16_t calculateCRC16(const uint8_t *data, size_t length) {
	uint16_t crc = 0xFFFF;

	for (size_t i = 0; i < length; ++i) {
		crc ^= data[i];
		for (int j = 0; j < 8; ++j) {
			crc = (crc & 1) ? ((crc >> 1) ^ 0xA001) : (crc >> 1);
		}
	}

	return crc;
}

// Function to verify and store data in EEPROM
void processReceivedData(const char *receivedString) {
	size_t stringLength = strlen(receivedString);

	if (stringLength < 4) {
		// The string is too short to contain CRC
		return;
	}

	
	char crcString[5];
	strncpy(crcString, receivedString + stringLength - 4, 4);// Extract CRC from the end of the string
	crcString[4] = '\0';

	// Convert CRC string to integer
	uint16_t receivedCRC;
	sscanf(crcString, "%04X", &receivedCRC);

	// Calculate CRC for the remaining part of the string
	uint16_t calculatedCRC = calculateCRC16((const uint8_t *)receivedString, stringLength - 4);

	// Verify CRC
	if (receivedCRC == calculatedCRC) {
		
		int address = 0;
		for (int i = 0; i < stringLength - 4; ++i) 
		{
			eeprom_write_byte((uint8_t *)address++, receivedString[i]);// Store the string and its CRC in EEPROM
		}
		eeprom_write_byte((uint8_t *)address++, '\0'); // Null-terminate the stored string
		eeprom_write_word((uint16_t *)address, receivedCRC);
	}
}

// Transmit stored data back to PC
void transmitStoredData() {
	// Transmit stored data back to PC
	for (int address = 0; ; ) {
		char currentChar = eeprom_read_byte((uint8_t *)address++);
		if (currentChar == '\0') {
			// Reached the end of stored string
			break;
		}
		// Transmit the character over UART 
		while (!(UCSR0A & (1 << UDRE0))); // Wait for the buffer to be empty
		UDR0 = currentChar;
	}
	
	
	while (!(UCSR0A & (1 << UDRE0)));// Transmit a newline character to separate multiple stored strings
	UDR0 = '\n';
}

int main(void) {
	// Initialize UART
	initUART();
	
	// Enable global interrupts if needed
	sei();

	// Main loop
	while (1) {
		                             
		if (UCSR0A & (1 << RXC0)) // Check if there is data available on UART
		{
			
			char receivedMessage[50];// Read the incoming message from Python
			int i = 0;
			while (1) {
				receivedMessage[i++] = UDR0;
				if (receivedMessage[i - 1] == '\n') {
					receivedMessage[i] = '\0';
					break;
				}
			}

			// Process and store data in EEPROM
			processReceivedData(receivedMessage);
		}

		

		// Transmit stored data back to PC
		for (int i = 0; i < 10000; i++) {
			transmitStoredData();
			_delay_ms(100); //  delay between transmissions
		}
	}
}
