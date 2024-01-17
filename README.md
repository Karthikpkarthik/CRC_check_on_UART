

---


# CRC16 Check on UART for ATmega328P

This repository contains an AVR C code implementation for CRC16 checking on UART communication using an ATmega328P microcontroller. The code is designed to interact with a Python script over UART, validate CRC16 checksums, store valid data in EEPROM, and transmit stored data back to the PC.

## Features

- **UART Communication:** Initialize UART communication with a baud rate of 9600, enabling both transmitter and receiver.
- **CRC16 Calculation:** Implement CRC16-CCITT calculation to ensure data integrity during communication.
- **EEPROM Storage:** Store valid data along with its CRC16 checksum in the EEPROM of the ATmega328P.
- **Data Transmission:** Transmit stored data back to the PC over UART with a newline character separating multiple strings.
- **Delay Functionality:** Add a delay between data transmissions for better communication reliability.

## How to Use

1. **Arduino Setup:** Load the provided code onto your ATmega328P microcontroller using Microchip Studio or any compatible IDE.
2. **Python Communication:** Write a Python script to communicate with the ATmega328P over UART. Ensure the CRC16 checksum is calculated and appended to the data before transmission.
3. **Data Verification:** The ATmega328P will receive the data, validate the CRC16 checksum, and store valid data in the EEPROM.
4. **Data Retrieval:** The microcontroller will continuously transmit stored data back to the PC at regular intervals.

## Dependencies

- **avr/io.h:** AVR standard input/output functions.
- **avr/eeprom.h:** AVR EEPROM functions for data storage.
- **avr/pgmspace.h:** AVR program space utilities.
- **util/delay.h:** AVR delay functions.
- **string.h:** Standard C string manipulation functions.
- **stdio.h:** Standard input/output functions.

.

---

