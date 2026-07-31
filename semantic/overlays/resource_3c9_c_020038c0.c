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
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts resource_3c9
 * 38c0 38dc`'s `+2` rule. Three of the five resolved targets are this
 * same overlay's other unindexed candidates: Func_02003bc8,
 * Func_02003e9c and Func_0200423c (not yet drafted; declared old-style
 * here since their signatures aren't established yet).
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_02003bc8();
extern void Func_02003e9c();
extern void Func_0200423c();

void Func_020038c0(void)
{
    Func_0808a018();
    Func_02003bc8();
    Func_02003e9c();
    Func_0200423c();
    Func_0808a020();
}
