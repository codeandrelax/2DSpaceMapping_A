/** @file auto_car.c
 *
 * @brief Fajl sa izvornim C kodom koji implementira logiku 
 * kretanja autonomnog vozila. 
 */

#define DEVICE_ADDR 0xE0
#define SERVO_PWM_FREQ 50
#define IR_PIN F2
#define SERVO_PIN F7

/**
 * Konfiguracija pinova za ostvarenje komunikacije putem 
 * softverskog I2C protokola.
 */
sbit Soft_I2C_Sda           at RB9_bit;
sbit Soft_I2C_Scl           at RB8_bit;
sbit Soft_I2C_Sda_Direction at TRISB9_bit;
sbit Soft_I2C_Scl_Direction at TRISB8_bit;

/**
 * Granicna vrijednost koja odredjuje kada treba da se zaustavi
 * vozilo, ako su sve udaljenosti vece od zadate.
 */
const unsigned STOP_THRESHOLD = 560;

/**
 * Niz vrijednost faktora popune PWM signala koji kontrolise rad
 * servo motora. Vrijednosti odgovaraju uglovima od priblizno
 * 0, 45, 90, 135 i 180 stepeni, respektivno.
 */
const unsigned DUTY_CYCLES[5] = {2, 5, 7, 10, 13};

/*
 * Niz koji cuva udaljenosti dobijene od IR senzora kroz svaku iteraciju
 * od 0 do 180 stepeni zakretanja servo motora
 */
unsigned distances[5] = {0,0,0,0,0};

/**
 * @brief Funkcija koja salje podatak preko I2C protokola
 * @param[in] dev_addr Adresa uredjaja kome se salje podatak
 * @param[in] reg_addr Adresa registra uredjaja u koji se podatak upisuje
 * @param[in] data_value Jednobajtni podatak koji se salje
 */
void I2C_Send_Message(unsigned short dev_addr, unsigned short reg_addr, unsigned short data_value)
{
  Soft_I2C_Start();
  Soft_I2C_Write(dev_addr);
  Soft_I2C_Write(reg_addr);
  Soft_I2C_Write(data_value);
  Soft_I2C_Stop();
}

/**
 * Upis vrijednosti u registre 0x02 - 0x05 upravlja vrijednostima
 * PWM kanala. Vrijednosti u tim registrima podesavaju pravac
 * kretanja autonomnog vozila.
 */
 
/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi kretanje vozila unaprijed.
 */
inline void drive_forward()
{
  I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x03, 115);
  I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x05, 115);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi blago skretanje vozila ulijevo.
 */
inline void drive_soft_left()
{
  I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x03, 110);
  I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x05, 120);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi blago skretanje vozila udesno.
 */
inline void drive_soft_right()
{
  I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x03, 120);
  I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x05, 110);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi naglo skretanje vozila ulijevo.
 */
inline void drive_hard_left()
{
  I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x03, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x05, 125);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi naglo skretanje vozila udesno.
 */
inline void drive_hard_right()
{
  I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x03, 125);
  I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x05, 0);
}

/**
 * @brief Funkcija koja podesava vrijednosti registara PWM kanala
 * tako da se obezbjedi zaustavljanje vozila.
 */
inline void drive_stop()
{
  I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x03, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
  I2C_Send_Message(DEVICE_ADDR, 0x05, 0);
}

/**
 * @brief Funkcija koja odlucuje naredni pravac kretanja vozila za
 * trenutno skenirani polukrug prostora i dobijene udaljenosti.
 */
void decide_direction()
{
  unsigned min_distance = distances[2];
  unsigned direction = 2;
  unsigned i = 0;
  for(; i<5; i++)
  {
    if(distances[i] < min_distance)
    {
      direction = i;
      min_distance = distances[i];
    }
  }
  if(min_distance >= STOP_THRESHOLD)
  {
    drive_stop();
    return;
  }
  switch(direction)
  {
    case 0:
      drive_hard_left();
      break;
    case 1:
      drive_soft_left();
      break;
    case 3:
      drive_soft_right();
      break;
    case 4:
      drive_hard_right();
    default:
      drive_forward();
  }
}

void main()
{
  unsigned pwm_period = 0;
  unsigned i = 0;
  
  // Konfiguracije registara MCU
  AD1PCFGL = 0xFFEF; // Podesi pin IR senzora kao analogni, ostatak digitalni
  LATB = 0;
  TRISB = 0;
  TRISB.IR_PIN = 1; // Podesi pin IR senzora kao ulazni
  ODCB.SERVO_PIN = 1; // Podesi servo pin kao open-drain za 5V PWM izlaz
  RPOR3 = 0x1200; // Remapiraj servo pin kao OC1 za omogucenje PWM-a
  
  // Inicijalizacija A/D konvertora
  ADC1_Init();

  //Inicijalizacija PWM modula
  pwm_period = PWM_Init(SERVO_PWM_FREQ, 1, 0, 2);
  PWM_Start(1);
  
  // Inicijalizacija softverskog I2C protokola
  Soft_I2C_Init();
  
  Delay_ms(100);

  /**
   * Podesavanje REG_MODE1 registra PWM kontrolera koji kontrolise
   * rad motora. Assert-uje se samo bit najmanje tezine koji
   * omogucuje reakciju na all-call adresiranje od strane master
   * uredjaja (adresa 0xE0).
   */
  I2C_Send_Message(DEVICE_ADDR, 0x00, 0x01);
     
  /**
   * Podesavanje LEDOUT registra PWM kontrolera koji kontrolise
   * rad motora. Upisivanje podatka 0xAA omogucuje kontrolu vrijednosti
   * PWM signala sa strane master uredjaja upisom u odgovarajuce registre
   */
  I2C_Send_Message(DEVICE_ADDR, 0xE8, 0xAA);

  Delay_ms(100);

  drive_forward();

  while(1)
  {
	// Zakreni servo od 0 do 180 stepeni u koracima i uzmi odmjerke
    for(i=0; i<5; i++)
    {
      PWM_Set_Duty(DUTY_CYCLES[i] * pwm_period / 100, 1);
      Delay_ms(200); // Potrebno kasnjenje da bi servo dobio dovoljno impulsa
      distances[i] = ADC1_Get_Sample(4);
    }
    decide_direction();
	
	// Zakreni servo od 180 do 0 stepeni u koracima i uzmi odmjerke
    for(i=4; i>=0; i--)
    {
      PWM_Set_Duty(DUTY_CYCLES[i] * pwm_period / 100, 1);
      Delay_ms(200);
      distances[i] = ADC1_Get_Sample(4);
    }
    decide_direction();
  }
}