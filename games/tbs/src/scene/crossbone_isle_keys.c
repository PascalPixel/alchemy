/// @file
/// Three of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle, each opening off one of the nine floors the cave descends through
/// (crossbone_isle.c) as the other five do (crossbone_isle_stepping_stones.c),
/// with four chests each. All three take the area name Crossbone Isle Cave. In
/// the order the party meets them:
///
/// - SceneId_CrossboneIsleKeys, off the third floor, a room of locked doors.
/// Four
///   Blue Keys lie about it until Catch takes them, and a Red Key shows only
///   while Reveal lasts. Walking into one of the four blue doors in a row with
///   a Blue Key opens it and uses the key up (four flags from
///   FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN), and the Red Key opens the red
///   door beyond them (FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN). The keys taken and
///   the doors opened are restored on entry.
/// - SceneId_CrossboneIsleStatues, off the fourth, where five statues of a
/// robed
///   woman wait, lit in circles by braziers in the dark. As the leader crosses
///   each one's stretch of the room it steps out after him unless he is hidden
///   by Cloak, and goes back home once he is past
///   (FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER while one is after him); Halt
///   stops one where it stands, and Reveal clears the translucent layer over
///   the room while it lasts. The first, second and fifth statues start from
///   per-frame checks of the leader's cell, the third and fourth from
///   step triggers in the three cells around their homes and the three south of
///   them.
/// - SceneId_CrossboneIsleBane, off the sixth, where Bane, a Venus Djinni,
/// hides.
///   Five pillars to push or move with Move, a ladder and a puddle that Frost
///   freezes into a pillar of ice make a way across the room. The first three
///   times the party comes near, Bane flees, the third time to the room's last
///   corner (FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE to _LAST_ESCAPE); there it
///   runs to whichever spot the leader steps toward unless Halt holds it.
///   Talked to while Halt holds it, Bane joins the party.

#include "save_block.h"
#include "djinn_ids.h"
#include "version.h"
#include "heap_slots.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle_keys.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "event_layer.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "pushed_actor.h"
#include "io_reg.h"
#include "resource.h"
#include "actor.h"
#include "animspr.h"
#include "mem.h"
#include "dma.h"
#include "task.h"
#include "call.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "iowrite.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "heap.h"

// Map-section temp flags: Scene_Arrive clears 0x300 to 0x3ff when
// the party enters another area, and other areas use the numbers
// for their own.
/// CrossboneIsleBane's third stone pillar has come down to two cells high or
/// less.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED 0x300

#if TBS_PATCH_1
#define CROSSBONE_PURSUER_STEP_Z 16
#define CROSSBONE_LEADER_STEP_Z 24
#else
#define CROSSBONE_PURSUER_STEP_Z 24
#define CROSSBONE_LEADER_STEP_Z 16
#endif

static void CrossboneIsleKeys_PushFirstPillar(void);
static void CrossboneIsleKeys_FollowSecondPillar(void);
static void CrossboneIsleKeys_PositionSecondPillarMarker(void);
static void CrossboneIsleKeys_InitializeKeys(void);
static void CrossboneIsleKeys_InitializeStatues(void);
static void CrossboneIsleKeys_InitializeBane(void);

/// CrossboneIsleKeys's placed actors, numbered in the order of
/// gCrossboneIsleKeysRoomActorTemplates: the keys, drawn as their item
/// icons (Colosso_ShowItemIcon) until Catch takes them.
enum CrossboneIsleKeysActorId {
  /// ★ Named from the game: "Blue Key" (MsgItemName), a key icon lying among
  /// the room's stone posts
  ACTOR_FIRST_BLUE_KEY = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Blue Key" (MsgItemName), the second key icon
  ACTOR_SECOND_BLUE_KEY,
  /// ★ Named from the game: "Blue Key" (MsgItemName), the third key icon
  ACTOR_THIRD_BLUE_KEY,
  /// ★ Named from the game: "Blue Key" (MsgItemName), the fourth key icon
  ACTOR_FOURTH_BLUE_KEY,
  /// Shown only while Reveal lasts.
  /// ★ Named from the game: "Red Key" (MsgItemName), which opens the red door
  ACTOR_RED_KEY
};

/// CrossboneIsleStatues's placed actors, numbered in the order of
/// gCrossboneIsleStatuesActorTemplates: the five statues that pursue the
/// leader, all of one sprite, each waiting at its home for the leader to cross
/// its stretch of the room.
enum CrossboneIsleStatuesActorId {
  /// ★ Named from what the player sees: a pink statue of a robed woman, lit in
  /// a circle by a brazier (its render)
  ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the second statue of a robed woman (its
  /// render)
  ACTOR_SECOND_STATUE,
  /// ★ Named from what the player sees: the third statue of a robed woman (its
  /// render)
  ACTOR_THIRD_STATUE,
  /// ★ Named from what the player sees: the fourth statue of a robed woman (its
  /// render)
  ACTOR_FOURTH_STATUE,
  /// ★ Named from what the player sees: the fifth statue of a robed woman (its
  /// render)
  ACTOR_FIFTH_STATUE
};

/// The events CrossboneIsleStatues's per-frame checks raise when the
/// leader crosses the first, second or fifth pursuer's stretch of the room.
/// The third and fourth chases start from step triggers instead.
enum CrossboneIsleStatuesRaisedTrigger {
  RAISED_FIRST_CHASE = 91,
  RAISED_SECOND_CHASE = 92,
  RAISED_FIFTH_CHASE = 96
};

/// CrossboneIsleBane's placed actors, numbered in the order of
/// gCrossboneIsleBaneActorTemplates.
enum CrossboneIsleBaneActorId {
  /// The five pillars, in Goma Cave's pillar sprite, pushed by walking into
  /// them or with Move.
  /// ★ Named from what the player sees: a brown stone pillar in the north-west
  /// of the room (its render)
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a brown stone pillar in the middle of
  /// the room, a chest on its top (its render)
  ACTOR_SECOND_PILLAR,
  /// ★ Named from what the player sees: a brown stone pillar in the west of the
  /// room (its render)
  ACTOR_THIRD_PILLAR,
  /// ★ Named from what the player sees: one of the two brown stone pillars
  /// side by side in the south-east (its render)
  ACTOR_FOURTH_PILLAR,
  /// ★ Named from what the player sees: the other of the two brown stone
  /// pillars in the south-east (its render)
  ACTOR_FIFTH_PILLAR,
  /// Frozen by Frost into a pillar of ice.
  /// ★ Named from what the player sees: a small blue puddle on the floor of the
  /// south-west (its render)
  ACTOR_PUDDLE,
  /// The chest on top of the second pillar, in the sprite of McCoy's Palace's
  /// chests, holding the Demon Axe: it follows the pillar until the pillar
  /// reaches the ladder, where the chest's examine trigger 107 takes its place,
  /// and it is drawn open once FLAG_CROSSBONE_ISLE_DEMON_AXE is set.
  /// ★ Named from what the player sees: a red chest standing on the second
  /// pillar (its render)
  ACTOR_SECOND_PILLAR_CHEST,
  /// Bane, a Venus Djinni, until it is caught.
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  ACTOR_BANE
};

enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

/// CrossboneIsleBane: the puddle is frozen.
/// ★ Named from what the player sees: a small blue puddle on the floor of the
/// lowest hall (its render)
#define FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// CrossboneIsleBane: the chest on the second pillar has reached the ladder.
/// ★ Named from what the player sees: a chest on top of a pillar between two
/// rows of spikes, below a ladder (its render)
#define FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER FLAG_SCENE_TEMP_06
/// CrossboneIsleBane: the leader last looked east from the ladder, which gives
/// the ladder cell its other metatile behaviors.
/// ★ Named from what the player sees: the ladder up the ledge beside the second
/// pillar, from which the leader can face east and jump across (its render)
#define FLAG_CROSSBONE_ISLE_BANE_LOOKED_EAST FLAG_SCENE_TEMP_07
// The Move events of CrossboneIsleBane's pillars also test FLAG_SCENE_TEMP_01
// and 03 to 05, which nothing sets.
/// CrossboneIsleStatues: a statue is after the leader.
#define FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER FLAG_SCENE_TEMP_20

/// Publishes the next raised-event trigger.
static __inline__ void MapLocalState_StoreStep(s16 *field, s32 step) {
  *field = step;
}

/// Lifts the first pillar and clears the terrain beneath its column.
static __inline__ void LiftPillar(s32 column, u8 motionFlags) {
  {
    s32 destinationColumn = column;
    MapGrid_CopyCellAttributes(61, 36, 1, 1, destinationColumn, 42);
  }
  Actor_GetById(ACTOR_FIRST_PILLAR)->motionFlags = motionFlags;
  Actor_GetById(ACTOR_FIRST_PILLAR)->y.fixed = PIXELS(32);
}

#include "common/staged_actor.h"

/// Talking to Bane in CrossboneIsleBane while Halt holds it
/// (TRIGGER_TALK_HALTED): Bane, a Venus Djinni, joins the party.
static void CrossboneIsleKeys_BaneJoins(void) {
  Djinn_JoinParty(ACTOR_BANE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
}

UNUSED static void CrossboneIsleKeys_UnusedHook(void) {}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_CrossboneIsleKeys) {
    return gCrossboneIsleKeysRoomEntrances;
  }
  if (selector == (s32)&SceneId_CrossboneIsleStatues) {
    return gCrossboneIsleStatuesEntrances;
  }
  if (selector == (s32)&SceneId_CrossboneIsleBane) {
    return gCrossboneIsleBaneEntrances;
  }
  return gCrossboneIsleKeysEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIsleKeysExits; }

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_CrossboneIsleKeys) {
    return gCrossboneIsleKeysRoomActorTemplates;
  }
  if (selector == (s32)&SceneId_CrossboneIsleStatues) {
    return gCrossboneIsleStatuesActorTemplates;
  }
  if (selector == (s32)&SceneId_CrossboneIsleBane) {
    return gCrossboneIsleBaneActorTemplates;
  }
  return gCrossboneIsleKeysActorTemplatesOther;
}

