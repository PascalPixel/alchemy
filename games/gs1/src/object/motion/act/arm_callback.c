#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/object/motion/act/arm_callback.h"

void Object_SetCallback(u8 *, void *);
void Battle_WaitMode0(s32 arg0);
extern u8 gRom[];

void Motion_ArmCb(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *object = ObjectTable_Get(arg0);
    if (object != NULL) {
        *(s16 *)(object + 0x64) = arg1;
        Object_SetCallback(object, gRom);
        Battle_WaitMode0(arg2);
    }
}
