/*
 * lcdI2C.cpp
 *
 *  Created on: 3/06/2021
 *      Author: omar
 */

#include <lcdI2C.h>
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"

uint8_t aux_0, aux_1;
uint8_t rows[4] = { 0x00, 0x40, 0x10, 0x50 };
uint32_t dUs = 10;

void delayMs(uint32_t ui32Ms)
{
    uint32_t aux = 3 * 1000;
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / aux));
}

void delayUs(uint32_t ui32Us)
{
    uint32_t aux = 3 * 1000000;
    SysCtlDelay(ui32Us * (SysCtlClockGet() / aux));
}

lcdI2C::lcdI2C(uint8_t lcd, uint32_t i2c, uint8_t row, uint8_t column)
{
    lcdaddr = lcd;
    i2caddr = i2c;
    this->row = row;
    this->column = column;
    displayControl = DISPLAY_O_F_CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF;
    entryModeSet = ENTRY_MODE_SET | SHIFT_RIGHT | SHIFT_DISPLAY_OFF;
    ledStatus = LCD_A;
}
lcdI2C::~lcdI2C(){}

void lcdI2C::LCD_INIT()
{

    uint8_t contador;
    delayMs(55);
    for (contador = 0; contador < 3; contador++)
    {
        lcdI2C::lcd_conf(FUNCTION_SET | MODE_8_BIT);
    }
    for (contador = 0; contador < 2; contador++)
    {
        lcdI2C::lcd_conf(FUNCTION_SET | MODE_4_BIT);
    }
    lcd_conf((MODE_2_LINE | MODE_5_8_DOTS) << 4);
    LCD_CMD_I2C(displayControl);
    LCD_CMD_I2C(entryModeSet);
    LCD_CMD_I2C(CLEAR);
    LCD_CMD_I2C(RETURN);
}

void lcdI2C::backlight(uint8_t status)
{
    I2CMasterSlaveAddrSet(i2caddr, lcdaddr, false);
    I2CMasterDataPut(i2caddr, ledStatus);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(i2caddr))
    {
    }
}

void lcdI2C::lcd_send(uint8_t dato, bool cmd)
{
    uint8_t conf;
    if (cmd == true)
    {
        conf = ledStatus;
    }
    else if (cmd == false)
    {
        conf = ledStatus | LCD_RS;
    }
    delayMs(1);
    aux_0 = aux_1 = 0;
    aux_0 = dato >> 4;
    aux_0 = aux_0 << 4;
    aux_1 = dato << 4;

    I2CMasterSlaveAddrSet(i2caddr, lcdaddr, false);
    I2CMasterDataPut(i2caddr, aux_0 | conf);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);

    I2CMasterDataPut(i2caddr, aux_0 | LCD_E | conf);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_CONT); //se manda los 4 bits mas significativos
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);

    I2CMasterDataPut(i2caddr, aux_0 | conf);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_CONT);
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);

    I2CMasterDataPut(i2caddr, aux_1 | LCD_E | conf);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_CONT); //se mandan los 4 bits menos significativos
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);

    I2CMasterDataPut(i2caddr, aux_1 | conf);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);
}

void lcdI2C::lcd_conf(uint8_t dato)
{
    delayMs(1);
    I2CMasterSlaveAddrSet(i2caddr, lcdaddr, false);
    I2CMasterDataPut(i2caddr, dato);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);
    I2CMasterDataPut(i2caddr, dato | LCD_E);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_CONT);
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);
    I2CMasterDataPut(i2caddr, dato);
    I2CMasterControl(i2caddr, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(i2caddr))
    {
    }
    delayUs(dUs);
}

void lcdI2C::LCD_CMD_I2C(uint8_t dato)
{
    lcd_send(dato, true);
}

void lcdI2C::LCD_PRINT_CHAR(uint8_t caracter)
{
    lcd_send(caracter, false);
}

