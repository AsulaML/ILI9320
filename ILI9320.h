#ifndef ILI9320_h
#define ILI9320_h

    #include "main.h"

    /* Private define ------------------------------------------------------------*/
    #define MAX_X  240UL
    #define MAX_Y  320UL  


    #define DP0  LATEbits.LATE0  //
    #define DP1  LATEbits.LATE1
    #define DP2  LATEbits.LATE2
    #define DP3  LATEbits.LATE3
    #define DP4  LATEbits.LATE4
    #define DP5  LATEbits.LATE5

    #define DP6  LATDbits.LATD0
    #define DP7  LATDbits.LATD1
    #define DP8  LATDbits.LATD2

    #define DP9  LATDbits.LATD3  //

    #define DP10 LATDbits.LATD4
    #define DP11 LATDbits.LATD5
    #define DP12 LATDbits.LATD6
    #define DP13 LATDbits.LATD7
    #define DP14 LATDbits.LATD8
    #define DP15 LATDbits.LATD9
    #define DP16 LATDbits.LATD10
    #define DP17 LATDbits.LATD11


    #define DP_WR LATGbits.LATG12     // Write
    #define DP_RD LATGbits.LATG14     // Read
    #define DP_CS LATDbits.LATD12     // Chip select
    #define DP_RS LATGbits.LATG13     // Register Selection

    #define DP_RESET LATEbits.LATE6   // RESET


    void init_ILI9320(void);
    void LCD_WritePicture(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *LCD_Datas);
    // Fonctions élémentaires de gestion du bus parallèle
    uint16_t LCD_read();
    //void LCD_write(uint16_t Data);   // interface 16 bits

    // Fonctions de gestion RD WR RS
    void LCD_WriteIndex(uint8_t index);
    void LCD_ReadData( uint16_t *LCD_Datas, uint16_t Size);

    // Fonctions de plus haut niveau
    void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
    uint16_t LCD_ReadReg(uint8_t LCDReg);
    void LCD_write();
    void LCD_WriteCOLOR( uint16_t COLOR, uint16_t Size);
    void LCD_WriteData( uint16_t *LCD_Datas, uint16_t Size);
    void  LCD_COLOR(uint16_t color);

    // Fonctions de controle du LCD
    void ili9320_SetCursor(uint16_t Xpos, uint16_t Ypos);
    void ili9320_DisplayOn(void);
    void ili9320_DisplayOff(void);

    // Fonctions graphiques
    void ili9320_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
    //void ili9320_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);
    void ili9320_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);

#endif
