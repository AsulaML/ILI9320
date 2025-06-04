// === ili9320_driver_refactor.c ===
#include "ILI9320.h"

// === Bas niveau: Contrôle du bus ===
static void ILI9320_SetDataBusOutput(void) {
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
}

static void ILI9320_SetDataBusInput(void) {
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
}

static void ILI9320_Output16(uint16_t value) {
    LATE = (LATE & 0xFFC0) | ((value << 1) & 0x003E);
    LATD = (LATD & 0xFFC0) | ((value >> 5) & 0x003F);
    LATD = (LATD & 0xF03F) | ((value >> 4) & 0x0FC0);
}

static uint16_t ILI9320_Input16(void) {
    uint16_t data = 0;
    data  = ((PORTE >> 1) & 0x001F);
    data |= ((PORTD & 0x0007) << 5);
    data |= ((PORTD & 0x0FF0) << 4);
    return data;
}

// === Communication ILI9320 ===
void ILI9320_WriteCommand(uint8_t cmd) {
    DP_RD = 1;
    DP_RS = 0;
    ILI9320_SetDataBusOutput();
    ILI9320_Output16(cmd);
    DP_CS = 0;
    DP_WR = 0;
    Nop();
    DP_WR = 1;
    DP_RS = 1;
}

void ILI9320_WriteData(uint16_t data) {
    ILI9320_SetDataBusOutput();
    ILI9320_Output16(data);
    Nop();
    DP_WR = 0;
    DP_WR = 1;
}

void ILI9320_WriteReg(uint8_t reg, uint16_t value) {
    ILI9320_WriteCommand(reg);
    ILI9320_WriteData(value);
    DP_CS = 1;
    ILI9320_SetDataBusInput();
}

uint16_t ILI9320_ReadReg(uint8_t reg) {
    uint16_t value;

    DP_RD = 1;
    DP_WR = 1;
    DP_RS = 0;
    ILI9320_SetDataBusOutput();
    ILI9320_Output16(reg);

    DP_CS = 0;
    DP_WR = 0;
    Nop();
    DP_WR = 1;
    DP_RS = 1;

    ILI9320_SetDataBusInput();
    Nop();

    DP_RD = 0;
    Nop();
    value = ILI9320_Input16();
    DP_RD = 1;

    DP_CS = 1;
    return value;
}

void ILI9320_ReadData(uint16_t *buffer, uint32_t size) {
    ILI9320_WriteCommand(0x22);
    ILI9320_SetDataBusInput();

    for (uint32_t i = 0; i < size; i++) {
        DP_RD = 0;
        Nop();
        buffer[i] = ILI9320_Input16();
        DP_RD = 1;
    }

    DP_CS = 1;
}

// === Initialisation simplifiée ===
static void ILI9320_Reset(void) {
    DP_RESET = 1;
    __delay_ms(1);
    DP_RESET = 0;
    __delay_ms(10);
    DP_RESET = 1;
    __delay_ms(50);
}

static void ILI9320_ConfigurePower(void) {
    ILI9320_WriteReg(0x10, 0x0000);
    ILI9320_WriteReg(0x11, 0x0000);
    ILI9320_WriteReg(0x12, 0x0000);
    ILI9320_WriteReg(0x13, 0x0000);
    __delay_ms(200);
    ILI9320_WriteReg(0x10, 0x17B0);
    ILI9320_WriteReg(0x11, 0x0137);
    __delay_ms(50);
    ILI9320_WriteReg(0x12, 0x0139);
    __delay_ms(50);
    ILI9320_WriteReg(0x13, 0x1B00);
    ILI9320_WriteReg(0x29, 0x000C);
    ILI9320_WriteReg(0x2B, 0x0000);
    __delay_ms(50);
}

static void ILI9320_ConfigureDisplay(void) {
    ILI9320_WriteReg(0x01, 0x0100);
    ILI9320_WriteReg(0x02, 0x0700);
    ILI9320_WriteReg(0x03, 0x10B0);
    ILI9320_WriteReg(0x04, 0x0000);
    ILI9320_WriteReg(0x08, 0x0202);
    ILI9320_WriteReg(0x09, 0x0000);
    ILI9320_WriteReg(0x0A, 0x0000);
    ILI9320_WriteReg(0x0C, 0x0000);
    ILI9320_WriteReg(0x0D, 0x0000);
    ILI9320_WriteReg(0x0F, 0x0000);
}

