/// @file
/// Briggs's ship at Alhafra's eastern harbour: the cabin the party enters from
/// the deck, the rooms down its stairs, and a back room behind a tall crate.
/// Below, the party overhears Briggs and his mate learn that Madra's mayor has
/// come (EasternAlhafraShip_MayorArrives) and fights Briggs and a Sea Fighter;
/// after the battle Briggs gives himself up and the mayors of Madra and
/// Alhafra settle his fate and his ship's (EasternAlhafraShip_BriggsIsCaught).
/// His wife Chaucha and their baby Eoleo then stay aboard, where Eoleo lifts a
/// vase of flowers with Psynergy (EasternAlhafraShip_EoleoLifts). Move or a
/// push shifts the tall crate from the doorway to the back room, and Tremor
/// knocks the Large Bread down from the crates stacked inside
/// (EasternAlhafraShip_KnockDownTheBread): given to the crying boy in
/// Alhafra, it has his father let the party into the Alhafran Cave.

#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The tall crate pushed or Moved off the back room's door, east onto cell
/// (24, 40).
#define FLAG_CRATE_MOVED 0x900
/// Tremor has shaken the stacked crates down off the crate under them and
/// knocked the Large Bread to the floor.
#define FLAG_BREAD_KNOCKED_DOWN 0x901
/// The Large Bread has been picked up.
#define FLAG_BREAD_TAKEN 0x902
/// Set the first time the baby Eoleo lifts the vase with Psynergy; his mother
/// tells of his power only then.
#define FLAG_EOLEO_LIFTED 0x8ae

/// Where the party comes back after the battle with Briggs and a Sea Fighter
/// of his crew, numbered 99 as The Broken Seal's Altin Peak numbers its own,
/// and the entrance it then counts as: the foot of the stairs down from the
/// room the party boards into.
enum { ENTRANCE_FOOT_OF_STAIRS = 3, ENTRANCE_AFTER_BRIGGS_BATTLE = 99 };

/// The cells just below the foot of those stairs, (28, 24) and (29, 24), where
/// the party first overhears Briggs (EasternAlhafraShip_MayorArrives).
enum { CELL_BELOW_THE_STAIRS = 20 };

/// The people and things aboard, numbering the rows of
/// gEasternAlhafraShip_ActorPlacements whose sprites are not the party's.
enum EasternAlhafraShipActorId {
  ACTOR_BRIGGS = ACTOR_FIRST_PLACED,
  /// Briggs's mate in the cabin: "He knew we needed a ship, and he knew we
  /// went east."
  ACTOR_BRIGGS_MATE,
  ACTOR_CHAUCHA,
  ACTOR_EOLEO,
  /// The vase of flowers Eoleo lifts with Psynergy.
  ACTOR_VASE,
  /// The tall crate before the back room's door, which Move or a push shifts
  /// aside (FLAG_CRATE_MOVED).
  ACTOR_TALL_CRATE,
  /// The stacked crates standing on another in the back room, which Tremor
  /// shakes down beside it.
  ACTOR_STACKED_CRATES,
  /// The Large Bread on top of them, an item icon on an unseen marker.
  ACTOR_LARGE_BREAD,
  /// Those who come aboard after the battle, waiting off the map until then.
  ACTOR_ALHAFRAS_MAYOR,
  ACTOR_MADRAS_MAYOR,
  ACTOR_MADRAS_ELDER,
  ACTOR_MADRAN_SOLDIER,
  /// Alhafra's soldiers.
  ///@{
  ACTOR_ALHAFRAN_SOLDIER,
  ACTOR_SECOND_ALHAFRAN_SOLDIER,
  ///@}
  /// The rest of Briggs's crew, who come aboard with him after the battle.
  ///@{
  ACTOR_FIRST_PIRATE,
  ACTOR_SECOND_PIRATE,
  ACTOR_THIRD_PIRATE,
  ///@}
  ACTOR_KRADEN
};

extern struct ActorPlacement gEasternAlhafraShip_ActorPlacements[];
void EasternAlhafraShip_BriggsIsCaught(void);
extern u32 gEasternAlhafraShip_BriggsRunsScript[];
extern s32 gEasternAlhafraShip_MateRunsScript[];

void Event_PickUpItem();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 Scene_GetActorPlacements(void) {
  return (s32)gEasternAlhafraShip_ActorPlacements;
}

/// Nothing visible happens as Felix begins to push or Move the tall crate in
/// front of the back room's door; the push or Move goes on as anywhere else.
void EasternAlhafraShip_StartMovingCrate(void) {}

