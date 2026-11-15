/// @file
/// The cave by the Shaman Village: overgrowth Whirlwind clears, a pillar Move
/// pushes into a gap, rocks Lift raises, Eddy the Mercury Djinni who runs from
/// the party, and in a linked game the three gladiators from Colosso.
#include "djinn_ids.h"
#include "edition.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// Set once Move has pushed the brown pillar into the gap at column 38, the
/// shortcut.
#define FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED 0x9a8
/// Set once Whirlwind has blown away the overgrowth (ACTOR_OVERGROWTH).
#define FLAG_SHAMAN_VILLAGE_CAVE_OVERGROWTH_CLEARED 0x9a9
/// Set once Whirlwind has blown away the lower overgrowth
/// (ACTOR_LOWER_OVERGROWTH).
#define FLAG_SHAMAN_VILLAGE_CAVE_LOWER_OVERGROWTH_CLEARED 0x9aa
/// Set as the gladiators from Colosso find the party: "I found Isaac!"
#define FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS 0xa70
/// Set while the party has lost to the gladiators, who wait for a rematch.
#define FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS 0xa71
/// Set once the party has beaten the gladiators.
#define FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS 0xa72
/// Set while the Golden Shirt the beaten gladiators leave lies in the cave.
#define FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT 0xa73

/// Where the gladiators' battle returns the party: beaten them, or lost.
enum ShamanVillageCaveEntrance {
  SHAMAN_VILLAGE_CAVE_AFTER_BEATING_GLADIATORS = 20,
  SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS = 21
};

void Message_CloseFinishedWindows(void);

/// The cave's actors, numbering the rows of gActorPlacements.
enum ShamanVillageCaveActorId {
  /// The ivy Whirlwind blows off the wall (SPRITE_IVY), and its twin by the
  /// lower passage.
  ACTOR_OVERGROWTH = ACTOR_FIRST_PLACED,
  ACTOR_HIDDEN_BOULDER,
  ACTOR_LOWER_OVERGROWTH,
  /// Four round boulders across the lower floor (SPRITE_ROUND_BOULDER).
  ACTOR_FIRST_BOULDER,
  ACTOR_LAST_BOULDER = ACTOR_FIRST_BOULDER + 3,
  /// The pillar Move pushes (SPRITE_STONE_PILLAR).
  ACTOR_PILLAR,
  /// The puddle Frost freezes (SPRITE_PUDDLE).
  ACTOR_PUDDLE,
  /// Eddy, the Mercury Djinni, placed until it joins
  /// (FLAG_DJINNI(DJINNI_EDDY)).
  ACTOR_EDDY,
  ACTOR_KRADEN,
  /// Colosso's three gladiators, come to call Isaac a cheat: "The name's
  /// Satrage.", "And I'm Azart!", "And don't forget Navampa!"
  ACTOR_SATRAGE,
  ACTOR_AZART,
  ACTOR_NAVAMPA,
  /// The Golden Shirt the gladiators leave (ShamanVillageCave_TakeGoldenShirt).
  ACTOR_GOLDEN_SHIRT
};

extern u32 gShamanVillageCave_GladiatorLeaves[];

extern s32 gShamanVillageCave_GladiatorFollowsOut[];

void Field_TryJumpForward();

extern s32 gShamanVillageCave_OvergrowthRests[];
extern u32 gShamanVillageCave_Settle[];

s32 Event_PickUpItem();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Whirlwind blows away one of the two overgrowths for good,
/// opening the cell behind it (FLAG_SHAMAN_VILLAGE_CAVE_OVERGROWTH_CLEARED and
/// FLAG_SHAMAN_VILLAGE_CAVE_LOWER_OVERGROWTH_CLEARED).
void ShamanVillageCave_BlowAwayOvergrowth(s32 unused, s32 overgrowth) {
  if (overgrowth != ACTOR_OVERGROWTH)
    goto notFirst;
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_OVERGROWTH_CLEARED);
  MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, overgrowth);
