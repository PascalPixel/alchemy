/// @file
/// Overlay 0x684's scene: its code, then its tables, in the order the ROM has them.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020000b8(void);

void Func_02000054(void) { Func_020000b8(); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 513;
  Actor_SetSpritePriority(8, 2);
  Actor_GetById(8)->collisionFlags |= 4;
  Actor_GetById(8)->collisionFlags |= 16;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(184),
    SCENE_EXIT_IF(1, 182, 7, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {202, CONDITION_ALWAYS, 1, PIXELS(720), 0, PIXELS(147), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0, 8, 0x197f, MSGNO(MsgChampaTreasureEncouragesLaziness)},
    {0x8d15, 8, 0x197f, MSGNO(MsgChampaVillagersStoppedWorking)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgChampaCaptainLastHope)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgChampaNeedsCaptainTreasure)},
    {TRIGGERS_END},
};
