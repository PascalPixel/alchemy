/*
 * Resource 37a cutscene beat at 0x02001be8 (192 bytes, 0x02001be8-0x02001ca7).
 *
 * Complete owner: `push {lr}` prologue at 0x02001be8 and the matching
 * interworking return `pop {r0} / bx r0` at 0x02001c9a.  r0 holds the popped
 * return address, so nothing is returned: the owner is `void`.  Control flow is
 * completely straight-line — there is not a single branch in the body.
 *
 * Pool map, derived by walking the owner from its prologue: every instruction
 * from 0x02001be8 to 0x02001c9c is reached, 0x02001c9e is the alignment
 * halfword, and the eight bytes at 0x02001ca0-0x02001ca7 are the literal pool
 * (0x0000102b, 0x00000811).  Nothing branches over or into them.
 *
 * All nineteen call sites were resolved with tools/lib/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are not the callees.
 * sites=19, distinct_targets=13 (17 veneer, 2 prologue), against the
 * inventory's calls=18 — the inventory field counts distinct targets and is a
 * known low lower bound.
 *
 *   0x02001bec -> veneer 0x02002af4 -> Func_080f9010
 *   0x02001bf8 -> veneer 0x02002a0c -> Func_0808a0d0
 *   0x02001c00 -> veneer 0x02002a24 -> Func_0808a100
 *   0x02001c0e -> veneer 0x02002a1c -> Func_0808a0f0
 *   0x02001c1c -> veneer 0x020029f4 -> Func_0808a090
 *   0x02001c28 -> veneer 0x02002a0c -> Func_0808a0d0
 *   0x02001c34 -> veneer 0x02002a74 -> Func_0808a1b8
 *   0x02001c3c -> veneer 0x02002a24 -> Func_0808a100
 *   0x02001c42 -> veneer 0x02002a54 -> Func_0808a170
 *   0x02001c4c -> veneer 0x02002a34 -> Func_0808a128
 *   0x02001c54 -> prologue 0x020025fc (overlay-local)
 *   0x02001c5c -> veneer 0x02002a3c -> Func_0808a138
 *   0x02001c62 -> veneer 0x020029cc -> Func_0808a010
 *   0x02001c6a -> veneer 0x02002a2c -> Func_0808a110
 *   0x02001c72 -> prologue 0x020025fc (overlay-local)
 *   0x02001c7e -> veneer 0x02002a0c -> Func_0808a0d0
 *   0x02001c8a -> veneer 0x02002a1c -> Func_0808a0f0
 *   0x02001c90 -> veneer 0x020029cc -> Func_0808a010
 *   0x02001c96 -> veneer 0x020029bc -> Func_080770c8
 *
 * Func_080770c8 is the global story-flag setter of the flag trio resolved in
 * semantic/overlays/resource_3cb_c_020010e8.c (0x080770c0 test, 0x080770c8
 * set, 0x080770d0 clear); this beat closes by setting flag 0x0811, which is
 * the same flag band the sibling owner at 0x020025b0 tests.  Func_0808a170 is
 * the cue emitter, here given the pooled cue id 0x102b.  The remaining
 * 0x0808a0xx/0x0808a1xx imports are the established six-argument cutscene
 * actor/camera ABI shared across this overlay family.
 *
 * Func_020025fc is a two-argument overlay-local helper (`push {r5, lr}`,
 * `pop {r5} / pop {r0} / bx r0`) that saves r1, calls one import with
 * (r0, 0), then a second with the saved r1; it is called twice here with the
 * same pair (16, 6).
 *
 * Shift-built constants are spelled with their assembled values and the source
 * shift noted, because the assembly builds them from 8-bit immediates.
 *
 * Uncertainties: the imports' argument counts are taken from the registers this
 * owner writes before each site; several of these imports are reached with
 * different arities elsewhere in the overlay, hence the old-style declarations.
 * No import's return value is consumed here.
 */

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_020046e2();
void Func_02004606();
void Func_02004626();
void Func_0200462c();
void Func_02004612();
void Func_02004636();
void Func_020046aa();
void Func_02004662();
void Func_02004698();
void Func_02004682();
void Func_02004252();
void Func_0200469a();
void Func_02004630();
void Func_02004270();
void Func_0200468c();
void Func_020046a8();
void Func_0200465e();
void Func_02004654();

                     

                     

                     

                     

void Func_02001be8(void)
{
    Func_020046e2(21);
    Func_02004606(0, 376, 184);                 /* 188 << 1 */
    Func_02004626(0, 0);
    Func_0200462c(16, 0x1780000, 0xb80000);     /* 188 << 17, 184 << 16 */
    Func_02004612(16, 0x10000, 0x8000);         /* 128 << 9, 128 << 8 */
    Func_02004636(16, 392, 168);                /* 196 << 1 */
    Func_020046aa(16, 0x8000, 30);              /* 128 << 8 */
    Func_02004662(16, 1);
    Func_02004698(0x102b);
    Func_02004682(16, 4, 30);
    Func_02004252(16, 6);
    Func_0200469a(0, 2);
    Func_02004630(6);
    Func_02004698(16, 3);
    Func_02004270(16, 6);
    Func_0200468c(16, 376, 184);                /* 188 << 1 */
    Func_020046a8(16, 0x6480000, 0x6480000);    /* 201 << 19, copied to r2 */
    Func_0200465e(4);
    Func_02004654(0x811);
}
