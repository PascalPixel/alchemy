/// @file
/// Champa, the pirates' village on the coast of the Great Western Sea, whose
/// people are fed once Briggs returns with the Ankohl jewels. Two of its
/// sailors spot Felix and run to warn Briggs (Champa_SailorsSpotFelix); at the
/// cliff where Obaba lives Alex, who had left Alhafra before them, meets the
/// party again (Champa_MeetAlex), Jenna and Sheba glaring at each other on the
/// way (Champa_JennaAndShebaGlare). In a game linked with The Broken Seal,
/// Feizhi arrives with a gift (Champa_FeizhiArrives). The getters pick later
/// tables once Briggs has sailed from Alhafra.

#include "system/task.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "system/io_reg.h"
#include "item_ids.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "field/object.h"
#include "scene/scene.h"
#include "window/window.h"
#include "field/actor.h"
#include "field/call.h"
#include "version.h"
#include "system/frame.h"
#include "field/actor_script.h"
#include "fixed_math.h"

struct Actor;
extern struct ActorPlacement gChampa_ActorPlacementsOnceBriggsSailed[];
extern s32 gChampa_Doors[];
s32 Event_PickUpItem();
extern struct Trigger gChampa_Triggers[];
extern struct Trigger gChampa_TriggersOnceBriggsSailed[];
extern s16 gChampa_NoDoorEdit[];
extern s16 gChampa_DoorEdit[];
s32 Tightrope_Place();
s32 Window_CloseMatchingId();

/// Sends an actor walking to where another stands, if it is on the map.
static inline void Actor_HeadToward(s32 id, s32 target) {
  struct Actor *actor = Actor_Get(target);

  if (actor != NULL)
    Actor_SetDestination(id, actor->x.part[0].integer,
                         actor->z.part[0].integer);
}

/// The town's own story, which no other scene reads.
#define FLAG_NEAR_THE_CLIFF 0x970 ///< the party walked up to Obaba's cliff
#define FLAG_TOLD_OF_OBABA 0x972  ///< a villager told of Obaba
#define FLAG_SAILORS_SPOTTED_FELIX 0x973 ///< two sailors ran to warn Briggs
#define FLAG_FEIZHIS_GIFT_WAITING 0xa74  ///< Feizhi holds out her gift
#define FLAG_FEIZHI_CAME 0xa75 ///< Feizhi came, taking Felix for a friend

/// Champa's people and things, numbering the rows of gChampa_ActorPlacements
/// whose sprites are not the party's; gChampa_ActorPlacementsOnceBriggsSailed
/// keeps them and adds the sailors who spot Felix.
enum ChampaActorId {
  /// The ends of the tightrope over the gap (Tightrope_Place).
  ///@{
  ACTOR_TIGHTROPE_START = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPE_END,
  ///@}
  /// The village's two flags, Champa's anchor on red.
  ///@{
  ACTOR_FLAG,
  ACTOR_SECOND_FLAG,
  ///@}
  /// "Oh, I'm so hungry... And there won't be anything to eat until..."
  ACTOR_HUNGRY_YOUNG_MAN,
  /// "When is Briggs going to return with food for the village?"
  ACTOR_WOMAN_WAITING_FOR_FOOD,
  /// "How did you find your way here, to the edge of Angara?"
  ACTOR_MAN_AT_THE_EDGE_OF_ANGARA,
  /// "Champa is impoverished, but soon, we will prosper again."
  ACTOR_WOMAN_ON_PROSPERITY,
  /// The old man who asks about Ankohl Ruins.
  ACTOR_OLD_MAN_ON_ANKOHL_RUINS,
  /// "The ruins at Ankohl are all that remains of a great sanctum..."
  ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS,
  /// The boy practicing Psynergy: "If I could pick up things without using
  /// my hands..."
  ACTOR_PRACTICING_BOY,
  /// "Once Briggs comes back, I can play with Eoleo again..."
  ACTOR_EOLEOS_FRIEND,
  /// Sailors at the cliff caves: "The cliff caves are a lot quieter with
  /// Briggs gone..."
  ACTOR_SAILOR_AT_THE_QUIET_CAVES,
  /// "Our matriarch is the last successor of the Ankohl."
  ACTOR_SAILOR_ON_THE_MATRIARCH,
  /// The sailor who tells strangers they are not welcome.
  ACTOR_UNWELCOMING_SAILOR,
  /// A rock drawn at half size.
  ACTOR_ROCK,
  /// "I bet you're wondering why only sailors and their families..."
  ACTOR_WOMAN_ON_THE_CLIFF,
  /// Who meet the party on the cliff (Champa_MeetAlex).
  ACTOR_ALEX,
  ACTOR_KARST,
  ACTOR_AGATIO,
  ACTOR_KRADEN,
  /// Feizhi, in a game linked with The Broken Seal (Champa_FeizhiArrives),
  /// and Kraden as he meets her.
  ACTOR_FEIZHI,
  ACTOR_KRADEN_MEETING_FEIZHI,
  /// Feizhi's gift, until it is taken (Champa_TakeFeizhisGift).
  ACTOR_FEIZHIS_GIFT,
  /// Once Briggs has sailed: "Whoa! You... You're Felix!"
  ACTOR_SAILOR_WHO_SPOTS_FELIX,
  /// "What? Really? It's Felix?"
  ACTOR_SECOND_SAILOR,
  ACTOR_BRIGGS
};

