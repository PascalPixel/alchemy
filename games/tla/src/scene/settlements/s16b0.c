#include "local.h"

extern u32 gInput;
extern const s16 gData_02002276[];
void ObjectDispatch_ApplyValueToChildren(struct ObjectRuntime *object, s32 value);

void Func_020016b0(struct ObjectRuntime *object)
{
    s32 angle;
    s32 turn;

    if (object->action != 0) {
        object->action--;
        return;
    }
    *(u8 *)((u8 *)object + 0x5a) = 0;
    if (GameFlag_Test(0x106))
        angle = -1;
    else
        angle = gData_02002276[(gInput >> 4) & 15];
    if (angle == -1) {
        Func_02001fe0(object, 9);
        return;
    }
    turn = (s16)(angle - object->angle);
    if (turn > 0x1000)
        turn = 0x1000;
    if (turn < -0x1000)
        turn = -0x1000;
    object->angle += turn;
    Func_02001fe0(object, 2);
    ObjectDispatch_ApplyValueToChildren(object, 48);
}
