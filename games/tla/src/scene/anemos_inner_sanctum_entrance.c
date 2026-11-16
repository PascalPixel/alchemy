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

/// Set once the Dullahan, the shadow keeping the light, has been beaten; he is
/// gone.
#define FLAG_DULLAHAN_BEATEN 0xa9f

/// The map's cell behaviour for the cells where Hover, cast once the six
/// statues stand in their places, opens the way down.
enum { CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER = 77 };

/// A glow's value with this bit drawn mirrored left to right; its low three
/// bits are its frame.
#define LIGHT_SPRITE_MIRRORED 0x80

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
  /// The fourteen coiled dragon statues, hidden by their script.
  ACTOR_FIRST_DRAGON_STATUE
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
void AnemosInnerSanctumEntrance_SetLightPriority(u16 value);
void AnemosInnerSanctumEntrance_OpenTheWay(void);

extern s32 gAnemosInnerSanctumEntrance_DragonStatueScript[];

struct Actor;

s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
/// The actors that carry a glow, gLightSpriteCount of them.
struct Actor *gLightSpriteActors[16] BSS_DATA;
/// The glows' sprite attributes, built each frame by LightSprites_Draw.
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
s16 gAnemosInnerSanctumEntrance_LightPicture BSS_DATA;

#include "text/msg_ids.h"

/// The call gAnemosInnerSanctumEntrance_DragonStatueScript makes: clears the
/// actor's sprite flags, hiding it.
s32 AnemosInnerSanctumEntrance_ClearScriptedActorSpriteFlags(
    struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
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

/// The shadow at the door speaks, the room warms, and the battle with the
/// Dullahan starts, one the party cannot run from; FLAG_SCENE_TEMP_01 brings
/// AnemosInnerSanctumEntrance_GlowRises on the party's return.
void AnemosInnerSanctumEntrance_ShadowChallenges(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_SHADOW, 2);
  Event_Wait(20);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)&MsgIAmTheShadow, 0, 6);
  gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_DULLAHAN_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(103, 3);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
}

/// After the Dullahan's battle: the shadow and the blue orb flash, the orb
/// swells and rises while the shadow shrinks away.
void AnemosInnerSanctumEntrance_GlowRises(void) {
  struct Actor *actor = Actor_GetById(ACTOR_BLUE_ORB);

  actor->motionFlags = 0;
  actor->scaleX = FIXED_ONE / 4;
  actor->scaleY = FIXED_ONE / 4;
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
      ACTOR_BLUE_ORB, gAnemosInnerSanctumEntrance_GlowShrinksScript);
  Event_Wait(10);
  Event_End();
}

/// Leaves the six statues standing in their places, their bob counter at 1 and
/// no per-frame hook.
void AnemosInnerSanctumEntrance_PlaceStatues(void) {
  s32 i;

  for (i = ACTOR_STATUE_A; i <= ACTOR_STATUE_F; i++) {
    struct Actor *actor = Actor_GetById(i);

    actor->riseCounter = 1;
    actor->update = 0;
  }
}

/// A lowering statue's per-frame hook: shows its sprite while the ground under
/// it is no more than 32 pixels down, and hides it below that.
void AnemosInnerSanctumEntrance_ShowByHeight(struct Actor *actor) {
  if ((actor->groundHeight = Map_GetTerrainHeight(
           0, actor->x.fixed, actor->z.fixed)) >= -toFIXED(32))
    Actor_SetSpriteFlags(actor, 1);
  else
    Actor_SetSpriteFlags(actor, 0);
}

extern s8 gAnemosInnerSanctumEntrance_StatueSpots[];

/// Move on a statue: once every statue stands on its cell of
/// gAnemosInnerSanctumEntrance_StatueSpots, sets FLAG_SCENE_TEMP_00, lights the
/// statues and leaves them placed.
void AnemosInnerSanctumEntrance_MoveStatue(s32 a, s32 b) {
  s32 i;

  const s8 *x = gAnemosInnerSanctumEntrance_StatueSpots;
  const s8 *z = gAnemosInnerSanctumEntrance_StatueSpots + 1;

  for (i = ACTOR_STATUE_A; i <= ACTOR_STATUE_F; i++) {
    struct Actor *actor = Actor_GetById(i);

    if (FIELD_CELL(actor->x.fixed) != *x)
      return;
    {
      s32 cellZ = *z;

      z += 2;
      x += 2;
      if (FIELD_CELL(actor->z.fixed) != cellZ)
        return;
    }
  }
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  Sound_Play(SE_ASCEND);
  AnemosInnerSanctumEntrance_SetLightPicture(1);
  AnemosInnerSanctumEntrance_PlaceStatues();
}

