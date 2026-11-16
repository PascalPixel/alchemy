/// @file
/// The Gondowan Cliffs, west of Alhafra, in two scenes that share this overlay:
/// the cliffs (SceneId_GondowanCliffs) and the pass a rock blocks until Grind
/// breaks it (SceneId_GondowanCliffsPass). Kindle, a Mars Djinni, waits at the
/// end of the cliffs (GondowanCliffs_FightKindle); Frost, Whirlwind, Move and
/// Scoop each open a way (GondowanCliffs_FreezeFirstPuddle,
/// GondowanCliffs_BlowAwayFoliage, GondowanCliffs_LeapStumpIntoWater,
/// GondowanCliffs_ScoopUpGeyser); and if the party heads west before helping
/// Piers recover his Black Orb, Kraden turns them back
/// (GondowanCliffs_TurnBackToHelpPiers).
///
/// The effect spawner and the cliffs' events are in gondowan_cliffs/, which a
/// shared field effect object links between, and the tables are in
/// data/gondowan_cliffs.c, which the overlay links after the field's tiles.

#include "system/task.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/actor_script.h"
#include "flags.h"
#include "djinn_ids.h"
#include "field/map_grid.h"
#include "field/field_sequence_event.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "field/call.h"
#include "graphics/vram_tab.h"
#include "macros.h"

extern s16 gGondowanCliffs_Entrances[];
extern struct ActorPlacement gGondowanCliffs_PassEntrances[];

#include "shared/tile_sprite_reset.h"

/// The cliffs' actors by their rows: the two puddles Frost freezes, the dog,
/// Kindle the Mars Djinni, and Kraden (Jenna's and Sheba's rows take their own
/// numbers).
enum {
  ACTOR_TIGHTROPE_POST_A = 8,
  ACTOR_TIGHTROPE_POST_B,
  ACTOR_ROPE_POST_A,
  ACTOR_ROPE_POST_B,
  ACTOR_FIRST_PUDDLE = 12,
  ACTOR_SECOND_PUDDLE = 13,
  ACTOR_IVY,
  /// The stump that stands in the stream once
  /// FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER is set.
  ACTOR_STUMP,
  ACTOR_DOG = 16,
  ACTOR_GEYSER, ///< the unseen point the geyser rises from
  ACTOR_KINDLE = 21,
  ACTOR_KRADEN = 22
};

/// The pass's actors by their rows: the tightrope's two posts follow the reset
/// script's actor.
enum { ACTOR_PASS_TIGHTROPE_POST_A = 9, ACTOR_PASS_TIGHTROPE_POST_B = 10 };

/// The pass's entrance an event sends the party in by, aboard the ship.
enum { ENTRANCE_PASS_BY_SHIP = 99 };

/// Talking to the Djinni at the end of the cliffs starts the battle with
/// Kindle, a Mars Djinni.
void GondowanCliffs_FightKindle(void) {
  Field_RequestDjinnBattle(ACTOR_KINDLE, DJINNI_KINDLE);
}

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    return gGondowanCliffs_PassEntrances;
  return gGondowanCliffs_Entrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gGondowanCliffs_ActorPlacements[];
extern struct ActorPlacement gGondowanCliffs_PassActorPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    return gGondowanCliffs_PassActorPlacements;
  return gGondowanCliffs_ActorPlacements;
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"
// clang-format on

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

// The Gondowan Cliffs' events and set-up, and the geysers Scoop digs up out of
// the yellow sand: each rises and falls on its timer, carrying the leader up
// its column of water (Geyser_Update, Geyser_Draw).

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/// Set as Frost freezes each puddle into a pillar of ice, for this visit.
#define FLAG_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_02