void lcdI2C::LCD_PRINT_STRING(const char *cadena)
{
    uint8_t cnt;
    for (cnt = 0; cadena[cnt] != '\0'; cnt++)
    {
        LCD_PRINT_CHAR(cadena[cnt]);
    }
}
void lcdI2C::LCD_DISPLAY_ON()
{
    displayControl |= DISPLAY_ON;
    LCD_CMD_I2C(displayControl);
}
void lcdI2C::LCD_DISPLAY_OFF()
{
    displayControl &= ~DISPLAY_ON;
    LCD_CMD_I2C(displayControl);
}

void lcdI2C::LCD_CURSOR_ON()
{
    displayControl |= CURSOR_ON;
    LCD_CMD_I2C(displayControl);
}
void lcdI2C::LCD_CURSOR_OFF()
{
    displayControl &= ~CURSOR_ON;
    LCD_CMD_I2C(displayControl);
}
void lcdI2C::LCD_BLINK_ON()
{
    displayControl |= BLINK_ON;
    LCD_CMD_I2C(displayControl);
}
void lcdI2C::LCD_BLINK_OFF()
{
    displayControl &= ~BLINK_ON;
    LCD_CMD_I2C(displayControl);
}

void lcdI2C::LCD_SET_CURSOR(uint8_t fila, uint8_t columna)
{
    if (fila <= row && columna <= column)
    {
        LCD_CMD_I2C(SET_DDRAM | rows[fila - 1] | (columna - 1));
    }
    else
    {
        LCD_RETURN();
        LCD_PRINT_STRING("COOR"); //Cursor Out Of Range COOR
        delayMs(500);
        LCD_CLEAR();
    }

}
void lcdI2C::LCD_SHIFT_RIGHT()
{
    LCD_CMD_I2C(CURSOR_DISPLAY_SHIFT | DISPLAY_SHIFT_RIGHT);
}
void lcdI2C::LCD_SHIFT_LEFT()
{
    LCD_CMD_I2C(CURSOR_DISPLAY_SHIFT | DISPLAY_SHIFT_LEFT);
}
void lcdI2C::LCD_CURSOR_SHIFT_RIGHT()
{
    LCD_CMD_I2C(CURSOR_DISPLAY_SHIFT | CURSOR_SHIFT_RIGHT);
}
void lcdI2C::LCD_CURSOR_SHIFT_LEFT()
{
    LCD_CMD_I2C(CURSOR_DISPLAY_SHIFT | CURSOR_SHIFT_LEFT);
}
void lcdI2C::LCD_TEXT_LEFT(){
    entryModeSet&=~SHIFT_RIGHT;
    LCD_CMD_I2C(entryModeSet);
}
void lcdI2C::LCD_TEXT_RIGHT(){
    entryModeSet|=SHIFT_RIGHT;
    LCD_CMD_I2C(entryModeSet);
}
void lcdI2C::LCD_DISPLAY_SHIFT_ON(){
    entryModeSet|=SHIFT_DISPLAY_ON;
    LCD_CMD_I2C(entryModeSet);
}
void lcdI2C::LCD_DISPLAY_SHIFT_OFF(){
    entryModeSet&=~SHIFT_DISPLAY_ON;
    LCD_CMD_I2C(entryModeSet);
}
void lcdI2C::LCD_CLEAR()
{
    LCD_CMD_I2C(CLEAR);
}
void lcdI2C::LCD_RETURN()
{
    LCD_CMD_I2C(RETURN);
}
void lcdI2C::LCD_BACKLIGHT_ON()
{
    ledStatus = LCD_A;
    backlight(ledStatus);
}
void lcdI2C::LCD_BACKLIGHT_OFF()
{
    ledStatus &= 0x00;
    backlight(ledStatus);
}
void lcdI2C::LCD_CUSTOM_CHAR(uint8_t pos, const uint8_t *cChar)
{
    uint8_t cnt;
    if (pos <= 8 && pos >= 1)
    {
        LCD_CMD_I2C(SET_CGRAM | ((pos - 1) * 8));
        for (cnt = 0; cnt < 8; cnt++)
        {
            LCD_PRINT_CHAR(cChar[cnt]);
        }
    }
}
