#include "types.h"

/*
 * resource_39f owner at 0x02001c34, 208 bytes: code 0x02001c34-0x02001cff and
 * the pool word 0xfffe0000 at 0x02001d00.
 *
 * The slot-14 member of the collision-footprint family (0x02001520 for slot 11,
 * 0x02001b84 for slot 13).  It marks the actor's tile 0xff, clears the four
 * orthogonal neighbours, and when the actor's Z reaches tile 27 it parks the
 * record - mode byte cleared, -2.0 written to +12 and +20 - notifies
 * Func_080770c8 and marks one extra tile at (43, 23).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule,
 * and resource_39f_c_02001520.c for the byte-exact Func_02000244 interface.
 *
 * The epilogue is `add sp, #8 / pop {r3, r5} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 */

void Func_0808a018();
s32 *Func_0808a080();
/* Byte-exact: (layer, x, z, width, height, value). */
s32 Func_02000244();
void Func_080770c8();
void Func_0808a020();

void Func_02001c34(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_0808a018();

    x = Func_0808a080(14)[2] >> 20;
    z = Func_0808a080(14)[4] >> 20;

    Func_02000244(2, x, z, 1, 1, 0xff);
    Func_02000244(2, x + 1, z, 1, 1, 0);
    Func_02000244(2, x - 1, z, 1, 1, 0);
    Func_02000244(2, x, z + 1, 1, 1, 0);
    Func_02000244(2, x, z - 1, 1, 1, 0);

    /* The Z tile is re-read from the record rather than reused: the register
     * holding it was decremented in place for the call above. */
    if ((Func_0808a080(14)[4] >> 20) == 27) {
        u8 *record = (u8 *)Func_0808a080(14);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;

        Func_080770c8(532);            /* 133 << 2 */
        Func_02000244(2, 43, 23, 1, 1, 0xff);
    }

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
