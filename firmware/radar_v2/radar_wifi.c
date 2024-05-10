/** @file radar_wifi.c
 *
 * @brief Fajl sa izvornim C kodom koji simulira funkcionalnost
 * radara, koji je u ovom slucaju spoj servo motora i IR senzora.
 * Servo motor se zakrece od 0 do 180 stepeni i unazad u koracima,
 * te kupi odmjerke sa IR senzora koje nakon toga preko WiFi modula
 * salje ka serverskoj aplikaciji koja iscrtava prostor oko radara.
 */

#include "esp8266.h"

#define PWM_FREQ 50 // Frekvencija PWM signala za servo motor
#define BAUD_RATE 115200 // Bitska brzina UART prenosa za ESP8266
#define RADAR_RESOLUTION 20 // Rezolucija radara (broj tacaka u kojima se uzima odmjerak)
#define BUFFER_LEN 1024 // Velicina bafera za povratne podatke ESP8266 modula
#define FILTER_LEN 5 // Duzina MA filtra koji filtrira senzorske podatke
#define SEPARATOR "," // Separatorski string za format poruke koja prenosi podatke
#define START_LABEL '%' // Predefinisani znak koji oznacava nastavak kretanja vozila
#define STOP_LABEL '#' // Predefinisani znak koji oznacava zaustavljanje kretanja vozila

#define IR_PIN F2
#define SERVO_PIN F7
#define ESP_TX_PIN F13
#define ESP_RX_PIN F14
#define LED_PIN F10

// Faktor popune PWM signala koji odgovara uglu od priblizno 0 stepeni servo motora
const double SERVO_MIN_DUTY = 2.5;
// Faktor popune PWM signala koji odgovara uglu od priblizno 180 stepeni servo motora
const double SERVO_MAX_DUTY = 12.5;
// Duzina poruke koja se salje udaljenom racunarskom sistemu
const unsigned MESSAGE_LEN = (RADAR_RESOLUTION + 1) * 10;

volatile char buffer[BUFFER_LEN];
volatile unsigned buf_idx = 0;
unsigned start_stop_flag = 0;

/**
 * @brief Prekidna rutina koja obradjuje prijem podatka preko UART veze
 */
void UART1_Receiver_Interrupt() iv 0x00002A
{
  IEC0.U1RXIE = 0;
  buffer[buf_idx] = UART1_Read();
  if(buffer[buf_idx] == START_LABEL)
  {
    start_stop_flag = 1;
  }
  else if(buffer[buf_idx] == STOP_LABEL)
  {
    start_stop_flag = 0;
  }
  buf_idx++;
  IEC0.U1RXIE = 1;
  IFS0.U1RXIF = 0;
}

/**
 * @brief Prekidna rutina koja obradjuje gresku preteka UART bafera za podatke
 */
void UART1_Error_Interrupt() iv 0x000096
{
  IEC4.U1ERIE = 0;
  U1STA.OERR  = 0;
  IEC4.U1ERIE = 1;
  IFS4.U1ERIF = 0;
}

/**
 * @brief Funkcija provjerava odgovor ESP8266 modula na prethodno datu komandu
 * @returns 1 - odgovor je OK, -1 - odgovor je ERROR, 0 - nema odgovora
 */
