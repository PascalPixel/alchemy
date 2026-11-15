/// @file
/// The Kibombo Mountains, the pass the Kibombo warriors guard on the way to
/// their village, in the rooms of this overlay: crates the party pushes and
/// vines it climbs while it keeps out of the guards' sight, a guard who sees
/// it sending it back, a guard dog and its bone, and the Jupiter Djinni Waft
/// (KibomboMountains_FightWaft).

#include "flags.h"
#include "djinn_ids.h"

/// Once the message of the Gabomba is carried the Kibombo let the party pass:
/// the guards and the dog are gone.
#define FLAG_KIBOMBO_LETS_PARTY_PASS FLAG_GABOMBA_MESSAGE_CARRIED
#define FLAG_KIBOMBO_GUARDS_REPORTED 0x880
#define FLAG_KIBOMBO_OVERGROWTH_CLEARED 0x882
#define FLAG_KIBOMBO_FIRST_PILLAR_DROPPED 0x8e7
#define FLAG_KIBOMBO_SECOND_PILLAR_DROPPED 0x8e8
/// For this visit: the lower guard knocked out by the crate, the dog shut in
/// south or north of the dropped crate or eating the bone, and the party turned
/// back by the guards.
#define FLAG_GUARD_KNOCKED_OUT FLAG_SCENE_TEMP_00
#define FLAG_DOG_SHUT_SOUTH FLAG_SCENE_TEMP_01
#define FLAG_DOG_EATING_BONE FLAG_SCENE_TEMP_02
#define FLAG_DOG_SHUT_NORTH FLAG_SCENE_TEMP_03
#define FLAG_TURNED_BACK FLAG_AREA_TEMP_00
/// For this visit: the first and second puddles of the Disk Axe room frozen,
/// the Waft room's puddle frozen and its sprout grown, and the dog's pillar and
/// the guard's crate dropped.
#define FLAG_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_10
#define FLAG_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_11
#define FLAG_PUDDLE_FROZEN FLAG_SCENE_TEMP_21
#define FLAG_SPROUT_GROWN FLAG_SCENE_TEMP_22
#define FLAG_DOG_PILLAR_DROPPED FLAG_SCENE_TEMP_23
#define FLAG_GUARD_CRATE_DROPPED FLAG_SCENE_TEMP_24
#include "item_ids.h"
#include "field/field.h"
#include "types.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/shared.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/field_event.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "field/call.h"
#include "system/ram_buffer.h"
#include "version.h"
#include "shared/tables.h"
#include "field/actor_script.h"

/// The room where Waft hides above the guards, by the rows of
/// gKibomboMountains_WaftPlacements.
enum KibomboMountainsWaftActorId {
  /// The warrior on watch above the dog, who also shouts when it barks.
  ACTOR_UPPER_GUARD = ACTOR_FIRST_PLACED,
  /// The warrior on watch below, whom the crate can knock out.
  ACTOR_LOWER_GUARD,
  ACTOR_GUARD_DOG,
  /// The puddle Frost turns into a pillar.
  ACTOR_PUDDLE,
  /// The sprout Growth turns into a vine up to Waft's ledge.
  ACTOR_SPROUT,
  /// The overgrowth Whirlwind clears from the small cavern's way in.
  ACTOR_OVERGROWTH,
  /// The pillar pushed off the ledge to shut the dog in.
  ACTOR_DOG_PILLAR,
  /// The crate pushed off the ledge onto the lower guard.
  ACTOR_GUARD_CRATE,
  /// The crate the bone lies on, which Tremor shakes.
  ACTOR_BONE_CRATE,
  ACTOR_BONE,
  ACTOR_WAFT,
  /// XXX: placed hidden and intangible below the guard's crate, and nothing
  /// else reads it; a render with its sprite shown would settle what it is.
  ACTOR_UNSEEN_MARKER
};

/// The room of the Disk Axe chest, by the rows of
/// gKibomboMountains_DiskAxeChestPlacements.
enum KibomboMountainsDiskAxeChestActorId {
  ACTOR_ROPE_WEST_POST = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_EAST_POST,
  ACTOR_FIRST_LOOKOUT,
  ACTOR_SECOND_LOOKOUT,
  ACTOR_THIRD_LOOKOUT,
  /// The pillar dropped to the chest's ledge.
  ACTOR_SECOND_DROPPED_PILLAR,
  /// The two crates the party hides behind from the first lookout.
  ACTOR_WEST_CRATE,
  ACTOR_EAST_CRATE,
  /// The two crates further south.
  ACTOR_LOWER_WEST_CRATE,
  ACTOR_LOWER_EAST_CRATE,
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  /// The pillar dropped to the lower path.
  ACTOR_FIRST_DROPPED_PILLAR,
  ACTOR_WEST_TORCH,
  ACTOR_EAST_TORCH,
  /// The warriors who turn the party back; ACTOR_WARRIOR_E also comes to
  /// hear the lookout's report.
  ACTOR_WARRIOR_A,
  ACTOR_WARRIOR_B,
  ACTOR_WARRIOR_C,
  ACTOR_WARRIOR_D,
  ACTOR_WARRIOR_E,
  ACTOR_WARRIOR_F
};