struct Actor;
struct EffectOptions;
/// MapWork.groundEffects read as its bit-fields: the footprints, and the kind
/// of wading (1 in rooms with a second level).
struct MapGroundEffects {
  u8 unusedBeforeGroundEffects[23]; ///< MapWork's fields before it
  u8 footprints : 2;
  u8 wading : 2;
  u8 unusedHighBits : 4; ///< no code names them
};
/// A geyser Scoop has dug up: it rises and falls on its timer, carrying the
/// platform at its top, as Yampi Desert's rising sand does.
struct Geyser {
  s16 id;              ///< its actor; 0 for a free entry
  s16 timer;           ///< frames into its period
  s16 height;          ///< pixels it has risen
  s16 period;          ///< frames in one rise and fall
  s16 riseAt;          ///< the timer it starts to rise at
  s16 fallAt;          ///< the timer it starts to fall at
  s16 max;             ///< pixels it rises to
  s16 min;             ///< pixels it falls to
  s16 unreadSetToOne;  ///< set to 1 by Geyser_Add and never read
  s16 step;            ///< pixels a frame: 4 rising, -8 falling, 0 at rest
  s16 unreadCleared;   ///< cleared by Geyser_Add and never read
  s16 phase;           ///< the platform's bob, 1/65536 turns
  struct Actor *rider; ///< the platform at its top, which the leader rides
};

/// The geysers' work, kept in the inventory snapshot's spare room.
struct GeyserWork {
  s16 entry;        ///< the VRAM cache entry holding the column's tiles
  s16 firstTerrain; ///< the terrain id and collision slot of the first entry
  struct Geyser entries[16];
  /// The OAM words Geyser_Draw queues: at most 80 sprites of three words.
  void *block;
};

#define GEYSER_WORK ((struct GeyserWork *)(gInventorySnapshot + 0x188))

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void Tightrope_Place(s32 a, s32 b, s32 c);
void Rope_Initialize(void);
void Geyser_Start(s32 unused, s32 firstTerrain);
void Tightrope_Initialize(void);

/// The leaping stump's update: its sprite turns a 32nd of a turn a frame.
void GondowanCliffs_SpinStump(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation -= TRIG_TURN / 32;
}

/// Waits until the actor has come down to height (16.16), a second at most.
void GondowanCliffs_WaitToLand(struct Actor *actor, s32 height) {
  s32 count = 60;

  while (count != 0) {
    System_Wait(1);
    count--;
    if (actor->y.fixed <= height)
      break;
  }
}

/// Move on the stump at the stream's edge: it rolls, leaps into the stream
/// and lands upright in the water with a splash, a step across for the party.
void GondowanCliffs_LeapStumpIntoWater(s32 mode, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 unused[3]; // Unused: gives the stack frame its 12 bytes.
  struct EffectOptions options;
  struct FieldSprite *sprite;
  u32 i;

  if (FIELD_CELL(actor->x.fixed) != 76)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
  if (FIELD_CELL(Actor_GetById(ACTOR_STUMP)->x.fixed) == 76) {
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    for (i = 0; i <= 17; i++) {
      System_Wait(1);
      sprite = actor->sprite;
      sprite->rotation -= TRIG_TURN / 256;
      actor->x.fixed -=
          Trig_Cos(((struct FieldSprite *)actor->sprite)->rotation) / 2;
      actor->targetX = TARGET_UNSET;
    }
    actor->update = GondowanCliffs_SpinStump;
    Actor_SetSpeed(id, 3);
    Actor_SetDestination(id, 1193, 530);
    actor->gravity = toFIXED(4) / 5;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->layer = 0;
    FieldActor_WaitForMoveById(id);
    GondowanCliffs_WaitToLand(actor, 0);
    Event_PlaySound(SE_THUMP);
    MapGrid_SetShake(5, 5, 1);
    Event_PlaySound(SE_RUMBLE);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    ((struct FieldSprite *)actor->sprite)->rotation = TRIG_TURN / 16;
    // Cleared through a word: as a function pointer the store sinks below the
    // effect options.
    *(u32 *)&actor->update = 0;
    options.type = SPRITE_SPLASH;
    options.startScaleX = FIXED_ONE / 2;
    options.targetScaleX = FIXED_ONE * 3 / 2;
    options.targetScaleY = FIXED_ONE * 6 / 5;
    options.startScaleY = FIXED_ONE * 4 / 5;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
                      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_SCALE_TO_TARGET,
                      &options);
    Event_PlaySound(SE_CAST);
    Actor_SetAnimation(id, STUMP_ANIMATION_SPLASH);
    FieldMapGrid_WaitForShakeToSettle();
    Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER);
  }
  Event_End();
}

