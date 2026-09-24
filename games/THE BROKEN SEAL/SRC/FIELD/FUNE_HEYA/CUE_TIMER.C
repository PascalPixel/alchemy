#include "TYPES.H"

/*
 * Fune room: a one-shot cue timer. While the timer word is nonzero it counts
 * down, and exactly at 70 remaining it fires the cue. Once it hits zero, when
 * the scene's timer condition is met the cue plays and the timer is rearmed
 * to 80.
 */

extern void Func_0200c0ee();
extern s32 Func_0200c09c();
extern void Func_0200c2de();
extern void Func_0200c116();

/* Same argument forwarding used by the byte-exact timer sibling. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    s32 value;
    s32 timer_address = 0x0200ff84;
    s32 remaining;
    if (*(s32 *)timer_address != 0) {
        remaining = *(s32 *)timer_address - 1;
        *(s32 *)timer_address = *(s32 *)timer_address - 1;
        if (remaining == 70)
            Call3(Func_0200c0ee, -1, -1, 0xe666);
    } else {
        value = Func_0200c09c();
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Func_0200c2de(181);
            Call3(Func_0200c116, 0x20000, 0x20000, 0x10000);
            *(s32 *)timer_address = 80;
        }
    }
}
