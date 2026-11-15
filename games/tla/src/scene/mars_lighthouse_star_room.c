/// @file
/// The Mars Lighthouse's dragon hall, high in the frozen lighthouse, where two
/// Flame Dragons sleep in a block of ice before a statue with a face.
///
/// Blaze on the dragon statue sends two fireballs round the hall to the ice,
/// which cracks; Burst on the crack frees the dragons, which wake and fight.
/// Beaten, they are Agatio and Karst, too weak and cold to go on: they ask
/// Felix to light the beacon and leave him the Mars Star. Taken up into the
/// Mythril Bag and set in the statue's face, the star turns the lighthouse
/// back to fire and its ice is gone, here and in every room below.

#include "system/task.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "flags.h"
#include "scene/ride.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "scene/mars_lighthouse_star_room.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "version.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "field/field_sequence_event.h"
#include "system/io_reg.h"
#include "battle/battle_unit.h"
#include "field/call.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "window/item_icon.h"
#include "field/map_grid.h"
#include "macros.h"
#include "system/input.h"
#include "system/field_effect.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "field/sprite.h"

/// A sprite with no tiles of its own: the code loads an item's icon into it,
/// or the frame of another sprite.
#define BLANK_SPRITE 489
#define ICE_CHIP_SPRITE 325
#define SPARK_SPRITE 285

#define FLAG_MARS_STAR_TAKEN 0xa3a ///< Felix took it from where Agatio left it
#define FLAG_ADEPTS_WOKE 0xa3b     ///< Agatio and Karst woke after the battle
#define FLAG_DRAGON_ICE_CRACKED 0xa4a
#define FLAG_FLAME_DRAGONS_BEATEN 0xa4b
/// From patch 1: the Mars Star was set in the statue once, and the lighthouse
/// thawed; set again, it only rises and falls back.
#define FLAG_MARS_STAR_SET 0xa9e
/// Both fireballs have reached the ice: Burst can break it.
#define FLAG_DRAGON_ICE_BURSTABLE FLAG_SCENE_TEMP_33
/// From patch 1: the ice is burst, and fireballs still on their way no longer
/// crack it.
#define FLAG_DRAGON_ICE_BURST FLAG_SCENE_TEMP_38

/// Where the battle with the Flame Dragons returns the party.
#define ENTRANCE_AFTER_BATTLE 20
/// The way in from the cracked floor above, landing below the crack.
#define ENTRANCE_FROM_CRACK 11

/// A flame thrower's flags (the flame thrower tables): the flame's reach in the
/// low four bits, and which flame it sends, or none.
enum FlameThrowerFlags {
  FLAME_FORWARD = 0x00,          ///< Ride_SpawnForward
  FLAME_LANDING_FORWARD = 0x10,  ///< Ride_SpawnLandingForward
  FLAME_LANDING_BACKWARD = 0x20, ///< Ride_SpawnLandingBackward
  FLAME_BACKWARD = 0x30,         ///< Ride_SpawnBackward
  FLAME_THROWER_DRY = 0x40,      ///< nothing comes out of it
  FLAME_SPARK = 0x80             ///< Ride_SpawnSpark
};

enum {
  /// The dragon statue before the statue with the face, in two halves; Blaze
  /// is cast at the east half and hits the west one.
  ACTOR_EAST_DRAGON_STATUE = 8,
  ACTOR_WEST_DRAGON_STATUE = 9,
  /// The crack the fireballs leave in the ice the dragons sleep in.
  ACTOR_ICE_CRACK = 10,
  ACTOR_AGATIO_DRAGON = 11, ///< the Flame Dragon that was Agatio
  ACTOR_KARST_DRAGON = 12,  ///< the Flame Dragon that was Karst
  ACTOR_AGATIO = 13,
  ACTOR_KARST = 14,
  /// The lights that rise from the dragons as they turn back into Adepts.
  ACTOR_AGATIO_LIGHT = 15,
  ACTOR_KARST_LIGHT = 16,
  ACTOR_EAST_FIREBALL = 17,
  ACTOR_WEST_FIREBALL = 18,
  /// What a fireball would launch ahead of it; this room launches nothing.
  ACTOR_EAST_FIREBALL_ROLL = 19,
  ACTOR_WEST_FIREBALL_ROLL = 20
};