/// Felix pushes or Moves the tall crate standing against the north wall below
/// the stairs. Shifted east to cell (24, 40), it uncovers a dark doorway in
/// the wall to the back room, and stays aside when the party comes back;
/// shifted back west to (23, 40), it covers the doorway again.
void EasternAlhafraShip_FinishMovingCrate(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (x == 24 && z == 40)
    GameFlag_Set(FLAG_CRATE_MOVED);
  if (x == 23 && z == 40)
    GameFlag_Clear(FLAG_CRATE_MOVED);
}

/// Felix casts Tremor on the stacked crates in the back room: he hops back as
/// they shift down beside the crate under them, and the Large Bread on top
/// falls to the floor, where he can pick it up.
void EasternAlhafraShip_KnockDownTheBread(void) {
  GameFlag_Set(FLAG_BREAD_KNOCKED_DOWN);
  Actor_GetById(gMainRoutineWork.leaderActorId)->velocityY = toFIXED(6);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 136, 488);
  Event_Wait(10);
  Actor_SetDestination(ACTOR_LARGE_BREAD, 120, 472);
  Actor_MoveToAndWaitById(ACTOR_STACKED_CRATES, 136, 475);
  Actor_GetById(ACTOR_LARGE_BREAD)->velocityY = toFIXED(6);
  Sound_Play(SE_POUND);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_LARGE_BREAD);
  Sound_Play(SE_DOOR_CLOSE);
  return;
}

/// Felix picks up the Large Bread that Tremor knocked to the floor of the back
/// room.
void EasternAlhafraShip_TakeTheBread(s32 id) {
  Event_PickUpItem(id, ITEM_LARGE_BREAD, FLAG_BREAD_TAKEN);
  return;
}

