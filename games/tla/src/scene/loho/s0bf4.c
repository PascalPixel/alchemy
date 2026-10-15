#include "types.h"

extern char MsgScene6A6No32;
extern char MsgScene6A6No36;
extern char MsgScene6A6No55;
extern char MsgScene6A6No56;

s32 GameFlag_Test(s32 flag);
void Func_02001380(s32 message);
void Func_02001390(s32 id, s32 flags);

void Func_02000bf4(void)
{
    if (GameFlag_Test(0x90a))
        Func_02001380((s32)&MsgScene6A6No32);
    else
        Func_02001380((s32)&MsgScene6A6No55);
    Func_02001390(11, 0);
}

void Func_02000c24(void)
{
    if (GameFlag_Test(0x90a))
        Func_02001380((s32)&MsgScene6A6No36);
    else
        Func_02001380((s32)&MsgScene6A6No56);
    Func_02001390(11, 0);
}
