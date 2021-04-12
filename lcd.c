/*************************************************************/
/*                 LCD CONTROL  ROUTINE                      */
/*************************************************************/
#include <mega128.h>
#include "mydef_ver3.h"
#include "lcd.h"

void lcd_init_HW(void)
{
    // initialize LCD control lines
    LCD_RS_0;
    LCD_RW_0;
    LCD_E_0;
    // initialize LCD control lines to output
    LCD_RS_1;
    LCD_RW_1;
    LCD_E_1;
    // initialize LCD data port to input
    // initialize LCD data lines to pull-up
    LCD_DATA_POUT = 0xFF;
    LCD_DATA_DDR = 0x00;
}

void lcd_busy_wait(void)
{
    LCD_RS_0;                // set RS to "control"
    LCD_DATA_DDR = 0x00;     // set data I/O lines to input (8bit)
    LCD_DATA_POUT = 0xFF;    // set pull-ups to on (8bit)
    LCD_RW_1;                // set R/W to "read"
    LCD_E_1;                 // set "E" line
    LCD_DELAY;               // wait

    while (LCD_DATA_PIN & (1 << LCD_BUSY))
    {

        LCD_RW_0;     // clear "E" line
        LCD_DELAY;   // wait
        LCD_DELAY;   // wait
        LCD_RW_1;     // set "E" line
        LCD_DELAY;   // wait
        LCD_DELAY;   // wait
    }
    LCD_RW_0;         // clear "E" line
}

void lcd_control_write(u08 data)
{
    lcd_busy_wait();     // wait until LCD not busy
    LCD_RS_0;            // set RS to "control"
    LCD_E_0;            // set R/W to "write"
    LCD_RW_1;             // set "E" line
    LCD_DATA_DDR = 0xFF; // set data I/O lines to output (8bit)
    LCD_DATA_POUT = data;// output data, 8bits
    LCD_DELAY;           // wait
    LCD_DELAY;           // wait
    LCD_RW_0;             // clear "E" line
    LCD_DATA_DDR = 0x00; // set data I/O lines to input (8bit)
    LCD_DATA_POUT = 0xFF;// set pull-ups to on (8bit)
}

u08 lcd_control_read(void)
{
    u08 data;
    lcd_busy_wait();      // wait until LCD not busy
    LCD_DATA_DDR = 0x00;  // set data I/O lines to input (8bit)
    LCD_DATA_POUT = 0xFF; // set pull-ups to on (8bit)
    LCD_RS_0;             // set RS to "control"
    LCD_E_1;             // set R/W to "read"
    LCD_RW_1;              // set "E" line
    LCD_DELAY;            // wait
    LCD_DELAY;            // wait
    data = LCD_DATA_PIN;  // input data, 8bits
    LCD_RW_0;              // clear "E" line
    return data;
}

void lcd_data_write(u08 data)
{
    lcd_busy_wait();            // wait until LCD not busy
    LCD_RS_1;                   // set RS to "data"
    LCD_E_0;                   // set R/W to "write"
    LCD_RW_1;                    // set "E" line
    LCD_DATA_DDR = 0xFF;        // set data I/O lines to output (8bit)
    LCD_DATA_POUT = data;       // output data, 8bits
    LCD_DELAY;                  // wait
    LCD_DELAY;                  // wait
    LCD_RW_0;                    // clear "E" line
    LCD_DATA_DDR = 0x00;        // set data I/O lines to input (8bit)
    LCD_DATA_POUT = 0xFF;       // set pull-ups to on (8bit)
}

/*************************************************************/
/********************* PUBLIC FUNCTIONS **********************/
/*************************************************************/

void lcd_init(void)
{
    //    lcd_init_HW();
    delay_ms(100);

    // LCD function set
    lcd_control_write(0x3A);
    delay_ms(60);

    // clear LCD
    lcd_control_write(0x01);
    delay_ms(60);   // wait 60ms

    // set entry mode
    lcd_control_write(0x0C);
    delay_ms(20);

    // move cursor to home
    lcd_control_write(0x02);
    delay_ms(20);

    // set data address to 0
    lcd_control_write(0x00);

}

void lcd_home(void)
{
    // move cursor to home
    lcd_control_write(0x02);
}

void lcd_clear(void)
{
    // clear LCD
    lcd_control_write(0x01);
}


void lcd_print_data(u08* data, u08 n_bytes)
{
    u08 i;
    // print data
    for (i = 0; i < n_bytes; i++)
    {
        lcd_data_write(data[i]);
    }
}