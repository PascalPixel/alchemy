#include "types.h"

/*
 * Resource 3b1 object activation at 0x020002f4.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020002f4, interworking return
 * `pop {r5, r6} / pop {r1} / bx r1` at 0x0200033a-0x0200033e.  The
 * control-flow walk reaches every halfword of the 76-byte span; there is no
 * literal pool and the next owner starts at 0x02000340, which already has a
 * byte-exact source in assets/code.
 *
 * Signature.  The return address is popped into r1, NOT r0, so r0 survives
 * and IS the result; `movs r0, #1` immediately precedes the epilogue, so the
 * owner returns a constant 1.  One argument, the object.
 *
 * Link base 0x02008000; see resource_3b1_c_0200012c.c for the jump-table
 * witness.  Import naming uses `stored_displacement + 2` into the veneer
 * band at 0x02006450+.
 *
 * Two masks are written as `movs rN,#13 / negs rN`, so the value is -13 =
 * 0xf3: it clears the two bits 0x0c and leaves the rest, before 0x04 is set.
 * The same register serves both stores — reading the second as a fresh
 * constant is the natural mistake.
 */

void Func_02006786();
void Func_020068d6();
                       /* veneer 0x6480 */
                       /* veneer 0x65a0 */

s32 Func_020002f4(u8 *object)
{
    u8 *record;

    record = *(u8 **) (object + 0x50);
    object[89] = 8;
    Func_02006786(object, 0);

    object[35] = (u8) ((object[35] & 0xfe) | 2);
    record[9] = (u8) ((record[9] & (u8) -13) | 4);
    record[21] = (u8) ((record[21] & (u8) -13) | 4);

    Func_020068d6(object, 15);
    return 1;
}