/// Frost on the first puddle: its ice pillar stands still and its cell can be
/// climbed, for this visit.
void GondowanCliffs_FreezeFirstPuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
  Event_SetFlag(FLAG_FIRST_PUDDLE_FROZEN);
}

/// Frost on the second puddle: its ice pillar stands 32 pixels tall and its
/// cell can be climbed, for this visit.
void GondowanCliffs_FreezeSecondPuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_SECOND_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  actor->groundHeight = toFIXED(32);
  actor->y.fixed = toFIXED(32);
  MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  Event_SetFlag(FLAG_SECOND_PUDDLE_FROZEN);
}

/// Whirlwind on the ivy blows it away for good.
void GondowanCliffs_BlowAwayFoliage(void) {
  GameFlag_Set(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN);
}

/// Scoop on the sand of cell behaviour 30 digs up the geyser for good.
void GondowanCliffs_ScoopUpGeyser(s32 unused, s32 cell) {
  struct Actor *actor;

  if (cell == 30) {
    actor = Actor_Get(ACTOR_GEYSER);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(ACTOR_GEYSER, 1912, 728);
    actor->groundHeight = actor->y.fixed =
        Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG);
  }
}

#include "text/msg_ids.h"

/// Talking to the dog barks at the leader, unless he faces the geyser's cell
/// beside it, which opens the field menu instead.
void GondowanCliffs_TalkToDog(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct {
    s32 x, y, z;
  } position;
  s32 *out = (s32 *)&position;

  out[0] = actor->x.fixed;
  out[1] = actor->y.fixed;
  out[2] = actor->z.fixed;
  FieldVector_AddPolarOffset(16, actor->facing, out);
  if (FIELD_CELL(position.x) == 119 && FIELD_CELL(position.z) == 45) {
    gFieldWork->fieldMenu = 1;
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceEachOther(ACTOR_DOG, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgRrruffRrruff);
    // "Rrruff! Rrruff!"
    Event_ShowMessage(ACTOR_DOG, 0);
    Actor_FaceDirection(ACTOR_DOG, FACING_WEST, 0);
    Event_End();
  }
}

extern struct Trigger gGondowanCliffs_PassTriggers[];
extern struct Trigger gGondowanCliffs_Triggers[];

/// Grind on the rock in the pass breaks it for good.
void GondowanCliffs_GrindTheRock(s32 unused, s32 id) {
  Actor_SetPosition(id, 0, 0);
  Event_SetFlag(FLAG_GONDOWAN_CLIFFS_ROCK_GROUND);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    return gGondowanCliffs_PassTriggers;
  return gGondowanCliffs_Triggers;
}

u16 gGondowanCliffs_SavedWaterColors[4] BSS_DATA;

/// Each frame: the four colours of the water's palette row turn one step every
/// 16 frames.
void GondowanCliffs_CycleWaterColors(void) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(0, 12);
  u32 phase = gFrameCount >> 4;
  s32 i;

  for (i = 3; i >= 0; i--) {
    u32 index = 3;

    index &= phase;
    *color++ = gGondowanCliffs_SavedWaterColors[index];
    phase++;
  }
}

/// Sets up the pass: on arriving by ship, the leader walks again; the
/// tightrope is strung and the water's colours start turning.
void GondowanCliffs_SetUpPass(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_PASS_BY_SHIP) {
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_SHIP;
    gMainRoutineWork.ownerOverride = 1;
    Event_SetDestination(SCENE_GONDOWAN_CLIFFS_PASS,
                         GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_NORTH);
  }
  Tightrope_Initialize();
  Value3((s32 (*)())Tightrope_Place, 0, ACTOR_PASS_TIGHTROPE_POST_A,
         ACTOR_PASS_TIGHTROPE_POST_B);
  Iwram_CopyWords((s32)gGondowanCliffs_SavedWaterColors,
                  (s32)&BG_PLTT_COLOR(0, 12), 8);
  Task_AddFrame(GondowanCliffs_CycleWaterColors, TASK_PASS_VBLANK);
}

