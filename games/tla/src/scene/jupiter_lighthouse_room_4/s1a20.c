/// @file
/// Scene code of overlay 0x69d, in ROM order.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_4.h"
#include "module/scene/scene.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Sink(struct CellTarget *puzzle, struct CellTargetEntry *tile);

s32 Func_02001bb8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *puzzle = &gTeleporters;
  struct CellTargetEntry *tile = puzzle->entries;
  s32 i;

  for (i = 0; i < puzzle->count; i++, tile++) {
    if (tile->x == x && tile->z == z) {
      if (!Event_GetFlag(tile->flag)) {
        Teleporter_Uncover(puzzle, tile);
        Event_SetFlag(tile->flag);
        puzzle->current = i;
        break;
      }
      if (i == puzzle->current)
        break;
      puzzle->current = i;
      Teleporter_Sink(puzzle, tile);
      GameFlag_SetByte(puzzle->flagByte, x);
      GameFlag_SetByte(puzzle->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void Func_02001c68(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct CellTarget *work = &gTeleporters;
  s32 x = ((s32)FieldGameFlag_GetByte(work->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(work->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Map_CenterOnFollowedPoint();
    Teleporter_DropIn();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = value;
  }
}

#include "scene_tables.h"
#include "objcmd.h"

void JupiterLighthouse_RevealThePassage(void);
void JupiterLighthouse_StartMovingPillar(void);
void JupiterLighthouse_FinishMovingPillar(void);
void JupiterLighthouse_UseRedKey(void);
void JupiterLighthouse_UseBlueKey(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(257),
    SCENE_EXIT(1, 258, 1),
    SCENE_EXIT(2, 256, 2),
    SCENE_EXIT(3, 257, 4),
    SCENE_EXIT(4, 257, 3),
    SCENE_EXIT(5, 257, 6),
    SCENE_EXIT(6, 257, 5),
    SCENE_EXIT(7, 257, 8),
    SCENE_EXIT(8, 257, 7),
    SCENE_EXIT(9, 263, 9),
    SCENE_EXIT(10, 263, 10),
    SCENE_EXIT(11, 257, 11),
    SCENE_EXIT(12, 256, 12),
    SCENE_EXIT(13, 256, 13),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {316, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(136), FACING_SOUTHEAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(136), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(168), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(168), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(200), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(200), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, -0x80000, 0, -0x80000, FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(520), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(520), FACING_SOUTHEAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(520), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(520), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(520), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXAMINE, 20, 0xa07, EVENT(JupiterLighthouse_TryLockedDoor)},
    {TRIGGER_EXAMINE, 21, 0xa08, EVENT(JupiterLighthouse_TryLockedDoor)},
    {USE_ITEM(243), 20, 0xa07, EVENT(JupiterLighthouse_UseRedKey)}, // the Red Key
    {USE_ITEM(244), 21, 0xa08, EVENT(JupiterLighthouse_UseBlueKey)}, // the Blue Key
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RevealThePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_HideThePassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByFirstCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveBySecondCyclone)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gPaletteCyclePausesInEvents[] = {
    1,
};

u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gFieldEffect_LifeScripts[] = {
    (s32)gFieldEffect_ShortLifeScript,
    (s32)gFieldEffect_MediumLifeScript,
    (s32)gFieldEffect_LongLifeScript,
};
