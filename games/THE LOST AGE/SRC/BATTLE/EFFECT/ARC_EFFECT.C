#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "RAM_BUFFER.H"
#include "ARC_EFFECT.H"



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
