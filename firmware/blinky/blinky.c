/** @file blinky.c
 *
 * @brief Fajl sa izvornim C kodom koji implementira jednostavan
 * primjer blinkanja LED, u svrhu testiranja programiranja MCU-a.
 */

void main() 
{
  // Konfiguracija internih registara pinova MCU-a
  AD1PCFG = 0xFFFF;
  TRISB.F2 = 0;
  LATB.F2 = 0;
  
  /**
   * Naredna petlja invertuje naponski nivo pina na koji je povezana
   * LED na svakih jednu sekundu.
   */
  while(1)
  {
    LATB.F2 = ~LATB.F2;
    Delay_ms(1000);
  }
}
