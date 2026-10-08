#include "types.h"
#include "io_reg.h"

extern const u32 ColorBuffer_ScaleThreeQuartersKernelCode[48];
extern const u32 ColorBuffer_HalveKernelCode[32];
extern const u32 ColorBuffer_BrightenKernelCode[34];
extern const u32 ColorBuffer_DarkenKernelCode[45];

struct ColorDmaDraftChannel {
    const void *source;
    void *destination;
    u32 control;
};

void ColorBuffer_ScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 code[48];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_ScaleThreeQuartersKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 48;
    ((void (*)(u8 *, u8 *, u32))code)(buffer, backup, bytes);
}

void ColorBuffer_Halve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 code[32];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_HalveKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 32;
    ((void (*)(u8 *, u8 *, u32))code)(buffer, backup, bytes);
}

void ColorBuffer_Brighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 code[34];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_BrightenKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 34;
    ((void (*)(u8 *, u32, u8 *, u32))code)(buffer, amount, backup, bytes);
}

void ColorBuffer_Darken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 code[45];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_DarkenKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 45;
    ((void (*)(u8 *, u32, u8 *, u32))code)(buffer, amount, backup, bytes);
}
