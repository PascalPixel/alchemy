#include "types.h"

/* battle/effects/scene_transition/finish_and_release_heap_block.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Battle_Apply(void *a, s32 b);
void Battle_Check(s32 a);
void Battle_Run(void);
void ScheduleCallback(s32);
void *Object_GetById(u32);
void BattleFx_PrepareBufferInterpolation(void);
extern u8 RomBytes_08097645;

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)ADDR_03001EA8;
    Battle_Run();
    ScheduleCallback((s32)&RomBytes_08097645);
    Battle_Apply(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Battle_Check(0x16);
}
