

#include "ILI9320.h"


uint8_t Is_ili9320_Initialized = 0;

void LCD_write()
{
    
}

void LCD_WritePicture(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *LCD_Datas)
{
    ili9320_SetCursor(Xpos, Ypos);      // position de l'image
    
    uint8_t LCD_Reg = 0x22;             // Ecriture dans la Gram
    uint16_t i;
    uint16_t Size = Xsize * Ysize;
    
    LCD_WriteReg(0x03, 0x10A0); // set GRAM write direction and BGR=1.
    
    LCD_WriteReg(0x52, Xpos);     // Horizontal Address Start Position
    LCD_WriteReg(0x53, Xpos+Xsize-1);     // Horizontal Address End Position
    LCD_WriteReg(0x50, Ypos);     // Vertical Address Start Position 
    LCD_WriteReg(0x51, Ypos+Ysize-1);     // Vertical Address End Position 
   // ili9320_SetCursor(0, 0);
    
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    for (i = 0; i < Size; i++) 
    {   
        
        // en sortie
        TRISE &= 0xFFC0;
        TRISD &= 0xF000;
        Nop();
        
        DP_WR = 1;
        DP_RS = 1;
        
        // LSBE  E0, E1, E2, E3, E4, E5 => DB0, DB1, DB2, DB3, DB4, DB5 = 6 bleu
        LATE = (LATE & 0xFFC0) + ((LCD_Datas[i]<<1) & 0x003E) + 1;

        // LSBD D0, D1, D2, D3, D4, D5, D6 to DB6, DB7, DB8, DB9, DB10, DB11
        LATD = (LATD & 0xFFC0) + ((LCD_Datas[i]>>5) & 0x003F);

        // MSBD to D12 => DP17
        LATD = (LATD & 0xF03F) + ((LCD_Datas[i]>>4) & 0x0FC0);

        // LSBD to D6, D7, D8, D9
        //LATD = (LATD & 0xFFF0) + ((LCD_Datas[i]>>5) & 0x0007);

        // MSBD to D10 => DP17
        //LATD = (LATD & 0xF00F) + ((LCD_Datas[i]>>4) & 0x0FF0);
        
        Nop();
        Nop();
        
        // Deuxième partie écriture de la donnée
        DP_WR = 0;

        //DP_WR = 1;
        
        
                   /*
        if(counter_y >= (Ysize-1))
        {
            counter_x = counter_x + 1;
            
            ili9320_SetCursor(counter_x+Xpos, Ypos);
            counter_y = 0;      
            
            
            // 1 cycle 100ns
            // Première partie écriture de l'index
            DP_RD = 1;
            DP_RS = 0;

            // LSBE to DP1, DP2, DP3, DP4, DP5
            LATE = (LATE & 0xFFC0) + ((0x22<<1) & 0x003E);

            // LSBD to DP6, DP7, DP8
            LATD = (LATD & 0xF000) + ((0x22>>5) & 0x0007);

            DP_CS = 0;
            DP_WR = 0;
        }
        
        counter_y = counter_y + 1;*/
    }
    
    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;     
}

void  LCD_COLOR(uint16_t color)
{
    uint16_t t = 0;
    for(t = 0; t<=320; t++)
    {
        ili9320_SetCursor(t, 0);
        LCD_WriteCOLOR(color, 240);
    }    
}


