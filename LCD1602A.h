/*
 * LCD1602A.h
 *
 *  Created on: May 5, 2025
 *      Author: Andrew Pan
 */

#ifndef LCD1602A_H_
#define LCD1602A_H_

#define LCD_RS  32
#define LCD_E   19
#define LCD_D4  18
#define LCD_D5  67
#define LCD_D6  111
#define LCD_D7  60


void LCD_pulseEnable();
void LCD_write4Bits(uint8_t nibble);
void LCD_send(uint8_t value, bool isData);
void LCD_clear();
void LCD_setCursor(uint8_t row, uint8_t col);
void LCD_print(char *str);
void displayWheelSpeeds(uint16_t left, uint16_t right, uint16_t back);
void displayDynamicRatio(uint16_t awd_ratio, uint16_t rwd_ratio);
void LCD_GPIO_Init();
void LCD_init();

#endif /* LCD1602A_H_ */
