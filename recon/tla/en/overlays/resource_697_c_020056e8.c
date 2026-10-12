/* Draft, differ 18 of 11: the native is the store-flag form of a logical not on a register (r0 = 1; r2 = state ^ 9; r0 -= (-r2 | r2) >> 31; bx lr, no frame), which this compiler emits for !value when value is a call result. Every form tried folds to a compare and branch with push {lr}: == 9, !(!= 9), 1 - (!= 9), (^ 9) == 0, !(^ 9), a result variable set to 1 and cleared or decremented, a delta temporary (xor or minus) then !delta, a u8 return, ?: 1 : 0. The listing section also holds an unnamed empty function (bx lr) after this one. Func_02000df8 has the same form with 255. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern u8 gSceneState[];

s32 Func_020056e8(void)
{
    return *(s16 *)gSceneState == 9;
}