/// The chest Reveal shows by the six gravestones, holding the Viking Helm.
#define ACTOR_VIKING_HELM_CHEST 64

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gChampa_ActorPlacements[];

const s16 *Scene_GetActorPlacements(void) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampa_ActorPlacementsOnceBriggsSailed;
  return gChampa_ActorPlacements;
}

/// The leader's update while it walks into one of the two cliff doorways: it
/// shrinks by 3/128 of its size each frame.
void Champa_ShrinkIntoDoorway(struct Actor *actor) {
  actor->scaleX += -(FIXED_ONE * 3 / 128);
  actor->scaleY += -(FIXED_ONE * 3 / 128);
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void Champa_OpenDoorAndEnter(const void *warp, s32 door, s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  id = gMainRoutineWork.leaderActorId;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void TilesetAnim_Resume(s32 a);

/// Walking into a door: the cliff doorways (1 and 2) shrink the leader as it
/// goes; door 7 starts its animated tiles and edits the map as it opens; the
/// others open plainly.
void Champa_EnterDoor(s32 door) {
  s32 *table = gChampa_Doors;

  if ((u32)(door - 1) <= 1)
    Actor_Get(gMainRoutineWork.leaderActorId)->update =
        (void (*)(void))Champa_ShrinkIntoDoorway;
  if (door == 7) {
    TilesetAnim_Resume(8);
    Champa_OpenDoorAndEnter(table, 7, 1);
  } else {
    Champa_OpenDoorAndEnter(table + 2, door, 0);
  }
}

/// Reveal by the six gravestones: the Viking Helm's chest stands out of the
/// ground.
void Champa_RevealTreasureSpot(void) {

  TreasureSpot_SetPosition(ACTOR_VIKING_HELM_CHEST, toFIXED(392), toFIXED(568));
  Actor_Get(ACTOR_VIKING_HELM_CHEST)->motionFlags = 0;
  Actor_Get(ACTOR_VIKING_HELM_CHEST)->y.fixed = toFIXED(32);
}

/// Reveal ends: the Viking Helm's chest goes back under the ground.
void Champa_HideTreasureSpot(void) {
  TreasureSpot_SetPosition(ACTOR_VIKING_HELM_CHEST, 0, 0);
}

#include "text/msg_ids.h"

/// Once Briggs has sailed, two sailors spot Felix and run to warn Briggs, who
/// flees from his house.
void Champa_SailorsSpotFelix(void) {

  Event_SetFlag(FLAG_SAILORS_SPOTTED_FELIX);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgChampaWhoaYouYoureFelix);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_Jump(ACTOR_SAILOR_WHO_SPOTS_FELIX, 6, 20);
  // "Whoa! You... You're Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 5);
  FieldCamera_MoveTo(176, CAMERA_KEEP, 256, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(ACTOR_SAILOR_WHO_SPOTS_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_SAILOR_WHO_SPOTS_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_SECOND_SAILOR, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SAILOR, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
  Actor_FaceDirectionAndWait(ACTOR_SECOND_SAILOR, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SAILOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "What? Really? It's Felix?"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_SAILOR, 0, 5);
  FieldActor_Jump(ACTOR_SAILOR_WHO_SPOTS_FELIX, 6, 0);
  FieldActor_Jump(ACTOR_SECOND_SAILOR, 6, 0);
  Actor_SetSpeed(ACTOR_SAILOR_WHO_SPOTS_FELIX, 1.8);
  Actor_SetSpeed(ACTOR_SECOND_SAILOR, 1.8);
  Actor_Get(ACTOR_SAILOR_WHO_SPOTS_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, -16);
  Actor_Get(ACTOR_SECOND_SAILOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SECOND_SAILOR, 0, -16);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_SECOND_SAILOR)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(15);
  Actor_ShowEmote(ACTOR_SAILOR_WHO_SPOTS_FELIX,
                  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "You're awfully persistent, aren't you?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 5);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SECOND_SAILOR, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(5);
  // "Briggs, get outta here! Felix is here to finish you off!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_SECOND_SAILOR, 0,
                                 5);
  // "What was that!? Felix? Here?"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  FieldActor_WatchAndSetScript(ACTOR_SAILOR_WHO_SPOTS_FELIX, ACTOR_BRIGGS);
  FieldActor_WatchAndSetScript(ACTOR_SECOND_SAILOR, ACTOR_BRIGGS);
  Actor_SetSpeed(ACTOR_BRIGGS, 1.2);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 32);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST);
  FieldCamera_MoveTo(208, CAMERA_KEEP, 296, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(15);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 15);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 23);
  // "Oh no! It's really him! I've got to get out of here!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 20);
  Actor_SetSpeed(ACTOR_BRIGGS, 2);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -48);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SAILOR, FACING_NORTH_NORTHEAST, 0);
  Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
  Actor_SetSpeed(ACTOR_SAILOR_WHO_SPOTS_FELIX, 2);
  Actor_SetSpeed(ACTOR_SECOND_SAILOR, 2);
  Actor_WalkByAndWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, -16, -16);
  Actor_WalkBy(ACTOR_SECOND_SAILOR, 16, -16);
  Actor_WalkByAndWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, -16);
  Actor_SetPosition(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_SAILOR, 0, -16);
  Actor_SetPosition(ACTOR_SECOND_SAILOR, 0, 0);
  Event_End();
}

