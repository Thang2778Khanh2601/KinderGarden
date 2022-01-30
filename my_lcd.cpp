#include <QDebug>
#include <QCoreApplication>
#include <iostream>
#include <QThread>

#include "rgb_lcd.h"

#include "my_lcd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/
// initialize hardware
void my_lcd_init(){
    qDebug() <<"lcd init\n";
    lcd_begin(16, 2, LCD_5x8DOTS);
    setCursor(0, 0);
    // Print a message to the lcd_
    lcd_write_string("Loading...");
//    QThread::sleep(30);
    setCursor(0, 0);
    lcd_write_string("Enter key:");
}

//set cursor
void my_lcd_set_cursor(int x, int y){
    setCursor(x, y);
}

//write char at current cursor
void my_lcd_write_char(char c){
    lcd_write(c);
}

//update display
void my_lcd_update_dipslay(){
}

//clear display
void my_lcd_clear_line(char line){
    setCursor(0, line);
    lcd_write_string("                ");
}

