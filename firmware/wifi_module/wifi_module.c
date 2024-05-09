/** @file wifi_module.c
 * 
 * @brief Fajl koji sadrži kod koji testira rad ESP8266-01 modula za 
 * WiFi komunikaciju. Sastoji se od koda koji vrši osnovne konfiguracije 
 * modula, povezivanje na WiFi mrežu, uspostavljanje komunikacije sa udaljenim
 * uređajem i konačno slanje obične poruke kontinualno u vremenu.
 */

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
const char* CMD_WIFI_CONN = "AT+CWJAP=\"etfbl.net\",\"\"\r\n";
const char* CMD_CONN_TYPE = "AT+CIPMUX=0\r\n";
const char* CMD_START_TCP = "AT+CIPSTART=\"TCP\",\"10.99.130.47\",8084\r\n";
const char* CMD_SEND = "AT+CIPSEND=7\r\n";

unsigned response = 0;

/**
 * @brief Funkcija rukovaoca prekida za prijem UART podataka.
 * Čita podatak dobijen od strane ESP8266-01 i šalje ga na drugi
 * UART preko koga se vrši debug-ovanje.
 */
void UART1_Receiver_Interrupt() iv 0x00002A
{
  IEC0.U1RXIE = 0;
  response = UART1_Read();
  UART2_Write(response);
  IEC0.U1RXIE = 1;
  IFS0.U1RXIF = 0;
}

/**
 * @brief Funkcija rukovaoca prekida koja se aktivira kada se
 * desi prekoračenje UART bafera za prijem podataka. Rukovaoc
 * čisti bit greške (OERR) čime omogućava nastavak UART prenosa. 
 */
void UART1_Error_Interrupt() iv 0x000096
{
  IEC4.U1ERIE = 0;
  U1STA.OERR  = 0;
  IEC4.U1ERIE = 1;
  IFS4.U1ERIF = 0;
}

void main()
{
  CLKDIV = 0;
  AD1PCFGL = 0xFFFF;
  TRISB = 0;
  LATB = 0;
  
  IPC2bits.U1RXIP = 5;
  IFS0.U1RXIF = 0;
  IEC0.U1RXIE = 1;
  
  IPC16bits.U1ERIP = 5;
  IFS4.U1ERIF = 0;
  IEC4.U1ERIE = 1;
  
  Unlock_IOLOCK();
  PPS_Mapping_NoLock(14, _INPUT, _U1RX);
  PPS_Mapping_NoLock(13, _OUTPUT, _U1TX);
  PPS_Mapping_NoLock(10, _INPUT, _U2RX);
  PPS_Mapping_NoLock(11, _OUTPUT, _U2TX);
  Lock_IOLOCK();
  
  UART1_Init(BAUD_RATE_WIFI);
  Delay_ms(500);
  UART2_Init(BAUD_RATE);
  Delay_ms(500);

  UART2_Write_Text("Sending reset command...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_RST);

  Delay_ms(2000);
  
  UART2_Write_Text("\r\nSending AT command...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_AT);

  Delay_ms(1000);
  
  UART2_Write_Text("\r\nSetting STA mode...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_MODE);

  Delay_ms(1000);
  
  UART2_Write_Text("\r\nSetting connection type...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_CONN_TYPE);

  Delay_ms(1000);

  UART2_Write_Text("\r\nListing available APs...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_LIST_AP);
  
  Delay_ms(5000);
  
  UART2_Write_Text("\r\nConnecting to the WiFi...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_WIFI_CONN);

  Delay_ms(5000);
    
  while(1)
  {
    UART2_Write_Text("\r\nStarting TCP connection...\r\n");
    Delay_ms(100);
    UART1_Write_Text(CMD_START_TCP);
    Delay_ms(2000);
    UART1_Write_Text(CMD_SEND);
    Delay_ms(2000);
    UART1_Write_Text("Hello\r\n");
    Delay_ms(2000);
  }
}