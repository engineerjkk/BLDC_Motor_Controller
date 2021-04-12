//*********************** SECTION m1 ********************
#include <mega128.h>
#include <stdio.h>
#include <delay.h>
#include "mydef_ver3.h"      
#include "lcd.h"


//*********************** SECTION m2 ********************

u16 rpm; //rpm 측정할 변수 10000rpm정도 세야하니까 4bit는 되어야함.
u08 lcd_mode;
u08 count;
u08 count_rpm;
u16 count_ls;
u16 count_4s;
u32 count_200ms;
u32 count_500ms;
u32 count_stop1;
u32 count_stop2;
bit stop;
bit start_stop;
bit block;
bit start;
bit decelerate; //감속
bit ccw;//역방향, 0이면 정방향, 1이면 역방향
bit cw_ccw;
bit change;
bit connection;//연결 됐는지 여부.
bit overload;//과부하
bit reset;//리셋
bit debug;
u08 freq;//주파수
u08 HS1;//홀센서 변수
u08 HS2;
u08 HS3;
u08 HS4;
u08 total;//홀센서의 총합, 21이 되면 0으로 초기화시킬거임.
u08 memory;
u08 old_H = 0;
u08 new_H = 0;

u08 str_BLDC[] = " JK BLDC";
u08 str_MOTOR[] = " MOTOR  ";
u08 str_title1[] = "Ver1.0: ";
u08 str_title2[] = "20.08.05";
u08 str_design1[] = "DESIGNED";
u08 str_design2[] = " BY KJK ";;
u08 str_keystart[] = "START!! ";
u08 str_keypress[] = "  PRESS ";
u08 str_discon1[] = "-DISCONN";
u08 str_discon2[] = "ECTION--";
u08 str_over[] = "----OVER";
u08 str_load[] = "LOAD----";
u08 str_start1[] = "     STA";
u08 str_start2[] = "RT!!     ";
u08 str_stop1[] = "     STO";
u08 str_stop2[] = "P!!     ";
u08 str_cw[] = " CW";
u08 str_ccw[] = "CCW";
u08 str_rpm[] = "RPM";
u08 str_mA[] = " mA ";

u08 str_sw1[] = "PUSH-SW1";
u08 str_sw2[] = "PUSH-SW2";
u08 str_sw3[] = "PUSH-SW3";
u08 str_sw4[] = "PUSH-SW4";
u08 str_blank[] = "       ";
u08 str_blank1[] = " ";
u08 str_blank2[] = "  ";

void motor_stop(void);
void beep(void);
void step_CW(u08 x);
void step_CCW(u08 x);
void pwm1_control(u08 a);
void RPM_control(u16 y);
void ADC_control(u16 x);
void decelerate_control(void);
void accerlate_control(void);
void impulsion_ccw(void);
void impulsion_cw(void);
//*********************** SECTION m3 ********************


void motor_stop(void)
{
    PORTD.0 = 0;
    PORTD.1 = 0;
    PORTD.2 = 0;
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 0;
}

void beep(void)
{
    BUZ_RLY = BUZ_RLY | BUZZER_ON;
    delay_ms(100);
    BUZ_RLY = BUZ_RLY & BUZZER_OFF;
}

void step_CW(u08 x)//정방향
{
    motor_stop();

    if (HS2 == 2) {        //step1
        PORTD = 0x04;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 6) {   //step2
        PORTD = 0x04;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 1) {    //step3
        PORTD = 0x02;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 3) {   //step4
        PORTD = 0x02;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 5) {   //step5
        PORTD = 0x01;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 4) {   //step6
        PORTD = 0x01;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }

}

void step_CCW(u08 x)
{

    if (HS2 == 2) {      //step1
        PORTD = 0x02;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 6) {  //step2
        PORTD = 0x01;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 1) {   //step3
        PORTD = 0x01;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 3) {  //step4
        PORTD = 0x04;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 5) {  //step5
        PORTD = 0x04;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 4) {  //step6
        PORTD = 0x02;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }

}

void test(u08 x)//정방향
{
    motor_stop();

    if (HS2 == 4) {        //step1
        PORTD = 0x04;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 2) {   //step2
        PORTD = 0x04;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 6) {    //step3
        PORTD = 0x02;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 1) {   //step4
        PORTD = 0x02;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 3) {   //step5
        PORTD = 0x01;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 5) {   //step6
        PORTD = 0x01;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }

}

