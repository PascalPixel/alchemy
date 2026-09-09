#include "types.h"

/*
 * Overlay resource_3ad. Map repaint sequence: mark the record for slot ten,
 * then run the tile and collision repaints with frame waits between them.
 */

/* Old-style declarations: overlay import arities vary per call site. */
extern u8 *Func_02001c18();
extern void Func_02001c5a();
extern void Func_02001be6();
extern void Func_02001bf2();
extern void Func_02001be8();
extern void Func_02001c0c();
extern void Func_02001c1e();
extern void Func_02001c30();
extern void Func_02001c26();
extern void Func_02001c46();
extern void Func_02001c3c();
extern void Func_02001c42();
extern void Func_02001c6c();
extern void Func_02001c80();
extern void Func_02001c92();
/*
 * The owner runs from 0x020000d4 for 220 bytes and holds no literal pool.
 * Two of the calls spell their trailing argument pair as locals; that is how
 * the reference puts those two values on the stack, so keep the spelling.
 * Every call here goes through an import veneer in this overlay.
 */
void SceneState_RunRepaintSequenceWithWaits(void)
{
    u8 *obj;
    int row;           /* Assigned at its first use, as the reference does. */

    obj = Func_02001c18(10);
    Func_02001c5a(10, 5);

    if (obj != 0) {
        Func_02001be6(obj, 0);
        obj[0x23] = 1;
    }

    row = 21;
    Func_02001bf2(41, 87, 2, 5, row, 59);
    Func_02001be8(4);

    { s32 f1 = 24; s32 g1 = 62; Func_02001c0c(3, 93, 1, 1,  f1, g1); }
    Func_02001c1e(1, 94, 1, 1, row, 55);
    Func_02001c30(43, 87, 2, 5, row, 58);
    Func_02001c26(4);

    Func_02001c46(41, 87, 2, 5, row, 58);
    Func_02001c3c(4);
    Func_02001c42(4);

    Func_02001c6c(21, 11, 2, 2, row, 13);
    { s32 f2 = 22; s32 g2 = 15; Func_02001c80(21, 11, 1, 1,  f2, g2); }
    Func_02001c92(19, 17, 1, 1, row, 14);
}
