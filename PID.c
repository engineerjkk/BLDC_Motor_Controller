//*********************** SECTION m1 ********************
#include <mega128.h>
#include <stdio.h>
#include <delay.h>
#include "mydef_ver3.h"      
#include "lcd.h"


//*********************** SECTION m2 ********************
d08 rpm_t;
d08 rpm_c;
d08 P_error;
d08 I_error;
d08 PI_val;
u08 lcd_mode;
d08 count;
u32 count_rpm;
u32 count_2s;
u32 count_4s;
u32 count_200ms;
u32 count_300ms;
u16 adc;
bit stop;;
bit start;
bit decelerate;
bit ccw;
bit cw_ccw;
bit change;
bit connection;
bit overload;
bit reset;
bit mode;
bit storage;
bit rpm_adc;
u08 freq;
u08 HS1;
u08 HS2;
u08 HS3;
u08 total;
d08 memory;
d08 KP;
d08 KI;
d08 KA;
d08 dt;


u08 str_BLDC[] = " 12V BLD";
u08 str_MOTOR[] = "C MOTOR ";
u08 str_CONTROLLER1[] = "  PI CON";
u08 str_CONTROLLER2[] = "TROLLER ";
u08 str_title1[] = "FIRMWARE";
u08 str_title2[] = " VERSION";
u08 str_title3[] = "Ver1.0: ";
u08 str_title4[] = "20.08.02";
u08 str_design1[] = "DESIGNED";
u08 str_design2[] = " BY     ";
u08 str_design3[] = "    Kang ";
u08 str_design4[] = "June Koo";
u08 str_target[] = " TARGET ";
u08 str_rpm[] = "RPM ";
u08 str_for[] = " [FOR]  ";
u08 str_rev[] = " [REV]  ";
u08 str_err[] = "ERROR : ";
u08 str_overload[] = "OVERLOAD";
u08 str_stop1[] = "STOP ROT";
u08 str_stop2[] = "ATION   ";
u08 str_discon1[] = "DISCONNE";
u08 str_discon2[] = "CTION   ";
u08 str_blank[] = "        ";
u08 str_keystart[] = "START!! ";
u08 str_keypress[] = "  PRESS ";
u08 str_mA[] = " mA ";
u08 str_C[] = "C ";
u08 str_T[] = "T ";
u08 str_blank1[] = " ";

void motor_stop(void);
void beep(void);
void step_CW(d08 x);
void step_CCW(d08 x);
void pwm1_control(u08 a);
void RPM_control(void);
void ADC_control(u16 x);
void decelerate_control(void);
void accerlate_control(void);

//*********************** SECTION m3 *******************

void beep(void)
{
    BUZ_RLY = BUZ_RLY | BUZZER_ON;
    delay_ms(100);
    BUZ_RLY = BUZ_RLY & BUZZER_OFF;
}

