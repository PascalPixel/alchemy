/// @file
/// Lemuria's rooms (SceneId_LemuriaRooms), a rooms map behind the city's
/// doors:
///
/// - Piers's uncle's house (LEMURIA_ROOMS_TO_LEMURIA_WEST_A), where Piers comes
///   home to learn his mother has died (LemuriaRooms_PiersVisitsUncle) and his
///   uncle sends a bird with a letter to Lunpa
///   (LemuriaRooms_LeavePiersToMourn);
/// - a house where a girl who has never left Lemuria lives
///   (LEMURIA_ROOMS_TO_LEMURIA_WEST_B);
/// - the Senate's hall (LEMURIA_ROOMS_TO_LEMURIA_WEST_C) and the two rooms
///   behind its arches (exits 4 to 7), where the senators hold to the sealing
///   of Alchemy and scoff at the party's Mind Read;
/// - Lunpa's tower (LEMURIA_ROOMS_TO_LEMURIA_EAST_A to
///   LEMURIA_ROOMS_TO_LEMURIA_EAST_C), its rooms joined by stairs and a ladder
///   (exits 8 to 11, 13 and 14), where Lunpa hears the party out and storms off
///   to the king (LemuriaRooms_MeetLunpa).

#include "flags.h"
#include "game/character_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "version.h"
#include "scene/scene.h"
#include "system/iwram_call.h"
#include "field/field_event.h"
#include "text/msg_ids.h"
#include "scene/cycle_pictures.h"
#include "field/actor_script.h"
#include "field/field_sequence.h"

/// Set once Piers has come home to his uncle
/// (LemuriaRooms_PiersVisitsUncle).
#define FLAG_PIERS_VISITED_UNCLE 0x965
/// Set once an old man of the houses has felt Mind Read: "Was that some form
/// of Psynergy?"
#define FLAG_MIND_READ_NOTICED 0x967

/// The placed actors, numbering the rows of gActorPlacements from
/// ACTOR_FIRST_PLACED; the rows for Jenna, Sheba and Piers go to their party
/// slots.
enum LemuriaRoomsActorId {
  /// "I have never been outside of Lemuria."
  ACTOR_GIRL_WHO_NEVER_LEFT = ACTOR_FIRST_PLACED,
  /// "Now that Piers has returned, maybe he'll share his tales of the outside
  /// world with me."
  ACTOR_WOMAN_AWAITING_PIERS_TALES,
  /// "Piers! You are alive!"
  ACTOR_PIERS_UNCLE,
  /// The uncle's white bird, which carries his letter to Lunpa: "All I need is
  /// a bird to carry it to him in his tower."
  ACTOR_BIRD,
  ACTOR_KRADEN,
  /// The senators, who hold to the sealing of Alchemy. "Are you the warriors
  /// from beyond the Sea of Time...?"
  ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE,
  /// "Long ago, the power of Alchemy was abused..."
  ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE,
  /// "And now, rumors abound of a group of fools..."
  ACTOR_OLD_MAN_ON_THE_RUMORS,
  /// "Come what may, I cannot approve of anyone setting Alchemy loose..."
  ACTOR_OLD_MAN_AGAINST_ALCHEMY,
  /// "Has Lemuria fallen into decay, I ask you!? Nay!"
  ACTOR_OLD_MAN_DENYING_DECAY,
  /// "But... Aren't you Lunpa, the Righteous Thief?"
  ACTOR_LUNPA,
  /// "Do you seek to know more about the draughts...?"
  ACTOR_OLD_MAN_ON_THE_DRAUGHTS
};

/// The cell behaviors the rooms' step triggers answer.
enum LemuriaRoomsCellBehavior {
  /// Inside the door of Piers's uncle's house, where his uncle stops the party
  /// on its way out (LemuriaRooms_LeavePiersToMourn).
  CELL_BEHAVIOR_UNCLES_DOORWAY = 30,
  /// The foot of the stairs in Lunpa's tower, where Lunpa comes to meet the
  /// party (LemuriaRooms_MeetLunpa).
  CELL_BEHAVIOR_FOOT_OF_LUNPAS_STAIRS = 31
};

/// SPRITE_WHITE_BIRD's own animations: standing, now and then stirring, and
/// beating its spread wings.
enum { ANIM_WHITE_BIRD_PERCH = 1, ANIM_WHITE_BIRD_FLAP = 3 };

/// How the rooms open: the Japanese edition opens them through a shape, the
/// others from around the leader.
#if !TLA_PATCH_1
#define ROOM_WINDOW_TRANSITION WINDOW_TRANSITION_SHAPE
#else
#define ROOM_WINDOW_TRANSITION WINDOW_TRANSITION_LEADER
#endif

void LemuriaRooms_PiersVisitsUncle(void);