short check_response()
{
  if(strstr(buffer, MESSAGE_OK) != 0)
  {
    return 1;
  }
  else if(strstr(buffer, MESSAGE_ERROR) != 0)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Funkcija koja ceka polling metodom na odgovor ESP8266 modula
 * @returns Krajnji dobijeni odgovor
 */
short poll_response()
{
  short response = 0;
  LATB.LED_PIN = 1;
  do
  {
    response = check_response();
    Delay_ms(100);
  }while(response == 0);
  memset(buffer, 0, BUFFER_LEN);
  buf_idx = 0;
  LATB.LED_PIN = 0;
  return response;
}

/**
 * @brief Funkcija koja uspostavlja povezivanje na WiFi mrezu
 */
void establish_wifi_connection()
{
  // Prvo resetujemo modul
  UART1_Write_Text(CMD_RST);
  Delay_ms(3000);
  memset(buffer, 0, BUFFER_LEN);
  buf_idx = 0;
  // Podesimo STATION rezim rada
  UART1_Write_Text(CMD_STA_MODE);
  poll_response();
  // Podesimo jednokanalnu komunikaciju
  UART1_Write_Text(CMD_SINGLE_CONN_TYPE);
  poll_response();
  // Povezemo se na WiFi mrezu
  UART1_Write_Text(CMD_WIFI_CONN);
  poll_response();
}

/**
 * @brief Funkcija koja salje poruku udaljenom racunarskom sistemu
 * preko WiFi mreze putem ESP8266 modula
 * @param[in] message Poruka koju treba poslati
 */
void send_wifi_message(char* message)
{
  char cmd[32];
  sprintf(cmd, CMD_SEND, strlen(message));
  UART1_Write_Text(cmd);
  poll_response();
  UART1_Write_Text(message);
}

void main()
{
  unsigned pwm_period = 0;
  unsigned i = 0, j = 0, sum = 0;
  unsigned adc_value = 0.0;
  short resp = 0;
  double duty_cycle = SERVO_MIN_DUTY;
  double duty_increment = (SERVO_MAX_DUTY - SERVO_MIN_DUTY) / RADAR_RESOLUTION;
  char distance_str[6];
  char angle_str[15];
  char message[MESSAGE_LEN];

  // Konfiguracija internih registara pinova MCU-a
  CLKDIV = 0;
  AD1PCFGL = 0xFFEF; // Podesi pin IR senzora kao analogni, ostatak digitalni
  TRISB = 0;
  LATB = 0;
  TRISB.IR_PIN = 1; // Podesi pin IR senzora kao ulazni
  TRISB.ESP_RX_PIN = 1; // Podesi RX pin UART veze kao ulazni
  ODCB.SERVO_PIN = 1; // Podesi servo pin kao open-drain za 5V PWM izlaz
  RPOR3 = 0x1200; // Remapiraj servo pin kao OC1 za omogucenje PWM-a
  RPOR6 = 0x0300; // Remapiraj pin za UART TX kao U1TX
  RPINR18 = 0x1F0E; // Remapiraj UART RX pin kao U1RX

  // Konfiguracija registara za prekide
  IPC2bits.U1RXIP = 5;
  IFS0.U1RXIF = 0;
  IEC0.U1RXIE = 1;
  IPC16bits.U1ERIP = 5;
  IFS4.U1ERIF = 0;
  IEC4.U1ERIE = 1;

  // Inicijalizuj A/D konvertor
  ADC1_Init();

  // Inicijalizuj PWM
  pwm_period = PWM_Init(PWM_FREQ, 1, 8, 2);
  PWM_Start(1);

  // Inicijalizuj UART
  UART1_Init(BAUD_RATE);

  Delay_ms(100);

  establish_wifi_connection();

  // Uspostavimo TCP vezu sa udaljenom aplikacijom
  UART1_Write_Text(CMD_START_TCP);
  resp = poll_response();

  memset(buffer, 0, BUFFER_LEN);
  buf_idx = 0;
  start_stop_flag = 1;

  /**
   * Svaka iteracija while petlje alternirajuce pomjera servo motor od 0 do 180
   * i nazad od 180 do 0 stepeni u koracima odredjenim rezolucijom radara.
   */
  while(1)
  {
    _wait_for_start_:
    if(start_stop_flag == 1)
    {
      memset(distance_str, 0, 6);
      memset(angle_str, 0, 15);
      memset(message, 0, MESSAGE_LEN);
      /**
       * Svaka iteracija for petlje pomjera servo motor na sledeci ugao
       * kao odgovarajuci inkrement faktora popune, koji zavisi od rezolucije
       * radara. U svakoj iteraciji se uzme odmjerak sa A/D konvertora,
       * pretvori u udaljenost (datoj kao napon sa senzora) i zajedno sa
       * faktorom popune za koji je dobijen odmjerak, konkatenira na string
       * koji se kasnije proslijedjuje korisniku preko WiFi veze.
       */
      for(i=0; i<=RADAR_RESOLUTION; i++)
      {
        if(start_stop_flag == 0)
        {
          goto _wait_for_start_;
        }
        PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
        Delay_ms(200);
        if(start_stop_flag == 0)
        {
          goto _wait_for_start_;
        }
        sum = 0;
        for(j=0; j<FILTER_LEN; j++)
        {
          sum += ADC1_Get_Sample(4);
          Delay_ms(25);
        }
        if(start_stop_flag == 0)
        {
          goto _wait_for_start_;
        }
        adc_value = sum / FILTER_LEN;
        WordToStr(adc_value, distance_str);
        FloatToStr(duty_cycle, angle_str);
        strncat(message, distance_str, 6);
        strcat(message, SEPARATOR);
        strncat(message, angle_str, 4);
        strcat(message, SEPARATOR);
        duty_cycle += duty_increment;
      }
      message[strlen(message) - 1] = 0;
      duty_cycle -= duty_increment;
      duty_increment *= (-1.0);
      if(resp == 1)
      {
        send_wifi_message(message);
      }
    }
    else
    {
      duty_cycle = SERVO_MIN_DUTY;
      duty_increment = (SERVO_MAX_DUTY - SERVO_MIN_DUTY) / RADAR_RESOLUTION;
      PWM_Set_Duty(SERVO_MIN_DUTY * pwm_period / 100, 1);
    }
  }
}