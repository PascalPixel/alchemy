/* Draft, differ 2 of 18: the native loads the flag bytes right after the
 * mask constant (movs r3, #7; ldr r2; ands r3, r4); this loads them after
 * the mask is built. Measured: the neighbours' barrier on bit and bytes
 * (GameFlag_SetBit) puts the and before the load (differ 2); the 7 set
 * before that barrier and anded after it gives the native order but swaps
 * the registers of the bytes and the mask (differ 16); bytes first is
 * differ 18; six minutes of permuter in round one found nothing. */
#include "types.h"
extern u8 GameFlagBytes[];

u32 GameFlag_Toggle(s32 flag)
{
    s32 flag_mask;
    u8 *flag_bytes;
    s32 flag_value;

    flag_mask = 1 << (7 & flag);
    flag_bytes = (u8 *)GameFlagBytes;
    flag = ((u32)flag << 0x14) >> 0x17;
    flag_bytes[flag] = (u8)(flag_bytes[flag] ^ flag_mask);
    flag_value = flag_bytes[flag] & flag_mask;
    return (u32)((0 - flag_value) | flag_value) >> 31;
}
