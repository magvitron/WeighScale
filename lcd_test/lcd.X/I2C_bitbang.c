

#define DELAY_MACR 5
#include <xc.h>
#include "I2C_bitbang.h"
#include "USART.h"
#include "delay.h"

//....................................................................
// This function generates an I2C Start Condition
//....................................................................
void I2CStart(void)
{
unsigned int i;

SDA_TRIS = 1;                   // ensure SDA & SCL are high
SCL = 1;
SDA_TRIS = 0;                   // SDA = output
SDA = 0;                        // pull SDA low
MSdelay(DELAY_MACR);
SCL = 0;                        // pull SCL low
}
//....................................................................
// This function generates an I2C Start Condition
//....................................................................
void I2CRestart(void)
{
unsigned int i;

SDA_TRIS = 1;                   // ensure SDA & SCL are high
SCL = 1;
SDA_TRIS = 0;                   // SDA = output
SDA = 0;                        // pull SDA low
MSdelay(DELAY_MACR);
SCL = 0;                        // pull SCL low
}



//....................................................................
// This function generates an I2C Stop Condition
//....................................................................
void I2CStop(void)
{
unsigned int i;

SCL = 0;                        // ensure SCL is low
SDA_TRIS = 0;                   // SDA = output
SDA = 0;                        // SDA low
MSdelay(DELAY_MACR);
SCL = 1;                        // pull SCL high
SDA_TRIS = 1;                   // allow SDA to be pulled high
MSdelay(DELAY_MACR);
SCL=0;                          // ensure SCL is low
}


//....................................................................
// Outputs a bit to the I2C bus
//....................................................................
void bit_out(unsigned char data)
{
unsigned int i;

SCL = 0;                        // ensure SCL is low
SDA_TRIS=0;                     // configure SDA as an output
SDA= (data>>7);                 // output the MSB
MSdelay(DELAY_MACR);
SCL = 1;                        // pull SCL high to clock bit
MSdelay(DELAY_MACR);
SCL = 0;                        // pull SCL low for next bit
}


//....................................................................
// Inputs a bit from the I2C bus
//....................................................................
unsigned char bit_in(unsigned char data)
{
SCL = 0;                        // ensure SCL is low
SDA_TRIS = 1;                   // configure SDA as an input
SCL = 1;                        // bring SCL high to begin transfer
delay(10);
data |= SDA_IN;                   // input the received bit
SCL = 0;                        // bring SCL low again.
return data;
}


//....................................................................
// Writes a byte to the I2C bus
//....................................................................
unsigned char I2CWrite(unsigned char data)
{
unsigned char i;                // loop counter
unsigned char ack;              // ACK bit

ack = 0;
for (i = 0; i < 8; i++)         // loop through each bit
    {
    bit_out(data);              // output bit
    data = data << 1;           // shift left for next bit
    }

bit_in(&ack);                   // input ACK bit
return ack;
}


//....................................................................
// Reads a byte from the I2C bus
//....................................................................
unsigned char I2CRead(unsigned char ack)
{
unsigned char i;                // loop counter
unsigned char ret=0;            // return value

for (i = 0; i < 8; i++)         // loop through each bit
    {
    ret = ret << 1;             // shift left for next bit
    ret = bit_in(ret);               // input bit
    }

bit_out(ack);                   // output ACK/NAK bit
send_string("I2C read:");
char buff[10];
intToAscci(ret,buff);
send_string(buff);
USART_newline();
return ret;
}


//.............................................................................
//          Polls the bus for ACK from device
//.............................................................................
char I2CAck (unsigned char control)
{
    long timeout=0;
unsigned char result=1;

while(result)
	{
	I2CStart();            // generate Restart condition
	result=I2CRead(control); // send control byte (WRITE command)
    timeout++;
    if(timeout>=10)
    {
        result = 0;
        break;
    }
    }
I2CStop();                     // generate Stop condition
return result;
}

void I2CNotAck()
{
    int i;
SCL = 0;                        // ensure SCL is low
SDA_TRIS=0;                     // configure SDA as an output
SDA= 1;                 // output the MSB
for (i=0;i<5;i++) NOP();
SCL = 1;                        // pull SCL high to clock bit
for (i=0;i<5;i++) NOP();
SCL = 0;                        // pull SCL low for next bit
}

/**
 * Send the address of the slave through the bus.
 * @param address Address of the slave.
 */
void I2CSendAddress(unsigned char Address, unsigned char I2C_Direction)
{
    I2CDeviceSetDeviceAddress(Address);
    if (I2C_Direction == I2C_Direction_Receiver)
        I2CWrite(deviceAddressRead);
    else if (I2C_Direction == I2C_Direction_Transmitter)
        I2CWrite(deviceAddressWrite);
}


/**
 * Sets the device address to the library. Change every time that slave changes.
 * @param address   Address of the slave NOT SHIFTED. DO NOT INCLUDE THE
 *                  READ/WRITE bit.
 */
void I2CDeviceSetDeviceAddress(unsigned char address)
{
    deviceAddressRead = (address << 1) | 0x01;
    deviceAddressWrite = (address << 1) & 0xFE;
}
/**
 * Write multiple bytes to a device register.
 * @param address First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 */
void I2CDeviceWriteBytes(unsigned char address,
                         unsigned char length,
                         unsigned char *data)
{
    unsigned char i;

    I2CStart();
    I2CWrite(deviceAddressWrite);
    I2CWrite(address);

    for (i = 0; i < length; i++)
    {
        I2CWrite(data[i]);
    }
    I2CStop();
}
