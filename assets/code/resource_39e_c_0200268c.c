typedef int s32;

/*
 * resource_39e owner at 0x0200268c, 76 bytes: a fully linear scripted
 * scene beat inside the Func_0808a018/Func_0808a020 bracket (idiom first
 * named in resource_38d_c_020005f4.c). Body: Func_0808a090(0, 160<<10,
 * 160<<9), forwards its own two arguments as Func_0808a0e0(0, arg0,
 * arg1), then Func_0808a128(0, 4, 0), Func_0808a100(0, 7),
 * Func_0808a0e8(0), Func_0808a100(0, 6), close bracket. Called 3x
 * within this overlay.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200268c through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x020026d2-0x020026d6, no
 * literal pool; the next owner's prologue (`push {r5, lr}`) begins
 * immediately at 0x020026d8.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e 268c 26d8's
 * +2 rule -- all eight sites resolve through the overlay veneer table
 * to the main-ROM Func_0808a0xx vocabulary).
 *
 * Uncertainty: callees and the forwarded arguments are identified by
 * shape only; the two shifted constants (160<<10, 160<<9) are
 * transcribed from the arithmetic, meaning unknown.
 */

extern void Func_02006a70(void);
extern void Func_02006ab4(void);
extern void Func_02006ab6(s32 id, s32 arg1, s32 arg2);
extern void Func_02006ae8(s32 id, s32 arg1, s32 arg2);
extern void Func_02006b08(s32 id);
extern void Func_02006b12(s32 id, s32 arg1);
extern void Func_02006b1a(s32 id, s32 arg1, s32 arg2);
extern void Func_02006b20(s32 id, s32 arg1);

void Func_0200268c(s32 arg0, s32 arg1)
{
    Func_02006a70();
    Func_02006ab6(0, 160 << 10, 160 << 9);
    Func_02006ae8(0, arg0, arg1);
    Func_02006b1a(0, 4, 0);
    Func_02006b12(0, 7);
    Func_02006b08(0);
    Func_02006b20(0, 6);
    Func_02006ab4();
}
