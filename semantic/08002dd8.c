#include "types.h"

/*
 * Main-image owners at 0x08002dd8 and 0x08002df0, 40 bytes total: a pair
 * of leaf helpers sharing one small "transfer state" table at
 * 0x03001e50, plus the table-base literal pool itself. Census row
 * (core census) flagged
 * this `shared_literal_thumb_helper_module` because the automated walk
 * saw two adjacent leaf functions sharing one trailing pool rather than
 * a single push-rooted owner.
 *
 * The source disassembly at asm/08002dd8.s already carries hand-written Japanese
 * comments identifying each function's role, kept and translated here
 * rather than re-derived: Func_08002dd8 is annotated "transfer-state
 * evacuation, Thumb auxiliary process -- shares its state-area address
 * with the following process"; Func_08002df0 is "transfer-state
 * recording, Thumb auxiliary process". Both are heavily used --
 * `bun tools/main_xref.ts` reports 333 and 72 direct call sites
 * respectively, and each is separately published as a Thumb function
 * pointer (0x08000154, 0x0800017c).
 *
 * Complete owners: `Func_08002dd8:` at 0x08002dd8 through `bx lr` at
 * 0x08002dec (20 bytes), one alignment halfword, then `Func_08002df0:`
 * at 0x08002df0 through `bx lr` at 0x08002dfa (12 bytes), then the
 * table-base pool word (0x03001e50) at 0x08002dfc, ending flush at
 * 0x08002e00. Both leaf (no `push`/`pop`), void.
 *
 * `Data_03001e50` is read here as a small table of 4-byte slots: the
 * first lookup in Func_08002dd8 indexes it by `arg0`; both functions'
 * writes target slot 0 or slot 1 (byte offset 0 or 4), selected by bit
 * 24 of either the looked-up entry (dd8) or the raw argument (df0). The
 * table's full extent and the meaning of the encoded value beyond "bit
 * 24 selects one of two slots" are not established.
 */

extern u8 Data_03001e50[];

void Func_08002dd8(s32 arg0)
{
    u32 *entrySlot = (u32 *)(Data_03001e50 + arg0 * 4);
    u32 entry = *entrySlot;

    if ((entry >> 22) != 0) {
        s32 byteOffset = (s32)((entry >> 22) & 4);
        *entrySlot = 0;
        *(u32 *)(Data_03001e50 + byteOffset) = entry;
    }
}

void Func_08002df0(s32 arg0)
{
    s32 byteOffset = (s32)(((u32)arg0 >> 22) & 4);
    *(u32 *)(Data_03001e50 + byteOffset) = (u32)arg0;
}
