#define WIFI_RX_PIN F13
#define WIFI_TX_PIN F14
#define UART_RX_PIN F10
#define UART_TX_PIN F11
#define BAUD_RATE_WIFI 115200
#define BAUD_RATE 115200

const char* CMD_AT = "AT\r\n";
const char* CMD_RST = "AT+RST\r\n";
const char* CMD_MODE = "AT+CWMODE=1\r\n";
const char* CMD_LIST_AP = "AT+CWLAP\r\n";

void response_wait()
{
  unsigned resp;
  unsigned counter = 0;
  while(1)
  {
    if(UART1_Data_Ready() == 1)
    {
      resp = UART1_Read();
      UART2_Write(resp);
      counter = 0;
    }
    else
    {
      counter++;
      Delay_ms(1);
    }
    if(counter == 5000)
      break;
  }
}

void main()
{
  char flag = 0;
  char ok_str[3] = {0,0,0};
  unsigned long clock = 0;
  unsigned long fosc = 0;
  char tmp_str[25];
  CLKDIV = 0;
  AD1PCFGL = 0xFFFF;
  TRISB = 0;
  LATB = 0;
  //TRISB.UART_RX_PIN = 1;
  
  Unlock_IOLOCK();
  PPS_Mapping_NoLock(14, _INPUT, _U1RX);
  PPS_Mapping_NoLock(13, _OUTPUT, _U1TX);
  PPS_Mapping_NoLock(10, _INPUT, _U2RX);
  PPS_Mapping_NoLock(11, _OUTPUT, _U2TX);
  Lock_IOLOCK();
  
  //RPINR19 = 0x1F0A;
  //RPOR5 = 0x0500;
  
//  clock = Clock_Mhz();
//  fosc = Get_Fosc_kHz();
  
  UART1_Init(BAUD_RATE_WIFI);
  Delay_ms(500);
  UART2_Init(BAUD_RATE);
  Delay_ms(500);
  
//  LongWordToStr(clock, tmp_str);
//
//  UART2_Write_Text("Clock = ");
//  UART2_Write_Text(tmp_str);
//  UART2_Write_Text("\r\n");
//
//  LongWordToStr(fosc, tmp_str);
//
//  UART2_Write_Text("Fosc = ");
//  UART2_Write_Text(tmp_str);
//  UART2_Write_Text("\r\n");
//
//  UART2_Write_Text("Sending reset command...\r\n");
//  Delay_ms(100);
//  UART1_Write_Text(CMD_RST);
//  response_wait();
//
//  Delay_ms(500);
//
//  UART2_Write_Text("Sending AT command...\r\n");
//  Delay_ms(100);
//  UART1_Write_Text(CMD_AT);
//  response_wait();
//
//  Delay_ms(500);
//
//  UART2_Write_Text("\nListing available APs...\r\n");
//  Delay_ms(100);
//  UART1_Write_Text(CMD_LIST_AP);
//  response_wait();
//
//  UART2_Write_Text("Start loopback...\r\n\r\n");
//
  while(1)
  {
    if(UART2_Data_Ready() == 1)
    {
      flag = UART2_Read();
      if(flag == 'a')
      {
        UART2_Write_Text(CMD_AT);
        UART1_Write_Text(CMD_AT);
        ok_str[0] = UART1_Read();
        ok_str[1] = UART1_Read();
        UART2_Write_Text("Response: ");
        UART2_Write_Text(ok_str);
        UART2_Write_Text("\r\n\r\n");
      }
    }
  }
}