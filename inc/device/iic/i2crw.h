#ifndef _I2CRW_H
#define _I2CRW_H
#define HARDWARE_I2C_PRESCALER_1  0
#define HARDWARE_I2C_PRESCALER_4  1
#define HARDWARE_I2C_PRESCALER_16 2
#define HARDWARE_I2C_PRESCALER_64 3
//! @}

//! @name I2C READ and WRITE BITS
//! @{
//! Eighth bit (LSB) of I2C address indicates a "read" or "write".
//! (The first seven bits are the 7-bit I2C address.)
#define I2C_READ_BIT    0x01
#define I2C_WRITE_BIT   0x00
//! @}

//! @name STATUS BITS
//! @{
#define STATUS_START               0x08
#define STATUS_REPEATED_START      0x10
#define STATUS_ADDRESS_WRITE_ACK   0x18
#define STATUS_ADDRESS_WRITE_NACK  0x20
#define STATUS_WRITE_ACK           0x28
#define STATUS_WRITE_NACK          0x30
#define STATUS_ARBITRATION_LOST    0x38
#define STATUS_ADDRESS_READ_ACK    0x40
#define STATUS_ADDRESS_READ_NACK   0x48
#define STATUS_READ_ACK            0x50
#define STATUS_READ_NACK           0x58
//! @}

//! @name TIMEOUT AND DELAY IN US
//! @{
#define HW_I2C_DELAY  1
#define HW_I2C_TIMEOUT  20000
//! @}

//! @name ACK OR NACK PARAMETER PASSED TO I2C_READ
//! @{
#define WITH_ACK  0  //!<  Use with i2c_read(WITH_ACK) to read with an acknowledge
#define WITH_NACK 1  //!<  Use with i2c_read(WITH_NACK) to read without an acknowledge.  Normally used after the last byte of a multi-byte read.
//! @}

//! @name OPTIONAL I2C Address MACRO
//! @{
#define I2C_8ADDR(address) (address >> 1)  //!< Use to convert an 8-bit I2C address to 7 bits.
//! @}




/*!
 LTC2943: Multicell Battery Gas Gauge with Temperature, Voltage and Current Measurement

@verbatim

The LTC®2943 measures battery charge state, battery voltage,
battery current and its own temperature in portable
product applications. The wide input voltage range allows
use with multicell batteries up to 20V. A precision coulomb
counter integrates current through a sense resistor between
the battery’s positive terminal and the load or charger.
Voltage, current and temperature are measured with an
internal 14-bit No Latency ΔΣ™ ADC. The measurements
are stored in internal registers accessible via the onboard
I2C/SMBus Interface.

I2C DATA FORMAT (MSB FIRST):

Data Out:
Byte #1                                    Byte #2                     Byte #3

START  SA6 SA5 SA4 SA3 SA2 SA1 SA0 W SACK  C7  C6 C5 C4 C3 C2 C1 C0 SACK D7 D6 D5 D4 D3 D2 D1 D0 SACK  STOP

Data In:
Byte #1                                    Byte #2                                    Byte #3

START  SA6 SA5 SA4 SA3 SA2 SA1 SA0 W SACK  C7  C6  C5 C4 C3 C2 C1 C0 SACK  Repeat Start SA6 SA5 SA4 SA3 SA2 SA1 SA0 R SACK

Byte #4                                   Byte #5
MSB                                       LSB
D15 D14  D13  D12  D11  D10  D9 D8 MACK   D7 D6 D5 D4 D3  D2  D1  D0  MNACK  STOP

START       : I2C Start
Repeat Start: I2c Repeat Start
STOP        : I2C Stop
SAx         : I2C Address
SACK        : I2C Slave Generated Acknowledge (Active Low)
MACK        : I2C Master Generated Acknowledge (Active Low)
MNACK       : I2c Master Generated Not Acknowledge
W           : I2C Write (0)
R           : I2C Read  (1)
Cx          : Command Code
Dx          : Data Bits
X           : Don't care



Example Code:

Read charge, current, and voltage.

    adc_command = LTC2943_SENSE_MONITOR | LTC2943_AUTOMATIC_MODE; // Builds commands to set LTC2943 to automatic mode
    ack |= LTC2943_write(LTC2943_I2C_ADDRESS, LTC2943_CONTROL_REG, adc_command);   // Sets the LTC2943 to automatic mode

    resistor = .1; // Resistor Value On Demo Board

    ack |= LTC2943_read_16_bits(LTC2943_I2C_ADDRESS, LTC2943_CHARGE_MSB_REG, &charge_code);  // Reads the ADC registers that contains charge value
    charge = LTC2943_code_to_coulombs(charge_code, resistor, prescalarValue); // Calculates charge from charge code, resistor and prescalar

    ack |= LTC2943_read_16_bits(LTC2943_I2C_ADDRESS, LTC2943_CURRENT_MSB_REG, &current_code); // Reads the voltage code across sense resistor
    current = LTC2943_code_to_current(current_code, resistor); // Calculates current from current code, resistor value.

    ack |= LTC2943_read_16_bits(LTC2943_I2C_ADDRESS, LTC2943_VOLTAGE_MSB_REG, &voltage_code);   // Reads voltage voltage code
    VIN = LTC2943_VIN_code_to_voltage(voltage_code);  // Calculates VIN voltage from VIN code and lsb


@endverbatim
*/

