/// @file
/// The entrance to the Anemos Inner Sanctum, sealed under Contigo, which the
/// party reaches with Teleport once it has all seventy-two Djinn
/// (SceneId_AnemosInnerSanctumEntrance).
#include "../field/actor_set_sprite_priority.c"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/event_commands.h"
#include "field/field_sequence.h"

#include "../field/actor_prepare.c"
#include "flags.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "graphics/palette_buffers.h"

struct AnemosInnerSanctumEntranceParty {
  u8 unk000[0x24b];
  u8 battleRule;
  u8 unk24c[8];
  u16 battleGroup;
};

void AnemosInnerSanctumEntrance_PlaceStatues(void);
void AnemosInnerSanctumEntrance_SetLightPicture(u16 value);
void Actor_MoveToObject(s32 id, s32 a);
void Field_TryJumpForward(void);
#include "field/field.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "field/scene_call.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "version.h"
#include "field/map_grid.h"
#include "field/field_event.h"
#include "graphics/vram_tab.h"
#include "system/frame.h"
#include "system/iwram_call.h"
#include "system/task.h"
#include "macros.h"
#include "shared/tables.h"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

/// The entrance room (gAnemosInnerSanctumEntrance_HallPlacements).
enum AnemosInnerSanctumEntranceActorId {
  /// "I am the shadow, the keeper of light."
  ACTOR_SHADOW = ACTOR_FIRST_PLACED,
  /// The six stone beasts set in place before the way opens.
  ///@{
  ACTOR_STATUE_A,
  ACTOR_STATUE_B,
  ACTOR_STATUE_C,
  ACTOR_STATUE_D,
  ACTOR_STATUE_E,
  ACTOR_STATUE_F,
  ///@}
  ACTOR_BLUE_ORB,
  /// The room's fourteen torches.
  ACTOR_FIRST_TORCH
};

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern s32 gAnemosInnerSanctumEntrance_GlowGrowsScript[];
extern s32 gAnemosInnerSanctumEntrance_GlowShrinksScript[];
extern s32 gAnemosInnerSanctumEntrance_ShadowScript[];
extern s8 gAnemosInnerSanctumEntrance_StatueOrder[];
void AnemosInnerSanctumEntrance_StartLightSprites(void);
void AnemosInnerSanctumEntrance_SetLightLevel(u16 value);
void AnemosInnerSanctumEntrance_OpenTheWay(void);

extern s32 gAnemosInnerSanctumEntrance_StatueScript[];

struct Actor;

extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpriteTile;
extern s16 gLightSpriteLargeOam;

#include "text/msg_ids.h"

s32 AnemosInnerSanctumEntrance_ClearScriptedActorSpriteFlags(
    struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[];
extern struct ActorPlacement gAnemosInnerSanctumEntrance_HallPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_HallPlacements;
  return gAnemosInnerSanctumEntrance_Placements;
}

void AnemosInnerSanctumEntrance_ShadowChallenges(void) {
  struct AnemosInnerSanctumEntranceParty *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_SHADOW, 2);
  Event_Wait(20);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)&MsgIAmTheShadow, 0, 6);
  party = (struct AnemosInnerSanctumEntranceParty *)&gMainRoutineWork;
  party->battleGroup = 0x1a9f;
  party->battleRule = 2;
  Field_RequestBattleFromEncounterTable(103, 3);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
}

void AnemosInnerSanctumEntrance_GlowRises(void) {
  struct Actor *object = Actor_GetById(ACTOR_BLUE_ORB);

  object->motionFlags = 0;
  object->scaleX = Q16_ONE / 4;
  object->scaleY = Q16_ONE / 4;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_LIGHT_APPEAR);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SHADOW), 7);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_BLUE_ORB), 7);
  Event_Wait(20);
  Sound_Play(SE_WARP);
  Actor_MoveToObject(ACTOR_BLUE_ORB, 8);
  wait(1);
  Actor_SetMovementTypeById(ACTOR_SHADOW,
                            gAnemosInnerSanctumEntrance_ShadowScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_BLUE_ORB,
                                   gAnemosInnerSanctumEntrance_GlowGrowsScript);
  Actor_SetMovementTypeAndWaitById(
      15, gAnemosInnerSanctumEntrance_GlowShrinksScript);
  Event_Wait(10);
  Event_End();
}

