#include "DMA.H"

struct WindowHBlankWork {
    u16 pages[2][322];
    u8 unknown_508[0x31];
    u8 page;
};

extern struct WindowHBlankWork *Data_03001ecc;

void BattleFx_StartWindowHBlankDma(void)
{
    struct WindowHBlankWork *work = Data_03001ecc;
    u16 *source = work->pages[work->page];
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    s32 value;
    s32 other;

    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    *(volatile u16 *)0x04000000 |= 0x6000;
    /* FAKEMATCH: a do-while(0) around the WININ copy makes the reference load
       the value before the WININ address */
    do {
        value = *source++;
        *(volatile u16 *)0x04000048 = value;
    } while (0);
    value = *source++;
    *(volatile u16 *)0x0400004a = value;
    *(volatile u16 *)0x04000040 = *source++;
    other = *source++;
    *(volatile u16 *)0x04000042 = other;
    other = 160;
    *(volatile u16 *)0x04000044 = other;
    *(volatile u16 *)0x04000046 = other;
    Dma_Set(source, (void *)0x04000040, 0xa6600001, (volatile u32 *)channel);
}
