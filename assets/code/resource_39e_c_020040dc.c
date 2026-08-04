typedef unsigned char u8;
typedef int s32;

/*
 * resource_39e owner at 0x020040dc, 76 bytes: flag-set + two-argument
 * pass-through closer. Sets story flag 0x140 (Func_080770c8, the
 * test/set pair named in resource_38d-family drafts), calls
 * Func_0808a428(141, 1), forwards its own two arguments unchanged to
 * Func_0808a430, clears byte 0x23 of the record behind the pointer
 * cell at 0x03001f30 (the same globalCtx cell resource_3c9_c_02002200.c
 * reads -- note the cell is READ BEFORE the Func_0808a430 call, its
 * byte written after), then Func_0808a448() / Func_0808a420(1) /
 * Func_080000c0(1). Called 3x within this overlay.
 *
 * Complete owner: `push {r5, r6, lr} / mov r6,r8 / push {r6}` at
 * 0x020040dc-0x20040e0 through `pop {r3} / mov r8,r3 / pop {r5, r6} /
 * pop {r0} / bx r0` at 0x2004118-0x2004120, one pool word
 * (0x03001f30) ending at 0x2004127; the next owner is this overlay's
 * own resource_39e_c_02004128.c, already drafted.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e 40dc 4128's
 * +2 rule).
 *
 * Uncertainty: the callees and both forwarded arguments are identified
 * by shape only.
 */

extern void Func_020084b0();
extern void Func_02008638();
extern void Func_0200864c();
extern void Func_02008666();
extern void Func_0200845a();

void Func_020040dc(s32 arg0, s32 arg1)
{
    u8 *globalCtx;

    Func_020084b0(160 << 1);
    Func_02008638(141, 1);
    globalCtx = *(u8 **)0x03001f30;
    Func_0200864c(arg0, arg1);
    globalCtx[0x23] = 0;
    Func_02008666();
    Func_0200864c(1);
    Func_0200845a(1);
}