extern struct SceneRegion gSceneRegions[];
extern u32 gQuestionMark_Script[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return gSceneRegions; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Piers's uncle asks whether the party came with Piers, then, once Piers has
/// visited him, is relieved Piers is alive, and later sends the party to
/// Lunpa's tower.
/// "Are you the travelers that came here with Piers?"
void LemuriaRooms_TalkToPiersUncle(s32 actorId) {
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) != 0) {
    Event_SetMessageId((s32)MsgLemuriaInteriorsGoToLunpasTowerHe);
    // "Go to Lunpa's tower. He will see you."
    Event_ShowMessage(actorId, 0);
    Actor_WatchAndSetScript(ACTOR_PIERS_UNCLE, ACTOR_FELIX);
  } else if (Event_GetFlag(FLAG_PIERS_VISITED_UNCLE) != 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgLemuriaInteriorsIAmSoRelieved);
    // "I am so relieved that Piers has returned to me alive. He is the last..."
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(actorId, FACING_SOUTH_SOUTHEAST, 0);
    Event_End();
  } else {
    Event_SetMessageId((s32)MsgLemuriaInteriorsAreYouTravelersThatCame);
    // "Are you the travelers that came here with Piers?"
    Event_OpenMessage(actorId, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      LemuriaRooms_PiersVisitsUncle();
    } else {
      Event_Wait(20);
      // "No? Ah, interesting. I wonder when Piers will come to see me."
      Event_ShowMessage(actorId, 0);
      Actor_FaceDirection(actorId, FACING_WEST, 0);
    }
  }
}

/// Mind Read on Piers's uncle: his thoughts follow how far Piers's homecoming
/// has gone.
void LemuriaRooms_MindReadPiersUncle(s32 unused, s32 id) {
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
    Event_SetMessageId((s32)MsgLemuriaInteriorsLunpaHasGivenKingTremendous);
    // "Lunpa has given the king tremendous aid on this matter. He will have..."
    Event_ShowMessage(id, 0);
  } else if (Event_GetFlag(FLAG_PIERS_VISITED_UNCLE)) {
    Event_SetMessageId((s32)MsgLemuriaInteriorsPoorPiersBeingBorn);
    // "Poor Piers... Being born in Lemuria and gifted with long life......"
    Event_ShowMessage(id, 0);
  } else {
    Event_SetMessageId((s32)MsgLemuriaInteriorsIWaswhatIsWordSurprised);
    // "I was°what is the word?° surprised when I heard Piers had..."
    Event_ShowMessage(id, 0);
  }
}

/// In the hall of the House of the Senate, an old senator asks whether the
/// party came from beyond the Sea of Time, and scorns either answer.
void LemuriaRooms_AskFromBeyondSeaOfTime(s32 actorId) {

  Event_SetMessageId((s32)MsgLemuriaInteriorsAreYouWarriorsFromBeyond);
  // "Are you the warriors from beyond the Sea of Time who returned with..."
  Event_OpenMessage(actorId, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    Event_Wait(10);
    // "What can you uncivilized louts hope to bring to Lemuria?"
    Event_SetMessageId((s32)MsgLemuriaInteriorsAreYouWarriorsFromBeyond + 1);
  } else {
    Event_Wait(20);
    // "Only savages and barbarians are wont to tell lies when confronted..."
    Event_SetMessageId((s32)MsgLemuriaInteriorsAreYouWarriorsFromBeyond + 2);
  }
  Event_ShowMessage(actorId, 0);
  return;
}

/// In a room behind the Senate hall's arches, a senator offers to tell of the
/// draughts that kept Lemurians long-lived, and tells of them either way.
void LemuriaRooms_AskAboutDraughts(s32 actorId) {

  Event_SetMessageId((s32)MsgLemuriaInteriorsDoYouSeekToKnow);
  // "Do you seek to know more about the draughts that provide us with..."
  Event_OpenMessage(actorId, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    Event_Wait(10);
    // "Once, Lemurians bottled special draughts to leave here and travel..."
    Event_SetMessageId((s32)MsgLemuriaInteriorsDoYouSeekToKnow + 1);
  } else {
    Event_Wait(20);
    // "Then I shall tell you this: So long as we drink of the Lemurian..."
    Event_SetMessageId((s32)MsgLemuriaInteriorsDoYouSeekToKnow + 2);
  }
  Event_ShowMessage(actorId, 0);
  return;
}