/// Halt on a pursuer in CrossboneIsleStatues: nothing beyond the
/// field Psynergy itself, which keeps it from stepping out.
static void CrossboneIsleKeys_HaltPursuer(void) {}

// Turn the Djinni toward the party leader each frame.
// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void CrossboneIsleKeys_FaceBaneTowardLeader(struct Actor *updated) {
#else
static s32 CrossboneIsleKeys_FaceBaneTowardLeader(struct Actor *updated) {
#endif
  struct Actor *actor = updated;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed,
                          leader->x.fixed - actor->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// In CrossboneIsleBane, the first time the party comes near
/// (step trigger 20): Bane notices them and hops away.
static void CrossboneIsleKeys_BaneFirstEscape(void) {
  struct Actor *djinni;

  if (Event_GetFlag(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE);
    Event_Start();
    FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Actor_PlaceCurrentWithinCameraBounds(ACTOR_BANE, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirection(ACTOR_BANE, FACING_EAST, 20);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_BANE, 2);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_BANE, PIXELS(1), PIXELS(1) / 2);
    Event_PlaySong(SE_JUMP);
    djinni = Actor_GetById(ACTOR_BANE);
    djinni->velocityY = PIXELS(8);
    Actor_WalkToAndWait(ACTOR_BANE, 584, 680);
    Actor_FaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
    Event_End();
  }
}

/// In CrossboneIsleBane, coming near again (step trigger 21): Bane hops,
/// dashes across the platforms and crashes into the far wall, shaking the room,
/// and is left squashing and stretching.
static void CrossboneIsleKeys_BaneSecondEscape(void) {
  struct Actor *spinningDjinni;
  s32 escapeComplete;
  struct Actor *jumpingDjinni;

  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
    escapeComplete = Event_GetFlag(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
    if (escapeComplete == 0) {
      GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
      Event_Start();

      // Focus on the Djinni before its next jump away.
      FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Actor_PlaceCurrentWithinCameraBounds(ACTOR_BANE, 1);
      FieldCamera_WaitForArrival();
      Actor_FaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_BANE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_RunRepeatedMotion(ACTOR_BANE, 2);
      Event_WaitFrames(20);
      Actor_SetSpeed(ACTOR_BANE, PIXELS(1), PIXELS(1) / 2);
      Event_PlaySong(SE_JUMP);
      jumpingDjinni = Actor_GetById(ACTOR_BANE);
      jumpingDjinni->velocityY = PIXELS(10);
      Actor_WalkToAndWait(ACTOR_BANE, 584, 664);
      Actor_FaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_BANE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_WaitFrames(30);

      // Dash across the platforms, then shake the room on landing.
      Actor_SetSpeed(ACTOR_BANE, PIXELS(8), PIXELS(1) / 4);
      Actor_WalkToAndWait(ACTOR_BANE, 664, 664);
      Actor_WalkToAndWait(ACTOR_BANE, 744, 664);
      Actor_WalkToAndWait(ACTOR_BANE, 824, 664);
      Event_WaitFrames(10);
      Event_PlaySong(SE_BANE_ESCAPE);
      MapGrid_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
      Event_WaitFrames(20);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Event_WaitFrames(30);

      // Leave the Djinni tilted while its scale animation continues.
      Actor_SetPosition(ACTOR_BANE, PIXELS(888), PIXELS(664));
      spinningDjinni = Actor_GetById(ACTOR_BANE);
      spinningDjinni->sprite->rotation = 0xf800;
      spinningDjinni->facing = 0;
      Actor_SetAnimationSpeed(spinningDjinni, 0);
      Actor_StartScript(spinningDjinni, gCrossboneIsleKeys_BaneSquashScript);
      Event_End();
    }
  }
}