/// The ride work seen from one flame thrower's entry: the work's header, then
/// the entry, as the getters below index it.
struct RideRow {
  u8 beforeEntries[20];
  struct RideEntry entry;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

/// The dragon statue's halves start with their sprite flags clear.
s32 MarsLighthouseStarRoom_SetUpStatue(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

// XXX: what Blaze does otherwise once the field work's unkd38 is set (its
// mode 3 for 2 when it finds no torch); the rest of blaze.c's reading of the
// mode would settle it.
/// Blaze is cast at the dragon statue.
void MarsLighthouseStarRoom_BeginBlazeOnStatue(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->unkd38 = 1;
}

/// Blaze reaches the statue: it shoots its two fireballs.
void MarsLighthouseStarRoom_BlazeStatue(void) {
  MarsLighthouseStarRoom_ShootFireballs();
}

/// Blaze ends; the fireballs go on alone.
void MarsLighthouseStarRoom_EndBlazeOnStatue(void) {}

#ifndef TLA_MARS_LIGHTHOUSE_STAR_ROOM_H
#define TLA_MARS_LIGHTHOUSE_STAR_ROOM_H

#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/input.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Trig_Cos(s32 angle);
void Actor_SetupScriptedPart(struct Actor *object, s32 priority);

static __inline__ struct Actor *ObjectCreate_(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "tla_edition.h"

void Actor_StartScript(struct Actor *object, const void *script);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
extern s16 gSlideOnIce_SteerAngles[];

#endif

extern u32 gMarsLighthouseStarRoom_ScatterIceChipsScript[];

/// Six chips of ice burst from an actor and bounce away.
void MarsLighthouseStarRoom_ScatterIceChips(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x.fixed + (Random16() << 4) - (Random16() << 4);
    struct Actor *object = ObjectCreate_(
        ICE_CHIP_SPRITE, x, (Random16() << 3) + source->y.fixed + px(4),
        source->z.fixed + (Random16() << 4) - (Random16() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16();
      radius = Random16();
      scale = ((u32)Random16() >> 1) + 0x4000;
      Actor_StartScript(object, gMarsLighthouseStarRoom_ScatterIceChipsScript);
      Actor_SetSpriteFlags(object, 0);
      /* FAKEMATCH(dead): an empty once-only block holds the order around it. */
      do {
      } while (0);
      object->velocityY = angle * 4 + px(2);
      object->velocityZ = Iwram_MulQ16(radius * 8, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = px(33) / 100;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = Q16_ONE / 2;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

/// The west fireball reaches the ice: a small crack shows, unless the east one
/// came first.
void MarsLighthouseStarRoom_WestFireballCracksIce(void) {
  struct Actor *object = Actor_Get(ACTOR_ICE_CRACK);

#if !TLA_PATCH_1
  if (Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 ||
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#else
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 &&
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#endif
    Event_PlaySound(SE_ICE_CRACK);
    object->x.fixed = object->homeX << 16;
    object->z.fixed = object->homeZ << 16;
    if (Event_GetFlag(FLAG_DRAGON_ICE_CRACKED) == 0) {
      object->scaleX = Q16_ONE * 3 / 5;
      object->scaleY = Q16_ONE * 3 / 5;
    }
  }
}

/// The east fireball reaches the ice: the whole crack shows, and Burst can
/// break it.
void MarsLighthouseStarRoom_EastFireballCracksIce(void) {
  struct Actor *object = Actor_Get(ACTOR_ICE_CRACK);

#if !TLA_PATCH_1
  if (Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 ||
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#else
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 &&
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#endif
    Event_PlaySound(SE_ICE_CRACK);
    Event_SetFlag(FLAG_DRAGON_ICE_BURSTABLE);
    object->x.fixed = object->homeX << 16;
    object->z.fixed = object->homeZ << 16;
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
    Event_SetFlag(FLAG_DRAGON_ICE_CRACKED);
  }
}

void MarsLighthouseStarRoom_ClearDragonIce(void);

/// Burst on the crack: a flash, the ice gone, chips flying, and the dragons
/// wake.
void MarsLighthouseStarRoom_BurstIce(s32 phase, s32 frame) {
  if (phase == 1) {
    Event_PlaySound(SE_574);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_Interpolate(60);
    MarsLighthouseStarRoom_ClearDragonIce();
    Actor_SetPosition(ACTOR_ICE_CRACK, 0, 0);
#if TLA_PATCH_1
    Event_SetFlag(FLAG_DRAGON_ICE_BURST);
#endif
    gFieldWork->raisedTrigger = 20;
  }
  if ((u32)(frame - 97) <= 174 && frame % 30 == 0)
    MarsLighthouseStarRoom_ScatterIceChips(ACTOR_ICE_CRACK);
  if (frame == 276) {
    PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
    PaletteFade_Interpolate(20);
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

#include "shared/actor_freeze.h"

void MarsLighthouseStarRoom_WakeAfterBattle(void);
void MarsLighthouseStarRoom_CreateMarsStar(s32 a);
void MarsLighthouseStarRoom_LayDownAdept(s32 actor);
void Ride_Create(void);
void CrackedTile_DropIn(void);
void MapGrid_CenterOnFollowedPoint(void);
void FieldEffect_StartBlankOverlay(s32 a);
extern struct Actor *gMarsLighthouseStarRoom_MarsStar;

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gMarsLighthouseStarRoom_MarsStar = NULL;
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  Ride_Create();
  FlameThrowers_Start(0, 0, 0, 88);
  if (scene == (s32)&SceneId_MarsLighthouseStarRoom) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
      // The ice is gone from the hall.
      MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
    }
    if (entrance == ENTRANCE_FROM_CRACK) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
      struct FieldWork *work = Ram_HeapSlots->fieldWork;
      s32 column = FieldGameFlag_GetByte(0x340);
      s32 row = FieldGameFlag_GetByte(0x348);
      struct Actor *other;
      s32 x;

      x = FIELD_CELL_CENTER(column);
      leader->x.fixed = x;
      leader->z.fixed = (row << 20) - px(296);
      other = work->camera;
      other->x.fixed = x;
      other->z.fixed = leader->z.fixed;
      MapGrid_CenterOnFollowedPoint();
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
    if (entrance == 9 || entrance == 10) {
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
    if (entrance == 1 || entrance == ENTRANCE_AFTER_BATTLE) {
      if (!Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
        FieldActor_SetSpritePriority(ACTOR_AGATIO_DRAGON, 3);
        FieldActor_SetSpritePriority(ACTOR_KARST_DRAGON, 3);
        Actor_GetById(ACTOR_KARST_DRAGON)->scaleX = -Q16_ONE;
      }
      if (Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN)) {
        MarsLighthouseStarRoom_ClearDragonIce();
        Actor_SetPosition(ACTOR_ICE_CRACK, 0, 0);
      } else if (Event_GetFlag(FLAG_DRAGON_ICE_CRACKED)) {
        Event_SetFlag(FLAG_DRAGON_ICE_BURSTABLE);
      } else {
        Actor_SetPosition(ACTOR_ICE_CRACK, 0, 0);
      }
      if (Event_GetFlag(FLAG_ADEPTS_WOKE)) {
        if (!Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
          Actor_SetPosition(ACTOR_AGATIO_DRAGON, 0, 0);
          Actor_SetPosition(ACTOR_KARST_DRAGON, 0, 0);
        }
        MarsLighthouseStarRoom_ClearDragonIce();
        System_Wait(10);
        if (!Event_GetFlag(FLAG_MARS_STAR_TAKEN)) {
          MarsLighthouseStarRoom_CreateMarsStar(0);
          if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
            MarsLighthouseStarRoom_LayDownAdept(ACTOR_AGATIO);
            MarsLighthouseStarRoom_LayDownAdept(ACTOR_KARST);
          }
        }
        if (Actor_GetById(ACTOR_AGATIO)->x.fixed > px(400)) {
          MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, 13);
          MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
        }
      } else if (entrance == ENTRANCE_AFTER_BATTLE) {
        MarsLighthouseStarRoom_WakeAfterBattle();
      }
    }
  }
  return 0;
}

void Map_CopyPalette(s32 scene, s32 a1);

s32 Scene_PrepareMap(void) {
  if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
    Map_CopyPalette(gMainRoutineWork.mapId +
                        (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                    0x5000000);
  if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE)
    Event_SetFlag(FLAG_FLAME_DRAGONS_BEATEN);
  return 0;
}

void Event_Start(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);

/// Free of the ice, the Flame Dragons rise and roar, and the battle begins.
void MarsLighthouseStarRoom_WakeFlameDragons(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MarsLighthouseStarRoom_ClearDragonIce();
  Event_PlaySound(MUS_SILENCE);
  FieldEvent_Wait(40);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, 8);
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 4, 10);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, 16);
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 20);
  Event_PlaySound(MUS_FLAME_DRAGONS);
  Actor_SetAnimation(ACTOR_AGATIO_DRAGON, 2);
  Actor_SetAnimation(ACTOR_KARST_DRAGON, 2);
  FieldEvent_Wait(24);
  Actor_SetPosition(ACTOR_AGATIO_DRAGON, 696, 232);
  Actor_SetPosition(ACTOR_KARST_DRAGON, 664, 232);
  Actor_SetAnimation(ACTOR_AGATIO_DRAGON, 3);
  Actor_SetAnimation(ACTOR_KARST_DRAGON, 3);
  Event_PlaySound(SE_TREMOR);
  MapGrid_SetShake(2, 4, 1);
  FieldEvent_Wait(20);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_BOSS_ROAR);
  Actor_SetAnimation(ACTOR_AGATIO_DRAGON, 4);
  Actor_SetAnimation(ACTOR_KARST_DRAGON, 4);
  FieldEvent_Wait(60);
  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_MarsLighthouseStarRoom,
                                       ENTRANCE_AFTER_BATTLE);
  gMainRoutineWork.battleBackground = SCENE_GONDOWAN_CLIFFS_HIDEOUT;
  Field_RequestBattleFromEncounterTable(102, 0);
}

