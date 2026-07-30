typedef signed int s32;

/*
 * Resource 375, owner at 0x02000ba8 (56 bytes: 0x02000ba8-0x02000bd7 code,
 * 0x02000bd8-0x02000bdf literal pool).
 *
 * Complete owner: `push {lr}` at 0x02000ba8, matching `pop {r0} / bx r0` at
 * 0x02000bd4.  The popped return address lands in r0, so nothing is returned:
 * the owner is void (HANDOVER §0).
 *
 * All five `bl` sites were resolved with tools/overlay_call_targets.ts; every
 * one reaches the import veneer table at 0x02001a54 and thence a main-image
 * import.  In call order: 0x080770c0, 0x080f9010, 0x08009178, 0x0808a0c8,
 * 0x0808a248.  The disassembler's printed targets are wrong for every overlay
 * `bl` and are ignored.
 *
 * 0x0200a0ac is an in-image address: this overlay is linked at 0x02008000, so
 * the word denotes file offset 0x20ac (a data block, not a RAM global).
 *
 * Uncertainty: the constants 0x0242, 0x9e, 0x2b/8, 0xe5/0xd9 and 3 are event,
 * resource and mode identifiers whose meanings are not established here, so
 * they are left numeric.
 */

/* Imports, old-style: their arities are not established, and the same import
 * can be reached with different argument counts elsewhere in this overlay.
 * Func_080770c0's return type must be spelled because it is tested. */
s32 Func_080770c0();
void Func_080f9010();
void Func_08009178();
void Func_0808a0c8();
void Func_0808a248();

void Func_02000ba8(void)
{
    if (Func_080770c0(0x242) == 0) {
        Func_080f9010(0x9e);
        Func_08009178((void *)0x0200a0ac, 0x2b, 8);
    }
    Func_0808a0c8(0, 0xe5, 0xd9);
    Func_0808a248(3);
}
