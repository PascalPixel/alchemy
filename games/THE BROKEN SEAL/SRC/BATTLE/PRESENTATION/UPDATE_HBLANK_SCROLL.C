/* While the presentation mode is 2, restart the H-blank DMA that feeds
   the BG2 control register from the current scroll record, and copy that
   record's affine parameters.

   FAKEMATCH: the BG2CNT value is read into its own temporary and the store
   sits in a one-pass loop, whose loop notes keep the first DMA source
   (record + 34) from being computed before the store. */
#include "DMA.H"

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block. Kind 44
   is the presentation state, kind 10 its scroll records. */
extern void *Data_03001e50[];

void BattlePres_UpdateHBlankScroll(void)
{
    u8 *records;
    u8 *record;

    if (((s32 *)*(void **)((u8 *)Data_03001e50 + 0xb0))[2] == 2) {
        records = *(u8 **)&Data_03001e50[10];
        record = records + *(s32 *)records * 320;
        {
            s32 control = *(u16 *)(record + 32);

            do {
                *(u16 *)0x0400000c = control;
            } while (0);
        }
        Dma_Set(record + 34, (void *)0x0400000c, 0xa2600001, (volatile u32 *)0x040000b0);
        Dma_Set(records + 16, (void *)0x04000020, 0x84000004, (volatile u32 *)0x040000d4);
    }
}
