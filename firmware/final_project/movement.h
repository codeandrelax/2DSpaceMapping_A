/** @file movement.h
 * 
 * @brief Heder fajl koji sadrzi konstante i prototipe funkcija koje 
 * opisuju i kontrolisu kretanje autonomnog vozila putem I2C komunikacije 
 * sa drajverskom plocom.
*/

#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

/* I2C adresa PWM kontrolera drajverske ploce */
#define DEVICE_ADDRESS 0x0E 

/* Maksimalna vrijednost faktora popune drajverskog PWM signala */
#define PWM_MAX_VAL 255 

/* Minimalna vrijednost faktora popune drajverskog PWM signala */
#define PWM_MIN_VAL 0 

/* Brzina kretanja vozila naprijed (data kao faktor popune) */
#define PWM_FORWARD_VAL 90

/* Faktor smanjenja brzine skretanja motora za blaga skretanja */
#define PWM_TURN_OFFSET 75

void init_driving();
void drive_forward();
void drive_soft_left();
void drive_soft_right();
void drive_hard_left();
void drive_hard_right();
void drive_stop();

#endif

/* end of file */