/*! @file
    @ingroup LTC2943
    Header for LTC2943: Multicell Battery Gas Gauge with Temperature, Voltage and Current Measurement
*/




//#include <Wire.h>


/*!
| LTC2943 I2C Address Assignment    | Value |
| :-------------------------------- | :---: |
| LTC2943_I2C_ADDRESS               | 0xC8  |
*/

/*! @name LTC2943 I2C Address
@{ */

#define LTC2943_I2C_ADDRESS 0x64
#define LTC2943_I2C_ALERT_RESPONSE  0x0C
//! @}


/*!
| Name                                              | Value |
| :------------------------------------------------ | :---: |
| LTC2943_STATUS_REG                                | 0x00  |
| LTC2943_CONTROL_REG                               | 0x01  |
| LTC2943_ACCUM_CHARGE_MSB_REG                      | 0x02  |
| LTC2943_ACCUM_CHARGE_LSB_REG                      | 0x03  |
| LTC2943_CHARGE_THRESH_HIGH_MSB_REG                | 0x04  |
| LTC2943_CHARGE_THRESH_HIGH_LSB_REG                | 0x05  |
| LTC2943_CHARGE_THRESH_LOW_MSB_REG                 | 0x06  |
| LTC2943_CHARGE_THRESH_LOW_LSB_REG                 | 0x07  |
| LTC2943_VOLTAGE_MSB_REG                           | 0x08  |
| LTC2943_VOLTAGE_LSB_REG                           | 0x09  |
| LTC2943_VOLTAGE_THRESH_HIGH_MSB_REG               | 0x0A  |
| LTC2943_VOLTAGE_THRESH_HIGH_LSB_REG               | 0x0B  |
| LTC2943_VOLTAGE_THRESH_LOW_MSB_REG                | 0x0C  |
| LTC2943_VOLTAGE_THRESH_LOW_LSB_REG                | 0x0D  |
| LTC2943_CURRENT_MSB_REG                           | 0x0E  |
| LTC2943_CURRENT_LSB_REG                           | 0x0F  |
| LTC2943_CURRENT_THRESH_HIGH_MSB_REG               | 0x10  |
| LTC2943_CURRENT_THRESH_HIGH_LSB_REG               | 0x11  |
| LTC2943_CURRENT_THRESH_LOW_MSB_REG                | 0x12  |
| LTC2943_CURRENT_THRESH_LOW_LSB_REG                | 0x13  |
| LTC2943_TEMPERATURE_MSB_REG                       | 0x14  |
| LTC2943_TEMPERATURE_LSB_REG                       | 0x15  |
| LTC2943_TEMPERATURE_THRESH_HIGH_REG               | 0x16  |
| LTC2943_TEMPERATURE_THRESH_LOW_REG                | 0x17  |
*/

