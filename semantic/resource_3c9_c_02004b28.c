#include "types.h"

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
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
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














extern void Func_0200a922(s32 dialogue_id);
extern void Func_020053c8(s32 id);
extern void Func_0200aa06(s32 arg0);
extern void Func_0200a806(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200a986(s32 arg0, s32 arg1);
extern void Func_0200a992(s32 arg0, s32 arg1);
extern void Func_0200a9aa(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200a9b6(void);
extern void Func_0200a8a4(s32 frames);
extern void Func_0200a95c(s32 id, s32 arg1);
extern void Func_0200a996(s32 id, s32 arg1, s32 arg2);
extern void Func_0200a96e(s32 id, s32 arg1);
extern void Func_0200542c(s32 id);
extern void Func_0200a9ce(s32 id, s32 arg1);
extern void Func_0200a8d4(s32 frames);
extern void Func_0200a9be(s32 id, s32 arg1, s32 arg2);
extern void Func_0200a9e8(s32 id, s32 arg1);
extern void Func_0200a8ee(s32 frames);
extern void Func_0200a99e(s32 id, s32 arg1);
extern void Func_02005464(s32 id);
void Func_02004b28(void)
{
    Func_0200a922(0x2829);
    Func_020053c8(21);
    Func_0200aa06(62);
    Func_0200a806(128 << 9, 128 << 9, 128 << 9);
    Func_0200a986(0x0004cccc, 0x9999);
    Func_0200a992(128 << 11, 128 << 8);
    Func_0200a9aa(192 << 16, (s32)0xffc00000, 238 << 16, 1);
    Func_0200a9b6();

    Func_0200a8a4(40);
    Func_0200a95c(21, 1);
    Func_0200a996(0x2015, 0, 40);

    Func_0200a96e(6, 3);
    Func_0200542c(6);
    Func_0200a9ce(21, 129 << 1);

    Func_0200a8d4(60);
    Func_0200a9be(0x2015, 0, 80);
    Func_0200a9e8(6, 129 << 1);

    Func_0200a8ee(40);
    Func_0200a99e(6, 2);
    Func_02005464(6);
}
