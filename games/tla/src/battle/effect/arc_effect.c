#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "arc_effect.h"



void BattleFx_StartArcEffect(void)
{
    struct ArcEffectWork *work = Runtime_AllocateHeapBlock(248, sizeof(*work));

    work->resource = Resource_FindFreeEntry();
    VramBlock_LoadCached(work->resource, 0x100, BattleFx_ArcTiles);
    work->group = 0;
    work->age[0] = 999;
    work->age[1] = 999;
    work->target = 0;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateArcEffect, 1152);
}

void BattleFx_EndArcEffect(void)
{
    struct ArcEffectWork *work = Ram_HeapSlots->block_62;

    Resource_ResetEntry(work->resource);
    Scheduler_RemoveCallback((u32)BattleFx_UpdateArcEffect);
    Runtime_ReleaseHeapBlock(248);
}
