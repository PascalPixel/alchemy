#include "types.h"

extern u8 Data_02003040[];

void Func_080fa458(void);
void Func_080fa490(void);
s32 WaitFrames(s32);

u8 AudioCommand_GetStateByte(void)
{
    return *(u8 *)0x02003000;
}

void AudioCommand_StopAllPlayers(void)
{
    Func_080fa458();
}

void AudioCommand_ResumeAllPlayers(void)
{
    Func_080fa490();
}

void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & 0x80;

    command &= 0x7f;
    if (toggle != 0)
        Data_02003040[0] ^= command;
    else
        Data_02003040[0] = command;
}

u8 AudioCommand_GetSecondaryStateByte(void)
{
    return *(u8 *)0x0200303c;
}

void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (*(u8 *)0x02003000 == 0)
            break;
        WaitFrames(1);
        wait_count++;
    } while (wait_count <= 299);
}

s32 AudioCommand_GetWidth(s32 command)
{
    if (command == 0x46 || command == 0x4b || command == 0x43)
        return 3;
    return 2;
}
