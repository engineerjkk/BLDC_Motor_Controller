#ifndef      __def__h
#define      __def__h

#include <delay.h> 
//*********************** SECTION h1 ********************
typedef unsigned char      u08;
typedef          char      s08;
typedef unsigned short      u16;
typedef          short      s16;
typedef unsigned long      u32;
typedef          long      s32;

//*********************** SECTION h2 ********************
#define ON      0x01
#define OFF      0x00

// LED DEFINE
#define LED8      PORTD.7 =0
#define LED7      PORTD.6   =0
#define LED6      PORTD.5   =0
#define LED5      PORTD.4   =0
#define LED4      PORTD.3   =0
#define LED3      PORTD.2   =0
#define LED2      PORTD.1   =0
#define LED1      PORTD.0   =0

#define DEBUG      PORTG.4

#define BUZ_RLY      PORTG         
#define   BUZZER_ON   0x08
#define BUZZER_OFF   0x07
#define RELAY_ON    0x01
#define RELAY_OFF   0x0E

#define KEY_IN      PINB
#define KEY_MASK   0x0F
#define HS_MASK     0b01110000
// Key Definition
#define START_STOP      0xE    //SW1 
#define CW_CCW          0xD    //SW2
#define DUTY_UP         0xB    //SW3  
#define DUTY_DOWN       0x7    //SW4 

//BCD KEY INPUT
#define BCD1        0xF0  
#define BCD2        0x0F


#define LCD_RS_0   PORTC.7 = 0
#define LCD_RS_1   PORTC.7 = 1
#define LCD_RW_0   PORTC.5 = 0
#define LCD_RW_1   PORTC.5 = 1
#define LCD_E_0      PORTC.6 = 0
#define LCD_E_1      PORTC.6 = 1
#define LCD_DELAY   delay_us(2)

#define LCD_DATA_DDR      DDRA
#define LCD_DATA_POUT      PORTA
#define LCD_DATA_PIN      PINA

//*********************** SECTION h3 ********************
void beep(void);
void RELAY(u08 x);

#endif