/// Felix talks to the baby Eoleo, who stays aboard with his mother Chaucha
/// once Briggs is caught: "Goo goo ga ga!" He lifts the vase of flowers beside
/// him with Psynergy and drops it with a splash. The first time, Chaucha says
/// he only lately started doing that. The screen is tinted toward yellow
/// (`Field_StartColorTransition`) while he holds the vase up.
void EasternAlhafraShip_EoleoLifts(s32 id) {
  struct Actor *actor = Actor_GetById(ACTOR_VASE);
  u8 *flags;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGooGooGa);
  // "Goo goo ga ga!"
  Event_ShowMessageAndWait(ACTOR_EOLEO, 0);
  Field_StartColorTransition(8, RGB(16, 16, 4));
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode((s32)Actor_GetById(id), 2);
  Sound_Play(SE_ASCEND);
  Event_Wait(30);
  flags = &actor->motionFlags;
  *flags = ACTOR_FLOATS;
  for (i = 0; i < 40; i++) {
    actor->y.fixed += toFIXED(1) / 5;
    Event_Wait(1);
  }
  Event_Wait(70);
  BattleEffect_SetObjectUpdateMode((s32)Actor_GetById(id), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->gravity = toFIXED(1) / 2;
  Event_Wait(5);
  Sound_Play(SE_WATER);
  if (Event_GetFlag(FLAG_EOLEO_LIFTED) == 0) {
    GameFlag_Set(FLAG_EOLEO_LIFTED);
    Event_Wait(30);
    Actor_FaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
    Event_Wait(10);
    // "Eoleo's only just recently started doing that... After that gem..."
    Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
    Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST - 1, 0);
  }
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the ship as the story stands: back from the battle, Briggs gives
/// himself up; once he is caught his crew are gone, and Chaucha and Eoleo stay
/// aboard after their visit to him, until the mast is cleared; and the tall
/// crate, the stacked crates and the Large Bread are where the party left them.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BRIGGS_BATTLE) {
    GameFlag_Set(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
    GameFlag_Set(FLAG_BRIGGS_CAUGHT);
    gMainRoutineWork.returnEntrance = ENTRANCE_FOOT_OF_STAIRS;
    gMainRoutineWork.entrance = ENTRANCE_FOOT_OF_STAIRS;
    EasternAlhafraShip_BriggsIsCaught();
  }
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS_MATE, 0, 0);
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS_MATE, 0, 0);
    if (!GameFlag_Get(FLAG_FAMILY_VISITED_BRIGGS)) {
      Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
      Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    }
  } else {
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
  }
  if (GameFlag_Get(FLAG_CRATE_MOVED))
    Actor_SetPosition(ACTOR_TALL_CRATE, 392, 648);
  Actor_GetById(ACTOR_TALL_CRATE)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_ShowItemIconById(ACTOR_LARGE_BREAD, ITEM_LARGE_BREAD);
  if (!GameFlag_Get(FLAG_BREAD_KNOCKED_DOWN)) {
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_GetById(ACTOR_STACKED_CRATES)->priorityFlags |=
          ACTOR_PRIORITY_PLATFORM;
      actor = Actor_GetById(ACTOR_LARGE_BREAD);
      actor->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
      actor->y.fixed += toFIXED(16);
      actor->groundHeight += toFIXED(16);
    }
  } else {
    Actor_SetPosition(ACTOR_STACKED_CRATES, 136, 475);
    if (!GameFlag_Get(FLAG_BREAD_TAKEN))
      Actor_SetPosition(ACTOR_LARGE_BREAD, 120, 472);
    else
      Actor_SetPosition(ACTOR_LARGE_BREAD, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u32 gQuestionMark_Script[];

#include "shared/question_mark_pop.h"

#include "shared/actor_pop_question_marks.h"

extern u32 gEasternAlhafraShip_LeaveUpTheStairsScript[];

/// Walking down from the foot of the stairs (cells (28, 24) and (29, 24)) on
/// the first visit, the party overhears Briggs and his mate: Madra's mayor has
/// come to Alhafra after them. Jenna's outburst gives the party away, and
/// Briggs and a Sea Fighter of his crew fight it.
void EasternAlhafraShip_MayorArrives(void) {
  struct MapWork *map;
  s32 result;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgTheMayorOf);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  map = Ram_HeapSlots->mapGrid;
  map->minX = 0;
  map->maxX = toFIXED(1024);
  map->minZ = 0;
  map->maxZ = toFIXED(1024);
  // "The mayor of Madra? Here?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 456, 408);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTH);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 416, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "You're certain?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "How did he know we came to Alhafra?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 15);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 23);
  // "He knew we needed a ship, and he knew we went east. It seems pretty..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH, 0);
  Event_Wait(20);
  // "Arr... Well, yeah... When you put it that way, it does make sense..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
  Event_Wait(10);
  // "He was looking to find the mayor of Alhafra..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "He must be hoping the mayor will tell him where we are..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
  Event_Wait(10);
  // "He'll probably tell the Alhafrans we're pirates, too, huh?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "Pirates!? That seems a little unfair!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What do you mean? We...are pirates, aren't we? I mean, we've taken..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "That's only because everyone in Champa would starve otherwise! We..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Ah..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "And even then, it's never a lot."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Wha-What's not?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  // "The stuff we take. We never take too much from any one town."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
  Event_Wait(10);
  // "Yeah, I guess not..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "And why do you think that is?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS_MATE, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS_MATE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_PopQuestionMarks(ACTOR_BRIGGS_MATE);
  Sound_Play(SE_LIGHT_APPEAR);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  // "Er... Why?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Because you never can tell how these wealthy towns will react. It's..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
  Event_Wait(10);
  // "Hey, yeah! Which is why we didn't steal anything from Alhafra..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Right! That's why we avoid the risky towns and just take what we..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "See? That's why you're our captain, Briggs! You've got brains coming..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Hmph! There's nothing brainy about that!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      30);
  Event_Wait(20);
  // "Did you hear that? It sounded like a girl! Who's out there!?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetSpeed(ACTOR_BRIGGS, 1.5);
  Actor_SetSpeed(ACTOR_BRIGGS_MATE, 1.5);
  Actor_SetMovementTypeById(ACTOR_BRIGGS_MATE,
                            (s32)gEasternAlhafraShip_MateRunsScript);
  Event_Wait(5);
  Actor_SetMovementTypeById(ACTOR_BRIGGS,
                            (s32)gEasternAlhafraShip_BriggsRunsScript);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  FieldCamera_MoveTo(448, CAMERA_KEEP, 416, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 15);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 23);
  // "What do you think you're doing?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(20);
  // "Don't you know that someone has been falsely imprisoned for your crimes?"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Er... What's your point?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "You have to come with us and attest to that man's innocence!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS_MATE, FACING_WEST, 0);
  Event_Wait(45);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  // "And...why would I want to do something like that?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Oh-ho! I was hoping we could avoid any ugliness, but you leave us no..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_Jump(ACTOR_KRADEN, 6, 15);
  Actor_Jump(ACTOR_KRADEN, 6, 23);
  // "We'll bring you in against your will if we must, but either way..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
  // "Isn't that right, Felix?"
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result == 0) {
    Event_Wait(20);
    Actor_FaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
    Event_Wait(20);
    // Yes: "Good for you! Let's teach these guys a lesson!"
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    gFieldWork->message++;
  } else {
    EasternAlhafraShip_CountAnswer();
    Event_Wait(20);
    Actor_FaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
    Event_Wait(20);
    gFieldWork->message++;
    // No: "After what they've done? Come on, we need to make them fess up!"
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 15);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 23);
  // "You don't stand a chance! We're hardened criminal types! We eat punks..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "Watch yourselves, you self-righteous little brats! You're the ones..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_WalkBy(ACTOR_BRIGGS, 0, -16);
  Actor_WalkByById(ACTOR_BRIGGS_MATE, 0, -16);
  Event_Wait(10);
  Event_SetAfterBattleDestination(SCENE_EASTERN_ALHAFRA_SHIP,
                                  ENTRANCE_AFTER_BRIGGS_BATTLE);

  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA, 1);
  Event_End();
}