void Event_End();
void Actor_MoveToObject();
void Scene_PlayMusic();
extern u8 MsgMarsLighthouseStarRoomWhyWhyAmILying[];
extern s32 gMarsLighthouseStarRoom_LightRiseScript[];
extern s32 gMarsLighthouseStarRoom_LightSinkScript[];
extern s32 gMarsLighthouseStarRoom_AgatioDragonShrinkScript[];
extern s32 gMarsLighthouseStarRoom_KarstDragonShrinkScript[];

/// After the battle the dragons shrink into two lights that sink to the floor,
/// and Agatio and Karst lie there: lost in the frozen lighthouse, its eye took
/// their will, and they ask Felix to light the beacon with the Mars Star.
void MarsLighthouseStarRoom_WakeAfterBattle(void) {
  s32 answer;
  struct Actor *dragon, *agatioLight, *karstLight;

  dragon = Actor_Get(ACTOR_AGATIO_DRAGON);
  agatioLight = Actor_Get(ACTOR_AGATIO_LIGHT);
  karstLight = Actor_Get(ACTOR_KARST_LIGHT);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MarsLighthouseStarRoom_ClearDragonIce();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_AGATIO), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KARST), 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO_LIGHT), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST_LIGHT), SPRITE_COLOR_WHITE);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(dragon, dragon->x.fixed, 0,
                                       dragon->z.fixed - px(8));
  agatioLight->motionFlags = 0;
  agatioLight->scaleX = Q16_ONE / 4;
  agatioLight->scaleY = Q16_ONE / 4;
  karstLight->motionFlags = 0;
  karstLight->scaleX = Q16_ONE / 4;
  karstLight->scaleY = Q16_ONE / 4;
  FieldCamera_MoveTo(680, CAMERA_KEEP, 228, 0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 680, 264);
  System_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO_DRAGON), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST_DRAGON), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST), SPRITE_COLOR_WHITE);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_WARP);
  Actor_MoveToObject(ACTOR_AGATIO_LIGHT, ACTOR_AGATIO_DRAGON);
  Actor_MoveToObject(ACTOR_KARST_LIGHT, ACTOR_KARST_DRAGON);
  System_Wait(1);
  Actor_SetScript(ACTOR_AGATIO_DRAGON,
                  (s32)&gMarsLighthouseStarRoom_AgatioDragonShrinkScript);
  Actor_SetScript(ACTOR_KARST_DRAGON,
                  (s32)&gMarsLighthouseStarRoom_KarstDragonShrinkScript);
  Actor_SetScript(ACTOR_AGATIO_LIGHT,
                  (s32)&gMarsLighthouseStarRoom_LightRiseScript);
  Actor_RunScriptAndWait(ACTOR_KARST_LIGHT,
                         (s32)&gMarsLighthouseStarRoom_LightRiseScript);
  MarsLighthouseStarRoom_LayDownAdept(ACTOR_AGATIO);
  MarsLighthouseStarRoom_LayDownAdept(ACTOR_KARST);
  Actor_SetScript(ACTOR_AGATIO_LIGHT,
                  (s32)&gMarsLighthouseStarRoom_LightSinkScript);
  Actor_RunScriptAndWait(ACTOR_KARST_LIGHT,
                         (s32)&gMarsLighthouseStarRoom_LightSinkScript);
  FieldEvent_Wait(10);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO), SPRITE_COLOR_NORMAL);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST), SPRITE_COLOR_NORMAL);
  FieldEvent_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_SetAnimation(ACTOR_AGATIO, 5);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)&MsgMarsLighthouseStarRoomWhyWhyAmILying);
  // "Why... Why am I lying here?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 80);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 1);
  FieldEvent_Wait(20);
  Event_PlaySound(MUS_KARST_AND_AGATIO);
  // "Wait, I think I remember... We were going to light the beacon..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_KARST, 1);
  Actor_SetAnimation(ACTOR_KARST, 5);
  FieldEvent_Wait(20);
  // "But the lighthouse was frozen. We wandered lost inside, trying to..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 20);
  // "We must hurry... If we delay, Prox is destroyed."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_SetAnimation(ACTOR_AGATIO, 6);
  // "It's no use... I'm absolutely drained... I can't even stand."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_KARST, 6);
  // "And the cold... Why am I so cold? We of the Fire Clan are meant to..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 20);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I feel the cold too, Karst... I... I'm afraid we're going to die up..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "I can feel it now... The long, dark sleep creeping into my soul..."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "Wait... I think I remember..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 20);
  // "The eye... It told us that we lack the will to go any farther."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_KARST, 7);
  // "The eye... I remember now... I felt its gaze pierce me to the core..."
  Event_ShowMessage(ACTOR_KARST, 0);
  // "That's right... And I remember fighting against Felix..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  // "I do, too... Was it a dream? An illusion?"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 40);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Felix? Is that you?"
  Event_OpenMessage(ACTOR_AGATIO, 0);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  if (answer == 0) {
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    Actor_SetAnimation(ACTOR_AGATIO, 7);
    Event_ShowMessage(ACTOR_AGATIO, 0);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
    Actor_SetAnimation(ACTOR_AGATIO, 6);
    Event_ShowMessage(ACTOR_AGATIO, 0);
  }
  FieldEvent_Wait(40);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KARST, 2);
  Event_ShowMessage(ACTOR_KARST, 0);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  MarsLighthouseStarRoom_CreateMarsStar(1);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_SetAnimationAndWait(ACTOR_AGATIO, 7);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_AGATIO, 10);
  Actor_SetAnimation(ACTOR_KARST, 9);
  Event_SetFlag(FLAG_ADEPTS_WOKE);
  Scene_PlayMusic();
  Event_End();
}

struct SpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
  u8 unk00[9];
  struct SpriteBits bits;
};

void MarsLighthouseStarRoom_KeepStarAtLeaderPriority(struct Actor *object) {
  struct Actor *owner = Actor_Get(gMainRoutineWork.leaderActorId);
  struct SpriteAttributes *to = (struct SpriteAttributes *)object->sprite;
  struct SpriteAttributes *from = (struct SpriteAttributes *)owner->sprite;
  struct SpriteBits source = from->bits;
  struct SpriteBits bits;

  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}

extern struct Actor *gMarsLighthouseStarRoom_MarsStar;

/// The Mars Star as an actor on the floor by Agatio, rising first when asked.
void MarsLighthouseStarRoom_CreateMarsStar(s32 rise) {
  struct Actor **slot = &gMarsLighthouseStarRoom_MarsStar;
  u8 *block;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 zero;
  s32 size;

  zero = 0;
  object = FieldActor_Create(BLANK_SPRITE, px(702), 0, px(224));
  *slot = object;
  if (object == NULL)
    return;
  sprite = object->sprite;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  object->retainSpriteCache = 1;
  object->update = MarsLighthouseStarRoom_KeepStarAtLeaderPriority;
  size = 0x608;
  sprite->nextFrame = zero;
  sprite->unk1b[0] = zero;
  block = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, size);
  Item_LoadIcon(ITEM_MARS_STAR);
  block += 0x400;
  Vram_Load(sprite->vramBlock, 128, block);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  if (rise) {
    System_Wait(1);
    (*slot)->velocityY = px(5) / 2;
    System_Wait(4);
    Event_PlaySound(SE_CLICK);
    System_Wait(8);
    Event_PlaySound(SE_CLICK);
  }
}