void KibomboMountains_GuardSpotsParty(s32 value);
void KibomboMountains_DogSpotsParty(s32 value);

void KibomboMountains_TremorDropsBone(void);

void KibomboMountains_JumpForward(void);

void KibomboMountains_FreezeFirstPuddle(void);

void KibomboMountains_FreezeSecondPuddle(void);

void KibomboMountains_GuardsReport(void);

void KibomboMountains_StepWestOfCrate(void);

void KibomboMountains_StepEastOfCrate(void);

void KibomboMountains_StepBeforeSecondLookout(void);

void KibomboMountains_StepBeforeSecondLookoutAgain(void);

void KibomboMountains_StepBeforeThirdLookout(void);

void KibomboMountains_StepBeforeThirdLookoutAgain(void);
void KibomboMountains_StepBeforeUpperGuard(void);
void KibomboMountains_StepBeforeUpperGuardAgain(void);
void KibomboMountains_StepBeforeLowerGuardAgain(void);
void KibomboMountains_StepSouthOfDog(void);

extern u32 gKibomboMountains_RaisedPillarScript[];
extern u32 gKibomboMountains_CrateScript[];
extern u32 gKibomboMountains_PatrolScript[];
extern u32 gKibomboMountains_LowerGuardPatrolScript[];
extern u32 gKibomboMountains_DogPatrolScript[];
extern u32 gKibomboMountains_SecondLookoutScript[];

void KibomboMountains_StepBeforeLowerGuard(void);

void Rope_Place(s32 a, s32 b, s32 c, s32 d);

#include "text/msg_ids.h"

void Field_TryJumpForward(void);

void KibomboMountains_FightWaft(void) {
  Field_RequestDjinnBattle(ACTOR_WAFT, DJINNI_WAFT);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "shared/actor_bob.h"

extern struct ActorPlacement gKibomboMountains_Placements[];
extern struct ActorPlacement gKibomboMountains_WaftPlacements[];
extern struct ActorPlacement gKibomboMountains_DiskAxeChestPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
    return gKibomboMountains_WaftPlacements;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
    return gKibomboMountains_DiskAxeChestPlacements;
  return gKibomboMountains_Placements;
}

void Pillar_SetMoved(void) {
  struct Actor *object = Actor_Get(ACTOR_PUDDLE);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
  Event_SetFlag(FLAG_PUDDLE_FROZEN);
}

/// Growth on the plant: it grows into a vine to climb to Waft.
void KibomboMountains_GrowVine(void) {
  MapGrid_CopyCellAttributes(30, 35, 1, 1, 28, 34);
  MapGrid_CopyCellAttributes(30, 34, 1, 1, 28, 33);
  Event_SetFlag(FLAG_SPROUT_GROWN);
}

#include "shared/actor_wait_to_land.h"

void KibomboMountains_StartPushingCrate(s32 unused, s32 id) {
  if (id == ACTOR_GUARD_CRATE)
    FieldActor_SetSpritePriority(ACTOR_GUARD_CRATE, 1);
}

extern s32 gKibomboMountains_BobScript[];

void Actor_StartScript(struct Actor *object, const u8 *script);
void Actor_StartIdle(struct Actor *object);