void AnemosInnerSanctumEntrance_PlaceStatues(void) {
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

void AnemosInnerSanctumEntrance_ShowByHeight(struct Actor *object) {
  if ((object->groundHeight = Map_GetTerrainHeight(0, object->x.fixed,
                                                   object->z.fixed)) >= -px(32))
    Actor_SetSpriteFlags(object, 1);
  else
    Actor_SetSpriteFlags(object, 0);
}

extern s8 gAnemosInnerSanctumEntrance_StatueSpots[];

void AnemosInnerSanctumEntrance_MoveStatue(s32 a, s32 b) {
  s32 i;

  const s8 *x = gAnemosInnerSanctumEntrance_StatueSpots;
  const s8 *z = gAnemosInnerSanctumEntrance_StatueSpots + 1;

  for (i = 9; i <= 14; i++) {
    struct Actor *object = Actor_GetById(i);

    if (FIELD_CELL(object->x.fixed) != *x)
      return;
    {
      s32 cellZ = *z;

      z += 2;
      x += 2;
      if (FIELD_CELL(object->z.fixed) != cellZ)
        return;
    }
  }
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  Sound_Play(SE_ASCEND);
  AnemosInnerSanctumEntrance_SetLightPicture(1);
  AnemosInnerSanctumEntrance_PlaceStatues();
}

void AnemosInnerSanctumEntrance_StepWhilePushing(s32 a) {
  struct MainRoutineWork *party;
  s32 held = Leader_CanPushObjectAhead();

  party = &gMainRoutineWork;
  if (held) {
    if (Field_FindNearbyTalkTarget(party->leaderActorId) != -1) {
      Leader_PushObjectAhead();
      AnemosInnerSanctumEntrance_MoveStatue(0, a);
    }
  } else {
    if (Actor_GetById(party->leaderActorId)->y.fixed > -px(28))
      Field_TryJumpForward();
  }
}

void AnemosInnerSanctumEntrance_OpenTheWay(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 shift = 0x1300000;
  s32 i;

  if (FIELD_CELL(object->z.fixed) <= 9)
    shift = -shift;
  Event_ResolvePendingActions(0);
  if (!GameFlag_Get(FLAG_SCENE_TEMP_00))
    return;
  if (MapGrid_GetCellBehaviorAt(object->layer, object->x.fixed,
                                object->z.fixed) != 77)
    return;
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
#if TLA_PATCH_1
  FieldActor_SetMoveTargetFixed(
      object, (object->x.fixed & FIELD_CELL_MASK) + px(8), object->y.fixed,
      (object->z.fixed & FIELD_CELL_MASK) + px(8));
#endif
  {
    s32 id = gMainRoutineWork.leaderActorId;

    if (shift > 0)
      Actor_FaceDirectionById(id, FACING_NORTH, 0);
    else
      Actor_FaceDirectionById(id, FACING_SOUTH, 0);
  }
  wait(10);
  i = 0;
next:
  {
    object = Actor_GetById(gAnemosInnerSanctumEntrance_StatueOrder[i]);
    object->motionFlags = 0;
    object->y.fixed += px(1) / 2;
    object->acceleration = px(1) / 16;
    object->speedLimit = px(2);
    Actor_SetSpriteFlags(object, 1);
    Actor_SetMoveTarget(object, object->x.fixed, -px(8), object->z.fixed);
    object->update = AnemosInnerSanctumEntrance_ShowByHeight;
    Sound_Play(SE_JUMP);
    wait((Random_Below(4)) + 3);
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
      s32 z = object->z.fixed - shift;

      Actor_SetMoveTarget(object, object->x.fixed, -px(8), z);
    }
  }
  if (++i <= 14)
    goto lower;
  Actor_WaitForMove(Actor_GetById(ACTOR_STATUE_A));
  wait(40);
  i = 9;
land:
  {
    object = Actor_GetById(i);
    object->z.fixed = (object->z.fixed & FIELD_CELL_MASK) + px(8);
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    object->update = 0;
  }
  if (++i <= 14)
    goto land;
  wait(3);
  Sound_Play(SE_THUMP);
  wait(10);
  if (!GameFlag_Get(0xa9f) && shift > 0)
    AnemosInnerSanctumEntrance_ShadowChallenges();
}

void AnemosInnerSanctumEntrance_DoNothing(void) {}

extern struct Trigger gAnemosInnerSanctumEntrance_Triggers[];
extern struct Trigger gAnemosInnerSanctumEntrance_HallTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_HallTriggers;
  return gAnemosInnerSanctumEntrance_Triggers;
}

static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

