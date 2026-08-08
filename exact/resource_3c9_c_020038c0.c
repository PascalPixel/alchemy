/*
 * resource_3c9 owner at 0x020038c0, 28 bytes: the shortest owner in
 * this overlay -- a scripted-scene bracket around three other
 * unindexed candidates from this same sweep.
 *
 * Complete owner: `push {lr}` at 0x020038c0 through `pop {r0} / bx r0`
 * at 0x020038d6-0x020038d8, no literal pool; the next owner's prologue
 * is exactly at 0x020038dc (this overlay's own
 * resource_3c9_c_020038dc.c, already drafted). No incoming arguments
 * are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 38c0 38dc`'s `+2` rule. Three of the five resolved targets are this
 * same overlay's other unindexed candidates: Func_02007490,
 * Func_02007768 and Func_02007b0c (not yet drafted; declared old-style
 * here since their signatures aren't established yet).
 */

extern void Func_020095f8(void);
extern void Func_02009610(void);
extern void Func_02007490();
extern void Func_02007768();
extern void Func_02007b0c();

void Func_020038c0(void)
{
    Func_020095f8();
    Func_02007490();
    Func_02007768();
    Func_02007b0c();
    Func_02009610();
}
