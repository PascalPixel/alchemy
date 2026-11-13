
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"
#include "flags.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "scene_ids.h"

struct AnemosInnerSanctumEntranceParty {
  u8 unk000[0x24b];
  u8 battleRule;
  u8 unk24c[8];
  u16 battleGroup;
};

void Func_02000494(void);
void Func_02000be0(u16 value);
void Actor_MoveToObject(s32 id, s32 a);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
s32 Field_FindNearbyTalkTarget(s32 id);
void Field_TryJumpForward(void);
s32 Leader_CanPushObjectAhead(void);
void Leader_PushObjectAhead(void);
#include "field/field.h"
#include "types.h"
#include "actor.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_call.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "graphics/vram_tab.h"
#include "system/frame.h"
#include "system/iwram_call.h"
#include "system/task.h"
#include "macros.h"
#include "shared/tables.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

extern s32 gData_02000e20[];
extern s32 gData_02000e5c[];
extern s32 gData_02000ea4[];
extern s8 gData_02000eec[];
void Func_02000b18(void);
void Func_02000ba4(s32 id, s32 value);
void Func_02000bd4(u16 value);
void Func_02000588(void);

extern s32 gData_02001970[];

struct Actor;

extern s16 gData_02001cde;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpriteTile;
extern s16 gData_02001de0;

#include "text/msg_ids.h"

s32 Func_0200033c(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[];
extern struct ActorPlacement gAnemosInnerSanctumEntrance_Room1Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_Room1Placements;
  return gAnemosInnerSanctumEntrance_Placements;
}

void Func_02000388(void) {
  struct AnemosInnerSanctumEntranceParty *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)&MsgIAmTheShadow, 0, 6);
  party = (struct AnemosInnerSanctumEntranceParty *)&gMainRoutineWork;
  party->battleGroup = 0x1a9f;
  party->battleRule = 2;
  Field_RequestBattleFromEncounterTable(103, 3);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
}

void Func_02000408(void) {
  struct Actor *object = Actor_GetById(15);

  object->motionFlags = 0;
  object->scaleX = 0x4000;
  object->scaleY = 0x4000;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_LIGHT_APPEAR);
  Actor_SetSpriteColor(Actor_GetById(8), 7);
  Actor_SetSpriteColor(Actor_GetById(15), 7);
  Event_Wait(20);
  Sound_Play(SE_WARP);
  Actor_MoveToObject(15, 8);
  wait(1);
  Actor_SetMovementTypeById(8, gData_02000ea4);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e20);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e5c);
  Event_Wait(10);
  Event_End();
}

void Func_02000494(void) {
  s32 i;

  for (i = 9; i < 15; i++) {
    struct Actor *object = Actor_GetById(i);
    u8 *flag = (u8 *)object + 98;
    s32 none = 0;
    s32 on = 1;

    *flag = on;
    *(s32 *)((u8 *)object + 108) = none;
  }
}

void Func_020004b4(struct Actor *object) {
  if ((object->terrainHeight = Map_GetTerrainHeight(0, object->x, object->z)) >=
      -0x200000)
    Actor_SetSpriteFlags(object, 1);
  else
    Actor_SetSpriteFlags(object, 0);
}

extern s8 gData_02000ee0[];

void Func_020004e4(s32 a, s32 b) {
  s32 i;

  const s8 *x = gData_02000ee0;
  const s8 *z = gData_02000ee0 + 1;

  for (i = 9; i <= 14; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x >> 20 != *x)
      return;
    {
      s32 cellZ = *z;

      z += 2;
      x += 2;
      if (object->z >> 20 != cellZ)
        return;
    }
  }
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  Sound_Play(SE_ASCEND);
  Func_02000be0(1);
  Func_02000494();
}

void Func_02000534(s32 a) {
  struct MainRoutineWork *party;
  s32 held = Leader_CanPushObjectAhead();

  party = &gMainRoutineWork;
  if (held) {
    if (Field_FindNearbyTalkTarget(party->currentOwner) != -1) {
      Leader_PushObjectAhead();
      Func_020004e4(0, a);
    }
  } else {
    if (Actor_GetById(party->currentOwner)->y > -0x1c0000)
      Field_TryJumpForward();
  }
}

