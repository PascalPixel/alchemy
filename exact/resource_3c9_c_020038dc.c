#include "types.h"

/*
 * resource_3c9 owner at 0x020038dc, 72 bytes: a scripted-scene bracket
 * around two other candidates from this sweep, a story-flag set, a
 * workspace write, and a closing call.
 *
 * Complete owner: `push {lr}` at 0x020038dc through `pop {r0} / bx r0`
 * at 0x0200391a-0x0200391c, followed by the alignment halfword and
 * one-word literal pool 0x0200391e-0x02003923; the next owner's
 * prologue is exactly at 0x02003924 (this overlay's own
 * resource_3c9_c_02003924.c candidate, not yet drafted). No incoming
 * arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 38dc 3924`'s `+2` rule. Calls this overlay's own Func_020048d8 and
 * Func_02004b28 (the latter already drafted,
 * resource_3c9_c_02004b28.c, void/no-argument).
 *
 * SHARED IDIOMS cited from the resource_3b9 mandate: scripted-scene
 * bracket open/close and the story-flag set (Func_080770c8), plus the
 * additive/subtractive workspace-write idiom on 0x03001ebc (first
 * named in resource_377_c_02000a0c.c) -- here workspace+448 = 512
 * (additive: 224<<1 + 64) and workspace+456 = 24 (subtractive:
 * 512 - 56, plain immediate).
 */

extern u8 *Data_03001ebc;

/*
 * Per-site call symbols: byte-matching this overlay's `bl` needs the RAW
 * pc-relative decode of the reference halfwords, not the semantic import
 * address -- confirmed on resource_3c9:3660, :1280 and :37c4 (see those
 * files). Eight sites in call order, from `overlay_show`'s plain listing:
 * 0x2009614, 0x20081bc, 0x2008410, 0x2009604, 0x20097a6, 0x20097b2,
 * 0x2009770, 0x2009654.
 */
void Func_02009614();  /* Func_0808a018 */
void Func_020081bc();  /* Func_020048d8, this overlay's own */
void Func_02008410();  /* Func_02004b28, this overlay's own */
void Func_02009604();  /* Func_080770c8 */
void Func_020097a6();  /* Func_0808a368 */
void Func_020097b2();  /* Func_0808a370 */
void Func_02009770();  /* Func_0808a248 */
void Func_02009654();  /* Func_0808a020 */

void Func_020038dc(void)
{
    u8 *workspace;

    Func_02009614();
    Func_020081bc();
    Func_02008410();
    Func_02009604(141 << 1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 512;   /* additive: 224<<1 + 64 */
    *(s32 *)(workspace + 456) = 24;    /* subtractive: 512 - 56 */

    Func_020097a6();
    Func_020097b2();
    Func_02009770(1);
    Func_02009654();
}
