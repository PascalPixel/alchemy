#include "object_lookup.h"
#include "types.h"

void Object_SetCallback(u8 *, void *);
void BattleRuntime_WaitIfModeZero(s32 arg0);
extern u8 Data_0809fc1c[];

void ObjectMotion_ArmCallback(s32 arg0, s32 arg1, s32 arg2) {
    u8 *object = ObjectTable_Get(arg0);
    if (object != NULL) {
        *(s16 *)(object + 0x64) = arg1;
        Object_SetCallback(object, Data_0809fc1c);
        BattleRuntime_WaitIfModeZero(arg2);
    }
}
