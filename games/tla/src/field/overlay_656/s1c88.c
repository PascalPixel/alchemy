#include "ov656.h"

void Func_02001944(s32 actor, s32 target);

// Places actors 10 and 11 as flags 0x930 and 0x931 say, then runs the pending step of flag 0x220 or 0x221.
void Func_02001c88(void)
{
    s32 flag;

    if (Ov656_FlagCheck(0x930))
    {
        flag = Ov656_FlagCheck(0x220);
        if (flag != 0)
        {
            Ov656_BlendStart(0, 25, 1, 3, 6, 25);
            Ov656_ObjSetPos(Object_GetById(10), 0x680000, 0, 0x1a80000);
            Object_GetById(10)->height = 0;
        }
        else
        {
            Ov656_ObjPlace(8, 0, 0);
            Ov656_BlendStart(0, 25, 1, 3, 6, 25);
            Ov656_ObjSetPos(Object_GetById(10), 0x680000, 0, 0x1a80000);
            Object_GetById(10)->height = 0;
        }
        Ov656_WaitFrames(1);
    }
    if (Ov656_FlagCheck(0x931))
    {
        if (Ov656_FlagCheck(0x221))
            ObjectMotion_SetActionVariant(9, 1);
        else
            Ov656_ObjPlace(9, 0, 0);
        Ov656_ObjSetPos(Object_GetById(11), 0x680000, 0xffe00000, 0x2180000);
        Object_GetById(11)->height = 0xffe00000;
        ObjectMotion_SetActionVariant(11, 1);
        Ov656_WaitFrames(1);
    }
    Func_02002db8(Data_02003014);
    if (Ov656_FlagCheck(0x220))
    {
        Func_02001944(8, 10);
        GameFlag_ClearBit(0x220);
    }
    else if (Ov656_FlagCheck(0x221))
    {
        Func_02001944(9, 11);
        GameFlag_ClearBit(0x221);
    }
}
