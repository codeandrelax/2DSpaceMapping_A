/** @file ir_distance.c
 *
 * @brief Fajl sa izvornim C kodom koji testira rad IR senzora
 * udaljenosti. Implementirana logika podrazumijeva ukljucenje
 * LED kada senzor ocita udaljenost objekta koja je manja od 
 * postavljene granice.
 */

/**
 * Konstanta vrijednost koja predstavlja granicu dozvoljene
 * udaljenosti objekta, nakon koje se ukljucuje LED. Odgovara
 * udaljenosti od priblizno 75 cm.
 */
const unsigned THRESHOLD = 0x0070;

void main() 
{ 
  unsigned adc_value=0x0000; // bafer za prijem odmjerka A/D konvertora
  // Konfiguracija internih registara pinova MCU-a
  AD1PCFG = 0xFFEF;
  TRISB.F10=0;
  TRISB.F2=1;
  // Inicijalizacija A/D konvertora
  ADC1_Init();
  // Testno ukljucenje LED i pauza od 5 sekundi
  LATB.F10 = 1;
  Delay_ms(5000);
  LATB.F10 = 0;
  while(1)
  { 
	// U svakoj iteraciji uzimamo odmjerak i poredimo sa granicom
    adc_value=ADC1_Get_Sample(4);
    if (adc_value > THRESHOLD)
       LATB.F10=1;
    else
       LATB.F10=0;
    // "Odspavaj" 25 ms da se osigura novo ocitavanje
    Delay_ms(25);
  }
}