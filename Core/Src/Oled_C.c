#include "Oled_C.h"

#include "fonts.h"                                     // next line calculated by font
// global variables
unsigned char display_font;                                                     // valid values are 1 2 3 4     (6 7 8 10 14 16 18 if uncommented)
unsigned int display_color = Color_Purple;                                                     // bits 15-0 are r4 r3 r2 r1 r0 g5 g4 g3 g2 g1 g0 b4 b3 b2 b1 b0
unsigned char display_x;                                                        // current x coordinate for OLED color click v100 - 0,0 is top left
unsigned char display_y;                                                        // current y coordinate for OLED color click v100 - 0,0 is top left
unsigned char last_width;                                                       // width of most recent character
unsigned char font_nrows;                                                       // number of rows in most recent font
unsigned char new_y;                                                            // next line calculated by font

SPI_HandleTypeDef spi_handle;

void OLED_CLS(){
	int j;

    OLED_C_command(0x1D,0x02);                                                  // Set Memory Read/Write mode
    OLED_C_MemorySize(0x00,0x5F,0x00,0x5F);                                     // whole screen
    DDRAM_access();
    for(j = 0 ; j < 9216 ; j++){                                                // black box   96x96=9216
        OLED_C_Color(0x00,0x00);                                                // black
    }
    display_font = 1;                                                          // default OLED font - valid values are 6 7 8 10 14 18
    display_color = Color_White;                                                // default OLED text colour
    new_y = 0;
}




void OLED_C_command (uint8_t reg_index, uint8_t reg_value){
		//Select index addr
	    //OLED_CS = 0;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
		//OLED_DC = 0;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
	    // 0=command
		//SPI1_Write(reg_index);
	    HAL_SPI_Transmit(&spi_handle, &reg_index, 1, 1000);

	    //OLED_CS = 1;
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
	    //Write data to reg
	    //OLED_CS = 0;
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
	    //OLED_DC = 1;                          // 1=data or parameter
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
	    HAL_SPI_Transmit(&spi_handle, &reg_value, 1, 1000);

	    //OLED_CS = 1;
	    //end
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);

}