#include "text/msg_ids.h"

void Event_Start(void);
void Event_End(void);

/// Karst: "Your hand...so warm..."
void MarsLighthouseStarRoom_HoldWarmHand(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_KARST, 5);
  FieldEvent_SetMessageId((s32)MsgMarsLighthouseStarRoomYourHandSoWarmPeople);
  // "Your hand...so warm... People have such warm hands... I had forgotten..."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_SetAnimation(ACTOR_KARST, 9);
  Event_End();
}

s32 Inventory_AddItem(s32 owner, s32 item);
struct Unit *Unit_Get(u32 unitId);
void Inventory_Equip(s32 owner, s32 slot);
void Actor_Release(struct Actor *object);
void FieldCamera_WaitForArrival(void);

/// Agatio bids Felix hurry; the first time, the Mars Star flies to Felix, goes
/// into the Mythril Bag, and the statue's face asks for it.
void MarsLighthouseStarRoom_TakeMarsStar(void) {
  struct Actor *leader;
  struct Actor *object;
  s32 i;
  s32 offset;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_AGATIO, 5);
  Event_SetMessageId((s32)MsgMarsLighthouseStarRoomItsTooLateForUs);
  // "It's too late for us now... Hurry, light the beacon."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_AGATIO, 10);
  if (!Event_GetFlag(FLAG_MARS_STAR_TAKEN)) {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
    object = gMarsLighthouseStarRoom_MarsStar;
    object->motionFlags = 0;
    object->speedLimit = px(4);
    object->acceleration = px(2);
    FieldActor_SetMoveTargetFixed(object, leader->x.fixed,
                                  leader->y.fixed + px(32), leader->z.fixed);
    FieldActor_WaitForMove(gMarsLighthouseStarRoom_MarsStar);
    Event_PlaySound(MUS_TREASURE_FOUND);
    PartyInventory_Remove(ITEM_MYTHRIL_BAG);
    Inventory_AddItem(4, ITEM_MYTHRIL_BAG_MARS_STAR);
    // "Felix got the Mars Star!"
    Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixGotMarsStar, 1);
    i = 0;
    offset = 216;
    for (; i <= 14; offset += 2, i++) {
      if ((*(u16 *)((u8 *)Unit_Get(4) + offset) & 0x1ff) ==
          ITEM_MYTHRIL_BAG_MARS_STAR) {
        Inventory_Equip(4, i);
        break;
      }
    }
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
    Event_SetFlag(FLAG_MARS_STAR_TAKEN);
    FieldActor_SetPositionAndResetMotion(gMarsLighthouseStarRoom_MarsStar, 0, 0,
                                         0);
    System_Wait(1);
    Actor_Release(gMarsLighthouseStarRoom_MarsStar);
    System_Wait(1);
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(680, CAMERA_KEEP, 146, 1);
    FieldCamera_WaitForArrival();
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
    PaletteFade_Interpolate(20);
    System_Wait(20);
    {
      // A halfword in its own local, so the address takes the register first.
      struct {
        u16 value;
      } white;

      white.value = 0x7fff;
      OBJ_PLTT_COLOR(0, 15) = white.value;
    }
    System_Wait(1);
    FieldMessage_ShowCentered((s32)MsgMarsLighthouseStarRoomYouWhoHoldStarOpen,
                              0, 0);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_Interpolate(20);
    System_Wait(20);
  }
  Event_End();
}

/// The block of ice the dragons slept in is gone from the floor.
void MarsLighthouseStarRoom_ClearDragonIce(void) {
  MapGrid_CopyCellsTo(46, 68, 40, 73, 5, 6);
  MapGrid_CopyCellAttributes(104, 9, 5, 6, 40, 9);
}

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

/// Agatio or Karst lies on the floor where the dragon fell.
void MarsLighthouseStarRoom_LayDownAdept(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSpriteFlags(Actor_Get(id), 0);
  FieldActor_SetSpritePriority(id, 3);
  object->motionFlags = 0;
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->collisionFlags = 0;
  if (id == ACTOR_AGATIO) {
    Actor_SetPosition(ACTOR_AGATIO, 700, 224);
    Actor_SetAnimation(ACTOR_AGATIO, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, id);
  } else {
    Actor_SetPosition(ACTOR_KARST, 660, 216);
    Actor_SetAnimation(ACTOR_KARST, 9);
    MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
  }
}

/// "There's a space for you to place something." Then the field menu opens,
/// for the party to choose what to place.
void MarsLighthouseStarRoom_ExamineStatue(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "There's a space for you to place something."
  Message_ShowCentered((s32)MsgMarsLighthouseStarRoomTheresSpace, 1);
  work->fieldMenu = 1;
  Event_End();
}

void Scene_PlayMusic(void);
void MarsLighthouseStarRoom_SpawnThawSparks(void);

#if !TLA_PATCH_1

/// The Mars Star set in the statue's face: the room shakes, sparks rise, the
/// ice melts away and the lighthouse turns to fire; the star falls back out.
void MarsLighthouseStarRoom_UseMarsStar(void) {
  struct Actor **slot;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *buffer;
  u16 *screen;
  s32 zero;

  screen = (u16 *)Ram_HeapSlots->paletteFade;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "Felix used the Mars Star!"
  Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_GEM_SET);
  slot = &gMarsLighthouseStarRoom_MarsStar;
  object = FieldActor_Create(BLANK_SPRITE, px(680), px(8), px(147));
  *slot = object;
  if (object != 0) {
    sprite = object->sprite;
    sprite->nextFrame = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = 0;
    object->retainSpriteCache = 1;
    sprite->unk1b[0] = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 680, 186);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 40);
  Event_PlaySound(MUS_SILENCE);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_RED, PALETTES_ALL);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(MarsLighthouseStarRoom_SpawnThawSparks, TASK_PASS_VBLANK);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove(MarsLighthouseStarRoom_SpawnThawSparks);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Map_CopyPalette(gMainRoutineWork.mapId +
                      (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                  screen);
  zero = 0;
  *screen = zero;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 186, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    object->velocityY = px(2);
    object->speedLimit = px(2);
    object->acceleration = px(1);
    FieldActor_SetMoveTarget(object, 672, 0, 160);
    FieldActor_WaitForMove((s32)*slot);
    Event_PlaySound(SE_CLICK);
    object = *slot;
    object->acceleration = px(1) / 2;
    object->velocityY = px(4);
    object->speedLimit = px(1);
    FieldActor_SetMoveTarget(object, 664, 0, 158);
    FieldActor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 670, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  object = *slot;
  if (object != 0)
    Actor_Release(object);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 680, 186);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Scene_PlayMusic();
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_THAWED);
  gMainRoutineWork.battleBackground = SCENE_NARIBWE;
  Event_End();
}

#else