void Actor_MoveToObject(s32 actor, s32 target);

/// The scene that asks where the party intends to go, to help recover the black
/// orb.
void GondowanCliffs_TurnBackToHelpPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 552, 488);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 568, 480);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 584, 472);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 608, 472);
  FieldEvent_SetMessageId((s32)MsgWhereDoYou);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  // "Felix! Where do you intend to go? We should try to help Piers..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 10);
  FieldActor_Jump(ACTOR_JENNA, 6, 15);
  FieldActor_Jump(ACTOR_JENNA, 6, 23);
  // "Kraden's right. I think we should try to help Piers, too."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_Stop(ACTOR_SHEBA);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
  // "I completely agree with Kraden and Jenna."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "It looks like the majority wins! Let's go back to Kibombo and help..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 584, 472);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 568, 480);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 552, 488);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 528, 488);
  Event_FinishScreenTransition();
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 512, 488);
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(GONDOWAN_CLIFFS_TO_WORLD_MAP_A);
}

void Rope_Place(s32 a, s32 b, s32 c, s32 d);
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high);

/// Sets up the scene being entered. The pass has its own set-up; the cliffs
/// turn the party back while Piers still needs their help, and otherwise
/// string the rope and tightrope, restore the ivy, the frozen puddles, the
/// geyser and the stump as their flags say, and start the water's colours.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 height;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS) {
    GondowanCliffs_SetUpPass();
    return 0;
  }
  if (GameFlag_Get(FLAG_KIBOMBO_PIERS_MET) &&
      !GameFlag_Get(FLAG_GABOMBA_CEREMONY_ENDED)) {
    GondowanCliffs_TurnBackToHelpPiers();
    return 0;
  }
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS) {
    Rope_Initialize();
    Rope_Place(0, FLAG_SCENE_TEMP_00, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
    if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
      Tightrope_Initialize();
      Tightrope_Place(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN))
      Actor_SetPosition(ACTOR_IVY, 0, 0);
    else
      Actor_GetById(ACTOR_IVY)->scaleY = FIXED_ONE * 5 / 4;
    if (GameFlag_Get(FLAG_FIRST_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
    if (GameFlag_Get(FLAG_SECOND_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
    Geyser_Start(0, 240);
    Geyser_Add(0, ACTOR_GEYSER, 0, 0, 64);
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG)) {
      actor = Actor_GetById(ACTOR_GEYSER);
      MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
      Actor_SetPosition(ACTOR_GEYSER, 1912, 728);
      height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
      actor->y.fixed = height;
      actor->groundHeight = height;
    }
    Actor_SetAnimation(ACTOR_DOG, 4);
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER)) {
      MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
      Actor_SetPosition(ACTOR_STUMP, 1193, 530);
      Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_GetById(ACTOR_STUMP)->collisionFlags = 0;
      Actor_SetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
    } else {
      Actor_GetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
    }
    Iwram_CopyWordsEntry(((u8 *)gGondowanCliffs_SavedWaterColors),
                         (void *)&BG_PLTT_COLOR(0, 12), 8);
    Task_AddFrame(GondowanCliffs_CycleWaterColors, TASK_PASS_VBLANK);
    ((struct MapGroundEffects *)Ram_HeapSlots->mapGrid)->wading = 1;
  }
  return 0;
}

extern s32 gGondowanCliffs_MapRects[];

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS)
    Ram_HeapSlots->mapGrid->cameraBoxes = (const s16 *)gGondowanCliffs_MapRects;
  return 0;
}

/// The whole-pixel half of a 16.16 coordinate.
#define PIXEL_PART(v) (((s16 *)&(v))[1])