//Send data to OLED C display
void OLED_C_data(uint8_t data_value){
    //OLED_CS = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
    //OLED_DC = 1;                          // 1=data or parameter
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
    HAL_SPI_Transmit(&spi_handle, &data_value, 1, 1000);
    //OLED_CS = 0;
    //end
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
}
// Init MCU function
void InitMCU(SPI_HandleTypeDef hspi){
	spi_handle = hspi;

// Init sequence for 96x96 OLED color module
    //OLED_RST = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);

	HAL_Delay(10);
    //OLED_RST = 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	HAL_Delay(10);
    /*  Soft reset */
    /*  Soft reset */
    OLED_C_command(SEPS114A_SOFT_RESET,0x00);                                   // reg 0x01 data IDX=0
    /* Standby ON/OFF*/
    OLED_C_command(SEPS114A_STANDBY_ON_OFF,0x01);                               // reg 0x14 data 0x01 Standby on
    HAL_Delay(5);                                                                // Wait for 5ms (1ms Delay Minimum)
    OLED_C_command(SEPS114A_STANDBY_ON_OFF,0x00);                               // Standby off   (start display oscillator)
    HAL_Delay(5);                                                                // 1ms Delay Minimum (1ms Delay Minimum)
    /* Display OFF */
    OLED_C_command(SEPS114A_DISPLAY_ON_OFF,0x00);
    /* Set Oscillator operation */
    OLED_C_command(SEPS114A_ANALOG_CONTROL,0x40);                               // using external resistor and internal OSC
    /* Set frame rate */
    OLED_C_command(SEPS114A_OSC_ADJUST,0x0F);                                   // frame rate : 95Hz
    /* Set active display area of panel */
    OLED_C_command(SEPS114A_DISPLAY_X1,0x00);
    OLED_C_command(SEPS114A_DISPLAY_X2,0x5F);                                   // 96 pixels wide
    OLED_C_command(SEPS114A_DISPLAY_Y1,0x00);
    OLED_C_command(SEPS114A_DISPLAY_Y2,0x5F);                                   // 96 pixels high
    /* Select the RGB data format and set the initial state of RGB interface port */
    OLED_C_command(SEPS114A_RGB_IF,0x00);                                       // RGB 8bit interface
    /* Set RGB polarity */
    OLED_C_command(SEPS114A_RGB_POL,0x00);
    /* Set display mode control */
    OLED_C_command(SEPS114A_DISPLAY_MODE_CONTROL,0x80);                         // SWAP:BGR, Reduce current : Normal, DC[1:0] : Normal
    /* Set MCU Interface */
    OLED_C_command(SEPS114A_CPU_IF,0x00);                                       // MPU External interface mode, 8bits
    /* Set Memory Read/Write mode */
    OLED_C_command(SEPS114A_MEMORY_WRITE_READ,0x00);
    /* Set row scan direction */
    OLED_C_command(SEPS114A_ROW_SCAN_DIRECTION,0x00);                           // Column : 0 --> Max, Row : 0 \81--> Max
    /* Set row scan mode */
    OLED_C_command(SEPS114A_ROW_SCAN_MODE,0x00);                                // Alternate scan mode
    /* Set column current */

    OLED_C_command(SEPS114A_COLUMN_CURRENT_R,0x6E);
    OLED_C_command(SEPS114A_COLUMN_CURRENT_G,0x4F);
    OLED_C_command(SEPS114A_COLUMN_CURRENT_B,0x77);
    /* Set row overlap */
    OLED_C_command(SEPS114A_ROW_OVERLAP,0x00);                                  // Band gap only
    /* Set discharge time */
    OLED_C_command(SEPS114A_DISCHARGE_TIME,0x01);                               // Discharge time : normal discharge
    /* Set peak pulse delay */
    OLED_C_command(SEPS114A_PEAK_PULSE_DELAY,0x00);
    /* Set peak pulse width */
    OLED_C_command(SEPS114A_PEAK_PULSE_WIDTH_R,0x02);
    OLED_C_command(SEPS114A_PEAK_PULSE_WIDTH_G,0x02);
    OLED_C_command(SEPS114A_PEAK_PULSE_WIDTH_B,0x02);
    /* Set precharge current */
    OLED_C_command(SEPS114A_PRECHARGE_CURRENT_R,0x14);
    OLED_C_command(SEPS114A_PRECHARGE_CURRENT_G,0x50);
    OLED_C_command(SEPS114A_PRECHARGE_CURRENT_B,0x19);
    /* Set row scan on/off  */
    OLED_C_command(SEPS114A_ROW_SCAN_ON_OFF,0x00);                              // Normal row scan
    /* Set scan off level */
    OLED_C_command(SEPS114A_SCAN_OFF_LEVEL,0x04);                               // VCC_C*0.75
    /* Set memory access point */
    OLED_C_command(SEPS114A_DISPLAYSTART_X,0x00);
    OLED_C_command(SEPS114A_DISPLAYSTART_Y,0x00);
    HAL_Delay(100);
    /* Display ON */
    OLED_C_command(SEPS114A_DISPLAY_ON_OFF,0x01);                               // reg 0x02 display on                         // reg 0x02 display on

    OLED_CLS();
}

//Sequence before writing data to memory
void DDRAM_access(){
    //OLED_CS = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);

	//OLED_DC = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);                                                               // 0=command

	//SPI1_Write(reg_index);
	uint8_t command_u8 = SEPS114A_DDRAM_DATA_ACCESS_PORT;
    HAL_SPI_Transmit(&spi_handle, &command_u8, 1, 100);// instruction 0x08 select DDRAM data access port
    //end
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
}

//Set memory area(address) to write a display data
void OLED_C_MemorySize(uint8_t X1, uint8_t X2, uint8_t Y1, uint8_t Y2){
    OLED_C_command(SEPS114A_MEM_X1,X1);
    OLED_C_command(SEPS114A_MEM_X2,X2);
    OLED_C_command(SEPS114A_MEM_Y1,Y1);
    OLED_C_command(SEPS114A_MEM_Y2,Y2);
}

//Select color
void OLED_C_Color(uint8_t colorMSB, uint8_t colorLSB ){
    OLED_C_data(colorMSB);
    OLED_C_data(colorLSB);
}

