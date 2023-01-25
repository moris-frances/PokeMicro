#ifndef __OLED_C_H
#define __OLED_C_H

//Commands/Registers
#define SEPS114A_SOFT_RESET 0x01
#define SEPS114A_DISPLAY_ON_OFF 0x02
#define SEPS114A_ANALOG_CONTROL 0x0F    //
#define SEPS114A_STANDBY_ON_OFF 0x14
#define SEPS114A_OSC_ADJUST 0x1A
#define SEPS114A_ROW_SCAN_DIRECTION 0x09
#define SEPS114A_DISPLAY_X1 0x30
#define SEPS114A_DISPLAY_X2 0x31
#define SEPS114A_DISPLAY_Y1 0x32
#define SEPS114A_DISPLAY_Y2 0x33
#define SEPS114A_DISPLAYSTART_X 0x38
#define SEPS114A_DISPLAYSTART_Y 0x39
#define SEPS114A_CPU_IF 0x0D
#define SEPS114A_MEM_X1 0x34
#define SEPS114A_MEM_X2 0x35
#define SEPS114A_MEM_Y1 0x36
#define SEPS114A_MEM_Y2 0x37
#define SEPS114A_MEMORY_WRITE_READ 0x1D
#define SEPS114A_DDRAM_DATA_ACCESS_PORT 0x08
#define SEPS114A_DISCHARGE_TIME 0x18
#define SEPS114A_PEAK_PULSE_DELAY 0x16
#define SEPS114A_PEAK_PULSE_WIDTH_R 0x3A
#define SEPS114A_PEAK_PULSE_WIDTH_G 0x3B
#define SEPS114A_PEAK_PULSE_WIDTH_B 0x3C
#define SEPS114A_PRECHARGE_CURRENT_R 0x3D
#define SEPS114A_PRECHARGE_CURRENT_G 0x3E
#define SEPS114A_PRECHARGE_CURRENT_B 0x3F
#define SEPS114A_COLUMN_CURRENT_R 0x40
#define SEPS114A_COLUMN_CURRENT_G 0x41
#define SEPS114A_COLUMN_CURRENT_B 0x42
#define SEPS114A_ROW_OVERLAP 0x48
#define SEPS114A_SCAN_OFF_LEVEL 0x49
#define SEPS114A_ROW_SCAN_ON_OFF 0x17
#define SEPS114A_ROW_SCAN_MODE 0x13
#define SEPS114A_SCREEN_SAVER_CONTEROL 0xD0
#define SEPS114A_SS_SLEEP_TIMER 0xD1
#define SEPS114A_SCREEN_SAVER_MODE 0xD2
#define SEPS114A_SS_UPDATE_TIMER 0xD3
#define SEPS114A_RGB_IF 0xE0
#define SEPS114A_RGB_POL 0xE1
#define SEPS114A_DISPLAY_MODE_CONTROL 0xE5

//Pins
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_5
#define OLED_RST_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOB
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA

#include "stm32l4xx_hal.h"
#include <stdint.h>                                                          // remember to rebuild all if you change any included files

// RGB 565 colours     -  bits 15-0 are r4 r3 r2 r1 r0 g5 g4 g3 g2 g1 g0 b4 b3 b2 b1 b0
#define Color_Black           0x0000      /*   0,   0,   0 */
#define Color_Navy            0x000F      /*   0,   0, 128 */
#define Color_DarkGreen       0x03E0      /*   0, 128,   0 */
#define Color_DarkCyan        0x03EF      /*   0, 128, 128 */
#define Color_Maroon          0x7800      /* 128,   0,   0 */
#define Color_Purple          0x780F      /* 128,   0, 128 */
#define Color_Olive           0x7BE0      /* 128, 128,   0 */
#define Color_LightGrey       0xC618      /* 192, 192, 192 */
#define Color_DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Color_Blue            0x001F      /*   0,   0, 255 */
#define Color_Green           0x07E0      /*   0, 255,   0 */
#define Color_Cyan            0x07FF      /*   0, 255, 255 */
#define Color_Red             0xF800      /* 255,   0,   0 */
#define Color_Magenta         0xF81F      /* 255,   0, 255 */
#define Color_Mustard         0xff00
#define Color_Yellow          0xFFE0      /* 255, 255,   0 */
#define Color_White           0xFFFF      /* 255, 255, 255 */
#define Color_Orange          0xFD20      /* 255, 165,   0 */
#define Color_GreenYellow     0xAFE5      /* 173, 255,  47 */
#define Color_Pink            0xFB1B


