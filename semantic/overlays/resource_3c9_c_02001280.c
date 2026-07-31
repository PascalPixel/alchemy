typedef int s32;

/*
 * resource_3c9 owner at 0x02001280, 72 bytes: a two-way branch on its
 * second argument, each arm calling the same three imports with
 * different constants -- the highest call count (6x) among this
 * overlay's unindexed candidates, per `bun tools/overlay_unindexed.ts
 * resource_3c9`.
 *
 * Complete owner: `push {r5, lr}` at 0x02001280 through `pop {r5} / pop
 * {r0} / bx r0` at 0x020012ba-0x020012be, followed by the two-word
 * literal pool 0x020012c0-0x020012c7; the next owner's prologue is
 * exactly at 0x020012c8, no gap. `arg1` is read before being
 * overwritten (the `cmp r1,#0` at entry), so it is a real argument;
 * `arg0` likewise survives into r5 unmodified and is used only as a
 * passed value, never dereferenced.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts resource_3c9
 * 1280 12c8`'s `+2` rule -- all four targets are main-image veneers.
 *
 * Uncertainty: `arg0` is passed straight into `Func_0808a080`, which
 * elsewhere in this project's tree always takes a small id and returns
 * a record pointer (e.g. resource_3b9_c_02001298.c); here the value
 * comes from this owner's own first parameter with no visible
 * conversion, so either this owner's caller already passes an id in
 * that slot, or `Func_0808a080` accepts more than one argument shape.
 * Recorded as a plain passed value rather than resolved.
 */

typedef unsigned char u8;

extern void Func_0808a158(s32 arg0, s32 arg1);
extern u8 *Func_0808a080();
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);

void Func_02001280(s32 arg0, s32 flag)
{
    if (flag != 0) {
        Func_0808a158(arg0, 0);
        Func_080091e0(Func_0808a080(arg0), 1);
        Func_0808a090(arg0, 0xcccc, 0x6666);
    } else {
        Func_0808a158(arg0, 15);
        Func_080091e0(Func_0808a080(arg0), 0);
    }
}
