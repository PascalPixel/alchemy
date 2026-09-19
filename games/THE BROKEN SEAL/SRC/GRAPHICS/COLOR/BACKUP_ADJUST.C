#include "DMA.H"

/* Linker sizes determine the stack allocation for the copied ARM entries. */
extern u8 ColorBuffer_BackupAndBrightenCodeSize[];
extern u8 ColorBuffer_BackupAndDarkenCodeSize[];
extern u8 ColorBuffer_BackupAndScaleThreeQuartersCodeSize[];
extern u8 ColorBuffer_BackupAndHalveCodeSize[];
extern u8 ColorBuffer_BackupAndBrightenPartialCodeSize[];
extern u8 ColorBuffer_BackupAndDarkenPartialCodeSize[];
extern u8 ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize[];
extern u8 ColorBuffer_BackupAndHalveNonzeroCodeSize[];

void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x8001fb8, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x8001f38, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x8001ea8, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x8001ef8, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/* Words containing four maximum-intensity bytes are left untouched. */
void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x80021c4, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/* Zero words are left untouched. */
void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x80020f4, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x800203c, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x8002098, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}
