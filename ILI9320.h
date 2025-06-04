#ifndef ILI9320_H
#define ILI9320_H

#include <xc.h>
#include <stdint.h>

// Dimensions de l'écran
#define ILI9320_WIDTH  240
#define ILI9320_HEIGHT 320

// Contrôle du bus parallèle
#define DP_CS     LATDbits.LATD12
#define DP_RS     LATGbits.LATG13
#define DP_WR     LATGbits.LATG12
#define DP_RD     LATGbits.LATG14
#define DP_RESET  LATEbits.LATE6

// Fonctions publiques
void init_ILI9320(void);

// Contrôle du LCD
void ILI9320_DisplayOn(void);
void ILI9320_DisplayOff(void);

// Fonctions registre
void ILI9320_WriteCommand(uint8_t cmd);
void ILI9320_WriteData(uint16_t data);
void ILI9320_WriteReg(uint8_t reg, uint16_t value);
uint16_t ILI9320_ReadReg(uint8_t reg);

// Données
void ILI9320_ReadData(uint16_t *buffer, uint32_t size);

// Curseur et affichage
void ILI9320_SetCursor(uint16_t x, uint16_t y);
void ILI9320_WritePicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
void ILI9320_FillColor(uint16_t color, uint32_t size);

// Fonctions bas niveau (optionnellement privées, à déplacer si nécessaire)
void ILI9320_SetDataBusOutput(void);
void ILI9320_SetDataBusInput(void);
void ILI9320_Output16(uint16_t value);
uint16_t ILI9320_Input16(void);

#endif // ILI9320_H