/// A step on a pushing cell: pushes the statue ahead and checks the statues'
/// places, or, with nothing to push, lets the leader jump forward off a ledge.
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
    if (Actor_GetById(party->leaderActorId)->y.fixed > -toFIXED(28))
      Field_TryJumpForward();
  }
}

/// Hover on the placed statues' cell: the six statues hop up, slide 19 cells
/// away from the leader and settle; facing north with the Dullahan unbeaten,
/// the shadow then challenges the party.
void AnemosInnerSanctumEntrance_OpenTheWay(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 shift = toFIXED(304);
  s32 i;

  if (FIELD_CELL(actor->z.fixed) <= 9)
    shift = -shift;
  Event_ResolvePendingActions(0);
  if (!GameFlag_Get(FLAG_SCENE_TEMP_00))
    return;
  if (MapGrid_GetCellBehaviorAt(actor->layer, actor->x.fixed, actor->z.fixed) !=
      CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER)
    return;
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
#if TLA_PATCH_1
  FieldActor_SetMoveTargetFX(
      actor, (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8), actor->y.fixed,
      (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
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
  // BUG: the loop runs seven times, reading one byte past the six statues of
  // gAnemosInnerSanctumEntrance_StatueOrder; BUGFIX stops at the sixth.
  do {
    actor = Actor_GetById(gAnemosInnerSanctumEntrance_StatueOrder[i]);
    actor->motionFlags = 0;
    actor->y.fixed += toFIXED(1) / 2;
    actor->acceleration = toFIXED(1) / 16;
    actor->speedLimit = toFIXED(2);
    Actor_SetSpriteFlags(actor, 1);
    Actor_SetMoveTarget(actor, actor->x.fixed, -toFIXED(8), actor->z.fixed);
    actor->update = AnemosInnerSanctumEntrance_ShowByHeight;
    Sound_Play(SE_JUMP);
    wait((Random_Below(4)) + 3);
#ifdef BUGFIX
  } while (++i < 6);
#else
  } while (++i <= 6);
#endif
  wait(40);
  Sound_Play(SE_KOLIMA_BRIDGE_POST);
  i = ACTOR_STATUE_A;
  do {
    actor = Actor_GetById(i);
    {
      FIXED z = actor->z.fixed - shift;

      Actor_SetMoveTarget(actor, actor->x.fixed, -toFIXED(8), z);
    }
  } while (++i <= ACTOR_STATUE_F);
  Actor_WaitForMove(Actor_GetById(ACTOR_STATUE_A));
  wait(40);
  i = ACTOR_STATUE_A;
  do {
    actor = Actor_GetById(i);
    actor->z.fixed = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->update = 0;
  } while (++i <= ACTOR_STATUE_F);
  wait(3);
  Sound_Play(SE_THUMP);
  wait(10);
  if (!GameFlag_Get(FLAG_DULLAHAN_BEATEN) && shift > 0)
    AnemosInnerSanctumEntrance_ShadowChallenges();
}

/// Unused: does nothing.
void AnemosInnerSanctumEntrance_DoNothing(void) {}

extern struct Trigger gAnemosInnerSanctumEntrance_Triggers[];
extern struct Trigger gAnemosInnerSanctumEntrance_HallTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_HallTriggers;
  return gAnemosInnerSanctumEntrance_Triggers;
}

/// Whether the game flag is set.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

/// Opens with the window transition. In the entrance room, gives the six
/// statues their glows and sprite priority bits, and with FLAG_SCENE_TEMP_00
/// set leaves them placed and lit. With FLAG_SCENE_TEMP_01 set, back from the
/// Dullahan, runs AnemosInnerSanctumEntrance_GlowRises and clears the flag.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineWork.next == (s32)&SceneId_AnemosInnerSanctumEntrance) {
    AnemosInnerSanctumEntrance_StartLightSprites();
    AnemosInnerSanctumEntrance_SetLightPriority(2);
    // LightSprites_Add, defined below, is called with no prototype in scope.
    LightSprites_Add(ACTOR_STATUE_A, 0);
    LightSprites_Add(ACTOR_STATUE_B, 1);
    LightSprites_Add(ACTOR_STATUE_C, 2);
    LightSprites_Add(ACTOR_STATUE_D, 3);
    LightSprites_Add(ACTOR_STATUE_E, LIGHT_SPRITE_MIRRORED | 0);
    LightSprites_Add(ACTOR_STATUE_F, LIGHT_SPRITE_MIRRORED | 1);
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

