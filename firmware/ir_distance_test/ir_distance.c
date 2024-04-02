const treshold=0x0070;

void main() {
  unsigned adc_value=0x0000;
  AD1PCFG = 0xFFEF;
  TRISB.F10=0;
  TRISB.F2=1;
  ADC1_Init();
  LATB.F10 = 1;
  Delay_ms(5000);
  LATB.F10 = 0;
  while(1){
    adc_value=ADC1_Get_Sample(4);
    if ( adc_value> treshold)
       LATB.F10=1;
    else
       LATB.F10=0;

    Delay_ms(25);
  }
}