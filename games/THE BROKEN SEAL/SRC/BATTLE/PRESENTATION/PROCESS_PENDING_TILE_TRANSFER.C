#include "DMA.H"
#include "IWRAM_CALL.H"

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    Iwram_FillWords(dst, size, value);
}

extern u8 *gBattleFxWork[2];

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);

/* Runs the pending BG tile transfer once the effect requests it: a plain
   copy to 0x06003500 followed by a refill, or the halved or three-quarter
   colour backup; otherwise counts the frames since the last transfer. */
void BattlePres_ProcessPendingTileTransfer(void)
{
    u8 *work;
    u8 *buffer;

    work = gBattleFxWork[0];
    if (*(s32 *)(work + 0x7824) == 1) {
        buffer = gBattleFxWork[1];
        switch (*(s32 *)(work + 0x7780)) {
        case 1:
            Dma_Set(buffer, (void *)0x06003500, 0x84002000, (volatile u32 *)0x040000d4);
            FillWords(buffer, 0x8000, *(s32 *)(work + 0x7784));
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
