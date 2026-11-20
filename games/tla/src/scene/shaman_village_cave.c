/// @file
/// The cave by the Shaman Village: weeds Whirlwind scatters, a pillar Move
/// pushes into a gap, boulders Lift raises, a puddle Frost freezes, Eddy the
/// Mercury Djinni who hops away from the party, and in a linked game the three
/// gladiators Isaac beat at Colosso.
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
#include "field/field_sequence.h"
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

/// Set once Move has pushed the stone pillar into the gap at column 38.
#define FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED 0x9a8
/// Set once Whirlwind has scattered the first clump of weeds
/// (ACTOR_FIRST_WEEDS).
#define FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED 0x9a9
/// Set once Whirlwind has scattered the second clump of weeds
/// (ACTOR_SECOND_WEEDS).
#define FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED 0x9aa
/// Set as the gladiators from Colosso find the party: "I found Isaac!"
#define FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS 0xa70
/// Set while the party has lost to the gladiators, who wait for a rematch.
#define FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS 0xa71
/// Set once the party has beaten the gladiators.
#define FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS 0xa72
/// Set while the Golden Shirt the beaten gladiators leave lies in the cave.
#define FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT 0xa73
/// Set as Lift raises boulder n (0 to 3) of the second room; while boulders 1
/// and 2 stand, the party can jump across onto them.
#define FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(n) (FLAG_SCENE_TEMP_00 + (n))
/// Set once Eddy, its hop south ended by the lifted third boulder, has winced
/// at the party.
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED FLAG_SCENE_TEMP_04
/// Set once Eddy has been fought or has joined: it no longer hops away from the
/// party.
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING FLAG_SCENE_TEMP_05
/// Set as the battle with Eddy begins: back in the cave, Eddy stands on the
/// ledge between the third and fourth boulders.
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT FLAG_SCENE_TEMP_06
/// Set once Frost has frozen the puddle into a pillar of ice.
#define FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN FLAG_AREA_TEMP_02

/// Where the gladiators' battle returns the party: beaten them, or lost.
enum ShamanVillageCaveEntrance {
  SHAMAN_VILLAGE_CAVE_AFTER_BEATING_GLADIATORS = 20,
  SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS = 21
};

void Message_CloseFinishedWindows(void);

/// The cave's actors, numbering the rows of gActorPlacements.
enum ShamanVillageCaveActorId {
  /// The weeds Whirlwind scatters from the wall below the first room's west
  /// passage, over the holds that climb it (SPRITE_IVY).
  ACTOR_FIRST_WEEDS = ACTOR_FIRST_PLACED,
  /// A round boulder placed in the map's top left corner, above every room's
  /// view; nothing moves it.
  ACTOR_HIDDEN_BOULDER,
  /// The second clump of weeds, in the second room.
  ACTOR_SECOND_WEEDS,
  /// Four round boulders standing in the gaps between the second room's
  /// ledges (SPRITE_ROUND_BOULDER).
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
  // The placement table's last row, in Isaac's sprite, is ACTOR_ISAAC.
};

extern u32 gShamanVillageCave_GladiatorLeaves[];

extern s32 gShamanVillageCave_GladiatorFollowsOut[];

void Field_TryJumpForward();

extern s32 gShamanVillageCave_WeedsScript[];
extern u32 gShamanVillageCave_PillarScript[];

s32 Event_PickUpItem();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Whirlwind scatters one of the two clumps of weeds for good, opening the cell
/// behind it.
void ShamanVillageCave_ScatterWeeds(s32 unused, s32 weeds) {
  if (weeds == ACTOR_FIRST_WEEDS) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED);
    MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, 8);
  }
  if (weeds == ACTOR_SECOND_WEEDS) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED);
    MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
  }
}

/// The pillar falls until it lands, or for a second, and stands in the gap
/// without its shadow.
void ShamanVillageCave_DropPillarIntoGap(s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 frames = 60;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (frames != 0) {
    System_Wait(1);
    if (actor->velocityY == 0)
      break;
    frames--;
  }
  System_Wait(10);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
}

/// Move on the stone pillar: its old cells open first.
void ShamanVillageCave_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(99, 10, 4, 3, 99, 18);
}

/// Pushed two spots to column 38, the pillar drops into the gap there and makes
/// the shortcut (FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED).
void ShamanVillageCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 x = FIELD_CELL(actor->x.fixed);

  if (x == 38) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED);
    actor->priorityFlags = 0;
    ShamanVillageCave_DropPillarIntoGap(id);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, x, 19);
  }
  MapGrid_CopyCellAttributes(99, 6, 4, 3, 99, 18);
}