/// A crate pushed off the ledge: dropped by the dog it shuts the dog in on one
/// side; dropped on the lower guard it knocks him out, and missing him only
/// puzzles him.
void KibomboMountains_DropCrate(s32 unused, s32 id) {
  struct Actor saved;

  if (id == ACTOR_DOG_PILLAR) {
    struct Actor *pillar = Actor_GetById(ACTOR_DOG_PILLAR);
    struct Actor *dog;
    s32 x;
    s32 z;
    s32 cell;

    if (!Event_GetFlag(FLAG_DOG_PILLAR_DROPPED) &&
        (x = pillar->x.fixed >> 20) == 47 &&
        (z = pillar->z.fixed >> 20) == 28) {
      Actor_WaitToLand(ACTOR_DOG_PILLAR);
      MapGrid_CopyCellAttributes(49, 28, 1, 1, x, z);
      Event_SetFlag(FLAG_DOG_PILLAR_DROPPED);
      Actor_SetSpritePriority(ACTOR_DOG_PILLAR, 2);
      dog = Actor_GetById(ACTOR_GUARD_DOG);
      cell = dog->z.fixed >> 20;
      if (cell == 27) {
        Sound_Play(SE_VOICE_ANIMAL_2);
        dog->z.fixed = px(440);
      } else if (cell == 28 || cell == 29) {
        Sound_Play(SE_VOICE_ANIMAL_3);
        dog->z.fixed = px(472);
      }
      if (pillar->z.fixed < dog->z.fixed)
        Event_SetFlag(FLAG_DOG_SHUT_SOUTH);
      else
        Event_SetFlag(FLAG_DOG_SHUT_NORTH);
    } else {
      Actor_SetSpritePriority(id, 1);
    }
  }
  if (id == ACTOR_GUARD_CRATE) {
    struct Actor *crate = Actor_GetById(ACTOR_GUARD_CRATE);
    struct Actor *guard;

    if (!Event_GetFlag(FLAG_GUARD_CRATE_DROPPED) &&
        crate->x.fixed >> 20 == 53 && crate->z.fixed >> 20 == 36) {
      s32 dx;

      Actor_WaitToLand(ACTOR_GUARD_CRATE);
      Actor_SetSpritePriority(ACTOR_GUARD_CRATE, 2);
      guard = Actor_GetById(ACTOR_LOWER_GUARD);
      dx = crate->x.fixed - guard->x.fixed;
      if (dx >= 0 ? dx < px(8) : guard->x.fixed - crate->x.fixed < px(8)) {
        guard->motionFlags = 0;
        guard->speedLimit = px(1);
        FieldActor_SetMoveTargetFixed(guard, crate->x.fixed, crate->y.fixed,
                                      crate->z.fixed);
        FieldActor_WaitForMove(guard);
        Actor_StartScript(crate, gKibomboMountains_BobScript);
        Actor_StartIdle(guard);
        guard->x.fixed = 0;
        guard->z.fixed = 0;
        Event_SetFlag(FLAG_GUARD_KNOCKED_OUT);
      } else {
        Iwram_CopyWords(&saved, guard, sizeof(saved));
        Actor_ResetTargetsAndVelocity(ACTOR_LOWER_GUARD);
        FieldActor_Jump(ACTOR_LOWER_GUARD, 4, 0);
        FieldEvent_Wait(30);
        Actor_ShowEmote(ACTOR_LOWER_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        Actor_FaceActor(ACTOR_LOWER_GUARD, ACTOR_GUARD_CRATE, 0);
        FieldEvent_Wait(60);
        Actor_SetAnimationAndWait(ACTOR_LOWER_GUARD, 4);
        FieldEvent_Wait(20);
        Iwram_CopyWords(guard, &saved, sizeof(saved));
        MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
        Event_SetFlag(FLAG_GUARD_CRATE_DROPPED);
      }
    }
  }
}

/// Tremor on the chest: the bone drops, and the dog, unless shut in, runs to it
/// and stays there eating.
void KibomboMountains_TremorDropsBone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetDestination(ACTOR_BONE, 764, 384);
  Actor_MoveToAndWait(ACTOR_BONE_CRATE, 746, 380);
  Actor_Get(ACTOR_BONE)->velocityY = px(4);
  FieldActor_WaitForMoveById(ACTOR_BONE);
  Actor_Get(ACTOR_BONE)->priorityFlags |= 2;
  if (!Event_GetFlag(FLAG_KIBOMBO_LETS_PARTY_PASS) &&
      !Event_GetFlag(FLAG_DOG_SHUT_SOUTH)) {
    FieldActor_Stop(ACTOR_GUARD_DOG);
    Actor_MoveToAndWait(ACTOR_GUARD_DOG, 764, 388);
    Actor_SetDestination(ACTOR_BONE, 764, 376);
    Actor_WalkToAndWait(ACTOR_GUARD_DOG, 764, 380);
    Actor_SetAnimation(ACTOR_GUARD_DOG, 4);
    Actor_SetAttachedEffect(ACTOR_GUARD_DOG,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Event_SetFlag(FLAG_DOG_EATING_BONE);
  }
  Event_End();
}

/// Whirlwind on the overgrowth opens the small cavern.
void KibomboMountains_WhirlwindClearsOvergrowth(void) {
  GameFlag_Set(FLAG_KIBOMBO_OVERGROWTH_CLEARED);
}

void KibomboMountains_JumpForward(void) { Field_TryJumpForward(); }

void KibomboMountains_FreezeFirstPuddle(void) {
  struct Actor *object = Actor_Get(ACTOR_FIRST_PUDDLE);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
  Event_SetFlag(FLAG_FIRST_PUDDLE_FROZEN);
}

void KibomboMountains_FreezeSecondPuddle(void) {
  struct Actor *object = Actor_Get(ACTOR_SECOND_PUDDLE);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
  Event_SetFlag(FLAG_SECOND_PUDDLE_FROZEN);
}

void KibomboMountains_StartPushingChestRoomCrate(s32 unused, s32 cell) {
  if (cell == ACTOR_LOWER_WEST_CRATE || cell == ACTOR_LOWER_EAST_CRATE)
    MapGrid_CopyCellAttributes(120, 33, 8, 2, 20, 33);
  if (cell == ACTOR_WEST_CRATE || cell == ACTOR_EAST_CRATE)
    MapGrid_CopyCellAttributes(112, 15, 16, 4, 8, 15);
}

void KibomboMountains_SetCrateCell(s32 id, s32 a, s32 b) {
  struct Actor *object = Actor_Get(id);
  s32 x = object->x.fixed >> 20;
  s32 z = object->z.fixed >> 20;

  MapGrid_CopyCellAttributes(a, b, 1, 1, x, z);
}

void KibomboMountains_FinishPushingChestRoomCrate(s32 unused, s32 cell) {
  if (cell == ACTOR_FIRST_DROPPED_PILLAR) {
    struct Actor *object = Actor_Get(ACTOR_FIRST_DROPPED_PILLAR);

    if (!Event_GetFlag(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED)) {
      s32 x = object->x.fixed >> 20;

      if (x == 4) {
        Actor_WaitToLand(ACTOR_FIRST_DROPPED_PILLAR);
        MapGrid_CopyCellAttributes(3, 28, 1, 1, x, 28);
        Event_SetFlag(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED);
      }
    }
  }
  if (cell == ACTOR_SECOND_DROPPED_PILLAR) {
    struct Actor *object = Actor_Get(ACTOR_SECOND_DROPPED_PILLAR);

    if (!Event_GetFlag(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED)) {
      s32 x = object->x.fixed >> 20;

      if (x == 28) {
        s32 z = object->z.fixed >> 20;

        if (z == 30) {
          Actor_WaitToLand(ACTOR_SECOND_DROPPED_PILLAR);
          MapGrid_CopyCellAttributes(32, 30, 1, 2, x, z);
          Event_SetFlag(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED);
        }
      }
    }
  }
  if (cell == ACTOR_WEST_CRATE || cell == ACTOR_EAST_CRATE) {
    KibomboMountains_SetCrateCell(ACTOR_WEST_CRATE, 8, 17);
    KibomboMountains_SetCrateCell(ACTOR_EAST_CRATE, 8, 17);
  }
  if (cell == ACTOR_LOWER_WEST_CRATE || cell == ACTOR_LOWER_EAST_CRATE) {
    KibomboMountains_SetCrateCell(ACTOR_LOWER_WEST_CRATE, 22, 33);
    KibomboMountains_SetCrateCell(ACTOR_LOWER_EAST_CRATE, 22, 33);
  }
}

extern struct Trigger gKibomboMountains_Triggers[];
extern struct Trigger gKibomboMountains_WaftTriggers[];
extern struct Trigger gKibomboMountains_DiskAxeChestTriggers[];
extern struct Trigger gKibomboMountains_CavernTriggers[];

const u8 *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
    return gKibomboMountains_WaftTriggers;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
    return gKibomboMountains_DiskAxeChestTriggers;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_3)
    return gKibomboMountains_CavernTriggers;
  return gKibomboMountains_Triggers;
}

