typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020048ac (60 bytes incl. one-word
 * pool, 14 calls).
 *
 * Derived span: no inventory row (item 28). `push {r5,r6,lr}` at
 * 0x020048ac, epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020048de-0x020048e2. The function reads no pool address until the
 * last call, and the trailing word at 0x020048e4 (0x00001111) is the mask
 * constant for that call, immediately followed by the next owner's push
 * at 0x020048e8 — so the pool is included in this span per the usual rule
 * (final return through the literal pool that follows it).
 *
 * `a`/`b`/`c` pass straight through to every helper call; the function
 * itself only ever reads the flags word (r3).
 */

void Func_0808a210();
void Func_0808a218();
void Func_08009128();
void Func_0808a010();

void Func_020048ac(s32 a, s32 b, s32 c, u32 flags)
{
    Func_0808a210(a, b, c, ~flags & 1);

    if (flags & 0x10000000) {
        Func_0808a218(a, b, c, flags & 0x10000000);
    }
    if (flags & 0x1000000) {
        Func_08009128(a, b, c, flags & 0x1000000);
    }

    Func_0808a010(flags & 0x1111);
}
