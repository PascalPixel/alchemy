typedef signed int s32;
typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 3bf paired-resource placement at 0x02000d54.
 *
 * The single argument selects an 8-byte entry from the table at 0x0200f73c
 * (`lsls r3,r0,#3`, then two word loads at +0 and +4).  r7 and r6 hold that
 * pair for the whole body and r8 preserves the original argument across the
 * calls, which is why the prologue pushes r8 through r7 and the epilogue
 * restores it.
 *
 * Each service takes four register arguments plus two stack arguments; the
 * stack pair is rewritten before every call and is reproduced faithfully here.
 * The fourth call happens only when the argument is exactly 1.
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention (see the note on the module's link base below);
 * none of them is reconstructed here.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: the
 * dispatcher at 0x02004638 loads its jump-table base as 0x0200c64c while the
 * table itself is embedded at offset 0x464c, and every entry in it likewise
 * points 0x8000 past the case body it selects.  Both the existing byte-exact
 * sources under assets/code and this file keep the printed-offset spelling for
 * symbol names and the raw pool value for absolute data addresses, which is
 * self-consistent; nothing here depends on which of the two bases is real.
 *
 * Uncertainty: the two table words are used only as opaque resource
 * identifiers here (they are biased by small constants, never dereferenced),
 * so they are typed s32 rather than as pointers.
 */
void Func_02006346(s32, s32, s32, s32, s32, s32);
void Func_02006358(s32, s32, s32, s32, s32, s32);
void Func_02006376(s32, s32, s32, s32, s32, s32);
void Func_02006390(s32, s32, s32, s32, s32, s32);

void Func_02000d54(s32 index)
{
    const u8 *table = (const u8 *)0x0200f73c;
    u32 offset = (u32)index << 3;
    s32 second = 0;
    s32 first;

    first = *(const s32 *)(table + offset);
    offset += 4;
    second = *(const s32 *)(table + offset);

    Func_02006346(0, 77, 1, 3, first, second);
    Func_02006358(1, 77, 1, 1, first + 1, second);
    Func_02006376(first, second - 45, 1, 1, first, second - 44);

    if (index == 1) {
        Func_02006390(first, second - 44, 1, 1, first, second - 43);
    }
}