void KibomboMountains_GuardsTurnPartyBack(void);
void Rope_Initialize(void);

#if !TLA_PATCH_1
#define EVENT_WORK_VALUE 0x201
#else
#define EVENT_WORK_VALUE 0x204
#endif

s32 Scene_Initialize(void) {
  struct Actor *object;
  s16 scene;
  s32 flag;
  s32 height;

  gFieldWork->transition = EVENT_WORK_VALUE;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT) {
    Actor_SetSpritePriority(ACTOR_WAFT, 1);
    Actor_SetSpritePriority(ACTOR_UNSEEN_MARKER, 1);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_UNSEEN_MARKER), 0);
    Actor_GetById(ACTOR_UNSEEN_MARKER)->collisionFlags = 0;
    Actor_GetById(ACTOR_UNSEEN_MARKER)->priorityFlags |= 2;
    Actor_GetById(ACTOR_SPROUT)->collisionFlags = 0;
    if (GameFlag_Get(FLAG_SPROUT_GROWN)) {
      Map_CopyCellAttributeRect(30, 35, 1, 1, 28, 34);
      Map_CopyCellAttributeRect(30, 34, 1, 1, 28, 33);
    }
    if (GameFlag_Get(FLAG_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
    if (GameFlag_Get(FLAG_DOG_PILLAR_DROPPED))
      MapGrid_CopyCellAttributes(49, 28, 1, 1, 47, 28);
    else
      Actor_SetSpritePriority(ACTOR_DOG_PILLAR, 1);
    if (GameFlag_Get(FLAG_GUARD_CRATE_DROPPED))
      MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
    flag = GameFlag_Get(FLAG_DOG_EATING_BONE);
    if (flag == 0) {
      struct Actor *lift;

      Actor_GetById(ACTOR_BONE_CRATE)->priorityFlags |= 0x80;
      lift = Actor_GetById(ACTOR_BONE);
      lift->priorityFlags |= 0x40;
      lift->y.fixed = px(224);
      lift->groundHeight = px(224);
      lift->collisionFlags = flag;
    } else {
      Actor_SetAttachedEffect(ACTOR_GUARD_DOG, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ATTACH);
    }
    Actor_ShowItemIconById(ACTOR_BONE, ITEM_BONE);
    if (GameFlag_Get(FLAG_KIBOMBO_OVERGROWTH_CLEARED))
      Actor_SetPosition(ACTOR_OVERGROWTH, 0, 0);
    else
      Actor_GetById(ACTOR_OVERGROWTH)->scaleY = Q16_ONE * 5 / 4;
  } else if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST) {
    Actor_SetSpritePriority(ACTOR_ROPE_WEST_POST, 1);
    Actor_SetSpritePriority(ACTOR_ROPE_EAST_POST, 1);
    Rope_Initialize();
    Rope_Place(0, 0x9e8, ACTOR_ROPE_WEST_POST, ACTOR_ROPE_EAST_POST);
    if (GameFlag_Get(FLAG_FIRST_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
    if (GameFlag_Get(FLAG_SECOND_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
    if (GameFlag_Get(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED)) {
      object = Actor_GetById(ACTOR_FIRST_DROPPED_PILLAR);
      Actor_SetPosition(ACTOR_FIRST_DROPPED_PILLAR, 72, 456);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed);
      object->groundHeight = height;
      object->y.fixed = height;
      MapGrid_CopyCellAttributes(3, 28, 1, 1, 4, 28);
    }
    if (GameFlag_Get(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED)) {
      object = Actor_GetById(ACTOR_SECOND_DROPPED_PILLAR);
      Actor_SetPosition(ACTOR_SECOND_DROPPED_PILLAR, 456, 488);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed);
      object->groundHeight = height;
      object->y.fixed = height;
      MapGrid_CopyCellAttributes(32, 30, 1, 2, 28, 30);
    }
    MapGrid_CopyCellAttributes(8, 15, 16, 4, 112, 15);
    MapGrid_CopyCellAttributes(20, 33, 8, 2, 120, 33);
    KibomboMountains_SetCrateCell(ACTOR_WEST_CRATE, 8, 17);
    KibomboMountains_SetCrateCell(ACTOR_EAST_CRATE, 8, 17);
    KibomboMountains_SetCrateCell(ACTOR_LOWER_WEST_CRATE, 22, 33);
    KibomboMountains_SetCrateCell(ACTOR_LOWER_EAST_CRATE, 22, 33);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 8)
      KibomboMountains_GuardsTurnPartyBack();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The first time the party climbs the pass: "Is the way clear?" Two warriors
/// report, and one goes back to his watch.
void KibomboMountains_GuardsReport(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgIsTheWay);
  Actor_SetPosition(ACTOR_WARRIOR_E, 312, 304);
  // "Is the way clear?"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  FieldCamera_MoveTo(256, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  Actor_ResetTargetsAndVelocity(ACTOR_SECOND_LOOKOUT);
  Actor_FaceActor(ACTOR_SECOND_LOOKOUT, ACTOR_WARRIOR_E, 0);
  Actor_WalkToAndWait(ACTOR_WARRIOR_E, 264, 312);
  Actor_WalkToAndWait(ACTOR_WARRIOR_E, 256, 400);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LOOKOUT, 3);
  // "This area is clear! The strange one has not been seen!"
  Event_ShowMessage(ACTOR_SECOND_LOOKOUT, 0);
  Actor_RunRepeatedMotion(ACTOR_SECOND_LOOKOUT, 1);
  // "And there?"
  Event_ShowMessage(ACTOR_SECOND_LOOKOUT, 0);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_E, 4);
  // "We have not seen the stranger near the village."
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  // "Do you believe he was truly headed toward Kibombo?"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_ShowEmote(ACTOR_SECOND_LOOKOUT, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "The guards here were only recently knocked unconscious."
  Event_ShowMessage(ACTOR_SECOND_LOOKOUT, 0);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Correct. We were under orders to maintain a strict watch over the..."
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LOOKOUT, 3);
  // "No one must be permitted into Kibombo until Akafubu has completed..."
  Event_ShowMessage(ACTOR_SECOND_LOOKOUT, 0);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Then we had better hurry to find the one who attacked our guards."
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_WalkToAndWait(ACTOR_WARRIOR_E, 264, 312);
  Actor_WalkTo(ACTOR_WARRIOR_E, 360, 312);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LOOKOUT, 3);
  // "Keep a good watch. Do not permit anyone to pass."
  Event_ShowMessage(ACTOR_SECOND_LOOKOUT, 0);
  FieldActor_WatchAndSetScript(ACTOR_SECOND_LOOKOUT,
                               gMainRoutineWork.leaderActorId);
  System_Wait(30);
  // "If anyone interrupts Akafubu's ceremony, he will hold you responsible..."
  Event_ShowMessage(ACTOR_SECOND_LOOKOUT, 0);
  Actor_RunRepeatedMotion(ACTOR_SECOND_LOOKOUT, 2);
  FieldEvent_Wait(60);
  Actor_SetPosition(ACTOR_WARRIOR_E, 0, 0);
  Actor_SetScript(ACTOR_SECOND_LOOKOUT, gKibomboMountains_SecondLookoutScript);
  Event_End();
  Event_SetFlag(FLAG_KIBOMBO_GUARDS_REPORTED);
}

