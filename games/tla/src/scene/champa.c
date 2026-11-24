/// @file
/// Champa, the hungry seafaring village on the edge of Angara, its huts on
/// cliffs and rafts about two waterfalls, whose people wait for Briggs to come
/// home with food. The party comes in from the south over a plank bridge and
/// up the path past six gravestones, where in a game linked with The Broken
/// Seal Feizhi runs up to give Felix the gift she meant for Isaac
/// (Champa_FeizhiArrives). Leaving by the same bridge the first time, the
/// party meets Alex, who left it on Indra, with Karst and Agatio
/// (Champa_MeetAlex); after he has gone, Kraden's hints leave Jenna and Sheba
/// puzzled (Champa_JennaAndShebaLookPuzzled). Once Briggs has sailed from
/// Alhafra, the people and triggers change, and two sailors at the mouth of
/// Briggs's cave spot Felix and send Briggs running inside
/// (Champa_SailorsSpotFelix).

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
#include "system/main_routine_state.h"
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
extern struct MapGridCellAnimation gChampa_Doors[];
s32 Event_PickUpItem();
extern struct Trigger gChampa_Triggers[];
extern struct Trigger gChampa_TriggersOnceBriggsSailed[];
extern u16 gChampa_NoDoor[];
extern u16 gChampa_SanctumDoorOpening[];
s32 Tightrope_Place();
/// Closes at once the portrait window of a speaker, if one is open
/// (Portrait_CloseWindow, through the overlay's own import row).
s32 Window_CloseMatchingId();

/// Sends an actor walking to where another stands, if it is on the map.
static inline void Actor_HeadToward(s32 id, s32 target) {
  struct Actor *actor = Actor_Get(target);

  if (actor != NULL)
    Actor_SetDestination(id, actor->x.part[0].integer,
                         actor->z.part[0].integer);
}

/// The town's own story, which no other scene reads.
#define FLAG_TOLD_OF_OBABA 0x972         ///< a villager told of Obaba
#define FLAG_SAILORS_SPOTTED_FELIX 0x973 ///< two sailors ran to warn Briggs
#define FLAG_FEIZHIS_GIFT_WAITING 0xa74  ///< Feizhi holds out her gift
#define FLAG_FEIZHI_CAME 0xa75 ///< Feizhi came, taking Felix for a friend
/// The party walked up the path into the village, so Alex meets it on the
/// plank bridge on its way back out (Champa_MeetAlex).
#define FLAG_WALKED_INTO_THE_VILLAGE 0x970

/// Kraden's sprite's animation 8: shaking his head again and again, slower
/// than ANIM_SHAKE_HEAD, until another animation is set.
#define ANIM_KRADEN_KEEP_SHAKING_HEAD 8

/// The map cells Champa's step and examine triggers answer; a door's cell
/// takes the number of the exit it leaves by.
enum ChampaCellBehavior {
  /// Across the path up from the plank bridge (Champa_WalkIntoTheVillage),
  /// and the plank bridge itself (Champa_MeetAlex).
  CELL_BEHAVIOR_PATH_INTO_THE_VILLAGE = 19,
  CELL_BEHAVIOR_PLANK_BRIDGE = 20,
  /// The sign at the village's south end: "Champa ahead!"
  CELL_BEHAVIOR_VILLAGE_SIGN = 24,
  /// Before the two sailors at the mouth of Briggs's cave
  /// (Champa_SailorsSpotFelix).
  CELL_BEHAVIOR_BEFORE_BRIGGS_CAVE = 26,
  /// The path past the six gravestones (Champa_FeizhiArrives).
  CELL_BEHAVIOR_GRAVESTONE_PATH = 100
};

/// The last of the two waterfalls' colours in background palettes 3, 4 and 5,
/// which run from colour 1, each a band of blues from light to dark and back.
#define WATERFALL_LAST_COLOR 14
/// The map's tile animation that swings the sanctum's stone door open, once.
#define SANCTUM_DOOR_ANIMATION 8
/// The same frames of the sanctum's door over and over, kept paused.
#define SANCTUM_DOOR_LOOP_ANIMATION 3

/// Champa's people and things, numbering the rows of gChampa_ActorPlacements
/// whose sprites are not the party's; gChampa_ActorPlacementsOnceBriggsSailed
/// keeps them and adds the sailors who spot Felix.
enum ChampaActorId {
  /// The ends of the tightrope over the gap (Tightrope_Place).
  ///@{
  ACTOR_TIGHTROPE_START = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPE_END,
  ///@}
  /// The village's two flags either side of its road, Champa's anchor on red.
  ///@{
  ACTOR_WEST_FLAG,
  ACTOR_EAST_FLAG,
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
  /// "The Champa sailors aren't...well liked outside of here, are we?"
  ACTOR_SAILOR_ASKING_IF_WELL_LIKED,
  /// A small rock by the practicing boy, drawn at half size.
  ACTOR_ROCK,
  /// "I bet you're wondering why only sailors and their families..."
  ACTOR_WOMAN_ON_THE_CLIFF,
  /// Who meet the party on the plank bridge at the village's south end
  /// (Champa_MeetAlex).
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
  /// "What? Really? It's Felix?" and "Briggs, get outta here!"
  ACTOR_SAILOR_WHO_WARNS_BRIGGS,
  ACTOR_BRIGGS
};

