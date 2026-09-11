#include "types.h"
#include "scene.h"
#include "abi/battle/effects/scene_transition/finish_and_release_heap_block.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 ScheduleCallback(s32);

struct Outer0808e0b0 *Object_GetById(u32);
void BattleFx_PrepareBufferInterpolation(void);

extern u8 gRom;

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)ADDR_03001EA8;
    Battle_Run();
    ScheduleCallback((s32)&gRom);
    Battle_Apply(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Battle_Check(0x16);
}