void step_CW(d08 x)
{


    if (HS2 == 6) {        //step1
        PORTD.0 = 0;
        PORTD.1 = 0;
        PORTD.2 = 1;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 4) {   //step2
        PORTD.0 = 0;
        PORTD.1 = 0;
        PORTD.2 = 1;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 5) {    //step3
        PORTD.0 = 0;
        PORTD.1 = 1;
        PORTD.2 = 0;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 1) {   //step4
        PORTD.0 = 0;
        PORTD.1 = 1;
        PORTD.2 = 0;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 3) {   //step5
        PORTD.0 = 1;
        PORTD.1 = 0;
        PORTD.2 = 0;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 2) {   //step6
        PORTD.0 = 1;
        PORTD.1 = 0;
        PORTD.2 = 0;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
}

void step_CCW(d08 x)
{


    if (HS2 == 6) {      //step1
        PORTD.0 = 0;
        PORTD.1 = 1;
        PORTD.2 = 0;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 4) {  //step2
        PORTD.0 = 1;
        PORTD.1 = 0;
        PORTD.2 = 0;
        OCR1AL = 0;
        OCR1BL = 0;
        OCR1CL = x;
    }
    else if (HS2 == 5) {   //step3
        PORTD.0 = 1;
        PORTD.1 = 0;
        PORTD.2 = 0;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 1) {  //step4
        PORTD.0 = 0;
        PORTD.1 = 0;
        PORTD.2 = 1;
        OCR1AL = 0;
        OCR1BL = x;
        OCR1CL = 0;
    }
    else if (HS2 == 3) {  //step5
        PORTD.0 = 0;
        PORTD.1 = 0;
        PORTD.2 = 1;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
    else if (HS2 == 2) {  //step6
        PORTD.0 = 0;
        PORTD.1 = 1;
        PORTD.2 = 0;
        OCR1AL = x;
        OCR1BL = 0;
        OCR1CL = 0;
    }
}


void pwm1_control(u08 x)
{
    u08 l, m, n, o, p;
    s16 duty_ratio;

    l = 0x30 + (x / 100);
    m = 0x30 + (x % 100) / 10;
    n = 0x30 + (x % 10);
    lcd_control_write(0x84);
    lcd_data_write(l);
    lcd_data_write(m);
    lcd_data_write(n);

    /* duty_ratio = x*0.83;           //100/120
     o = 0x30+ duty_ratio/10;
     p = 0x30+ duty_ratio%10;
     lcd_control_write(0x84);
     lcd_data_write(o);
     lcd_data_write(p); */

    lcd_control_write(0x83);
    lcd_print_data(str_blank1, 1);

    //lcd_control_write(0x86);          
    //lcd_print_data("%",1); 

    lcd_control_write(0x87);
    lcd_print_data(str_blank1, 1);
}



void RPM_control_C(void)
{
    u16 l, m, n, o;

    l = 0x30 + ((int)rpm_c) / 1000;
    m = 0x30 + (((int)rpm_c) / 100) % 10;
    n = 0x30 + ((((int)rpm_c) / 10) % 100) % 10;
    o = 0x30 + ((int)rpm_c) % 10;
    lcd_control_write(0xC8);
    lcd_data_write(l);
    lcd_data_write(m);
    lcd_data_write(n);
    lcd_data_write(o);

    lcd_control_write(0xCC);
    lcd_print_data(str_rpm, 4);

}

void RPM_control_T(void)
{
    u16 l, m, n, o;

    l = 0x30 + ((int)rpm_t) / 1000;
    m = 0x30 + (((int)rpm_t) / 100) % 10;
    n = 0x30 + ((((int)rpm_t) / 10) % 100) % 10;
    o = 0x30 + ((int)rpm_t) % 10;
    lcd_control_write(0x88);
    lcd_data_write(l);
    lcd_data_write(m);
    lcd_data_write(n);
    lcd_data_write(o);

    lcd_control_write(0x80);
    lcd_print_data(str_target, 8);

    lcd_control_write(0x8C);
    lcd_print_data(str_rpm, 4);


}

void ADC_control(u16 x)
{
    u16 a, b, c;

    c = x / 100;
    b = (x % 100) / 10;
    a = x % 10;
    lcd_control_write(0xC8);
    lcd_data_write(0x30 + 0);
    lcd_data_write(0x30 + c);
    lcd_data_write(0x30 + b);
    lcd_data_write(0x30 + a);

    lcd_control_write(0xCC);
    lcd_print_data(str_mA, 4);
}

void decelerate_control(void)
{
    if (count < 10) count = 0;
    else count = count - 0.02;

    if (ccw == 0) step_CW(count);
    else step_CCW(count);
}

void accerlate_control(void)
{
    if (count < 10) count = 10;
    else count = count + 0.02;

    if (ccw == 0) step_CW(count);
    else step_CCW(count);

}

void PI_control(void)
{
    dt = 0.001;  //초당 1000번 처리(anti-windup)

    KP = 0.000012;
    KI = 0.00000001;

    P_error = rpm_c - rpm_t;
    I_error += P_error * dt;

    PI_val = P_error * KP + I_error * KI;

    if (PI_val > 250) PI_val = 250;
    else if (PI_val < -250) PI_val = -250;



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
interrupt[TIM0_OVF] void timer0_ovf_isr(void) //1ms
{
    // Reinitialize Timer 0 value
    TCNT0 = 0x53;
    count_300ms++;
    count_2s++;
    count_4s++;
    count_200ms++;

    if (connection == 1) {
        if (overload == 0) {
            if (stop == 1) {

                decelerate_control();

            }
            //////////////6 step/////////////////////////

            else {

                if (ccw == 0) {         //CLOCKWISE 
                    if (decelerate == 1) {
                        if (cw_ccw == 1) {

                            if (storage == 0) { memory = count; storage = 1; }

                            decelerate_control();
                            step_CCW(count);

                            if (count == 0) cw_ccw = 0;
                        }


                        else {
                            if (count < memory) {

                                accerlate_control();
                                step_CW(count);
                            }

                            else {
                                decelerate = 0;
                                storage = 0;
                            }
                        }

                    }
                    else {
                        if (count < 20) count = 20;
                        else {

                            PI_control();

                            count = count - PI_val;
                            step_CW(count);

                        }
                    }
                }

                else {
                    if (decelerate == 1) {
                        if (cw_ccw == 1) {

                            if (storage == 0) { memory = count; storage = 1; }

                            decelerate_control();
                            step_CW(count);

                            if (count == 0) cw_ccw = 0;
                        }

                        else {
                            if (count < memory) {

                                accerlate_control();
                                step_CCW(count);
                            }

                            else {
                                decelerate = 0;
                                storage = 0;
                            }
                        }

                    }
                    else {
                        if (count < 20) count = 20;
                        else {
                            PI_control();

                            count = count - PI_val;

                            step_CCW(count);

                        }
                    }
                }

            }
            ///////////////////ADC////////////////////////////
            if (rpm_c < 4000) adc = 6 * read_adc(0);
            else adc = 5 * read_adc(0);
        }
    }

}

// Timer2 overflow interrupt service routine
interrupt[TIM2_OVF] void timer2_ovf_isr(void)
{
    // Reinitialize Timer2 value
    TCNT2 = 0x91;
    count_rpm++;

}





void main(void)
{

    //*********************** SECTION m2 ******************** 
    u08 i;
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
 // Clock value: 172.800 kHz
 // Mode: Normal top=0xFF
 // OC0 output: Disconnected
 // Timer Period: 1.0012 ms
    ASSR = 0 << AS0;
    TCCR0 = (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (1 << CS02) | (0 << CS01) | (0 << CS00);
    TCNT0 = 0x53;
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
    TCNT1L = 0x00;
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
    // Clock value: 11059.200 kHz
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    // Timer Period: 0.010037 ms
    TCCR2 = (0 << WGM20) | (0 << COM21) | (0 << COM20) | (0 << WGM21) | (0 << CS22) | (0 << CS21) | (1 << CS20);
    TCNT2 = 0x91;
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
    // ADC Clock frequency: 691.188 kHz
    // ADC Voltage Reference: AREF pin
    ADMUX = ADC_VREF_TYPE;
    ADCSRA = (1 << ADEN) | (0 << ADSC) | (0 << ADFR) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
    SFIOR = (0 << ACME);

    //********************************************** 

    lcd_mode = 1;

    lcd_init();
    lcd_clear();
    lcd_home();


    lcd_control_write(0x80);
    lcd_print_data(str_BLDC, 8);

    lcd_control_write(0x88);
    lcd_print_data(str_MOTOR, 8);

    delay_ms(1000);

    lcd_control_write(0xC0);
    lcd_print_data(str_CONTROLLER1, 8);

    lcd_control_write(0xC8);
    lcd_print_data(str_CONTROLLER2, 8);

    delay_ms(3000);

    lcd_home();
    lcd_clear();

    lcd_control_write(0x80);
    lcd_print_data(str_title1, 8);

    lcd_control_write(0x88);
    lcd_print_data(str_title2, 8);

    delay_ms(1000);

    lcd_control_write(0xC0);
    lcd_print_data(str_title3, 8);

    lcd_control_write(0xC8);
    lcd_print_data(str_title4, 8);

    delay_ms(3000);

    lcd_home();
    lcd_clear();

    lcd_control_write(0x80);
    lcd_print_data(str_design1, 8);

    lcd_control_write(0x88);
    lcd_print_data(str_design2, 8);

    delay_ms(1000);

    lcd_control_write(0xC0);
    lcd_print_data(str_design3, 8);

    lcd_control_write(0xC8);
    lcd_print_data(str_design4, 8);

    delay_ms(3000);

    lcd_home();
    lcd_clear();

    lcd_control_write(0x80);
    lcd_print_data(str_keypress, 8);

    lcd_control_write(0x88);
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
    total = 0;
    start = 0;
    change = 0;
    storage = 0;
    rpm_adc = 0;
    count = 20;
    memory = 0;
    decelerate = 0;
    rpm_c = 0;
    rpm_t = 1000;
    mode = 0;
    PI_val = 0;
    //*********************** SECTION m10 ******************** 
    while (1)
    {
        /////////////////////MASK//////////////////////////////////                 
        new_key = KEY_IN & KEY_MASK;
        HS1 = PINE & HS_MASK;
        HS3 = PINE & HS_MASK;
        //////////////////////START!!///////////////////////////////
        if (new_key == START_STOP) start = 1;
        //////////////////////스위치/////////////////////////////////// 
        if (start == 1) {
            if (new_key != old_key) {
                switch (new_key) {
                case START_STOP:
                    if (stop == 1) { stop = 0; beep(); }
                    else stop = 1;

                    break;
                case CW_CCW:
                    if (ccw == 0) { ccw = 1; decelerate = 1; cw_ccw = 1; }
                    else { ccw = 0; decelerate = 1; cw_ccw = 1; }

                    break;
                case UP:
                    if (rpm_t >= 6000) rpm_t = 6000;
                    else rpm_t = rpm_t + 200;


                    break;
                case DOWN:
                    if (rpm_t <= 1000) rpm_t = 1000;
                    else rpm_t = rpm_t - 200;


                    break;
                }
            }

            old_key = new_key;


            /////////////////////DISCONNECTION////////////////////// 
            if (HS1 < 7)
                connection = 0;
            else
                connection = 1;

            ////////////////////////////////////////////////////////   
            if (connection == 0) {

                lcd_control_write(0x80);
                lcd_print_data(str_err, 8);

                lcd_control_write(0x88);
                lcd_print_data(str_blank, 8);

                lcd_control_write(0xC0);
                lcd_print_data(str_discon1, 8);

                lcd_control_write(0xC8);
                lcd_print_data(str_discon2, 8);

                beep();
                delay_ms(500);

                reset = 1;
            }
            else {
                ////////////////overload//////////////////////////// 
                if ((adc != 0) & (adc > 350)) overload = 1;
                else overload = 0;

                if (overload == 1) {

                    lcd_control_write(0x80);
                    lcd_print_data(str_err, 8);

                    lcd_control_write(0x88);
                    lcd_print_data(str_overload, 8);

                    lcd_control_write(0xC0);
                    lcd_print_data(str_stop1, 8);

                    lcd_control_write(0xC8);
                    lcd_print_data(str_stop2, 8);

                    beep();
                    delay_ms(2000);

                    adc = 0;
                    reset = 1;    //1 SEC PERIOD   
                }
                else {
                    if (reset == 1) {
                        count = 0;
                        reset = 0;
                        stop = 1;
                    }

                    ////////////////////CW///////////////////////////////////////////
                    if (ccw == 0) {
                        lcd_control_write(0xC0);
                        lcd_print_data(str_for, 8);
                    }
                    else {
                        lcd_control_write(0xC0);
                        lcd_print_data(str_rev, 8);
                    }

                    //////////////////SPEED/////////////////////////////// 

                    if (count <= 15) rpm_c = 0;
                    else {
                        if (HS2 != (HS1 >> 3)) {
                            HS2 = (HS3 >> 3);
                            total = total + HS2;
                        }

                        if (total >= 21) {
                            rpm_c = 4000000 / count_rpm;
                            count_rpm = 0;
                            total = 0;
                        }
                    }

                    //////////////RPM&ADC PERIOD///////////////  

                    if ((count_4s > 4000) & (rpm_adc == 0)) {

                        change = 1;
                        count_4s = 0;

                        rpm_adc = 1;
                    }
                    else if ((count_4s > 4000) & (rpm_adc == 1)) {

                        change = 0;
                        count_4s = 0;

                        rpm_adc = 0;      //4SEC PERIOD
                    }
                    //////////////////////RPM&ADC DISPLAY//////////     
                    if (count_300ms > 300) {
                        RPM_control_T();

                        if (change == 0) RPM_control_C();
                        else ADC_control(adc);

                        count_300ms = 0;
                    }

                }
            }
        }
    }
}

//*********************** SECTION m13 ********************