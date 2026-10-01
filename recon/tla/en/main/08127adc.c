/*
 * Draft: Summon_TakeCharge, ported from its ☀️ twin (⚓️'s 9 channels, the
 * battle work from the heap slot, % for Math_Mod); it goes before
 * Summon_ReleaseCharge in SRC/BATTLE/SUMMON/CHARGE.C. Score 280, three
 * scheduling steps: the listing clears i (movs r1, #0) before loading the
 * count, stores the new channel before copying r7 for the mask offset, and
 * loads the mask before building the bit. Setting i before the count does
 * not move it.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

#define CH_CNT 9

struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];   /* 0x10 */
    u32 used_masks[6];  /* 0x1c */
    s8 channels[6];     /* 0x34 */
    u8 unknown_3a[6];
    u8 count;           /* 0x40 */
};

s32 Summon_TakeCharge(s32 no, s32 n)
{
    struct SummonChargeState *w;
    s32 num;
    s32 i;
    s32 retry;
    s32 ch;

    w = (struct SummonChargeState *)Ram_HeapSlots->battle_work;
    num = w->count;
    for (i = 0; i < num; i++) {
        if (w->class_ids[i] == no)
            break;
    }
    if (i != num) {
        retry = 0;
        if (w->channels[i] < 0) {
            w->channels[i] = 1;
            w->used_masks[i] = 3;
            return 0x8001;
        }
        for (; retry <= 31; retry++) {
            ch = (w->channels[i] + 1) % CH_CNT;
            w->channels[i] = ch;
            if ((w->used_masks[i] & (1 << (s8)ch)) == 0)
                break;
        }
        w->used_masks[i] |= 1 << w->channels[i];
        return w->channels[i];
    }
    if (num <= 4) {
        w->channels[num] = -1;
        w->class_ids[num] = no;
        w->used_masks[num] = 0;
        w->count = num + 1;
        return CH_CNT;
    }
    return -1;
}
