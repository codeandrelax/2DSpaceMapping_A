#include "hcsr04.h"

void main()
{
  unsigned distance = 0;
  char txt[6];
  AD1PCFGL = 0xFFFF;
  LATB = 0;
  TRISB = 0;
  TRISB.F2 = 1;
  RPOR3 = 0x0300;
  RPINR18 = 0x1F02;
  UART1_Init(9600);
  HCSR04_Init();
  UART1_Write_Text("STARTING\n");
  Delay_ms(100);
  while(1)
  {
    distance = HCSR04_Get_Distance();
    WordToStr(distance, txt);
    UART1_Write_Text(txt);
    UART1_Write('\n');
    Delay_ms(1000);
  }
}