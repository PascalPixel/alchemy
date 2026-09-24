/* NONMATCHING: 188 bytes, candidate 180 (2026-09-24). Readable retry loop.
 * Remaining: the reference hoists the 65 item id into sl and recomputes
 * state + 216 in each loop preheader; here state + 216 is shared in r7. */
#include "TYPES.H"

s32 Engine_OwnerGetState();
s32 Main_08077028();
s32 Engine_DebugGetItem();
void Main_080772b0();
void Main_08077050();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void GomaIriguchi_Func02000fc8(void)
{
    u8 *state;
    u8 *item;
    u16 *items;
    s32 tries;
    s32 i;

    state = (u8 *)Engine_OwnerGetState(2);
    tries = 0;
retry:
    if (++tries > 1000)
        *(u16 *)(state + 244) = 0;
    if (Main_08077028(2, 65) == -1) {
        for (i = 0, items = (u16 *)(state + 216); i <= 14; i++) {
            item = (u8 *)Value1(Engine_DebugGetItem, *items++);
            if (item[2] == 1)
                goto drop;
        }
        for (i = 0, items = (u16 *)(state + 216); i <= 14; i++, items++) {
            item = (u8 *)Value1(Engine_DebugGetItem, *items);
            if ((*(u16 *)(item + 2) & 0x8ff) == 0 && item[12] == 1) {
            drop:
                Main_080772b0(2, i);
                goto retry;
            }
        }
        goto retry;
    }
    for (i = 0, items = (u16 *)(state + 216); i <= 14; i++) {
        if (*items++ == 65)
            Main_08077050(2, i);
    }
}
