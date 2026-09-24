/* Draft, not exact (2026-09-24): 2 differing halfwords, 148 of 148 bytes.
   Residual: after the DISPCNT store the reference loads the WININ value
   before the WININ address; this C loads the address first. A sweep of
   inline, shared and separate value locals over all five stores (1024
   variants), do-while wraps, a register pointer and non-volatile stores
   all stop at these two halfwords. */
#include "DMA.H"

struct WindowHBlankWork {
    u16 pages[2][322];
    u8 unknown_508[0x31];
    u8 page;
};

extern struct WindowHBlankWork *Data_03001ecc;

void Func_0808f498(void)
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
    value = *source++;
    *(volatile u16 *)0x04000048 = value;
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
