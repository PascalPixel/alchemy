#include "types.h"

extern s32 Func_08004278(void *);
extern s32 Func_080072f0(s32, s32, s32, s32);

void Func_08090658(void)
{
    u8 *state = *(u8 **)0x03001ECC;
    s8 *duration = (s8 *)(state + 0x53c);
    u16 value;

    if (*duration != 0) {
        s8 *step = (s8 *)(state + 0x53d);

        if (*step >= *duration) {
            volatile u16 *dma0;

            *duration = 0;
            Func_08004278((void *)Func_08090658);
            dma0 = (volatile u16 *)0x040000b0;
            dma0[5] = dma0[5] & 0xc5ff;
            dma0[5] = dma0[5] & 0x7fff;
            (void)dma0[5];
            return;
        } else {
            s8 *endPtr = (s8 *)(state + 0x53b);
            s32 delta = *endPtr - *(s8 *)(state + 0x53a);
            s32 v;

            (*step)++;
            v = Func_080072f0(delta * *step, *duration, delta, 0x03000380);
            *(u16 *)(state + 0x52a) = *(s8 *)(state + 0x53a) + v;
        }
    }

    {
        u16 *valuePtr = (u16 *)(state + 0x52a);
        u8 *toggle = state + 0x539;
        s32 v2;
        s32 blend;
        s32 masked;
        s32 idx;
        const u8 *table = (const u8 *)0x0809e8ee;
        s32 i;

        value = *valuePtr;
        v2 = value - 1;
        *toggle ^= 1;

        blend = 0;
        if ((v2 & 32) != 0) {
            blend = 15;
        }
        masked = v2 & 31;
        idx = masked << 1;

        i = 0;
        do {
            u8 entry = table[idx & 63];
            u8 *p = state + 0x508 + (entry >> 1);

            if (entry & 1) {
                *p = (*p & 0x0f) | (blend << 4);
            } else {
                *p = (*p & 0xf0) | blend;
            }
            i++;
            idx++;
        } while (i <= 1);
    }

    {
        volatile u16 *ime = (volatile u16 *)0x04000208;
        u16 *count = (u16 *)0x02002090;
        s32 savedIme;
        s32 counter;

        savedIme = *ime;
        *ime = 0;
        counter = *count;
        if (counter <= 31) {
            s32 offset12 = counter * 12;
            u8 *entry = (u8 *)count + offset12;

            *count = counter + 1;
            {
                u32 *slot = (u32 *)(entry + 4);
                *slot++ = (u32)(state + 0x508);
                *slot++ = 0x06000000;
                *slot = 0x84000008;
            }
        }
        *ime = savedIme;
    }
}
