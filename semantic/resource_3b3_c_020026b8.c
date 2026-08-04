#include "types.h"

/* resource_3b3 callback owner at 0x020026b8, 148 bytes: scan actors 8-11 for
 * one occupying a narrow box relative to actor 0, forward its two-bit handle
 * mode, and mark actor 0 on y/z rejection paths. Prologue through the sole
 * return at 0x02002736-0x0200273a, with four pool words through 0x0200274b.
 * The label order preserves the binary's four-call lexical sequence. */
u8 *Func_0808a080();
void Func_0808a1e0();

static s32 IntegerTowardZero_020026b8(s32 value)
{
    if (value < 0)
        value += 0xffff;
    return value >> 16;
}

void Func_020026b8(void)
{
    u8 *hero = Func_0808a080(0);
    s32 selector = 8;
    u8 *actor;

loop:
    actor = Func_0808a080(selector);

    if (IntegerTowardZero_020026b8(*(s32 *)(hero + 12))
        != IntegerTowardZero_020026b8(*(s32 *)(actor + 12)))
        goto mark_and_continue;

    if (*(s32 *)(hero + 16) > *(s32 *)(actor + 16) - 0x80000
        || *(s32 *)(hero + 16) <= *(s32 *)(actor + 16) - 0x180000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 8) - 0x100000 > *(s32 *)(actor + 8)
        || *(s32 *)(actor + 8) >= *(s32 *)(hero + 8) + 0x100000)
        goto continue_loop;

    {
        u8 *handle = *(u8 **)(actor + 80);
        Func_0808a1e0(0, (handle[9] >> 2) & 3);
    }
    goto done;

mark_and_continue:
    hero = Func_0808a080(0);
    hero[35] |= 1;

continue_loop:
    selector++;
    if (selector <= 11)
        goto loop;

done:
    return;
}