/// The Mars Star set in the statue's face: the room shakes, sparks rise, the
/// ice melts away and the lighthouse turns to fire; the star falls back out.
void MarsLighthouseStarRoom_UseMarsStar(void) {
  s32 flag;
  struct Actor **slot, **placedStarSlot;
  struct Actor *object, *placedStar;
  struct FieldSprite *sprite, *placedStarSprite;
  u8 *buffer, *placedStarGlyphs;
  u16 *screen;

  flag = Event_GetFlag(FLAG_MARS_STAR_SET);
  if (flag == 0) {
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    // "Felix used the Mars Star!"
    Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    System_Wait(1);
    Event_PlaySound(SE_GEM_SET);
    slot = &gMarsLighthouseStarRoom_MarsStar;
    object = (struct Actor *)FieldActor_Create(BLANK_SPRITE, px(680), px(8),
                                               px(147));
    *slot = object;
    if (object != 0) {
      sprite = object->sprite;
      sprite->nextFrame = 0;
      sprite->fullColor = 0;
      sprite->palette = 0;
      sprite->priority = 2;
      object->motionFlags = 0;
      object->retainSpriteCache = 1;
      sprite->unk1b[0] = 0;
      object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
      Item_LoadIcon(ITEM_MARS_STAR);
      Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
      HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    }
    FieldEvent_Wait(20);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 680, 186);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 80);
    Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    object = *slot;
    if (object != 0) {
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      object->velocityY = px(2);
      object->speedLimit = px(2);
      object->acceleration = px(1);
      FieldActor_SetMoveTarget(object, 672, 0, 160);
      FieldActor_WaitForMove((s32)*slot);
      Event_PlaySound(SE_CLICK);
      object = *slot;
      object->acceleration = px(1) / 2;
      object->velocityY = px(4);
      object->speedLimit = px(1);
      FieldActor_SetMoveTarget(object, 664, 0, 158);
      FieldActor_WaitForMove((s32)*slot);
    }
    FieldEvent_Wait(20);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
        ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 670, 170);
    FieldEvent_Wait(1);
    Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags |=
        ACTOR_FACES_MOTION;
    object = *slot;
    if (object != 0)
      Actor_Release(object);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
    Scene_PlayMusic();
    Event_End();
    goto L_020093b0;
  }
  screen = (u16 *)Ram_HeapSlots->paletteFade;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "Felix used the Mars Star!"
  Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_GEM_SET);
  placedStarSlot = &gMarsLighthouseStarRoom_MarsStar;
  placedStar = FieldActor_Create(BLANK_SPRITE, px(680), px(8), px(147));
  *placedStarSlot = placedStar;
  if (placedStar != 0) {
    placedStarSprite = placedStar->sprite;
    placedStarSprite->nextFrame = 0;
    placedStarSprite->unk1b[0] = 0;
    placedStarSprite->fullColor = 0;
    placedStarSprite->palette = 0;
    placedStarSprite->priority = 2;
    placedStar->motionFlags = 0;
    placedStar->retainSpriteCache = 1;
    placedStar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    placedStarGlyphs = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(placedStarSprite->vramBlock, 128, placedStarGlyphs + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 680, 186);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 40);
  Event_PlaySound(MUS_SILENCE);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_RED, PALETTES_ALL);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(MarsLighthouseStarRoom_SpawnThawSparks, TASK_PASS_VBLANK);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove(MarsLighthouseStarRoom_SpawnThawSparks);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Map_CopyPalette(gMainRoutineWork.mapId +
                      (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                  screen);
  *screen = 0;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 186, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  placedStar = *placedStarSlot;
  if (placedStar != 0) {
    placedStar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    placedStar->velocityY = px(2);
    placedStar->speedLimit = px(2);
    placedStar->acceleration = px(1);
    FieldActor_SetMoveTarget(placedStar, 672, 0, 160);
    FieldActor_WaitForMove((s32)*placedStarSlot);
    Event_PlaySound(SE_CLICK);
    placedStar = *placedStarSlot;
    placedStar->acceleration = px(1) / 2;
    placedStar->velocityY = px(4);
    placedStar->speedLimit = px(1);
    FieldActor_SetMoveTarget(placedStar, 664, 0, 158);
    FieldActor_WaitForMove((s32)*placedStarSlot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 670, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  placedStar = *placedStarSlot;
  if (placedStar != 0)
    Actor_Release(placedStar);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Scene_PlayMusic();
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_THAWED);
  Event_SetFlag(FLAG_MARS_STAR_SET);
  gMainRoutineWork.battleBackground = SCENE_NARIBWE;
  Event_End();
L_020093b0:
  return;
}

#endif

/// A rising spark as its update reads the actor: its frames, kept where an
/// actor keeps its watched actor.
struct RiseTimer {
  u8 beforeFrames[0x68];
  s32 frames;
};

/// A spark rises and grows for a second, then is gone.
void MarsLighthouseStarRoom_UpdateThawSpark(struct Actor *object) {
  struct RiseTimer *timer = (struct RiseTimer *)object;

  s32 growth;

  object->y.fixed += px(1) / 2;
  growth = 0x51e;
  object->scaleX += growth;
  object->scaleY += growth;
  timer->frames--;
  if (timer->frames == 1)
    FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
  else if (timer->frames == 0)
    Actor_Release(object);
}

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
void Actor_SetBlendMode(struct Actor *object, u32 value);

/// While the lighthouse thaws, sparks rise from the floor every other frame.
void MarsLighthouseStarRoom_SpawnThawSparks(void) {
  u32 frame = gFrameCount;
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 x, r;
  s32 z;
  s32 column;

  if (frame & 1)
    return;
  if ((frame >> 1) & 1) {
    r = Random16();
    x = r << 3;
    x -= r;
    z = Random16() * 15;
    x <<= 2;
    x >>= 16;
    column = 155;
  } else {
    r = Random16();
    x = r << 3;
    x -= r;
    z = Random16() * 15;
    x <<= 2;
    x >>= 16;
    column = 178;
  }
  column <<= 18;
  z <<= 3;
  x <<= 16;
  x += column;
  z = (u32)z >> 16;
  z <<= 16;
  z += px(460);
  object = ObjectCreate_(SPARK_SPRITE, x, px(32), z);
  if (object != NULL) {
    /* FAKEMATCH(barrier): an empty asm clobbering r0. */
    asm("" : : : "r0");
    object->retainSpriteCache = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite = object->sprite;
    sprite->priority = 2;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = Q16_ONE / 2;
    object->scaleY = Q16_ONE / 2;
    *(s32 *)&object->watchedActor = 60;
    object->update = MarsLighthouseStarRoom_UpdateThawSpark;
    Actor_SelectAnimation(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

extern s32 gMarsLighthouseStarRoom_WestFireballRoute[];
extern s32 gMarsLighthouseStarRoom_EastFireballRoute[];

/// The statue's two fireballs set off round the hall, west and east.
void MarsLighthouseStarRoom_ShootFireballs(void) {
  Actor_Attach(ACTOR_WEST_DRAGON_STATUE, ACTOR_WEST_FIREBALL,
               ACTOR_WEST_FIREBALL_ROLL, 0,
               gMarsLighthouseStarRoom_WestFireballRoute);
  Actor_Attach(ACTOR_WEST_DRAGON_STATUE, ACTOR_EAST_FIREBALL,
               ACTOR_EAST_FIREBALL_ROLL, 0x8000,
               gMarsLighthouseStarRoom_EastFireballRoute);
}

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

extern u8 gInventorySnapshot[];

#include "shared/ride_update_carried.h"

extern u32 gScriptedPart_Script[];

void Actor_StartScript(struct Actor *object, const void *script);

#include "shared/actor_reset_scripted.h"

void Ride_SpawnForward(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],
                                           position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    ((void (*)())Actor_ResetScripted)(object, 2);
    object->update = Flame_UpdateForward;
    if (flag != 0)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

extern u8 gInventorySnapshot[];

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);

#include "shared/ride_update_landing.h"

void Actor_SetupScriptedPart(struct Actor *object, s32 priority);

#include "shared/actor_setup_scripted_part.h"

#define DEFINE_RIDE_SPAWN_BACKWARD(name, call1)                                \
  void name(s32 *position, s32 speed, s32 flag) {                              \
    struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],  \
                                             position[1], position[2]);        \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->riseBaseY = -speed;                                              \
      Actor_SetupScriptedPart(object, 3);                                      \
      object->update = call1;                                                  \
      if (flag != 0)                                                           \
        object->retainSpriteCache = 2;                                         \
      Actor_SelectAnimation(object, 2);                                        \
    }                                                                          \
  }

