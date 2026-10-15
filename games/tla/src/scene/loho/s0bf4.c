#include "types.h"

extern char MsgLoho32;
extern char MsgLoho36;
extern char MsgLoho55;
extern char MsgLoho56;

s32  GameFlag_Test(s32 flag);
void Func_02001380(s32 message);
void Func_02001390(s32 id, s32 flags);

void Func_02000bf4(void)
{
    if (GameFlag_Test(0x90a))
        Func_02001380((s32)&MsgLoho32);
    else
        Func_02001380((s32)&MsgLoho55);
    Func_02001390(11, 0);
}

void Func_02000c24(void)
{
    if (GameFlag_Test(0x90a))
        Func_02001380((s32)&MsgLoho36);
    else
        Func_02001380((s32)&MsgLoho56);
    Func_02001390(11, 0);
}
