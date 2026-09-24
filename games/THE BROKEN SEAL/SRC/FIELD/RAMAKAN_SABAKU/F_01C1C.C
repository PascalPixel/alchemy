#include "TYPES.H"

s32 Main_080001a8();
s32 Main_080001d0();
s32 Engine_VramLoad();
s32 Engine_TaskAddCallback();

/* A one-halfword struct keeps the zero a register copy, not a halfword pool
 * constant out of reach. */
struct Half {
    u16 v;
};

/* Claim a VRAM slot for the sand effect, load its tiles, clear two
 * counters and start the effect task. */
void RamakanSabaku_Func02001c1c(void)
{
    s32 slot;
    u16 *slotp;
    struct Half zero;

    Main_080001a8(0x2009fa0, 0x200a730);
    slotp = (u16 *)0x0200a6d0;
    /* FAKEMATCH: the do-while wrap keeps the slot address in r5 across the
     * call, and the size temporary schedules the slot store after it. */
    do {
        slot = Main_080001d0();
    } while (0);
    {
        s32 size = 0x480;

        *slotp = slot;
        Engine_VramLoad((s16)slot, size, 0);
    }
    zero.v = 0;
    *(u16 *)0x0200a6be = zero.v;
    *(u16 *)0x0200b030 = zero.v;
    Engine_TaskAddCallback(0x20098a5, 0xc76);
}