notFirst:
  if (overgrowth != ACTOR_LOWER_OVERGROWTH)
    goto done;
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOWER_OVERGROWTH_CLEARED);
  MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
done:
  return;
}

void ShamanVillageCave_DropPillarIntoGap(s32 id);

/// The pillar falls until it lands, or for a second, then vanishes into the
/// gap it fills.
void ShamanVillageCave_DropPillarIntoGap(s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 frames = 60;

  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (frames != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    frames--;
  }
  System_Wait(10);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
}

/// Move on the brown pillar: its old cells open first.
void ShamanVillageCave_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(99, 10, 4, 3, 99, 18);
}

/// Pushed two spots to column 38, the pillar drops into the gap there and makes
/// the shortcut (FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED).
void ShamanVillageCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x = FIELD_CELL(object->x.fixed);

  if (x == 38) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED);
    object->priorityFlags = 0;
    ShamanVillageCave_DropPillarIntoGap(id);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, x, 19);
  }
  MapGrid_CopyCellAttributes(99, 6, 4, 3, 99, 18);
}

/// Frost freezes the puddle (actor 16) into a pillar of ice (flag 0x302).
void ShamanVillageCave_FreezePuddle(void) {
  struct Actor *object = Actor_Get(ACTOR_PUDDLE);

  Event_SetFlag(FLAG_AREA_TEMP_02);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
}

/// Eddy, the Mercury Djinni (actor 17), runs from the leader, unless the two
/// stand on opposite sides of the boundary between 8-pixel columns 36 and 37.
/// With flag 514 set and the leader in the upper half (z cell 95 or less) it
/// slides down two cells, turns, settles with emote 258 and sets flag 516, only
/// once; otherwise it slides two cells along z, one and then three along x
/// away from the leader with a hop, and back along z.
void ShamanVillageCave_EddyFlees(void) {
  struct Actor *object = Actor_GetById(ACTOR_EDDY);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 leaderX = leader->x.fixed >> 19;
  s32 leaderZ = leader->z.fixed >> 19;
  s32 objectX = object->x.fixed >> 19;
  s32 dx;
  s32 dz;

  if (leaderX <= 36 && objectX > 37)
    return;
  if (leaderX > 37 && objectX <= 36)
    return;
  dx = 1;
  if (leaderX > 36)
    dx = -dx;
  dz = -1;
  if (leaderZ <= 95)
    dz = 1;
  if (dz == 1 && Event_GetFlag(FLAG_SCENE_TEMP_02) != 0) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_04) != 0)
      return;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_Jump(ACTOR_EDDY, 4, 20);
    Actor_SetDestinationOffset(ACTOR_EDDY, 0, 32);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldCamera_MoveToActorById(17, 1);
    FieldActor_StartRepeatedMotion(ACTOR_EDDY, 2);
    Actor_FaceDirection(ACTOR_EDDY, FACING_WEST, 0);
    Actor_SetDestinationOffset(ACTOR_EDDY, -8, 0);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldActor_Jump(ACTOR_EDDY, 4, 20);
    Actor_SetAttachedEffect(ACTOR_EDDY,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(30);
    Actor_GetById(ACTOR_EDDY)->steeringFlags &= ~1;
    Actor_WalkBy(ACTOR_EDDY, 8, 0);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_EDDY, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldEvent_Wait(30);
    Event_End();
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  dz <<= 5;
  FieldActor_Jump(ACTOR_EDDY, 4, 20);
  Actor_SetDestinationOffset(ACTOR_EDDY, 0, dz);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, dx << 4, 0);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, dx * 48, 0);
  object->velocityY = px(8);
  dz = -dz;
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, 0, dz);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Event_End();
}

/// Talking to Eddy, the Mercury Djinni, starts its battle.
void ShamanVillageCave_FightEddy(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_05);
  Event_SetFlag(FLAG_SCENE_TEMP_06);
  Event_RequestDjinnBattle(ACTOR_EDDY, DJINNI_EDDY);
}