/// In CrossboneIsleBane, the third time (step trigger 22): Bane recovers,
/// hops down a winding path to the room's last corner and keeps watching the
/// leader.
static void CrossboneIsleKeys_BaneLastEscape(void) {
  struct Actor *djinni;

  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0 &&
      Event_GetFlag(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE);
    Event_Start();

    // Restore the sprite before the last jump and winding escape path.
    djinni = Actor_GetById(ACTOR_BANE);
    djinni->sprite->rotation = 0;
    Actor_SetAnimationSpeed(djinni, 16);
    Event_PlaySong(SE_JUMP);
    djinni = Actor_GetById(ACTOR_BANE);
    djinni->velocityY = PIXELS(8);
    Actor_FaceDirection(ACTOR_BANE, FACING_WEST, 30);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_BANE, 2);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_BANE, PIXELS(1), PIXELS(1) / 2);
    Event_PlaySong(SE_JUMP);
    djinni = Actor_GetById(ACTOR_BANE);
    djinni->velocityY = PIXELS(4);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 680);
    Event_WaitFrames(10);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetSpeed(ACTOR_BANE, PIXELS(2), PIXELS(1));
    Actor_WalkToAndWait(ACTOR_BANE, 880, 696);
    Actor_WalkToAndWait(ACTOR_BANE, 882, 704);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 712);
    Actor_WalkToAndWait(ACTOR_BANE, 878, 720);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 728);
    Actor_WalkToAndWait(ACTOR_BANE, 882, 736);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 744);
    Actor_WalkToAndWait(ACTOR_BANE, 878, 752);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 760);
    Actor_SetPosition(ACTOR_BANE, PIXELS(856), PIXELS(824));
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_BANE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    djinni = Actor_GetById(ACTOR_BANE);
#ifdef UBFIX
    djinni->update = CrossboneIsleKeys_FaceBaneTowardLeader;
#else
    djinni->update =
        (void (*)(struct Actor *))CrossboneIsleKeys_FaceBaneTowardLeader;
#endif
    Event_End();
  }
}

/// In CrossboneIsleBane, stepping on the step triggers around Bane's
/// corner (30 to 41): Bane runs to the matching spot, as fast as the leader,
/// unless Halt holds it.
static void CrossboneIsleKeys_BaneKeepsAway(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 targetByteOffset;
  const u8 *targets;
  s32 zByteOffset;
  struct FieldWork *work;

  work = gFieldWork;
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
    if (gSaveBlock.haltedActor != ACTOR_BANE) {
      targetByteOffset = work->stepTrigger;
      djinni = Actor_GetById(ACTOR_BANE);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      djinni->speedLimit = leader->speedLimit;
      djinni = Actor_GetById(ACTOR_BANE);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      djinni->acceleration = leader->speedLimit;
      targetByteOffset -= 30;
      targets = (const u8 *)gCrossboneIsleKeys_BaneWalkTargets;
      // The table stores consecutive signed X/Z pixel pairs.
      targetByteOffset *= 2 * sizeof(s32);
      zByteOffset = targetByteOffset + sizeof(s32);
      Actor_ResetAndSetPositionInMode2(
          ACTOR_BANE, *(const s32 *)(targets + targetByteOffset),
          *(const s32 *)(targets + zByteOffset));
    }
  }
}

/// Halt on Bane in CrossboneIsleBane: it freezes in place.
static void CrossboneIsleKeys_HaltBane(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_BANE, 0);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_CrossboneIsleKeys) {
    return gCrossboneIsleKeysRoomTriggers;
  }
  if (selector == (s32)&SceneId_CrossboneIsleStatues) {
    return gCrossboneIsleStatuesTriggers;
  }
  if (selector == (s32)&SceneId_CrossboneIsleBane) {
    return gCrossboneIsleBaneTriggers;
  }
  return gCrossboneIsleKeysTriggersOther;
}

/// Unused: walking south into CrossboneIsleKeys's third or fourth blue door
/// jumps the leader forward where he can (Field_TryJumpForward).
///
/// Both rows that name it follow a plain step trigger row for the same door
/// (CrossboneIsleKeys_OpenThirdBlueDoor, CrossboneIsleKeys_OpenFourthBlueDoor),
/// which Scene_FindTrigger always finds first; those events jump forward
/// themselves after a dozen steps pressed south against the door, so the player
/// sees the same thing.
static void CrossboneIsleKeys_UnusedJumpPastBlueDoor(void) {
  Field_TryJumpForward();
}

/// Opens the selected blue door, playing its opening when asked.
static void CrossboneIsleKeys_OpenBlueDoor(s32 passageInfo) {
  s32 columnOffset = (passageInfo & PASSAGE_INDEX_MASK) * 4;
  s32 cellColumn = columnOffset + 77;
  s32 attributeColumn = columnOffset + 13;

  if (passageInfo & PASSAGE_PLAY_OPENING) {
    Sound_PlaySong(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_CopyCells(79, 29, 1, 3, cellColumn, 40);
    WaitFrames(40);
  }
  MapGrid_CopyCells(80, 29, 1, 3, cellColumn, 40);
  MapGrid_CopyCellAttributes(attributeColumn, 40, 1, 1, attributeColumn, 41);
  MapGrid_CopyCellAttributes(attributeColumn, 40, 1, 1, attributeColumn, 42);
}

/// Opens the red door, playing its opening when asked.
static void CrossboneIsleKeys_OpenRedDoor(s32 passageInfo) {
  if ((passageInfo & PASSAGE_PLAY_OPENING) != 0) {
    Event_PlaySong(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_CopyCells(84, 29, 1, 3, 70, 49);
    WaitFrames(60);
  }
  MapGrid_CopyCells(85, 29, 1, 3, 70, 49);
  MapGrid_CopyCellAttributes(6, 49, 1, 1, 6, 50);
  MapGrid_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

/// Facing CrossboneIsleKeys's red door with the Red Key (step trigger
/// 50): the door rumbles open, and the key is used up.
static void CrossboneIsleKeys_UseRedKey(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    if (Event_GetFlag(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) == 0) {
      if (PartyInventory_FindMember(ITEM_RED_KEY) != -1) {
        GameFlag_Set(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN);
        CrossboneIsleKeys_OpenRedDoor(PASSAGE_PLAY_OPENING);
        PartyInventory_Remove(ITEM_RED_KEY);
      }
    }
  }
}

/// Facing a blue door with a Blue Key: the door rumbles open, and the key is
/// used up.
static void CrossboneIsleKeys_UseBlueKey(s32 doorIndex) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 flag;

  if (leader->facing != FACING_NORTH) {
    return;
  }
  flag = doorIndex + FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN;
  if (GameFlag_Get(flag) != 0) {
    return;
  }
  if (PartyInventory_FindMember(ITEM_BLUE_KEY) == -1) {
    return;
  }
  GameFlag_Set(flag);
  CrossboneIsleKeys_OpenBlueDoor(PASSAGE_PLAY_OPENING | doorIndex);
  PartyInventory_Remove(ITEM_BLUE_KEY);
}

/// Touching CrossboneIsleKeys's first blue door (step trigger 51).
static void CrossboneIsleKeys_OpenFirstBlueDoor(void) {
  CrossboneIsleKeys_UseBlueKey(0);
}

/// Touching CrossboneIsleKeys's second blue door (step trigger 52).
static void CrossboneIsleKeys_OpenSecondBlueDoor(void) {
  CrossboneIsleKeys_UseBlueKey(1);
}

/// Touching CrossboneIsleKeys's third blue door (step trigger 53);
/// pressing on it from the north looks ahead every dozen blocked steps.
static void CrossboneIsleKeys_OpenThirdBlueDoor(void) {
  struct Actor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIsleKeys_UseBlueKey(2);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gFieldWork->blockedSteps;
    if (*blockedSteps > 12) {
      Field_TryJumpForward();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

/// Touching CrossboneIsleKeys's fourth blue door (step trigger 54);
/// pressing on it from the north looks ahead every dozen blocked steps.
static void CrossboneIsleKeys_OpenFourthBlueDoor(void) {
  struct Actor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIsleKeys_UseBlueKey(3);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gFieldWork->blockedSteps;
    if (*blockedSteps > 12) {
      Field_TryJumpForward();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

/// Reveal in CrossboneIsleKeys: the hidden Red Key appears on its
/// cell.
static void CrossboneIsleKeys_ShowRedKey(void) {
  s32 keyActorId = ACTOR_RED_KEY;
  s32 x = PIXELS(408);
  s32 z = PIXELS(776);

  MapGrid_CopyCells(25, 45, 1, 2, 25, 48);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_RED_KEY) == 0)
    Import_ActorSetPosition(keyActorId, x, z);
  Event_WaitFrames(1);
}

/// Reveal wears off in CrossboneIsleKeys: the Red Key's cell is
/// hidden again.
static void CrossboneIsleKeys_HideRedKey(void) {
  MapGrid_CopyCells(24, 48, 1, 2, 25, 48);
  Actor_SetPosition(ACTOR_RED_KEY, PIXELS(8), PIXELS(8));
}

/// Collects the selected key, hides its actor and remembers it was
/// taken.
static inline void CrossboneIsleKeys_CollectKey(s32 actorId, s32 itemId,
                                                s32 collectedFlag) {
  ItemDisplay_ShowItem(itemId, 3);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  PartyInventory_GiveItem(itemId, 0);
  Import_ActorSetPosition(actorId, 0, 0);
  GameFlag_Set(collectedFlag);
}

/// Catch on CrossboneIsleKeys's first Blue Key: the party gets it.
static void CrossboneIsleKeys_CatchFirstBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY);
}

/// Catch on CrossboneIsleKeys's second Blue Key: the party gets it.
static void CrossboneIsleKeys_CatchSecondBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1);
}