/// The first senator Mind Read touches turns on the party, asks whether that
/// was Psynergy, and laughs it off: Lemurians are masters of Psynergy.
void LemuriaRooms_NoticeMindRead(s32 unused, s32 actorId) {
  s32 facing;

  facing = Actor_GetById(actorId)->facing;
  Event_SetFlag(FLAG_MIND_READ_NOTICED);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(actorId, ACTOR_FELIX, 20);
  Event_SetMessageId((s32)MsgLemuriaInteriorsWasThatSomeForm);
  // "Was that some form of Psynergy?"
  Event_OpenMessage(actorId, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    Event_Wait(10);
    // "Ho ho ho! We Lemurians are masters of Psynergy all! Your powers are..."
    Event_SetMessageId((s32)MsgLemuriaInteriorsWasThatSomeForm + 1);
  } else {
    Event_Wait(20);
    // "No? Curious. But it does not matter. Lemurians are masters of..."
    Event_SetMessageId((s32)MsgLemuriaInteriorsWasThatSomeForm + 2);
  }
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirection(actorId, facing, 0);
  Event_Wait(10);
  Event_End();
  return;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets the rooms up: once Piers is at his mother's grave his uncle stands
/// where he sent his bird from, and once Lunpa has gone to the king his tower
/// is empty.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, ROOM_WINDOW_TRANSITION);
  Actor_SetSpritePriority(ACTOR_BIRD, 1);
  Actor_SetSpritePriority(ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE, 2);
  Actor_SetSpritePriority(ACTOR_PIERS_UNCLE, 2);
  actor = Actor_GetById(ACTOR_OLD_MAN_ON_THE_RUMORS);
  actor->motionFlags = 0;
  actor->groundHeight = 0;
  actor->y.fixed = 0;
  actor = Actor_GetById(ACTOR_OLD_MAN_DENYING_DECAY);
  actor->motionFlags = 0;
  actor->groundHeight = 0;
  actor->y.fixed = 0;
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
    Actor_WalkToAndWait(ACTOR_PIERS_UNCLE, 100, 112);
    Actor_GetById(ACTOR_PIERS_UNCLE)->facing = FACING_SOUTH_SOUTHEAST;
    wait(1);
  }
  if (Event_GetFlag(FLAG_MET_LUNPA))
    Actor_SetPosition(ACTOR_LUNPA, 0, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Piers comes home to his uncle, who is relieved to find him alive.
/// "I am sorry it has taken me so long to visit, Uncle."
void LemuriaRooms_PiersVisitsUncle(void) {
  struct Actor *leader;

  Event_SetFlag(FLAG_PIERS_VISITED_UNCLE);
  Party_RemoveMember(PIERS);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriorsIAmSorryItHas);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 112, 128);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 16, -16, FACING_WEST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  // "I am sorry it has taken me so long to visit, Uncle."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
  Event_Wait(20);
  Actor_Jump(ACTOR_PIERS_UNCLE, 6, 15);
  Actor_Jump(ACTOR_PIERS_UNCLE, 6, 23);
  // "Piers! You are alive!"
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  // "Yes, I've managed to survive against a great many trials."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  // "What a fascinating coincidence! We've stumbled across Piers's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I must thank you all for looking after my nephew."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Event_Wait(45);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_Wait(20);
  // "This is Felix and Kraden. They are my friends, and have aided me..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
  Event_Wait(20);
  // "If my sister could only hear how timid young Piers has become a..."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_Wait(35);
  // "Where is my mother? We've been so busy since we arrived that I..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_WalkByAndWait(ACTOR_PIERS, 48, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Event_Wait(40);
  Actor_WalkByAndWait(ACTOR_PIERS, -24, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Now that I think about it, this place is a mess. What happened?..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "...No! She hasn't...fallen ill again, has she?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "There is still illness in Lemuria?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "My mother was born with a weak heart..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
  // "Piers... I'm sorry... Your return comes too late..."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_WalkByAndWait(ACTOR_PIERS, -12, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "After the tidal wave washed you to sea, your mother suddenly fell ill..."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
  Event_Wait(10);
  // "The shock of losing you was too great for her poor heart."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
  // "She held out hope for your safe return until the very end."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "Perhaps your mother gave herself up to ensure your survival..."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
  Event_Wait(10);
  // "Does that not sound like your mother?"
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_LEADER_LOOK_AROUND);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Where is she now?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
  Event_Wait(20);
  // "She rests in the cemetery now. Check the headstones... You will find..."
  Event_ShowMessageThenWait(ACTOR_PIERS_UNCLE, 0, 10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_STAND);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_PIERS, 0.3);
  Actor_GetById(ACTOR_PIERS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_PIERS, 12, 0);
  Event_Wait(1);
  Actor_GetById(ACTOR_PIERS)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(55);
  Actor_WatchAndSetScript(ACTOR_FELIX, ACTOR_PIERS);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_PIERS);
  Actor_WatchAndSetScript(ACTOR_PIERS_UNCLE, ACTOR_PIERS);
  Actor_SetSpeed(ACTOR_PIERS, 2.2);
  Actor_WalkByAndWait(ACTOR_PIERS, -8, 88);
  Actor_Stop(ACTOR_FELIX);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PIERS_UNCLE);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(20);
  // "What should we do, Felix? Should we follow Piers?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    // "After all we've been through, the least we can do is offer Piers..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    gFieldWork->message += 1;
    // "You're right. Piers probably needs some time alone right now."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  }
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
  return;
}

/// Frames of each of the bird's two loops round the room.
#define BIRD_FLIGHT_FRAMES 192

