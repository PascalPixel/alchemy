typedef signed int s32;

/*
 * Resource 3b6 table selector at 0x02000064.
 *
 * Complete owner: `push {lr}` at 0x02000064, interworking return
 * `pop {r1} / bx r1` at 0x02000086.  A four-word literal pool occupies
 * 0x0200008c-0x0200009b; it is data reached only by the `ldr rN,[pc,#imm]`
 * loads above it.
 *
 * Signature.  The epilogue pops the saved return address into r1, so r0
 * survives the return and IS the result (HANDOVER section 0, interworking
 * epilogue rule).  The owner returns a pointer-sized value.
 *
 * Link base.  This overlay is linked at 0x02008000, so a pool word in
 * 0x0200_8xxx..0200_bxxx is an in-image address at `offset = value - 0x8000`.
 * Witness: the byte-exact sibling assets/code/resource_3b6_c_02000044.c
 * returns 0x02009040 and 0x02008e00, and file offsets 0x1040 and 0x0e00 hold
 * two parallel tables of the same record shape (identical leading records
 * 006c ffff 0001 0000 0000 00c8 ...).  The three results below resolve the
 * same way, to offsets 0x19d0, 0x1670 and 0x1310.
 *
 * Import naming.  Both `bl` sites reach the same import.  The disassembler
 * prints 0x02000a34 and 0x02000a42, but an overlay `bl` stores
 * `target_offset - 2`, so both truly compute overlay offset 0x9c8 — an entry
 * of this overlay's eight-byte veneer table, whose trailing word is the
 * main-image address 0x080770c0.  The import is named for that real address
 * rather than for either printed label.  (The byte-exact sibling at
 * 0x02000044 predates this rule and spells the same import `Func_02000a14`;
 * that name is the disassembler's artefact, not a distinct callee.)
 */

/* Old-style declaration: the import's real interface is not established here,
 * and this overlay reaches several imports with varying argument counts. */
s32 Func_080770c0();

/* Sibling selector at 0x02000044 uses this same import as a boolean query on
 * a numeric id, so it is read here as "is variant <id> selected". */

s32 Func_02000064(void)
{
    /* Ids 0x950 and 0x962 are queried in order; the first match wins. */
    if (Func_080770c0(0x950) != 0) {
        return 0x020099d0; /* in-image table at file offset 0x19d0 */
    }

    if (Func_080770c0(0x962) != 0) {
        return 0x02009670; /* in-image table at file offset 0x1670 */
    }

    return 0x02009310; /* in-image table at file offset 0x1310 */
}
