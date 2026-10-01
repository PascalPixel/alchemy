#include "TYPES.H"
extern u8 Flash_Handler3[];

/* ☀️'s: number every slot node and clear its link, then empty the 256
   bucket heads. */
void AudioTrack_ResetSlotBuckets(void)
{
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    index = 0;
    limit = 0x3FF;
    zero = 0;
    record = *(u8 **)Flash_Handler3 + 4;
    do {
        *(s32 *)(record + 4) = index;
        index++;
        *(s32 *)record = zero;
        record += 12;
    } while (index <= limit);
    slot = (s32 *)(*(u8 **)Flash_Handler3 + 0x3000);
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}
