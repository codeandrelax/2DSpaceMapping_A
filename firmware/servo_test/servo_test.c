/** @file servo_test.c
 *
 * @brief Fajl sa izvornim C kodom koji testira rad servo motora.
 */

#define PWM_FREQ 50

void main() 
{
  unsigned int pwm_period = 0;
  float duty_cycle = 2.5;
  // Konfiguracija internih registara pinova MCU-a
  AD1PCFGL = 0xFFFF;
  TRISB.F7 = 0;
  LATB.F7 = 0;
  /**
   * Podesavanje pina koji kontrolise servo kao open-drain da bismo
   * omogucili PWM signal ciji je visok nivo 5V sto je neophodno za
   * rad koristenog servo motora.
   */
  ODCB.F7 = 1; 
  RPOR3 = 0x1200; // Remapiraj servo pin kao Output Compare 1 da se omoguci PWM
  // Inicijalizacija i pokretanje PWM kontrolnog izlaza
  pwm_period = PWM_Init(PWM_FREQ, 1, 0, 2);
  PWM_Start(1);
  PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
  /**
   * Sledeca petlja kontinualno zakrece servo motor od 0 do 180
   * stepeni i nazad.
   */
  while(1)
  {
    Delay_ms(5000);
    if (duty_cycle > 5.0)
      duty_cycle = 2.5;
    else
      duty_cycle = 15.0;
    PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
  }
}