/// Once Piers has gone to his mother's grave, his uncle stops the party on its
/// way out, asks it to leave Piers to his mourning, tells of the mission the
/// king gave Piers, and sends his bird with a letter so that Lunpa will see the
/// party.
void LemuriaRooms_LeavePiersToMourn(void) {
  struct Actor *actor;
  s32 startBuf[3], targetBuf[3], posBuf[3];
  s32 *start, *target, *pos;
  FIXED radius;
  s32 angle;
  s32 result, frame, phase, sine, answer;

  actor = Actor_GetById(ACTOR_BIRD);
  result = Event_GetFlag(FLAG_PIERS_VISITED_UNCLE);
  if (result == 0)
    return;
  Event_SetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE);
  {
    struct Actor *other = Actor_GetById(ACTOR_PIERS_UNCLE);
    other->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriorsWait);
  Actor_SetSpeed(ACTOR_PIERS_UNCLE, 0.8);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Wait..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 136, 152);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
  // "I ask that you leave Piers to his mourning for now..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Now he is struggling with the loss of his mother. He needs time."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(ACTOR_FELIX, 8, 16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
  // "You came to Lemuria to learn more of Alchemy, did you not?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(ACTOR_FELIX, -8, -16);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I suspected as much. Piers was right to bring you here."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
  // "Piers told me his secret... The mission he was given by His Highness..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Even had the tidal wave not carried Piers off, he would have left..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_WalkByAndWait(ACTOR_PIERS_UNCLE, 12, 12);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "You look confused... Piers told you about this, did he not?"
  Event_OpenMessage(ACTOR_PIERS_UNCLE, 0);
  answer = Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (answer == 0) {
    FieldEvent_Wait(30);
    // "Of course... He must trust you a great deal."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                   10);
    gFieldWork->message = (gFieldWork->message) + 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message = (gFieldWork->message) + 1;
    // "Curious... I can't understand why he would do that..."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                   10);
  }
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "If Lord Conservato had not led the senate to oppose him, he would..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  // "I wonder what news Piers has brought with him."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "Either way, I do know why he has brought you here."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Felix... Don't tell me that he hasn't at least told you that much!"
  Event_OpenMessage(ACTOR_PIERS_UNCLE, 0);
  answer = Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (answer == 0) {
    FieldEvent_Wait(30);
    // "You say he's told you, but you look rather confused."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                   10);
    gFieldWork->message = (gFieldWork->message) + 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message = (gFieldWork->message) + 1;
    // "Did Piers tell you nothing before bringing you here to Lemuria?"
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                   10);
  }
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Unfortunately, I know little more than that..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 20);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "While Piers confronts his sorrow over the loss of his mother, you..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_WalkToAndWait(ACTOR_PIERS_UNCLE, 89, 92);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "I shall write you a letter..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS_UNCLE, 2);
  FieldEvent_Wait(10);
  // "So that you can see Lunpa..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_SHAKE_HEAD);
  // "Lunpa has locked himself in his tower and rarely comes out, but if I..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_SetAnimation(ACTOR_PIERS_UNCLE, ANIM_NOD);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_BIRD_CALL);
  FieldEvent_Wait(60);
  FieldActor_StartRepeatedMotion(ACTOR_BIRD, 3);
  Actor_SetAttachedEffect(ACTOR_BIRD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(ACTOR_BIRD, 3);
  Actor_SetAttachedEffect(ACTOR_BIRD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_SetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_FLAP);
  actor->motionFlags = 0;
  start = startBuf;
  start[0] = actor->x.fixed;
  start[1] = actor->y.fixed;
  start[2] = actor->z.fixed;
  target = targetBuf;
  target[0] = toFIXED(116);
  target[1] = 0;
  target[2] = toFIXED(110);
  angle = FACING_NORTH_NORTHWEST;
  for (frame = 0; frame < BIRD_FLIGHT_FRAMES; frame++) {
    phase = frame * (DEGtoANG(180)) / BIRD_FLIGHT_FRAMES;
    radius = Trig_Sin(phase) * 32;
    angle += TRIG_TURN / 128;
    pos = posBuf;
    pos[0] = start[0] + (target[0] - start[0]) * frame / BIRD_FLIGHT_FRAMES;
    sine = Trig_Sin(phase);
    pos[1] = start[1] + (target[1] - start[1]) * frame / BIRD_FLIGHT_FRAMES +
             sine * 56;
    pos[2] = start[2] + (target[2] - start[2]) * frame / BIRD_FLIGHT_FRAMES;
    Vector_AddPolarOffset(radius, angle, pos);
    actor->x.fixed = pos[0];
    actor->y.fixed = pos[1];
    actor->z.fixed = pos[2];
    actor->facing = angle + DEGtoANG(90);
    if (frame == 128) {
      Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
    }
    System_Wait(1);
  }
  actor->facing = FACING_WEST;
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_PERCH);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS_UNCLE, 0.8);
  Actor_WalkToAndWait(ACTOR_PIERS_UNCLE, 100, 112);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS_UNCLE, 0.4);
  Actor_WalkByAndWait(ACTOR_PIERS_UNCLE, 8, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_FLAP);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_PERCH);
  FieldEvent_Wait(20);
  {
    struct Actor *other = Actor_GetById(ACTOR_PIERS_UNCLE);
    other->steeringFlags &= ~ACTOR_FACES_MOTION;
  }
  Actor_WalkByAndWait(ACTOR_PIERS_UNCLE, -8, 0);
  FieldEvent_Wait(1);
  {
    struct Actor *other = Actor_GetById(ACTOR_PIERS_UNCLE);
    other->steeringFlags |= ACTOR_FACES_MOTION;
  }
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "All I need is a bird to carry it to him in his tower."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS_UNCLE, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_SetAnimation(ACTOR_BIRD, ANIM_WHITE_BIRD_FLAP);
  FieldEvent_Wait(40);
  actor->motionFlags = 0;
  start[0] = actor->x.fixed;
  start[1] = actor->y.fixed;
  start[2] = actor->z.fixed;
  target[0] = toFIXED(132);
  target[1] = toFIXED(32);
  target[2] = toFIXED(100);
  angle = FACING_SOUTH + TRIG_TURN / 128;
  for (frame = 0; frame < BIRD_FLIGHT_FRAMES; frame++) {
    radius = Trig_Sin(frame * (DEGtoANG(90)) / BIRD_FLIGHT_FRAMES) * 40;
    angle += TRIG_TURN * 3 / 256;
    pos[0] = start[0] + (target[0] - start[0]) * frame / BIRD_FLIGHT_FRAMES;
    sine = Trig_Sin(frame * (DEGtoANG(180)) / BIRD_FLIGHT_FRAMES);
    pos[1] = start[1] + (target[1] - start[1]) * frame / BIRD_FLIGHT_FRAMES +
             (sine <<= 5);
    pos[2] = start[2] + (target[2] - start[2]) * frame / BIRD_FLIGHT_FRAMES;
    Vector_AddPolarOffset(radius, angle, pos);
    actor->x.fixed = pos[0];
    actor->y.fixed = pos[1];
    actor->z.fixed = pos[2];
    actor->facing = angle + DEGtoANG(90);
    if (frame == 153) {
      Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_WEST_SOUTHWEST, 0);
    }
    System_Wait(1);
  }
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_NORTH_NORTHWEST, 0);
  actor->facing = FACING_NORTH_NORTHEAST;
  start[0] = actor->x.fixed;
  start[1] = actor->y.fixed;
  start[2] = actor->z.fixed;
  target[0] = toFIXED(90);
  target[1] = toFIXED(4);
  target[2] = toFIXED(64);
  for (frame = 0; frame < 16; frame++) {
    actor->x.fixed = start[0] + (target[0] - start[0]) * frame / 16;
    actor->y.fixed = start[1] + (target[1] - start[1]) * frame / 16;
    actor->z.fixed = start[2] + (target[2] - start[2]) * frame / 16;
    System_Wait(1);
  }
  Actor_SetPosition(ACTOR_BIRD, 0, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS_UNCLE, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS_UNCLE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "There. You should be able to see him now."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_UNCLE, 0,
                                 10);
  FieldActor_SetSpritePriority(ACTOR_PIERS_UNCLE, 2);
  Event_End();
}

