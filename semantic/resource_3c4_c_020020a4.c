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

void Func_0200519e();
void Func_0200515a();
Slot_020020a4 *Func_020051d0();
Slot_020020a4 *Func_020051d8();
void Func_0200517e();
Slot_020020a4 *Func_020051f4();
Slot_020020a4 *Func_020051fc();
void Func_020051a2();
Slot_020020a4 *Func_02005218();
Slot_020020a4 *Func_02005220();
void Func_020051c6();
Slot_020020a4 *Func_0200523c();
Slot_020020a4 *Func_02005244();
void Func_020051ea();
Slot_020020a4 *Func_02005260();
Slot_020020a4 *Func_02005268();
void Func_0200520e();
Slot_020020a4 *Func_02005284();
Slot_020020a4 *Func_02005294();
Slot_020020a4 *Func_020052a4();
Slot_020020a4 *Func_020052b2();
Slot_020020a4 *Func_020052c2();
Slot_020020a4 *Func_020052d0();
Slot_020020a4 *Func_020052e0();
Slot_020020a4 *Func_020052ee();
Slot_020020a4 *Func_020052fe();
Slot_020020a4 *Func_0200530c();
s32 Func_020052e8();
void Func_02005318();
void Func_02005310();
void Func_020053a0();
void Func_020053ba();
void Func_020053c6();
void Func_02005334();
void Func_0200532a();
void Func_02005400();
void Func_020052e2();
void Func_0200530e();
void Func_02005364();
s32 Func_02005354();
void Func_02005376();
void Func_02005406();
void Func_02005420();
void Func_0200542c();
void Func_0200539a();
void Func_02005398();
void Func_02005466();
void Func_02005348();
void Func_02005374();
void Func_020053ca();
void Func_020053de();

                    

                     

                               

                     

void Func_020020a4(void)
{
    s32 column;
    s32 row;
    s32 matched;

    Func_0200519e();
    Func_0200515a(83, 45, 11, 8, 19, 45);

    column = Func_020051d0(19)->column;
    row = Func_020051d8(19)->row >> 20;
    Func_0200517e(20, 56, 1, 1, column >> 20, row);

    column = Func_020051f4(20)->column;
    row = Func_020051fc(20)->row >> 20;
    Func_020051a2(20, 56, 1, 1, column >> 20, row);

    column = Func_02005218(21)->column;
    row = Func_02005220(21)->row >> 20;
    Func_020051c6(20, 56, 1, 1, column >> 20, row);

    column = Func_0200523c(22)->column;
    row = Func_02005244(22)->row >> 20;
    Func_020051ea(20, 56, 1, 1, column >> 20, row);

    column = Func_02005260(23)->column;
    row = Func_02005268(23)->row >> 20;
    Func_0200520e(20, 56, 1, 1, column >> 20, row);

    matched = 0;
    if ((Func_02005284(19)->column >> 20) == 25
        && (Func_02005294(19)->row >> 20) == 49) {
        matched = 1;
    }
    if ((Func_020052a4(20)->column >> 20) == 23
        && (Func_020052b2(20)->row >> 20) == 49) {
        matched++;
    }
    if ((Func_020052c2(21)->column >> 20) == 25
        && (Func_020052d0(21)->row >> 20) == 47) {
        matched++;
    }
    if ((Func_020052e0(22)->column >> 20) == 23
        && (Func_020052ee(22)->row >> 20) == 47) {
        matched++;
    }
    if ((Func_020052fe(23)->column >> 20) == 24
        && (Func_0200530c(23)->row >> 20) == 48) {
        matched++;
    }

    if (matched == 5) {
        if (Func_020052e8(0x984) != 0) {
            /* The only path that does not run Func_020053de. */
            return;
            Func_02005318();
        }
        Func_02005310(20);
        Func_020053a0(0xcccc, 0x1999);
        Func_020053ba(0x01d80000, -1, 0x030c0000, 1);
        Func_020053c6();
        Func_02005334(30);
        Func_0200532a(0x984);
        Func_02005400(158);
        Func_020052e2(Data_0200b3ec, 32, 46);
        Func_0200530e(24, 60, 1, 1, 32, 47);
        Func_02005364(40);
    } else if (Func_02005354(0x984) != 0) {
        Func_02005376(20);
        Func_02005406(0xcccc, 0x1999);
        Func_02005420(0x01d80000, -1, 0x030c0000, 1);
        Func_0200542c();
        Func_0200539a(30);
        Func_02005398(0x984);
        Func_02005466(159);
        Func_02005348(Data_0200b40c, 32, 46);
        Func_02005374(31, 47, 1, 1, 32, 47);
        Func_020053ca(40);
    }

    Func_020053de();
}
