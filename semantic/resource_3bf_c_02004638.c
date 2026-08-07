#include "types.h"

/*
 * Resource 3bf cutscene-stage dispatcher at 0x02004638.
 *
 * The stage word at 0x0200dfa4 selects one of eight bodies through a
 * `mov pc,r3` jump table.  The compare is `cmp r3,#7` / `bhi`, an UNSIGNED
 * bound check, so the word is read as u32 here and any value above 7 returns
 * without doing anything.
 *
 * Seven of the eight cases share a tail: they load one identifier and run the
 * same pair of calls.  The identifiers, in table order, are 0x2414, 0x2415,
 * 0x2416, 0x2417, 0x2418, (case 5 is the long body), 0x241a and 0x241b — the
 * run 0x2414..0x241b with 0x2419 used inside case 5.  Case 5 is the only one
 * with its own sequence.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: this
 * dispatcher loads its jump-table base as 0x0200c64c while the table is
 * embedded at offset 0x464c, and each entry likewise points 0x8000 past the
 * case body it selects.  That is the clearest evidence of the shift in the
 * whole overlay.  Symbol names below keep the tree's printed-offset spelling,
 * matching the byte-exact sources under assets/code.
 *
 * The callees are other entry points of this same overlay module; none of them
 * is reconstructed here.
 */
void Func_02009d70();
void Func_02009d88();
void Func_02009da6();
void Func_02009ccc();
void Func_02009db8();
void Func_02009cde();
void Func_02009dca();
void Func_02009cf0();
void Func_02009db6();
void Func_02009dce();

                     

void Func_02004638(void)
{
    u32 stage = *(volatile u32 *)0x0200dfa4;
    s32 id;

    switch (stage) {
    case 0: id = 0x2414; break;
    case 1: id = 0x2415; break;
    case 2: id = 0x2416; break;
    case 3: id = 0x2417; break;
    case 4: id = 0x2418; break;
    case 6: id = 0x241a; break;
    case 7: id = 0x241b; break;

        Func_02009d70(21, 0xd000, 0);
        Func_02009d88(50);
        Func_02009da6(21, 0xb000, 0);
        Func_02009ccc(50);
    case 5:
        Func_02009db8(21, 0x5000, 0);
        Func_02009cde(50);
        Func_02009dca(0x2419);
        Func_02009cf0(21, 0);
        return;

    default:
        return;
    }

    Func_02009db6(id);
    Func_02009dce(21, 0);
}