DEFINE_RIDE_SPAWN_BACKWARD(Ride_SpawnBackward, Flame_UpdateForward)

void Ride_SpawnLandingForward(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],
                                           position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (flag != 0)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

DEFINE_RIDE_SPAWN_BACKWARD(Ride_SpawnLandingBackward, Flame_UpdateLanding)

s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x.fixed, b->x.fixed) < px(12)) {
    if ((u32)(a->y.fixed - b->y.fixed - 1) <= 0xffffe) {
      if (DIFF(a->z.fixed, b->z.fixed) < px(8))
        return 1;
    }
  }
  return 0;
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

#include "shared/ride_update_drift.h"

extern u32 gScriptedPart_Script[];

#include "shared/actor_setup_scripted.h"

void Actor_SetBlendMode(struct Actor *object, u32 value);
void Ride_UpdateDrift();
/* VIEW(s1b78): routine Actor_SetupScripted as void Actor_SetupScripted(struct
 * Actor *object, s32 priority); the definition has void
 * Actor_SetupScripted(struct Actor *object); not tried: Actor_SetupScripted's
 * declaration holds a FAKEMATCH tag. */
extern void
Actor_SetupScripted__s1b78(struct Actor *object,
                           s32 priority) __asm__("Actor_SetupScripted");

void Ride_SpawnSpark(s32 *position, s32 base, s32 mode) {
  struct Actor *object = ObjectCreate_(SPARK_SPRITE, position[0], position[1],
                                       position[2] - px(2));
  s32 spin;

  if (object != 0) {
    object->bounce = Trig_Cos((Random_Below(0x3000)) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() << 1) + base;
    spin = (u16)Random16() - 0x8000;
    object->timer = spin;
    Actor_SetupScripted__s1b78(object, 2);
    object->update = Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

typedef struct Actor Actor;

/// A flame thrower of the ride work, as FlameThrowers_Puff reads it.
typedef struct Entry {
  s16 active;
  s16 timer; ///< frames to its next puff
  u16 flags; ///< enum FlameThrowerFlags: which flame, or none
  /// Puffs even off screen, and its flames keep their sprites.
  s16 offScreen;
  struct Actor *actor;
  s32 reach; ///< 16.16 pixels, from the flags' low four bits
} Entry;

/// The ride work as FlameThrowers_Puff reads it: a flame reaching its stop
/// sets stopped, which calls back once.
typedef struct State {
  u8 unusedBeforeStopped[6];
  s16 stopped;
  s16 wasStopped;
  u8 unusedAfterWasStopped[4];
  s16 runsDuringPsynergy; ///< Ride_SetMode: not paused by Psynergy
  void (*onStopped)();
  Entry entries[4];
} State;

/// The heap slots as FlameThrowers_Puff reads them: the field work through a
/// union, which keeps its read after the stores before it.
typedef struct HeapSlotsView {
  u8 unusedBeforeMapGrid[32];
  struct MapWork *mapGrid;
  u8 unusedBeforeFieldWork[108 - 36];
  union {
    s32 word;
    void *pointer;
  } fieldWork;
} HeapSlotsView;

/// Each frame, every flame thrower in view sends its flame: forward, landing
/// forward or back, back, or a spark, by its flags, every ninth frame. They
/// pause while a menu is open, or a Psynergy is used unless the ride work runs
/// them through it; reaching a stop calls the stop's callback once.
void FlameThrowers_Puff(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  struct MapWork *map;
  s32 *view;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)RIDE_WORK;
  map = ((HeapSlotsView *)Ram_HeapSlots)->mapGrid;
  entry = state->entries;
  view = &map->viewX;
  cameraX = view[0];
  cameraX &= FIELD_PIXEL_MASK;
  cameraZ = view[1] & FIELD_PIXEL_MASK;
  saved[1] = map->origin[1];
  saved[0] = ((HeapSlotsView *)Ram_HeapSlots)->fieldWork.word;
  if (state->stopped != 0 && state->wasStopped == 0 && state->onStopped != 0)
    state->onStopped();
  v = (u16)state->stopped;
  state->wasStopped = v;
  state->stopped = 0;
  for (i = 3; i >= 0; i--, entry++) {
    struct Actor *actor;
    s32 flag, x, y, offScreen, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (((struct FieldWork *)saved[0])->psynergy != 0)
      flag |= 1;
    mark = &actor->paused;
    *mark = flag;
    if (state->runsDuringPsynergy != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dz = actor->z.fixed;
    dx = actor->x.fixed - cameraX;
    y = actor->y.fixed;
    dz -= cameraZ;
    y = dz - y;
    offScreen = entry->offScreen;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (offScreen == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 0xf0;
      switch (y) {
      case FLAME_SPARK:
        Ride_SpawnSpark((void *)&entry->actor->x, entry->reach, offScreen);
        break;
      case FLAME_BACKWARD:
        Ride_SpawnBackward(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      case FLAME_LANDING_BACKWARD:
        Ride_SpawnLandingBackward(&entry->actor->x.fixed, entry->reach,
                                  offScreen);
        break;
      case FLAME_LANDING_FORWARD:
        Ride_SpawnLandingForward(&entry->actor->x.fixed, entry->reach,
                                 offScreen);
        break;
      case FLAME_FORWARD:
        Ride_SpawnForward(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}

/// The flame's sprite as Ride_Create reads it: its VRAM block, and the bit that
/// keeps the block cached.
struct RideAnimation {
  u8 beforeVramBlock[0x10];
  u8 vramBlock;
  u8 keepEntry : 1;
};

extern u8 gInventorySnapshot[];

void Ride_Create(void) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *object;
  struct RideAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  object->retainSpriteCache = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->vramBlock;
  animation->keepEntry = 1;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(struct Actor *object);

#include "shared/ride_release_handle.h"

void Ride_SetMode(s16 value) { RIDE_WORK->value0e = value; }

/// The ride work as FlameThrowers_Start fills it: the hit trigger and the stop
/// callback, then the flame throwers and their stops.
struct FlameThrowerBeam {
  u8 beforeHitTrigger[10];
  u16 hitTrigger; ///< the step trigger a flame that hits the leader raises
  u8 beforeOnStopped[4];
  s32 onStopped; ///< called once a flame reaches its stop
  struct MarsLighthouseBeamTarget targets[4];
  struct MarsLighthouseBeamStop {
    u16 kind;
    u16 value;
  } stops[4];
};

void FlameThrowers_Start(u16 *targets, u16 *stops, s32 onStopped,
                         s32 hitTrigger) {
  struct FlameThrowerBeam *beam = (struct FlameThrowerBeam *)RIDE_WORK;
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    for (i = 0; i <= 3 && (id = *targets++) > 0; i++) {
      s32 flags = *targets;
      s32 height = flags & 15;
      struct Actor *object;

      flags &= 0xf0;
      object = Actor_Get(id);
      targets++;
      if (object != NULL) {
        Actor_SetSpriteFlags(object, 0);
        object->riseCounter = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = 1;
        target->flags = flags;
        target->object = Actor_Get(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i <= 3; i++) {
        s32 kind = *stops++;

        beam->stops[i].kind = kind;
        if (kind <= 0)
          break;
        beam->stops[i].value = *stops++;
      }
    }
  }
  beam->onStopped = onStopped;
  beam->hitTrigger = hitTrigger;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 control = 0x3f10;
      REG_BLDCNT = control;
    }
  }
  Task_AddFrame(FlameThrowers_Puff, TASK_PASS_VBLANK);
}

s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.a;
}

void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.a = value;
}

s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].c = value;
}

s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.b;
}