/// Catch on CrossboneIsleKeys's third Blue Key: the party gets it.
static void CrossboneIsleKeys_CatchThirdBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2);
}

/// Catch on CrossboneIsleKeys's fourth Blue Key: the party gets it.
static void CrossboneIsleKeys_CatchFourthBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3);
}

/// Catch on CrossboneIsleKeys's revealed Red Key: the party gets it.
static void CrossboneIsleKeys_CatchRedKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_RED_KEY, ITEM_RED_KEY,
                               FLAG_CROSSBONE_ISLE_RED_KEY);
}

/// Starts the first chase when an uncloaked leader enters its area.
static void CrossboneIsleKeys_CheckFirstPursuerTrigger(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct FieldWork *work = gFieldWork;

  if (!Event_GetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) &&
      gSaveBlock.cloaked == 0 && gSaveBlock.haltedActor != ACTOR_FIRST_STATUE &&
      (u32)(column - 21) <= 2 && row >= 10 && row <= 11) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
    work->raisedTrigger = RAISED_FIRST_CHASE;
  }
}

/// Raised in CrossboneIsleStatues (91): the first pursuer follows the
/// leader, then goes back home.
static void CrossboneIsleKeys_DodgeFirstPursuer(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  Actor_SetSpeed(ACTOR_FIRST_STATUE, PIXELS(19) / 10, PIXELS(19) / 20);
  Event_PlaySong(SE_THUMP);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_FIRST_STATUE, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_FIRST_STATUE);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
  Event_WaitFrames(4);
  Event_PlaySong(SE_THUMP);
  Actor_SetDestinationOffset(ACTOR_FIRST_STATUE, 0, CROSSBONE_PURSUER_STEP_Z);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_FIRST_STATUE, 360, 152);
  Actor_WaitForMoveById(ACTOR_FIRST_STATUE);
  Event_End();
  GameFlag_Clear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/// Starts the second chase when the leader enters column ten.
static void CrossboneIsleKeys_CheckSecondPursuerTrigger(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct FieldWork *work = gFieldWork;

  if (Event_GetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) == 0 &&
      gSaveBlock.cloaked == 0 &&
      gSaveBlock.haltedActor != ACTOR_SECOND_STATUE && column == 10 &&
      (u32)(row - 16) <= 2) {
    Event_SetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
    MapLocalState_StoreStep(&work->raisedTrigger, RAISED_SECOND_CHASE);
  }
}

/// Raised in CrossboneIsleStatues (92): the second pursuer follows the
/// leader, then goes back home.
static void CrossboneIsleKeys_DodgeSecondPursuer(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  Actor_SetSpeed(ACTOR_SECOND_STATUE, PIXELS(17) / 10, PIXELS(17) / 20);
  Event_PlaySong(SE_THUMP);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_SECOND_STATUE, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_SECOND_STATUE);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  Event_PlaySong(SE_THUMP);
  Event_WaitFrames(4);
  Actor_SetDestinationOffset(ACTOR_SECOND_STATUE, 0, 16);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_SECOND_STATUE, 168, 264);
  Actor_WaitForMoveById(ACTOR_SECOND_STATUE);
  Event_End();
  GameFlag_Clear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/// Moves a pursuer to the leader column and remembers its last column.
static void CrossboneIsleKeys_TrackPursuerColumn(s32 pursuerIndex) {

  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  s32 pursuerId = pursuerIndex + ACTOR_THIRD_STATUE;

  if (gSaveBlock.haltedActor == pursuerId)
    return;
  if (column == gCrossboneIsleKeys_SlotColumns[pursuerIndex])
    return;

  Actor_SetSpeedParameters(pursuerId, PIXELS(9) / 2, PIXELS(9) / 4);
  Sound_PlaySong(SE_THUMP);
  Actor_ResetAndSetPosition(pursuerId, (column << 4) + 8, 360);

  gCrossboneIsleKeys_SlotColumns[pursuerIndex] = column;

  if (row <= 22) {
    Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0, 8);
  }
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
}

/// Touching step trigger 53 in CrossboneIsleStatues: the third pursuer keeps
/// to the leader's column.
static void CrossboneIsleKeys_ThirdStatueShadowsLeader(void) {
  CrossboneIsleKeys_TrackPursuerColumn(0);
}

/// Touching step trigger 54 in CrossboneIsleStatues: the fourth pursuer
/// keeps to the leader's column.
static void CrossboneIsleKeys_FourthStatueShadowsLeader(void) {
  CrossboneIsleKeys_TrackPursuerColumn(1);
}

/// Unused: tracks the fifth pursuer's column; nothing calls it.
UNUSED static void CrossboneIsleKeys_TrackFifthPursuerColumn(void) {
  CrossboneIsleKeys_TrackPursuerColumn(2);
}

