#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneDialogue_RunActorFourteenDialogue11AA Func_02000284
#define ActorPresentation_SetTwoSceneCells Func_020013b4

s32 Func_02001b94();
s32 Func_02001bcc(s32, s32);
s32 Func_02001be6();
s32 Func_02001c36(s32, s32, s32);
s32 Func_02001c5c(s32);
s32 Func_02001c6c(s32, s32);
s32 Func_02001c88(s32, s32);
s32 Func_02001cb2(s32, s32);
void Func_02002c44(s32, s32, s32, s32, s32, s32);
void Func_02002c76(s32, s32, s32, s32, s32, s32);

void SceneDialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Func_02001b94();
    Func_02001c36(0xE, 0, 0xA);
    Func_02001c5c(0x11AA);
    Func_02001c6c(0xE, 0);
    if (Func_02001bcc(0, 0) == 0) {
        Func_02001c88(0xE, 0);
    } else {
        work = *(void **)0x03001EBC;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Func_02001cb2(0xE, 0);
    }
    Func_02001be6();
}

void ActorPresentation_SetTwoSceneCells(void)
{
    {
        s32 extent = 2;

        Func_02002c44(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Func_02002c76(25, 15, 2, 2, extent, extent);
    }
}