s32 ShamanVillageCave_SettleOvergrowth(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->priority = 2;
  object->motionFlags = 0;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Lift raises one of the four rocks (actors 11 to 14), each for good.
void ShamanVillageCave_LiftFirstRock(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  return;
}

void ShamanVillageCave_LiftSecondRock(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_01);
  return;
}

void ShamanVillageCave_LiftThirdRock(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_02);
  return;
}

void ShamanVillageCave_LiftFourthRock(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_03);
  return;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// The Golden Shirt the gladiators leave goes to the party, if there is room.
void ShamanVillageCave_TakeGoldenShirt(void) {
  s32 result;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result = Value3((s32 (*)())Event_PickUpItem, ACTOR_GOLDEN_SHIRT,
                  ITEM_GOLDEN_SHIRT, -1);
  if (result != 0)
    goto taken;
  Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
taken:
  Event_End();
  return;
}

/// The three gladiators stand where their rematch left them.
void ShamanVillageCave_PlaceGladiators(void) {
  s32 angle;

  angle = FACING_SOUTH_SOUTHWEST;
  Actor_SetPositionAndFacing(ACTOR_SATRAGE, px(231), px(199), angle);
  Actor_SetPositionAndFacing(ACTOR_AZART, px(220), px(191), angle);
  Actor_SetPositionAndFacing(ACTOR_NAVAMPA, px(204), px(191), angle);
  return;
}

/// The Golden Shirt lies where the gladiators left it
/// (FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT).
void ShamanVillageCave_PlaceGoldenShirt(void) {
  s32 angle;

  FieldActor_ShowItemIconById(ACTOR_GOLDEN_SHIRT, ITEM_GOLDEN_SHIRT);
  angle = FACING_NORTH;
  Actor_SetPositionAndFacing(ACTOR_GOLDEN_SHIRT, px(212), px(199), angle);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
  return;
}

/// The fighters from Colosso, after a linked game, find the party and challenge
/// it.
void ShamanVillageCave_MeetColossoFans(void) {
  s32 angle;

  if (Event_GetFlag(FLAG_TRANSFERRED_WON_COLOSSO) == 0)
    return;
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_ISAAC)) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS);
  FieldEvent_SetMessageId((s32)MsgIFoundName);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STAND);
  // "I found Isaac!"
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 200, 221);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(5);
  Actor_WalkBy(ACTOR_FELIX, -16, 8);
  FieldActor_LaunchFromLeader(ACTOR_ISAAC, 0, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 8, 16, FACING_NORTH);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 177, 1);
  FieldCamera_WaitForArrival();
  angle = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_SATRAGE, px(199), px(130), angle);
  Actor_WalkTo(ACTOR_SATRAGE, 231, 199);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(ACTOR_AZART, px(199), px(130), angle);
  Actor_WalkTo(ACTOR_AZART, 220, 191);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(ACTOR_NAVAMPA, px(199), px(130), angle);
  Actor_WalkTo(ACTOR_NAVAMPA, 204, 191);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 225, 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(ACTOR_SATRAGE);
  Actor_FaceDirection(ACTOR_SATRAGE, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WaitForMoveById(ACTOR_AZART);
  Actor_FaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WaitForMoveById(ACTOR_NAVAMPA);
  Actor_FaceDirectionAndWait(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST);
  Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 2);
  // "We've been looking for you, Isaac!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Who are you guys?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "The name's Satrage. You took me out in the last round at Colosso."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_AZART, 2);
  FieldEvent_Wait(5);
  // "And I'm Azart!"
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "And don't forget Navampa!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Oh. It's you guys."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_SATRAGE, 2);
  FieldEvent_Wait(5);
  // "Is that any way to say hello to some old friends?"
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What's your problem?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "We want to talk to you about your fighting style at Colosso."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, 4);
  // "Yeah. And we think you shouldn't have won, Isaac. We think you..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "It sounds like you resent the fact that Isaac beat you..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimation(ACTOR_NAVAMPA, 4);
  Actor_SetAnimation(ACTOR_SATRAGE, 4);
  Actor_SetAnimationAndWait(ACTOR_AZART, 4);
  // "We've just been thinking about it, and there's no way that Isaac..."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Right. So you think I cheated just because I used my Pynergy..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_FaceEachOther(ACTOR_AZART, 21, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I don't know how you did it. I'm just sayin' I think you cheated is..."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_FaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  // "Ah, but Psynergy is a warrior's skill, every bit as much as your..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, 4);
  // "You're not listening, are you, old man? That fight was fixed, and..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetDestinationOffset(ACTOR_AZART, -16, 24);
  Actor_SetDestinationOffset(ACTOR_NAVAMPA, -16, 24);
  Actor_SetDestinationOffset(ACTOR_SATRAGE, -16, 24);
  FieldActor_WaitForMoveById(ACTOR_SATRAGE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      SCENE_SHAMAN_VILLAGE_CAVE, SHAMAN_VILLAGE_CAVE_AFTER_BEATING_GLADIATORS);
  Event_SetDefeatDestination(SCENE_SHAMAN_VILLAGE_CAVE,
                             SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS);
  Field_RequestBattleFromEncounterTable(102, 2);
}