/// The boy practicing Psynergy stands up straight, tells of Eoleo's strange
/// ability and goes back to squashing himself.
void Champa_TalkToPracticingBoy(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);

  actor->scaleY = FIXED_ONE;
  actor->scaleX = FIXED_ONE;
  FieldEvent_SetMessageId((s32)MsgChampaEoleoBriggssSonHasA);
  // "Eoleo, Briggs's son, has a really strange ability... I'll bet if I..."
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirectionAndWait(actorId, FACING_EAST);
  Actor_SetMovementType(actor, MOVEMENT_TYPE_SQUASH);
}

/// A villager asks whether the party came to Champa for Briggs's treasure, and
/// answers the yes or the no.
void Champa_TellOfBriggsTreasure(s32 actor) {
  s32 msg = (s32)&MsgChampaHeyYouJustCameTo;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// The old man asks whether the party has been to Ankohl Ruins, or, once Briggs
/// has sailed, whether they would know why he took so long to return.
void Champa_AskAboutAnkohlRuins(s32 actor) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    FieldEvent_SetMessageId((s32)MsgChampaBriggsTookALongTime);
    // "Briggs took a long time to return to us. You want to know why?"
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampaBriggsTookALongTime + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampaBriggsTookALongTime + 2);
    }
    Event_ShowMessage(actor, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampaYouveTheLookOfA);
    // "You've the look of a traveler about you... Have you been to Ankohl..."
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampaYouveTheLookOfA + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampaYouveTheLookOfA + 2);
    }
    Event_ShowMessage(actor, 0);
  }
}

/// A villager tells of Obaba, the last of the Ankohl, the first time, and then
/// of how she acts since Briggs came back.
void Champa_TellOfObaba(s32 actor) {

  if (!Event_GetFlag(FLAG_TOLD_OF_OBABA)) {
    Event_SetFlag(FLAG_TOLD_OF_OBABA);
    FieldEvent_SetMessageId((s32)MsgChampaObabaIsTheLastPossessor);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampaYoudThinkObabaWouldBe);
  }
  // "You'd think Obaba would be glad that Briggs was back. She acts like..."
  Event_ShowMessage(actor, 0);
}

/// The sailor tells strangers that Champa's sailors are not welcoming, and
/// answers the yes or the no.
void Champa_SailorsNotWelcome(s32 actor) {
  s32 msg = (s32)&MsgChampaTheChampaSailorsArentWell;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// Feizhi's gift taken: the Golden Ring, which stays if the party cannot carry
/// it.
void Champa_TakeFeizhisGift(s32 actor) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (!Value3(Event_PickUpItem, actor, ITEM_GOLDEN_RING, -1)) {
    Event_ClearFlag(FLAG_FEIZHIS_GIFT_WAITING);
  }
  Event_End();
}

/// Sets Feizhi's gift, the Golden Ring, where she left it, and marks it
/// waiting.
void Champa_FeizhiHoldsHerGift(void) {
  FieldActor_ShowItemIconById(ACTOR_FEIZHIS_GIFT, ITEM_GOLDEN_RING);
  {
    Actor_SetPositionAndFacing(ACTOR_FEIZHIS_GIFT, toFIXED(312), toFIXED(593),
                               FACING_NORTH);
  }
  Event_SetFlag(FLAG_FEIZHIS_GIFT_WAITING);
}

/// With Hsu rescued in The Broken Seal's data, Feizhi comes to Champa taking
/// Felix for Isaac, talks with Kraden and leaves her gift; from patch 1 not
/// once Isaac is in the party.
void Champa_FeizhiArrives(void) {
#if TLA_PATCH_1
#endif

  if (!Event_GetFlag(FLAG_TRANSFERRED_HSU_RESCUED_BY_PARTY)) {
  } else {
#if TLA_PATCH_1
    // Once Isaac is in the party Feizhi no longer comes; the Japanese edition
    // has no such test.
    if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_ISAAC))) {
    } else
