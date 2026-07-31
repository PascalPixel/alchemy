typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020037b4 (36 bytes, 5 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` at 0x020037b4, epilogue `pop {r5,r6,r7} / pop {r0} / bx
 * r0` at 0x020037d0-0x020037d4; no pool needed, the halfword at 0x20037d6
 * is the alignment pad, immediately followed by the next owner's push at
 * 0x020037d8 (already this overlay's row `0x020037d8 | 4 calls`), so the
 * span is 0x020037b4-0x020037d8, 36 bytes.
 *
 * A flat sweep, index 28..35 inclusive (`cmp r5,#35; bls`), setting bit 3
 * of each object's `+0x59` flags byte.
 */

u8 *Func_02009ca8();

void Func_020037b4(void)
{
    s32 index;

    for (index = 28; index <= 35; index++) {
        u8 *obj = Func_02009ca8(index);
        obj[0x59] |= 8;
    }
}