/// Talking to a gladiator starts the rematch.
void ShamanVillageCave_FightGladiators(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(
      SCENE_SHAMAN_VILLAGE_CAVE, SHAMAN_VILLAGE_CAVE_AFTER_BEATING_GLADIATORS);
  Event_SetDefeatDestination(SCENE_SHAMAN_VILLAGE_CAVE,
                             SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS);
  Field_RequestBattleFromEncounterTable(102, 2);
}

void ShamanVillageCave_LoseToColossoFans(void) {
  s32 angle = FACING_NORTHWEST;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 273, 0);
  FieldEvent_SetMessageId((s32)MsgHeyTheseGuys);
  Actor_SetPositionAndFacing(ACTOR_FELIX, px(184), px(229), angle);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, px(200), px(221), angle);
  {
    s32 turn = 0xc000;

    Actor_SetPositionAndFacing(ACTOR_KRADEN, px(218), px(320), turn);
  }
  Actor_SetAnimation(ACTOR_FELIX, 19);
  Actor_SetAnimation(ACTOR_ISAAC, 39);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 2);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Hey, these guys aren't very strong at all!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Ha! We were right! They were cheating at Colosso!"
  Event_OpenMessage(ACTOR_SATRAGE, 0);
  if (EventYesNo_Ask(1) == 0) {
    Message_CloseFinishedWindows();
    FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
    gFieldWork->message++;
  } else {
    Message_CloseFinishedWindows();
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  }
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, 4);
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 373, 1);
  FieldEvent_Wait(100);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 1);
  FieldEvent_Wait(20);
  Event_SetDestination((s16)gMainRoutineWork.restartScene,
                       (s16)gMainRoutineWork.restartEntrance);
#if TLA_PATCH_1
  WorldMap_SetReturnLeavingScene(gMainRoutineWork.next,
                                 gMainRoutineWork.entrance);
#endif
  gFieldWork->transition = 0;
}

