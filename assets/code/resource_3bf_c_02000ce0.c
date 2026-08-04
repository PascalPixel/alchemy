typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3bf scene step at 0x02000ce0.
 *
 * Same family as 0x02000dcc / 0x02000e80 / 0x02000f30, with one extra guard.
 * r7 keeps the SIGNED halfword at workspace offset 364 for the whole body,
 * while r5 keeps the biased form (r7 - 40) used as the sub-scene selector.
 * When the flag query for 0x941 reports non-zero AND that selector is exactly
 * 4, the body is skipped entirely.
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
void Func_020019a4();
void Func_0200631a();
void Func_02006324();
s32 Func_020062f2();
s32 Func_02006316();
void Func_02006356();
void Func_020064c2();

void Func_02000ce0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s16 id;
    s32 selector;

    if (Func_020062f2(234) == -1) {
        return;
    }

    id = *(s16 *)(workspace + 364);
    selector = id - 40;

    if (Func_02006316(0x941) != 0 && selector == 4) {
        return;
    }

    Func_020019a4(selector);
    Func_020064c2(157);
    Func_0200631a(0x30000, 0x30000, 0x10000);
    Func_02006324(-1, -1, 0xe666);
    Func_02006356(id + 808);
}