/// The chest Reveal shows by the six gravestones, holding the Viking Helm.
#define ACTOR_VIKING_HELM_CHEST FIELD_TREASURE_ACTOR

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gChampa_ActorPlacements[];

const s16 *Scene_GetActorPlacements(void) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampa_ActorPlacementsOnceBriggsSailed;
  return gChampa_ActorPlacements;
}

/// Felix, each frame as he walks into the tent on one of the two rafts: he
/// shrinks by 3/128 of his size, as if going in under the canvas.
void Champa_ShrinkIntoTent(struct Actor *actor) {
  actor->scaleX += -(FIXED_ONE * 3 / 128);
  actor->scaleY += -(FIXED_ONE * 3 / 128);
}

/// Walking into a door of Champa: it opens with a sound, Felix walks in (lined
/// up on the door first when `mode` is 0, straight on when 1) and the screen
/// goes to the room behind it. The door's map edit is not played.
void Champa_OpenDoorAndEnter(const struct MapGridCellAnimation *opening,
                             s32 door, s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_Get(gMainRoutineState.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  id = gMainRoutineState.leaderActorId;
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

void TilesetAnim_Resume(s32 channel);

/// Walking north into one of Champa's doors or cave mouths takes the party in:
/// into the tents on the two rafts Felix shrinks as he goes, the sanctum's
/// stone door swings open first (SANCTUM_DOOR_ANIMATION), and the others he
/// simply walks through.
void Champa_EnterDoor(s32 door) {
  struct MapGridCellAnimation *table = gChampa_Doors;

  if ((u32)(door - CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A) <= 1)
    Actor_Get(gMainRoutineState.leaderActorId)->update =
        (ObjectUpdate)Champa_ShrinkIntoTent;
  if (door == CHAMPA_TO_CHAMPA_SANCTUM) {
    TilesetAnim_Resume(SANCTUM_DOOR_ANIMATION);
    Champa_OpenDoorAndEnter(&table[0], CHAMPA_TO_CHAMPA_SANCTUM, 1);
  } else {
    Champa_OpenDoorAndEnter(&table[1], door, 0);
  }
}

/// Reveal cast anywhere in Champa shows the hidden chest among the six
/// gravestones east of the path into the village, holding the Viking Helm, for
/// as long as the Psynergy lasts.
void Champa_RevealTreasureSpot(void) {

  TreasureSpot_SetPosition(ACTOR_VIKING_HELM_CHEST, toFIXED(392), toFIXED(568));
  Actor_Get(ACTOR_VIKING_HELM_CHEST)->motionFlags = 0;
  Actor_Get(ACTOR_VIKING_HELM_CHEST)->y.fixed = toFIXED(32);
}

/// When Reveal fades, the chest among the six gravestones is hidden again.
void Champa_HideTreasureSpot(void) {
  TreasureSpot_SetPosition(ACTOR_VIKING_HELM_CHEST, 0, 0);
}

#include "text/msg_ids.h"

/// Walking up to the two sailors by the mouth of Briggs's cave in the cliff
/// (cells (12, 19) and (12, 20)) after Briggs has escaped from Alhafra: they
/// turn, "Whoa! You... You're Felix!", and yell to Briggs, who comes out, sees
/// that it is really him and runs back into the cave, the sailors after him.
void Champa_SailorsSpotFelix(void) {

  Event_SetFlag(FLAG_SAILORS_SPOTTED_FELIX);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
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
  Actor_FaceDirectionAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SAILOR_WHO_WARNS_BRIGGS,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
  Actor_FaceDirectionAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS,
                             FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SAILOR_WHO_WARNS_BRIGGS,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "What? Really? It's Felix?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 5);
  FieldActor_Jump(ACTOR_SAILOR_WHO_SPOTS_FELIX, 6, 0);
  FieldActor_Jump(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 6, 0);
  Actor_SetSpeed(ACTOR_SAILOR_WHO_SPOTS_FELIX, 1.8);
  Actor_SetSpeed(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 1.8);
  Actor_Get(ACTOR_SAILOR_WHO_SPOTS_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, -16);
  Actor_Get(ACTOR_SAILOR_WHO_WARNS_BRIGGS)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, -16);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_SAILOR_WHO_WARNS_BRIGGS)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(15);
  Actor_ShowEmote(ACTOR_SAILOR_WHO_SPOTS_FELIX,
                  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "You're awfully persistent, aren't you?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 5);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_SPOTS_FELIX, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS,
                             FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(5);
  // "Briggs, get outta here! Felix is here to finish you off!"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 5);
  // "What was that!? Felix? Here?"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  FieldActor_WatchAndSetScript(ACTOR_SAILOR_WHO_SPOTS_FELIX, ACTOR_BRIGGS);
  FieldActor_WatchAndSetScript(ACTOR_SAILOR_WHO_WARNS_BRIGGS, ACTOR_BRIGGS);
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
  Actor_FaceDirection(ACTOR_SAILOR_WHO_WARNS_BRIGGS, FACING_NORTH_NORTHEAST, 0);
  Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
  Actor_SetSpeed(ACTOR_SAILOR_WHO_SPOTS_FELIX, 2);
  Actor_SetSpeed(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 2);
  Actor_WalkByAndWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, -16, -16);
  Actor_WalkBy(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 16, -16);
  Actor_WalkByAndWait(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, -16);
  Actor_SetPosition(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 0);
  Actor_WalkByAndWait(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, -16);
  Actor_SetPosition(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 0);
  Event_End();
}

