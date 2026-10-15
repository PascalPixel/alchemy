#include "ov656.h"

void Func_02001464(void);
void Func_0200155c(void);

// Restores the scene from its flags: two map edits, actors 8 to 11 and two scheduled tasks.
void Func_02002970(void)
{
    struct Ov656Object *object;

    Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
    Func_02002dc8(Data_0200304e);
    if (Ov656_FlagChk(0x982))
    {
        Ov656_MapCopy(4, 124, 31, 64, 3, 2);
        Ov656_FlagSet(0x202);
        Ov656_FlagSet(0x203);
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    if (Ov656_FlagChk(0x202))
    {
        object = Object_GetById(8);
        Ov656_MapCopy(35, 68, 35, 69, 1, 1);
        Ov656_BlendStart(35, 0, 1, 1, 35, 9);
        object->motion_flags = 0;
        Ov656_ObjSetPos(object, 0x2380000, 0xfff00000, 0x980000);
        ObjectMotion_SetActionVariant(8, 3);
        Ov656_FlagSet(0x204);
        Ov656_ObjPlace(10, 0x2380000, 0x980000);
    }
    if (Ov656_FlagChk(0x204))
    {
        if (Ov656_FlagChk(0x202))
            Object_SetModeById(8, 3);
        else
            Object_SetModeById(8, 2);
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    if (!Ov656_FlagChk(0x109))
        Ov656_FlagSet(0x203);
    if (Ov656_FlagChk(0x203))
    {
        object = Object_GetById(9);
        Ov656_MapCopy(29, 68, 29, 69, 1, 1);
        Ov656_BlendStart(35, 0, 1, 1, 29, 9);
        object->motion_flags = 0;
        object->y = 0xfff00000;
        ObjectMotion_SetActionVariant(9, 3);
        Ov656_FlagSet(0x205);
        Ov656_ObjPlace(11, 0x1d80000, 0x980000);
    }
    if (Ov656_FlagChk(0x205))
    {
        if (Ov656_FlagChk(0x203))
            Object_SetModeById(9, 3);
        else
            Object_SetModeById(9, 2);
    }
    Ov656_WaitFrames(1);
    Func_02002db8(Data_02003014);
    Ov656_TaskAdd(Func_02001464, 0x480);
    if (!Ov656_FlagChk(0x210))
        Object_SetPartAttribute(Object_GetById(10), 15);
    if (!Ov656_FlagChk(0x211))
        Object_SetPartAttribute(Object_GetById(11), 15);
    OV656_TASK_ADD(Func_0200155c, 0x480);
}