void ShamanVillageCave_BeatColossoFans(void) {
  s16 *target;
  s32 angle = FACING_NORTHEAST;

  Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 273, 0);
  FieldEvent_SetMessageId((s32)MsgTheseGuysWere);
  Actor_SetPositionAndFacing(ACTOR_FELIX, px(184), px(229), angle);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, px(200), px(221), angle);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, px(208), px(237), angle);
  Actor_SetAnimation(ACTOR_AZART, 8);
  Actor_SetAnimation(ACTOR_SATRAGE, 8);
  Actor_SetAnimation(ACTOR_NAVAMPA, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(ACTOR_AZART, 2);
  FieldEvent_Wait(5);
  // "These guys were phenomenal!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  // "Which one of you didn't think they won Colosso fair and square?"
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  // "It wasn't me! I think both you and Navampa were saying something..."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "So. You don't still think our victory at Colosso was fixed, do you?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, 3);
  FieldEvent_Wait(10);
  // "No way, Isaac! We've seen the error of our ways!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "We promise we won't badmouth you or call you a fraud ever again...."
  Event_OpenMessage(ACTOR_SATRAGE, 0);
  while (FieldEvent_AskYesNoWithActorAnimation(0, 0) != 0) {
    Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_OpenMessage(ACTOR_AZART, 0);
    gFieldWork->message--;
  }
  gFieldWork->message++;
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_SetAnimation(ACTOR_AZART, 1);
  Actor_SetAnimation(ACTOR_SATRAGE, 1);
  Actor_SetAnimation(ACTOR_NAVAMPA, 1);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  ShamanVillageCave_PlaceGoldenShirt();
  FieldEvent_Wait(60);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_AZART, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_SATRAGE, 1);
  Actor_SetSpeed(ACTOR_AZART, 1);
  Actor_SetSpeed(ACTOR_NAVAMPA, 1);
  Actor_SetScript(ACTOR_SATRAGE, gShamanVillageCave_GladiatorLeaves);
  FieldEvent_Wait(20);
  Actor_SetScript(ACTOR_AZART, gShamanVillageCave_GladiatorFollowsOut);
  FieldEvent_Wait(20);
  Actor_SetScript(ACTOR_NAVAMPA, gShamanVillageCave_GladiatorFollowsOut);
  FieldEvent_Wait(100);
  Actor_FaceEachOther(ACTOR_ISAAC, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_WALK);
  target = (s16 *)Actor_Get(ACTOR_FELIX);
  if (target != 0)
    Actor_SetDestination(ACTOR_ISAAC, target[5], target[9]);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  target = (s16 *)Actor_Get(ACTOR_FELIX);
  if (target != 0)
    Actor_SetDestination(ACTOR_KRADEN, target[5], target[9]);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

/// The party goes through a passage in the rock: everyone stops and the screen
/// fades.
void ShamanVillageCave_EnterPassage(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  u32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_Get(id);
  if (object)
    object->motionFlags = 0;
  id++;
  if (id < 64)
    goto next;
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  FieldEvent_Wait(5);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

s32 Scene_Initialize(void) {
  struct Actor *object;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_OVERGROWTH_CLEARED))
    Actor_SetPosition(ACTOR_OVERGROWTH, 0, 0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOWER_OVERGROWTH_CLEARED))
    Actor_SetPosition(ACTOR_LOWER_OVERGROWTH, 0, 0);
  if (Event_GetFlag(FLAG_DJINNI(DJINNI_EDDY)) ||
      Event_GetFlag(FLAG_DJINNI_HIDING(DJINNI_EDDY)))
    Event_SetFlag(FLAG_SCENE_TEMP_05);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED)) {
    object = Actor_Get(ACTOR_PILLAR);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->x.fixed = px(616);
    object->y.fixed = 0;
    Actor_SetSpriteFlags(object, 0);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, 38, 19);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PUDDLE), 0);
  Actor_SetAnimation(ACTOR_PUDDLE, 1);
  if (Event_GetFlag(FLAG_AREA_TEMP_02)) {
    object = Actor_Get(ACTOR_PUDDLE);
    Actor_SetAnimation(ACTOR_PUDDLE, 5);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_PUDDLE), 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->y.fixed = 0;
    MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_06))
    Actor_SetPosition(ACTOR_EDDY, 328, 792);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_SATRAGE), 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AZART), 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_NAVAMPA), 2);
  if ((!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
       (gMainRoutineWork.entrance ==
            SHAMAN_VILLAGE_CAVE_AFTER_BEATING_GLADIATORS ||
        gMainRoutineWork.entrance ==
            SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS)) ||
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS)) {
    ShamanVillageCave_PlaceGladiators();
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance ==
            SHAMAN_VILLAGE_CAVE_AFTER_BEATING_GLADIATORS) {
      ShamanVillageCave_BeatColossoFans();
    } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
               gMainRoutineWork.entrance ==
                   SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS) {
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
      ShamanVillageCave_LoseToColossoFans();
    }
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT))
    ShamanVillageCave_PlaceGoldenShirt();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 firstBlownAway;
  s32 secondBlownAway;

  firstBlownAway = Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_OVERGROWTH_CLEARED);
  if (firstBlownAway == 0)
    goto firstDone;
  MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, ACTOR_OVERGROWTH);
