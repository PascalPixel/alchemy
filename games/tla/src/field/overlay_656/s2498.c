#include "ov656.h"

void Func_0200143c(void);
void Func_02001944(s32 actor, s32 id);
void Func_0200245c(s32 actor);

// Restores six of the scene's map edits and actors 16 to 22 from flags 0x938 to 0x93e, then runs the pending step of one of flags 0x228 to 0x22e.
void Func_02002498(void)
{
    Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
    Func_02002dc8(Data_02003010);
    Object_GetById(8)->priority_flags |= 32;
    ObjectMotion_SetActionVariant(8, 1);
    if (Ov656_FlagCheck(0x938))
    {
        Ov656_BlendStart(44, 0, 1, 1, 48, 32);
        Ov656_ObjSetPos(Object_GetById(16), 0x3080000, 0, 0x2080000);
        Object_GetById(16)->height = 0;
        if (!Ov656_FlagCheck(0x228))
            Ov656_ObjPlace(9, 0, 0);
    }
    else
    {
        Func_0200245c(9);
    }
    if (Ov656_FlagCheck(0x939))
    {
        Ov656_BlendStart(44, 0, 1, 1, 53, 32);
        Ov656_ObjSetPos(Object_GetById(17), 0x3580000, 0, 0x2080000);
        Object_GetById(17)->height = 0;
        if (!Ov656_FlagCheck(0x229))
            Ov656_ObjPlace(10, 0, 0);
    }
    else
    {
        Func_0200245c(10);
    }
    if (Ov656_FlagCheck(0x93a))
    {
        Ov656_BlendStart(44, 0, 1, 1, 43, 37);
        Ov656_ObjSetPos(Object_GetById(18), 0x2b80000, 0, 0x2580000);
        Object_GetById(18)->height = 0;
        if (!Ov656_FlagCheck(0x22a))
            Ov656_ObjPlace(11, 0, 0);
    }
    else
    {
        Func_0200245c(11);
    }
    if (Ov656_FlagCheck(0x93b))
    {
        Ov656_BlendStart(44, 0, 1, 1, 48, 37);
        Ov656_ObjSetPos(Object_GetById(19), 0x3080000, 0, 0x2580000);
        Object_GetById(19)->height = 0;
        if (!Ov656_FlagCheck(0x22b))
            Ov656_ObjPlace(12, 0, 0);
    }
    else
    {
        Func_0200245c(12);
    }
    if (Ov656_FlagCheck(0x93c))
    {
        Ov656_BlendStart(44, 0, 1, 1, 50, 37);
        Ov656_ObjSetPos(Object_GetById(20), 0x3280000, 0, 0x2580000);
        Object_GetById(20)->height = 0;
        if (!Ov656_FlagCheck(0x22c))
            Ov656_ObjPlace(13, 0, 0);
    }
    else
    {
        Func_0200245c(13);
    }
    if (Ov656_FlagCheck(0x93e))
    {
        Ov656_BlendStart(44, 0, 1, 1, 48, 43);
        Ov656_ObjSetPos(Object_GetById(22), 0x3080000, 0, 0x2b80000);
        Object_GetById(22)->height = 0;
        if (!Ov656_FlagCheck(0x22e))
            Ov656_ObjPlace(15, 0, 0);
    }
    else
    {
        Func_0200245c(15);
    }
    Func_02002db8(Data_02003034);
    Ov656_TaskAdd(Func_0200143c, 0x480);
    if (Ov656_FlagCheck(0x228))
    {
        Func_02001944(9, 16);
        GameFlag_ClearBit(0x228);
    }
    else if (Ov656_FlagCheck(0x229))
    {
        Func_02001944(10, 17);
        GameFlag_ClearBit(0x229);
    }
    else if (Ov656_FlagCheck(0x22a))
    {
        Func_02001944(11, 18);
        GameFlag_ClearBit(0x22a);
    }
    else if (Ov656_FlagCheck(0x22b))
    {
        Func_02001944(12, 19);
        GameFlag_ClearBit(0x22b);
    }
    else if (Ov656_FlagCheck(0x22c))
    {
        Func_02001944(13, 20);
        GameFlag_ClearBit(0x22c);
    }
    else if (Ov656_FlagCheck(0x22e))
    {
        Func_02001944(15, 22);
        GameFlag_ClearBit(0x22e);
    }
}