#include "shared/ride_set_slot.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
extern u8 gInventorySnapshot[];

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision
#define KNOCK_BACK_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/leader_knock_back.c"
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP

/// Hit by the flames, the leader is knocked back.
void MarsLighthouseStarRoom_KnockBackIfHit(void) {
  if (RIDE_WORK->active)
    Leader_KnockBack();
}

extern u8 gInventorySnapshot[];
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

// XXX: is the car an actor (its stopped halfword sits where an actor's action
// is, its steps in unk70)? The code that stores the car's pointer at the ride
// state's 0x6c would settle it.
/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
s32 Fireball_CarryLeader(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
  struct Actor *leader;
  s32 frame[3]; // Unused: gives the stack frame its 12 bytes.

  if (event->transitionActive == 0) {
    ScreenTransition_Start(event->transition, event->transitionFrames);
    event->transitionActive = 1;
  }
  Event_ClearFlag(FLAG_MENU_OPEN);
  event->psynergy = 0;
  event->fieldMenu = 0;
  event->pauseMenu = 0;
  event->aButton = 0;
  event->encounter = 0;
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  // UB: the fall damage takes no arguments and returns nothing.
  ((s32 (*)())Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ride = RIDE_WORK;
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->timer == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x.fixed,
                                                leader->z.fixed) != 7) {
      leader->x.fixed += ((s32 *)ride->car->unk70)[0];
      leader->z.fixed += ((s32 *)ride->car->unk70)[2];
    } else {
      if (((s32 *)ride->car->unk70)[0] != 0)
        leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + px(8);
      if (((s32 *)ride->car->unk70)[2] != 0)
        leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + px(8);
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = NULL;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/ride_if_active.h"

s32 MarsLighthouseStarRoom_IsSliding(void) { return RIDE_WORK->active; }

/// A sprite as the crumbs copy it from the flame's: its OAM attributes, scale,
/// VRAM block, size and layer count, and its animation record.
struct SpriteView {
  u8 beforeOam[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 afterOam[2];
  s32 scale; ///< as field_sprite.h's FieldSprite has it
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 otherFlags : 7;
  u16 rotation;
  u8 dimensions[2]; ///< width and height
  u8 beforeLayerCount[5];
  u8 layerCount;
  u8 beforeRecord[12];
  void *record;
};

/// The ride work as the crumbs read it: the VRAM block of the flame Ride_Create
/// makes, whose sprite the crumbs share, and that flame.
struct Work {
  u8 beforeVramBlock[12];
  u16 vramBlock;
  u8 beforeFlame[98];
  struct Actor *flame;
};

extern u32 gMarsLighthouseStarRoom_CrumbScript[];

#define WORK ((struct Work *)RIDE_WORK)

/// A crumb breaks from the rolling actor as it shrinks away.
void MarsLighthouseStarRoom_ScatterCrumbs(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->flame == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x.fixed + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z.fixed + d * 16 - e * 16;
    y = c * 8 + actor->y.fixed + px(4);
    object = (struct Actor *)Import_ActorCreate(BLANK_SPRITE, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct SpriteView *sprite;
      struct SpriteView *source;
      s32 velocity;

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gMarsLighthouseStarRoom_CrumbScript);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + px(2);
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      velocity = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = px(33) / 100;
      object->scaleX = scale;
      object->scaleY = scale;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      sprite = object->sprite;
      source = work->flame->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->layerCount = 0;
      sprite->dimensions[0] = source->dimensions[0];
      sprite->dimensions[1] = source->dimensions[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

extern u32 gEmbers_Script[];

#include "shared/embers_spawn.h"

/// An attached actor holds still while an event runs.
void Actor_HoldDuringEvent(struct Actor *object) {
  object->paused = Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Actor_RollAndCrumble(struct Actor *actor) {
  struct Vector3 position;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct FieldWork *fieldWork;
    struct FlameHitWork *other;
    struct Actor *target;

    flag = Event_GetFlag(FLAG_MENU_OPEN);
    if (work->psynergy != 0)
      flag |= 1;
    actor->paused = flag;

    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    FieldVector_AddPolarOffset(8, actor->facing, &position);
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, position.x, position.z) ==
        7) {
      actor->targetZ = TARGET_UNSET;
      actor->targetY = TARGET_UNSET;
      actor->targetX = TARGET_UNSET;
      actor->acceleration = px(1) / 4;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedActor != NULL)
        ((void (*)(struct Actor *))actor->watchedActor)(actor);
    }

    map = Ram_HeapSlots->mapGrid;
    tileX = FIELD_CELL(actor->x.fixed);
    tileZ = FIELD_CELL(actor->z.fixed);
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = FIELD_CELL(map->layers[1].offsetX);
    heightA = FIELD_CELL(map->layers[1].offsetY);
    widthB = FIELD_CELL(map->layers[2].offsetX);
    heightB = FIELD_CELL(map->layers[2].offsetY);

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_ICE_CRACK);
          Embers_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                       FIELD_CELL_CENTER(z), actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_CRUMBLE);
          Embers_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                       FIELD_CELL_CENTER(z), actor->facing);
        }
      }
    }

    if (actor->scaleX < Q16_ONE) {
      actor->scaleX += Q16_ONE / 16;
      actor->scaleY += Q16_ONE / 16;
    }
    FieldActor_SetMoveTargetFixed(actor, position.x, position.y, position.z);

    fieldWork = Ram_HeapSlots->fieldWork;
    other = RIDE_WORK;
    target = Actor_Get(gMainRoutineWork.leaderActorId);
    if (ABS(actor->x.fixed - target->x.fixed) < px(20) &&
        (u32)(actor->y.fixed - target->y.fixed + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z.fixed - target->z.fixed) < px(20)) {
      other->active = 2;
      other->car = actor;
      fieldWork->stepTrigger = other->value0a + 2;
    }
  } else if (state == 1) {
    actor->paused = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= Q16_ONE / 16;
      actor->scaleY -= Q16_ONE / 16;
      MarsLighthouseStarRoom_ScatterCrumbs(actor);
    } else {
      actor->z.fixed = 0;
      actor->y.fixed = 0;
      actor->x.fixed = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
#undef WORK
#undef ABS

/// Sends an actor along a scripted path from another, with what it launches.
void Actor_Attach(s32 sourceId, s32 id, s32 kind, s32 value,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedActor = source;
  object->timer = 0;
  *(s16 *)((u8 *)object + 0x66) = value;
  *(u8 *)((u8 *)object + 0x62) = kind;
  object->update = Actor_HoldDuringEvent;
}

#include "shared/actor_launch_ahead.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

extern u8 gInventorySnapshot[];

#include "shared/actor_park.h"

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

extern volatile s16 gSlideOnIce_Stopped;
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Actor_HasArrived(struct Actor *object);

#include "shared/slide_on_ice_from_edge.c"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef TLA_MARS_LIGHTHOUSE_STAR_ROOM_H
#undef DIFF
#undef DEFINE_RIDE_SPAWN_BACKWARD
#undef ABS
#undef IWRAM
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP
#undef WORK
#undef Position
extern s32 gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "system/io_write.h"
#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetShakeFixed(a, b, c);
}

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  struct CellTarget *work = &gCrackedTiles;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        CrackedTile_Crack(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      CrackedTile_Fall(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 dx = FieldGameFlag_GetByte(gCrackedTiles.flagByte);
  s32 dz = FieldGameFlag_GetByte(gCrackedTiles.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gCrackedTiles.flagByte, 0);
    GameFlag_SetByte(gCrackedTiles.flagByte + 8, 0);
    leader->x.fixed = FIELD_CELL_CENTER(dx + x);
    leader->z.fixed = FIELD_CELL_CENTER(dz + z);
    marker = work->camera;
    marker->x.fixed = leader->x.fixed;
    marker->z.fixed = leader->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldWork->transition = effect;
  }
}

