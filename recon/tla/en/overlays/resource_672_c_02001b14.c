/* Near miss, overlay 672 Func_02001b14 (0x02009b14): differ 30 of 58, one
 * cause. The native loads the party state's address twice, once in the else
 * branch and once at the entrance's second read; here global CSE's partial
 * redundancy elimination (the gcse dump: "PRE: redundant insn 106 ... edge
 * (1,6), copy expression 0") loads it once into r2 on both paths. The
 * native also loads the address before the offset's first half at both
 * reads. Measured: "+l" barriers on a pointer to the party state at each
 * read, input-only barriers, loop notes before each read and a volatile view
 * all leave the shared load (28 to 41 differ). The position call through
 * GabombaStatueChamber_ObjPosSet is measured: direct, its shifts are scheduled before the
 * actor's id. */
#include "../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"

void Func_02000f00(void);
void Func_02001010(void);
void Func_02001064(void);
void Func_020010d4(void);
void Func_02001710(void);

void Func_02001b14(void)
{
    Func_02001e80(8);
    Func_02001e80(9);
    Func_02001e80(10);
    WaitFrames(1);
    if (GameFlag_Test(0x8fd)) {
        Func_02001010();
        Func_02001064();
        GabombaStatueChamber_ObjPosSet(8, 0x1a80000, 0x980000);
        WaitFrames(1);
        Func_02000f00();
    } else {
        s32 entrance;

        entrance = gPartyState.entrance;
        if (entrance == 5) {
            Func_020010d4();
            return;
        }
        if (entrance == 6) {
            Func_02001710();
            return;
        }
    }
    if (gPartyState.entrance == 3) {
        GameFlag_ClearBit(0xa27);
        GameFlag_SetBit(0xa28);
    }
}