void LCD_WriteRegAddress( uint8_t Address)
{
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((Address<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((Address>>5) & 0x0007);

    Nop();
    
	DP_CS = 0;
	DP_WR = 0;

    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
}


void LCD_WriteCOLOR(uint16_t COLOR, uint16_t Size)
{
    uint8_t LCD_Reg = 0x22;
    uint16_t i;
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    Nop();
    
	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    for (i = 0; i < Size; i++) {
        
        // LSBE to DP1, DP2, DP3, DP4, DP5
        LATE = (LATE & 0xFFC1) + ((COLOR<<1) & 0x003E);

        // LSBD to DP6, DP7, DP8
        LATD = (LATD & 0xFFF8) + ((COLOR>>5) & 0x0007);

        // MSBD to D10 => DP17
        LATD = (LATD & 0xF00F) + ((COLOR>>4) & 0x0FF0);
        
        Nop();
        
        // Deuxième partie écriture de la donnée
        DP_WR = 0;

        DP_WR = 1;
    }
    
    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;     
}

uint16_t dataread[100];

void LCD_ReadData( uint16_t *LCD_Datas, uint16_t Size)
{
    uint8_t LCD_Reg = 0x22;
    uint8_t i = 0;
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    
    for (i = 0; i < 100; i++) {

           // en 3
        TRISE |= 0x003F;
        TRISD |= 0x0FFF;

        // Deuxième partie lecture de la donnée
        DP_RD = 0;

        dataread[i] = ((PORTE>>1) & 0x001F);
        dataread[i] += ((PORTD & 0x0007) << 5);
        dataread[i] += ((PORTD & 0x0FF0) << 4);

        //datae = PORTE;
        //datad = PORTD;
        DP_RD = 1;
    }

    DP_CS = 1;   

}

void LCD_WriteData( uint16_t *LCD_Datas, uint16_t Size)
{
    uint8_t LCD_Reg = 0x22;
    uint16_t i;
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;
    Nop();
    
    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    Nop();
	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    for (i = 0; i < Size; i++) {

        // LSBE to DP1, DP2, DP3, DP4, DP5
        LATE = (LATE & 0xFFC1) + ((LCD_Datas[i]<<1) & 0x003E);

        // LSBD to DP6, DP7, DP8
        LATD = (LATD & 0xFFF8) + ((LCD_Datas[i]>>5) & 0x0007);

        // MSBD to D10 => DP17
        LATD = (LATD & 0xF00F) + ((LCD_Datas[i]>>4) & 0x0FF0);
        
        Nop();
        
        // Deuxième partie écriture de la donnée
        DP_WR = 0;

        DP_WR = 1;
    }
    
    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;     
}

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
    /*
    uint16_t datae = 0;
    uint16_t datad = 0;
    // LSBE to DP1, DP2, DP3, DP4, DP5
    datae = (datae & 0xFFC1) + ((LCD_RegValue<<1) & 0x003E);
    // LSBD to DP6, DP7, DP8
    datad = (datad & 0xFFF8) + ((LCD_RegValue>>5) & 0x0007);
    // MSBD to D10 => DP17
    datad = (datad & 0xF00F) + ((LCD_RegValue>>4) & 0x0FF0);
    */
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;
    Nop();
    
    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    Nop();
	DP_CS = 0;
	DP_WR = 0;
   
	/* Write 16-bit Index */    

    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    
    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC1) + ((LCD_RegValue<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xFFF8) + ((LCD_RegValue>>5) & 0x0007);

    // MSBD to D10 => DP17
    LATD = (LATD & 0xF00F) + ((LCD_RegValue>>4) & 0x0FF0);
  
    // Deuxième partie écriture de la donnée
	DP_WR = 0;

    DP_WR = 1;

    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
	DP_CS = 1;     
}

uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
	uint16_t data = 0;

	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
    DP_WR = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    // MSBD to D10 => DP17
    //LATD = (LATD & 0xF000) + ((LCD_Reg>>6) & 0x0FF0);

	DP_CS = 0;

	DP_WR = 0;
    
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
	Nop();
            
   
	/* Write 16-bit Index */    
    
    DP_WR = 1;
    DP_RS = 1;    

        // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    // Deuxième partie lecture de la donnée
	DP_RD = 0;

    data = ((PORTE>>1) & 0x001F);
    data += ((PORTD & 0x0007) << 5);
    data += ((PORTD & 0x0FF0) << 4);
    
    DP_RD = 1;
	DP_CS = 1;

    //__delay_ms(50);
    
	return(data);     
}



void ili9320_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  LCD_WriteReg(0x20, Ypos);
  LCD_WriteReg(0x21, Xpos);      
}