/// Touching step trigger 61 in CrossboneIsleStatues: the third pursuer
/// follows the leader, then goes back home.
static void CrossboneIsleKeys_DodgeThirdPursuer(void) {
  struct Actor *leader;

  if (gSaveBlock.haltedActor != ACTOR_THIRD_STATUE) {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    Actor_SetSpeed(ACTOR_THIRD_STATUE, PIXELS(17) / 10, PIXELS(17) / 20);
    Event_PlaySong(SE_THUMP);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_THIRD_STATUE, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_THIRD_STATUE);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
    Event_WaitFrames(4);
    Event_PlaySong(SE_THUMP);
    Actor_SetDestinationOffset(ACTOR_THIRD_STATUE, 0, 16);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_SetDestination(ACTOR_THIRD_STATUE, 264, 360);
    Actor_WaitForMoveById(ACTOR_THIRD_STATUE);
    Event_WaitFrames(10);
    Event_End();
  }
}

/// Touching step trigger 62 in CrossboneIsleStatues: the fourth pursuer lines
/// up and goes after the leader.
static void CrossboneIsleKeys_DodgeFourthPursuer(void) {
  struct Actor *leader;
  s32 leaderColumn;

  if (gSaveBlock.haltedActor != ACTOR_FOURTH_STATUE) {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    Actor_SetSpeedParameters(ACTOR_FOURTH_STATUE, PIXELS(17) / 10,
                             PIXELS(17) / 20);
    Sound_PlaySong(SE_THUMP);
    leaderColumn = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed / PIXELS(16);
    if (leaderColumn >
        Actor_GetById(ACTOR_FOURTH_STATUE)->x.fixed / PIXELS(16)) {
      Actor_OffsetPositionAndResetMotion(ACTOR_FOURTH_STATUE, 8, 0);
    }
    leaderColumn = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed / PIXELS(16);
    if (leaderColumn <
        Actor_GetById(ACTOR_FOURTH_STATUE)->x.fixed / PIXELS(16)) {
      FieldActor_SetDestinationOffset(ACTOR_FOURTH_STATUE, -8, 0);
    }
    Actor_WaitForMoveById(ACTOR_FOURTH_STATUE);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(ACTOR_FOURTH_STATUE, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_FOURTH_STATUE);
    Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0,
                                       CROSSBONE_LEADER_STEP_Z);
    Event_WaitFrames(4);
    Sound_PlaySong(SE_THUMP);
    Actor_OffsetPositionAndResetMotion(ACTOR_FOURTH_STATUE, 0, 16);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_ResetAndSetPosition(ACTOR_FOURTH_STATUE, 344, 360);
    Actor_WaitForMoveById(ACTOR_FOURTH_STATUE);
    Event_WaitFrames(10);
    Event_End();
  }
}

/// Starts the fifth chase when the leader enters column nineteen.
static void CrossboneIsleKeys_CheckFifthPursuerTrigger(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct FieldWork *work = gFieldWork;
  struct SaveBlock *state = &gSaveBlock;

  if (state->haltedActor != ACTOR_FIFTH_STATUE &&
      Event_GetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) == 0 &&
      state->cloaked == 0 && column == 19 && (u32)(row - 15) <= 1) {
    Event_SetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
    MapLocalState_StoreStep(&work->raisedTrigger, RAISED_FIFTH_CHASE);
  }
}

/// Raised in CrossboneIsleStatues (96): the fifth pursuer follows the
/// leader, then goes back home.
static void CrossboneIsleKeys_DodgeFifthPursuer(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  Actor_SetSpeed(ACTOR_FIFTH_STATUE, PIXELS(17) / 10, PIXELS(17) / 20);
  Event_PlaySong(SE_THUMP);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_FIFTH_STATUE, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_FIFTH_STATUE);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  Event_PlaySong(SE_THUMP);
  Actor_SetDestinationOffset(ACTOR_FIFTH_STATUE, 0, 16);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_FIFTH_STATUE, 312, 232);
  Actor_WaitForMoveById(ACTOR_FIFTH_STATUE);
  Event_End();
  GameFlag_Clear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/// Reveal in CrossboneIsleStatues: the translucent layer over the room
/// vanishes while it lasts.
static void CrossboneIsleKeys_CastRevealInStatueRoom(void) {
  u32 coefficient = 208;

  coefficient <<= 4;
  REG_BLDALPHA = coefficient;
}

/// Reveal wears off in CrossboneIsleStatues: the translucent layer
/// returns.
static void CrossboneIsleKeys_RevealEndsInStatueRoom(void) {
  u16 value = 0x0607;

  REG_BLDALPHA = value;
}

/// At CrossboneIsleBane's ladder by the second pillar (step trigger 60):
/// climb up or down, look ahead to the east, or push the first pillar from the
/// floor to the west.
static void CrossboneIsleKeys_UseLadder(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  u16 facing = leader->facing;

  if (facing == FACING_NORTH) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        MapGrid_CopyCellAttributes(46, 43, 1, 1, destinationColumn,
                                   destinationRow);
      }
    }
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_BANE_LOOKED_EAST);
    Field_ClimbUp();
  } else if (facing == FACING_SOUTH) {
    Field_ClimbDown();
  } else if (facing == FACING_EAST) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        MapGrid_CopyCellAttributes(58, 36, 1, 1, destinationColumn,
                                   destinationRow);
      }
    }
    GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_LOOKED_EAST);
    Field_TryJumpForward();
  } else if (facing == FACING_WEST) {
    if (leader->y.fixed == 0) {
      CrossboneIsleKeys_PushFirstPillar();
    } else {
      Field_TryJumpForward();
    }
  }
}

UNUSED static void CrossboneIsleKeys_UnusedCallback(void) {}

/// Frost on CrossboneIsleBane's puddle: it freezes into a pillar of
/// ice the party can stand on.
static void CrossboneIsleKeys_FreezePuddle(void) {
  struct Actor *puddle = Actor_GetById(ACTOR_PUDDLE);

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 40, 55);
  if (puddle != 0) {
    u8 *motionFlags = &Actor_GetById(ACTOR_PUDDLE)->motionFlags;
    u8 *priorityFlags = &puddle->priorityFlags;

    *motionFlags = 0;
    *priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN);
}

/// Walking into CrossboneIsleBane's first pillar from the ladder side
/// (step trigger 70): facing east, look ahead; otherwise push the pillar.
static void CrossboneIsleKeys_LookAheadOrPushFirstPillar(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == 0) {
    Field_TryJumpForward();
  } else {
    CrossboneIsleKeys_PushFirstPillar();
  }
}

/// Restores the first pillar row before applying its current footprint.
static void CrossboneIsleKeys_RestoreFirstPillarTerrain(void) {
  MapGrid_CopyCellAttributes(57, 42, 1, 1, 40, 42);
  MapGrid_CopyCellAttributes(57, 42, 1, 1, 41, 42);
  MapGrid_CopyCellAttributes(58, 42, 1, 1, 42, 42);
  MapGrid_CopyCellAttributes(62, 37, 3, 1, 37, 42);

  Actor_GetById(ACTOR_FIRST_PILLAR)->motionFlags = ACTOR_FOLLOWS_TERRAIN;
}

