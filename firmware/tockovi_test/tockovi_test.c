#define DEVICE_ADDR 0xE0

// Software I2C connections
sbit Soft_I2C_Sda           at RB2_bit;
sbit Soft_I2C_Scl           at RB3_bit;
sbit Soft_I2C_Sda_Direction at TRISB2_bit;
sbit Soft_I2C_Scl_Direction at TRISB3_bit;

/* Function that addresses device, then desired register, and puts some value in it */
void I2C_Send_Message(unsigned short dev_addr, unsigned short reg_addr, unsigned short data_value)
{
     Soft_I2C_Start();
     Soft_I2C_Write(dev_addr);
     Soft_I2C_Write(reg_addr);
     Soft_I2C_Write(data_value);
     Soft_I2C_Stop();
}
void main() {

     /* Set all analog pins as digital, set port B pins as output and initialize to zero */
     AD1PCFGL = 0xFFFF;
     LATB = 0;
     TRISB = 0;

     /* Software I2C initializiton */
     Soft_I2C_Init();
     Delay_ms(100);

     /* Set MODE1 register of PCA9633
      * AIx                   - Auto-Increment disabled
      * SLEEP bit set to 0    - Normal Mode
      * SUBx bit set to 0     - PCA does not respond to subadresses
      * ALLCALL bit set to 1  - PCA9633 responds to ALLCALL address (0xE0)
     */
     I2C_Send_Message(DEVICE_ADDR, 0x00, 0x01);


     /* Set LEDOUT register of PCA9633
      * LEDx output state control - all set to 0x10 which results in 0xAA value put into LEDOUT register
     */
     I2C_Send_Message(DEVICE_ADDR, 0xE8, 0xAA);

     Delay_ms(100);

     /* Set PWMx (x = 0, 1, 2, 3) registers to values according to datasheet in order for vehice to go forward*/
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 255);

     while(1);
}