void main() 
{
  AD1PCFG = 0xFFFF;
  TRISB.F2 = 0;
  LATB.F2 = 0;

  while(1)
  {
    LATB.F2 = ~LATB.F2;
  }
}