/// After the battle Briggs gives himself up. Chaucha pleads for him, and the
/// mayors of Madra and Alhafra settle what becomes of him and his crew and take
/// his ship for Madra.
void EasternAlhafraShip_BriggsIsCaught(void) {
  struct Actor *emote;
  struct Actor *briggs;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWellThatTakes);
  Actor_SetPosition(ACTOR_BRIGGS, 436, 416);
  FieldActor_SetSpriteRotationById(ACTOR_BRIGGS, DEGtoANG(90));
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS), 0);
  Actor_SetPosition(ACTOR_BRIGGS_MATE, 456, 432);
  FieldActor_SetSpriteRotationById(ACTOR_BRIGGS_MATE, DEGtoANG(180));
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS_MATE), 0);
  Actor_GetById(ACTOR_BRIGGS_MATE)->facing = FACING_SOUTH;
  Actor_SetPosition(ACTOR_FIRST_PIRATE, 456, 460);
  FieldActor_SetSpriteRotationById(ACTOR_FIRST_PIRATE, DEGtoANG(180));
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_FIRST_PIRATE), 0);
  Actor_SetPosition(ACTOR_SECOND_PIRATE, 420, 424);
  Actor_SetSpriteRotationById(ACTOR_SECOND_PIRATE, TRIG_TURN * 3 / 4);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_SECOND_PIRATE), 0);
  Actor_SetPosition(ACTOR_THIRD_PIRATE, 388, 440);
  Actor_SetSpriteRotationById(ACTOR_THIRD_PIRATE, DEGtoANG(90));
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_THIRD_PIRATE), 0);
  Actor_SetPosition(ACTOR_KRADEN, 456, 376);
  Actor_SetPosition(ACTOR_JENNA, 472, 392);
  Actor_SetPosition(ACTOR_SHEBA, 472, 376);
  Actor_SetPosition(ACTOR_CHAUCHA, 456, 552);
  Actor_SetPosition(ACTOR_SECOND_ALHAFRAN_SOLDIER, 472, 280);
  Actor_SetPosition(ACTOR_ALHAFRAN_SOLDIER, 440, 280);
  Actor_SetPosition(ACTOR_MADRAN_SOLDIER, 456, 296);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 472, 296);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 440, 296);
  Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 456, 304);
  Actor_SetSpeed(ACTOR_SECOND_ALHAFRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1);
  Actor_SetSpeed(ACTOR_BRIGGS, 1);
  Actor_SetSpeed(ACTOR_BRIGGS_MATE, 1);
  Actor_SetSpeed(ACTOR_FIRST_PIRATE, 1);
  Actor_SetSpeed(ACTOR_SECOND_PIRATE, 1);
  Actor_SetSpeed(ACTOR_THIRD_PIRATE, 1);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 408, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "Well, that takes care of that!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  // "And I'm willing to bet everyone Briggs has been harassing will be..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "This should set everyone in Indra at ease... They all seemed to fear..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "We lost..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH - 1, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTH + 1, 0);
  Event_Wait(50);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "Even pirates have honor... I'll go quietly. I won't make a scene..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "Go on... Hand us over to Madra or Alhafra or whoever..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "Good... First off, you can tell the Madrans that the man called..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Fine. So, what, I just have to say that Piers isn't one of my men?"
  Event_OpenMessage(ACTOR_BRIGGS, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(10);
    // Yes: "That's right. You tell the Madrans that, and they can release..."
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
    Event_Wait(10);
    gFieldWork->message += 1;
    // No: "Saying Piers is innocent won't clear you of your own crimes..."
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  }
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "We've secured Briggs's vow... We should take him to the Madrans..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  // "Wait!"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_FollowActor(ACTOR_CHAUCHA, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 456, 504);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 16, 0);
  Actor_WalkToAndWaitById(ACTOR_CHAUCHA, 472, 424);
  Event_Wait(10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  // "Can't you find it in your hearts to forgive Briggs and his men?"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "And who are you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "My name is Chaucha. I am Briggs's wife."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "So you're a pirate, too?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "No! Chaucha hasn't done a thing! Let her be!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "I know Briggs and his men have caused some great trouble with their..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  // "And I know their crimes cannot be ignored..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  // "If you know all that, then how can you ask us just to forgive him?"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "The only reason Briggs became a pirate was to help the people of Champa."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 15);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH - 1, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTH + 1, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(10);
  // "We Champa are a seafaring folk. Our livelihood depends entirely upon..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  // "But in recent years, the sea has changed... We can't live the life we..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "The oceans have warmed, and the fish have vanished from our waters."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "The soil in Champa is rocky and barren. No crops can grow there."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "No crops? So you have no grains? And no meat? It sounds like life in..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  emote = (struct Actor *)Actor_ShowEmote(ACTOR_BRIGGS,
                                          EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  emote->update = 0;
  Actor_SetPositionAndResetMotion(emote, (emote->x.fixed + toFIXED(22)),
                                  (emote->y.fixed + toFIXED(8)),
                                  emote->z.fixed);
  Event_Wait(30);
  // "It's not hard! It's unbearable! Our children can't eat! Our village..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  Event_Wait(20);
  // "These men promised to return with food enough for all our people, but..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "So you became pirates...thieves..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "We don't know where Champa is..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(20);
  // "It sits on the southeast corner of Angara..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Event_Wait(20);
  // "Southeast Angara? Isn't that near Xian and Lama Temple?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "You have a good memory! If you were to head south through the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "That's so far away..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "Well, yeah! You think we could be successful pirates so close to home?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  // "If they raided any towns near Champa, it would be obvious to all who..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "And no one back home could forgive me if they knew. Not even my..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 20);
  Actor_WalkByById(ACTOR_ALHAFRAS_MAYOR, 0, 8);
  // "But where does that leave us? Are we simply to accept your looting..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  emote = (struct Actor *)Actor_ShowEmote(
      ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  emote->update = 0;
  Actor_SetPositionAndResetMotion(emote, (emote->x.fixed + toFIXED(22)),
                                  (emote->y.fixed + toFIXED(8)),
                                  emote->z.fixed);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  // "I'm shocked, I tell you. Shocked!"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "What's so shocking?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "It looks like they really are pirates, just like you said..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Mister Mayor, I am appalled that you could not take our word for it!"
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "He probably wouldn't even have believed we were from Madra if the..."
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(10);
  // "Am I supposed to believe the accusations of a complete stranger?"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Is this your position, Mayor?"
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "How can I be expected to believe a tidal wave could wash Indra into..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "Or that a group of men could walk on foot! from Madra all the way to..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_SHAKE_HEAD);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  // "It sounds to me like you had forgotten about us altogether!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I'm not sure you remember the last time we met. Do you?"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "We met when I was just a child... It must have been more than ten..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
  Event_Wait(20);
  // "Twenty years! It's been twenty years since I last saw you."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  // "Has it really been so long?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  // "It's been a long time indeed!"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  // "I guess we can't blame you for forgetting his face."
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Don't be silly! I didn't forget his face! His face merely... changed!"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_MADRAS_ELDER, 3);
  Actor_SetAttachedEffect(ACTOR_MADRAS_ELDER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "Ahem! Forgive my interrupting your interruption, but..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAN_SOLDIER, FACING_SOUTH, 0);
  Event_Wait(20);
  FieldCamera_MoveWithinMapBounds(toFIXED(464), CAMERA_KEEP, toFIXED(344), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "Would I be correct in assuming that I am speaking to the leaders of..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, 0, 16);
  Event_Wait(10);
  // "You are correct! I am the mayor of Madra."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_WalkByById(ACTOR_ALHAFRAS_MAYOR, 0, 16);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_ELDER, 0, 16);
  Actor_SetAnimationById(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
  Event_Wait(10);
  // "And I..." and "I am..."
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_MADRAS_ELDER, 13, 1, 13, 4,
                                    ACTOR_ALHAFRAS_MAYOR, 17, 11, 22, 6, 0);
  Actor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "I am the mayor of Alhafra."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  // "I am Madra's town elder."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  // "And I am a close personal friend of Shin, whose lover was injured by..."
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH - 1, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "I am the scholar Kraden."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH - 1, 0);
  Event_Wait(20);
  // "He is Felix, the leader of our group."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "I am Jenna, Felix's sister."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  // "I am called Sheba. I come from the town of Lalivero."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationById(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "Now, what errand has brought you all here?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "We came for Briggs..." and "The mayor of Madra claims..."
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_MADRAS_MAYOR, 12, 1, 25, 4,
                                    ACTOR_ALHAFRAS_MAYOR, 9, 11, 14, 6, 0);
  Actor_StartRepeatedMotion(ACTOR_MADRAS_MAYOR, 3);
  Actor_SetAttachedEffect(ACTOR_MADRAS_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
  FieldEffect_SetAttached(ACTOR_ALHAFRAS_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "We followed Briggs here from Madra."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Of course, we have good reason to suspect them of being pirates."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  emote = (struct Actor *)Actor_ShowEmote(ACTOR_BRIGGS,
                                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  {
    FIXED x = emote->x.fixed;
    FIXED y = emote->y.fixed;
    emote->update = 0;
    Actor_SetPositionAndResetMotion(emote, x + toFIXED(22), y + toFIXED(8),
                                    emote->z.fixed);
  }
  Event_Wait(50);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  // "Ahem! And I was honestly taken aback to hear a businessman being..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
  // "If they truly are pirates, then why would they go to the trouble of..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_Jump(ACTOR_MADRAS_ELDER, 6, 15);
  Actor_Jump(ACTOR_MADRAS_ELDER, 6, 23);
  // "Because the very funds they used to purchase the ship were pillaged..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "If the ship was bought with Madran gold, then it belongs to the Madrans!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Event_Wait(30);
  // "What do you say? If you give them the ship, will that fix everything?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  briggs = Actor_GetById(ACTOR_BRIGGS);
  Actor_SetPosition(ACTOR_BRIGGS, briggs->x.part[0].integer + 20,
                    briggs->z.part[0].integer + 8);
  Actor_SetSpriteRotationById(ACTOR_BRIGGS, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS), SPRITE_FLAG_SHADOW);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Actor_Jump(ACTOR_BRIGGS, 6, 40);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  Actor_SetPosition(ACTOR_BRIGGS_MATE, 456, 440);
  Actor_SetSpriteRotationById(ACTOR_BRIGGS_MATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS_MATE),
                       SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_FIRST_PIRATE, 456, 456);
  Actor_SetSpriteRotationById(ACTOR_FIRST_PIRATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_FIRST_PIRATE),
                       SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_SECOND_PIRATE, 424, 424);
  Actor_SetSpriteRotationById(ACTOR_SECOND_PIRATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_SECOND_PIRATE),
                       SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_THIRD_PIRATE, 408, 440);
  Actor_SetSpriteRotationById(ACTOR_THIRD_PIRATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_THIRD_PIRATE),
                       SPRITE_FLAG_SHADOW);
  Actor_FaceDirection(ACTOR_BRIGGS_MATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FIRST_PIRATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_PIRATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_THIRD_PIRATE, FACING_NORTH, 0);
  Actor_Jump(ACTOR_FIRST_PIRATE, 6, 0);
  Actor_Jump(ACTOR_SECOND_PIRATE, 6, 0);
  Actor_Jump(ACTOR_THIRD_PIRATE, 6, 0);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 40);
  Actor_SetAnimationById(ACTOR_FIRST_PIRATE, ANIM_SHAKE_HEAD);
  Actor_SetAnimationById(ACTOR_SECOND_PIRATE, ANIM_SHAKE_HEAD);
  Actor_SetAnimationById(ACTOR_THIRD_PIRATE, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 368, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "Then where will you go?"
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "I won't run, and I won't hide. ...Actually, I don't feel up for much..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  // "Then you probably won't mind being locked up, will you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  Event_Wait(20);
  // "Briggs..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Don't worry, Chaucha! I'll be back!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_HEART, 40);
  // "Take care of Eoleo!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_PIRATE, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_SOUTHWEST, 0);
  Event_Wait(15);
  Actor_SetAnimationById(ACTOR_SECOND_PIRATE, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_THIRD_PIRATE, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 16, -16);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -32);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -32);
  Actor_WalkByById(ACTOR_SECOND_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_THIRD_PIRATE, 16, -16);
  Actor_WalkBy(ACTOR_JENNA, -16, -32);
  Actor_WalkByAndWait(ACTOR_SHEBA, -16, -32);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_STAND);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_WalkByById(ACTOR_BRIGGS_MATE, 16, 0);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_SECOND_PIRATE, 16, 0);
  Actor_WalkByById(ACTOR_THIRD_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 384, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BRIGGS_MATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_PIRATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Event_Wait(20);
  // "Your strange powers surprised me, but even without them, you would..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkByById(ACTOR_FIRST_PIRATE, 16, 0);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_THIRD_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_SECOND_PIRATE, 16, 0);
  Actor_WalkBy(ACTOR_THIRD_PIRATE, 0, -16);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_THIRD_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 336, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "We'll be taking that boat, then."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
  Event_Wait(10);
  // "You're taking our ship!? You're worse than we ever were!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_FIRST_PIRATE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Aren't you at least going to pay us for the boat!?"
  Event_ShowMessageThenWait(ACTOR_FIRST_PIRATE, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Quiet! All of you!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_FIRST_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SECOND_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_THIRD_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Do as you must, but I have one request..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 416, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 336, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "You have to let Chaucha and Eoleo stay in the boat until it's repaired."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_MADRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWaitById(ACTOR_MADRAS_MAYOR, 456, 312);
  Event_Wait(1);
  Actor_GetById(ACTOR_MADRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -16);
  Actor_WalkBy(ACTOR_THIRD_PIRATE, 0, -16);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  // "You haven't done Alhafra any harm, Briggs..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0,
                            10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  // "At least keep him locked up until the boat is repaired..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -32);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -32);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -32);
  Actor_WalkBy(ACTOR_THIRD_PIRATE, 0, -32);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -32);
  Actor_FaceDirectionById(ACTOR_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_WEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 296, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  // "And our prisoner, Piers... Is he one of your men?"
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I don't know any Piers! Sounds to me like you've made a pretty big..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirectionById(ACTOR_MADRAS_ELDER, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  // "See! It's just as I said! Piers is innocent!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_StartRepeatedMotion(ACTOR_MADRAN_SOLDIER, 3);
  Actor_SetAttachedEffect(ACTOR_MADRAN_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  // "You must return to Madra and free him immediately!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "...Me? What, alone?!?"
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "Only two of us will remain here. You can return to Madra with the others."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_NOD);
  Event_Wait(10);
  // "Understood! We'll return immediately!"
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_SetAnimationById(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_MADRAN_SOLDIER, 1.8);
  Actor_WalkByAndWait(ACTOR_MADRAN_SOLDIER, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAN_SOLDIER, -16, 0);
  Actor_SetPosition(ACTOR_MADRAN_SOLDIER, 0, 0);
  Event_Wait(20);
  Actor_WalkByAndWaitById(ACTOR_ALHAFRAN_SOLDIER, 16, 16);
  Actor_FaceDirectionById(ACTOR_ALHAFRAN_SOLDIER, FACING_EAST, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAN_SOLDIER, 2);
  Event_Wait(10);
  // "Can you come with us? ...Please?"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAN_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(20);
  Actor_SetMovementTypeById(ACTOR_SECOND_ALHAFRAN_SOLDIER,
                            gEasternAlhafraShip_LeaveUpTheStairsScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_BRIGGS,
                            gEasternAlhafraShip_LeaveUpTheStairsScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_BRIGGS_MATE,
                            gEasternAlhafraShip_LeaveUpTheStairsScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_FIRST_PIRATE,
                            gEasternAlhafraShip_LeaveUpTheStairsScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_SECOND_PIRATE,
                            gEasternAlhafraShip_LeaveUpTheStairsScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_THIRD_PIRATE,
                            gEasternAlhafraShip_LeaveUpTheStairsScript);
  Event_Wait(75);
  Actor_WalkByAndWaitById(ACTOR_ALHAFRAN_SOLDIER, 16, 0);
  Actor_WalkByAndWait(ACTOR_ALHAFRAN_SOLDIER, 0, -32);
  Actor_WalkByAndWait(ACTOR_ALHAFRAN_SOLDIER, -32, 0);
  Actor_SetPosition(ACTOR_ALHAFRAN_SOLDIER, 0, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  // "It looks like that wraps this whole mess up..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  // "Let us return to my mansion for a nice cup of tea..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkByAndWaitById(ACTOR_ALHAFRAS_MAYOR, 16, 0);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, 0, -16);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_ELDER, 16, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(20);
  // "You say your name is Kraden?"
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 320, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH, 0);
  Actor_WalkByById(ACTOR_FELIX, 16, 0);
  Actor_WalkByAndWaitById(ACTOR_KRADEN, 16, 0);
  Actor_WalkBy(ACTOR_FELIX, 0, -32);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -32);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "Thank you very much for the help with Briggs."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "If you have the time, please come see me at my mansion before you go..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "It's nothing special, but I would like to thank you for everything..."
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_ELDER, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, 0, -32);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, -32, 0);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, -16, 0);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 0, 0);
  Actor_WaitForMoveById(ACTOR_ALHAFRAS_MAYOR);
  Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(30);
  // "Why do I get the feeling the mayor of Alhafra prefers to deal with my..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "Thank all of you very much."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "Due to your efforts, Piers is going to be released."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTH + 1, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, -32, -8);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 24, -16);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, 16, 16);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(20);
  // "And now, this fine ship belongs to Madra."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "None of this would have come to pass were it not for you. Thank you."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "Unfortunately, I don't have any reward I can offer you here..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "Oh, pish posh! We don't need any reward, do we, Felix?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_SHAKE_HEAD);
    // Yes: "Ha! Your honesty is refreshing, Felix!"
    Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    EasternAlhafraShip_CountAnswer();
    Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
    Event_Wait(20);
    gFieldWork->message += 1;
    // No: "That's as may be, but I still feel quite indebted to you."
    Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  }
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "So, what shall we do?"
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_PopQuestionMarks(ACTOR_MADRAS_MAYOR);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "I know... Come to Madra after our boat has been fixed and we've..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "We are still recovering from the effects of the tidal wave, so it..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Well, shall we be going?"
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 0, -40);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, -24, 0);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 0, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "I see, so..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  FieldEffect_SetAttached(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "Oh! N-Nothing. Never mind..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  // "I do get to stay on the boat, right? At least until it's repaired?"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  // "Yes. That's where I'll be if you need me. ...With Eoleo..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 472, 376);
  FieldCamera_FollowActor(ACTOR_CHAUCHA, 1);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_WalkBy(ACTOR_KRADEN, -16, -32);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, -32);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_STAND);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 472, 320);
  Actor_WalkBy(ACTOR_JENNA, 16, -32);
  Actor_WalkBy(ACTOR_SHEBA, 16, -32);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 472, 264);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 440, 264);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Event_Wait(30);
  // "I feel kind of bad for Chaucha."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "What can we do about it? After all, she's the one married to a pirate."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(25);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(20);
  // "So, what should we do now?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "I suggest that we try to find Piers. He could prove enlightening!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  // "That's true... He seems to be a Water Adept. He might be able to tell..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
  Event_Wait(20);
  // "Do you think we should help them fix the boat?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "Say, if we help with the boat, maybe they'll give us a ride back to..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  // "What do you say, Felix?"
  Event_OpenMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(20);
    // Yes: "All right! Let's go see what we can do to get this thing..."
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    // No: "You're opposed to it? Well, since you seem to have your own..."
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  }
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestinationById(ACTOR_JENNA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Event_End();
  return;
}

