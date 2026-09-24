#include "TYPES.H"

s32 Main_080001a8();
s32 Main_080001d0();
s32 Engine_VramLoad();
s32 Engine_TaskAddCallback();

struct Half {
    u16 v;
};

/* NONMATCHING: 90 of 92 bytes, 4 halfword edits (2026-09-24). The zero
 * is a one-halfword struct (movhi pool reach). The reference loads the
 * slot address into r5 before the Main_080001d0 call; here it is loaded
 * after it. */
void Func_02001c1c(void)
{
    s32 slot;
    u16 *slotp;
    struct Half zero;

    Main_080001a8(0x2009fa0, 0x200a730);
    slotp = (u16 *)0x0200a6d0;
    slot = Main_080001d0();
    *slotp = slot;
    Engine_VramLoad((s16)slot, 0x480, 0);
    zero.v = 0;
    *(u16 *)0x0200a6be = zero.v;
    *(u16 *)0x0200b030 = zero.v;
    Engine_TaskAddCallback(0x20098a5, 0xc76);
}