void Func_02000588(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 shift = 0x1300000;
  s32 i;

  if (object->z >> 20 <= 9)
    shift = -shift;
  Event_ResolvePendingActions(0);
  if (!GameFlag_Get(FLAG_SCENE_TEMP_00))
    return;
  if (MapGrid_GetCellBehaviorAt(object->terrainId, object->x, object->z) != 77)
    return;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
#if TLA_PATCH_1
  FieldActor_SetMoveTargetFixed(object, (object->x & 0xfff00000) + 0x80000,
                                object->y, (object->z & 0xfff00000) + 0x80000);
#endif
  {
    s32 id = gMainRoutineWork.currentOwner;

    if (shift > 0)
      Actor_ArmCallback(id, 0xc000, 0);
    else
      Actor_ArmCallback(id, 0x4000, 0);
  }
  wait(10);
  i = 0;
next:
  {
    object = Actor_GetById(gData_02000eec[i]);
    object->motionFlags = 0;
    object->y += 0x8000;
    object->acceleration = 0x1000;
    object->speedLimit = 0x20000;
    Actor_SetSpriteFlags(object, 1);
    Actor_SetMoveTarget(object, object->x, 0xfff80000, object->z);
    object->update = Func_020004b4;
    Sound_Play(SE_JUMP);
    wait((Random16() * 4 >> 16) + 3);
  }
  if (++i <= 6)
    goto next;
  wait(40);
  Sound_Play(SE_KOLIMA_BRIDGE_POST);
  i = 9;
lower:
  {
    object = Actor_GetById(i);
    {
      s32 z = object->z - shift;

      Actor_SetMoveTarget(object, object->x, 0xfff80000, z);
    }
  }
  if (++i <= 14)
    goto lower;
  Actor_WaitForMove(Actor_GetById(9));
  wait(40);
  i = 9;
land:
  {
    object = Actor_GetById(i);
    object->z = (object->z & 0xfff00000) + 0x80000;
    object->motionFlags = 3;
    object->update = 0;
  }
  if (++i <= 14)
    goto land;
  wait(3);
  Sound_Play(SE_THUMP);
  wait(10);
  if (!GameFlag_Get(0xa9f) && shift > 0)
    Func_02000388();
}

void Func_02000710(void) {}

extern struct Trigger gAnemosInnerSanctumEntrance_Triggers[];
extern struct Trigger gAnemosInnerSanctumEntrance_Room1Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_Room1Triggers;
  return gAnemosInnerSanctumEntrance_Triggers;
}

static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

static __inline__ void SetObjectValue(s32 id, s32 value) {
  /* FAKEMATCH(pin): the value pinned to r1, which the ROM loads before the id
   * (movs r1. */
  register s32 v asm("r1") = value;

  Func_02000ba4(id, v);
}