/// Restores support under the first pillar at the eastern cell.
static void CrossboneIsleKeys_RestoreEastUnderfootCell(void) {

  MapGrid_CopyCellAttributes(58, 41, 1, 1, 42, 42);
  Actor_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// Restores support under the first pillar at the middle cell.
static void CrossboneIsleKeys_RestoreMiddleUnderfootCell(void) {

  MapGrid_CopyCellAttributes(44, 42, 1, 1, 41, 42);
  Actor_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// Restores support under the first pillar at the western cell.
static void CrossboneIsleKeys_RestoreWestUnderfootCell(void) {

  MapGrid_CopyCellAttributes(39, 42, 1, 1, 40, 42);
  Actor_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// Move on CrossboneIsleBane's first pillar: its support cells are
/// rebuilt from its height and column.
static void CrossboneIsleKeys_MoveFirstPillar(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FIRST_PILLAR);
  s32 x = actor->x.fixed;
  s32 height = actor->y.fixed;
  s32 column = x / PIXELS(16);
  u8 lowered;

  if (height == 0) {
    Actor_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  CrossboneIsleKeys_RestoreFirstPillarTerrain();
  lowered = 0;
  Actor_GetById(ACTOR_FIRST_PILLAR)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (column == 40) {
    CrossboneIsleKeys_RestoreWestUnderfootCell();
  } else if (column == 42) {
    CrossboneIsleKeys_RestoreEastUnderfootCell();
  } else if (column == 41) {
    CrossboneIsleKeys_RestoreMiddleUnderfootCell();
  } else if (column == 39) {
    LiftPillar(column, lowered);
  } else if (column == 38) {
    LiftPillar(column, lowered);
  } else if (column == 37) {
    LiftPillar(column, lowered);
  }
}

/// Pushing CrossboneIsleBane's first pillar (step trigger 51); standing at
/// the ladder in column 38, the leader climbs instead.
static void CrossboneIsleKeys_PushFirstPillar(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *pillar = Actor_GetById(ACTOR_FIRST_PILLAR);
  s32 leaderColumn = leader->x.fixed / PIXELS(16);
  s32 pillarColumn = pillar->x.fixed / PIXELS(16);

  if (leaderColumn == 38 && pillarColumn != 38) {
    u16 facing = leader->facing;

    if (facing == FACING_NORTH) {
      Field_ClimbUp();
      return;
    }
    if (facing == FACING_SOUTH) {
      Field_ClimbDown();
      return;
    }
  }

  CrossboneIsleKeys_RestoreFirstPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveFirstPillar();
}

/// Leaves the chest where the second pillar has reached the ladder: the chest's
/// actor is hidden, its examine trigger takes its place and the ladder cell is
/// restored.
static void CrossboneIsleKeys_ReleaseSecondPillarChest(void) {
  Task_Remove(CrossboneIsleKeys_FollowSecondPillar);
  Actor_SetPosition(ACTOR_SECOND_PILLAR_CHEST, PIXELS(0), PIXELS(0));
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_LOOKED_EAST) != 0) {
    MapGrid_CopyCellAttributes(58, 36, 1, 1, 45, 43);
  } else {
    MapGrid_CopyCellAttributes(46, 43, 1, 1, 45, 43);
  }
  CrossboneIsleKeys_PositionSecondPillarMarker();
  GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER);
}

/// Move on CrossboneIsleBane's second pillar: the chest on top of it stays
/// at the ladder cell once the pillar reaches it.
static void CrossboneIsleKeys_MoveSecondPillar(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_SECOND_PILLAR);
  s32 column = pillar->x.fixed / PIXELS(16);
  s32 row = pillar->z.fixed / PIXELS(16);

  if (column == 45 && row == 43) {
    CrossboneIsleKeys_ReleaseSecondPillarChest();
  }
}

/// Pushing CrossboneIsleBane's second pillar (step trigger 52); at the ladder,
/// the chest on top of it stays there.
static void CrossboneIsleKeys_PushSecondPillar(void) {
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveSecondPillar();
}

/// Restores the terrain at both ends of the third pillar row.
static void CrossboneIsleKeys_RestoreThirdPillarTerrain(void) {

  MapGrid_CopyCellAttributes(38, 56, 1, 1, 38, 55);
  MapGrid_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

/// Restores the third pillar support at the western end.
static void CrossboneIsleKeys_RestoreThirdPillarWestCell(void) {

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 38, 55);
  Actor_GetById(ACTOR_THIRD_PILLAR)->motionFlags = 0;
}

/// Restores the third pillar support at the eastern end.
static void CrossboneIsleKeys_RestoreThirdPillarEastCell(void) {

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 42, 55);
  Actor_GetById(ACTOR_THIRD_PILLAR)->motionFlags = 0;
}

/// Move on CrossboneIsleBane's third pillar: its support is rebuilt
/// from its height and destination.
static void CrossboneIsleKeys_MoveThirdPillar(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_THIRD_PILLAR);
  s32 heightCells = pillar->y.fixed / PIXELS(16);
  s32 column = pillar->x.fixed / PIXELS(16);
  s32 row = pillar->z.fixed / PIXELS(16);

  if (!Event_GetFlag(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED) &&
      heightCells <= 2) {
    CrossboneIsleKeys_RestoreThirdPillarWestCell();
    GameFlag_Set(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED);
  }
  if (row == 55) {
    if (column == 42) {
      CrossboneIsleKeys_RestoreThirdPillarEastCell();
    }
    if (column == 38) {
      CrossboneIsleKeys_RestoreThirdPillarWestCell();
    }
  } else {
    CrossboneIsleKeys_RestoreThirdPillarTerrain();
  }
}

/// Pushing CrossboneIsleBane's third pillar (step trigger 50).
static void CrossboneIsleKeys_PushThirdPillar(void) {
  CrossboneIsleKeys_RestoreThirdPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveThirdPillar();
}

/// Walking into CrossboneIsleBane's third pillar (step trigger 54): facing
/// south, the rising sequence plays; otherwise the pillar is pushed.
static void CrossboneIsleKeys_TouchThirdPillar(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == FACING_SOUTH) {
    FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
  } else {
    CrossboneIsleKeys_PushThirdPillar();
  }
}