/// Talking to the boy practising Psynergy by his small rock: he stands up
/// straight, tells of Eoleo's strange ability, and goes back to straining at
/// the rock, squashed down with the effort.
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

/// Talking to the woman who waited for Champa to prosper, once Briggs has
/// sailed: "Hey, you just came to Champa not too long ago, didn't you?" Either
/// way she tells how Briggs found ancient pirate treasure and fed the
/// village.
void Champa_TellOfBriggsTreasure(s32 actor) {
  s32 msg = (s32)&MsgChampaHeyYouJustCameTo;

  FieldEvent_SetMessageId(msg);
  // "Hey, you just came to Champa not too long ago, didn't you?" Yes: "You'll
  // be pleased to know that Briggs found ancient pirate treasure..." No:
  // "Well, you wouldn't know it to look at us now, but Champa was quite
  // poor..."
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

/// Talking to the old man by the old woman: he asks whether the party has been
/// to Ankohl Ruins, or, once Briggs has sailed, whether they want to know why
/// Briggs took so long to return, and answers the yes or the no.
void Champa_AskAboutAnkohlRuins(s32 actor) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    FieldEvent_SetMessageId((s32)MsgChampaBriggsTookALongTime);
    // "Briggs took a long time to return to us. You want to know why?" Yes: "He
    // was being held by an evil man who stole his money..." No: "Briggs
    // overcame great danger to return home to us. I admire him."
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
    // "You've the look of a traveler about you... Have you been to Ankohl
    // Ruins?" Yes: "Only those who have mastered the powers of the earth can
    // enter the ruins." No: "My mistake... These old eyes aren't what they used
    // to be..."
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

/// Talking to the sailor who speaks of the matriarch, once Briggs has sailed:
/// the first time he tells of Obaba, last of the Ankohl, and after that of how
/// she acts now that Briggs is back.
void Champa_TellOfObaba(s32 actor) {

  if (!Event_GetFlag(FLAG_TOLD_OF_OBABA)) {
    Event_SetFlag(FLAG_TOLD_OF_OBABA);
    FieldEvent_SetMessageId((s32)MsgChampaObabaIsTheLastPossessor);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampaYoudThinkObabaWouldBe);
  }
  // "Obaba is the last possessor of the ancient secrets of Ankhol..." the
  // first time, then "You'd think Obaba would be glad that Briggs was back."
  Event_ShowMessage(actor, 0);
}

