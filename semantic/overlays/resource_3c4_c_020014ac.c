/*
 * resource_3c4 @ 0x020014ac (84 bytes: 78 code + alignment + one pool word).
 *
 * Sets slot 14 into one of two states from a queried flag: either the byte at
 * +98 goes to 0 and bit 3 of the byte at +89 is cleared, or +98 goes to 1 and
 * bit 3 of +89 is set.  Both branches reach the same `strb` at 0x020014f6.
 *
 * Func_0200458e is called twice with different selectors (0x201 from the pool
 * word, then 0x200 built with `movs`/`lsls`); only the second result is used.
 *
 * Each state uses its own accessor pair, so the two fetches are kept separate
 * rather than folded into one descriptor pointer.
 *
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

s32 Func_0200458e(s32 selector);
u8 *Func_020045d0(s32 slot);
u8 *Func_020045dc(s32 slot);
u8 *Func_020045ec(s32 slot);
u8 *Func_020045f8(s32 slot);

void Func_020014ac(void)
{
    u8 *state;
    u8 *flags;

    Func_0200458e(0x201);
    if (Func_0200458e(0x200) != 0) {
        state = Func_020045d0(14);
        state[98] = 0;
        flags = Func_020045dc(14);
        flags[89] = (u8)(flags[89] & 0xf7);
    } else {
        state = Func_020045ec(14);
        state[98] = 1;
        flags = Func_020045f8(14);
        flags[89] = (u8)(flags[89] | 8);
    }
}
