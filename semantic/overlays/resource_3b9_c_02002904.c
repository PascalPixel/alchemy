typedef int s32;

/*
 * resource_3b9 owner at 0x02002904, 96 bytes: a linear setup sequence --
 * one call with two large fixed-point-looking constants, then three
 * near-identical calls with a shared second constant (214<<1) and a
 * varying first, then a short closing run.
 *
 * Complete owner: `push {lr}` at 0x02002904 through `pop {r0} / bx r0`
 * at 0x02002958-0x0200295a, followed by the two-word literal pool
 * 0x0200295c-0x02002963; the next owner's prologue is at 0x02002964
 * (this overlay's own resource_3b9_c_02002964.c, since drafted). No incoming arguments are read before being overwritten, so
 * `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * Uncertainty: none of the nine callees are identified beyond call
 * shape; the two large pool constants (0xcccc, 0x19999) are recorded as
 * passed values.
 */

extern void Func_0808a018(void);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a360(void);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a0c0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a248(s32 arg0);

void Func_02002904(void)
{
    Func_0808a018();
    Func_0808a090(0, 0x19999, 0xcccc);
    Func_0808a360();
    Func_0808a100(0, 2);
    Func_0808a0c0(0, 800, 428);
    Func_0808a0c0(0, 700, 428);
    Func_0808a0b8(0, 600, 428);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(22);
}
