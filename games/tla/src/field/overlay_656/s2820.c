#include "ov656.h"

void Func_02001474(void);
void Func_02001a9c(void);

// Sets up the scene by flags 0x93f, 0x9e4 and 0x22f: actor 64, actors 8 to 13 and the camera.
void Func_02002820(void)
{
    struct Ov656Object *object;

    Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
    OV656_TASK_ADD(Func_02001474, 0x480);
    if (Ov656_FlagCheck(0x93f))
    {
        object = Object_GetById(64);
        object->motion_flags = 0;
        Ov656_ObjSetPos(Object_GetById(64), 0x880000, 0xffe00000, 0x1880000);
        object->height = 0xfff00000;
        ObjectMotion_SetActionVariant(64, 3);
        Object_SetPartAttribute(Object_GetById(64), 1);
        Ov656_ObjPlace(8, 0, 0);
        Ov656_ObjPlace(9, 0, 0);
        Ov656_ObjPlace(10, 0, 0);
        Ov656_ObjPlace(11, 0, 0);
        Ov656_ObjPlace(12, 0, 0);
        Ov656_ObjPlace(13, 0, 0);
        Ov656_WaitFrames(1);
    }
    if (Ov656_FlagCheck(0x9e4))
    {
        struct Ov656Object *marker = Object_GetById(64);

        Ov656_ObjPlace(64, marker->x, marker->z + 0xfed00000);
        Object_SetPartAttribute(Object_GetById(64), 0);
        Ov656_BlendStart(6, 0, 1, 2, 6, 4);
        Ov656_CamSet(0x80000, 0x100000, 0xf80000, 0x1000000);
        Object_AttachWorkTargetToObject(gPartyState.current_owner, 0);
        Func_02002c10();
        Ov656_WaitFrames(1);
    }
    if (Ov656_FlagCheck(0x22f))
    {
        Func_02001a9c();
        GameFlag_ClearBit(0x22f);
    }
}
