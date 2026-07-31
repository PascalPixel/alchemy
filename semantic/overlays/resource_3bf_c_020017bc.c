typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3bf conditional event trigger at 0x020017bc.
 *
 * Three guards must all hold before the body runs, and the assembly evaluates
 * them in short-circuit order:
 *   1. the flag query for 0x225 must report 0;
 *   2. the state query for 21 must report non-zero;
 *   3. the SIGNED halfword Data_02000240[294] (byte offset 588) must be 0.
 *
 * The body marks 0x225, issues the two message addresses 0x020097bd and
 * 0x0200975d, and stores 96 into the halfword at workspace offset 386.  The
 * workspace pointer comes from the IWRAM slot at 0x03001ebc and is held in r5
 * across all four calls.  Both the offset 386 store and the Data_02000240
 * indexing match the already-adopted sibling
 * assets/code/resource_3bf_c_02001718.c.
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
 */
s32 Func_020018e4();
void Func_080000d8();
s32 Func_080770c0();
void Func_080770c8();

void Func_020017bc(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    const s16 *table = (const s16 *)0x02000240;

    if (Func_080770c0(0x225) != 0) {
        return;
    }
    if (Func_020018e4(21) == 0) {
        return;
    }
    if (table[294] != 0) {
        return;
    }

    Func_080770c8(0x225);
    Func_080000d8(0x020097bd);
    Func_080000d8(0x0200975d);
    *(s16 *)(workspace + 386) = 96;
}
