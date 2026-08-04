#include "types.h"

/*
 * resource_3b9 owner at 0x02001298, 884 bytes: the largest owner
 * drafted in this overlay so far -- one long scripted-scene bracket, no
 * branches, 92 call sites over 23 distinct veneer targets (per `bun
 * tools/overlay_call_targets.ts resource_3b9 1298 160c`). Same family
 * and shape as resource_3b9_c_02002668.c ("largest drafted this
 * overlay" at 440 bytes), just twice the length; drafted rather than
 * parked because it is mechanically simple, not structurally hard --
 * unlike the 66-case dispatcher and the >700-byte Data_03001ebc owner,
 * which stay parked for a dedicated pass.
 *
 * Complete owner: `push {r5, r6, lr} / mov r6,sl / mov r5,r9 / push
 * {r5, r6} / mov r6,r8 / push {r6}` at 0x02001298-0x020012a2 through
 * `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6} /
 * pop {r0} / bx r0` at 0x020015de-0x020015ea (HANDOVER's void epilogue
 * shape, restoring the three extra callee-saved registers), followed by
 * eight words of literal pool 0x020015ec-0x0200160b; the next owner's
 * prologue is exactly at 0x0200160c (this overlay's own
 * resource_3b9_c_0200160c.c, already drafted). No incoming arguments
 * are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with the `+2` rule.
 *
 * SHARED IDIOMS cited rather than re-derived: scripted-scene bracket
 * open/close, wait-n-frames, and the story-flag/dialogue calls (headers
 * of resource_3b9_c_0200160c.c and resource_3b9_c_02002668.c).
 *
 * REGISTER CACHE, the reason for the extra push/pop: r8 caches the
 * 0x03001ebc CELL address (not the workspace pointer -- re-dereferenced
 * fresh both times it is used, since the intervening ~60 calls could in
 * principle move it); sl and r9 cache the two workspace displacements
 * (448, 456) computed once and reused by both write blocks. The first
 * write block is the ADDITIVE/SUBTRACTIVE idiom back to back: 448
 * additive (224<<1 + 64 = 512) then 456 subtractive (512 - 56 = 456,
 * chained from the same register) -- both idiom names and their first
 * examples are in resource_377_c_02000a0c.c's header. The second write
 * block repeats displacement 448 with the additive idiom's canonical
 * value, 513 (same 224<<1 + 65 as resource_3b9_c_0200160c.c), and
 * displacement 456 with a plain immediate (16). So across the whole
 * owner the sequence is workspace+448 = 512, then 513; workspace+456 =
 * 32, then 16 -- the same two-stage state-progression shape resource_377
 * documents, with different numbers.
 *
 * record19 and record20 (Func_0808a080(19)/(20)) both get field+24 set
 * to the SAME pool word, 0xffff0000 (s32 -65536) -- one load, two
 * stores, reproduced here as one shared constant used twice. The
 * Func_0808a080(21) record is used only as Func_080091e0's first
 * argument and never stored to a local.
 *
 * Uncertainty: none of the callees are identified beyond call shape;
 * every id/constant is recorded as a passed value; the three struct
 * offsets (+6, +24, +28) are read as raw byte offsets, no layout
 * established.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a100(s32 id, s32 arg1);
extern u8 *Func_0808a080();
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, void *data);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_02001c5c(s32 arg0, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a248(s32 arg0);

void Func_02001298(void)
{
    u8 *workspace = Data_03001ebc;
    u8 *record;
    s32 shared_dialogue_ptr;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080f9010(247);

    Func_0808a100(8, 2);
    Func_0808a100(9, 2);
    Func_0808a100(10, 2);
    Func_0808a100(11, 2);
    Func_0808a100(12, 2);
    Func_0808a100(13, 2);
    Func_0808a100(14, 0);
    Func_0808a100(15, 0);
    Func_0808a100(16, 0);
    Func_0808a100(17, 0);
    Func_0808a100(18, 0);

    Func_080091e0(Func_0808a080(21), 0);

    record = Func_0808a080(19);
    *(s32 *)(record + 24) = (s32)0xffff0000;
    record = Func_0808a080(20);
    *(s32 *)(record + 24) = (s32)0xffff0000;

    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_080000c0(1);

    /* Cached-displacement additive/subtractive idiom on the workspace,
     * first named in resource_377_c_02000a0c.c. */
    *(s32 *)(workspace + 448) = 512;   /* additive: 224<<1 + 64 */
    *(s32 *)(workspace + 456) = 32;    /* subtractive: 512 - 56 */

    Func_0808a360();
    Func_0808a370();

    Func_0808a010(40);
    Func_0808a138(16, 1);
    Func_0808a010(20);
    Func_0808a090(16, 0xcccc, 0x6666);
    Func_0808a0d0(16, 164, 226 << 2);
    Func_0808a010(20);
    Func_0808a100(16, 9);
    Func_0808a010(40);
    Func_0808a100(16, 10);
    Func_0808a010(60);
    Func_0808a100(16, 1);
    Func_0808a010(20);
    Func_0808a0d0(16, 164, 230 << 2);
    Func_0808a0d0(16, 185, 230 << 2);
    Func_0808a1b8(16, 192 << 8, 20);
    Func_0808a0d0(16, 185, 229 << 2);
    Func_0808a100(16, 11);
    Func_0808a010(40);
    Func_0808a138(16, 1);
    Func_0808a010(60);
    Func_0808a138(16, 3);
    Func_0808a010(40);

    shared_dialogue_ptr = 0x0200af88;
    Func_0808a098(16, (void *)shared_dialogue_ptr);

    Func_0808a010(80);
    Func_0808a1f0(16, 129 << 1);
    Func_0808a010(60);

    Func_0808a1b8(14, 208 << 8, 0);
    Func_0808a1b8(15, 160 << 7, 0);
    Func_0808a1b8(17, 0, 0);
    Func_0808a1b8(18, 128 << 8, 20);
    Func_0808a1f0(14, 129 << 1);
    Func_0808a1f0(15, 129 << 1);
    Func_0808a1f0(17, 129 << 1);
    Func_0808a1f0(18, 129 << 1);

    Func_0808a010(60);
    Func_0808a1b8(14, 192 << 6, 0);
    Func_0808a1b8(15, 192 << 6, 0);
    Func_0808a1b8(17, 192 << 6, 0);
    Func_02001c5c(18, 192 << 6);

    Func_0808a0a8(16);

    record = Func_0808a080(16);
    *(u16 *)(record + 6) = 208 << 8;
    *(s32 *)(record + 24) = 128 << 9;
    *(s32 *)(record + 28) = 128 << 9;

    Func_0808a010(20);
    Func_0808a100(16, 0);
    Func_0808a010(40);
    Func_0808a100(19, 5);
    Func_0808a100(20, 5);
    Func_0808a010(60);
    Func_0808a1b8(16, 192 << 6, 20);
    Func_0808a100(16, 8);
    Func_0808a010(20);
    Func_0808a100(14, 4);
    Func_0808a100(15, 4);
    Func_0808a100(17, 4);
    Func_0808a110(18, 4);
    Func_0808a010(40);
    Func_0808a110(16, 4);
    Func_0808a010(10);
    Func_0808a090(16, 128 << 10, 128 << 9);
    Func_0808a0d0(16, 162, 229 << 2);
    Func_0808a0d0(16, 162, 0x37a);
    Func_0808a100(19, 1);
    Func_0808a100(20, 1);
    Func_0808a0d0(16, 184, 0x35f);
    Func_0808a0d0(16, 184, 199 << 2);
    Func_0808a0f0(16, 0, 0);

    /* Second write to the same two cached displacements: the additive
     * idiom's canonical value 513 (224<<1 + 65, same as
     * resource_3b9_c_0200160c.c) at +448, plain 16 at +456. */
    *(s32 *)(workspace + 448) = 513;
    *(s32 *)(workspace + 456) = 16;

    Func_0808a368();
    Func_0808a370();

    *(s32 *)(workspace + 448) = 128 << 1;

    Func_0808a248(69);
    Func_0808a020();
}
