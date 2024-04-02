#define PWM_FREQ 50

void main() {
  unsigned int pwm_period = 0;
  float duty_cycle = 2.5;
  AD1PCFGL = 0xFFFF;
  TRISB.F7 = 0;
  LATB.F7 = 0;
  ODCB.F7 = 1;
  RPOR3 = 0x1200;
  pwm_period = PWM_Init(PWM_FREQ, 1, 0, 2);
  PWM_Start(1);
  PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
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