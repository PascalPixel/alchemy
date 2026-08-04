typedef int s32;

/*
 * resource_3c9 owner at 0x02004b28, 196 bytes: a short scripted-scene
 * sequence -- a dialogue line, three calls to a local overlay helper
 * (id 21/6/6), and a run of id-tagged position/wait/dialogue-flag
 * calls, no branches.
 *
 * Complete owner: `push {lr}` at 0x02004b28 through `pop {r0} / bx r0`
 * at 0x02004bd2-0x02004bd6, followed by the five-word literal pool
 * 0x02004bd8-0x02004beb; the next owner's prologue is exactly at
 * 0x02004bec (this overlay's own resource_3c9_c_02004bec.c candidate,
 * not yet drafted). No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts resource_3c9
 * 4b28 4bec`'s `+2` rule. `Func_02000894` is an ordinary already-indexed
 * overlay-internal function (not one of this overlay's unindexed
 * candidates), called three times with a small id.
 *
 * SHARED IDIOMS, all previously named in the resource_3b9 mandate and
 * cited rather than re-derived: `Func_0808a170` (dialogue by id),
 * `Func_0808a010` (wait n frames), `Func_0808a138`/`Func_0808a1f0`/
 * `Func_0808a188` (id-tagged position/flag calls), `Func_0808a208`
 * (two-raw-constant-pair shape) and `Func_0808a210` (big-shifted, -1
 * or negative, big-shifted, 1 -- here the second argument is
 * 0xffc00000 rather than -1, same call shape).
 *
 * Uncertainty: none of the eleven callees are identified beyond call
 * shape; every id/constant is recorded as a passed value.
 */

extern void Func_02000894(s32 id);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_080f9010(s32 arg0);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);

void Func_02004b28(void)
{
    Func_0808a170(0x2829);
    Func_02000894(21);
    Func_080f9010(62);
    Func_080091f0(128 << 9, 128 << 9, 128 << 9);
    Func_0808a208(0x0004cccc, 0x9999);
    Func_0808a208(128 << 11, 128 << 8);
    Func_0808a210(192 << 16, (s32)0xffc00000, 238 << 16, 1);
    Func_0808a218();

    Func_0808a010(40);
    Func_0808a138(21, 1);
    Func_0808a188(0x2015, 0, 40);

    Func_0808a138(6, 3);
    Func_02000894(6);
    Func_0808a1f0(21, 129 << 1);

    Func_0808a010(60);
    Func_0808a188(0x2015, 0, 80);
    Func_0808a1f0(6, 129 << 1);

    Func_0808a010(40);
    Func_0808a130(6, 2);
    Func_02000894(6);
}