void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  tile = gVramCacheEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gLightSpriteOams;
  picture = gAnemosInnerSanctumEntrance_LightPicture *
            ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];

    if (actor != 0 && actor->x.fixed != 0) {
      s32 ox = actor->x.fixed - viewX;
      FIXED y = actor->y.fixed - viewY + toFIXED(8);
      FIXED z = actor->z.fixed - viewZ - viewY;
      s32 oy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 mode = gLightSpritesLarge;
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
        *word++ = oy | (ox << 16) | (frame & LIGHT_SPRITE_MIRRORED) << 21 |
                  0x40002000;
        *word = (tile + picture + (frame & 7) * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
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
        *word = (tile + frame * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

/// Takes `size` bytes from the heap for a picture's decompression.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

/// Sets up the glow sprites from the field's cached tiles D, with no picture
/// row chosen yet.
void AnemosInnerSanctumEntrance_StartLightSprites(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWordsEntry(gLightSpriteActors, 64);
  Decompress_TaggedLz(gField_CachedTilesD, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0x1800, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 0;
  gLightSpritePriority = -1;
  gAnemosInnerSanctumEntrance_LightPicture = 0;
}

void LightSprites_Add(s32 id, s32 value) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    actor->timer = value;
    gLightSpriteActors[gLightSpriteCount] = actor;
    gLightSpriteCount++;
  }
}

/// Sets the glow sprites' priority; -1 takes each actor's own.
void AnemosInnerSanctumEntrance_SetLightPriority(u16 value) {
  gLightSpritePriority = value;
}

/// Chooses the glows' picture: 0 the still glow, 1 the five-frame flicker.
void AnemosInnerSanctumEntrance_SetLightPicture(u16 value) {
  gAnemosInnerSanctumEntrance_LightPicture = value;
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The blue orb's rise: it grows and climbs for 40 frames.
s32 gAnemosInnerSanctumEntrance_GlowGrowsScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 64),
    SC_ADD(SW_Y, -toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

/// The blue orb shrinking back as it sinks, then gone to the origin.
s32 gAnemosInnerSanctumEntrance_GlowShrinksScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_ADD(SW_Y, toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

/// The shadow shrinking away for 40 frames, then gone to the origin.
s32 gAnemosInnerSanctumEntrance_ShadowScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

/// The cells, x then z, where the six statues must stand before Hover opens the
/// way.
s8 gAnemosInnerSanctumEntrance_StatueSpots[] = {11, 27, 11, 28, 12, 27,
                                                12, 28, 13, 27, 13, 28};

/// The order the statues hop up in when the way opens.
s8 gAnemosInnerSanctumEntrance_StatueOrder[] = {
    ACTOR_STATUE_D, ACTOR_STATUE_F, ACTOR_STATUE_A,
    ACTOR_STATUE_C, ACTOR_STATUE_E, ACTOR_STATUE_B,
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

/// The coiled dragon statues' script, from ACTOR_FIRST_DRAGON_STATUE on: hides
/// each one's sprite, then stops.
LATE_DATA s32 gAnemosInnerSanctumEntrance_DragonStatueScript[] = {
    SC_CALL(AnemosInnerSanctumEntrance_ClearScriptedActorSpriteFlags),
    SC_END,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(AnemosInnerSanctumEntrance),
    SCENE_EXIT_UNLESS(1, ContigoGeoglyph, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumDragonSkinChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ContigoGeoglyph, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ContigoGeoglyph, 24, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctumOrihalconChest, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The scene's other room's actors: none.
LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

/// The entrance room's actors, in AnemosInnerSanctumEntranceActorId's order.
LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_HallPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DULLAHAN, FLAG_DULLAHAN_BEATEN, MOVEMENT_TYPE_STAND,
                    200, 0, 104, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
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
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 152, 0,
                    168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 152, 0,
                    216, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 248, 0,
                    168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 248, 0,
                    216, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 696, 0,
                    240, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 744, 0,
                    240, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    232, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    288, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    232, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    768, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    712, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    768, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The scene's other room's triggers: none.
LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The entrance room's exits, its pushing cells, Move on each statue and Hover
/// on the statues' cell.
LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_HallTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_WALKING, 33, CONDITION_ALWAYS,
     EVENT(AnemosInnerSanctumEntrance_StepWhilePushing)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER,
     CONDITION_ALWAYS, EVENT(AnemosInnerSanctumEntrance_StepWhilePushing)},
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
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER,
     IF_FLAG_SET(FLAG_SCENE_TEMP_00),
     EVENT(AnemosInnerSanctumEntrance_OpenTheWay)},
    {TRIGGER_TABLE_END},
};
