#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/battle/effects/object_control/resume_object.h"

void Object_SetAction(void *, s32);

extern u8 gCell[];

void BattleFx_ResumeObject(void)
{
    u8 *object = ObjectTable_Get();
    if (object != 0) {
        if (*(void (**)(void))(object + 0x6C) == Battle_Run) {
            u8 *state = gCell;
            *(s32 *)(object + 0x6C) = *(s32 *)(state + 0x250);
            *(s32 *)(state + 0x250) = 0;
            Battle_Apply(object, *(s8 *)(state + 0x249));
        }
        object[0x5B] = 0;
        Object_SetAction(object, 16);
    }
}