void pwm1_control(u08 x)
{
    u08 l, m, n, o, p;
    s16 duty_ratio;

    /*l = 0x30+ (x/100);
    m = 0x30+ (x%100)/10;
    n = 0x30+ (x%10);
    lcd_control_write(0x84);
    lcd_data_write(l);
    lcd_data_write(m);
    lcd_data_write(n);*/

    duty_ratio = x * 0.39;           //160/255
    o = 0x30 + duty_ratio / 10;
    p = 0x30 + duty_ratio % 10;
    lcd_control_write(0x84);
    lcd_data_write(o);
    lcd_data_write(p);

    lcd_control_write(0x83);
    lcd_print_data(str_blank1, 1);

    lcd_control_write(0x86);
    lcd_print_data("%", 1);

    lcd_control_write(0x87);
    lcd_print_data(str_blank1, 1);
}



void RPM_control(u16 y)
{
    u16 l, m, n, o, p;
    bit j = 0;


    if (count == 0) {
        lcd_control_write(0xC0);
        lcd_data_write(0x30 + j);
        lcd_data_write(0x30 + j);
        lcd_data_write(0x30 + j);
        lcd_data_write(0x30 + j);
        lcd_data_write(0x30 + j);
    }
    else {
        l = 0x30 + y / 10000;
        m = 0x30 + (y % 10000) / 1000;
        n = 0x30 + (y % 1000) / 100;
        o = 0x30 + ((y % 100)) / 10;
        p = 0x30 + y % 10;
        lcd_control_write(0xC0);
        lcd_data_write(l);
        lcd_data_write(m);
        lcd_data_write(n);
        lcd_data_write(o);
        lcd_data_write(p);
    }
    lcd_control_write(0xC5);
    lcd_print_data(str_rpm, 3);
}


void ADC_control(u16 x)
{
    u16 a, b, c, d;

    d = (x / 1000);
    c = (x % 1000) / 100;
    b = (x % 100) / 10;
    a = x % 10;
    lcd_control_write(0xC0);
    lcd_data_write(0x30 + d);
    lcd_data_write(0x30 + c);
    lcd_data_write(0x30 + b);
    lcd_data_write(0x30 + a);

    /*lcd_control_write(0xC);
    lcd_print_data(str_blank1, 1);*/

    lcd_control_write(0xC4);
    lcd_print_data(str_mA, 4);
}

void decelerate_control(void)
{
    if (count_200ms > 10) { //200_ms가 10개 초과, 즉 2초가 넘으면
        if (count > 20) { //count(속도)가 20개를 넘으면 count는 10씩 감소를 한다. 그리고 2초 타이머는 초기화가되지,
            count = count - 10;
            count_200ms = 1;
        }
        else {
            count = 0;//count(속도)는 0
            cw_ccw = 0;//
        }
    }
}

void accerlate_control(void)
{
    if (count_200ms > 10) {
        if (count < 50) {
            count = 50;
            count_200ms = 1;
        }
        else if (count < 250) {
            count = count + 10;
            count_200ms = 1;
        }
    }
}

void impulsion_cw(void)
{
    {

        OCR1AL = 255;
        OCR1BL = 0;
        OCR1CL = 0;
        PORTD = 0b00000100;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 255;
        OCR1CL = 0;
        PORTD = 0b00000100;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 255;
        OCR1CL = 0;
        PORTD = 0b00000001;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = 255;
        PORTD = 0b00000001;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = 255;
        PORTD = 0b00000010;
        delay_ms(1);

        OCR1AL = 255;
        OCR1BL = 0;
        OCR1CL = 0;
        PORTD = 0b00000010;
        delay_ms(1);

        OCR1AL = 255;
        OCR1BL = 0;
        OCR1CL = 0;
        PORTD = 0b00000100;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 255;
        OCR1CL = 0;
        PORTD = 0b00000100;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 255;
        OCR1CL = 0;
        PORTD = 0b00000001;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = 255;
        PORTD = 0b00000001;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = 255;
        PORTD = 0b00000010;
        delay_ms(1);

        OCR1AL = 255;
        OCR1BL = 0;
        OCR1CL = 0;
        PORTD = 0b00000010;
        delay_ms(1);

        OCR1AL = 255;
        OCR1BL = 0;
        OCR1CL = 0;
        PORTD = 0b00000100;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 255;
        OCR1CL = 0;
        PORTD = 0b00000100;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 255;
        OCR1CL = 0;
        PORTD = 0b00000001;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = 255;
        PORTD = 0b00000001;
        delay_ms(1);
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = 255;
        PORTD = 0b00000010;
        delay_ms(1);
        OCR1AL = 255;
        OCR1BL = 0;
        OCR1CL = 0;
        PORTD = 0b00000010;
        delay_ms(1);
    }


}
void impulsion_ccw(void) {

    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b0000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b00000010;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b0000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b00000010;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b0000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b00000010;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b0000001;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 0;
    OCR1CL = 250;
    PORTD = 0b00000010;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 250;
    OCR1BL = 0;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000100;
    delay_ms(1);
    OCR1AL = 0;
    OCR1BL = 250;
    OCR1CL = 0;
    PORTD = 0b00000001;
    delay_ms(1);

}