static void ILI9320_SetGammaCurve(void) {
    ILI9320_WriteReg(0x30, 0x0006);
    ILI9320_WriteReg(0x31, 0x0101);
    ILI9320_WriteReg(0x32, 0x0003);
    ILI9320_WriteReg(0x35, 0x0106);
    ILI9320_WriteReg(0x36, 0x0B02);
    ILI9320_WriteReg(0x37, 0x0302);
    ILI9320_WriteReg(0x38, 0x0707);
    ILI9320_WriteReg(0x39, 0x0007);
    ILI9320_WriteReg(0x3C, 0x0600);
    ILI9320_WriteReg(0x3D, 0x020B);
}

static void ILI9320_ConfigureGRAM(void) {
    ILI9320_WriteReg(0x50, 0x0000);
    ILI9320_WriteReg(0x51, 0x00EF);
    ILI9320_WriteReg(0x52, 0x0000);
    ILI9320_WriteReg(0x53, 0x013F);
    ILI9320_WriteReg(0x60, 0x2700);
    ILI9320_WriteReg(0x61, 0x0001);
    ILI9320_WriteReg(0x6A, 0x0000);
}

static void ILI9320_ConfigurePanelControl(void) {
    ILI9320_WriteReg(0x80, 0x0000);
    ILI9320_WriteReg(0x81, 0x0000);
    ILI9320_WriteReg(0x82, 0x0000);
    ILI9320_WriteReg(0x83, 0x0000);
    ILI9320_WriteReg(0x84, 0x0000);
    ILI9320_WriteReg(0x85, 0x0000);
    ILI9320_WriteReg(0x90, 0x0010);
    ILI9320_WriteReg(0x92, 0x0000);
    ILI9320_WriteReg(0x93, 0x0003);
    ILI9320_WriteReg(0x95, 0x0110);
    ILI9320_WriteReg(0x97, 0x0000);
    ILI9320_WriteReg(0x98, 0x0000);
}

void ILI9320_SetCursor(uint16_t x, uint16_t y) {
    ILI9320_WriteReg(0x20, y);
    ILI9320_WriteReg(0x21, x);
}

void ILI9320_WritePicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    uint32_t size = (uint32_t)w * h;
    ILI9320_WriteReg(0x52, x);
    ILI9320_WriteReg(0x53, x + w - 1);
    ILI9320_WriteReg(0x50, y);
    ILI9320_WriteReg(0x51, y + h - 1);
    ILI9320_WriteReg(0x03, 0x10A0);
    ILI9320_SetCursor(x, y);
    ILI9320_WriteCommand(0x22);
    for (uint32_t i = 0; i < size; i++) {
        ILI9320_WriteData(data[i]);
    }
    DP_CS = 1;
}

void ILI9320_FillColor(uint16_t color, uint32_t size) {
    ILI9320_WriteCommand(0x22);
    for (uint32_t i = 0; i < size; i++) {
        ILI9320_WriteData(color);
    }
    DP_CS = 1;
}

void ILI9320_DisplayOn(void) {
    ILI9320_WriteReg(0x07, 0x0173);
}

void ILI9320_DisplayOff(void) {
    ILI9320_WriteReg(0x07, 0x0000);
}

void init_ILI9320(void) {
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    TRISGbits.TRISG12 = 0;
    TRISGbits.TRISG14 = 0;
    TRISDbits.TRISD12 = 0;
    TRISGbits.TRISG13 = 0;
    TRISEbits.TRISE6 = 0;

    DP_CS = 1;
    DP_WR = 1;
    DP_RD = 1;
    DP_RS = 1;
    DP_RESET = 1;

    ILI9320_Reset();
    ILI9320_WriteReg(0xE5, 0x8000);
    ILI9320_WriteReg(0x00, 0x0001);
    __delay_ms(50);

    ILI9320_ConfigureDisplay();
    ILI9320_ConfigurePower();
    ILI9320_SetGammaCurve();
    ILI9320_ConfigureGRAM();
    ILI9320_ConfigurePanelControl();

    ILI9320_DisplayOn();
    __delay_ms(20);

    ILI9320_SetCursor(0, 0);
}
