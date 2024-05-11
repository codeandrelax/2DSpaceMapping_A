/** @file libhcsr04.h
 *
 * @brief Header fajl koji sadrzi osovne funkcije i konstante za rad sa
 * HC-SR04 ultrazvucnim senzorom udaljenosti.
 *
 * @warning Odabrani pinovi su predefinisani i moraju biti:
 * - ECHO = RB10
 * - TRIG = RB11
 */

#ifndef _HCSR04_H_
#define _HCSR04_H_

/* Konstane koje oznacavaju mjerni opseg senzora (u cm) */
#define HCSR04_MIN_DISTANCE 2
#define HCSR04_MAX_DISTANCE 400

void HCSR04_Init();
unsigned HCSR04_Get_Distance();

#endif
/* end of file */