//*********************** SECTION m5 ********************
// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
    ADMUX = adc_input | ADC_VREF_TYPE;
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(10);
    // Start the AD conversion
    ADCSRA |= (1 << ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1 << ADIF)) == 0);
    ADCSRA |= (1 << ADIF);
    return ADCW;
}

// Timer 0 overflow interrupt service routine
interrupt[TIM0_OVF] void timer0_ovf_isr(void)//2ms
{
    TCNT0 = 0x53;
    count_rpm++;

}


// Timer2 overflow interrupt service routine
interrupt[TIM2_OVF] void timer2_ovf_isr(void)    //20ms
{
    // Reinitialize Timer2 value
    TCNT2 = 0x28;
    count_ls++;
    count_4s++;
    count_200ms++;
    count_500ms++;
}
interrupt[TIM1_OVF] void timer1_ovf_isr(void)    //20ms
{
    TCNT1 = 0x28;
    impulsion_cw();
}




void main(void)
{

    //*********************** SECTION m2 ******************** 
    u08 i;
    u16 adc, adc2;
    u08 a, b, c, d;
    u08 old_key, new_key;

    //*********************** SECTION m6 ********************
    DDRA = 0x00;
    PORTA = 0x00;      //LCD DATA PORT

    DDRB = 0xf0;       //INPUT & OUTPUT MODE
    PORTB = 0x0f;      //PULL UP ON

    DDRC = 0xff;
    PORTC = 0x00;       //BCD KEY INPUT AND LCD CONTROL PORT   

    DDRD = 0xFF;
    PORTD = 0x00;        //LED DRIVE PORT

    DDRE = 0x00;
    PORTE = 0x00 | (0x03 << 6);

    DDRF = 0x00;
    PORTF = 0xF0;       //BCD KEY INPUT PORT

    DDRG = 0xFF;
    PORTG = 0x00;      //BUZZER AND RELAY CONTROL PORT            



 //-----------------------------------------------        
 // Timer/Counter 0 initialization
 // Clock source: System Clock
 // Clock value: 86.400 kHz
 // Mode: Normal top=0xFF
 // OC0 output: Disconnected
 // Timer Period: 2.0023 ms
    ASSR = 0 << AS0;
    TCCR0 = (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (1 << CS02) | (0 << CS01) | (1 << CS00);
    TCNT0 = TCNT0 = 0x53;
    OCR0 = 0x00;


    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 11059.200 kHz
    // Mode: Ph. correct PWM top=0x00FF
    // OC1A output: Non-Inverted PWM
    // OC1B output: Non-Inverted PWM
    // OC1C output: Non-Inverted PWM
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer Period: 0.046115 ms
    // Output Pulse(s):
    // OC1A Period: 46.115 us Width: 0 us
    // OC1B Period: 46.115 us Width: 0 us
    // OC1C Period: 46.115 us Width: 0 us
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    // Compare C Match Interrupt: Off
    TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0) | (1 << COM1C1) | (0 << COM1C0) | (0 << WGM11) | (1 << WGM10);
    //COM10 = 10 : non-inverting mode  //WGM : 타이머1 8비트 Phase Correct PWM Mode 
    TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10); //분주비 설정
    TCNT1H = 0x00;
    TCNT1L = 0xFF;
    ICR1H = 0x00;
    ICR1L = 0x00;
    OCR1AH = 0x00;
    OCR1AL = 0x00;
    OCR1BH = 0x00;
    OCR1BL = 0x00;
    OCR1CH = 0x00;
    OCR1CL = 0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: 10.800 kHz
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    // Timer Period: 20 ms
    TCCR2 = (0 << WGM20) | (0 << COM21) | (0 << COM20) | (0 << WGM21) | (1 << CS22) | (0 << CS21) | (1 << CS20);
    TCNT2 = 0x28;
    OCR2 = 0x00;


    EICRA == 0x00;
    EICRB = 0x00;
    EIMSK == 0x00;

    EIFR = 0x00 | 0xc0;


    UCSR0B = 0x00;
    UCSR1B == 0x00;

    SPCR == 0x00;
    TWCR == 0x00;
    //*********************** SECTION m7 ********************
    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK = (0 << OCIE2) | (1 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (0 << OCIE0) | (1 << TOIE0);
    ETIMSK = (0 << TICIE3) | (0 << OCIE3A) | (0 << OCIE3B) | (0 << TOIE3) | (0 << OCIE3C) | (0 << OCIE1C);

    // Analog Comparator initialization
    // Analog Comparator: Off
    // Analog Comparator Input Capture by Timer/Counter 1: Off
    ACSR = 0x80;
    SFIOR = 0x00;

    // ADC initialization
    // ADC Clock frequency: 345.600 kHz
    // ADC Voltage Reference: AREF pin
    ADMUX = ADC_VREF_TYPE;
    ADCSRA = (1 << ADEN) | (0 << ADSC) | (0 << ADFR) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
    SFIOR = (0 << ACME);

    //********************************************** 

    lcd_mode = 1;

    lcd_init();
    lcd_clear();
    lcd_home();


    lcd_control_write(0x80);
    lcd_print_data(str_BLDC, 8);

    lcd_control_write(0xC0);
    lcd_print_data(str_MOTOR, 8);

    delay_ms(1000);
    delay_ms(1000);

    lcd_home();
    lcd_clear();

    lcd_control_write(0x80);
    lcd_print_data(str_title1, 8);

    lcd_control_write(0xC0);
    lcd_print_data(str_title2, 8);

    delay_ms(1000);
    delay_ms(1000);

    lcd_home();
    lcd_clear();

    lcd_control_write(0x80);
    lcd_print_data(str_design1, 8);

    lcd_control_write(0xC0);
    lcd_print_data(str_design2, 8);

    delay_ms(1000);
    delay_ms(1000);

    lcd_home();
    lcd_clear();

    lcd_control_write(0x80);
    lcd_print_data(str_keypress, 8);

    lcd_control_write(0xC0);
    lcd_print_data(str_keystart, 8);


    //*********************** SECTION m8 *******************         
    #asm("sei")
        //*********************** SECTION m9 ********************

        connection = 0;
    overload = 0;
    reset = 0;
    stop = 1;
    ccw = 0;
    cw_ccw = 0;
    BUZ_RLY = 0x00;
    old_key = 0;
    HS1 = 0;
    HS2 = 0;
    HS3 = 0;
    HS4 = 0;
    total = 0;
    start = 0;
    change = 0;
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    start_stop = 0;
    block = 0;
    memory = 0;
    decelerate = 0;
    count = 0;
    //*********************** SECTION m10 ******************** 
    while (1)
    {
        /////////////////////MASK//////////////////////////////////                    
        new_key = KEY_IN & KEY_MASK;
        HS1 = PINE & HS_MASK;
        PORTD = HS1;
        HS2 = (HS1 >> 4);
        HS3 = PINE & HS_MASK;
        PORTC.0 = 0;
        //////////////////////START!!///////////////////////////////
        if (new_key == START_STOP) start = 1; //start를 누르면 start=1이 된다. 이때부터 버튼이 활성화가 된다. 
        //////////////////////스위치/////////////////////////////////// 
        if (start == 1) {

            if (new_key != old_key) {

                switch (new_key) {
                case START_STOP:
                    adc2 = read_adc(0) * 6;// beep();
                    if (stop == 1) { stop = 0; start_stop = 1; block = 0; }
                    else { stop = 1; start_stop = 0; block = 1; }//block이 1이되면 stop이므로, 다른버튼이 먹히지 않게 할라고,

                    break;
                case CW_CCW:
                    adc2 = read_adc(0) * 6;
                    //beep(); //그렇지만 이건 블럭에서 독립적이라 이 버튼은 먹힘.
                    if (ccw == 0) { ccw = 1; cw_ccw = 1; decelerate = 1; }//정방향이었으면 역방향1, cw_ccw버튼 활성화, 감속 활성화
                    else { ccw = 0; cw_ccw = 1; decelerate = 1; }//역방향이었으면, 역방향0=정방향, cw_ccw버튼 활성화, 감속 활성화.

                    break;
                case DUTY_UP:  if (block == 0) {//Start상태인 block이 0일때에만
                    adc2 = read_adc(0) * 6;


                    if (start == 1) {

                        if (count < 30) {
                            count = 30;
                            if (ccw == 0) {
                                accerlate_control();
                                step_CW(count);


                            }
                            else
                                accerlate_control();
                            step_CCW(count);

                        }

                        else if
                            (count >= 255) {
                            count = 255;
                        }
                        else {
                            count = count + 5;
                        }
                    }
                    break;

                }

                            break;
                case DUTY_DOWN: if (block == 0) {// start상태인 block이 0일때만 ducy가 작용된다.
                    if ((count < 50)) count = 50;
                    else if (start == 1) {
                        count = count - 5;
                    }
                    adc2 = read_adc(0) * 6;
                }

                              break;
                }
                //adc2에는 버튼을 누를때마다 저장을 한다.
            }
            old_key = new_key;

            /////////////////////DISCONNECTION////////////////////// 
            if (HS2 < 1)//홀센서에 아무것도 입력이 되지 않으면 연결되지 않음
                connection = 0;
            else
                connection = 1;//홀센서값이 하나라도 입력이 되면 연결됨.

            ////////////////////////////////////////////////////////   
            if (connection == 0) {//연결이 되지 않았을경우.

                lcd_control_write(0x80);
                lcd_print_data(str_discon1, 8);

                lcd_control_write(0xC0);
                lcd_print_data(str_discon2, 8);

                //beep();
                delay_ms(500);

                reset = 1;//리셋이 된다. 다시 시작을 하는거지.
                ///////////////////////OVERLOAD//////////////////////        
            }
            else {

                if (overload == 1) {//과부화가 걸린경우에는,

                    lcd_control_write(0x80);
                    lcd_print_data(str_over, 8);

                    lcd_control_write(0xC0);
                    lcd_print_data(str_load, 8);

                    // beep();
                    delay_ms(500);//딜레이 주고,

                    if ((count_ls > 50) & (b == 0)) { //count_ls당 20ms니까, 1초가 되면 초기화,
                        b = 1;
                        count_ls = 1;
                    }
                    else if ((count_ls > 50) & (b == 1)) {
                        b = 0;
                        count_ls = 1;

                        adc = 0;
                        reset = 1;        //1 SEC PERIOD

                    }

                }
                else {// 과부하가 걸리지 않은경우 >> 정상적으로 동작을 한다.
                    if (reset == 1) {
                        count = 0;
                        reset = 0;
                    }
                    //////////////////////soft stop///////////////////////////////////      
                    if (stop == 1) { //스탑 상태인데
                        if (start_stop == 0) { // 버튼을 누르지 않았어
                            if (ccw == 0) step_CW(count);//정방향이면 그냥 정방향으로 돌아
                            else step_CCW(count);//역방향이면 그대로 역방향 돌아.
                        }
                        else {

                            decelerate_control();

                            if (ccw == 0) {
                                lcd_control_write(0x80);
                                lcd_print_data(str_cw, 3);

                                step_CW(count);
                            }
                            else {
                                lcd_control_write(0x80);
                                lcd_print_data(str_ccw, 3);

                                step_CCW(count);
                            }
                        }
                    }
                    //////////////6 step/////////////////////////

                    else {
                        if (start_stop == 1) {
                            new_H = HS2;
                            if (ccw == 0) step_CW(count);
                            else step_CCW(count);
                            if (new_H == old_H)
                            {
                                test(count);
                            }
                            old_H = new_H;
                        }
                        else {
                            if (ccw == 0) {         //CLOCKWISE
                                if (decelerate == 1) {
                                    if (cw_ccw == 1) {

                                        if (a == 0) { memory = count; a = 1; }

                                        decelerate_control();

                                        lcd_control_write(0x80);
                                        lcd_print_data(str_ccw, 3);

                                        step_CCW(count);
                                    }


                                    else {
                                        if (count != memory) {

                                            accerlate_control();

                                            lcd_control_write(0x80);
                                            lcd_print_data(str_cw, 3);

                                            step_CW(count);
                                        }

                                        else {
                                            decelerate = 0;
                                            a = 0;
                                        }
                                    }

                                }
                                else {
                                    lcd_control_write(0x80);
                                    lcd_print_data(str_cw, 3);

                                    step_CW(count);
                                }
                            }

                            else {
                                if (decelerate == 1) {
                                    if (cw_ccw == 1) {

                                        if (a == 0) { memory = count; a = 1; }

                                        decelerate_control();

                                        lcd_control_write(0x80);
                                        lcd_print_data(str_cw, 3);

                                        step_CW(count);
                                    }

                                    else {
                                        if (count != memory) {
                                            accerlate_control();

                                            lcd_control_write(0x80);
                                            lcd_print_data(str_ccw, 3);

                                            step_CCW(count);
                                        }

                                        else {
                                            decelerate = 0;
                                            a = 0;
                                        }
                                    }

                                }
                                else {
                                    lcd_control_write(0x80);
                                    lcd_print_data(str_ccw, 3);

                                    step_CCW(count);
                                }
                            }
                        }
                    }
                    ////////////SPEED///////////////////////

                    if (HS4 != (HS3 >> 4)) {
                        HS4 = (HS3 >> 4);
                        total = total + HS4;
                    }

                    if (total >= 21) {
                        rpm = 30000 / count_rpm;
                        count_rpm = 0;
                        total = 0;
                    }

                    //////////////////ADC///////////////////////                 
                    adc = read_adc(0) * 6;
                    //////////////////DISPLAY///////////////////

                    //////////////RPM&ADC PERIOD///////////////    
                    if ((count_4s > 200) & (b == 0)) {   //동작하든말든  무조건 4초마다 왔다갔다한다.

                        b = 1;
                        change = 1;
                        count_4s = 1;
                    }
                    else if ((count_4s > 200) & (b == 1)) {

                        b = 0;
                        change = 0;
                        count_4s = 1;        //4SEC PERIOD
                    }
                    //////////////START&STOP PERIOD///////////////      
                    if ((stop == 1) & (start_stop == 0)) {//0.5초 동안 LCD에 stop을 표시해주는 프로그램.
                        if (c == 0) { count_500ms = 0; c = 1; }
                        else {
                            if (count_500ms < 25) {//0.5초 동안 LCD에 stop을 표시만함

                                lcd_control_write(0x80);
                                lcd_print_data(str_stop1, 8);

                                lcd_control_write(0xC0);
                                lcd_print_data(str_stop2, 8);
                            }
                            else {
                                //0.5초가 지나면 이제 진짜 stop.
                                start_stop = 1;
                                c = 0;
                            }
                        }
                    }
                    else if ((stop == 1) & (start_stop == 1))//멈춰있는동안에 ADC와 RPM을 번갈아 출력 그대로 유지해주기위햇.
                    {
                        pwm1_control(count);

                        if (change == 0) {

                            ADC_control(adc);
                        } //change 가 0이면 ADC를 lcd에 표출.
                        else {

                            RPM_control(rpm);//change가 1이면 rpm을 LCD에 표출
                        }
                    }

                    else if ((stop == 0) & (start_stop == 1)) {////0.5간 start를 출력해주는 프로그램.
                        if (d == 0) { count_500ms = 0; d = 1; }
                        else {
                            if (count_500ms < 25) {

                                lcd_control_write(0x80);
                                lcd_print_data(str_start1, 8);

                                lcd_control_write(0xC0);
                                lcd_print_data(str_start2, 8);
                            }
                            else {

                                start_stop = 0;
                                d = 0;
                            }
                        }
                    }
                    else {//start가 활성화된, 돌아가고있을동안 4초간 왔다갔다하면서 번갈아 출력해줌.
                        pwm1_control(count);
                        if (change == 0) {

                            ADC_control(adc);
                        }
                        else
                        {

                            RPM_control(rpm);
                        }
                    }
                }
                /////////////////////OVERLOAD////////////////////////////// 
                if (count >= 200) {
                    if (((adc != 0) & (adc2 != 0) & (adc > (1.5 * adc2))) | (adc >= 15000)) // adc(실시간 값)보다 저장된 값(adc2)의 1.5배보다 커지면 overload.
                        overload = 1;
                    else overload = 0;
                }
            }

        }
    }
}