// below routine is under development and contains many bodges
// put an ascii character on screen
void drawChar(unsigned char x_co_ord, unsigned char y_co_ord, unsigned char ascii_in, uint16_t color, unsigned char OLED_font){
signed char row;                                                                // character row
unsigned char col;                                                              // character column
unsigned long font_row;                                                         // data in current row
unsigned long font_pointer;                                                     // table of addresses 1 byte ??, 2 bytes offset, 1 byte ??
unsigned long font_offset;
unsigned char ascii_off;

// see Microchip AN1182 for font coding...
// 0x00 word -  bits 13,12 orient - 11,10 bits per pixel  -  8 extended glyph - 7 to 0 font ID - rest reserved
// 0x02 word - first character
// 0x04 word - last character
// 0x06 word - height
// glyph entries follow - 4 bytes (12 bytes extended glyph) -  1 byte Glyph width - 3 bytes offset
// orient 00=normal, 01=270deg, 10=180deg, 11=90deg
// bits per pixel 00=1, 01=2 (anti alias), 1x reserved
// extended glyph 0=normal, 1=extended

    if (OLED_font == 1){
        font_nrows = guiFont_Roboto_Mono11x23_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Roboto_Mono11x23_Regular[font_pointer];               // character width
            font_offset = guiFont_Roboto_Mono11x23_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Roboto_Mono11x23_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Roboto_Mono11x23_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Roboto_Mono11x23_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Roboto_Mono11x23_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Roboto_Mono11x23_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 2){
        font_nrows = guiFont_Exo_2_Condensed10x16_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Exo_2_Condensed10x16_Regular[font_pointer];               // character width
            font_offset = guiFont_Exo_2_Condensed10x16_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Exo_2_Condensed10x16_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Exo_2_Condensed10x16_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Exo_2_Condensed10x16_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Exo_2_Condensed10x16_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Exo_2_Condensed10x16_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 3){
        font_nrows = guiFont_Exo_2_Condensed15x23_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Exo_2_Condensed15x23_Regular[font_pointer];               // character width
            font_offset = guiFont_Exo_2_Condensed15x23_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Exo_2_Condensed15x23_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Exo_2_Condensed15x23_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Exo_2_Condensed15x23_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Exo_2_Condensed15x23_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Exo_2_Condensed15x23_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 4){
        font_nrows = guiFont_Exo_2_Condensed21x32_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Exo_2_Condensed21x32_Regular[font_pointer];               // character width
            font_offset = guiFont_Exo_2_Condensed21x32_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Exo_2_Condensed21x32_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Exo_2_Condensed21x32_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Exo_2_Condensed21x32_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Exo_2_Condensed21x32_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Exo_2_Condensed21x32_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }


    if (OLED_font == 6){
        font_nrows = guiFont_Tahoma_6_Regular[6];                               // number of bytes in each entry

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){                                  // read pixels right to left
            font_pointer = (ascii_off*4)+8;                                     // skip 8 byte header
            last_width = guiFont_Tahoma_6_Regular[font_pointer];                // read out character width (excluding filler bytes)
            font_offset = guiFont_Tahoma_6_Regular[font_pointer+1];             // get offset low
            font_offset += guiFont_Tahoma_6_Regular[font_pointer+2]*256;        // get offset middle
            font_offset += (unsigned long)guiFont_Tahoma_6_Regular[font_pointer+3]*65536;  // get offset high and force long type calculation
            font_offset += row;                                                 // add in the row offset
            if(last_width > 8){                                                 // if width > 8 then we need 2 bytes per row
                font_offset += row;                                             // so double up byte offset
            }
            font_row = guiFont_Tahoma_6_Regular[font_offset];                   // get a row of up to 8 pixels
            if(last_width > 8){                                                 // if width > 8 then need second byte of pixels
                font_row += (guiFont_Tahoma_6_Regular[font_offset+1])*256;      // so go fetch a further byte
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 7){
        font_nrows = guiFont_Tahoma_7_Regular[6];                               // number of bytes in each entry

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){                                  // read pixels right to left
            font_pointer = (ascii_off*4)+8;                                     // skip 8 byte header
            last_width = guiFont_Tahoma_7_Regular[font_pointer];                // read out character width (excluding filler bytes)
            font_offset = guiFont_Tahoma_7_Regular[font_pointer+1];             // get offset low
            font_offset += guiFont_Tahoma_7_Regular[font_pointer+2]*256;        // get offset middle
            font_offset += (unsigned long)guiFont_Tahoma_7_Regular[font_pointer+3]*65536;  // get offset high and force long type calculation
            font_offset += row;                                                 // add in the row offset
            if(last_width > 8){                                                 // if width > 8 then we need 2 bytes per row
                font_offset += row;                                             // so double up byte offset
            }
            font_row = guiFont_Tahoma_7_Regular[font_offset];                   // get a row of up to 8 pixels
            if(last_width > 8){                                                 // if width > 8 then need second byte of pixels
                font_row += (guiFont_Tahoma_7_Regular[font_offset+1])*256;      // so go fetch a further byte
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 8){
        font_nrows = guiFont_Tahoma_8_Regular[6];                               // number of bytes in each entry

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){                                  // read pixels right to left
            font_pointer = (ascii_off*4)+8;                                     // skip 8 byte header
            last_width = guiFont_Tahoma_8_Regular[font_pointer];                // read out character width (excluding filler bytes)
            font_offset = guiFont_Tahoma_8_Regular[font_pointer+1];             // get offset low
            font_offset += guiFont_Tahoma_8_Regular[font_pointer+2]*256;        // get offset middle
            font_offset += (unsigned long)guiFont_Tahoma_8_Regular[font_pointer+3]*65536;  // get offset high and force long type calculation
            font_offset += row;                                                 // add in the row offset
            if(last_width > 8){                                                 // if width > 8 then we need 2 bytes per row
                font_offset += row;                                             // so double up byte offset
            }
            font_row = guiFont_Tahoma_8_Regular[font_offset];                   // get a row of up to 8 pixels
            if(last_width > 8){                                                 // if width > 8 then need second byte of pixels
                font_row += (guiFont_Tahoma_8_Regular[font_offset+1])*256;      // so go fetch a further byte
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 10){
        font_nrows = guiFont_Tahoma_10_Regular[6];                              // number of bytes in each entry

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){                                  // read pixels right to left
            font_pointer = (ascii_off*4)+8;                                     // skip 8 byte header
            last_width = guiFont_Tahoma_10_Regular[font_pointer];               // read out character width (excluding filler bytes)
            font_offset = guiFont_Tahoma_10_Regular[font_pointer+1];            // get offset low
            font_offset += guiFont_Tahoma_10_Regular[font_pointer+2]*256;       // get offset middle
            font_offset += (unsigned long)guiFont_Tahoma_10_Regular[font_pointer+3]*65536;  // get offset high and force long type calculation
            font_offset += row;                                                 // add in the row offset
            if(last_width > 8){                                                 // if width > 8 then we need 2 bytes per row
                font_offset += row;                                             // so double up byte offset
            }
            font_row = guiFont_Tahoma_10_Regular[font_offset];                  // get a row of up to 8 pixels
            if(last_width > 8){                                                 // if width > 8 then need second byte of pixels
                font_row += (guiFont_Tahoma_10_Regular[font_offset+1])*256;     // so go fetch a further byte
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 14){
        font_nrows = guiFont_Tahoma_14_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Tahoma_14_Regular[font_pointer];               // character width
            font_offset = guiFont_Tahoma_14_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Tahoma_14_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Tahoma_14_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Tahoma_14_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Tahoma_14_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Tahoma_14_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 16){
        font_nrows = guiFont_Tahoma_16_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Tahoma_16_Regular[font_pointer];               // character width
            font_offset = guiFont_Tahoma_16_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Tahoma_16_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Tahoma_16_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Tahoma_16_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Tahoma_16_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Tahoma_16_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }

    if (OLED_font == 18){
        font_nrows = guiFont_Tahoma_18_Regular[6];

        ascii_off = ascii_in & 0x7f;                                            // map high values to low ones
        if(ascii_off >= 32){                                                    // always assume character 0x20 space as first character
            ascii_off -= 32;                                                    // remap
        }
        for(row = font_nrows; row > 0; row--){
            font_pointer = (ascii_off*4)+8;                                     // skip header
            last_width = guiFont_Tahoma_18_Regular[font_pointer];               // character width
            font_offset = guiFont_Tahoma_18_Regular[font_pointer+1];            // 3 bytes offset value
            font_offset |= guiFont_Tahoma_18_Regular[font_pointer+2] << 8;
            font_offset |= (unsigned long)guiFont_Tahoma_18_Regular[font_pointer+3] << 16;
            font_offset += row;
            if(last_width > 8){                                                 // two bytes in each row
                font_offset += row;
            }
            if(last_width > 16){                                                // 3 bytes in each row
                font_offset += row;
            }
            font_row = guiFont_Tahoma_18_Regular[font_offset];                  // build up row from 1, 2 or 3 bytes
            if(last_width > 8){
                font_row |= (guiFont_Tahoma_18_Regular[font_offset+1]) << 8;
            }
            if(last_width > 16){
                font_row |= (unsigned long)(guiFont_Tahoma_18_Regular[font_offset+2]) << 16;   // force long arithmetic
            }
            for(col = 0; col <= last_width ; col++){                            // count through pixels in row
                DDRAM_access();
                if((font_row >> col)&0x01){
                    drawPixel(x_co_ord+col,y_co_ord+row,color);                 // if bit is set, draw coloured pixel
                }
                else{
                    drawPixel(x_co_ord+col,y_co_ord+row,Color_Black);           // if bit is clear, draw black pixel
                }
            }
        }
        if( display_x <= (95-last_width)){                                      // are we still within display?
            display_x += last_width;                                            // if so move x coordinate right
            display_x++;                                                        // inter-character gap
        }
        else{
            display_x = 95;                                                     // set right hand edge
        }
    }


}

void drawPixel(char px, char py, uint16_t color){
	char swapped_x;

    if(px <96){                                                                 // check on screen
        swapped_x = 95 - px;                                                    // move x coordinate left/right
    }
    else{
        swapped_x = 95;                                                         // left side of screen
    }
    OLED_C_MemorySize( swapped_x,  swapped_x,  py,  py);                        // set display position
    DDRAM_access();                                                             // select data memory on display
    OLED_C_Color(color >> 8,color & 0xff);                                      // white
}


void drawFullScreenBitmap(uint8_t bitmap[], int size_of_bitmap){
//	startscroll();
	int y_offset = 0;
    OLED_C_command(0x1D,0x02);                                                  // Set Memory Read/Write mode
    OLED_C_MemorySize(0x00,0x5F,0x00,0x5F);                                     // whole screen
    DDRAM_access();
    int counter = 0;
	for(int i=size_of_bitmap; i>=1; i--)
	{
		counter++;
		if((size_of_bitmap>96*96*2)&&(counter>=96*96))
		{                                            // Set Memory Read/Write mode
//		    OLED_C_MemorySize(0x00,0x5F,0x00,0x5F);
//		    DDRAM_access();// whole screen
			y_offset=y_offset+2;
			counter = 0;
			i=size_of_bitmap-y_offset*96;
		}
		OLED_C_Color(bitmap[i--],bitmap[i]);
	}


}
void drawPokemonAndPokedex(uint8_t bitmap[], int size_of_bitmap, char *string, int size_of_string){
	drawFullScreenBitmap(bitmap , size_of_bitmap);
	HAL_Delay(3000);
	drawString(0x00, 0x00, string);

}


void drawString(char x_pos, char y_pos, char *text){                              // output text to OLED using current font and colour
char ochar;
unsigned int opoint = 0;

    display_x = x_pos;
    display_y = y_pos;
    do{
        ochar = text[opoint];                                                   // get next chc in string
        if(ochar){
            if((display_x>50&&text[opoint]==' ')||text[opoint]=='\n')
            {
            	opoint++;
            	display_y = display_y + font_nrows;
            	display_x = 0;
            	continue;
            }// do not process null terminator
            drawChar(display_x,display_y,ochar,display_color,display_font);
            opoint++;

           }
            	// go for next chc

    }while(ochar);                                                              // loop until null terminator found
    display_y = display_y + font_nrows;
}