// clang-format off
/// Briggs and his mate running out to the party once Jenna gives it away
/// (EasternAlhafraShip_MayorArrives).
u32 gEasternAlhafraShip_BriggsRunsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 28),
    SC_SYNC,
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(12, 0, 12),
    SC_SYNC,
    SC_MOVE(456, 0, 440),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(1),
    SC_END,
};

s32 gEasternAlhafraShip_MateRunsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(12, 0, -12),
    SC_SYNC,
    SC_MOVE_BY(52, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(12, 0, 12),
    SC_SYNC,
    SC_MOVE(472, 0, 440),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(1),
    SC_END,
};

/// The way up the stairs and off the ship that Briggs, his crew and their
/// guard take once he is caught.
u32 gEasternAlhafraShip_LeaveUpTheStairsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(472, 0, 264),
    SC_SYNC,
    SC_MOVE(440, 0, 264),
    SC_SYNC,
    SC_RELEASE,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(EasternAlhafraShip),
    SCENE_EXIT_UNLESS(EASTERN_ALHAFRA_SHIP_TO_EASTERN_ALHAFRA, EasternAlhafra, EASTERN_ALHAFRA_FROM_EASTERN_ALHAFRA_SHIP, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, EasternAlhafraShip, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, EasternAlhafraShip, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, EasternAlhafraShip, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, EasternAlhafraShip, 4, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gEasternAlhafraShip_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 396, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 88, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOWER_VASE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 128, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 149, 0, 475, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 149, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, CELL_BELOW_THE_STAIRS, FLAG_MAYOR_ARRIVED_AT_THE_SHIP,
     EVENT(EasternAlhafraShip_MayorArrives)},
    {TRIGGER_TALK, ACTOR_EOLEO, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_EoleoLifts)},
    {TRIGGER_TALK, ACTOR_CHAUCHA, CONDITION_ALWAYS, MSGNO(MsgEoleoAndI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEO, CONDITION_ALWAYS,
     MSGNO(MsgIMGonna)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAUCHA, CONDITION_ALWAYS,
     MSGNO(MsgIfEoleoGets)},
    {TRIGGER_TALK, ACTOR_LARGE_BREAD, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_TakeTheBread)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TALL_CRATE, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_StartMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TALL_CRATE, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_FinishMovingCrate)},
    {TRIGGER_PUSH, 30, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_StartMovingCrate)},
    {TRIGGER_PUSHED, 30, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_FinishMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_STACKED_CRATES,
     FLAG_BREAD_KNOCKED_DOWN, EVENT(EasternAlhafraShip_KnockDownTheBread)},
    {TRIGGER_TABLE_END},
};

u32 gQuestionMark_Script[] = {
    SC_RELEASE,
};