// Sets the event route to 0x204. In the first room, gives objects 9 to 14
// their values and priority bits, and under flag 0x200 runs Func_02000494.
// Under flag 0x201, runs a screen transition and Func_02000408, then clears
// the flag.
s32 Scene_Initialize(void) {
  s32 *route =
      &((struct AnemosInnerSanctumEntranceEventWork *)Ram_HeapSlots->eventWork)
           ->route;
  s32 value = 0x204;

  /* FAKEMATCH(barrier): an empty asm after the address and the value. */
  __asm__("");
  *route = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnemosInnerSanctumEntrance) {
    Func_02000b18();
    Func_02000bd4(2);
    SetObjectValue(9, 0);
    SetObjectValue(10, 1);
    SetObjectValue(11, 2);
    SetObjectValue(12, 3);
    SetObjectValue(13, 128);
    SetObjectValue(14, 129);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
    Actor_GetById(9)->priorityFlags |= 0xc0;
    Actor_GetById(10)->priorityFlags |= 0x80;
    Actor_GetById(11)->priorityFlags |= 0x80;
    Actor_GetById(12)->priorityFlags |= 0x80;
    Actor_GetById(13)->priorityFlags |= 0x80;
    Actor_GetById(14)->priorityFlags |= 0x80;
    if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
      Func_02000be0(1);
      Func_02000494();
    }
  }
  if (Flag_Test(FLAG_SCENE_TEMP_01)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000408();
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern struct LightOam gData_02001d20[];
extern s16 gData_02001de2;
extern s16 gData_02001de4;

void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gData_02001d20;
  picture = gData_02001de4 * ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gData_02001cde; i++) {
    struct Actor *actor = gLightSpriteActors[i];

    if (actor != 0 && actor->x != 0) {
      s32 ox = actor->x - viewX;
      s32 y = actor->y - viewY + 0x80000;
      s32 z = actor->z - viewZ - viewY;
      s32 oy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 mode = gData_02001de0;
      u32 *word;

      if (mode == 0) {
        s32 frame = actor->timer;
        s32 x = ox >> 16;
        s32 sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | (frame & 0x80) << 21 | 0x40002000;
        *word = (tile + picture + (frame & 7) * 8) | 0x800;
        if (gData_02001de2 == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_02001de2;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 frame = actor->timer;
        s32 x = ox >> 16;
        s32 sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_02001de2 == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_02001de2;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void LightSprites_InitA(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWordsEntry(gLightSpriteActors, 64);
  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2 = -1;
}

void Func_02000b18(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWordsEntry(gLightSpriteActors, 64);
  Decompress_TaggedLz(gField_CachedTilesD, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0x1800, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2 = -1;
  gData_02001de4 = 0;
}

void Func_02000ba4(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->timer = value;
    gLightSpriteActors[gData_02001cde] = object;
    gData_02001cde++;
  }
}

void Func_02000bd4(u16 value) { gData_02001de2 = value; }

void Func_02000be0(u16 value) { gData_02001de4 = value; }

// clang-format off
u32 gData_02000d6c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02000da8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02000de4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gData_02000e20[] = {
    23, 9, 0x400, 23, 10, 0x400,
    23, 6, -0x4000, 0, 1, 13,
    40, 0, 17,
};

s32 gData_02000e5c[] = {
    23, 9, -2048, 23, 10, -2048,
    23, 6, 0x4000, 0, 1, 13,
    24, 0, 3, 0, 1, 17,
};

s32 gData_02000ea4[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};
// clang-format on

s8 gData_02000ee0[] = {11, 27, 11, 28, 12, 27, 12, 28, 13, 27, 13, 28};

s8 gData_02000eec[] = {
    12, 14, 9, 11, 13, 10,
};

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gData_02000d6c,
    gData_02000da8,
    gData_02000de4,
};

LATE_DATA s32 gData_02001970[] = {
    46,
    (s32)Func_0200033c,
    17,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(AnemosInnerSanctumEntrance),
    SCENE_EXIT_UNLESS(1, Contigo4, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctum1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Contigo4, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Contigo4, 24, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctum4, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Room1Placements[] =
    {
        ACTOR_PLACEMENT(180, 0xa9f, MOVEMENT_TYPE_STAND, 200, 0, 104,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(430, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(430, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 456,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(430, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 472,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(430, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 424,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(430, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 456,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(430, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 472,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                        TALK_FACING_KEEP, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 152, 0, 168,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 152, 0, 216,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 248, 0, 168,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 248, 0, 216,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 696, 0, 240,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 744, 0, 240,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 424, 0, 232,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 424, 0, 288,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 504, 0, 232,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 504, 0, 288,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 424, 0, 712,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 424, 0, 768,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 504, 0, 712,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gData_02001970, 504, 0, 768,
                        FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
        {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_WALKING, 33, CONDITION_ALWAYS, EVENT(Func_02000534)},
    {STEP_TRIGGER_WALKING, 77, CONDITION_ALWAYS, EVENT(Func_02000534)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 77,
     IF_FLAG_SET(0x200), EVENT(Func_02000588)},
    {TRIGGER_TABLE_END},
};