void KibomboMountains_WalkSlowly(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SelectAnimation(object, 1);
  object->acceleration = px(1) / 4;
  object->speedLimit = px(1) / 2;
}

/// Caught: "Here! The intruder is here!" The warriors march the party back down
/// the road.
void KibomboMountains_GuardsTurnPartyBack(void) {
  s32 leader = gMainRoutineWork.leaderActorId;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_WARRIOR_A, 248, 472);
  Actor_SetPosition(ACTOR_WARRIOR_B, 264, 472);
  Actor_SetPosition(ACTOR_WARRIOR_C, 235, 483);
  Actor_SetPosition(ACTOR_WARRIOR_D, 232, 496);
  Actor_SetPosition(ACTOR_WARRIOR_E, 277, 483);
  Actor_SetPosition(ACTOR_WARRIOR_F, 280, 496);
  KibomboMountains_WalkSlowly(leader);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_A);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_B);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_C);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_D);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_E);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_F);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgHereTheIntruder);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Here! The intruder is here!"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_FaceActor(ACTOR_WARRIOR_E, ACTOR_WARRIOR_C, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_C, 3);
  Actor_FaceActor(ACTOR_WARRIOR_E, leader, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Kibombo does not permit visitors!"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_E, 3);
  // "You just follow this road right back to where you came from!"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_FaceActor(ACTOR_WARRIOR_C, ACTOR_WARRIOR_A, 0);
  Actor_ShowEmote(ACTOR_WARRIOR_C, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Guard! Get him out of here!"
  Event_ShowMessage(ACTOR_WARRIOR_C, 0);
  Actor_WalkTo(ACTOR_WARRIOR_A, 248, 536);
  FieldEvent_Wait(1);
  Actor_WalkTo(ACTOR_WARRIOR_B, 264, 536);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_C, leader);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_D, leader);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_E, leader);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_F, leader);
  FieldEvent_Wait(2);
  Actor_Get(leader)->steeringFlags &= ~1;
  Actor_WalkToAndWait(leader, 256, 552);
  FieldEvent_Wait(1);
  Actor_Get(leader)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAnimation(ACTOR_WARRIOR_A, 1);
  Actor_SetAnimation(ACTOR_WARRIOR_B, 1);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_WARRIOR_A, 4);
  FieldEvent_Wait(3);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_B, 4);
  FieldEvent_Wait(20);
  Actor_WalkTo(ACTOR_WARRIOR_A, 248, 488);
  FieldEvent_Wait(2);
  Actor_WalkToAndWait(ACTOR_WARRIOR_B, 264, 488);
  Actor_WalkTo(ACTOR_WARRIOR_A, 248, 440);
  Actor_WalkTo(ACTOR_WARRIOR_B, 264, 440);
  Actor_WalkTo(ACTOR_WARRIOR_C, 232, 456);
  Actor_WalkTo(ACTOR_WARRIOR_D, 232, 472);
  Actor_WalkTo(ACTOR_WARRIOR_E, 280, 456);
  Actor_WalkToAndWait(ACTOR_WARRIOR_F, 280, 472);
  Actor_SetPosition(ACTOR_WARRIOR_A, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_B, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_C, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_D, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_E, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_F, 0, 0);
  Event_End();
  Event_SetFlag(FLAG_TURNED_BACK);
}