/*! @name Registers
@{ */
// Registers
#define LTC2943_STATUS_REG                          0x00
#define LTC2943_CONTROL_REG                         0x01
#define LTC2943_ACCUM_CHARGE_MSB_REG                0x02
#define LTC2943_ACCUM_CHARGE_LSB_REG                0x03
#define LTC2943_CHARGE_THRESH_HIGH_MSB_REG          0x04
#define LTC2943_CHARGE_THRESH_HIGH_LSB_REG          0x05
#define LTC2943_CHARGE_THRESH_LOW_MSB_REG           0x06
#define LTC2943_CHARGE_THRESH_LOW_LSB_REG           0x07
#define LTC2943_VOLTAGE_MSB_REG                     0x08
#define LTC2943_VOLTAGE_LSB_REG                     0x09
#define LTC2943_VOLTAGE_THRESH_HIGH_MSB_REG         0x0A
#define LTC2943_VOLTAGE_THRESH_HIGH_LSB_REG         0x0B
#define LTC2943_VOLTAGE_THRESH_LOW_MSB_REG          0x0C
#define LTC2943_VOLTAGE_THRESH_LOW_LSB_REG          0x0D
#define LTC2943_CURRENT_MSB_REG                     0x0E
#define LTC2943_CURRENT_LSB_REG                     0x0F
#define LTC2943_CURRENT_THRESH_HIGH_MSB_REG         0x10
#define LTC2943_CURRENT_THRESH_HIGH_LSB_REG         0x11
#define LTC2943_CURRENT_THRESH_LOW_MSB_REG          0x12
#define LTC2943_CURRENT_THRESH_LOW_LSB_REG          0x13
#define LTC2943_TEMPERATURE_MSB_REG                 0x14
#define LTC2943_TEMPERATURE_LSB_REG                 0x15
#define LTC2943_TEMPERATURE_THRESH_HIGH_REG         0x16
#define LTC2943_TEMPERATURE_THRESH_LOW_REG          0x17
//! @}

/*!
| Command Codes                                 | Value     |
| :-------------------------------------------- | :-------: |
| LTC2943_AUTOMATIC_MODE                        | 0xC0      |
| LTC2943_SCAN_MODE                             | 0x80      |
| LTC2943_MANUAL_MODE                           | 0x40      |
| LTC2943_SLEEP_MODE                            | 0x00      |
| LTC2943_PRESCALAR_M_1                         | 0x00      |
| LTC2943_PRESCALAR_M_4                         | 0x08      |
| LTC2943_PRESCALAR_M_16                        | 0x10      |
| LTC2943_PRESCALAR_M_64                        | 0x18      |
| LTC2943_PRESCALAR_M_256                       | 0x20      |
| LTC2943_PRESCALAR_M_1024                      | 0x28      |
| LTC2943_PRESCALAR_M_4096                      | 0x30      |
| LTC2943_PRESCALAR_M_4096_2                    | 0x31      |
| LTC2943_ALERT_MODE                            | 0x04      |
| LTC2943_CHARGE_COMPLETE_MODE                  | 0x02      |
| LTC2943_DISABLE_ALCC_PIN                      | 0x00      |
| LTC2943_SHUTDOWN_MODE                         | 0x01      |
*/

/*! @name Command Codes
@{ */
// Command Codes
#define LTC2943_AUTOMATIC_MODE                  0xC0
#define LTC2943_SCAN_MODE                       0x80
#define LTC2943_MANUAL_MODE                     0x40
#define LTC2943_SLEEP_MODE                      0x00

#define LTC2943_PRESCALAR_M_1                   0x00
#define LTC2943_PRESCALAR_M_4                   0x08
#define LTC2943_PRESCALAR_M_16                  0x10
#define LTC2943_PRESCALAR_M_64                  0x18
#define LTC2943_PRESCALAR_M_256                 0x20
#define LTC2943_PRESCALAR_M_1024                0x28
#define LTC2943_PRESCALAR_M_4096                0x30
#define LTC2943_PRESCALAR_M_4096_2              0x31

#define LTC2943_ALERT_MODE                      0x04
#define LTC2943_CHARGE_COMPLETE_MODE            0x02

#define LTC2943_DISABLE_ALCC_PIN                0x00
#define LTC2943_SHUTDOWN_MODE                   0x01

extern int smbus_write_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length);
extern int smbus_read_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length);
extern int bcm2835_I2C_Init(void);
extern int set_addr(unsigned char addr);
#endif