/// Talking to the sailor who asks "The Champa sailors aren't...well liked
/// outside of here, are we?", and hearing his answer to the yes or the no.
void Champa_AskIfSailorsAreWellLiked(s32 actor) {
  s32 msg = (s32)&MsgChampaTheChampaSailorsArentWell;

  FieldEvent_SetMessageId(msg);
  // "The Champa sailors aren't...well liked outside of here, are we?" Yes:
  // "Hey, that hurts... You could at least lie to me, just to be polite..."
  // No: "Really? That's nice to hear. But if you do hear anything, don't
  // believe a word of it."
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

/// Talking to the charm Feizhi left on the path takes it, the Golden Ring; if
/// the party cannot carry it, it stays there for later.
void Champa_TakeFeizhisGift(s32 actor) {

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (!Value3(Event_PickUpItem, actor, ITEM_GOLDEN_RING, -1)) {
    Event_ClearFlag(FLAG_FEIZHIS_GIFT_WAITING);
  }
  Event_End();
}

/// Feizhi's gift, the Golden Ring, shown as an item on the path where she set
/// it down, and marked as waiting to be taken.
void Champa_FeizhiHoldsHerGift(void) {
  FieldActor_ShowItemIconById(ACTOR_FEIZHIS_GIFT, ITEM_GOLDEN_RING);
  {
    Actor_SetPositionAndFacing(ACTOR_FEIZHIS_GIFT, toFIXED(312), toFIXED(593),
                               FACING_NORTH);
  }
  Event_SetFlag(FLAG_FEIZHIS_GIFT_WAITING);
}

/// Walking up the path past the six gravestones (cells (18, 35) to (20, 35)),
/// in a game linked with a Broken Seal save in which Hsu was rescued, brings
/// Feizhi running up behind with "Isaac!". She takes Felix for Isaac, finds he
/// is not, is teased by Kraden, and leaves the party the good luck charm she
/// made for Isaac, the Golden Ring, before she goes home. In every edition but
/// the Japanese she no longer comes once Isaac has joined the party.
void Champa_FeizhiArrives(void) {
#if TLA_PATCH_1
#endif

  if (!Event_GetFlag(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH)) {
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
      FieldEvent_EndLastingPsynergy(0);
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
        // "Oh. I must have been mistaken."
        FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
        gFieldState->message = (gFieldState->message + 1);
      } else {
        gFieldState->message = (gFieldState->message + 1);
        // "No, you are not Isaac, are you?"
        FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      }
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      FieldActor_LaunchFromLeader(ACTOR_KRADEN_MEETING_FEIZHI, 16, 0,
                                  FACING_SOUTH);
      FieldActor_WaitForMoveById(ACTOR_KRADEN_MEETING_FEIZHI);
      FieldEvent_Wait(20);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
      // "Young lady, are you looking for someone by the name of Isaac?"
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
      FieldEvent_Wait(5);
      // "Yes, old man! I am looking for one called Isaac!"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
      // "Where is Isaac?"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_FaceEachOther(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(ACTOR_KRADEN_MEETING_FEIZHI, FACING_SOUTH, 0);
      Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_SHAKE_HEAD);
      // "I'm sorry... Isaac isn't with us..."
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
      FieldActor_Jump(ACTOR_FEIZHI, 2, 0);
      // "Well then, where can I find Isaac?"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
      // "Why do you want to find him so badly?"
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
      FieldEvent_Wait(5);
      // "Me? I am Feizhi... Isaac helped me once."
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      // "He helped Ulmuch. Ulmuch was trapped, but Isaac saved him. He is a..."
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE,
                      30);
      // "I suppose... But why are you looking for him? Do you like him? Is..."
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
      // "You're a mean old man. It is not polite to tease me."
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      // "That's what it is! You like Isaac!"
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      // "No! I am so embarrassed!"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_SOUTH);
      FieldEvent_Wait(10);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE,
                      30);
      // "Don't worry, Feizhi... We'll tell Isaac how you feel!"
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_FaceDirectionAndWait(ACTOR_FEIZHI, FACING_NORTH);
      FieldEvent_Wait(10);
      Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
      // "I... Really?"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
      FieldEvent_Wait(5);
      // "Then...will you give him this when you tell him?"
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Champa_FeizhiHoldsHerGift();
      Actor_FaceEachOther(ACTOR_KRADEN_MEETING_FEIZHI, ACTOR_FELIX, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(ACTOR_KRADEN_MEETING_FEIZHI, FACING_SOUTH, 0);
      Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      // "What is it?"
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      // "Isaac is on a dangerous journey. I am worried about him, so I made..."
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_ShowEmote(ACTOR_KRADEN_MEETING_FEIZHI,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
      // "Ah, a good luck charm... I see."
      FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_MEETING_FEIZHI, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      // "I am going home now, old man. Thank you for doing this favor for me."
      FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 5);
      Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
      FieldEvent_Wait(10);
      EventCamera_FollowActor(ACTOR_FELIX, 1);
      Actor_SetSpeed(ACTOR_FEIZHI, 1.2);
      Actor_WalkBy(ACTOR_FEIZHI, 0, 100);
      FieldEvent_Wait(10);
      Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN_MEETING_FEIZHI, 0);
      FieldEvent_Wait(30);
      Actor_SetAnimation(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
      FieldEvent_Wait(30);
      Actor_SetAnimation(ACTOR_KRADEN_MEETING_FEIZHI, ANIM_WALK);
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

/// Champa's two waterfalls flow: every other frame, the bands of blues they are
/// drawn in, colours 1 to WATERFALL_LAST_COLOR of background palettes 3, 4 and
/// 5, turn one place on.
void Champa_CycleWaterfallPalettes(void) {
  if ((gFrameCount & 1) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(3, WATERFALL_LAST_COLOR);
    for (index = WATERFALL_LAST_COLOR; index != 1; index--)
      BG_PLTT_COLOR(3, index) = BG_PLTT_COLOR(3, index - 1);
    BG_PLTT_COLOR(3, 1) = saved;
    saved = BG_PLTT_COLOR(4, WATERFALL_LAST_COLOR);
    for (index = WATERFALL_LAST_COLOR; index != 1; index--)
      BG_PLTT_COLOR(4, index) = BG_PLTT_COLOR(4, index - 1);
    BG_PLTT_COLOR(4, 1) = saved;
    saved = BG_PLTT_COLOR(5, WATERFALL_LAST_COLOR);
    for (index = WATERFALL_LAST_COLOR; index != 1; index--)
      BG_PLTT_COLOR(5, index) = BG_PLTT_COLOR(5, index - 1);
    BG_PLTT_COLOR(5, 1) = saved;
  }
}

s32 Tightrope_Initialize();

/// Sets up the village as the party enters: the waterfalls flowing, the
/// sanctum's door shut, the tightrope strung, the sailors and Briggs as the
/// story leaves them, and Feizhi's gift where she left it. Returns 0.
s32 Scene_Initialize(void) {

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_AddFrame((s32)Champa_CycleWaterfallPalettes, TASK_PASS_VBLANK);
  TilesetAnim_Pause(SANCTUM_DOOR_ANIMATION);
  Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_WEST_FLAG), 0);
  Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_EAST_FLAG), 0);
  FieldActor_SetSpritePriority(ACTOR_WOMAN_WAITING_FOR_FOOD, 3);
  Actor_Get(ACTOR_ROCK)->scaleX = FIXED_ONE / 2;
  Actor_Get(ACTOR_ROCK)->scaleY = FIXED_ONE / 2;
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_TIGHTROPE_START, ACTOR_TIGHTROPE_END);
  TilesetAnim_Pause(SANCTUM_DOOR_LOOP_ANIMATION);
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    FieldActor_SetSpritePriority(ACTOR_SAILOR_AT_THE_QUIET_CAVES, 2);
    FieldActor_SetSpritePriority(ACTOR_SAILOR_ON_THE_MATRIARCH, 3);
    FieldActor_SetSpritePriority(ACTOR_SAILOR_ASKING_IF_WELL_LIKED, 3);
    FieldActor_SetSpritePriority(ACTOR_EOLEOS_FRIEND, 3);
    Actor_SetMovementType(Actor_Get(ACTOR_PRACTICING_BOY),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    if (Event_GetFlag(FLAG_SAILORS_SPOTTED_FELIX)) {
      Actor_SetPosition(ACTOR_SAILOR_WHO_SPOTS_FELIX, 0, 0);
      Actor_SetPosition(ACTOR_SAILOR_WHO_WARNS_BRIGGS, 0, 0);
      Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    }
  }
  if (Event_GetFlag(FLAG_FEIZHIS_GIFT_WAITING)) {
    Champa_FeizhiHoldsHerGift();
  }
  return 0;
}