#endif
    {
      Event_SetFlag(FLAG_FEIZHI_CAME);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgChampaIsaac);
      {
        Actor_SetPositionAndFacing(ACTOR_FEIZHI, toFIXED(314), toFIXED(679),
                                   FACING_NORTH);
      }
      // "Isaac!"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_WalkToAndWait(ACTOR_FELIX, 314, 580);
      Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
      FieldEvent_Wait(10);
      EventCamera_FollowActor(ACTOR_FEIZHI, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(40);
      Actor_WalkToAndWait(ACTOR_FEIZHI, 314, 624);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
      // "Isaac, is it you? Do my eyes deceive me?"
      Event_OpenMessage(ACTOR_FEIZHI, 0);
      if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
        FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
        gFieldWork->message = (gFieldWork->message + 1);
      } else {
        gFieldWork->message = (gFieldWork->message + 1);
        FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      }
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      FieldActor_LaunchFromLeader(ACTOR_KRADEN_MEETING_FEIZHI, 16, 0,
                                  FACING_SOUTH);
      FieldActor_WaitForMoveById(ACTOR_KRADEN_MEETING_FEIZHI);
      FieldEvent_Wait(20);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, 3);
      FieldEvent_Wait(10);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_FaceEachOther(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(ACTOR_KRADEN_MEETING_FEIZHI, FACING_SOUTH, 0);
      Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, 4);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
      FieldActor_Jump(ACTOR_FEIZHI, 2, 0);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE,
                      30);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
      FieldEvent_Wait(10);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE,
                      30);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
      FieldEvent_Wait(10);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, 3);
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Champa_FeizhiHoldsHerGift();
      Actor_FaceEachOther(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(ACTOR_KRADEN_MEETING_FEIZHI, FACING_SOUTH, 0);
      Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_FEIZHI, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_FEIZHI, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, 3);
      FieldEvent_Wait(10);
      Actor_SetAnimationAndWait(ACTOR_FEIZHI, 3);
      FieldEvent_Wait(10);
      EventCamera_FollowActor(ACTOR_FELIX, 1);
      Actor_SetSpeed(ACTOR_FEIZHI, 1.2);
      Actor_WalkBy(ACTOR_FEIZHI, 0, 100);
      FieldEvent_Wait(10);
      Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN_MEETING_FEIZHI, 0);
      FieldEvent_Wait(30);
      Actor_SetAnimation(ACTOR_KRADEN_MEETING_FEIZHI, 3);
      Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
      FieldEvent_Wait(30);
      Actor_SetAnimation(ACTOR_KRADEN_MEETING_FEIZHI, 2);
      Actor_HeadToward(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX);
      FieldActor_WaitForMoveById(ACTOR_KRADEN_MEETING_FEIZHI);
      Actor_SetPosition(ACTOR_KRADEN_MEETING_FEIZHI, 0, 0);
      Actor_SetPosition(ACTOR_FEIZHI, 0, 0);
      Event_End();
    }
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampa_TriggersOnceBriggsSailed;
  return gChampa_Triggers;
}

/// Every other frame, turns colours 1 to 14 of background palettes 3, 4 and 5
/// one place on.
void Champa_RotatePaletteColors(void) {
  if ((gFrameCount & 1) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(3, 14);
    for (index = 14; index != 1; index--)
      BG_PLTT_COLOR(3, index) = BG_PLTT_COLOR(3, index - 1);
    BG_PLTT_COLOR(3, 1) = saved;
    saved = BG_PLTT_COLOR(4, 14);
    for (index = 14; index != 1; index--)
      BG_PLTT_COLOR(4, index) = BG_PLTT_COLOR(4, index - 1);
    BG_PLTT_COLOR(4, 1) = saved;
    saved = BG_PLTT_COLOR(5, 14);
    for (index = 14; index != 1; index--)
      BG_PLTT_COLOR(5, index) = BG_PLTT_COLOR(5, index - 1);
    BG_PLTT_COLOR(5, 1) = saved;
  }
}

s32 Tightrope_Initialize();

s32 Scene_Initialize(void) {

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_AddFrame((s32)Champa_RotatePaletteColors, TASK_PASS_VBLANK);
  TilesetAnim_Pause(8);
  Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_FLAG), 0);
  Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_SECOND_FLAG), 0);
  FieldActor_SetSpritePriority(ACTOR_WOMAN_WAITING_FOR_FOOD, 3);
  Actor_Get(ACTOR_ROCK)->scaleX = FIXED_ONE / 2;
  Actor_Get(ACTOR_ROCK)->scaleY = FIXED_ONE / 2;
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_TIGHTROPE_START, ACTOR_TIGHTROPE_END);
  TilesetAnim_Pause(3);
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    FieldActor_SetSpritePriority(ACTOR_SAILOR_AT_THE_QUIET_CAVES, 2);
    FieldActor_SetSpritePriority(ACTOR_SAILOR_ON_THE_MATRIARCH, 3);
    FieldActor_SetSpritePriority(ACTOR_UNWELCOMING_SAILOR, 3);
    FieldActor_SetSpritePriority(ACTOR_EOLEOS_FRIEND, 3);
    Actor_SetMovementType(Actor_Get(ACTOR_PRACTICING_BOY),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    if (Event_GetFlag(FLAG_SAILORS_SPOTTED_FELIX)) {
      Actor_SetPosition(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 0);
      Actor_SetPosition(ACTOR_SECOND_SAILOR, 0, 0);
      Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    }
  }
  if (Event_GetFlag(FLAG_FEIZHIS_GIFT_WAITING)) {
    Champa_FeizhiHoldsHerGift();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {

  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    MapGrid_CopyCellsLayered(11, 51, 5, 4, 16, 23);
    MapGrid_CopyCellAttributes(11, 51, 5, 6, 16, 23);
  }
  return 0;
}

extern u32 gChampa_GlareSparkScript[];