/// Clears the support row before repainting the fourth and fifth pillars.
static void CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain(void) {

  MapGrid_CopyCellAttributes(48, 55, 1, 1, 49, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 50, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 51, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

/// Places support cells beneath the fourth and fifth pillars.
static void CrossboneIsleKeys_RepaintFourthAndFifthPillarCells(void) {
  struct Actor *pillar;
  s32 column;

  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();

  pillar = Actor_GetById(ACTOR_FOURTH_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  MapGrid_CopyCellAttributes(53, 55, 1, 1, column, 55);
  pillar = Actor_GetById(ACTOR_FIFTH_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  MapGrid_CopyCellAttributes(53, 55, 1, 1, column, 55);
}

/// Move on CrossboneIsleBane's fourth pillar: the support row it
/// shares with the fifth is refreshed.
static void CrossboneIsleKeys_MoveFourthPillar(void) {
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
}

/// Pushing CrossboneIsleBane's fourth pillar (step trigger 53).
static void CrossboneIsleKeys_PushFourthPillar(void) {
  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveFourthPillar();
}

/// Move on CrossboneIsleBane's fifth pillar: the support row it shares
/// with the fourth is refreshed.
static void CrossboneIsleKeys_MoveFifthPillar(void) {
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
}

/// Unused: a task that moves the fifth pillar and refreshes its terrain;
/// nothing calls it.
UNUSED static void CrossboneIsleKeys_PushFifthPillar(void) {
  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveFifthPillar();
}

/// Keeps the chest on top of the second pillar as the pillar moves.
static void CrossboneIsleKeys_FollowSecondPillar(void) {
  struct Actor *target = Actor_GetById(ACTOR_SECOND_PILLAR_CHEST);
  struct Actor *source = Actor_GetById(ACTOR_SECOND_PILLAR);

  target->y.fixed = PIXELS(32);
  target->x.fixed = source->x.fixed;
  target->z.fixed = source->z.fixed + PIXELS(1);
}

/// Puts the chest's examine trigger 107 where the second pillar stands.
static void CrossboneIsleKeys_PositionSecondPillarMarker(void) {
  struct Actor *frame = Actor_GetById(ACTOR_SECOND_PILLAR);

  TreasureSpot_SetActorPosition(107, frame->x.fixed,
                                frame->z.fixed + PIXELS(1));
}

/// Chooses pillar drawing priorities from the leader height and row.
static void CrossboneIsleKeys_UpdatePillarPriorities(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 priority;

  if (leader->y.fixed > PIXELS(16)) {
    priority = ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = priority;
    if (Actor_GetById(ACTOR_THIRD_PILLAR)->y.fixed == 0)
      Actor_GetById(ACTOR_THIRD_PILLAR)->priorityFlags = priority;
    Actor_GetById(ACTOR_FOURTH_PILLAR)->priorityFlags = priority;
  } else {
    if (Actor_GetById(ACTOR_THIRD_PILLAR)->y.fixed == 0 &&
        Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed / PIXELS(16) > 56) {
      Actor_SetSpritePriority(ACTOR_THIRD_PILLAR, 3);
    } else {
      Actor_SetSpritePriority(ACTOR_THIRD_PILLAR, 1);
      Actor_GetById(ACTOR_THIRD_PILLAR)->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC;
    }
    priority = 0;
    Actor_GetById(ACTOR_FOURTH_PILLAR)->priorityFlags = priority;
  }
  Actor_GetById(ACTOR_FIFTH_PILLAR)->priorityFlags = priority;
}

/// Sets up whichever of the three rooms the party enters: the keys and doors
/// of CrossboneIsleKeys, the pursuers of CrossboneIsleStatues,
/// or the pillars and Bane of CrossboneIsleBane.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleKeys) {
    CrossboneIsleKeys_InitializeKeys();
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleStatues) {
    CrossboneIsleKeys_InitializeStatues();
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleBane) {
    CrossboneIsleKeys_InitializeBane();
  }
  return 0;
}

/// Unused: the Blue Keys still lying about would have flashed their icons. In
/// CrossboneIsleKeys they would have shown their item icons one after another,
/// every 16 frames.
///
/// Nothing adds it as a task, so the keys never flash.
UNUSED static void CrossboneIsleKeys_UnusedUpdateKeyIcons(void) {
  if (++gCrossboneIsleKeys_IconTimer > 16) {
    gCrossboneIsleKeys_IconTimer = 0;
  }
  switch (gCrossboneIsleKeys_IconTimer) {
  case 12:
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
      Actor_SetPosition(ACTOR_FIRST_BLUE_KEY, PIXELS(232), PIXELS(872));
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
      Actor_SetPosition(ACTOR_SECOND_BLUE_KEY, PIXELS(296), PIXELS(824));
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
      Actor_SetPosition(ACTOR_THIRD_BLUE_KEY, PIXELS(328), PIXELS(760));
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
      Actor_SetPosition(ACTOR_FOURTH_BLUE_KEY, PIXELS(360), PIXELS(872));
    }
    break;
  case 10:
    Colosso_ShowItemIcon(ACTOR_FIRST_BLUE_KEY, 0);
    break;
  case 8:
    Colosso_ShowItemIcon(ACTOR_SECOND_BLUE_KEY, 0);
    break;
  case 6:
    Colosso_ShowItemIcon(ACTOR_THIRD_BLUE_KEY, 0);
    break;
  case 4:
    Colosso_ShowItemIcon(ACTOR_FOURTH_BLUE_KEY, 0);
    break;
  case 2:
    Colosso_ShowItemIcon(ACTOR_RED_KEY, 1);
    break;
  }
}

#include "common/colosso_item_icon.h"

/// Restores collected keys and already opened passages.
static void CrossboneIsleKeys_InitializeKeys(void) {
  WaitFrames(1);
  Colosso_ShowItemIcon(ACTOR_RED_KEY, ITEM_RED_KEY);
  Colosso_ShowItemIcon(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
    Actor_SetPosition(ACTOR_FIRST_BLUE_KEY, PIXELS(232), PIXELS(872));
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
    Actor_SetPosition(ACTOR_SECOND_BLUE_KEY, PIXELS(296), PIXELS(824));
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
    Actor_SetPosition(ACTOR_THIRD_BLUE_KEY, PIXELS(328), PIXELS(760));
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
    Actor_SetPosition(ACTOR_FOURTH_BLUE_KEY, PIXELS(360), PIXELS(872));
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(0);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 1) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(1);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 2) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(2);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 3) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(3);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) != 0) {
    CrossboneIsleKeys_OpenRedDoor(0);
  }
}

/// Configures the pursuers, their chase triggers and blending.
static void CrossboneIsleKeys_InitializeStatues(void) {
  Actor_GetById(ACTOR_FIRST_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_GetById(ACTOR_SECOND_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_GetById(ACTOR_THIRD_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_GetById(ACTOR_FOURTH_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_GetById(ACTOR_FIRST_STATUE)->scaleX = 7 * Q16_ONE / 10;
  Actor_GetById(ACTOR_SECOND_STATUE)->scaleX = 7 * Q16_ONE / 10;
  Actor_GetById(ACTOR_THIRD_STATUE)->scaleX = 7 * Q16_ONE / 10;
  Actor_GetById(ACTOR_FOURTH_STATUE)->scaleX = 7 * Q16_ONE / 10;
  Actor_GetById(ACTOR_FIFTH_STATUE)->scaleX = 7 * Q16_ONE / 10;
  Task_AddOrUpdateFrame(CrossboneIsleKeys_CheckFifthPursuerTrigger,
                        TASK_PASS_FRAME);
  Task_AddOrUpdateFrame(CrossboneIsleKeys_CheckSecondPursuerTrigger,
                        TASK_PASS_FRAME);
  Task_AddFrame(CrossboneIsleKeys_CheckFirstPursuerTrigger, TASK_PASS_FRAME);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(7, 6), &REG_BLDALPHA);
}

/// Restores movable pillars, terrain and the Djinni escape progress.
static void CrossboneIsleKeys_InitializeBane(void) {
  struct Actor *djinni;

  // Put the chest back on the second pillar and rebuild all pillar support
  // cells.
  Actor_GetById(ACTOR_SECOND_PILLAR_CHEST)->motionFlags = 0;
  Task_AddOrUpdateFrame(CrossboneIsleKeys_FollowSecondPillar, TASK_PASS_FRAME);
  Task_AddFrame(CrossboneIsleKeys_UpdatePillarPriorities, TASK_PASS_FRAME);
  TreasureSpot_SetPosition(107, 0, 0);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DEMON_AXE) != 0) {
    Import_ActorSetAnimation(ACTOR_SECOND_PILLAR_CHEST, 2);
  }
  CrossboneIsleKeys_MoveFirstPillar();
  CrossboneIsleKeys_MoveSecondPillar();
  CrossboneIsleKeys_MoveThirdPillar();
  CrossboneIsleKeys_MoveFourthPillar();
  CrossboneIsleKeys_MoveFifthPillar();
#if TBS_PATCH_1
  Actor_SetSpritePriority(ACTOR_FIRST_PILLAR, 3);
#endif
  Actor_GetById(ACTOR_FOURTH_PILLAR)->motionFlags = 0;
  Actor_GetById(ACTOR_FIFTH_PILLAR)->motionFlags = 0;
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN) != 0) {
    CrossboneIsleKeys_FreezePuddle();
    Import_ActorSetAnimation(ACTOR_PUDDLE, 5);
  }

  // Restore the surviving Djinni at the last completed escape stage.
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
      Actor_SetPosition(ACTOR_BANE, PIXELS(856), PIXELS(824));
      djinni = Actor_GetById(ACTOR_BANE);
