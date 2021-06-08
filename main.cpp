/*
 Pruebas para la pantalla i2c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "lcdI2C.h"

#define LCD_I2C 0x27
void configI2C0();
uint8_t customChar0[]={
                       0x1F,
                       0x1F,
                       0x0E,
                       0x04,
                       0x00,
                       0x00,
                       0x00,
                       0x00
                };
uint8_t customChar1[]={
                       0x00,
                       0x1F,
                       0x1F,
                       0x0E,
                       0x04,
                       0x00,
                       0x00,
                       0x00
};

uint8_t customChar2[]={
                       0x00,
                       0x00,
                       0x1F,
                       0x1F,
                       0x0E,
                       0x04,
                       0x00,
                       0x00
};
uint8_t customChar3[]={
                       0x11,
                       0x0A,
                       0x11,
                       0x0A,
                       0x15,
                       0x0A,
                       0x0E,
                       0x11
};

lcdI2C lcd_0(LCD_I2C, I2C0_BASE, 2, 16);
uint8_t cnt;

int main(void)
{

    SysCtlClockSet(
            SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ
                    | SYSCTL_SYSDIV_2_5); //Configura el reloj a 80MHz
    configI2C0();
    lcd_0.LCD_INIT();
    /*
    lcd_0.LCD_CUSTOM_CHAR(1,customChar0);
    lcd_0.LCD_CUSTOM_CHAR(2,customChar1);
    lcd_0.LCD_CUSTOM_CHAR(3,customChar2);
    lcd_0.LCD_CUSTOM_CHAR(4,customChar3);
    */
    lcd_0.LCD_CURSOR_ON();
    while (1)
    {

        lcd_0.LCD_CLEAR();
        lcd_0.LCD_RETURN();
        lcd_0.LCD_TEXT_RIGHT();
        lcd_0.LCD_SET_CURSOR(1,5);
        lcd_0.LCD_DISPLAY_SHIFT_OFF();
        lcd_0.LCD_PRINT_STRING("test 0");
        delayMs(500);

        lcd_0.LCD_CLEAR();
        lcd_0.LCD_RETURN();
        lcd_0.LCD_PRINT_STRING("test 1");
        delayMs(500);

    }
}

void configI2C0()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Habilita el periferico puerto B donde se encuentra el I2C0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0); // configura el periferico I2C0
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0); // resetea el periferico
    GPIOPinConfigure(GPIO_PB2_I2C0SCL); //configura el pin pb2 como el scl
    GPIOPinConfigure(GPIO_PB3_I2C0SDA); //configura el pin pb3 como el sda
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2); // configura a travez del mux interno la funcion de i2c al pin 2 del puerto B
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3); //configura a travez del mux intenermo la funcion de i2c al pin 3 del puerto B
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false); // habilita el puerto con una velocidad de 100Kpbs
}
