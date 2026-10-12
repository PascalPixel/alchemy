#include "types.h"
#include "object_runtime.h"

s32 GameFlag_Test(s32 flag);
extern u32 gInput;
extern s16 Data_02003272[];
void Func_02002f80(struct ObjectRuntime *object, s32 mode);
void ObjectDispatch_ApplyValueToChildren(struct ObjectRuntime *object, s32 value);

void Func_020017dc(struct ObjectRuntime *object)
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
        angle = Data_02003272[(gInput >> 4) & 15];
    if (angle == -1) {
        Func_02002f80(object, 9);
        return;
    }
    turn = (s16)(angle - object->angle);
    if (turn > 0x1000)
        turn = 0x1000;
    if (turn < -0x1000)
        turn = -0x1000;
    object->angle += turn;
    Func_02002f80(object, 2);
    ObjectDispatch_ApplyValueToChildren(object, 48);
}
