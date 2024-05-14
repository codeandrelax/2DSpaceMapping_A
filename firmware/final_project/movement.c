/** @file movement.c
 * 
 * @brief Fajl sa izvornim kodom koji implementira funkcije za kontrolu
 * kretanja autonomnog vozila.
*/

#include "movement.h"

/**
 * @brief Funkcija koja salje podatak PWM kontroleru preko I2C protokola
 * @param[in] reg_addr Adresa registra uredjaja u koji se podatak upisuje
 * @param[in] data_value Jednobajtni podatak koji se salje
 */
void I2C_Send_Message(unsigned short reg_addr, unsigned short data_value)
{
  I2C1_Start();
  I2C1_Write(DEVICE_ADDRESS);
  I2C1_Write(reg_addr);
  I2C1_Write(data_value);
  I2C1_Stop();
}

/**
 * @brief Funkcija koja inicijalizuje PWM kontroler za I2C
 * komunikaciju.
 */
void init_driving()
{
  I2C_Send_Message(0x00, 0x01);
  I2C_Send_Message(0xE8, 0xAA);
}

/**
 * Upis vrijednosti u registre 0x02 - 0x05 upravlja vrijednostima
 * PWM kanala. Vrijednosti u tim registrima podesavaju pravac
 * kretanja autonomnog vozila.
 */
 
/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi kretanje vozila unaprijed.
 */
void drive_forward()
{
  I2C_Send_Message(0x02, PWM_MIN_VAL);
  I2C_Send_Message(0x03, PWM_FORWARD_VAL);
  I2C_Send_Message(0x04, PWM_MIN_VAL);
  I2C_Send_Message(0x05, PWM_FORWARD_VAL);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi blago skretanje vozila ulijevo.
 */
void drive_soft_left()
{
  I2C_Send_Message(0x02, PWM_MIN_VAL);
  I2C_Send_Message(0x03, PWM_MIN_VAL);
  I2C_Send_Message(0x04, PWM_MIN_VAL);
  I2C_Send_Message(0x05, PWM_MAX_VAL - PWM_TURN_OFFSET);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi blago skretanje vozila udesno.
 */
void drive_soft_right()
{
  I2C_Send_Message(0x02, PWM_MIN_VAL);
  I2C_Send_Message(0x03, PWM_MAX_VAL - PWM_TURN_OFFSET);
  I2C_Send_Message(0x04, PWM_MIN_VAL);
  I2C_Send_Message(0x05, PWM_MIN_VAL);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi naglo skretanje vozila ulijevo.
 */
void drive_hard_left()
{
  I2C_Send_Message(0x02, PWM_MIN_VAL);
  I2C_Send_Message(0x03, PWM_MIN_VAL);
  I2C_Send_Message(0x04, PWM_MIN_VAL);
  I2C_Send_Message(0x05, PWM_MAX_VAL);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi naglo skretanje vozila udesno.
 */
void drive_hard_right()
{
  I2C_Send_Message(0x02, PWM_MIN_VAL);
  I2C_Send_Message(0x03, PWM_MAX_VAL);
  I2C_Send_Message(0x04, PWM_MIN_VAL);
  I2C_Send_Message(0x05, PWM_MIN_VAL);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi zaustavljanje vozila.
 */
void drive_stop()
{
  I2C_Send_Message(0x02, PWM_MIN_VAL);
  I2C_Send_Message(0x03, PWM_MIN_VAL);
  I2C_Send_Message(0x04, PWM_MIN_VAL);
  I2C_Send_Message(0x05, PWM_MIN_VAL);
}