void Actor_StartScript(struct Actor *actor, const void *script);

#include "shared/question_mark_pop.h"

#include "shared/actor_pop_question_marks.h"

#include "shared/actor_cycle_pictures.h"

/// Lunpa comes to meet the party in his tower and asks after Piers, learns from
/// Kraden that his old friend Babi has died and that his grandson Dodonpa rules
/// the village he settled, hears Kraden's theory of the world's decay, and
/// bursts his stuck door open to take the party to the king.
void LemuriaRooms_MeetLunpa(void) {
  struct Actor *leader;
  struct Actor *actor;
  struct Actor *target;
  s32 result;

  Event_SetFlag(FLAG_MET_LUNPA);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriorsHonestlyImAmazedThatAnyone);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_SetSpeed(ACTOR_LUNPA, 1);
  Actor_WalkByAndWait(ACTOR_LUNPA, 32, 0);
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, -32);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  // "Honestly, I'm amazed that anyone can travel so far across the world..."
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 440, 352);
  Actor_SetPosition(ACTOR_FELIX, 440, 352);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_SOUTH_SOUTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -32, 0, FACING_SOUTH);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Event_Wait(10);
  FieldCamera_MoveTo(432, CAMERA_KEEP, 368, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Say, Piers isn't with you! Er...why not?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "His mother passed away... He's gone to visit her grave."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  // "Of course... His mother... He loved her dearly. A terrible loss......"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  // "Well, Piers may not be here, but we still have much to discuss."
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "If Piers brought you, you must be working to break the seal on..."
  Event_OpenMessage(ACTOR_LUNPA, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result == 0) {
    Event_Wait(20);
    // "Ah, Piers has done quite well in finding you!"
    Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    gFieldWork->message++;
    // "That doesn't make sense... He wouldn't have brought you to Lemuria..."
    Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What mission could the good King Hydros have assigned to Piers?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What? Piers has told you nothing of his mission?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_StartRepeatedMotion(ACTOR_LUNPA, 3);
  Actor_SetAttachedEffect(ACTOR_LUNPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "...Really?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  Event_Wait(15);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "So you actually have nothing at all to do with Alchemy!?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Our parents were kidnapped by the Fire Clan, far to the north. We are..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "King Hydros told me that Venus and Mercury have been ignited once..."
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
  // "I'm afraid I know nothing of any northern "Fire Clan.""
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 20);
  Actor_WalkByAndWait(ACTOR_LUNPA, 12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "But that doesn't explain you, sir. You're clearly not related to..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  // "Kraden joined us on this quest on behalf of Lord Babi of Tolbi."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetSpeed(ACTOR_LUNPA, 2);
  Actor_WalkByAndWait(ACTOR_LUNPA, -12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  // "...Babi?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I only know of one man named Babi... But it couldn't..."
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Master Lunpa, the Babi we speak of is the same man you knew long ago."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  // "Ah! So...he's still out there, living in the outside world?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Well, that might not be the most accurate way of putting it."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_LUNPA, 0.8);
  Actor_WalkByAndWait(ACTOR_LUNPA, 12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Most...accurate? My, you certainly have an odd way of turning a phrase..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "He was living, yes... Until recently, that is..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(10);
  // "So, Babi passed away... But only recently, you say?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "When he finally ran out of his mystic draughts, his spirit began to wane."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
  // "Then...I no longer know anyone in the world outside."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "But... Aren't you Lunpa, the Righteous Thief?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_WalkByAndWait(ACTOR_LUNPA, -12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "I was called that once, long, long ago. Why do you ask?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I'll bet there's at least one person in the outside world whom you..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_PopQuestionMarks(ACTOR_LUNPA);
  Event_PlaySound(SE_LIGHT_APPEAR);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(25);
  // "Who would that be?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "There's a town called Lunpa in northern Angara. A man named Donpa..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "I settled that village! And Donpa... He is my son..."
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Now, your grandson, Dodonpa, rules over your village."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetSpeed(ACTOR_LUNPA, 2);
  Actor_WalkByAndWait(ACTOR_LUNPA, 12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Rules? Rules is an ill- sounding word."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "But it's an accurate one, Lunpa. Dodonpa is an evil thief, who has..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_WalkByAndWait(ACTOR_LUNPA, -12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_LUNPA, 6, 15);
  Actor_Jump(ACTOR_LUNPA, 6, 23);
  // "What has become of my son? How could he permit such a thing to happen?"
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Master Lunpa, don't you have any idea how old Donpa is?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "He is as I am. An old man... He may be older even than I."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  FieldActor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  // "If this news outrages you, why don't you go and set Dodonpa straight?"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "If only I could..."
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(20);
  // "Is this the only reason we're here? To bring news of the outside world?"
  Event_OpenMessage(ACTOR_JENNA, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result == 0) {
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
    Event_Wait(10);
    // "You are kind, Felix, but there are much more important matters at hand."
    Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
    gFieldWork->message++;
    // "Yes, yes... I am sorry we were sidetracked. We have much more to..."
    Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 10);
  }
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "But before we begin, there is one thing I must say..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(25);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "You don't mind, do you, Felix?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result == 0) {
    Event_Wait(20);
    // "Clearly, your judgment on this matter is fogged, Felix. I must speak."
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    gFieldWork->message++;
    // "I knew you'd agree."
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST - 1, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "I have discussed this with none but Felix, but...I have my own..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST - 1, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "What do you mean?"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "Only that I have my own reasons for wanting to see the beacons lit..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST - 1, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(30);
  // "Why are you telling us all of this now?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "Because I feel this quest is deeply linked to Lemuria."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  // "I thought it odd that Lord Babi should order me to research Alchemy."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Hey, wait a minute... Is this what you promised to tell us once we..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "Yes, yes... May I continue?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 0.6);
  Actor_WalkByAndWait(ACTOR_KRADEN, 12, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 12, 0);
  Event_Wait(10);
  // "Long ago, before Alchemy's power was sealed... There were many..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "At the moment that Alchemy was sealed away, most of those..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "I have formed certain theories now, suggesting that this may be..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_LUNPA, 3);
  Actor_SetAttachedEffect(ACTOR_LUNPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Let me get this straight... You felt you couldn't tell us this until..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST + 1, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "And you only wanted to come to Lemuria to test your theories and see..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "But even if everything you said is true, what does it all mean?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_LUNPA, 1);
  Actor_WalkByAndWait(ACTOR_LUNPA, 12, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  // "If your theories are correct, the world itself will wane and..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Piers never spoke of this to you, did he?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_WalkByAndWait(ACTOR_KRADEN, -12, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(10);
  // "If you came to this theory on your own, you are indeed a wise..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What do you mean?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "King Hydros and I, as well as others in Lemuria, hold the same..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "You do!?!"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(25);
  // "Civilizations thrive by building upon the knowledge of the past."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(10);
  // "Knowledge gained in one generation should be taken even further by..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
  // "But where is the world today?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  // "The great civilizations of old have all withered and vanished. We are..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(30);
  // "You have encountered many cultures in your travels. Are any of them..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_SHAKE_HEAD);
  // "No, they are not... But this is not the only sign of the decay you..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  // "I think it best that you hear the rest directly from His Majesty..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST + FACING_NUDGE, 0);
  Event_Wait(20);
  // "Yes, Kraden! We shall go to visit the king!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Ah, but King Hydros is in counsel with Lord Conservato. We might not..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 42);
  Actor_Jump(ACTOR_LUNPA, 6, 15);
  Actor_Jump(ACTOR_LUNPA, 6, 23);
  // "Conservato? That fool. He refuses to acknowledge what he can see with..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_WalkByAndWait(ACTOR_LUNPA, -2, -12);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(10);
  // "I will meet with the king and press our evidence upon Conservato...."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetSpeed(ACTOR_LUNPA, 1.6);
  Actor_WalkByAndWait(ACTOR_LUNPA, -8, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, 20);
  MapGrid_SetShake(1, 1, 1);
  Actor_SetSpeed(ACTOR_LUNPA, 1.9);
  Event_PlaySound(SE_IMPACT);
  Actor_Jump(ACTOR_LUNPA, 2, 0);
  Actor_GetById(ACTOR_LUNPA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, -12);
  Event_Wait(1);
  Actor_GetById(ACTOR_LUNPA)->steeringFlags |= ACTOR_FACES_MOTION;
  MapGrid_SetShake(0, 0, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Gah! Stupid door!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetSpeed(ACTOR_LUNPA, 0.6);
  Actor_GetById(ACTOR_LUNPA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, -24);
  Event_Wait(1);
  Actor_GetById(ACTOR_LUNPA)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, ANIM_NOD);
  Event_Wait(20);
  Actor_Jump(ACTOR_LUNPA, 6, 15);
  Actor_Jump(ACTOR_LUNPA, 6, 23);
  actor = Actor_GetById(ACTOR_LUNPA);
  actor->timer = 0;
  actor->update = (ObjectUpdate)Actor_CyclePictures;
  // "HAHHH-AHH!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_SetSpeed(ACTOR_LUNPA, 2.2);
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, 32);
  target = Actor_GetById(ACTOR_LUNPA);
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_SetShake(4, 4, 1);
  Actor_GetById(ACTOR_LUNPA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, -16);
  Event_Wait(1);
  Actor_GetById(ACTOR_LUNPA)->steeringFlags |= ACTOR_FACES_MOTION;
  target->timer = 3;
  wait(1);
  target->update = NULL;
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_LUNPA), 1);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Pah! At last, you are open!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(25);
  // "To the king's chambers!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_LUNPA, 0, 10);
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, 48);
  Actor_SetPosition(ACTOR_LUNPA, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Lunpa ought to be as old as Lord Babi was, but he has so much energy!"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "He's got a fire in him."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_Jump(ACTOR_KRADEN, 6, 15);
  Actor_Jump(ACTOR_KRADEN, 6, 23);
  // "I do wish I could have heard more about Lord Babi, though."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(15);
  // "But come, Felix! To the king's palace we go!"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_SHEBA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Event_End();
}

#include "shared/scene_entrances_default.c"

/// The tower's ways between floors: the top of the stairs in the room below,
/// and the ladder down through the floor of the room above.
struct SceneRegion gSceneRegions[] = {
    {416, 32, 592, 432, 48, 608, CONDITION_ALWAYS, 13},
    {688, -24, 608, 704, -8, 624, CONDITION_ALWAYS, 14},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemuriaRooms),
    SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_WEST_A, Lemuria,
                      LEMURIA_FROM_LEMURIA_ROOMS_WEST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_WEST_B, Lemuria,
                      LEMURIA_FROM_LEMURIA_ROOMS_WEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_WEST_C, Lemuria,
                      LEMURIA_FROM_LEMURIA_ROOMS_WEST_C, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LemuriaRooms, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaRooms, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, LemuriaRooms, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, LemuriaRooms, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LemuriaRooms, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemuriaRooms, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, LemuriaRooms, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, LemuriaRooms, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_EAST_A, Lemuria,
                      LEMURIA_FROM_LEMURIA_ROOMS_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, LemuriaRooms, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, LemuriaRooms, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_EAST_B, Lemuria,
                      LEMURIA_FROM_LEMURIA_ROOMS_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ROOMS_TO_LEMURIA_EAST_C, Lemuria,
                      LEMURIA_FROM_LEMURIA_ROOMS_EAST_C, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 480, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 588, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 552, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 328, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 552, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LUNPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 416, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 792, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

/// The ways out, the steps that start Piers's uncle's letter and Lunpa's
/// talk, the people talked to and read, and the bookcases.
struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, LEMURIA_ROOMS_TO_LEMURIA_WEST_A, CONDITION_ALWAYS,
     LEMURIA_ROOMS_TO_LEMURIA_WEST_A},
    {TRIGGER_EXIT, LEMURIA_ROOMS_TO_LEMURIA_WEST_B, CONDITION_ALWAYS,
     LEMURIA_ROOMS_TO_LEMURIA_WEST_B},
    {TRIGGER_EXIT, LEMURIA_ROOMS_TO_LEMURIA_WEST_C, CONDITION_ALWAYS,
     LEMURIA_ROOMS_TO_LEMURIA_WEST_C},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, LEMURIA_ROOMS_TO_LEMURIA_EAST_A, CONDITION_ALWAYS,
     LEMURIA_ROOMS_TO_LEMURIA_EAST_A},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH,
     LEMURIA_ROOMS_TO_LEMURIA_EAST_B, CONDITION_ALWAYS,
     LEMURIA_ROOMS_TO_LEMURIA_EAST_B},
    {TRIGGER_EXIT, LEMURIA_ROOMS_TO_LEMURIA_EAST_C, CONDITION_ALWAYS,
     LEMURIA_ROOMS_TO_LEMURIA_EAST_C},
    {TRIGGER_STEP, CELL_BEHAVIOR_UNCLES_DOORWAY, FLAG_PIERS_AT_MOTHERS_GRAVE,
     EVENT(LemuriaRooms_LeavePiersToMourn)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FOOT_OF_LUNPAS_STAIRS, FLAG_MET_LUNPA,
     EVENT(LemuriaRooms_MeetLunpa)},
    {TRIGGER_TALK, ACTOR_GIRL_WHO_NEVER_LEFT, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriorsIHaveNeverBeenOutside)},
    {TRIGGER_TALK, ACTOR_WOMAN_AWAITING_PIERS_TALES, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriorsNowThatPiersHasReturned)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WHO_NEVER_LEFT,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsWhoKnowsWhatWondersLie)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_AWAITING_PIERS_TALES,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsSenatePassedLawThatProhibits)},
    {TRIGGER_TALK, ACTOR_PIERS_UNCLE, CONDITION_ALWAYS,
     EVENT(LemuriaRooms_TalkToPiersUncle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS_UNCLE, CONDITION_ALWAYS,
     EVENT(LemuriaRooms_MindReadPiersUncle)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE, CONDITION_ALWAYS,
     EVENT(LemuriaRooms_AskFromBeyondSeaOfTime)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriorsLongAgoPowerOfAlchemy)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RUMORS, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriorsAndNowRumorsAbound)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_AGAINST_ALCHEMY, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriorsComeWhatMayICannot)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_DENYING_DECAY, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriorsHasLemuriaFallenIntoDecay)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_DRAUGHTS, CONDITION_ALWAYS,
     EVENT(LemuriaRooms_AskAboutDraughts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_MIND_READ_NOTICED, EVENT(LemuriaRooms_NoticeMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_MIND_READ_NOTICED, EVENT(LemuriaRooms_NoticeMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_ON_THE_RUMORS | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_MIND_READ_NOTICED, EVENT(LemuriaRooms_NoticeMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_AGAINST_ALCHEMY | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_MIND_READ_NOTICED, EVENT(LemuriaRooms_NoticeMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_DENYING_DECAY | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_MIND_READ_NOTICED, EVENT(LemuriaRooms_NoticeMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_ON_THE_DRAUGHTS | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_MIND_READ_NOTICED, EVENT(LemuriaRooms_NoticeMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ASKING_OF_THE_OUTSIDE,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsWhatDoesHydrosThinkWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_ALCHEMYS_ABUSE,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsEverSinceAlchemyWasSealed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RUMORS,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsIfAlchemyWereLoosedUpon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AGAINST_ALCHEMY,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsAllOfMembersOfSenate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_DENYING_DECAY,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsCertainlyLemuriaHasLostSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_DRAUGHTS,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriorsEvenThoughWeNoLonger)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsTheresBookCalledGreatAdventures))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsIFoundLetterFromBabi))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledHistory))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS,
     FIND_MESSAGE(
         MSGNO(MsgLemuriaInteriorsHeresBookCalledElementalLighthouses))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledGreatLemurian))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledAlchemy))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledAlchemyAndAncient))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 207, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledDiary))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 208, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsWhatInWhatsWithAll))},
    {TRIGGER_TABLE_END},
};

/// The script each "?" bubble runs: it is released once its frames are up.
u32 gQuestionMark_Script[] = {
    SC_RELEASE,
};