#include "text/msg_ids.h"

void MarsLighthouseStarRoom_ExamineStatue(void);

extern s32 gMarsLighthouseStarRoom_StatueScript[];

// clang-format off
u32 gMarsLighthouseStarRoom_ScatterIceChipsScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gMarsLighthouseStarRoom_WestFireballRoute[] = {
    46,
    (s32)Actor_PopUp,
    4,
    0x2680000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2680000,
    0,
    0xf80000,
    1,
    0,
    2,
    4,
    0x2380000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2500000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)MarsLighthouseStarRoom_WestFireballCracksIce,
    46,
    (s32)Actor_Park,
    17,
};

s32 gMarsLighthouseStarRoom_EastFireballRoute[] = {
    46,
    (s32)Actor_PopUp,
    4,
    0x2a80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2a80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3280000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3000000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)MarsLighthouseStarRoom_EastFireballCracksIce,
    46,
    (s32)Actor_Park,
    17,
};

u32 gScriptedPart_Script[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

u32 gMarsLighthouseStarRoom_CrumbScript[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gEmbers_Script[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gSlideOnIce_SteerAngles[] = {
    -1, 0, -32768, -1, -16384, -8192, -24576, -16384,
    16384, 8192, 24576, 16384, -1, 0, -32768, -1,
    -1, 0, -32768, -1, -16384, -1, -1, -16384,
    16384, -1, -1, 16384, -1, 0, -32768, -1,
};

s16 gSlideOnIce_AnglesByKey[] = {
    -1, 0, -32768, -1, -16384, 0, -32768, -16384,
    16384, 0, -32768, 16384, -1, 0, -32768, -1,
};

u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

s32 gMarsLighthouseStarRoom_StatueScript[] = {
    SC_CALL(MarsLighthouseStarRoom_SetUpStatue),
    SC_END,
};

s32 gMarsLighthouseStarRoom_LightRiseScript[] = {
    SC_ADD(SW_SCALE_X, 0x400),
    SC_ADD(SW_SCALE_Y, 0x400),
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

s32 gMarsLighthouseStarRoom_LightSinkScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_ADD(SW_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gMarsLighthouseStarRoom_AgatioDragonShrinkScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gMarsLighthouseStarRoom_KarstDragonShrinkScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseStarRoom),
    SCENE_EXIT(1, MarsLighthouseStarRoom, 2),
    SCENE_EXIT(2, MarsLighthouseStarRoom, 1),
    SCENE_EXIT(3, MarsLighthouseStarRoom, 4),
    SCENE_EXIT(4, MarsLighthouseStarRoom, 3),
    SCENE_EXIT(5, MarsLighthouseEntranceSolBladeChest, 5),
    SCENE_EXIT(6, MarsLighthouseEntranceValkyrieMailChest, 6),
    SCENE_EXIT(7, MarsLighthouseEntranceSolBladeChest, 7),
    SCENE_EXIT(8, MarsLighthouseEntranceValkyrieMailChest, 8),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS,
                    (s32)gMarsLighthouseStarRoom_StatueScript, 712, 0, 264,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS,
                    (s32)gMarsLighthouseStarRoom_StatueScript, 648, 0, 264,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, 7, 680, 0, 240,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_DRAGON, FLAG_MARS_LIGHTHOUSE_THAWED, 7, 664, 0,
                    216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_DRAGON, FLAG_MARS_LIGHTHOUSE_THAWED, 7, 696, 0,
                    216, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {81, 2, CONDITION_ALWAYS, 2},
    {81, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 90, IF_FLAG_SET(FLAG_FLAME_DRAGONS_BEATEN),
     EVENT(Fireball_CarryLeader)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(MarsLighthouseStarRoom_BeginBlazeOnStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 9,
     CONDITION_ALWAYS, EVENT(MarsLighthouseStarRoom_BlazeStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9,
     CONDITION_ALWAYS, (u32)(MarsLighthouseStarRoom_EndBlazeOnStatue)},
    {TRIGGER_RAISED, 20, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_WakeFlameDragons)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 20,
     IF_FLAG_SET(FLAG_DRAGON_ICE_BURSTABLE),
     EVENT(MarsLighthouseStarRoom_BurstIce)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_HoldWarmHand)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgMarsLighthouseStarRoomImSoColdColderThan)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_TakeMarsStar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgMarsLighthouseStarRoomImSoTiredFog)},
    {TRIGGER_EXAMINE, 21, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_ExamineStatue)},
    {USE_ITEM(ITEM_MYTHRIL_BAG_MARS_STAR), 1045, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_UseMarsStar)},
    {TRIGGER_TABLE_END},
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};