/// Sends the party back to the foot of the pass, by whether it was turned back
/// on this visit.
void KibomboMountains_SendPartyBack(void) {
  s32 moved = Event_GetFlag(FLAG_TURNED_BACK);
  s32 scene = SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST;

  if (moved)
    Event_SetDestination(scene, 9);
  else
    Event_SetDestination(scene, 8);
}

/// A warrior sees the party: "Guards! An intruder!"
void KibomboMountains_GuardSpotsParty(s32 id) {
  s32 leader = gMainRoutineWork.leaderActorId;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_ResetTargetsAndVelocity(id);
  Actor_ShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(id, leader, 20);
  FieldEvent_SetMessageId(MsgGuardsAnIntruder);
  // "Guards! An intruder!"
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  // "Someone is trying to pass! To arms, Kibombo warriors! To arms!"
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  KibomboMountains_SendPartyBack();
  Event_End();
}

/// The dog sees the party and barks for the warriors.
void KibomboMountains_DogSpotsParty(s32 id) {
  s32 leader = gMainRoutineWork.leaderActorId;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(id);
  Actor_ShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(id, leader, 20);
  FieldEvent_SetMessageId(MsgGrrrrrRrrruffRrufff);
  // "Grrrrr! Rrrruff! Rrufff!"
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  // "An intruder! Repel the intruder immediately!"
  Event_ShowMessage(ACTOR_UPPER_GUARD, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  KibomboMountains_SendPartyBack();
  Event_End();
}

/// Stepping in the first lookout's sight, unless the crate hides the leader.
void KibomboMountains_StepWestOfCrate(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *crate = Actor_Get(ACTOR_WEST_CRATE);

  if ((crate->z.fixed - leader->z.fixed >= 0
           ? crate->z.fixed - leader->z.fixed
           : leader->z.fixed - crate->z.fixed) < px(16) &&
      crate->x.fixed > leader->x.fixed)
    return;
  KibomboMountains_GuardSpotsParty(ACTOR_FIRST_LOOKOUT);
}

void KibomboMountains_StepEastOfCrate(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *crate = Actor_Get(ACTOR_EAST_CRATE);

  if ((crate->z.fixed - leader->z.fixed >= 0
           ? crate->z.fixed - leader->z.fixed
           : leader->z.fixed - crate->z.fixed) < px(16) &&
      crate->x.fixed < leader->x.fixed)
    return;
  KibomboMountains_GuardSpotsParty(ACTOR_FIRST_LOOKOUT);
}

void KibomboMountains_StepBeforeSecondLookout(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_SECOND_LOOKOUT);
}

void KibomboMountains_StepBeforeSecondLookoutAgain(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_SECOND_LOOKOUT);
}

void KibomboMountains_StepBeforeThirdLookout(void) {
  if (gMainRoutineWork.movementMode != 3)
    KibomboMountains_GuardSpotsParty(ACTOR_THIRD_LOOKOUT);
}

