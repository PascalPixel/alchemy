#include "types.h"

extern u8 *Func_0808a080();
extern void Func_080770c8();
extern void Func_0808a100();
extern void Func_0808a148();
extern void Func_0808a010();
extern void Func_0808a1f0();
extern void Func_0808a130();
extern void Func_0808a108();
extern void Func_0808a090();
extern void Func_2002286();
extern void Func_0808a0b8();
extern void Func_0808a1e8();
extern void Func_0808a250();

/* Find the nearest companion actor, perform the interaction, then restore it. */
void Func_02001180(void)
{
    s32 slot = *(s32 *)(0x02000240 + 500);
    u8 *leader = Func_0808a080(slot);
    s32 nearest_slot = 9;
    s32 nearest_distance = 0x100000;
    s32 candidate;

    Func_080770c8(0x200);
    for (candidate = 9; candidate <= 12; candidate++) {
        u8 *other = Func_0808a080(candidate);
        if (other != 0) {
            s32 dx = ((*(s32 *)(leader + 8) - *(s32 *)(other + 8)) & 0xffff);
            s32 dz = ((*(s32 *)(leader + 16) - *(s32 *)(other + 16)) & 0xffff);
            s32 distance = (dx < 0 ? -dx : dx) + (dz < 0 ? -dz : dz);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_slot = candidate;
            }
        }
    }

    Func_0808a100(0, 1);
    leader = Func_0808a080(0);
    leader[90] &= (u8)~1;
    Func_0808a148(0, nearest_slot, 0);
    Func_0808a010(20);
    Func_0808a1f0(0, 0x102);
    Func_0808a130(0, 2);
    Func_0808a010(60);
    Func_0808a1f0(0, 0x101);

    leader = Func_0808a080(0);
    *(u16 *)(leader + 6) = (u16)((*(u16 *)(Func_0808a080(0) + 6) + 0x2000) & 0xffff);
    Func_0808a100(0, 5);
    Func_0808a108(0, 24);
    Func_0808a090(0, 0x1999, 0x0ccc);
    leader = Func_0808a080(0);
    *(void **)(leader + 108) = (void *)0x02008345;

    {
        u8 *other = Func_0808a080(nearest_slot);
        /* 0x02001284 is inline pool data skipped by the branch above. */
        if (0) Func_2002286();
        if (other != 0)
            Func_0808a0b8(0, *(s16 *)(other + 10), *(s16 *)(other + 18));
    }

    Func_0808a010(60);
    Func_0808a1e8(nearest_slot, 0x104, 0);
    Func_0808a010(60);
    Func_0808a1f0(0, 0x100);
    leader = Func_0808a080(0);
    leader[90] |= 1;
    leader = Func_0808a080(0);
    *(void **)(leader + 108) = 0;
    Func_0808a250(53, 4);
}
