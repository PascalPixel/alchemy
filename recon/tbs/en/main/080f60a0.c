/* Draft, not exact (2026-09-24): 2 differing halfwords, 168 of 168 bytes.
   Residual: before the IWRAM colour-adjust call the reference loads the
   routine address after setting r0; this candidate loads it first. */

#include "DMA.H"

extern u8 *Data_03001eec[2];

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);

void Func_080f60a0(void)
{
    u8 *work;
    u8 *buffer;

    work = Data_03001eec[0];
    if (*(s32 *)(work + 0x7824) == 1) {
        buffer = Data_03001eec[1];
        switch (*(s32 *)(work + 0x7780)) {
        case 1:
            Dma_Set(buffer, (void *)0x06003500, 0x84002000, (volatile u32 *)0x040000d4);
            ((void (*)(u8 *, s32, s32))0x03000168)(buffer, 0x8000, *(s32 *)(work + 0x7784));
            break;
        case 2:
            if (*(s32 *)(work + 0x7784) == 50)
                ColorBuffer_BackupAndHalveNonzero(buffer, (u8 *)0x06003500, 0x8000);
            else
                ColorBuffer_BackupAndScaleNonzeroThreeQuarters(buffer, (u8 *)0x06003500, 0x8000);
            break;
        }
        *(s32 *)(work + 0x7824) = 0;
        *(s32 *)(work + 0x7820) = 1;
    } else {
        (*(s32 *)(work + 0x7820))++;
    }
}