void KibomboMountains_StepBeforeThirdLookoutAgain(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_THIRD_LOOKOUT);
}

void KibomboMountains_StepBeforeUpperGuard(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_UPPER_GUARD);
}

void KibomboMountains_StepBeforeUpperGuardAgain(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_UPPER_GUARD);
}

void KibomboMountains_StepNorthOfDog(void) {
  if (!Event_GetFlag(FLAG_DOG_EATING_BONE) &&
      !Event_GetFlag(FLAG_DOG_SHUT_SOUTH))
    KibomboMountains_DogSpotsParty(ACTOR_GUARD_DOG);
}

void KibomboMountains_StepBeforeLowerGuard(void) {
  if (!Event_GetFlag(FLAG_GUARD_KNOCKED_OUT))
    KibomboMountains_GuardSpotsParty(ACTOR_LOWER_GUARD);
}

void KibomboMountains_StepBeforeLowerGuardAgain(void) {
  if (!Event_GetFlag(FLAG_GUARD_KNOCKED_OUT))
    KibomboMountains_GuardSpotsParty(ACTOR_LOWER_GUARD);
}

void KibomboMountains_StepSouthOfDog(void) {
  if (!Event_GetFlag(FLAG_DOG_SHUT_NORTH) &&
      !Event_GetFlag(FLAG_DOG_EATING_BONE))
    KibomboMountains_DogSpotsParty(ACTOR_GUARD_DOG);
}

#undef EVENT_WORK_VALUE
#include "text/msg_ids.h"

void KibomboMountains_FightWaft(void);
void Pillar_SetMoved(void);
void KibomboMountains_GrowVine(void);
void KibomboMountains_WhirlwindClearsOvergrowth(void);
void KibomboMountains_StepNorthOfDog(void);

extern u32 gKibomboMountains_FirstLookoutScript[];

