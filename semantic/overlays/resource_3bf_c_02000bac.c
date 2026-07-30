typedef signed int s32;

/*
 * Resource 3bf scene step at 0x02000bac.
 *
 * Complete owner: `push {lr}` at 0x02000bac through the interworking return
 * `pop {r0} / bx r0` at 0x02000be4.  The word at 0x02000be8 is this owner's
 * one-entry literal pool (0x00000943), reached by `ldr r0, [pc, #8]`.
 *
 * Return type: the epilogue pops the return address into r0, so nothing is
 * returned and the owner is `void` (HANDOVER section 0, interworking-epilogue
 * rule).
 *
 * CALL TARGETS.  An overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so `tools/overlay_show.ts`'s `bl` annotations
 * are wrong for every overlay.  The names below come from
 * `bun tools/overlay_call_targets.ts resource_3bf bac`, which resolves each
 * site and, for an import, reads the real main-image address out of the
 * overlay veneer's trailing word.  Six of the seven sites land in this
 * overlay's import veneer table (0x02005588..0x020057b0, eight bytes per entry,
 * `ldr r4,[pc,#0] / bx r4 / .word <main-image address + Thumb bit>`); the
 * seventh is an ordinary in-image call to the sibling owner at 0x02000b4c,
 * which already has a byte-exact source under assets/code.
 *
 * The disassembler prints that in-image call as `bl 0x02001728`, and the same
 * callee is printed as `bl 0x020017d6` from 0x02000c88 in the neighbouring
 * owner at 0x02000c78 — two spellings, one callee.  That is exactly the
 * decoding defect the rule above corrects.
 *
 * Argument evidence, per site, from the immediates set in the preceding block:
 *   0x0808a090(12, 0x00010000, 0x00008000)   0x80<<9 and 0x80<<8, i.e. the
 *                                            usual 16.16 pair 1.0 and 0.5
 *   0x0808a0b8(12, 248, 376)                 376 is 0xbc<<1
 *   0x0808a0e8(12)
 *   0x080f9010(215)
 *   0x0808a010(60)
 *   0x02000b4c()                             takes no arguments (its
 *                                            byte-exact source is `void(void)`)
 *   0x080770c8(0x943)                        the pooled word
 *
 * Slot 12 is the same subject the neighbouring owners 0x02000c78 and
 * 0x02000b4c operate on, so this reads as one step of the same scripted scene.
 * No further meaning is asserted for the imports; their interfaces are left
 * open with old-style declarations because this overlay reaches several of
 * them with differing argument counts from other owners.
 */

/* Imports, named by the main-image address in their veneer's trailing word. */
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_080f9010();
void Func_0808a010();
void Func_080770c8();

/* In-image sibling; byte-exact source: assets/code/resource_3bf_c_02000b4c.c */
void Func_02000b4c(void);

void Func_02000bac(void)
{
    Func_0808a090(12, 0x00010000, 0x00008000);
    Func_0808a0b8(12, 248, 376);
    Func_0808a0e8(12);
    Func_080f9010(215);
    Func_0808a010(60);
    Func_02000b4c();
    Func_080770c8((s32)0x943);
}
