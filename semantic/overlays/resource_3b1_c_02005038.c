typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02005038 (48 bytes, 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` at 0x02005038, epilogue `pop {r5,r6,r7} / pop {r0} / bx
 * r0` at 0x02005060-0x02005064; no pool, the halfword at 0x02005066 is
 * the alignment pad, immediately followed by the next owner's push at
 * 0x02005068, already this overlay's row `0x02005068 | 1 call`, so the
 * span is 0x02005038-0x02005068, 48 bytes.
 *
 * A first-match scan, index 0..8 inclusive: on the first `a1` for which
 * `Func_0200b4e4` returns non-zero, call `Func_0200b59a(a0, 0, 0)` and
 * stop; `a0`/`a1` both increment together each iteration.
 */

s32 Func_0200b4e4();
void Func_0200b59a();

void Func_02005038(s32 a0, s32 a1)
{
    s32 i;

    for (i = 0; i <= 8; i++, a0++, a1++) {
        if (Func_0200b4e4(a1) != 0) {
            Func_0200b59a(a0, 0, 0);
            break;
        }
    }
}
