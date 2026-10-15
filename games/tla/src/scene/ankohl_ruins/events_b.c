#include "ankohl_ruins.h"

s32 Func_020005c0(struct ObjectRuntime *object)
{
    ObjectDispatch_SetSingleChildFlags(object, 0);
    ((struct FieldSprite *)object->animation)->priority = 2;
    object->priority_flags &= 0xfe;
    object->motion_flags = 0;
    {
        s32 step = -0x40000;

        /* FAKEMATCH: the object passes through an asm that reads the step, so the step's pool load comes before the y load as the native has; plain, the y load comes first (one reordered row). Measured against it: += (s32)0xfffc0000, the constant first, y and constant temporaries, the y update before the motion_flags store (differ 13), the step behind its own "+l" barrier (differ 2 and 4), the step pinned to r2 behind a one-pass block (differ 2). */
        __asm__("" : "+l"(object) : "l"(step));
        object->y += step;
    }
    return 0;
}

extern const u8 Data_020032a0[];
extern const u8 Data_020032b8[];
extern const u8 Data_020034f8[];
extern const u8 Data_02003570[];
extern const u8 Data_020035d0[];
extern const u8 Data_02003708[];
extern const u8 Data_02003750[];
extern const u8 Data_02003768[];

const u8 *Func_02000600(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov686Room1)
        return Data_020032b8;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room2)
        return Data_020034f8;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room3)
        return Data_02003570;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room4)
        return Data_020035d0;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room5)
        return Data_02003708;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room6)
        return Data_02003750;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room8)
        return Data_02003768;
    return Data_020032a0;
}

void Func_02000698(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 2;
}

void Func_020006b4(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 0;
}