/// Sets the event route to 0x204. In the first room, gives the six statues
/// their values and priority bits, and under flag 0x200 runs
/// AnemosInnerSanctumEntrance_PlaceStatues. Under flag 0x201, runs a screen
/// transition and AnemosInnerSanctumEntrance_GlowRises, then clears the flag.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineWork.next == (s32)&SceneId_AnemosInnerSanctumEntrance) {
    AnemosInnerSanctumEntrance_StartLightSprites();
    AnemosInnerSanctumEntrance_SetLightLevel(2);
    // LightSprites_Add, defined below, is called with no prototype in scope.
    LightSprites_Add(ACTOR_STATUE_A, 0);
    LightSprites_Add(ACTOR_STATUE_B, 1);
    LightSprites_Add(ACTOR_STATUE_C, 2);
    LightSprites_Add(ACTOR_STATUE_D, 3);
    LightSprites_Add(ACTOR_STATUE_E, 128);
    LightSprites_Add(ACTOR_STATUE_F, 129);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x40;
    Actor_GetById(ACTOR_STATUE_A)->priorityFlags |= 0xc0;
    Actor_GetById(ACTOR_STATUE_B)->priorityFlags |= 0x80;
    Actor_GetById(ACTOR_STATUE_C)->priorityFlags |= 0x80;
    Actor_GetById(ACTOR_STATUE_D)->priorityFlags |= 0x80;
    Actor_GetById(ACTOR_STATUE_E)->priorityFlags |= 0x80;
    Actor_GetById(ACTOR_STATUE_F)->priorityFlags |= 0x80;
    if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
      AnemosInnerSanctumEntrance_SetLightPicture(1);
      AnemosInnerSanctumEntrance_PlaceStatues();
    }
  }
  if (Flag_Test(FLAG_SCENE_TEMP_01)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    AnemosInnerSanctumEntrance_GlowRises();
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern struct LightOam gLightSpriteOam[];
extern s16 gAnemosInnerSanctumEntrance_LightLevel;
extern s16 gAnemosInnerSanctumEntrance_LightPicture;

void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gVramCacheEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gLightSpriteOam;
  picture = gAnemosInnerSanctumEntrance_LightPicture *
            ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];

    if (actor != 0 && actor->x.fixed != 0) {
      s32 ox = actor->x.fixed - viewX;
      s32 y = actor->y.fixed - viewY + px(8);
      s32 z = actor->z.fixed - viewZ - viewY;
      s32 oy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 mode = gLightSpriteLargeOam;
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
        if (gAnemosInnerSanctumEntrance_LightLevel == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gAnemosInnerSanctumEntrance_LightLevel;
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
        if (gAnemosInnerSanctumEntrance_LightLevel == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gAnemosInnerSanctumEntrance_LightLevel;
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
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 0;
  gAnemosInnerSanctumEntrance_LightLevel = -1;
}

void AnemosInnerSanctumEntrance_StartLightSprites(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWordsEntry(gLightSpriteActors, 64);
  Decompress_TaggedLz(gField_CachedTilesD, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0x1800, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpriteLargeOam = 0;
  gAnemosInnerSanctumEntrance_LightLevel = -1;
  gAnemosInnerSanctumEntrance_LightPicture = 0;
}

void LightSprites_Add(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->timer = value;
    gLightSpriteActors[gLightSpriteCount] = object;
    gLightSpriteCount++;
  }
}

void AnemosInnerSanctumEntrance_SetLightLevel(u16 value) {
  gAnemosInnerSanctumEntrance_LightLevel = value;
}

void AnemosInnerSanctumEntrance_SetLightPicture(u16 value) {
  gAnemosInnerSanctumEntrance_LightPicture = value;
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctumEntrance_GlowGrowsScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 64),
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

s32 gAnemosInnerSanctumEntrance_GlowShrinksScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_ADD(SW_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gAnemosInnerSanctumEntrance_ShadowScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

s8 gAnemosInnerSanctumEntrance_StatueSpots[] = {11, 27, 11, 28, 12, 27,
                                                12, 28, 13, 27, 13, 28};

s8 gAnemosInnerSanctumEntrance_StatueOrder[] = {
    12, 14, 9, 11, 13, 10,
};

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LATE_DATA s32 gAnemosInnerSanctumEntrance_StatueScript[] = {
    46,
    (s32)AnemosInnerSanctumEntrance_ClearScriptedActorSpriteFlags,
    17,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(AnemosInnerSanctumEntrance),
    SCENE_EXIT_UNLESS(1, ContigoGeoglyph, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumDragonSkinChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ContigoGeoglyph, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ContigoGeoglyph, 24, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctumOrihalconChest, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_HallPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DULLAHAN, 0xa9f, MOVEMENT_TYPE_STAND, 200, 0, 104,
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
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 152, 0, 168,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 152, 0, 216,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 248, 0, 168,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 248, 0, 216,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 696, 0, 240,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 744, 0, 240,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 424, 0, 232,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 424, 0, 288,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 504, 0, 232,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 504, 0, 288,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 424, 0, 712,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 424, 0, 768,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 504, 0, 712,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_StatueScript, 504, 0, 768,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_HallTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_WALKING, 33, CONDITION_ALWAYS,
     EVENT(AnemosInnerSanctumEntrance_StepWhilePushing)},
    {STEP_TRIGGER_WALKING, 77, CONDITION_ALWAYS,
     EVENT(AnemosInnerSanctumEntrance_StepWhilePushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_A, FLAG_SCENE_TEMP_00,
     EVENT(AnemosInnerSanctumEntrance_MoveStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_B, FLAG_SCENE_TEMP_00,
     EVENT(AnemosInnerSanctumEntrance_MoveStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_C, FLAG_SCENE_TEMP_00,
     EVENT(AnemosInnerSanctumEntrance_MoveStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_D, FLAG_SCENE_TEMP_00,
     EVENT(AnemosInnerSanctumEntrance_MoveStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_E, FLAG_SCENE_TEMP_00,
     EVENT(AnemosInnerSanctumEntrance_MoveStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_F, FLAG_SCENE_TEMP_00,
     EVENT(AnemosInnerSanctumEntrance_MoveStatue)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 77,
     IF_FLAG_SET(FLAG_SCENE_TEMP_00),
     EVENT(AnemosInnerSanctumEntrance_OpenTheWay)},
    {TRIGGER_TABLE_END},
};