#ifdef UBFIX
      djinni->update = CrossboneIsleKeys_FaceBaneTowardLeader;
#else
      djinni->update =
          (void (*)(struct Actor *))CrossboneIsleKeys_FaceBaneTowardLeader;
#endif
    } else if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0) {
      Actor_SetPosition(ACTOR_BANE, PIXELS(888), PIXELS(664));
      djinni = Actor_GetById(ACTOR_BANE);
      djinni->sprite->rotation = 0;
      Actor_SetAnimationSpeed(djinni, 16);
    } else if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
      Actor_SetPosition(ACTOR_BANE, PIXELS(584), PIXELS(680));
    } else {
      Actor_SetPosition(ACTOR_BANE, PIXELS(584), PIXELS(664));
    }
  }
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gCrossboneIsleKeys_BaneSquashScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(64),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gCrossboneIsleKeys_BaneWalkTargets[] = {
    888, 856,
    872, 856,
    856, 856,
    840, 856,
    824, 856,
    824, 840,
    824, 824,
    840, 824,
    856, 824,
    872, 824,
    888, 824,
    888, 840,
};

struct SceneEntrance gCrossboneIsleKeysEntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleKeysRoomEntrances[] = {
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleStatuesEntrances[] = {
    { CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleBaneEntrances[] = {
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsleKeysExits[] = {
    SCENE_EXITS(CrossboneIsleKeys),
    SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_WEST),
    SCENE_EXITS(CrossboneIsleStatues),
    SCENE_EXIT(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_WEST),
    SCENE_EXITS(CrossboneIsleBane),
    SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_WEST),
    SCENE_EXITS_END,
};

struct ActorTemplate gCrossboneIsleKeysActorTemplatesOther[] = {
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleKeysRoomActorTemplates[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleStatuesActorTemplates[] = {
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleBaneActorTemplates[] = {
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(616), 0, PIXELS(680), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(856), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(888), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(888), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(888), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(760), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    // Bane, until it is caught
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_BANE), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gCrossboneIsleKeysTriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gCrossboneIsleKeysRoomTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_ShowRedKey) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HideRedKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FIRST_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchFirstBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_SECOND_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchSecondBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_THIRD_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchThirdBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FOURTH_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchFourthBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_RED_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchRedKey) },
    { TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UseRedKey) },
    { TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenFirstBlueDoor) },
    { TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenSecondBlueDoor) },
    { TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenThirdBlueDoor) },
    { TRIGGER_STEP, 54, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenFourthBlueDoor) },
    // Unused: the plain step trigger rows above answer codes 53 and 54 first.
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UnusedJumpPastBlueDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UnusedJumpPastBlueDoor) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_KEYS_333_COINS, FIND_COINS(333) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_KEYS_FAIRY_RING, FIND_ITEM(ITEM_FAIRY_RING) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_KEYS_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_KEYS_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { TRIGGERS_END },
};

struct Trigger gCrossboneIsleStatuesTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST },
    { TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_ThirdStatueShadowsLeader) },
    { TRIGGER_STEP, 54, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_FourthStatueShadowsLeader) },
    { TRIGGER_STEP, 61, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_DodgeThirdPursuer) },
    { TRIGGER_STEP, 62, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_DodgeFourthPursuer) },
    { TRIGGER_RAISED, RAISED_FIFTH_CHASE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_DodgeFifthPursuer) },
    { TRIGGER_RAISED, RAISED_SECOND_CHASE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_DodgeSecondPursuer) },
    { TRIGGER_RAISED, RAISED_FIRST_CHASE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_DodgeFirstPursuer) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CastRevealInStatueRoom) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_RevealEndsInStatueRoom) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIRST_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_SECOND_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_THIRD_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FOURTH_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIFTH_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_STATUES_444_COINS, FIND_COINS(444) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_STATUES_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_STATUES_STORM_GEAR, FIND_ITEM(ITEM_STORM_GEAR) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_STATUES_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gCrossboneIsleBaneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 50, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_PushThirdPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 51, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_PushFirstPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 52, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_PushSecondPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 53, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_PushFourthPillar)},
#if TBS_PATCH_1
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 61, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_PushFirstPillar)},
#endif
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 54, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_TouchThirdPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 60, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_UseLadder)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 70, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_LookAheadOrPushFirstPillar)},
    {TRIGGER_STEP, 20, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneFirstEscape)},
    {TRIGGER_STEP, 21, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneSecondEscape)},
    {TRIGGER_STEP, 22, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneLastEscape)},
    {TRIGGER_STEP, 30, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 31, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 32, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 33, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 34, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 35, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 36, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 37, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 38, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 39, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 40, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, 41, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_TALK_HALTED, ACTOR_BANE, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_BaneJoins)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_BANE, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_HaltBane)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE,
     FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN,
     EVENT(CrossboneIsleKeys_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_SCENE_TEMP_01,
     EVENT(CrossboneIsleKeys_MoveFirstPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR,
     FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER,
     EVENT(CrossboneIsleKeys_MoveSecondPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR, FLAG_SCENE_TEMP_03,
     EVENT(CrossboneIsleKeys_MoveThirdPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR, FLAG_SCENE_TEMP_04,
     EVENT(CrossboneIsleKeys_MoveFourthPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_PILLAR, FLAG_SCENE_TEMP_05,
     EVENT(CrossboneIsleKeys_MoveFifthPillar)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_CROSSBONE_ISLE_BANE_666_COINS,
     FIND_COINS(666)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 105,
     FLAG_CROSSBONE_ISLE_BANE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 106, FLAG_CROSSBONE_ISLE_BANE_ELIXIR,
     FIND_ITEM(ITEM_ELIXIR)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 107, FLAG_CROSSBONE_ISLE_DEMON_AXE,
     FIND_ITEM(ITEM_DEMON_AXE)},
    {TRIGGERS_END},
};

u32 gCrossboneIsleKeys_SlotColumns[] = {
    16,
    21,
    0,
};

s32 gCrossboneIsleKeys_IconTimer = 0;
