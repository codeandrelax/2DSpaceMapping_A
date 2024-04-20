/** @file radar.c
 *
 * @brief Fajl sa izvornim C kodom koji simulira funkcionalnost
 * radara, koji je u ovom slucaju spoj servo motora i IR senzora.
 * Servo motor se zakrece od 0 do 180 stepeni i unazad, u koracima
 * i kupi odmjerke sa IR senzora koje nakon toga prezentuje pomocu
 * UART na USB konvertera na terminal korisnickog racunara.
 */

#define PWM_FREQ 50 // Frekvencija PWM signala za servo motor
#define BAUD_RATE 9600 // Bitska brzina UART prenosa
#define RADAR_RESOLUTION 20 // Rezolucija radara (broj tacaka u kojima se uzima odmjerak)

#define IR_PIN F2
#define SERVO_PIN F7
#define UART_RX_PIN F10
#define UART_TX_PIN F11

// Faktor popune PWM signala koji odgovara uglu od priblizno 0 stepeni servo motora
const double SERVO_MIN_DUTY = 2.5; 
// Faktor popune PWM signala koji odgovara uglu od priblizno 180 stepeni servo motora
const double SERVO_MAX_DUTY = 12.5;
// Naponski kvant A/D konvertora dobijen kao kolicnik referentnog napona i maks. binarne rijeci ADC (10 bita)
const double DELTA_V = 3.3 / 1024.0;

/**
 * @brief Funkcija koja konvertuje podatak o udaljenosti iz realnog broja u string
 * @param[in] value Realan broj koji predstavlja izmjerenu udaljenost 
 * @param[out] res Rezultantni string dobijen nakon konverzije, duzine 5 karaktera
 */
void distance_to_str(double value, char* res)
{
  /**
   * Funkcija ima elemente hard-kodovanja iz razloga sto je format broja koji
   * predstavlja udaljenost uvijek jasno poznat (jednocifren realan broj).
   * Funkcija je implementirana zbog mana koje unosi funkcija FloatToStr
   * iz Conversions biblioteke (dodavanje e^-n u string za brojeve < 0.1).
   */
  int stripped = (int)(value * 1000.0);
  res[0] = 0x30 + (int)(stripped / 1000);
  res[1] = '.';
  res[2] = 0x30 + (int)((stripped % 1000) / 100);
  res[3] = 0x30 + (int)((stripped % 100) / 10);
  res[4] = 0x30 + (int)(stripped % 10);
  res[5] = '\0';
  // Rezultantni string predstavlja konvertovani realan broj sa 3 cifre iza zareza, bez zaokruzivanja
}

void main() 
{
  unsigned pwm_period = 0;
  unsigned i = 0;
  double duty_cycle = SERVO_MIN_DUTY;
  double adc_value = 0.0;
  double duty_increment = 0.5;
  char distance_str[15];
  char angle_str[15];
  // Konfiguracija internih registara pinova MCU-a
  AD1PCFGL = 0xFFEF; // Podesi pin IR senzora kao analogni, ostatak digitalni
  TRISB = 0;
  LATB = 0;
  TRISB.IR_PIN = 1; // Podesi pin IR senzora kao ulazni
  TRISB.UART_RX_PIN = 1; // Podesi RX pin UART veze kao ulazni
  ODCB.SERVO_PIN = 1; // Podesi servo pin kao open-drain za 5V PWM izlaz
  RPOR3 = 0x1200; // Remapiraj pin za UART TX kao U1TX
  RPOR5 = 0x0300; // Remapiraj servo pin kao OC1 za omogucenje PWM-a
  RPINR18 = 0x1F0A; // Remapiraj UART RX pin kao U1RX
  // Inicijalizuj A/D konvertor
  ADC1_Init();
  // Inicijalizuj PWM
  pwm_period = PWM_Init(PWM_FREQ, 1, 0, 2);
  PWM_Start(1);
  // Inicijalizuj UART
  UART1_Init(BAUD_RATE);
  
  Delay_ms(100);
  
  // Poslati vrijednost kvanta napona A/D konvertora na UART
  FloatToStr(DELTA_V, distance_str);
  UART1_Write_Text("Voltage chunk: ");
  UART1_Write_Text(distance_str);
  UART1_Write('\n');
  /**
   * Svaka iteracija while petlje alternirajuce pomjera servo motor od 0 do 180 
   * i nazad od 180 do 0 stepeni u koracima odredjenim rezolucijom radara. 
   */
  while(1)
  {
    i = 0;
	/**
	 * Svaka iteracija for petlje pomjera servo motor na sledeci ugao 
	 * kao odgovarajuci inkrement faktora popune, koji zavisi od rezolucije
	 * radara. U svakoj iteraciji se uzme odmjerak sa A/D konvertora,
	 * pretvori u udaljenost (datoj kao napon sa senzora) i zajedno sa 
	 * faktorom popune za koji je dobijen odmjerak, posalje korisniku
	 * preko UART veze.
	 */
    for(;i<=RADAR_RESOLUTION;i++)
    {
      PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
      Delay_ms(300);
      adc_value = ADC1_Get_Sample(4) * DELTA_V;
      distance_to_str(adc_value, distance_str);
      FloatToStr(duty_cycle, angle_str);
      UART1_Write_Text("Distance: ");
      UART1_Write_Text(distance_str);
      UART1_Write_Text("; Duty: ");
      UART1_Write_Text(angle_str);
      UART1_Write('\n');
      duty_cycle += duty_increment;
    }
    duty_increment *= (-1.0);
  }
}