/**

@brief Clear OLED screen function.
@description This function writes black to the entire OLED screen.
*/
void OLED_CLS();


/**

@brief Draw string function.
@param x_pos X position on OLED screen to start drawing the text.
@param y_pos Y position on OLED screen to start drawing the text.
@param text Null-terminated string to be displayed on OLED screen.
@description This function outputs the given text to the OLED using the current font and color.
*/
void drawString(char x_pos, char y_pos, char *text);
/**

@brief Send command to OLED C display function.
@param reg_index Register index.
@param reg_value Register value.
@description This function sends a command to the OLED C display.
*/
void OLED_C_command (uint8_t reg_index, uint8_t reg_value);

/**

@brief Send data to OLED C display function.
@param data_value Data value to be sent to OLED C display.
@description This function sends data to the OLED C display.
*/
void OLED_C_data(uint8_t data_value);

/**

@brief Initialize MCU function.
@param hspi Handle for the SPI module.
@description This function initializes the MCU.
*/
void InitMCU(SPI_HandleTypeDef hspi);

/**

@brief Sequence before writing data to memory function.
@description This function runs a sequence before writing data to memory.
*/
void DDRAM_access();
/**

@brief Set memory area function.
@param X1 X coordinate 1.
@param X2 X coordinate 2.
@param Y1 Y coordinate 1.
@param Y2 Y coordinate 2.
@description This function sets the memory area (address) to write display data.
*/
void OLED_C_MemorySize(uint8_t X1, uint8_t X2, uint8_t Y1, uint8_t Y2);

/**

@brief Select color function.
@param colorMSB Most significant byte of color value.
@param colorLSB Least significant byte of color value.
@description This function selects a color to be used for OLED display.
*/

void OLED_C_Color(uint8_t colorMSB, uint8_t colorLSB );

/**

@brief Display an ASCII character function.
@param x_co_ord X coordinate of the character.
@param y_co_ord Y coordinate of the character.
@param ascii_in ASCII character to be displayed.
@param color Color of the character.
@param OLED_font OLED font to be used.
@description This function is under development and contains many bodges. It is used to put an ASCII character on the OLED screen.
*/
void drawChar(unsigned char x_co_ord, unsigned char y_co_ord, unsigned char ascii_in, uint16_t color, unsigned char OLED_font);
/**

@brief Draw a single pixel function.
@param px X coordinate of the pixel.
@param py Y coordinate of the pixel.
@param color Color of the pixel.
@description This function sets a single pixel on the OLED screen. It requires left/right swap to convert 0,0 from top left logical to top right display requirement.
*/
void drawPixel(char px, char py, uint16_t color);
/**

@brief Draw full-screen bitmap function.
@param bitmap Array of bitmap data.
@param size_of_bitmap Size of the bitmap.
@description This function displays a full-screen bitmap on the OLED screen.
*/
void drawFullScreenBitmap(uint8_t pika[],int size_of_bitmap);
/**

@brief Draw Pokemon and Pokedex function.
@param bitmap Array of bitmap data.
@param size_of_bitmap Size of the bitmap.
@param string String to be displayed.
@param size_of_string Size of the string.
@description This function displays a bitmap and a string on the OLED screen.
*/
void drawPokemonAndPokedex(uint8_t bitmap[], int size_of_bitmap, char string[], int size_of_string);
#endif
