/*
 * LCD1602A.c
 *
 *  Created on: May 5, 2025
 *      Author: Andrew Pan
 */
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"
#include <stdio.h>
#include <LCD1602A.h>




void LCD_pulseEnable() {
    GPIO_writePin(LCD_E, 1);
    DEVICE_DELAY_US(1);  // tiny delay
    GPIO_writePin(LCD_E, 0);
    DEVICE_DELAY_US(50); // let LCD latch the data
}

void LCD_write4Bits(uint8_t nibble) {
    GPIO_writePin(LCD_D4, (nibble >> 0) & 1);
    GPIO_writePin(LCD_D5, (nibble >> 1) & 1);
    GPIO_writePin(LCD_D6, (nibble >> 2) & 1);
    GPIO_writePin(LCD_D7, (nibble >> 3) & 1);
    LCD_pulseEnable();
}
void LCD_send(uint8_t value, bool isData) {
    GPIO_writePin(LCD_RS, isData ? 1 : 0);
    LCD_write4Bits(value >> 4);     // high nibble
    LCD_write4Bits(value & 0x0F);   // low nibble
}


void LCD_clear() {
    LCD_send(0x01, 0); // Clear screen
    DEVICE_DELAY_US(2000);
}

void LCD_setCursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = { 0x00, 0x40 };
    LCD_send(0x80 | (col + row_offsets[row]), 0);
}

void LCD_print(char *str) {
    while (*str) {
        LCD_send(*str++, 1); // send each char as data
    }
}

void displayWheelSpeeds(uint16_t left, uint16_t right, uint16_t back) {
    char buf[17];
    LCD_setCursor(0, 0);
    sprintf(buf, " L%4uR%4uB%4u", left, right, back);
    LCD_print(buf);
}
void displayDynamicRatio(uint16_t awd_ratio, uint16_t rwd_ratio) {
    char buf[17];

    sprintf(buf, "F:B %u:%u", awd_ratio, rwd_ratio);
    LCD_setCursor(1, 0);
    LCD_print(buf);
}

void LCD_GPIO_Init() {
    GPIO_setDirectionMode(LCD_RS, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(LCD_E,  GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(LCD_D4, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(LCD_D5, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(LCD_D6, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(LCD_D7, GPIO_DIR_MODE_OUT);
}

void LCD_init() {
    DEVICE_DELAY_US(15000); // wait after power on

    LCD_write4Bits(0x03); // special init sequence
    DEVICE_DELAY_US(4500);
    LCD_write4Bits(0x03);
    DEVICE_DELAY_US(4500);
    LCD_write4Bits(0x03);
    DEVICE_DELAY_US(150);
    LCD_write4Bits(0x02); // switch to 4-bit mode

    LCD_send(0x28, 0); // Function set: 4-bit, 2 lines
    LCD_send(0x0C, 0); // Display ON, cursor OFF
    LCD_send(0x06, 0); // Entry mode: move cursor right
    LCD_send(0x01, 0); // Clear display
    DEVICE_DELAY_US(2000);
}


