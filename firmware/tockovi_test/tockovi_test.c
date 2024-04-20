/** @file tockovi_test.c
 *
 * @brief Fajl sa izvornim C kodom koji testira pokretanje autonomnog vozila.
 * VAZNO: Sav kod detaljno je dokumentovan u fajlu auto_car.c !
 */

#define DEVICE_ADDR 0xE0

sbit Soft_I2C_Sda           at RB2_bit;
sbit Soft_I2C_Scl           at RB3_bit;
sbit Soft_I2C_Sda_Direction at TRISB2_bit;
sbit Soft_I2C_Scl_Direction at TRISB3_bit;

void I2C_Send_Message(unsigned short dev_addr, unsigned short reg_addr, unsigned short data_value)
{
    Soft_I2C_Start();
    Soft_I2C_Write(dev_addr);
    Soft_I2C_Write(reg_addr);
    Soft_I2C_Write(data_value);
    Soft_I2C_Stop();
}

void main() 
{	
    AD1PCFGL = 0xFFFF;
    LATB = 0;
    TRISB = 0;

    Soft_I2C_Init();
    Delay_ms(100);

    I2C_Send_Message(DEVICE_ADDR, 0x00, 0x01);
    I2C_Send_Message(DEVICE_ADDR, 0xE8, 0xAA);

    Delay_ms(100);

    I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x03, 255);
    I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x05, 255);

    while(1);
}