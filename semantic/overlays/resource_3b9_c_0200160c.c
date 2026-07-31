typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

/*
 * resource_3b9 owner at 0x0200160c, 124 bytes: a short scripted-scene
 * bracket -- open, one workspace write, two bookkeeping calls, a wait,
 * this overlay's own id-17 trampoline pair either side of a dialogue
 * line and a story-flag test, then close.
 *
 * Complete owner: `push {r5, lr}` at 0x0200160c through `pop {r5} / pop
 * {r0} / bx r0` at 0x02001670-0x02001674, followed by the alignment
 * halfword and the four-word literal pool 0x02001678-0x02001687; the
 * next owner's prologue is exactly at 0x02001688 (this overlay's own
 * resource_3b9_c_02001688.c). No incoming arguments are read before
 * being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule against the raw image (`bun tools/overlay_call_targets.ts
 * resource_3b9 160c 1688`).
 *
 * SHARED IDIOMS cited rather than re-derived, all previously documented
 * elsewhere in this overlay family:
 *   - Func_0808a018 / Func_0808a020 are the scripted-scene bracket
 *     open/close pair (comment first spelled out in
 *     resource_38d_c_020005f4.c and repeated throughout resource_38d/
 *     resource_389).
 *   - Func_0808a010(n) is "wait n frames" (resource_36f_c_020002e8.c).
 *   - Func_0808a170(id) is "show a dialogue line by id"
 *     (resource_38d_c_020005f4.c).
 *   - Func_080770c0(id) / Func_080770c8(id) are the story-flag
 *     test/set pair (resource_374_c_02000248.c,
 *     resource_371_c_020039fc.c).
 *   - Func_0808a370 is "scene bookkeeping, no arguments"
 *     (resource_371_c_020039fc.c), one of the trio with Func_0808a360
 *     and Func_0808a368 that this overlay's other owners call together.
 *   - The `movs r3,#236 / lsls r3,#1` / `ldrh+adds+strh` block against
 *     the 0x03001ebc workspace is the SKIP-BEAT COUNTER, the u16 at
 *     workspace + 472 (first named in resource_38b_c_02000584.c,
 *     bumped the same way in resource_374_c_02000248.c and
 *     resource_374_c_02000bbc.c). Guarded here by the story-flag test.
 *   - The `movs r2,#224 / lsls r2,#1 / ldr r3,[r5,#0] / adds r3,r3,r2 /
 *     adds r2,#65 / str r2,[r3,#0]` block is the ADDITIVE
 *     displacement/value idiom against the same workspace, first named
 *     in resource_377_c_02000a0c.c: displacement 448, value 513
 *     (224<<1 + 65 = 513). Same displacement, same value, same shape.
 *
 * Both id-17 calls are this overlay's own trampolines
 * (resource_3b9_c_02001c48.c, resource_3b9_c_02001c5c.c); the id
 * argument (17) is identical at all three call sites, unlike the
 * per-actor ids (12-20) this family usually passes.
 *
 * Uncertainty: the two dialogue/flag ids (0x206e, 0x8a4, 0x8a3) are
 * recorded as passed constants, not decoded further.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a360();
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a170(s32 dialogue_id);
extern s32 Func_080770c0(s32 flag_id);
extern void Func_080770c8(s32 flag_id);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_0200160c(void)
{
    u8 *workspace = Data_03001ebc;

    Func_0808a018();
    *(s32 *)(workspace + 448) = 513;      /* additive idiom: 224<<1 + 65 */

    Func_0808a360();
    Func_0808a370();

    Func_0808a010(20);
    Func_02001c5c(17, 160 << 7);
    Func_0808a170(0x206e);

    if (Func_080770c0(0x8a4) != 0) {
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
    }

    Func_02001c48(17);
    Func_02001c5c(17, 192 << 6);
    Func_080770c8(0x8a3);

    Func_0808a020();
}