void Actor_StartScript(struct Actor *actor, const void *script);

/// A spark at a point, its sprite turned to `angle`, for `frames` frames.
void Champa_SpawnGlareSpark(s32 x, s32 y, s32 z, s32 angle, s32 frames) {
  s16 value = angle;
  struct Actor *actor;
  struct FieldSprite *sprite;

  actor = FieldActor_Create(SPRITE_EMOTE, x, y, z);
  if (actor != 0) {
    sprite = actor->sprite;
    Actor_SelectAnimation(actor, 1);
    Actor_StartScript(actor, gChampa_GlareSparkScript);
    actor->motionFlags = 0;
    actor->wait = frames;

    sprite->flags = 0;
    sprite->rotation = value;
  }
}

/// Two sparks over each of the two actors in turn, as Jenna and Sheba glare.
void Champa_JennaAndShebaGlare(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_Get(firstId);
  struct Actor *second = Actor_Get(secondId);
  s32 rise = toFIXED(28);
  FIXED y;

  y = first->y.fixed;
  Champa_SpawnGlareSpark(first->x.fixed, y + rise, first->z.fixed,
                         toFIXED(1) / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  y = second->y.fixed;
  Champa_SpawnGlareSpark(second->x.fixed, y + rise, second->z.fixed,
                         toFIXED(15) / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  Champa_SpawnGlareSpark(first->x.fixed, first->y.fixed + rise, first->z.fixed,
                         toFIXED(1) / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  Champa_SpawnGlareSpark(second->x.fixed, second->y.fixed + rise,
                         second->z.fixed, toFIXED(15) / 16, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
}

struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed);
void Portrait_OpenWindow(s32 object, s32 a, s32 b, s32 c);

/// Event_ShowTwoMessagesAndWait's first half, kept in the scene: two actors
/// speak at once, the second message waiting for the first when delay is
/// nonzero; Champa_CloseTwoMessages closes them.
void Champa_ShowTwoMessages(s32 first, s32 firstX, s32 firstY,
                            s32 firstPortraitX, s32 firstPortraitY, s32 second,
                            s32 secondX, s32 secondY, s32 secondPortraitX,
                            s32 secondPortraitY, s32 delay) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 firstSprite = Actor_GetSpriteById(first);
  s32 secondSprite = Actor_GetSpriteById(second);

  Message_OpenWindow((s16)event->message++, firstX, firstY,
                     Speaker_GetVoice(firstSprite) << 16);
  Portrait_OpenWindow(firstSprite, 0, firstPortraitX, firstPortraitY);
  if (delay != 0) {
    while (!Message_AllTyped())
      System_Wait(delay);
  }
  Message_OpenWindow((s16)event->message++, secondX, secondY,
                     Speaker_GetVoice(secondSprite) << 16);
  Portrait_OpenWindow(secondSprite, 0, secondPortraitX, secondPortraitY);
  while (!Message_AllTyped())
    System_Wait(1);
  System_Wait(1);
}

/// Closes the two windows Champa_ShowTwoMessages opened.
void Champa_CloseTwoMessages(s32 first, s32 second) {
  s32 firstSprite;
  s32 secondSprite;

  firstSprite = Actor_GetSpriteById(first);
  secondSprite = Actor_GetSpriteById(second);
  Window_CloseMatchingId(firstSprite);
  Window_CloseMatchingId(secondSprite);
}

/// Walking up to Obaba's cliff sets FLAG_NEAR_THE_CLIFF, so Alex waits there.
void Champa_ApproachTheCliff(void) { GameFlag_Set(FLAG_NEAR_THE_CLIFF); }

/// Shows the speaker's message by its actor (FieldMessage_ShowAtActor).
static __inline__ void Champa_MessageOpen(s32 speaker, s32 flags) {
  FieldMessage_ShowAtActor(speaker, flags);
}

/// Near Obaba's cliff Alex waits: "I see you've caught up with me again"
/// (MsgChampaISeeYouveCaughtUp), and the party answers him.
void Champa_MeetAlex(void) {

  if (!Event_GetFlag(FLAG_NEAR_THE_CLIFF)) {
    return;
  }
  Event_SetFlag(FLAG_CHAMPA_MET_ALEX);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgChampaISeeYouveCaughtUp);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_SetSpeed(ACTOR_KARST, 1);
  Actor_SetSpeed(ACTOR_AGATIO, 1);
  Actor_SetPosition(ACTOR_ALEX, 312, 520);
  Actor_SetPosition(ACTOR_KARST, 456, 616);
  Actor_SetPosition(ACTOR_AGATIO, 168, 616);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  // "I see you've caught up with me again, Felix."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_WalkToAndWait(ACTOR_FELIX, 312, 616);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 600, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(5);
  Event_PlaySound(MUS_ALEX);
  Actor_WalkToAndWait(ACTOR_ALEX, 304, 576);
  FieldEvent_Wait(5);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 0, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -32, 0, FACING_NORTH_NORTHEAST);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Alex!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  // "Alex? Who is Alex?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(10);
  // "Alex is a Water Adept. We had been traveling together, but he left..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "A Water Adept?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Champa_MessageOpen(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(15);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(35);
    gFieldWork->message = (gFieldWork->message + 1);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  }
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldEvent_Wait(5);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    FieldEvent_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
    FieldEvent_Wait(10);
    Event_ShowMessage(ACTOR_SHEBA, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    FieldEvent_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
    FieldEvent_Wait(10);
    gFieldWork->message = (gFieldWork->message + 1);
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_ShowMessage(ACTOR_KARST, 0);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  Actor_WalkToAndWait(ACTOR_KARST, 344, 616);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KARST, 3);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_WalkToAndWait(ACTOR_AGATIO, 232, 616);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_WalkByAndWait(ACTOR_AGATIO, 16, 0);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_Jump(ACTOR_AGATIO, 4, 25);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(15);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 42);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkByAndWait(ACTOR_KRADEN, 8, -8);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  Actor_Get(ACTOR_ALEX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_ALEX, -7, 7);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_ALEX)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  FieldActor_StartRepeatedMotion(ACTOR_ALEX, 3);
  Actor_SetAttachedEffect(ACTOR_ALEX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_SetSpeed(ACTOR_ALEX, 0.6);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  Actor_WalkBy(ACTOR_ALEX, 8, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_EAST);
  FieldEvent_Wait(5);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_KRADEN, 0.6);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(2);
  Actor_Get(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_KRADEN, -8, 8);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 42);
  FieldActor_Jump(ACTOR_KARST, 6, 15);
  FieldActor_Jump(ACTOR_KARST, 6, 23);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
  Event_ShowMessage(ACTOR_KARST, 0);
  FieldActor_Jump(ACTOR_AGATIO, 6, 15);
  FieldActor_Jump(ACTOR_AGATIO, 6, 23);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetSpeed(ACTOR_KARST, 1.8);
  Actor_WalkByAndWait(ACTOR_KARST, -12, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
  Actor_SetSpeed(ACTOR_AGATIO, 1.8);
  Actor_WalkByAndWait(ACTOR_AGATIO, 12, 0);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 42);
  FieldActor_Jump(ACTOR_AGATIO, 6, 15);
  FieldActor_Jump(ACTOR_AGATIO, 6, 23);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(15);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_KARST, 3);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_EAST);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_AGATIO);
  FieldActor_WatchAndSetScript(ACTOR_PIERS, ACTOR_AGATIO);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_AGATIO);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_AGATIO);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_AGATIO);
  Actor_SetSpeed(ACTOR_AGATIO, 1);
  Actor_WalkByAndWait(ACTOR_AGATIO, 20, 0);
  Actor_WalkByAndWait(ACTOR_AGATIO, 2, 32);
  Actor_WalkByAndWait(ACTOR_AGATIO, 32, 32);
  Actor_WalkByAndWait(ACTOR_AGATIO, 0, 32);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_KARST, 3);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_KARST);
  FieldActor_WatchAndSetScript(ACTOR_PIERS, ACTOR_KARST);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_KARST);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_KARST);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_KARST);
  Actor_SetSpeed(ACTOR_KARST, 1);
  Actor_WalkByAndWait(ACTOR_KARST, 0, 32);
  Actor_WalkByAndWait(ACTOR_KARST, -20, 24);
  Actor_WalkByAndWait(ACTOR_KARST, 0, 48);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(20);
  Actor_SetPosition(ACTOR_KARST, 0, 0);
  Actor_SetPosition(ACTOR_AGATIO, 0, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_ALEX);
  FieldActor_WatchAndSetScript(ACTOR_PIERS, ACTOR_ALEX);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_ALEX);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_ALEX);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_ALEX);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  Actor_WalkByAndWait(ACTOR_ALEX, 22, 24);
  EventCamera_FollowActor(ACTOR_ALEX, 1);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 32);
  Actor_WalkByAndWait(ACTOR_ALEX, -16, 16);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 8);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHWEST);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 648, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 32);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait(25);
  Actor_SetSpeed(ACTOR_KRADEN, 0.4);
  Actor_Get(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -24);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_KRADEN, 8);
  FieldEvent_Wait(30);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 80);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 624, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_PlaySound(MUS_CHAMPA);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 1);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(45);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST + 1);
  FieldEvent_Wait(25);
  Champa_JennaAndShebaGlare(ACTOR_JENNA, ACTOR_SHEBA);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_JENNA, 5, 5, 18, 8, 6, 6, 14, 8, 9,
                                    0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Champa_ShowTwoMessages(5, 8, 5, 18, 8, 6, 8, 14, 8, 9, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHWEST);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Champa_CloseTwoMessages(5, 6);
    FieldEvent_Wait(15);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    Champa_CloseTwoMessages(5, 6);
    FieldEvent_Wait(35);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    gFieldWork->message = (gFieldWork->message + 1);
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 45);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_JENNA, 6, 15);
  FieldActor_Jump(ACTOR_JENNA, 6, 23);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_SHEBA, 6, 15);
  FieldActor_Jump(ACTOR_SHEBA, 6, 23);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -8);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(5);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(15);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  Actor_HeadToward(5, gMainRoutineWork.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  Actor_HeadToward(6, gMainRoutineWork.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  Actor_HeadToward(7, gMainRoutineWork.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  Actor_HeadToward(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 280, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Champa),
    SCENE_EXIT_UNLESS(1, ChampaRooms, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ChampaRooms, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ChampaRooms, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ChampaRooms, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ChampaRooms, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ChampaRooms, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, ChampaSanctum, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ChampaCliffCavesBriggs, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, WorldMap, 40, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, ChampaCliffCavesHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, ChampaCliffCavesObaba, 19, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
/// Champa's actors, in ChampaActorId's order.
struct ActorPlacement gChampa_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 296, 0, 408, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 328, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 432, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, 8, 440, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 408, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Champa's actors once Briggs has sailed from Alhafra: the sailors who spot
/// Felix and Briggs too.
struct ActorPlacement gChampa_ActorPlacementsOnceBriggsSailed[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_FLAG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 328, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 432, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 392, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 472, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 472, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 296, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 296, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 264, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// A door's map edit: none.
s16 gChampa_NoDoorEdit[] = {
    -1,
};

/// Door 7's map edit as it opens.
s16 gChampa_DoorEdit[] = {
    36, 2, 2, 2, 6, 36, 4, 2,
    2, 6, -1,
};
// clang-format on

/// The doors' map edits for Champa_OpenDoorAndEnter: door 7's, then the
/// others'.
s32 gChampa_Doors[] = {
    (s32)gChampa_DoorEdit,
    0x9001d,
    (s32)gChampa_NoDoorEdit,
    0,
};

/// Champa's exits, doors, talks, Mind Read and Psynergy.
struct Trigger gChampa_Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 24, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgChampaChampaAhead))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Champa_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Champa_HideTreasureSpot)},
    {TRIGGER_STEP, 100, FLAG_FEIZHI_CAME, EVENT(Champa_FeizhiArrives)},
    {TRIGGER_STEP, 19, FLAG_NEAR_THE_CLIFF, EVENT(Champa_ApproachTheCliff)},
    {TRIGGER_STEP, 20, FLAG_CHAMPA_MET_ALEX, EVENT(Champa_MeetAlex)},
    {TRIGGER_TALK, ACTOR_HUNGRY_YOUNG_MAN, CONDITION_ALWAYS,
     MSGNO(MsgChampaOhImSoHungryAnd)},
    {TRIGGER_TALK, ACTOR_WOMAN_WAITING_FOR_FOOD, CONDITION_ALWAYS,
     MSGNO(MsgChampaWhenIsBriggsGoingTo)},
    {TRIGGER_TALK, ACTOR_MAN_AT_THE_EDGE_OF_ANGARA, CONDITION_ALWAYS,
     MSGNO(MsgChampaHowDidYouFindYour)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_PROSPERITY, CONDITION_ALWAYS,
     MSGNO(MsgChampaChampaIsImpoverishedButSoon)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
     EVENT(Champa_AskAboutAnkohlRuins)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
     MSGNO(MsgChampaTheRuinsAtAnkohlAre)},
    {TRIGGER_TALK, ACTOR_PRACTICING_BOY, CONDITION_ALWAYS,
     EVENT(Champa_TalkToPracticingBoy)},
    {TRIGGER_TALK, ACTOR_EOLEOS_FRIEND, CONDITION_ALWAYS,
     MSGNO(MsgChampaOnceBriggsComesBackI)},
    {TRIGGER_TALK, ACTOR_SAILOR_AT_THE_QUIET_CAVES, CONDITION_ALWAYS,
     MSGNO(MsgChampaTheCliffCavesAreA)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_MATRIARCH, CONDITION_ALWAYS,
     MSGNO(MsgChampaOurMatriarchIsTheLast)},
    {TRIGGER_TALK, ACTOR_UNWELCOMING_SAILOR, CONDITION_ALWAYS,
     EVENT(Champa_SailorsNotWelcome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_YOUNG_MAN,
     CONDITION_ALWAYS, MSGNO(MsgChampaWhereCouldBriggsBeWasting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_FOR_FOOD,
     CONDITION_ALWAYS, MSGNO(MsgChampaBriggsPromisedToBringEnough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_THE_EDGE_OF_ANGARA,
     CONDITION_ALWAYS, MSGNO(MsgChampaChampaIsAllAloneOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PROSPERITY,
     CONDITION_ALWAYS, MSGNO(MsgChampaIBetBriggsHasAlready)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_ANKOHL_RUINS,
     CONDITION_ALWAYS, MSGNO(MsgChampaIWantSomeoneToSolve)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS,
     CONDITION_ALWAYS, MSGNO(MsgChampaTheySayTheChamberAt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTICING_BOY,
     CONDITION_ALWAYS, MSGNO(MsgChampaIfICouldPickUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEOS_FRIEND,
     CONDITION_ALWAYS, MSGNO(MsgChampaEoleosSoMuchYoungerThan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_AT_THE_QUIET_CAVES,
     CONDITION_ALWAYS, MSGNO(MsgChampaWhyHaventBriggsAndHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_MATRIARCH,
     CONDITION_ALWAYS, MSGNO(MsgChampaObabasLockedHerselfInHer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UNWELCOMING_SAILOR,
     CONDITION_ALWAYS, MSGNO(MsgChampaIfBriggsWouldJustBring)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CLIFF, CONDITION_ALWAYS,
     MSGNO(MsgChampaIBetYoureWonderingWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CLIFF,
     CONDITION_ALWAYS, MSGNO(MsgChampaStandingHereStaringAcrossThe)},
    {TRIGGER_TALK, ACTOR_FEIZHIS_GIFT, CONDITION_ALWAYS,
     EVENT(Champa_TakeFeizhisGift)},
    {TRIGGER_TABLE_END},
};

/// Champa's triggers once Briggs has sailed, the sailors' warning among them.
struct Trigger gChampa_TriggersOnceBriggsSailed[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 24, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgChampaChampaAhead))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Champa_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Champa_HideTreasureSpot)},
    {TRIGGER_STEP, 100, FLAG_FEIZHI_CAME, EVENT(Champa_FeizhiArrives)},
    {TRIGGER_STEP, 19, FLAG_NEAR_THE_CLIFF, EVENT(Champa_ApproachTheCliff)},
    {TRIGGER_STEP, 20, FLAG_CHAMPA_MET_ALEX, EVENT(Champa_MeetAlex)},
    {TRIGGER_STEP, 26, FLAG_SAILORS_SPOTTED_FELIX,
     EVENT(Champa_SailorsSpotFelix)},
    {TRIGGER_TALK, ACTOR_HUNGRY_YOUNG_MAN, CONDITION_ALWAYS,
     MSGNO(MsgChampaBriggsCameBackWithA)},
    {TRIGGER_TALK, ACTOR_WOMAN_WAITING_FOR_FOOD, CONDITION_ALWAYS,
     MSGNO(MsgChampaDontTalkToMeAbout)},
    {TRIGGER_TALK, ACTOR_MAN_AT_THE_EDGE_OF_ANGARA, CONDITION_ALWAYS,
     MSGNO(MsgChampaLetsSeeThemCallChampa)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_PROSPERITY, CONDITION_ALWAYS,
     EVENT(Champa_TellOfBriggsTreasure)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
     EVENT(Champa_AskAboutAnkohlRuins)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS, CONDITION_ALWAYS,
     MSGNO(MsgChampaBriggsLooksSoReliableSo)},
    {TRIGGER_TALK, ACTOR_PRACTICING_BOY, CONDITION_ALWAYS,
     MSGNO(MsgChampaThoseBagsAreFullOf)},
    {TRIGGER_TALK, ACTOR_EOLEOS_FRIEND, CONDITION_ALWAYS,
     MSGNO(MsgChampaIWantToPlayWith)},
    {TRIGGER_TALK, ACTOR_SAILOR_AT_THE_QUIET_CAVES, CONDITION_ALWAYS,
     MSGNO(MsgChampaBriggsFoughtTheOddsTo)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_MATRIARCH, CONDITION_ALWAYS,
     EVENT(Champa_TellOfObaba)},
    {TRIGGER_TALK, ACTOR_UNWELCOMING_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgChampaNowNobodyHasToDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_YOUNG_MAN,
     CONDITION_ALWAYS, MSGNO(MsgChampaBriggsIsSoGenerousHe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_FOR_FOOD,
     CONDITION_ALWAYS, MSGNO(MsgChampaErrrrgIWasSoFull)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_THE_EDGE_OF_ANGARA,
     CONDITION_ALWAYS, MSGNO(MsgChampaGoshHowMuchAreAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PROSPERITY,
     CONDITION_ALWAYS, MSGNO(MsgChampaThingsAreGoingToBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_ANKOHL_RUINS,
     CONDITION_ALWAYS, MSGNO(MsgChampaWereLuckyBriggsHadntFound)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_ANKOHL_RUINS,
     CONDITION_ALWAYS, MSGNO(MsgChampaBriggsWasAlwaysSuchA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTICING_BOY,
     CONDITION_ALWAYS, MSGNO(MsgChampaAllOfOurStockhousesAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEOS_FRIEND,
     CONDITION_ALWAYS, MSGNO(MsgChampaEoleoSaidHeCouldntPlay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_AT_THE_QUIET_CAVES,
     CONDITION_ALWAYS, MSGNO(MsgChampaIfChampaGetsEnoughMoney)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_MATRIARCH,
     CONDITION_ALWAYS, MSGNO(MsgChampaIsObabaInAGood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UNWELCOMING_SAILOR,
     CONDITION_ALWAYS, MSGNO(MsgChampaNobodyReallyLikesToPlay)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CLIFF, CONDITION_ALWAYS,
     MSGNO(MsgChampaWhenBriggsSetSailWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CLIFF,
     CONDITION_ALWAYS, MSGNO(MsgChampaIWasJustGazingOut)},
    {TRIGGER_TALK, ACTOR_FEIZHIS_GIFT, CONDITION_ALWAYS,
     EVENT(Champa_TakeFeizhisGift)},
    {TRIGGER_TABLE_END},
};

/// A glare spark's script: it is released when its wait runs out.
u32 gChampa_GlareSparkScript[] = {
    SC_RELEASE,
};