void init_ILI9320(void)
{
    uint16_t data;
    
    
    //ANSDbits.ANSD6 = 0;
    //ANSDbits.ANSD7 = 0;
    
    // Bits du bus parallèle 
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;
    DP_WR = 1;
    DP_RD = 1;    
    DP_RS = 1; 
    DP_RESET = 1;
    
    // Screen Control 
    TRISGbits.TRISG12 = 0; // DP_WR
    TRISGbits.TRISG14 = 0; // DP_RD
    TRISDbits.TRISD12 = 0; // DP_CS
    TRISGbits.TRISG13 = 0; // DP_RS
    TRISEbits.TRISE6 = 0;  // DP_RESET

    data = LCD_ReadReg(0x00);    
    
    // VCI=2.8V
    // ************* Reset LCD Driver ****************//
    DP_RESET = 1;
    __delay_ms(1); // Delay 1ms
    DP_RESET = 0;
    __delay_ms(10); // Delay 50ms // This delay time is necessary
    DP_RESET = 1;
    __delay_ms(50); // Delay 50 ms

   
    // ************* Start Initial Sequence **********//
    LCD_WriteReg(0xE5, 0x8000); // Set the Vcore voltage and this setting is must.
    
    LCD_WriteReg(0x00, 0x0001); // Start internal OSC.
    __delay_ms(50); // Wait at least 10ms to let the frequency of oscillator stable and then do the other function setting.
  
    __delay_ms(50); // Wait at least 10ms to let the frequency of oscillator stable and then do the other function setting.
    LCD_WriteReg(0x01, 0x0100); // set SS and SM bit
    LCD_WriteReg(0x02, 0x0700); // set 1 line inversion
    LCD_WriteReg(0x03, 0x10B0); // set GRAM write direction and BGR=1.

    LCD_WriteReg(0x04, 0x0000); // Resize register

    LCD_WriteReg(0x08, 0x0202); // set the back porch and front porch
    LCD_WriteReg(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_WriteReg(0x0A, 0x0000); // FMARK function
    LCD_WriteReg(0x0C, 0x0000); // RGB interface setting
    LCD_WriteReg(0x0D, 0x0000); // Frame marker Position
    LCD_WriteReg(0x0F, 0x0000); // RGB interface polarity  
    __delay_ms(50);   
    
    
    //LCD_WriteReg(0x07, 0x0101); // Display control: VCOM to GND; Internal operations On
    //__delay_ms(50);   
    
    //*************Power On sequence ****************//
    LCD_WriteReg(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteReg(0x11, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_WriteReg(0x12, 0x0000); // VREG1OUT voltage
    LCD_WriteReg(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
    __delay_ms(200);            // Dis-charge capacitor power voltage
    LCD_WriteReg(0x10, 0x17B0); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteReg(0x11, 0x0137); // R11h=0x0031 at VCI=3.3V DC1[2:0], DC0[2:0], VC[2:0]
    __delay_ms(50);               // Delay 50ms
    LCD_WriteReg(0x12, 0x0139); // R12h=0x0138 at VCI=3.3V VREG1OUT voltage
    __delay_ms(50);               // Delay 50ms
    LCD_WriteReg(0x13, 0x1B00); // R13h=0x1800 at VCI=3.3V VDV[4:0] for VCOM amplitude
    LCD_WriteReg(0x29, 0x000C); // R29h=0x0008 at VCI=3.3V VCM[4:0] for VCOMH
    LCD_WriteReg(0x2B, 0x0000); // 
    __delay_ms(50);
    
    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x21, 0x0000); // GRAM Vertical Address

    // ----------- Adjust the Gamma Curve ----------//
    LCD_WriteReg(0x30, 0x0006);
    LCD_WriteReg(0x31, 0x0101);
    LCD_WriteReg(0x32, 0x0003);

    LCD_WriteReg(0x35, 0x0106);
    LCD_WriteReg(0x36, 0x0B02);
    LCD_WriteReg(0x37, 0x0302);
    LCD_WriteReg(0x38, 0x0707);
    LCD_WriteReg(0x39, 0x0007);

    LCD_WriteReg(0x3C, 0x0600);
    LCD_WriteReg(0x3D, 0x020B); 

    //------------------ Set GRAM area ---------------//
    LCD_WriteReg(0x50, 0x0000); // Horizontal GRAM Start Address
    LCD_WriteReg(0x51, 0x00EF); // Horizontal GRAM End Address
    LCD_WriteReg(0x52, 0x0000); // Vertical GRAM Start Address
    LCD_WriteReg(0x53, 0x013F); // Vertical GRAM Start Address

    LCD_WriteReg(0x60, 0x2700); // Gate Scan Line
    LCD_WriteReg(0x61, 0x0001); // NDL,VLE, REV
    LCD_WriteReg(0x6A, 0x0000); // set scrolling line

    //-------------- Partial Display Control ---------//
    LCD_WriteReg(0x80, 0x0000);
    LCD_WriteReg(0x81, 0x0000);
    LCD_WriteReg(0x82, 0x0000);
    LCD_WriteReg(0x83, 0x0000);
    LCD_WriteReg(0x84, 0x0000);
    LCD_WriteReg(0x85, 0x0000);


    //-------------- Panel Control -------------------//
    LCD_WriteReg(0x90, 0x0010);
    LCD_WriteReg(0x92, 0x0000);
    LCD_WriteReg(0x93, 0x0003);
    LCD_WriteReg(0x95, 0x0110);
    LCD_WriteReg(0x97, 0x0000);
    LCD_WriteReg(0x98, 0x0000);
    
   


   // LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    
  //  LCD_WriteReg(0x20, 0x0001); // GRAM horizontal Address
  //  LCD_WriteReg(0x21, 0x0000); // GRAM Vertical Address
    

    
  //  LCD_ReadData(image_data_D, 1000);
  //  __delay_ms(50);
    
    
    //-------------- 262K color and display ON --------------//
    LCD_WriteReg(0x07, 0x0021);
    __delay_ms(20);   
    LCD_WriteReg(0x07, 0x0061);
    __delay_ms(20);   
   LCD_WriteReg(0x07, 0x0173);  
    __delay_ms(20);
    
//    data = LCD_ReadReg(0x00);    
//    __delay_ms(50);    
/*data = LCD_ReadReg(0x00);    
__delay_ms(50);    

    LCD_WriteData(image_data_D, 100);
    
    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x21, 0x0000); // GRAM Vertical Address
   
    LCD_ReadData(image_data_D, 100);

    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x21, 0x0000); // GRAM Vertical Address
   */
   
    
//    data = LCD_ReadReg(0x00);
//            
//    __delay_ms(50);
//
//    LCD_ReadData(image_data_D, 1000);
//       
//    data = LCD_ReadReg(0x00);
//    
//    LCD_WriteReg(0x07, 0x0173);  
    
    /* Set the Cursor */ 
    ili9320_SetCursor(0, 0);
}

void ili9320_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  LCD_WriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(0x11, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(0x12, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude*/

  LCD_WriteReg(0x10, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(0x11, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */

  LCD_WriteReg(0x12, 0x0139); /* VREG1OUT voltage */

  LCD_WriteReg(0x13, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  LCD_WriteReg(0x29, 0x0013); /* VCM[4:0] for VCOMH */
 
  /* Display On */
  LCD_WriteReg(0x07, 0x0173); /* 262K color and display ON */
}

void ili9320_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  LCD_WriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(0x11, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(0x12, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude*/
  
  LCD_WriteReg(0x29, 0x0000); /* VCM[4:0] for VCOMH */
  
  /* Display Off */
  LCD_WriteReg(0X07, 0x0); 
}




/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  */
void ili9320_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_write(0x22);

  /* Write 16-bit GRAM Reg */
  LCD_write(RGBCode);
}



/**
  * @brief  Displays a bitmap picture..
  * @param  BmpAddress: Bmp picture address.
  * @param  Xpos:  Bmp X position in the LCD
  * @param  Ypos:  Bmp Y position in the LCD    
  * @retval None
  */
void ili9320_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  /* Read bitmap size */
  size = *(volatile uint32_t *) (pbmp + 2);
  size |= (*(volatile uint32_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(volatile uint32_t *) (pbmp + 10);
  index |= (*(volatile uint32_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;
  /* Set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  LCD_WriteReg(0x03, 0x1008);

  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  LCD_write(0x22);
 
  for(index = 0; index < size; index++)
  {
    /* Write 16-bit GRAM Reg */
    LCD_write(*(volatile uint16_t *)pbmp);
    pbmp += 2;
  }
 
  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  LCD_WriteReg(0x03, 0x1018);
}


///**
//  * @brief  Displays picture..
//  * @param  pdata: picture address.
//  * @param  Xpos:  Image X position in the LCD
//  * @param  Ypos:  Image Y position in the LCD
//  * @param  Xsize: Image X size in the LCD
//  * @param  Ysize: Image Y size in the LCD
//  * @retval None
//  */
//void ili9320_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
//{
//  uint32_t index = 0, size = 0;
//
//  size = (Xsize * Ysize);
//
//  /* Set Cursor */
//  ili9320_SetCursor(Xpos, Ypos);  
//  
//  /* Prepare to write GRAM */
//  LCD_write(0x22);
// 
//  for(index = 0; index < size; index++)
//  {
//
//    /* Write 16-bit GRAM Reg */
//    LCD_WriteData(pdata, size);
//    pdata += 2;
//  }
//}