// clang-format off
u32 gKibomboMountains_RaisedPillarScript[] = {
    SC_ANIMATION(2),
    SC_END,
    SC_ANIMATION(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, px(24)),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(KibomboMountainsWaft),
    SCENE_EXIT_UNLESS(1, WorldMap, 26, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboMountainsDiskAxeChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboMountainsDiskAxeChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, KibomboMountainsDiskAxeChest, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, KibomboMountains3, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, KibomboMountains3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, KibomboMountains3, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountainsDiskAxeChest),
    SCENE_EXIT_UNLESS(1, WorldMap, 25, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboMountainsWaft, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboMountainsWaft, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, KibomboMountainsWaft, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountains3),
    SCENE_EXIT_UNLESS(1, KibomboMountainsWaft, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboMountainsWaft, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboMountainsWaft, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gKibomboMountains_CrateScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gKibomboMountains_Placements[] = {
    {SCENE_TABLE_END},
};

u32 gKibomboMountains_PatrolScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, 129),
    SC_ANIMATION(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_ANIMATION(4),
    SC_WAIT(90),
    SC_ANIMATION(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(4),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gKibomboMountains_LowerGuardPatrolScript[] = {                                                               
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.1),                                                
      SC_SET(SW_COLLISION, 129),                                               
      SC_ANIMATION(2),                                                               
      SC_MOVE(856, 0, 584),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),                       
      SC_WAIT(30),                                                             
      SC_ANIMATION(4),                                                               
      SC_WAIT(90),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(792, 0, 584),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(30),                                                             
      SC_ANIMATION(4),                                                               
      SC_WAIT(90),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gKibomboMountains_BobScript[] = {
    SC_CALL(Actor_Bob),
    SC_END,
};

u32 gKibomboMountains_DogPatrolScript[] = {
    SC_SPEED(1.25),
    SC_ACCELERATION(0.25),
    SC_SET(SW_COLLISION, 129),
    SC_ANIMATION(3),
    SC_SET(SW_VELOCITY_Y, px(3)),
    SC_MOVE(760, 0, 488),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(764, 0, 472),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(760, 0, 456),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(764, 0, 440),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(760, 0, 424),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(764, 0, 408),
    SC_SYNC,
    SC_MOVE(760, 0, 392),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(3)),
    SC_MOVE(764, 0, 408),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(760, 0, 424),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(764, 0, 440),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(760, 0, 456),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(764, 0, 472),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(760, 0, 488),
    SC_SYNC,
    SC_MOVE(764, 0, 504),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct ActorPlacement gKibomboMountains_WaftPlacements[] = {
    ACTOR_PLACEMENT(83, FLAG_KIBOMBO_LETS_PARTY_PASS, (s32)gKibomboMountains_PatrolScript, 616, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, FLAG_KIBOMBO_LETS_PARTY_PASS, (s32)gKibomboMountains_LowerGuardPatrolScript, 824, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(189, FLAG_KIBOMBO_LETS_PARTY_PASS, (s32)gKibomboMountains_DogPatrolScript, 760, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 712, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 888, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(288, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WAFT), MOVEMENT_TYPE_STAND, 440, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

u32 gKibomboMountains_FirstLookoutScript[] = {                                                               
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.1),                                                
      SC_SET(SW_COLLISION, 129),                                               
      SC_ANIMATION(2),                                                               
      SC_MOVE(184, 0, 296),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),                       
      SC_WAIT(30),                                                             
      SC_ANIMATION(4),                                                               
      SC_WAIT(90),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(248, 0, 296),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(30),                                                             
      SC_ANIMATION(4),                                                               
      SC_WAIT(90),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gKibomboMountains_SecondLookoutScript[] = {                                                               
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.1),                                                
      SC_SET(SW_COLLISION, 129),                                               
      SC_ANIMATION(2),                                                               
      SC_MOVE(248, 0, 448),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),                       
      SC_WAIT(30),                                                             
      SC_ANIMATION(4),                                                               
      SC_WAIT(90),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(264, 0, 448),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(30),                                                             
      SC_ANIMATION(4),                                                               
      SC_WAIT(90),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

struct ActorPlacement gKibomboMountains_DiskAxeChestPlacements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, FLAG_KIBOMBO_LETS_PARTY_PASS, (s32)gKibomboMountains_FirstLookoutScript, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, FLAG_KIBOMBO_LETS_PARTY_PASS, (s32)gKibomboMountains_SecondLookoutScript, 248, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, FLAG_KIBOMBO_LETS_PARTY_PASS, (s32)gKibomboMountains_PatrolScript, 424, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(250, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 456, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 136, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 312, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 328, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 440, 0, 536, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, (s32)gKibomboMountains_CrateScript, 88, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, (s32)gKibomboMountains_RaisedPillarScript, 232, 0, 536, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, (s32)gKibomboMountains_RaisedPillarScript, 280, 0, 536, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gKibomboMountains_PatrolScript, 0, 0, 0, 0x3f00, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gKibomboMountains_LowerGuardPatrolScript, 0, 0, 0, 0x4100, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gKibomboMountains_PatrolScript, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gKibomboMountains_LowerGuardPatrolScript, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gKibomboMountains_PatrolScript, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gKibomboMountains_LowerGuardPatrolScript, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gKibomboMountains_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKibomboMountains_WaftTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 6, IF_FLAG_SET(FLAG_KIBOMBO_OVERGROWTH_CLEARED), 6},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 20, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeUpperGuard)},
    {TRIGGER_STEP, 21, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeUpperGuardAgain)},
    {TRIGGER_STEP, 22, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepNorthOfDog)},
    {TRIGGER_STEP, 23, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeLowerGuard)},
    {TRIGGER_STEP, 24, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeLowerGuardAgain)},
    {TRIGGER_STEP, 25, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepSouthOfDog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT(KibomboMountains_JumpForward)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_DropCrate)},
    {TRIGGER_TALK, ACTOR_GUARD_DOG, IF_FLAG_SET(FLAG_DOG_EATING_BONE), MSGNO(MsgSniffSniffLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_DOG, IF_FLAG_SET(FLAG_DOG_EATING_BONE), MSGNO(MsgABoneThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_DOG, CONDITION_ALWAYS, MSGNO(MsgThisRockIs)},
    {TRIGGER_TALK, ACTOR_GUARD_CRATE, IF_FLAG_SET(FLAG_GUARD_KNOCKED_OUT), MSGNO(MsgHeyWhoS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_CRATE, IF_FLAG_SET(FLAG_GUARD_KNOCKED_OUT), MSGNO(MsgItSDark)},
    {TRIGGER_TALK, ACTOR_WAFT, CONDITION_ALWAYS, EVENT(KibomboMountains_FightWaft)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT(Pillar_SetMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SPROUT_GROWN, EVENT(KibomboMountains_GrowVine)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_OVERGROWTH, CONDITION_ALWAYS, EVENT(KibomboMountains_WhirlwindClearsOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOG_PILLAR, CONDITION_ALWAYS, EVENT(KibomboMountains_DropCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, FLAG_GUARD_KNOCKED_OUT, EVENT(KibomboMountains_StartPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GUARD_CRATE, FLAG_GUARD_KNOCKED_OUT, EVENT(KibomboMountains_DropCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), -14320, FLAG_DOG_EATING_BONE, EVENT(KibomboMountains_TremorDropsBone)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 2064, FLAG_DOG_EATING_BONE, EVENT(KibomboMountains_TremorDropsBone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibomboMountains_DiskAxeChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 30, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepWestOfCrate)},
    {TRIGGER_STEP, 31, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepEastOfCrate)},
    {TRIGGER_STEP, 32, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeSecondLookout)},
    {TRIGGER_STEP, 33, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeSecondLookoutAgain)},
    {TRIGGER_STEP, 34, FLAG_KIBOMBO_GUARDS_REPORTED, EVENT(KibomboMountains_GuardsReport)},
    {TRIGGER_STEP, 35, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeThirdLookout)},
    {TRIGGER_STEP, 36, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBeforeThirdLookoutAgain)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_WEST_POST, 0x9e8, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_FIRST_PUDDLE_FROZEN, EVENT(KibomboMountains_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_SECOND_PUDDLE_FROZEN, EVENT(KibomboMountains_FreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_DROPPED_PILLAR, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DROPPED_PILLAR, FLAG_KIBOMBO_FIRST_PILLAR_DROPPED, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_WEST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_EAST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibomboMountains_CavernTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TABLE_END},
};
// clang-format on
