/** @file hcsr04.c
 *
 * @brief Fajl sa izvornim kodom koji implementira funkcije za rad sa
 * HC-SR04 ultrazvucnim senzorom.
 */

#include "hcsr04.h"

volatile unsigned distance = 0; // Varijabla koja skladisti mjerenu udaljenost
volatile unsigned meas_done = 0; // Fleg koji indicira zavrsetak mjerenja

/**
 * Sledeci fatkor se koristi prilikom konverzije izmjerenog
 * vremenskog intervala u odgovarajucu udaljenost od objekta.
 * Dobijen je u odnosu na podesavanje tajmera (inkrement tajmera
 * na svake 2 us) i znanje da je udaljenost od objekta jednaka
 * c * t / 2. Dijeljenje sa 2 je zbog prelaska duplog puta ultrazvucnog
 * talasa, a c je brzina zvuka (343 m/s). Inkrement na 2 sekunde
 * i prelazak duplog puta se medjusobno pokrate, pa se faktor dobije
 * kao direktno pretvaranje iz m/s u cm/us.
 */
const float CVT_FACTOR = 0.0343;

/**
 * @brief Rukovac prekida za State Change Notification prekid
 *
 * @details Ova funkcija je glavna za logiku rada i mjerenje udaljenosti.
 * Funkcionise na principu detektovanja stanja ECHO pina. Kada se posalje
 * ultrazvucni burst, ECHO pin se postavlja na log. '1'. Tada je potrebno
 * aktivirati tajmer koji zapocinje mjerenje vremena. Kada se detektuju
 * povratni ultrazvucni talasi, senzor spusta ECHO pin na log '0'.
 * Tada se mjerenje vremena zaustavlja, cita se izmjereni vremenski interval
 * te se pretvara u stvarnu udaljenost i konacno resetuje registar tajmera.
 */
void cn_interrupt() iv 0x00003A
{
  IEC1.CNIE = 0;
  if(PORTB.F10 == 1)
  {
    T1CON.TON = 1;
  }
  else
  {
    T1CON.TON = 0;
    distance = (unsigned)((float)TMR1 * CVT_FACTOR);
    meas_done = 1;
    TMR1 = 0;
  }
  IFS1.CNIF = 0;
  IEC1.CNIE = 1;
}

/**
 * @brief Funkcija koja vrsi inicijalizaciju pinova i registara potrebnih
 * za mjerenje udaljenosti pomocu HC-SR04 senzora.
 */
void HCSR04_Init()
{
  /* Podesi RB10 kao ulazni pin */
  TRISB.F10 = 1;
  /* Podesavanje State Change Notification prekida */
  CNEN2.CN16IE = 1;
  IFS1.CNIF = 0;
  IEC1.CNIE = 1;
  /* Podesavanje prescaler-a prvog tajmera (inkrement na svake 2 us) */
  T1CON = 0x0010;
  /* Reset registra prvog tajmera u kome se vrsi odbrojavanje*/
  TMR1 = 0;
}

/**
 * @brief Funkcija koja salje zahtjev za pocetak mjerenja i vraca
 * izmjerenu udaljenost.
 */
unsigned HCSR04_Get_Distance()
{
  /* Generisati impuls trajanja 10 us na TRIGGER pinu da zapocne mjerenje */
  LATB.F11 = 1;
  Delay_us(10);
  LATB.F11 = 0;
  /* Sacekaj dok se ne zavrsi mjerenje */
  while(0 == meas_done);
  /* Obradi i vrati izmjereni podatak */
  meas_done = 0;
  if(distance >= HCSR04_MAX_DISTANCE)
  {
    return HCSR04_MAX_DISTANCE;
  }
  else if(distance <= HCSR04_MIN_DISTANCE)
  {
    return HCSR04_MIN_DISTANCE;
  }
  else
  {
    return distance;
  }
}

/* end of file */