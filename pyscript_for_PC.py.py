import serial
import time

# Set the COM port and baud rate
COM_PORT = 'COM7'  # Replace 'x' with the actual COM port assigned to your Arduino
BAUD_RATE = 9600

# Open the serial port
ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)

# Function to calculate CRC16
def calculate_crc16(data):
    crc = 0xFFFF

    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1

    return crc & 0xFFFF

try:
    # Read string from the file
    # with open('data.txt', 'r') as file:
        input_string = input("enter the string:")

        # Calculate CRC16 for the string
        crc_value = calculate_crc16(input_string.encode('utf-8'))

        # Append CRC to the string
        input_string += f"{crc_value:04X}"

        # Transmit the string with CRC to Arduino
        ser.write(input_string.encode())

        # Wait for a short time to allow Arduino to process the message
        time.sleep(0.1)

        # Read and print the response from Arduino
        response = ser.readline().decode().strip()
        print(f"Received from Arduino: {response}")

except KeyboardInterrupt:
    # Close the serial port when the program is interrupted
    ser.close()
    print("Serial port closed.")
