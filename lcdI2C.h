/*
 * lcdI2C.h
 *
 *  Created on: 3/06/2021
 *      Author: omar
 * RS------>P0
 * RW------>P1
 * E------->P2
 * A------->P3
 * D4/D0------>P4
 * D5/D1------>P5
 * D6/D2------>P6
 * D7/D3------>P7
 * DATOS DE I2C
 * D7-D6-D5-D4-A-E-RW-RS
 * MSB---------------LSB
 */

#ifndef LCDI2C_H_
#define LCDI2C_H_
#include <stdint.h>

#define CLEAR   0x01 //limpia la pantalla
#define RETURN  0x02//Retorna al inicio
#define ENTRY_MODE_SET 0x04
#define DISPLAY_O_F_CONTROL 0x08
#define CURSOR_DISPLAY_SHIFT 0x10
#define FUNCTION_SET 0x20
#define SET_CGRAM   0x40
#define SET_DDRAM   0x80

#define LCD_RS 0x01
#define LCD_RW 0X02
#define LCD_E  0x04
#define LCD_A  0x08

//Entry mode set DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
//0   0   0   0   0   1   I/D S
#define SHIFT_RIGHT 0x02
#define SHIFT_LEFT  0x00
#define SHIFT_DISPLAY_ON 0x01
#define SHIFT_DISPLAY_OFF 0x00

//Display on/off control DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
//0   0   0   0   1   D   C   B
#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0x00
#define CURSOR_ON  0x02
#define CURSOR_OFF 0x00
#define BLINK_ON    0x01
#define BLINK_OFF   0x00

//Cursor of display shift DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
//0   0   0   1   S/C R/L 0   0
#define CURSOR_SHIFT_LEFT 0x00 //Corrimiento de cursor a la izquierda
#define CURSOR_SHIFT_RIGHT 0x04 //Corrimiento de cursor a la derecha
#define DISPLAY_SHIFT_LEFT 0x08 //Corrimiento de pantalla a la izquierda
#define DISPLAY_SHIFT_RIGHT 0x0C//Corrimiento de pantalla a la derecha
//Function set DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
//0   0   1   DL  N   F   0   0
#define MODE_8_BIT 0x10
#define MODE_4_BIT 0x00
#define MODE_1_LINE 0x00
#define MODE_2_LINE 0x08
#define MODE_5_8_DOTS 0x00
#define MODE_5_10_DOTS 0x04

void delayMs(uint32_t ui32Ms);
void delayUs(uint32_t ui32Ms);

class lcdI2C
{
private:
    uint8_t lcdaddr;
    uint32_t i2caddr;
    uint8_t row;
    uint8_t column;
    uint8_t displayControl;
    uint8_t entryModeSet;
    uint8_t ledStatus;
    void backlight(uint8_t status);
    void lcd_send(uint8_t dato, bool cmd);
    void lcd_conf(uint8_t dato);
    void LCD_CMD_I2C(uint8_t dato);
public:
    lcdI2C(uint8_t lcd, uint32_t i2c, uint8_t row, uint8_t column);
    ~lcdI2C();
    void LCD_INIT();
    void LCD_PRINT_CHAR(uint8_t caracter);
    void LCD_PRINT_STRING(const char *cadena);
    void LCD_TEXT_LEFT();
    void LCD_TEXT_RIGHT();
    void LCD_DISPLAY_SHIFT_ON();
    void LCD_DISPLAY_SHIFT_OFF();
    void LCD_DISPLAY_ON();
    void LCD_DISPLAY_OFF();
    void LCD_CURSOR_ON();
    void LCD_CURSOR_OFF();
    void LCD_BLINK_ON();
    void LCD_BLINK_OFF();
    void LCD_SET_CURSOR(uint8_t fila, uint8_t columna);
    void LCD_SHIFT_RIGHT();
    void LCD_SHIFT_LEFT();
    void LCD_CURSOR_SHIFT_RIGHT();
    void LCD_CURSOR_SHIFT_LEFT();
    void LCD_CLEAR();
    void LCD_RETURN();
    void LCD_BACKLIGHT_ON();
    void LCD_BACKLIGHT_OFF();
    void LCD_CUSTOM_CHAR(uint8_t pos,const uint8_t *cChar );

};

#endif /* LCDI2C_H_ */
