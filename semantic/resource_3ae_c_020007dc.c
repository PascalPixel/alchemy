typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3ae, owner at 0x020007dc (144 bytes including its five-word pool at
 * 0x02000858..0x0200086a).
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x020087dd == Func_020007dc + 1 as the payload of FOUR separate
 * entries, all keyed 0xffff000a under condition word 0x0000c602 and all in the
 * leading block of a table record group.  So it is the standard "leave this
 * area" transition handler for the overlay, invoked with no arguments from
 * four map records.
 *
 * Prologue `push {lr}` at 0x020007dc; epilogue `pop {r0} / bx r0` at
 * 0x02000854.  The return address is popped into r0, so r0 does not survive
 * and the owner is void.  No frame.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x020007de -> Func_0808a018   (scene open)
 *   0x020007e4 -> Func_080f9010   (id 158)
 *   0x020007f2 -> Func_0808a090
 *   0x020007fa -> Func_0808a1e0
 *   0x0200081a -> Func_0808a0c8
 *   0x02000824 -> Func_08009178
 *   0x02000836 -> Func_0808a0c8
 *   0x02000840 -> Func_08009178
 *   0x02000846 -> Func_0808a010   (beat delay, 16 frames)
 *   0x0200084c -> Func_0808a248
 *   0x02000850 -> Func_0808a020   (scene close)
 *
 * sites=11 equals the inventory's calls=11, so the call set is complete.
 *
 * Selector: `Data_02000240[224]`, the room id, read as a signed halfword and
 * compared against 0x6b and 0x70 — exactly the family of the byte-exact
 * sibling `assets/code/resource_3ae_c_02000040.c` and of the exported
 * accessors at 0x020000a0 / 0x0200019c.
 *
 * Link base: `Data_020096b8` and `Data_020096ce` are in-image data under the
 * proven 0x02008000 base (file offsets 0x16b8 and 0x16ce), the same band the
 * sibling at 0x02000040 spells symbolically.  Each is handed to Func_08009178
 * with a pair of small coordinates, which is the destination-descriptor shape.
 *
 * The two arms are behaviourally parallel but are DISTINCT sites with distinct
 * descriptors and coordinates; they are not merged.
 *
 * Uncertainties: 158 passed to Func_080f9010 sits in the same argument
 * position as the 113 that `Func_0200051c` passes, so it reads as the same id
 * space (a sound or effect cue).  The Func_0808a0c8 pairs are preserved as
 * computed; the units are not witnessed here.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_080f9010();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a1e0();
void Func_0808a248();
void Func_08009178();

extern s16 Data_02000240[];

extern u8 Data_020096b8[];
extern u8 Data_020096ce[];

void Func_020007dc(void)
{
    s32 room;

    Func_0808a018();
    Func_080f9010(158);

    /* movs r1,#128 / lsls r1,r1,#8 == 0x8000; movs r2,#128 / lsls #7 == 0x4000 */
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a1e0(0, 3);

    room = Data_02000240[224];

    if (room == 0x6b) {
        /* movs r1,#152 / lsls #1 == 304; movs r2,#174 / lsls #3 == 1392 */
        Func_0808a0c8(0, 304, 1392);
        Func_08009178(Data_020096b8, 78, 86);
    } else if (room == 0x70) {
        Func_0808a0c8(0, 248, 192);
        Func_08009178(Data_020096ce, 74, 9);
    }

    Func_0808a010(16);
    Func_0808a248(3);
    Func_0808a020();
}