firstDone:
  secondBlownAway =
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOWER_OVERGROWTH_CLEARED);
  if (secondBlownAway == 0)
    goto done;
  MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
done:
  return 0;
}

void ShamanVillageCave_JumpForward(void) {
  Field_TryJumpForward();
  return;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ShamanVillageCave),
    SCENE_EXIT_UNLESS(1, WorldMap, 56, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ShamanVillageCave, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ShamanVillageCave, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ShamanVillageCave, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ShamanVillageCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ShamanVillageCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, ShamanVillageCave, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ShamanVillageCave, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, ShamanVillageCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, WorldMap, 57, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

s32 gShamanVillageCave_OvergrowthRests[] = {
    22, 11, 0x18000, 46, (s32)ShamanVillageCave_SettleOvergrowth, 17,
};

// clang-format off
u32 gShamanVillageCave_Settle[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gShamanVillageCave_OvergrowthRests, 40, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gShamanVillageCave_OvergrowthRests, 232, 0, 608, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gShamanVillageCave_Settle, 584, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_EDDY), MOVEMENT_TYPE_STAND, 264, 0, 760, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_FACING(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {STEP_TRIGGER_FACING(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {STEP_TRIGGER_FACING(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 50, 0x205, EVENT(ShamanVillageCave_EddyFlees)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 41, 0x201, EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 42, 0x201, EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 43, 0x202, EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 44, 0x202, EVENT(ShamanVillageCave_JumpForward)},
    {TRIGGER_STEP, 100, FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS, EVENT(ShamanVillageCave_MeetColossoFans)},
    {TRIGGER_TALK, ACTOR_EDDY, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightEddy)},
    {TRIGGER_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_AZART, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_GOLDEN_SHIRT, CONDITION_ALWAYS, EVENT(ShamanVillageCave_TakeGoldenShirt)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_OVERGROWTH, CONDITION_ALWAYS, EVENT(ShamanVillageCave_BlowAwayOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_LOWER_OVERGROWTH, CONDITION_ALWAYS, EVENT(ShamanVillageCave_BlowAwayOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER, 0x200, EVENT(ShamanVillageCave_LiftFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 1, 0x201, EVENT(ShamanVillageCave_LiftSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 2, 0x202, EVENT(ShamanVillageCave_LiftThirdRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_LAST_BOULDER, 0x203, EVENT(ShamanVillageCave_LiftFourthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, 0x302, EVENT(ShamanVillageCave_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED, EVENT(ShamanVillageCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED, EVENT(ShamanVillageCave_FinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

/// The beaten gladiators walk out of the cave, one after another.
u32 gShamanVillageCave_GladiatorLeaves[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(232, 227),
    SC_SYNC,
    SC_MOVE_XZ(217, 243),
    SC_SYNC,
    SC_MOVE_XZ(217, 334),
    SC_SYNC,
    SC_RELEASE,
};
// clang-format on

s32 gShamanVillageCave_GladiatorFollowsOut[] = {
    39,       2, 48, 0xe00000,
    0xc30000, 1, 43, (s32)gShamanVillageCave_GladiatorLeaves,
};