/// Frost freezes the puddle into a pillar of ice.
void ShamanVillageCave_FreezePuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_PUDDLE);

  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
}

/// Eddy, the Mercury Djinni, hops away from the leader who comes near on its
/// side of the third boulder's column: two cells south (north when the leader
/// is south of it), four cells east or west away from the leader, the last
/// three in a leap, and back. Once the third boulder is lifted, its first hop
/// south is its last: it lands, steps west, jumps, backs away and winces.
void ShamanVillageCave_EddyFlees(void) {
  struct Actor *actor = Actor_GetById(ACTOR_EDDY);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 leaderX = FIELD_TILE(leader->x.fixed);
  s32 leaderZ = FIELD_TILE(leader->z.fixed);
  s32 eddyX = FIELD_TILE(actor->x.fixed);
  s32 dx;
  s32 dz;

  if (leaderX <= 36 && eddyX > 37)
    return;
  if (leaderX > 37 && eddyX <= 36)
    return;
  dx = 1;
  if (leaderX > 36)
    dx = -dx;
  dz = -1;
  if (leaderZ <= 95)
    dz = 1;
  if (dz == 1 &&
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2)) != 0) {
    if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED) != 0)
      return;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_Jump(ACTOR_EDDY, 4, 20);
    Actor_SetDestinationOffset(ACTOR_EDDY, 0, 32);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldCamera_MoveToActorById(ACTOR_EDDY, 1);
    FieldActor_StartRepeatedMotion(ACTOR_EDDY, 2);
    Actor_FaceDirection(ACTOR_EDDY, FACING_WEST, 0);
    Actor_SetDestinationOffset(ACTOR_EDDY, -8, 0);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldActor_Jump(ACTOR_EDDY, 4, 20);
    Actor_SetAttachedEffect(ACTOR_EDDY,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(30);
    Actor_GetById(ACTOR_EDDY)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkBy(ACTOR_EDDY, 8, 0);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_EDDY, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldEvent_Wait(30);
    Event_End();
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  dz *= 32;
  FieldActor_Jump(ACTOR_EDDY, 4, 20);
  Actor_SetDestinationOffset(ACTOR_EDDY, 0, dz);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, dx * 16, 0);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, dx * 48, 0);
  actor->velocityY = toFIXED(8);
  dz = -dz;
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, 0, dz);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Event_End();
}

/// Talking to Eddy, the Mercury Djinni, starts its battle.
void ShamanVillageCave_FightEddy(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT);
  Event_RequestDjinnBattle(ACTOR_EDDY, DJINNI_EDDY);
}

/// The call gShamanVillageCave_WeedsScript makes: the weeds cast no shadow, are
/// drawn at priority 2 and stand still.
s32 ShamanVillageCave_SetUpWeeds(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  ((struct FieldSprite *)actor->sprite)->priority = 2;
  actor->motionFlags = 0;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Lift raises the first boulder.
void ShamanVillageCave_LiftFirstBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(0));
}

/// Lift raises the second boulder.
void ShamanVillageCave_LiftSecondBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1));
}

/// Lift raises the third boulder.
void ShamanVillageCave_LiftThirdBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2));
}

/// Lift raises the fourth boulder.
void ShamanVillageCave_LiftFourthBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(3));
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// The Golden Shirt the gladiators leave goes to the party, if there is room.
void ShamanVillageCave_TakeGoldenShirt(void) {
  s32 result;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result = Value3((s32 (*)())Event_PickUpItem, ACTOR_GOLDEN_SHIRT,
                  ITEM_GOLDEN_SHIRT, -1);
  if (result == 0)
    Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
  Event_End();
}

/// The three gladiators stand where their rematch left them.
void ShamanVillageCave_PlaceGladiators(void) {
  s32 angle;

  angle = FACING_SOUTH_SOUTHWEST;
  Actor_SetPositionAndFacing(ACTOR_SATRAGE, toFIXED(231), toFIXED(199), angle);
  Actor_SetPositionAndFacing(ACTOR_AZART, toFIXED(220), toFIXED(191), angle);
  Actor_SetPositionAndFacing(ACTOR_NAVAMPA, toFIXED(204), toFIXED(191), angle);
}

