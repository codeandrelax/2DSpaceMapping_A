#define PWM_FREQ 50
#define BAUD_RATE 9600
#define RADAR_RESOLUTION 20
#define DIST_STR_LEN 5
#define ANGLE_STR_LEN 5

#define IR_PIN F2
#define SERVO_PIN F7
#define UART_RX_PIN F10
#define UART_TX_PIN F11

// RPOR values : 3 - U1TX, 18 - OC1 (PWM)
// RPINR18 - U1RX (0x1F0D)

const double SERVO_MIN_DUTY = 2.5;
const double SERVO_MAX_DUTY = 12.5;
const double DELTA_V = 3.3 / 1024.0;
const double DUTY_CYCLE_INCREMENT = (SERVO_MAX_DUTY - SERVO_MIN_DUTY) / ((double)RADAR_RESOLUTION);
const int MESSAGE_LEN = (DIST_STR_LEN + ANGLE_STR_LEN + 1) * (RADAR_RESOLUTION + 1) + 1;

void distance_to_str(double, char*);

void main() 
{
  unsigned pwm_period = 0;
  unsigned i = 0;
  double duty_cycle = SERVO_MIN_DUTY;
  double adc_value = 0.0;
  double duty_increment = 0.5;
  char message[MESSAGE_LEN];
  char distance_str[15];
  char angle_str[15];
  // Initialize pin configurations
  AD1PCFGL = 0xFFEF;
  TRISB = 0;
  LATB = 0;
  TRISB.IR_PIN = 1;
  TRISB.UART_RX_PIN = 1;
  ODCB.SERVO_PIN = 1;
  // Configure remappable registers
  RPOR3 = 0x1200;
  RPOR5 = 0x0300;
  RPINR18 = 0x1F0A;
  // Initialize ADC
  ADC1_Init();
  // Initialize PWM
  pwm_period = PWM_Init(PWM_FREQ, 1, 0, 2);
  PWM_Start(1);
  // Initialze UART
  UART1_Init(BAUD_RATE);
  Delay_ms(100);
  FloatToStr(DELTA_V, distance_str);
  UART1_Write_Text("Voltage chunk: ");
  UART1_Write_Text(distance_str);
  UART1_Write('\n');
  while(1)
  {
    i = 0;
    memset(message, '\0', MESSAGE_LEN);
    for(;i<=RADAR_RESOLUTION;i++)
    {
      PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
      Delay_ms(300);
      adc_value = ADC1_Get_Sample(4) * DELTA_V;
      distance_to_str(adc_value, distance_str);
      //if(strstr(distance_str, "e") != 0)
      //  strcpy(distance_str, "0.00");
      //distance_str[5] = '\0';
      FloatToStr(duty_cycle, angle_str);
      //angle_str[5] = '\0';
      //if(i != 0)
        //strcat(message, ",");
      //strcat(message, distance_str);
      //strcat(message, ",");
      //strcat(message, angle_str);
      UART1_Write_Text("Distance: ");
      UART1_Write_Text(distance_str);
      UART1_Write_Text("; Duty: ");
      UART1_Write_Text(angle_str);
      UART1_Write('\n');
      duty_cycle += duty_increment;
    }
    //message[MESSAGE_LEN - 1] = '\0';
    //UART1_Write_Text(message);
    //UART1_Write('\n');
    duty_increment *= (-1.0);
  }
}

void distance_to_str(double value, char* res)
{
  int stripped = (int)(value * 1000.0);
  res[0] = 0x30 + (int)(stripped / 1000);
  res[1] = '.';
  res[2] = 0x30 + (int)((stripped % 1000) / 100);
  res[3] = 0x30 + (int)((stripped % 100) / 10);
  res[4] = 0x30 + (int)(stripped % 10);
  res[5] = '\0';
}