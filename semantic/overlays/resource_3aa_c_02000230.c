typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3aa owner at 0x02000230, 52 bytes: code 0x02000230-0x02000251, a
 * two-byte `movs r0, r0` alignment filler at 0x02000252, and the four-word
 * literal pool at 0x02000254-0x02000263.  The next inventory row starts at
 * 0x02000264, so the owner is exactly its advertised span and nothing live
 * escapes it.
 *
 * Behaviour: publish request code 0x209 into the word at +448 of the block a
 * pointer in IWRAM points at, then, only for the edition whose selector
 * halfword `Data_02000240[224]` reads 0x67, run the overlay's own routine at
 * 0x02000264.  Returns 0 unconditionally.
 *
 * Return type, by the interworking-epilogue rule: `pop {r1} / bx r1` pops the
 * return address into r1, so r0 survives and is the result; it is set to 0
 * immediately before the pop on both paths.
 *
 * The selector test and the 0x02000240 base are the same ones the byte-exact
 * siblings `assets/code/resource_3aa_c_02000030.c` and `_020000a4.c` use, and
 * the 448-byte displacement is `224 * 2`, i.e. the same element of a signed
 * halfword table.  The store, however, is a full word `str` at that same
 * displacement through a *different* base (the pointer read from 0x03001ebc),
 * so the two accesses are kept distinct here rather than merged.
 *
 * Call accounting: one `bl`, at 0x02000248, resolved with
 * `bun tools/overlay_call_targets.ts resource_3aa 0230` to the in-overlay
 * prologue at file offset 0x0264 - an ordinary intra-overlay call, not an
 * import.  (The disassembler's annotation 0x020004ae is the usual overlay `bl`
 * mis-decoding.)  No argument register is set before it, so the caller's r0-r3
 * reach it unchanged; this owner asserts no arguments for it.
 */

/* In-overlay callee at file offset 0x0264. Old-style: this call site sets no
 * argument register, so its arity is not established here. */
void Func_02000264();

/* Signed halfword table in RAM; index 224 is the edition selector. */
extern s16 Data_02000240[];

/* IWRAM slot holding the pointer to the block the request word is written to. */
#define WORKSPACE_POINTER (*(u32 **)0x03001ebc)

s32 Func_02000230(void)
{
    WORKSPACE_POINTER[448 / 4] = 0x209;

    if (Data_02000240[224] == 0x67) {
        Func_02000264();
    }
    return 0;
}
