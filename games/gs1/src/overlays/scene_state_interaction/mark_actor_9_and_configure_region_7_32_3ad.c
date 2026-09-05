#include "types.h"

/*
 * resource_3ad owner at 0x02000094, 64 bytes.
 *
 * Marks record 9 as active, then runs the six-argument entry and one more call.
 *
 * The two stack arguments are locals ASSIGNED AFTER the guard, not initialised
 * at their declaration. Initialising them up front makes the compiler
 * materialise and store them one at a time (`movs r3,#8 / str / movs r3,#32 /
 * str`); assigning them after the record writes keeps both live at once, which
 * is the `movs r3,#8 / movs r2,#32 / str / str` the reference has.
 */

struct Rec_3ad {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[49];
    u8 f85;                     /* +85 */
};

struct Rec_3ad *Func_02001bd8();
void Func_02001bb0();

void SceneState_MarkActor9AndConfigureRegion7_32(void)
{
    struct Rec_3ad *rec = Func_02001bd8(9);
    s32 p5, p6;

    if (rec != 0) {
        rec->f35 = 1;
        rec->f85 = 0;
    }

    p5 = 8;
    p6 = 32;
    Func_02001bb0(7, 32, 1, 1, p5, p6);
    Func_02001bd8(516);
}