/// Once Briggs has sailed, crates of the food he brought stand in the village.
/// Returns 0.
s32 Scene_PrepareMap(void) {

  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    MapGrid_CopyCellsLayered(11, 51, 5, 4, 16, 23);
    MapGrid_CopyCellAttributes(11, 51, 5, 6, 16, 23);
  }
  return 0;
}

extern u32 gChampa_QuestionMarkScript[];

void Actor_StartScript(struct Actor *actor, const void *script);

/// A question mark over someone's head, the emote sprite's, turned to `angle`,
/// for `frames` frames.
void Champa_SpawnQuestionMark(s32 x, s32 y, s32 z, s32 angle, s32 frames) {
  s16 value = angle;
  struct Actor *actor;
  struct FieldSprite *sprite;

  actor = FieldActor_Create(SPRITE_EMOTE, x, y, z);
  if (actor != 0) {
    sprite = actor->sprite;
    Actor_SelectAnimation(actor, 1);
    Actor_StartScript(actor, gChampa_QuestionMarkScript);
    actor->motionFlags = 0;
    actor->wait = frames;

    sprite->flags = 0;
    sprite->rotation = value;
  }
}

/// Jenna and Sheba, turned to each other while Kraden talks of what he has
/// learned, puzzle at each other: a white bubble with a question mark in it,
/// tilted towards the other, pops up over Jenna's head and then over Sheba's,
/// twice each, each with a chime, before they both say they do not follow him.
/// The bubble is the emote sprite's question mark, turned 22.5 degrees one way
/// or the other.
void Champa_JennaAndShebaLookPuzzled(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_Get(firstId);
  struct Actor *second = Actor_Get(secondId);
  s32 rise = toFIXED(28);
  FIXED y;

  y = first->y.fixed;
  Champa_SpawnQuestionMark(first->x.fixed, y + rise, first->z.fixed,
                           DEGtoANG(22.5), 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  y = second->y.fixed;
  Champa_SpawnQuestionMark(second->x.fixed, y + rise, second->z.fixed,
                           DEGtoANG(337.5), 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  Champa_SpawnQuestionMark(first->x.fixed, first->y.fixed + rise,
                           first->z.fixed, DEGtoANG(22.5), 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  Champa_SpawnQuestionMark(second->x.fixed, second->y.fixed + rise,
                           second->z.fixed, DEGtoANG(337.5), 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
}

/// Event_ShowTwoMessagesAndWait's first half, kept in the scene: two actors
/// speak at once, the second message waiting for the first when delay is
/// nonzero; Champa_CloseTwoMessages closes them.
void Champa_ShowTwoMessages(s32 first, s32 firstX, s32 firstY,
                            s32 firstPortraitX, s32 firstPortraitY, s32 second,
                            s32 secondX, s32 secondY, s32 secondPortraitX,
                            s32 secondPortraitY, s32 delay) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  s32 firstSprite = Actor_GetSpriteById(first);
  s32 secondSprite = Actor_GetSpriteById(second);

  Message_OpenWindow((s16)event->message++, firstX, firstY,
                     MESSAGE_WINDOW_VOICE(Speaker_GetVoice(firstSprite)));
  Portrait_OpenWindow(firstSprite, 0, firstPortraitX, firstPortraitY);
  if (delay != 0) {
    while (!Message_AllTyped())
      System_Wait(delay);
  }
  Message_OpenWindow((s16)event->message++, secondX, secondY,
                     MESSAGE_WINDOW_VOICE(Speaker_GetVoice(secondSprite)));
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

/// Nothing visible happens when the party, coming in from the south, walks up
/// the path past the plank bridge (cells (17, 40) to (22, 40)): the game notes
/// that it has entered the village, so that Alex meets it on the bridge on its
/// way back out.
void Champa_WalkIntoTheVillage(void) {
  GameFlag_Set(FLAG_WALKED_INTO_THE_VILLAGE);
}

/// Shows the speaker's message by its actor (FieldMessage_ShowAtActor).
static __inline__ void Champa_MessageOpen(s32 speaker, s32 flags) {
  FieldMessage_ShowAtActor(speaker, flags);
}

/// Walking back south onto the plank bridge out of Champa (cell (19, 42)) the
/// first time, after going into the village, has Alex call "I see you've
/// caught up with me again, Felix" and come down the path to meet the party.
/// Karst and Agatio walk up from either side to press it on to Jupiter
/// Lighthouse, Alex tells Kraden that Lord Babi is dead, and once they have
/// gone, Kraden's hints about what he has learned leave Jenna and Sheba
/// demanding to be told.
void Champa_MeetAlex(void) {

  if (!Event_GetFlag(FLAG_WALKED_INTO_THE_VILLAGE)) {
    return;
  }
  Event_SetFlag(FLAG_CHAMPA_MET_ALEX);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
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
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
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
  // "Ah, and I see you've made a new friend! You..." Yes: "And you've found
  // yourself a new Water Adept..." No: "Please... It's quite obvious that you
  // decided..."
  Champa_MessageOpen(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(15);
    // "And you've found yourself a new Water Adept! Do you mean to cast me..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(35);
    gFieldState->message = (gFieldState->message + 1);
    // "Please... It's quite obvious that you decided to replace me with a..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  }
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "What do you want with us now, Alex? Or did you forget you were the one..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "My dear Jenna..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldEvent_Wait(5);
  // "Have you come to despise me so, after all..." Yes: "That's right, Felix!
  // Why should we care about..." No: "Duh! You're the one who left us. It's
  // pretty..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    FieldEvent_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
    FieldEvent_Wait(10);
    // "That's right, Felix! Why should we care about someone who just left..."
    Event_ShowMessage(ACTOR_SHEBA, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    FieldEvent_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
    FieldEvent_Wait(10);
    gFieldState->message = (gFieldState->message + 1);
    // "Duh! You're the one who left us. It's pretty clear who despises whom..."
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "My, aren't we a bitter bunch? And I was just trying to help you with..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "We have no need of your help. We can light the lighthouses without you."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "Oh... Can you really?"
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
  // "Karst!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KARST, ANIM_NOD);
  // "That's right, Karst! I'm flattered that you remember!"
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  // "What is this, Alex? What's going on?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Ah, how can I explain this..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  // "What's going on is Alex is demonstrating his remarkable foresight once..."
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
  // "Oh, have you not had the pleasure of an introduction?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
  // "He stayed back at the ship when I saw them in Madra."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_WalkByAndWait(ACTOR_AGATIO, 16, 0);
  // "I am Agatio."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_Jump(ACTOR_AGATIO, 4, 25);
  // "Let's see... That guy's Felix, and the girls are Jenna and Sheba..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH_NORTHEAST);
  // "They look like an unreliable bunch of ragamuffins."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(15);
  // "Yes, they are rather useless, aren't they? That's why I finally had to..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "But now, I believe I may have been...hasty in my judgment."
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
  // "You are Piers?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "H-How do you know my name?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "How soon they forget! We met you before, when you were being...detained."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Who could forget the amateur Adept changing water to ice in that..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 42);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  // "Ah! You saw that! That must be how you knew Piers was a Water Adept."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "In fact, I was even thinking of borrowing his ship at one point."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You would steal someone's boat? That's so typical, Alex!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  // "You wound me, Jenna... I only meant to borrow it."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Whatever you might have meant, that doesn't make it right!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "And just when I was about to seize the opportunity, you had to come back."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  // "You mean from Kibombo..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I was mere moments too late."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkByAndWait(ACTOR_KRADEN, 8, -8);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Isn't that a shame."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  Actor_Get(ACTOR_ALEX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_ALEX, -7, 7);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_ALEX)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Still, because of that, I did meet Karst and Agatio, so perhaps it was..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Can't you just stop talking about yourself for one second, Alex?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  FieldActor_StartRepeatedMotion(ACTOR_ALEX, 3);
  Actor_SetAttachedEffect(ACTOR_ALEX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
  // "Enough of your childish gibes! Alex, I want to know what makes these..."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_SetSpeed(ACTOR_ALEX, 0.6);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
  Actor_WalkBy(ACTOR_ALEX, 8, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Because they brought us the orb that Akafubu had taken. That shows..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "How so?"
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  // "The Kibombo are quite warlike... or were, at any rate. Recovering the..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "They were just typical villagers, easily frightened with a small..."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "No... They would have used less abrupt methods than those you favor..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_EAST);
  FieldEvent_Wait(5);
  // "Oh? What methods would those be?"
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Alex said it clearly enough. We wouldn't terrorize people into doing..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 42);
  FieldActor_Jump(ACTOR_KARST, 6, 15);
  FieldActor_Jump(ACTOR_KARST, 6, 23);
  // "Enough of this!"
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
  // "Stop changing the subject! When are you going to light the remaining..."
  Event_ShowMessage(ACTOR_KARST, 0);
  FieldActor_Jump(ACTOR_AGATIO, 6, 15);
  FieldActor_Jump(ACTOR_AGATIO, 6, 23);
  // "Yes! Our patience is beginning to wear thin!"
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetSpeed(ACTOR_KARST, 1.8);
  Actor_WalkByAndWait(ACTOR_KARST, -12, 0);
  FieldEvent_Wait(5);
  // "If you waste much more of our time, we'll take those Elemental Stars..."
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
  // "If we had a Wind Adept like Sheba and the Elemental Stars, we wouldn't..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  // "Excuse me?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I am, of course, opposed to their line of thinking."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "My experience with Saturos and Menardi suggests that, while they were..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You mean to say that they failed to solve the riddles?"
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "Miserably..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 42);
  FieldActor_Jump(ACTOR_AGATIO, 6, 15);
  FieldActor_Jump(ACTOR_AGATIO, 6, 23);
  // "Are you suggesting they were simple brutes, incapable of logic and..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I'm afraid so..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_NORTH_NORTHWEST);
  // "Alex... Whose side are you on?"
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "I am on no one's side. My only concern is to see the lighthouse..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  // "So you mean to leave the task to Felix?"
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  // "Then why have we come all this way looking for Felix?"
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  // "I merely wanted to confirm that he and his companions had not..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  // "Is that truly all?"
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I also wanted to introduce Felix to the two of you."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Why would you want to do that?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "With Saturos and Menardi gone, you probably felt very little pressure..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "What do you mean by that!?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "With Saturos and Menardi gone, I felt another pair might... How shall..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  // "Is that some kind of threat?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(15);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Take it as you wish. I just wanted to provide you with the proper..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_KARST, ANIM_NOD);
  // "So, what? You've used us to your satisfaction, and now, our role is done?"
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  // "You only wanted us around to flex a little muscle and scare them into..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_AGATIO, FACING_EAST);
  // "I suppose we will let you go today..."
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
  Actor_SetAnimationAndWait(ACTOR_KARST, ANIM_NOD);
  // "But we will always be nearby... pushing you to make your way to..."
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
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I look forward to seeing you soon, Felix, somewhere very near Jupiter..."
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
  // "Kraden, am I right in recalling that you pursue this quest on the..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 32);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "This is true. I'm studying Alchemy because Lord Babi has ordered it."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What about it?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "Ah, what a pity. It would seem all your studies... have been for naught."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "What are you getting at, Alex?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Only that it seems your Lord Babi has at last succumbed to the ravages..."
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
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_KRADEN_KEEP_SHAKING_HEAD);
  FieldEvent_Wait(30);
  // "Lord Babi is dead?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "Oh, so you didn't know?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Lord Babi? Dead? Could that really be true?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  // "Alex! How would you know such a thing?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Do not mistake me! We had no hand in Babi's passing."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "However, the people of Tolbi may be under the impression that our..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Alex! Cut the melodrama and get to the point for a change!"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Simply put, I would recommend against traveling to north Gondowan for..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I just thought it might be better if you focused on your quest..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  // "How kind of you."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Take it as you will..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_FaceDirectionAndWait(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(10);
  // "Very well. May we meet again."
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
  // "So the only reason Alex came here was to threaten us?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "And to bring us news of Babi's death, despite the fact that we asked..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Perhaps he had his own reasons for telling Kraden..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What do you mean, Piers?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  // "If Kraden undertook this quest on behalf of Babi, then he no longer..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  // "Oh. Of course! I hadn't thought of that. Your orders were to find..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "That's right. But if Babi really has died, you're free to go where you..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "That's not true, Sheba."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Certainly, my original reason for researching Alchemy was for Lord..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // ""Was for Lord Babi"? That's the past tense! What are your reasons now?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "This is no longer solely for research... I do this because of what..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(45);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Your research?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What you learned on this quest?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST + 1);
  FieldEvent_Wait(25);
  Champa_JennaAndShebaLookPuzzled(ACTOR_JENNA, ACTOR_SHEBA);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // Jenna: "I'm not following you at all!" Sheba: "I don't understand a word
  // of this."
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_JENNA, 5, 5, 18, 8, ACTOR_SHEBA, 6,
                                    14, 8, 9, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Of course you don't! Felix is the only one I've spoken to about any of..."
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
  // Jenna: "What!? You knew!?" Sheba: "That's not fair! Tell us!" Yes:
  // "Please, Felix... Let's not say too much..." No: "Felix is right. Now is
  // not the time to tell you about it all..."
  Champa_ShowTwoMessages(ACTOR_JENNA, 8, 5, 18, 8, ACTOR_SHEBA, 8, 14, 8, 9, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHWEST);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Champa_CloseTwoMessages(ACTOR_JENNA, ACTOR_SHEBA);
    FieldEvent_Wait(15);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Please, Felix... Let's not say too much about it just yet."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    Champa_CloseTwoMessages(ACTOR_JENNA, ACTOR_SHEBA);
    FieldEvent_Wait(35);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    gFieldState->message = (gFieldState->message + 1);
    // "Felix is right. Now is not the time to tell you about it all..."
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 45);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "It's...far too complicated."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_JENNA, 6, 15);
  FieldActor_Jump(ACTOR_JENNA, 6, 23);
  // "Well, when WILL you tell us?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "I am sure we will tell you once we get to Lemuria."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_SHEBA, 6, 15);
  FieldActor_Jump(ACTOR_SHEBA, 6, 23);
  // "Do you promise?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "I promise..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -8);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  FieldEvent_Wait(5);
  // "Piers? You don't mind waiting?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I cannot read minds, like Sheba, but I have a guess at what you're..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "A guess? What do you mean?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You are a great scholar, Kraden, are you not?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I believe your theory is correct. And to prove it, I want to return to..."
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  Actor_HeadToward(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  Actor_HeadToward(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  Actor_HeadToward(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_HeadToward(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
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
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A, ChampaRooms,
                      CHAMPA_ROOMS_FROM_CHAMPA_SOUTHWEST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_B, ChampaRooms,
                      CHAMPA_ROOMS_FROM_CHAMPA_SOUTHWEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_NORTH, ChampaRooms,
                      CHAMPA_ROOMS_FROM_CHAMPA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_WEST, ChampaRooms,
                      CHAMPA_ROOMS_FROM_CHAMPA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_NORTHEAST, ChampaRooms,
                      CHAMPA_ROOMS_FROM_CHAMPA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_ROOMS_EAST, ChampaRooms,
                      CHAMPA_ROOMS_FROM_CHAMPA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_SANCTUM, ChampaSanctum,
                      CHAMPA_SANCTUM_FROM_CHAMPA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_CLIFF_CAVES_BRIGGS,
                      ChampaCliffCavesBriggs,
                      CHAMPA_CLIFF_CAVES_BRIGGS_FROM_CHAMPA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CHAMPA,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_CLIFF_CAVES_HOUSES,
                      ChampaCliffCavesHouses,
                      CHAMPA_CLIFF_CAVES_HOUSES_FROM_CHAMPA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_TO_CHAMPA_CLIFF_CAVES_OBABA, ChampaCliffCavesObaba,
                      CHAMPA_CLIFF_CAVES_OBABA_FROM_CHAMPA, EXIT_FLAG_NONE),
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
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 440, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 408, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
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

/// Champa's actors once Briggs has sailed from Alhafra: the sailors at the mouth
/// of his cave and Briggs too.
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
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 472, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 472, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
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
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 296, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 296, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 264, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// A doorway with no door: nothing to open.
u16 gChampa_NoDoor[] = {
    MAPGRID_ANIMATION_STEPS_END,
};

/// The sanctum's door opening in two steps.
u16 gChampa_SanctumDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(36, 2, 2, 2, 6),
    MAPGRID_ANIMATION_STEP(36, 4, 2, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
// clang-format on

/// The doors' map edits for Champa_OpenDoorAndEnter, which does not play them:
/// the sanctum's, then the others'. The sanctum's door opens through
/// SANCTUM_DOOR_ANIMATION instead.
struct MapGridCellAnimation gChampa_Doors[] = {
    {gChampa_SanctumDoorOpening, 29, 9},
    {gChampa_NoDoor, 0, 0},
};

/// Champa's exits, doors, talks, Mind Read and Psynergy.
struct Trigger gChampa_Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_B, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTH,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_WEST,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTHEAST,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_EAST,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_SANCTUM,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_CLIFF_CAVES_BRIGGS, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {TRIGGER_EXIT, CHAMPA_TO_WORLD_MAP, CONDITION_ALWAYS, CHAMPA_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_VILLAGE_SIGN,
     CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgChampaChampaAhead))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Champa_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Champa_HideTreasureSpot)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GRAVESTONE_PATH, FLAG_FEIZHI_CAME,
     EVENT(Champa_FeizhiArrives)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PATH_INTO_THE_VILLAGE,
     FLAG_WALKED_INTO_THE_VILLAGE, EVENT(Champa_WalkIntoTheVillage)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PLANK_BRIDGE, FLAG_CHAMPA_MET_ALEX,
     EVENT(Champa_MeetAlex)},
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
    {TRIGGER_TALK, ACTOR_SAILOR_ASKING_IF_WELL_LIKED, CONDITION_ALWAYS,
     EVENT(Champa_AskIfSailorsAreWellLiked)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ASKING_IF_WELL_LIKED,
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
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_A, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_ROOMS_SOUTHWEST_B, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTH,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_WEST,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_NORTHEAST,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_ROOMS_EAST,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CHAMPA_TO_CHAMPA_SANCTUM,
     CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_CLIFF_CAVES_BRIGGS, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {TRIGGER_EXIT, CHAMPA_TO_WORLD_MAP, CONDITION_ALWAYS, CHAMPA_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CHAMPA_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS,
     EVENT(Champa_EnterDoor)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_VILLAGE_SIGN,
     CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgChampaChampaAhead))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Champa_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Champa_HideTreasureSpot)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GRAVESTONE_PATH, FLAG_FEIZHI_CAME,
     EVENT(Champa_FeizhiArrives)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PATH_INTO_THE_VILLAGE,
     FLAG_WALKED_INTO_THE_VILLAGE, EVENT(Champa_WalkIntoTheVillage)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PLANK_BRIDGE, FLAG_CHAMPA_MET_ALEX,
     EVENT(Champa_MeetAlex)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_BRIGGS_CAVE, FLAG_SAILORS_SPOTTED_FELIX,
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
    {TRIGGER_TALK, ACTOR_SAILOR_ASKING_IF_WELL_LIKED, CONDITION_ALWAYS,
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ASKING_IF_WELL_LIKED,
     CONDITION_ALWAYS, MSGNO(MsgChampaNobodyReallyLikesToPlay)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CLIFF, CONDITION_ALWAYS,
     MSGNO(MsgChampaWhenBriggsSetSailWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CLIFF,
     CONDITION_ALWAYS, MSGNO(MsgChampaIWasJustGazingOut)},
    {TRIGGER_TALK, ACTOR_FEIZHIS_GIFT, CONDITION_ALWAYS,
     EVENT(Champa_TakeFeizhisGift)},
    {TRIGGER_TABLE_END},
};

/// A question mark's script: it is released when its wait runs out.
u32 gChampa_QuestionMarkScript[] = {
    SC_RELEASE,
};
