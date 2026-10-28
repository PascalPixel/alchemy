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
#include "task.h"
#include "ram_buffer.h"
#include "arc_effect.h"

void BattleEffect_StartArcEffect(void) {
  struct ArcEffectWork *work = Runtime_AllocateHeapBlock(248, sizeof(*work));

  work->resource = Resource_FindFreeEntry();
  VramCache_Load(work->resource, 0x100, gBattleEffect_ArcTiles);
  work->group = 0;
  work->age[0] = 999;
  work->age[1] = 999;
  work->target = 0;
  Task_Add((s32)BattleEffect_UpdateArcEffect, TASK_PASS_VBLANK);
}

void BattleEffect_EndArcEffect(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->block62;

  Resource_ResetEntry(work->resource);
  Task_Remove((u32)BattleEffect_UpdateArcEffect);
  Runtime_ReleaseHeapBlock(248);
}