/// The Golden Shirt lies where the gladiators left it
/// (FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT).
void ShamanVillageCave_PlaceGoldenShirt(void) {
  s32 angle;

  FieldActor_ShowItemIconById(ACTOR_GOLDEN_SHIRT, ITEM_GOLDEN_SHIRT);
  angle = FACING_NORTH;
  Actor_SetPositionAndFacing(ACTOR_GOLDEN_SHIRT, toFIXED(212), toFIXED(199),
                             angle);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
}

/// When The Broken Seal's save had won Colosso, the three gladiators Isaac beat
/// there find the party once he has joined it, and challenge it.
void ShamanVillageCave_MeetGladiators(void) {
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
  Actor_SetPositionAndFacing(ACTOR_SATRAGE, toFIXED(199), toFIXED(130), angle);
  Actor_WalkTo(ACTOR_SATRAGE, 231, 199);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(ACTOR_AZART, toFIXED(199), toFIXED(130), angle);
  Actor_WalkTo(ACTOR_AZART, 220, 191);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(ACTOR_NAVAMPA, toFIXED(199), toFIXED(130), angle);
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
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  // "Yeah. And we think you shouldn't have won, Isaac. We think you..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "It sounds like you resent the fact that Isaac beat you..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimation(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_SATRAGE, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_AZART, ANIM_SHAKE_HEAD);
  // "We've just been thinking about it, and there's no way that Isaac..."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Right. So you think I cheated just because I used my Pynergy..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_FaceEachOther(ACTOR_AZART, ACTOR_NAVAMPA, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I don't know how you did it. I'm just sayin' I think you cheated is..."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_FaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Ah, but Psynergy is a warrior's skill, every bit as much as your..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
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

/// Back in the cave after losing the rematch, the gladiators crow over the
/// party: "Hey, these guys aren't very strong at all!"
void ShamanVillageCave_LoseToGladiators(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 273, 0);
  FieldEvent_SetMessageId((s32)MsgHeyTheseGuys);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(184), toFIXED(229),
                             FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(200), toFIXED(221),
                             FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(218), toFIXED(320),
                             FACING_NORTH);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
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
    // Yes: "I knew it!"
    FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
    gFieldWork->message++;
  } else {
    Message_CloseFinishedWindows();
    gFieldWork->message++;
    // No: "It takes a lot to reach the finals at Colosso... A lot more..."
    FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  }
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  // "You look like you've got business in here, but there's no way you're..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Give it up! We're staying here. We've got our eyes peeled, and..."
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

/// Back in the cave after winning the rematch, the gladiators marvel: "These
/// guys were phenomenal!"
void ShamanVillageCave_BeatGladiators(void) {
  struct Actor *felix;

  Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 273, 0);
  FieldEvent_SetMessageId((s32)MsgTheseGuysWere);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(184), toFIXED(229),
                             FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(200), toFIXED(221),
                             FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(208), toFIXED(237),
                             FACING_NORTHEAST);
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
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "No way, Isaac! We've seen the error of our ways!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "We promise we won't badmouth you or call you a fraud ever again. Will..."
  Event_OpenMessage(ACTOR_SATRAGE, 0);
  while (FieldEvent_AskYesNoWithActorAnimation(0, 0) != 0) {
    Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // No: "Don't be that way! We've learned our lesson! Forgive us!"
    Event_OpenMessage(ACTOR_AZART, 0);
    gFieldWork->message--;
  }
  gFieldWork->message++;
  // Yes: "You're not such a bad kid after all!"
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_SetAnimation(ACTOR_AZART, ANIM_STAND);
  Actor_SetAnimation(ACTOR_SATRAGE, ANIM_STAND);
  Actor_SetAnimation(ACTOR_NAVAMPA, ANIM_STAND);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You came out here looking for treasure, didn't you?"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  ShamanVillageCave_PlaceGoldenShirt();
  FieldEvent_Wait(60);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Here, this was in the chest we found."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  // "You were nice enough to forgive us, so we think you should have it."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Right, well... I guess we'll be leaving now."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "By the time the next Colosso rolls around, we ought to be strong..."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_AZART, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Maybe we'll see you there."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_WALK);
  felix = Actor_Get(ACTOR_FELIX);
  if (felix != NULL)
    Actor_SetDestination(ACTOR_ISAAC, felix->x.part[0].integer,
                         felix->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  felix = Actor_Get(ACTOR_FELIX);
  if (felix != NULL)
    Actor_SetDestination(ACTOR_KRADEN, felix->x.part[0].integer,
                         felix->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

/// The party goes through a passage in the rock: everyone stops and the screen
/// fades.
void ShamanVillageCave_EnterPassage(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  u32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
    actor = Actor_Get(id);
    if (actor)
      actor->motionFlags = 0;
  }
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
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED))
    Actor_SetPosition(ACTOR_FIRST_WEEDS, 0, 0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED))
    Actor_SetPosition(ACTOR_SECOND_WEEDS, 0, 0);
  if (Event_GetFlag(FLAG_DJINNI(DJINNI_EDDY)) ||
      Event_GetFlag(FLAG_DJINNI_HIDING(DJINNI_EDDY)))
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED)) {
    actor = Actor_Get(ACTOR_PILLAR);
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->x.fixed = toFIXED(616);
    actor->y.fixed = 0;
    Actor_SetSpriteFlags(actor, 0);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, 38, 19);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PUDDLE), 0);
  Actor_SetAnimation(ACTOR_PUDDLE, 1);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN)) {
    actor = Actor_Get(ACTOR_PUDDLE);
    Actor_SetAnimation(ACTOR_PUDDLE, 5);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_PUDDLE), 0);
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->y.fixed = 0;
    MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT))
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
      ShamanVillageCave_BeatGladiators();
    } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
               gMainRoutineWork.entrance ==
                   SHAMAN_VILLAGE_CAVE_AFTER_LOSING_TO_GLADIATORS) {
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
      ShamanVillageCave_LoseToGladiators();
    }
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT))
    ShamanVillageCave_PlaceGoldenShirt();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 firstScattered;
  s32 secondScattered;

  firstScattered =
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED);
  if (firstScattered != 0)
    MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, 8);
  secondScattered =
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED);
  if (secondScattered != 0)
    MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
  return 0;
}