/// The swinging actors' callback, the same as Yampi Desert's platforms: for
/// each of the 16 entries it moves the actor on its timer, writes its height to
/// the collision and cell maps, and carries its rider with a bob.
void Geyser_Update(void) {
  struct GeyserWork *work = GEYSER_WORK;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Geyser *entry = work->entries;
  s32 firstTerrain = work->firstTerrain;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision = (s32 *)Ram_MapCollision + firstTerrain;

  for (; i <= 15; collision++, entry++, i++) {
    struct Actor *actor;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    actor = Actor_GetById(entry->id);
    if (actor->x.fixed == 0 && actor->z.fixed == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Sound_Play(SE_CURSE_FALL);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(SOUND_FADE_OUT_EXIT_SE);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = FIELD_CELL(actor->x.fixed) + FIELD_CELL(actor->z.fixed) * 128;
    terrain[index] = (u8)work->firstTerrain + i;
    *collision = ((PIXEL_PART(actor->y.fixed) + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y.fixed = actor->y.fixed + -toFIXED(16);
    } else {
      FIXED wave;

      entry->phase += 0x1000;
      wave = Iwram_MulFX(toFIXED(2), Trig_Sin(entry->phase)) + -toFIXED(14);
      rider->y.fixed = actor->y.fixed + toFIXED(entry->height) + wave;
    }
    rider->x.fixed = actor->x.fixed;
    rider->z.fixed = actor->z.fixed;
  }
}

/// Draws the swinging actors' ropes and heads (the 16 records of
/// gInventorySnapshot
/// + 0x188 kept by Geyser_Add): for each raised actor, a rope segment every
/// 16 pixels of its height and the bobbing head on top, at most 80 sprites,
/// queued by Oam_AddSpriteAtDepthClamped.
void Geyser_Draw(void) {
  struct GeyserWork *work = GEYSER_WORK;
  struct Geyser *swing = work->entries;
  s32 k;
  u32 tile = gVramCacheEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 scrollX, scrollZ, lift;
  struct Actor *actor;
  s32 h, x;
  FIXED y;
  s32 screenY;
  FIXED z;
  s32 depth;
  u32 *p;

  scrollX = view[0];
  scrollX &= FIELD_PIXEL_MASK;
  scrollZ = view[1] & FIELD_PIXEL_MASK;
  lift = map->origin[1];
  for (k = 15; k >= 0; k--, swing++) {
    if (swing->id == 0 || swing->height == 0)
      continue;
    actor = Actor_GetById(swing->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    for (h = 0; h < swing->height; h += 16) {
      if (count > 79)
        continue;
      x = actor->x.fixed - scrollX;
      y = actor->y.fixed + toFIXED(h) - lift;
      z = actor->z.fixed - scrollZ - lift;
      x = FIXEDtoINT(x) - 8;
      screenY = z - y;
      screenY = FIXEDtoINT(screenY) - 16;
      depth = FIXEDtoINT(y + z) + 58;
      if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
        continue;
      x &= 511;
      screenY &= 255;
      p = oam;
      *p++ = 0;
      *p++ = screenY | OBJ_X(x) | (OBJ_SIZE_16X16 | OBJ_256_COLORS);
      *p = (tile + frame) | OBJ_PRIORITY(2);
      Oam_AddSpriteAtDepthClamped(oam, depth);
      oam += 3;
      count++;
    }
    if (count > 79)
      continue;
    x = actor->x.fixed - scrollX;
    y = actor->y.fixed + toFIXED(swing->height) - lift;
    z = actor->z.fixed - scrollZ - lift;
    screenY = z - y;
    x = FIXEDtoINT(x) - 8;
    screenY = ((screenY + Math_MulFX(-toFIXED(2), Trig_Sin(swing->phase)) +
                -toFIXED(2)) >>
               16) -
              4;
    depth = FIXEDtoINT(y + z) + 58;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      continue;
    x &= 511;
    screenY &= 255;
    p = oam;
    *p++ = 0;
    *p++ = screenY | OBJ_X(x) | (OBJ_SIZE_16X16 | OBJ_256_COLORS);
    *p = (tile + frame - 32) | OBJ_PRIORITY(2);
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  }
}

/// Starts the geysers: clears their work unless the party keeps its place,
/// loads the column's tiles and starts Geyser_Update and Geyser_Draw.
void Geyser_Start(s32 unused, s32 firstTerrain) {
  struct GeyserWork *work = GEYSER_WORK;
  void *buffer;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords((s32)work, sizeof(struct GeyserWork));
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Decompress_TaggedLz(gField_GeyserTiles, buffer);
  work->entry = VramCache_FindFreeEntry();
  Vram_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(80 * 3 * sizeof(u32));
  work->firstTerrain = firstTerrain;
  FieldTask_AddFrame((s32)Geyser_Update, TASK_PASS_VBLANK + 13);
  FieldTask_AddFrame((s32)Geyser_Draw, TASK_PASS_VBLANK);
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}

/// Makes actor id the geyser of entry slot, rising from low to high pixels
/// every six seconds, with an unseen platform at its top for the leader.
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high) {
  struct GeyserWork *work = GEYSER_WORK;
  struct Geyser *entry = &work->entries[slot];
  struct Actor *actor = Actor_Get(id);
  struct Actor *rider;

  Actor_Get(id);
  Actor_SetSpriteFlags(actor, 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unreadSetToOne = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    entry->step = 0;
    entry->phase = 0;
    entry->unreadCleared = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }
  rider = (FieldActor_Create(ACTOR_CREATE_NO_SPRITE << 12, actor->x.fixed,
                             actor->y.fixed, actor->z.fixed));
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  entry->rider = rider;
  return entry;
}

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef PIXEL_PART

// The Gondowan Cliffs' tables, which the scene's getters return.

#include "text/msg_ids.h"

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
extern s32 gGondowanCliffs_ResetTileSpriteScript[];

void GondowanCliffs_FightKindle(void);
void GondowanCliffs_FreezeFirstPuddle(void);
void GondowanCliffs_TalkToDog(void);

LATE_DATA s32 gGondowanCliffs_ResetTileSpriteScript[] = {
    SC_CALL(TileSprite_Reset),
    SC_END,
};

// clang-format off
LATE_DATA s16 gGondowanCliffs_Entrances[] = {
    0, -1, 680, 0, 280, 0x4000, 0, -1,
    -1, -1, -1, 0, 1, -1, 536, 0,
    488, 0, 0, 512, 0, 0x800, 864, 0,
    2, -1, 0x7e8, 0, 792, 0x8000, 0, 512,
    0, 0x800, 864, 0, 3, -1, 0x628, 0,
    680, 0x4000, 0, 512, 0, 0x800, 864, 0,
    99, -1, 0x4f8, 0, 552, 0x4000, 0, 512,
    0, 0x800, 864, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

LATE_DATA struct ActorPlacement gGondowanCliffs_PassEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0x40000118, toFIXED(-1), -0x1, 0xffff, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(GondowanCliffs),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_GONDOWAN_CLIFFS_HIDEOUT, GondowanCliffsHideout, GONDOWAN_CLIFFS_HIDEOUT_FROM_GONDOWAN_CLIFFS, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanCliffsPass),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_PASS_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_PASS_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gGondowanCliffs_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1560, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1624, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1304, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1880, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1576, 0, 672, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1232, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1928, 0, 728, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 888, 0, 232, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(424, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 816, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_KINDLE), MOVEMENT_TYPE_STAND, 1336, 0, 648, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGondowanCliffs_PassActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)gGondowanCliffs_ResetTileSpriteScript, 330, 0, 79, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 304, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LATE_DATA struct Trigger gGondowanCliffs_PassTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 11, CONDITION_ALWAYS, EVENT(GondowanCliffs_GrindTheRock)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGondowanCliffs_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, IF_FLAG_SET(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN), 3},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(GondowanCliffs_TalkToDog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, MSGNO(MsgDigHereYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgIKnewIt)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgITriedTo)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgIWonderWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgIfTheOther)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgILongTo)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(GondowanCliffs_FightKindle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, FLAG_SCENE_TEMP_00, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FLAG_FIRST_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, FLAG_SECOND_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, EVENT(GondowanCliffs_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER, EVENT(GondowanCliffs_LeapStumpIntoWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, EVENT(GondowanCliffs_ScoopUpGeyser)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gGondowanCliffs_MapRects[] = {
    0x2000200, 0x3600400, 0x600, 0x1601000, -1, -1,
};
// clang-format on
