/*
 * resource_3c4 @ 0x020020a4 (604 bytes: 584 code + alignment + five pool
 * words).
 *
 * Pins five slots (19..23) to their own 12.20 grid cells, then checks whether
 * all five have been brought to the puzzle's solved cells and takes one of two
 * near-identical outcome sequences.
 *
 * The counter is built by five independent two-part tests: each slot's +8 and
 * +16 words, shifted right by 20 (signed `asrs`), must equal that slot's pair.
 * The first test assigns 1 and the other four increment, so `count == 5` means
 * every slot matched.
 *
 * Control flow detail worth keeping: when the puzzle is solved *and* the 0x984
 * flag is already set, the reference branches straight to 0x020022e4 and so
 * skips the closing Func_020053de call that every other path makes.
 *
 * Pool words 0x0200b3ec and 0x0200b40c are data addresses.  This overlay is
 * linked at 0x02008000 while the inventory prints a 0x02000000 base, so they
 * are file offsets 0x33ec and 0x340c — in-image data blocks, spelled here with
 * their runtime addresses to match the byte-exact neighbours.  0x00000984,
 * 0x0000cccc and 0x00001999 are values, not addresses.
 *
 * `add sp,#8 ; pop {r5, r6} ; pop {r0} ; bx r0` return: void.
 */
#include "types.h"

typedef struct Slot_020020a4 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020020a4;

extern u8 Data_0200b3ec[];
extern u8 Data_0200b40c[];

void Func_08009178();
void Func_080091c0();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
Slot_020020a4 *Func_0808a080();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_080f9010();






void Func_020020a4(void)
{
    s32 column;
    s32 row;
    s32 matched;

    Func_0808a018();
    Func_080091c0(83, 45, 11, 8, 19, 45);

    column = Func_0808a080(19)->column;
    row = Func_0808a080(19)->row >> 20;
    Func_080091c0(20, 56, 1, 1, column >> 20, row);

    column = Func_0808a080(20)->column;
    row = Func_0808a080(20)->row >> 20;
    Func_080091c0(20, 56, 1, 1, column >> 20, row);

    column = Func_0808a080(21)->column;
    row = Func_0808a080(21)->row >> 20;
    Func_080091c0(20, 56, 1, 1, column >> 20, row);

    column = Func_0808a080(22)->column;
    row = Func_0808a080(22)->row >> 20;
    Func_080091c0(20, 56, 1, 1, column >> 20, row);

    column = Func_0808a080(23)->column;
    row = Func_0808a080(23)->row >> 20;
    Func_080091c0(20, 56, 1, 1, column >> 20, row);

    matched = 0;
    if ((Func_0808a080(19)->column >> 20) == 25
        && (Func_0808a080(19)->row >> 20) == 49) {
        matched = 1;
    }
    if ((Func_0808a080(20)->column >> 20) == 23
        && (Func_0808a080(20)->row >> 20) == 49) {
        matched++;
    }
    if ((Func_0808a080(21)->column >> 20) == 25
        && (Func_0808a080(21)->row >> 20) == 47) {
        matched++;
    }
    if ((Func_0808a080(22)->column >> 20) == 23
        && (Func_0808a080(22)->row >> 20) == 47) {
        matched++;
    }
    if ((Func_0808a080(23)->column >> 20) == 24
        && (Func_0808a080(23)->row >> 20) == 48) {
        matched++;
    }

    if (matched == 5) {
        if (Func_080770c0(0x984) != 0) {
            /* The only path that does not run Func_020053de. */
            Func_0808a020();
            return;
        }
        Func_0808a010(20);
        Func_0808a208(0xcccc, 0x1999);
        Func_0808a210(0x01d80000, -1, 0x030c0000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_080770c8(0x984);
        Func_080f9010(158);
        Func_08009178(Data_0200b3ec, 32, 46);
        Func_080091c0(24, 60, 1, 1, 32, 47);
        Func_0808a010(40);
    } else if (Func_080770c0(0x984) != 0) {
        Func_0808a010(20);
        Func_0808a208(0xcccc, 0x1999);
        Func_0808a210(0x01d80000, -1, 0x030c0000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_080770d0(0x984);
        Func_080f9010(159);
        Func_08009178(Data_0200b40c, 32, 46);
        Func_080091c0(31, 47, 1, 1, 32, 47);
        Func_0808a010(40);
    }

    Func_0808a020();
}