/// The leader jumps across the gap ahead if there is ground to land on.
void ShamanVillageCave_JumpForward(void) { Field_TryJumpForward(); }

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(ShamanVillageCave),
    SCENE_EXIT_UNLESS(SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST, WorldMap,
                      WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ShamanVillageCave, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ShamanVillageCave, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ShamanVillageCave, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ShamanVillageCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ShamanVillageCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, ShamanVillageCave, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ShamanVillageCave, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, ShamanVillageCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST, WorldMap,
                      WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_EAST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The weeds' script: they stand half as tall again, and are set up by
/// ShamanVillageCave_SetUpWeeds.
s32 gShamanVillageCave_WeedsScript[] = {
    SC_SET(SW_SCALE_Y_DUPLICATE, FIXED_ONE * 3 / 2),
    SC_CALL(ShamanVillageCave_SetUpWeeds),
    SC_END,
};

// clang-format off
/// The pillar's script: it stands still.
u32 gShamanVillageCave_PillarScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gShamanVillageCave_WeedsScript, 40, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gShamanVillageCave_WeedsScript, 232, 0, 608, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gShamanVillageCave_PillarScript, 584, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_EDDY), MOVEMENT_TYPE_STAND, 264, 0, 760, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
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
    {TRIGGER_STEP, 50, FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING, EVENT(ShamanVillageCave_EddyFlees)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 41, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 42, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 43, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 44, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(ShamanVillageCave_JumpForward)},
    {TRIGGER_STEP, 100, FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS, EVENT(ShamanVillageCave_MeetGladiators)},
    {TRIGGER_TALK, ACTOR_EDDY, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightEddy)},
    {TRIGGER_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_AZART, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_GOLDEN_SHIRT, CONDITION_ALWAYS, EVENT(ShamanVillageCave_TakeGoldenShirt)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_WEEDS, CONDITION_ALWAYS, EVENT(ShamanVillageCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_WEEDS, CONDITION_ALWAYS, EVENT(ShamanVillageCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(0), EVENT(ShamanVillageCave_LiftFirstBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 1, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(ShamanVillageCave_LiftSecondBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 2, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(ShamanVillageCave_LiftThirdBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_LAST_BOULDER, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(3), EVENT(ShamanVillageCave_LiftFourthBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN, EVENT(ShamanVillageCave_FreezePuddle)},
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

/// The gladiator behind the others steps up to their path and follows them out.
s32 gShamanVillageCave_GladiatorFollowsOut[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(224, 195),
    SC_SYNC,
    SC_GOTO(gShamanVillageCave_GladiatorLeaves),
};
