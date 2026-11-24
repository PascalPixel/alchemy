/// @file
/// Madra, the walled town of eastern Gondowan that Felix reaches over the
/// Dehkan Plateau. Briggs's Champa pirates have just raided it to free their
/// leader, so the two guards at the south gate stop the party and look it
/// over before letting it in. Outside the wall in the north-east corner is the
/// hole the Champa dug, with a ladder down into the catacombs; a worker in it
/// turns Felix away until the guards have let him in. Piers is held in the jail
/// on the west side, and coming out of it after he has made his shaft of ice,
/// Felix is granted passage to Osenia by Milady. Once Briggs is caught the
/// Kibombo have raided the town in turn, the hole is being sealed and the
/// guards question the party again. When Felix brings Piers home the hole is
/// filled in, and leaving the mayor's house after the welcome the mayor gives
/// him the Cyclone Chip and Karst, come looking for Menardi, confronts him.

#include "flags.h"
#include "sprite_ids.h"
#include "item_ids.h"
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
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/field_event.h"
#include "scene/scene.h"
#include "field/scene_call.h"
#include "window/window.h"
#include "field/field_sprite.h"
#include "field/field_sequence.h"
#include "text/msg_ids.h"

/// Set once the worker has popped up out of the Champa's hole on this visit.
#define FLAG_MADRA_WORKER_SEEN_AT_HOLE FLAG_SCENE_TEMP_80
/// Set once Felix has told the worker he is coming through the hole.
#define FLAG_MADRA_TOLD_WORKER_COMING_THROUGH FLAG_SCENE_TEMP_81
/// Set once the boy has asked the party about the Champa boat, or about
/// Briggs; he no longer turns to face Felix first.
#define FLAG_MADRA_BOY_HAS_ASKED FLAG_AREA_TEMP_00
/// Set once Felix has told the boat owner that Jenna is with him.
#define FLAG_MADRA_TOLD_OF_JENNA FLAG_AREA_TEMP_01
/// Set as the guards let the party go after questioning it about the
/// Kibombo's raid.
#define FLAG_MADRA_RAID_QUESTIONING_DONE 0x914
/// Set once Karst has come to Madra looking for Menardi; she is gone from the
/// town afterwards.
#define FLAG_MADRA_KARST_MET 0x98e
/// Set once the party has heard Karst call for her sister, this visit.
#define FLAG_MADRA_KARST_CALLED_FOR_MENARDI FLAG_SCENE_TEMP_0F

/// The cells whose step triggers run Madra's events; a door's cell takes the
/// number of the exit it leads by.
enum MadraCellBehavior {
  /// The Champa's hole outside the north-east wall, cell (46, 15).
  CELL_BEHAVIOR_MADRA_CHAMPAS_HOLE = 16,
  /// The gate's inner cells, (29, 25) and (30, 25).
  CELL_BEHAVIOR_MADRA_GATE_INSIDE = 19,
  /// The gate's outer cells, (29, 26) and (30, 26).
  CELL_BEHAVIOR_MADRA_GATE_OUTSIDE,
  /// The path east of the mayor's house, (13, 17) to (13, 19).
  CELL_BEHAVIOR_MADRA_PATH_PAST_KARST
};

/// The town's placed actors, by the rows of gMadra_Placements, the first
/// visit's: Kraden, out of sight, steps out from behind Felix at the gate, and
/// Milady and her guard wait out of sight until they hurry to the jail.
enum MadraActorId {
  /// The townsfolk, each named for what they talk of on the first visit; their
  /// lines change once the Kibombo have raided the town (FLAG_BRIGGS_CAUGHT)
  /// and again when the party returns with Piers.
  ACTOR_YOUNG_MAN_ON_THE_ELDER = ACTOR_FIRST_PLACED,
  ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
  ACTOR_OLD_MAN_ON_THE_WAVE,
  /// Who owns the boats the tidal wave ruined; on the party's return she asks
  /// after Jenna for Isaac's party.
  ACTOR_BOAT_OWNER,
  ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
  ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
  /// The boy who asks about the Champa boat and Briggs.
  ACTOR_BRIGGS_FAN,
  ACTOR_GIRL_ON_THE_SUPERCONTINENT,
  ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
  ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
  ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
  ACTOR_GIRL_ON_THE_BIG_HOLE,
  ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
  ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
  /// Two jail guards, one whose friend was on guard when Briggs's men
  /// attacked, one whose partner ran off.
  ACTOR_GUARD_ON_HIS_FRIEND,
  ACTOR_GUARD_ON_HIS_PARTNER,
  /// The guards who stop the party at the gate.
  ACTOR_GATE_GUARD,
  ACTOR_SECOND_GATE_GUARD,
  /// The worker who pops up out of the Champa's hole into the catacombs.
  ACTOR_HOLE_WORKER,
  /// The two who saw the shaft of ice in the jail, there once Piers has used
  /// Frost.
  ACTOR_ICE_SHAFT_WITNESS,
  ACTOR_FRIGHTENED_WITNESS,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  /// The item merchant, back at her stand once Briggs is caught.
  ACTOR_ITEM_MERCHANT,
  ACTOR_KRADEN,
  /// The lady who grants the party passage to Osenia, called "Milady", and
  /// her guard.
  ACTOR_LADY,
  ACTOR_LADYS_GUARD,
  /// Who keeps the item stand while her sister recovers from the raid.
  ACTOR_ITEM_MERCHANTS_SIBLING,
  /// The map's third treasure spot, where the mayor sets the Cyclone Chip down
  /// (Madra_MeetKarst); out of sight otherwise.
  ACTOR_CYCLONE_CHIP = FIELD_TREASURE_ACTOR + 2
};

/// The rows of gMadra_PlacementsOnReturn that differ: Karst, who comes
/// looking for Menardi, and the mayor, back from Alhafra.
enum MadraOnReturnActorId {
  ACTOR_KARST = ACTOR_LADY,
  ACTOR_MAYOR,
  /// A man with a pack who wanders in front of the jail, asking whether the
  /// Kibombo scare the party.
  ACTOR_MAN_AFRAID_OF_THE_KIBOMBO
};

extern struct ActorPlacement gMadra_Placements[];
extern struct ActorPlacement gMadra_PlacementsOnReturn[];

void Shop_Run(s32 a, s32 b);

void Madra_GrantPassageToOsenia(void);

void Madra_MeetKarst(void);
extern struct Trigger gMadra_TriggersBeforeRaid[];
extern struct Trigger gMadra_TriggersAfterRaid[];
extern struct Trigger gMadra_TriggersOnReturn[];

extern u16 gMadra_DoorOpening[];
extern u16 gMadra_CavernHomeDoorOpening[];

void FieldCamera_WaitForArrival(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gMadra_PlacementsOnReturn;
  return gMadra_Placements;
}

extern struct MapGridCellAnimation gMadra_Doors[];

/// Felix walks up to one of Madra's doors: it opens with a door's sound, he
/// steps through and the screen fades to the room inside. The doors are the
/// left cavern home's two in the cliff at the top of town, the mayor's house on
/// the north-west, the two of the inn (its ground floor and its upstairs), the
/// house of the couple wary of Gondowan by the south-west corner, the mushroom
/// couple's house and the house east of it, and the round sanctum by the south
/// wall. The jail's doorway on the west side is a plain exit, with no door to
/// open.
///
/// Each door cell's step trigger is the number of the exit it leads by, and
/// `gMadra_Doors` holds, at that number less one, the cells that redraw the
/// door open. Everyone else on the map stops while Felix walks in.
void Madra_EnterDoor(void) {
  struct FieldState *state;
  struct Actor *actor;
  s16 *trigger;
  u32 id;
  s32 index;

  state = Ram_HeapSlots->fieldState;
  Event_Start();
  Event_EndLastingPsynergy(0);
  for (id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
    actor = Actor_GetById(id);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  trigger = &state->stepTrigger;
  index = *trigger - 1;
  Event_PlaySound(SE_DOOR_OPEN);
  {
    s32 x = gMadra_Doors[index].x;
    s32 y = gMadra_Doors[index].y;

    MapGrid_AnimateCells(gMadra_Doors[index].steps, x, y);
  }
  Actor_GetById(gMainRoutineState.leaderActorId)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Madra's shops under the market tents, as Shop_Run numbers them.
enum { SHOP_MADRA_WEAPONS = 4, SHOP_MADRA_ARMOR = 5, SHOP_MADRA_ITEMS = 6 };

/// Talking to the weapon merchant, the bald man in red at the east tent of the
/// market, from across his counter opens his shop. Spoken to from any other
/// side he talks of the merchants fighting off the Champa, and later of the
/// strange weapons the Kibombo left behind.
void Madra_TalkToWeaponMerchant(s32 arg) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineState.leaderActorId)->facing) ==
      FACING_NORTH) {
    Shop_Run(SHOP_MADRA_WEAPONS, arg);
    return;
  }
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    Event_SetMessageId((s32)&MsgMadraItLooksLikeKibomboMust);
  else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    Event_SetMessageId((s32)&MsgMadraArmorerAndIAreBusy);
  else
    Event_SetMessageId((s32)&MsgMadraAfterChampaOpenedJailCells);
  // "After the Champa opened the jail cells, they came over here... All..."
  Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
}

/// Talking to the armor merchant, the young man in green at the middle tent of
/// the market, from across his counter opens his shop. From any other side he
/// moans of a bone bruise from the fighting, and later talks of the pirates and
/// the Kibombo.
void Madra_TalkToArmorMerchant(s32 arg) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineState.leaderActorId)->facing) ==
      FACING_NORTH) {
    Shop_Run(SHOP_MADRA_ARMOR, arg);
    return;
  }
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    Event_SetMessageId((s32)&MsgMadraPeopleAreSayingChampa);
  else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    Event_SetMessageId((s32)&MsgMadraIKnewThereWasSomething);
  else
    Event_SetMessageId((s32)&MsgMadraOwOwOwIThink);
  // "Ow. Ow. Ow... I think I got a bone bruise. I hate those..."
  Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
}

/// Talking to whoever keeps the west tent of the market from across the
/// counter opens the item shop. Spoken to from any other side, at first the
/// merchant's sister, keeping the tent, says the merchant was hurt in the
/// Champa's attack; once Briggs is caught the merchant is back, healed, and
/// later worries about her boyfriend Shin.
void Madra_TalkToItemMerchant(s32 arg) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineState.leaderActorId)->facing) ==
      FACING_NORTH) {
    Shop_Run(SHOP_MADRA_ITEMS, arg);
    return;
  }
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    Event_SetMessageId((s32)&MsgMadraMyBoyfriendShinSeems);
  else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    Event_SetMessageId((s32)&MsgMadraAllMyInjuriesHaveHealed);
  else
    Event_SetMessageId((s32)&MsgMadraMySisterWasHurtWhen);
  // "My sister was hurt when the Champa attacked... She's recovering, but..."
  Event_ShowMessage(arg, 0);
}

/// Karst, told the party does not know her sister, shakes her head and sends
/// it off: "If you don't know my sister, then I have nothing to say to you."
void Madra_KarstSendsPartyAway(void) {
  Event_SetMessageId((s32)&MsgMadraIfYouDontKnow);
  Actor_SetAnimationAndWait(ACTOR_KARST, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "If you don't know my sister, then I have nothing to say to you. Get..."
  Event_ShowMessageThenWait(ACTOR_KARST, 0, 10);
}

/// Talking to Karst, the red-haired woman outside the mayor's house when Felix
/// brings Piers home: she asks whether the party has seen a girl named Menardi.
/// Answering yes, and yes again when she asks whether her sister is dead, makes
/// her threaten the party; either no sends it away.
void Madra_KarstAsksAboutMenardi(void) {
  Event_SetMessageId((s32)MsgMadraHaveYouByChanceSeen);
  // "Have you by chance seen a girl named Menardi?"
  Event_OpenMessage(ACTOR_KARST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    // "You have? And... And you're telling me that my sister is...dead?"
    Event_OpenMessage(ACTOR_KARST, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      // "If you continue these lies, I might take it upon myself to shut
      // you..."
      Event_ShowMessageThenWait(ACTOR_KARST, 0, 10);
    } else {
      Madra_KarstSendsPartyAway();
    }
  } else {
    Madra_KarstSendsPartyAway();
  }
}

struct Actor *Actor_GetById(u32 objectId);
void Actor_SetDestinationOffsetById(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSpeedParameters(s32 id, FIXED speed, FIXED acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_SetAnimationAndWait(s32 id, s32 mode);
void MapGrid_PlayMapEdit(const void *data, s32 a, s32 b);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 y,
                                                    s32 z);

/// Casting Mind Read on Karst outside the mayor's house: she feels it, smiles
/// and asks whether the party is too dense to be afraid, then tells it to leave
/// her to her thoughts.
void Madra_MindReadKarst(void) {
  Event_SetMessageId((s32)MsgMadraUsingPsynergyOnMe);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Using Psynergy on me? Are you so dense that you don't know when to..."
  Event_ShowMessageThenWait(ACTOR_KARST, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KARST, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "I don't have time to play games with you. Now, leave me to my thoughts!"
  Event_ShowMessageThenWait(ACTOR_KARST, 0, 10);
}

/// Walking towards the mayor's house along the path east of it (cells (13, 17)
/// to (13, 19)) when Felix brings Piers home, the party hears Karst, standing
/// just east of the path, call for her sister: "Where could Menardi have gone?
/// Where are you, my sister?" She calls once a visit.
void Madra_KarstCallsForMenardi(void) {
  if (Event_GetFlag(FLAG_MADRA_KARST_CALLED_FOR_MENARDI) == 0) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    Event_SetMessageId((s32)&MsgMadraWhereCouldMenardiHaveGone);
    // "Where could Menardi have gone? Where are you, my sister?"
    Event_ShowMessageThenWait(ACTOR_KARST, 0, 10);
    Event_End();
    Event_SetFlag(FLAG_MADRA_KARST_CALLED_FOR_MENARDI);
  }
}

/// Talking to the man with a pack who wanders in front of the jail when Felix
/// brings Piers home: "Everyone here in Madra is afraid of the Kibombo now. Do
/// they scare you, too?" Either way he says their smoke signals have since
/// announced a truce.
void Madra_AskAfraidOfKibombo(s32 actor) {
  s32 msg = (s32)&MsgMadraEveryoneHereInMadra;

  Event_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// The worker in the Champa's hole outside the north-east wall asks "You comin'
/// through here?", and whatever Felix answers, tells him nobody can come
/// through while they work and he must enter Madra through the front gates.
/// Once told yes, he says only that.
void Madra_AskComingThroughHole(void) {
  s32 msg;

  Actor_FaceActor(ACTOR_HOLE_WORKER, ACTOR_FELIX, 0);
  if (Event_GetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH)) {
    Event_SetMessageId((s32)&MsgMadraYeahWellWereWorkin);
  } else {
    msg = (s32)&MsgMadraYouCominThroughHere;
    Event_SetMessageId(msg);
    // "You comin' through here?"
    Event_OpenMessage(ACTOR_HOLE_WORKER, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH);
      Event_Wait(10);
      FieldActor_ShowEmote(ACTOR_HOLE_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_HOLE_WORKER, ANIM_NOD);
      Event_SetMessageId(msg + 2);
    }
  }
  Event_ShowMessage(ACTOR_HOLE_WORKER, 0);
}

/// Once the Kibombo have raided Madra, the worker in the Champa's hole asks
/// "Oh, uh... You coming through?", and whatever Felix answers, says they are
/// sealing the hole up and sends him round to the main gates.
void Madra_AskComingThroughSealedHole(void) {
  s32 msg;

  Actor_FaceActor(ACTOR_HOLE_WORKER, ACTOR_FELIX, 0);
  if (Event_GetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH)) {
    Event_SetMessageId((s32)&MsgMadraYeahUhWereSealingHole);
  } else {
    msg = (s32)&MsgMadraOhUhYouComingThrough;
    Event_SetMessageId(msg);
    // "Oh, uh... You coming through?"
    Event_OpenMessage(ACTOR_HOLE_WORKER, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH);
      Event_Wait(10);
      FieldActor_ShowEmote(ACTOR_HOLE_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_HOLE_WORKER, ANIM_NOD);
      Event_SetMessageId(msg + 2);
    }
  }
  Event_ShowMessage(ACTOR_HOLE_WORKER, 0);
}

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is.
static inline void Madra_PlaceFacing(s32 actorId, s32 x, s32 z, u16 facing) {
  Actor_SetPositionAndFacing(actorId, x, z, facing);
}

/// Felix steps into the hole the Champa dug among the flowers outside the town
/// wall in the north-east corner (cell (46, 15)). Once the guards at the gate
/// have let the party in, the ladder in it takes him down into the catacombs.
/// Before that, the first time on a visit, a worker pops his head up out of
/// the hole, Felix hops back out of his way, and the worker turns him away
/// (Madra_AskComingThroughHole); later steps that visit do nothing.
void Madra_WorkerBlocksHole(void) {
  if (Event_GetFlag(FLAG_MADRA_GATE_INSPECTED)) {
    Event_PlaySound(SE_MAP_EXIT);
    Import_EventSetExitRequest(MADRA_TO_MADRA_CATACOMBS_LADDERS_EAST);
  } else if (!Event_GetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Madra_PlaceFacing(ACTOR_HOLE_WORKER, toFIXED(744), toFIXED(257),
                      FACING_SOUTH);
    Actor_GetById(ACTOR_FELIX)->velocityY = toFIXED(5);
    Actor_SetSpeed(ACTOR_FELIX, 2);
    Actor_WalkToAndWait(ACTOR_FELIX, 720, 244);
    Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
    Madra_PlaceFacing(ACTOR_FELIX, toFIXED(720), toFIXED(244),
                      FACING_SOUTHEAST);
    Event_SetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE);
    Madra_AskComingThroughHole();
    Event_End();
  }
}

/// Once the Kibombo have raided Madra, stepping into the Champa's hole (cell
/// (46, 15)) the first time on a visit makes the worker pop up out of it again,
/// Felix hops back, and the worker says they are sealing it
/// (Madra_AskComingThroughSealedHole). Later steps that visit do nothing, and
/// the hole no longer leads down.
void Madra_WorkerBlocksSealedHole(void) {
  if (!Event_GetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Madra_PlaceFacing(ACTOR_HOLE_WORKER, toFIXED(744), toFIXED(256),
                      FACING_SOUTH);
    Actor_GetById(ACTOR_FELIX)->velocityY = toFIXED(5);
    Actor_SetSpeed(ACTOR_FELIX, 2);
    Actor_WalkToAndWait(ACTOR_FELIX, 720, 244);
    Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
    Madra_PlaceFacing(ACTOR_FELIX, toFIXED(720), toFIXED(244),
                      FACING_SOUTHEAST);
    Event_SetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE);
    Madra_AskComingThroughSealedHole();
    Event_End();
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gMadra_TriggersOnReturn;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gMadra_TriggersAfterRaid;
  return gMadra_TriggersBeforeRaid;
}

/// Makes the cells around the Champa's hole, (45, 14) to (47, 16), plain
/// ground, copied from spare cells at (109, 14), so that stepping there no
/// longer runs anything.
void Madra_SealHoleCells(void) {
  MapGrid_CopyCellAttributes(109, 14, 3, 3, 45, 14);
}

/// When Felix brings Piers home the Champa's hole is filled in: a mound of
/// earth is drawn where the ladder was, copied from spare cells at (51, 10),
/// and its cells become plain ground.
void Madra_SealHole(void) {
  MapGrid_CopyCellsLayered(51, 10, 3, 3, 45, 10);
  Madra_SealHoleCells();
}

/// Sets up the town as the party enters: the hole is filled in once Felix
/// brings Piers home; otherwise the worker stands down in it, out of sight
/// unless he has already popped up this visit, and the two who saw the shaft of
/// ice are gone once Briggs is caught. Coming out of the jail Felix is granted
/// passage to Osenia (Madra_GrantPassageToOsenia), and on the return, coming
/// out of the mayor's house after the farewell, he is met once by the mayor and
/// Karst (Madra_MeetKarst).
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    Madra_SealHole();
  } else {
    // The worker stands down in the hole.
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HOLE_WORKER), 0);
    actor = Actor_GetById(ACTOR_HOLE_WORKER);
    actor->radius = Actor_GetById(ACTOR_HOLE_WORKER)->radius * 4 / 3;
    ((struct FieldSprite *)Actor_GetById(ACTOR_HOLE_WORKER)->sprite)->flags |=
        SPRITE_FLAG_SUNK;
    ((struct Sprite *)Actor_GetById(ACTOR_HOLE_WORKER)->sprite)->sunkRows = 8;
    ((struct FieldSprite *)Actor_GetById(ACTOR_HOLE_WORKER)->sprite)->dirty = 1;
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0 &&
        Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
      Actor_SetPosition(ACTOR_ICE_SHAFT_WITNESS, 0, 0);
      Actor_SetPosition(ACTOR_FRIGHTENED_WITNESS, 0, 0);
      if (Event_GetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE) == 0)
        Actor_SetPosition(ACTOR_HOLE_WORKER, 0, 0);
    } else if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0 ||
               Event_GetFlag(FLAG_MADRA_GATE_INSPECTED) != 0 ||
               Event_GetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE) == 0) {
      Actor_SetPosition(ACTOR_HOLE_WORKER, 0, 0);
    }
  }
  if (gMainRoutineState.entrance == MADRA_FROM_MADRA_ROOMS_HOUSES_WEST &&
      Event_GetFlag(FLAG_OSENIA_PASSAGE_GRANTED) == 0)
    Madra_GrantPassageToOsenia();
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0 &&
      Event_GetFlag(FLAG_MADRA_FAREWELL_SAID) != 0) {
    if (gMainRoutineState.entrance == MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHWEST &&
        Event_GetFlag(FLAG_MADRA_KARST_MET) == 0) {
      Madra_MeetKarst();
      Event_SetFlag(FLAG_MADRA_KARST_MET);
    }
    if (Event_GetFlag(FLAG_MADRA_KARST_MET))
      Actor_SetPosition(ACTOR_LADY, 0, 0);
  } else {
    Actor_SetPosition(ACTOR_CYCLONE_CHIP, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Talking to the strongman outside the jail who saw Piers raise his shaft of
/// ice: "Was I just dreaming when that shaft of ice appeared?" Told yes, he is
/// relieved; told no, he hopes Felix is joking.
void Madra_AskAboutIceShaft(s32 actor) {
  s32 msg;

  Event_Start();
  Event_EndLastingPsynergy(0);
  msg = (s32)&MsgMadraWasIJustDreamingWhen;
  Event_SetMessageId(msg);
  // "Was I just dreaming when that shaft of ice appeared?"
  Event_OpenMessage(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// The actor the lines of the boy who asks about Briggs open beside.
// BUG: the boy's lines open beside ACTOR_FIRST_PLACED, the young man who talks
// of the elder, wherever he stands, and not beside the boy:
// FieldMessage_ShowAtActor places the window by its speaker's place on screen.
// In a render of the talk the young man is off screen to the north-west and
// every window stands at the screen's left edge; with BUGFIX's speaker, the
// boy, they stand over his head.
#ifdef BUGFIX
#define BRIGGS_FAN_SPEAKER ACTOR_BRIGGS_FAN
#else
#define BRIGGS_FAN_SPEAKER ACTOR_FIRST_PLACED
#endif

/// Talking to the boy in a blue shirt beside the round sanctum: he asks whether
/// the party came over the Dehkan Plateau and saw the boat grounded near the
/// cave, or, once Briggs is caught, whether Felix is the one who beat him up.
void Madra_AskAboutBriggs(void) {
  s32 msg;

  Event_Start();
  Event_EndLastingPsynergy(0);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    msg = (s32)&MsgMadraHeyMisterAreYouGuy;
    Event_SetMessageId(msg);
    // "Hey, mister! Are you the guy who beat up Briggs?"
    Event_OpenMessage(BRIGGS_FAN_SPEAKER, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(BRIGGS_FAN_SPEAKER, 0);
  } else {
    if (Event_GetFlag(FLAG_MADRA_BOY_HAS_ASKED) == 0)
      Actor_FaceActor(ACTOR_BRIGGS_FAN, ACTOR_FELIX, 0);
    msg = (s32)&MsgMadraDidYouComeHereOver;
    Event_SetMessageId(msg);
    // "Did you come here over the Dehkan Plateau? Yeah? You must have seen..."
    Event_OpenMessage(BRIGGS_FAN_SPEAKER, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(BRIGGS_FAN_SPEAKER, 0);
  }
  Actor_FaceDirection(ACTOR_BRIGGS_FAN, FACING_SOUTH, 0);
  Event_SetFlag(FLAG_MADRA_BOY_HAS_ASKED);
  Event_End();
}

/// Casting Mind Read on the boy beside the sanctum before he has asked his
/// question makes him ask it aloud, as talking to him does
/// (Madra_AskAboutBriggs); while Briggs is free he starts, an exclamation mark
/// over his head, and turns to Felix first.
void Madra_MindReadBriggsFan(void) {
  s32 msg, x, z;

  Event_Start();
  Event_EndLastingPsynergy(0);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    msg = (s32)&MsgMadraHeyMisterAreYouGuy;
    Event_SetMessageId(msg);
    // "Hey, mister! Are you the guy who beat up Briggs?"
    Event_OpenMessage(BRIGGS_FAN_SPEAKER, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(BRIGGS_FAN_SPEAKER, 0);
  } else {
    if (Event_GetFlag(FLAG_MADRA_BOY_HAS_ASKED) == 0) {
      x = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
      z = 30;
      FieldActor_ShowEmote(ACTOR_BRIGGS_FAN, x, z);
      Actor_FaceActor(ACTOR_BRIGGS_FAN, ACTOR_FELIX, 0);
    }
    msg = (s32)&MsgMadraDidYouComeHereOver;
    Event_SetMessageId(msg);
    // "Did you come here over the Dehkan Plateau? Yeah? You must have seen..."
    Event_OpenMessage(BRIGGS_FAN_SPEAKER, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(BRIGGS_FAN_SPEAKER, 0);
  }
  Actor_FaceDirection(ACTOR_BRIGGS_FAN, FACING_SOUTH, 0);
  Event_SetFlag(FLAG_MADRA_BOY_HAS_ASKED);
  Event_End();
}

/// Talking to the auburn-haired boat owner south-east of the jail when Felix
/// brings Piers home: she asks whether a girl named Jenna is with him. Told
/// yes, she says the four weatherworn travellers with the amazing ship must
/// have been looking for them; told no, she asks him to let Jenna know Isaac
/// is looking for her, and asks again next time.
void Madra_AskAboutJenna(void) {
  s32 flags;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(ACTOR_BOAT_OWNER, ACTOR_FELIX, 0);
  Event_SetMessageId((s32)&MsgMadraIsThereByAnyChance);
  flags = 0;
  // "Is there by any chance a girl named Jenna with you?" Yes: "Ah! Those
  // people must have been looking for..." No: "Oh... Well, if you run across
  // anyone named..."
  Event_OpenMessage(ACTOR_BOAT_OWNER, flags);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_SetFlag(FLAG_MADRA_TOLD_OF_JENNA);
    // "Ah! Those people must have been looking for you! There were four of..."
    Event_ShowMessage(ACTOR_BOAT_OWNER, 0);
    (gFieldState->message)++;
  } else {
    (gFieldState->message)++;
    // "Oh... Well, if you run across anyone named Jenna, let her know Isaac..."
    Event_ShowMessage(ACTOR_BOAT_OWNER, 0);
  }
  Event_End();
}

struct Actor *Actor_GetById(u32 objectId);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSpeedParameters(s32 id, FIXED speed, FIXED acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_SetAnimationAndWait(s32 id, s32 mode);
void Actor_WalkToById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_LaunchFromLeader(s32 id, s32 a, s32 b, s32 c);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
void ItemDisplay_ShowItem(s32 item, s32 a);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void FieldMessage_Show(s32 message, s32 position);
void FieldCamera_FollowActor(s32 id, s32 mode);
void Actor_SetSpriteColor(struct Actor *actor, s32 attribute);

/// The first time Felix walks north into the south gate (cells (29, 26) and
/// (30, 26)), the two guards stop the party: "You there! Hold your ground!"
/// Kraden asks what the trouble is, one guard walks round the party looking it
/// over, and they agree it is not the Champa, who raided the town to free
/// Briggs, and let it pass.
void Madra_InspectPartyAtGate(void) {
  struct Actor *actor;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgMadraYouThereHoldYourGround);
  // "You there! Hold your ground!"
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
  FieldCamera_MoveTo(476, CAMERA_KEEP, 420, 1);
  Actor_WalkBy(ACTOR_SECOND_GATE_GUARD, 3, 8);
  Actor_WalkByAndWait(ACTOR_GATE_GUARD, -3, 8);
  Actor_WaitForMoveById(ACTOR_SECOND_GATE_GUARD);
  Actor_WalkToAndWait(ACTOR_FELIX, 472, 426);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 1, FACING_NORTH);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  // "What seems to be the trouble?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
  Event_Wait(20);
  // "Just wait there..."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GATE_GUARD, ACTOR_SECOND_GATE_GUARD, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_SECOND_GATE_GUARD, ACTOR_GATE_GUARD, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_SECOND_GATE_GUARD, ACTOR_GATE_GUARD, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_SECOND_GATE_GUARD, ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_GetById(ACTOR_SECOND_GATE_GUARD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_SECOND_GATE_GUARD, 3);
  Actor_WalkToAndWait(ACTOR_SECOND_GATE_GUARD, 457, 402);
  Actor_WalkByAndWait(ACTOR_SECOND_GATE_GUARD, 16, 0);
  Actor_WalkToAndWait(ACTOR_SECOND_GATE_GUARD, 472, 420);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_SECOND_GATE_GUARD, 484, 420);
  Actor_GetById(ACTOR_SECOND_GATE_GUARD)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  Actor_FaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH, 0);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
  // "They don't look like they're Champa..."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
  // "I agree..."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
  Actor_FaceDirectionAndWait(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
  Event_Wait(5);
  // "Sorry for the delay. You may pass."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
  Actor_GetById(ACTOR_SECOND_GATE_GUARD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_SECOND_GATE_GUARD, 3);
  Actor_WalkByAndWait(ACTOR_SECOND_GATE_GUARD, 0, -16);
  Actor_WalkToAndWait(ACTOR_SECOND_GATE_GUARD, 475, 400);
  Actor_WalkByAndWait(ACTOR_SECOND_GATE_GUARD, -16, 0);
  Actor_GetById(ACTOR_SECOND_GATE_GUARD)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  FieldCamera_MoveTo(476, CAMERA_KEEP, 396, 1);
  Actor_WalkToAndWait(ACTOR_SECOND_GATE_GUARD, 459, 416);
  Actor_FaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  // "What were you checking us for?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 2);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GATE_GUARD, 2);
  Event_Wait(20);
  // "Our town was sacked by pirates..."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
  Event_Wait(10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  // "They'd come to free their leader, Briggs."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldActor_ShowEmote(ACTOR_SECOND_GATE_GUARD,
                       EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
  // "They came from the east, a small party, and we repelled them with..."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
  FieldActor_ShowEmote(ACTOR_SECOND_GATE_GUARD,
                       EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  // "But then, foreigners struck from the west, exploiting a weak point..."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  // "That's over now, though... Feel free to relax and enjoy your time..."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
  Actor_SetSpeed(ACTOR_GATE_GUARD, 0.3);
  Actor_SetSpeed(ACTOR_SECOND_GATE_GUARD, 0.3);
  Actor_WalkBy(ACTOR_GATE_GUARD, 3, -8);
  Actor_WalkBy(ACTOR_SECOND_GATE_GUARD, -3, -8);
  Actor_WaitForMoveById(ACTOR_GATE_GUARD);
  Actor_WaitForMoveById(ACTOR_SECOND_GATE_GUARD);
  Actor_FaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_GetById(gMainRoutineState.leaderActorId);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_SetFlag(FLAG_MADRA_GATE_INSPECTED);
  Actor_SetPosition(ACTOR_HOLE_WORKER, 0, 0);
  Event_End();
}

/// Coming out of the jail after seeing Piers raise his shaft of ice, Felix
/// meets Milady and her guard, who hurry up crying "Has Piers escaped!?"; she
/// asks whether the party is going to Osenia and grants it passage over the
/// drawbridge. It runs once (FLAG_PIERS_INTERROGATED, then
/// FLAG_OSENIA_PASSAGE_GRANTED).
void Madra_GrantPassageToOsenia(void) {
  if (!GameFlag_Get(FLAG_PIERS_INTERROGATED))
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveTo(124, CAMERA_KEEP, 420, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_LADY, 0.8);
  Actor_SetSpeed(ACTOR_LADYS_GUARD, 0.8);
  Event_StartScreenTransition();
  Actor_SetPosition(ACTOR_LADY, 172, 332);
  Actor_SetPosition(ACTOR_LADYS_GUARD, 172, 324);
  Event_SetMessageId((s32)MsgMadraHasPiersEscaped);
  Event_WaitTransitionFrames();
  // "Has Piers escaped!?"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_WalkTo(ACTOR_LADY, 156, 372);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_LADYS_GUARD, 156, 372);
  Actor_WalkTo(ACTOR_FELIX, 76, 388);
  Actor_WaitForMoveById(ACTOR_LADY);
  Actor_WalkTo(ACTOR_LADY, 108, 380);
  Actor_WaitForMoveById(ACTOR_LADYS_GUARD);
  Actor_WalkTo(ACTOR_LADYS_GUARD, 124, 380);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_LADY);
  Actor_WaitForMoveById(ACTOR_LADYS_GUARD);
  Actor_FaceDirection(ACTOR_LADY, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LADY, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
  Actor_SetAttachedEffect(ACTOR_LADYS_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_LADY,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_LADY, 102, 340);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_LADYS_GUARD, 102, 340);
  Actor_WaitForMoveById(ACTOR_LADY);
  Actor_SetPosition(ACTOR_LADY, 4, 4);
  Actor_WaitForMoveById(ACTOR_LADYS_GUARD);
  Actor_SetPosition(ACTOR_LADYS_GUARD, 4, 4);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_Wait(40);
  Event_Wait(120);
  Actor_SetPosition(ACTOR_LADY, 102, 340);
  Actor_SetPosition(ACTOR_LADYS_GUARD, 102, 340);
  Actor_WalkToAndWait(ACTOR_LADYS_GUARD, 100, 364);
  Actor_WalkTo(ACTOR_LADYS_GUARD, 124, 380);
  Actor_WalkToAndWait(ACTOR_LADY, 100, 380);
  Actor_WaitForMoveById(ACTOR_LADYS_GUARD);
  Actor_WalkTo(ACTOR_LADYS_GUARD, 132, 396);
  Actor_WalkToAndWait(ACTOR_LADY, 108, 396);
  Actor_WaitForMoveById(ACTOR_LADYS_GUARD);
  Actor_FaceActorById(ACTOR_FELIX, ACTOR_LADY, 0);
  Actor_FaceDirection(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_LADY, FACING_NORTH, 0);
  FieldActor_ShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Oh, I'm so relieved... So Piers hasn't escaped?"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_FaceActorById(ACTOR_LADYS_GUARD, ACTOR_LADY, 0);
  Actor_SetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
  Event_Wait(20);
  // "If he had escaped, the elder would no longer be able to protect him."
  Event_ShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
  Actor_FaceActorById(ACTOR_LADY, ACTOR_LADYS_GUARD, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_LADY, 2);
  Event_Wait(20);
  // "But...from the look of things, it seems there was a scuffle..."
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
  Event_Wait(20);
  // "The cell is in disarray..."
  Event_ShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
  FieldActor_ShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "What happened in there, I wonder?"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_FaceDirection(ACTOR_LADY, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
  Event_Wait(20);
  // "It seems Piers is a man of many mysteries."
  Event_ShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
  Event_Wait(20);
  // "Not the least of which being that boat he arrived in..."
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_LADY, FACING_WEST, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Oh, you there..."
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  // "You look like travelers."
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
  Actor_FaceActorById(ACTOR_LADYS_GUARD, ACTOR_LADY, 0);
  Event_Wait(20);
  // "Milady, now is not the time to be speaking with strangers!"
  Event_ShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
  Actor_FaceActorById(ACTOR_LADY, ACTOR_LADYS_GUARD, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LADY, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "It's all right... This one has such kind eyes."
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_FaceActorById(ACTOR_LADY, ACTOR_FELIX, 0);
  Event_Wait(30);
  // "When did you arrive? And where are you going?"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_RunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
  Event_Wait(20);
  // "My lady, please..."
  Event_ShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Are you...going to Osenia?" Yes: "I knew it!" No: "No need to be shy, my
  // friends."
  FieldMessage_ShowAtActor(ACTOR_LADY, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    // "I knew it!"
    Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
    (gFieldState->message)++;
  } else {
    (gFieldState->message)++;
    // "No need to be shy, my friends."
    Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  }
  Actor_FaceActorById(ACTOR_LADY, ACTOR_LADYS_GUARD, 0);
  Event_Wait(20);
  // "One requires permission to cross the bridge to Osenia, correct?"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_LADY, 2);
  Event_Wait(20);
  // "Then I hereby grant my permission to these four travelers!"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  FieldActor_ShowEmote(ACTOR_LADYS_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
  Event_Wait(20);
  // "Relay that message at once!"
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Actor_WalkByAndWaitById(ACTOR_LADYS_GUARD, 6, 0);
  Actor_WalkToAndWait(ACTOR_LADYS_GUARD, 164, 316);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_LADY, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
  Event_Wait(20);
  // "You may all continue your journey and cross into Osenia."
  Event_ShowMessageThenWait(ACTOR_LADY, 0, 10);
  Actor_SetPosition(ACTOR_LADYS_GUARD, 0, 0);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
  Event_Wait(20);
  GameFlag_Set(FLAG_OSENIA_PASSAGE_GRANTED);
  Actor_WalkByAndWaitById(ACTOR_LADY, 30, 0);
  Actor_WalkToAndWait(ACTOR_LADY, 164, 316);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_LADY, 0, 0);
  Event_Wait(20);
  Event_End();
}

/// Once the Kibombo have raided Madra, the first time Felix walks north into
/// the south gate (cells (29, 26) and (30, 26)) a guard cries "Hey! You lot!
/// Hold it right there!", but the other says the party could not be involved,
/// and with Kraden's answers they let it go.
void Madra_StopPartyAfterRaid(void) {
  struct Actor *actor;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgMadraHeyYouLotHold);
  // "Hey! You lot! Hold it right there!"
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
  FieldCamera_MoveTo(476, CAMERA_KEEP, 400, 1);
  Actor_WalkByAndWait(ACTOR_GATE_GUARD, -3, 8);
  Actor_WalkToAndWait(ACTOR_FELIX, 472, 426);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  Actor_LaunchFromLeader(ACTOR_KRADEN, 16, 1, FACING_NORTH);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_WalkByAndWait(ACTOR_SECOND_GATE_GUARD, 3, 8);
  Actor_FaceDirectionAndWait(ACTOR_SECOND_GATE_GUARD, FACING_EAST);
  Actor_SetAnimation(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Hang on, there's no need to stop them. They couldn't be involved in..."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "If you're talking about the pirate incident, I believe we resolved..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Actor_FaceDirectionAndWait(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_SHAKE_HEAD);
  // "This has nothing to do with the Champa this time, old man! This..."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_KRADEN, 6, 15);
  Actor_Jump(ACTOR_KRADEN, 6, 23);
  // ""Old man"?!? Hmph!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_GATE_GUARD, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  // "Wait... Did you say you were attacked again? By the Kibombo?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_SECOND_GATE_GUARD, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  // "Do you know about the Kibombo?" Yes: "You must be pretty worldly,
  // considering that..." No: "Not many people do... They're a barbaric..."
  Event_OpenMessage(ACTOR_SECOND_GATE_GUARD, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH_NORTHWEST);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(25);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
    // "You must be pretty worldly, considering that they rarely leave the..."
    Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
    (gFieldState->message)++;
  } else {
    Event_Wait(25);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
    (gFieldState->message)++;
    // "Not many people do... They're a barbaric tribe from central..."
    Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
  }
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  // "When did all this happen?!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_SECOND_GATE_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE,
                       30);
  // "Not too long ago..."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GATE_GUARD, 2);
  Event_Wait(20);
  // "Right after the mayor left, actually..."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 10);
  if (Event_GetFlag(FLAG_KIBOMBO_WARRIORS_FOUND)) {
    Event_SetMessageId((s32)MsgMadraItWasThemWasnt);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    Event_Wait(20);
    // "It was them, wasn't it, Felix?"
    Event_OpenMessage(ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
      // "Of course! They did seem to be acting awfully strange when we saw..."
      Event_ShowMessage(ACTOR_KRADEN, 0);
      (gFieldState->message)++;
    } else {
      Event_Wait(20);
      (gFieldState->message)++;
      Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
      // "Surely you remember, Felix! The cave? Those strange men we saw in..."
      Event_ShowMessage(ACTOR_KRADEN, 0);
    }
  }
  Event_SetMessageId((s32)MsgMadraWasAnythingStolen);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Was anything stolen?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_GATE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_GATE_GUARD, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHWEST, 0);
  FieldActor_ShowEmote(ACTOR_SECOND_GATE_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE,
                       30);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTHEAST, 0);
  // "Only a single black orb... Everything else was untouched."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "A black orb..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "I'm afraid we don't have much more information than that."
  Event_ShowMessageThenWait(ACTOR_GATE_GUARD, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
  Event_Wait(10);
  // "You helped us with the Champa, so, I'll tell you what... Maybe you..."
  Event_ShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
  Actor_SetSpeed(ACTOR_GATE_GUARD, 0.3);
  Actor_SetSpeed(ACTOR_SECOND_GATE_GUARD, 0.3);
  Actor_WalkBy(ACTOR_GATE_GUARD, 3, -8);
  Actor_WalkBy(ACTOR_SECOND_GATE_GUARD, -3, -8);
  Actor_WaitForMoveById(ACTOR_GATE_GUARD);
  Actor_WaitForMoveById(ACTOR_SECOND_GATE_GUARD);
  Actor_FaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  Event_SetFlag(FLAG_MADRA_RAID_QUESTIONING_DONE);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_GetById(gMainRoutineState.leaderActorId);
  if (actor != NULL)
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_End();
}

/// Sets an actor's animation, and with Madra_SetAnimationThenWait waits frames
/// after.
#define Madra_SetAnimation(id, animation)                                      \
  do {                                                                         \
    Actor_SetAnimation(id, animation);                                         \
  } while (0)
#define Madra_SetAnimationThenWait(id, animation, frames)                      \
  do {                                                                         \
    Actor_SetAnimation(id, animation);                                         \
    Event_Wait(frames);                                                        \
  } while (0)

/// Coming out of the mayor's house after Piers's farewell, Felix hears the
/// mayor call "Felix, wait!" and is given the Cyclone Chip promised for the
/// boat (with no room for it, the mayor sets it down beside his house to be
/// picked up later). Then Karst, hearing Felix's name, confronts the party
/// about Menardi, learns of Isaac and swears to have her revenge.
void Madra_MeetKarst(void) {
  /* FAKEMATCH(wrapper): the animations set through Madra_SetAnimation's
     block statements. */
  struct Actor *actor;
  FIXED x = toFIXED(153);
  FIXED z;

  z = toFIXED(259);
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_StartScreenTransition();
  // The second flag is the one after the six DataTransfer_Apply copies from
  // The Broken Seal's save, which no code sets.
  if (GameFlag_Get(FLAG_TRANSFERRED_VAULT_WARNED_OF_ESCAPED_THIEVES) ||
      GameFlag_Get(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + TRANSFERRED_FLAG_COUNT))
    GameFlag_Set(FLAG_BANDIT_ON_WORLD_MAP);
  Event_SetMessageId((s32)MsgMadraFelixWait);
  // "Felix, wait!"
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 150, 280);
  Actor_WalkByById(ACTOR_FELIX, 16, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -16, 0, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(20);
  MapGrid_AnimateCells(
      gMadra_Doors[MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST - 1].steps,
      gMadra_Doors[MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST - 1].x,
      gMadra_Doors[MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST - 1].y);
  Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_MAYOR, x, z,
                                                 FACING_SOUTH);
  Actor_WalkByAndWaitById(ACTOR_MAYOR, 0, 4);
  Madra_SetAnimationThenWait(ACTOR_MAYOR, ANIM_NOD, 20);
  // "I believe I promised you a reward for helping with the boat, didn't I?"
  FieldMessage_ShowAtActor(ACTOR_MAYOR, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_FELIX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_FELIX, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_FELIX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_FELIX, 0);
  Actor_FaceDirectionById(ACTOR_MAYOR, FACING_SOUTH, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
    Actor_FaceDirectionById(ACTOR_MAYOR, FACING_SOUTH, 0);
    Event_Wait(20);
    Madra_SetAnimation(ACTOR_MAYOR, ANIM_SHAKE_HEAD);
    Event_Wait(33);
    Event_Wait(20);
    // "Imagine! You came all this way to put us at ease, and I totally..."
    Event_ShowMessageAndWait(ACTOR_MAYOR, 0);
    (gFieldState->message)++;
  } else {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
    Actor_FaceDirectionById(ACTOR_MAYOR, FACING_SOUTH, 0);
    Event_Wait(20);
    Madra_SetAnimation(ACTOR_MAYOR, ANIM_SHAKE_HEAD);
    Event_Wait(33);
    Event_Wait(20);
    (gFieldState->message)++;
    // "Well, you may have forgotten, Felix, but I have not."
    Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  }
  FieldActor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Regardless, I told you that I would reward you when the boat was..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  Madra_SetAnimation(ACTOR_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "That being the case, I'd like to give you that reward now."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Event_Wait(20);
  // "We can't accept it now, though, could we, Jenna?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  Event_Wait(20);
  // "Madra is still in shambles from the disaster... We couldn't possibly..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  FieldActor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_Wait(20);
  // "Nonsense! Misfortune or no, a promise is a promise!"
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  Actor_WalkByAndWaitById(ACTOR_MAYOR, 16, 8);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH, 0);
  if (PartyInventory_FindRoom(ITEM_CYCLONE_CHIP) >= 0) {
    ItemDisplay_ShowItem(ITEM_CYCLONE_CHIP,
                         ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
    PartyInventory_GiveItem(ITEM_CYCLONE_CHIP, 0);
    (gFieldState->message)++;
    Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
    GameFlag_Set(FLAG_MADRA_CYCLONE_CHIP_GIVEN);
  } else {
    s32 count = gFieldState->message + 1;

    Event_SetMessageId((s32)MsgMadraMayorTriedToGiveFelix);
    MessageArgument_Push(ITEM_CYCLONE_CHIP, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_Show((gFieldState->message)++, 5);
    Actor_RunRepeatedMotion(ACTOR_MAYOR, 2);
    Event_ShowMessageAndWait(ACTOR_MAYOR, 0);
    Actor_WalkByAndWaitById(ACTOR_MAYOR, 16, 0);
    MessageArgument_Push(ITEM_CYCLONE_CHIP, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_Show((gFieldState->message)++, 5);
    Actor_WalkByAndWait(ACTOR_MAYOR, -16, 0);
    Actor_FaceDirectionAndWait(ACTOR_MAYOR, FACING_SOUTH);
    Actor_SetPosition(ACTOR_CYCLONE_CHIP, 192, 256);
    gFieldState->message = count;
  }
  Actor_WalkByAndWait(ACTOR_MAYOR, -16, -8);
  Actor_FaceDirectionAndWait(ACTOR_MAYOR, FACING_SOUTH);
  FieldActor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "I am glad that I was able to give it to you before you left."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  Madra_SetAnimation(ACTOR_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  // "Your travels seem to take you from danger into greater danger. Please..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Madra_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Madra_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Madra_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_MAYOR, FACING_NORTH);
  Actor_WalkToAndWait(ACTOR_MAYOR, 153, 259);
  Actor_SetPosition(ACTOR_MAYOR, 0, 0);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  Event_Wait(20);
  // "Thank you, Mr. Mayor."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  // "...Felix?"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActorById(ACTOR_FELIX, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_KARST, 0);
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_KARST, 1);
  Event_Wait(60);
  Sound_Play(MUS_CONFRONTATION);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You... He just called you Felix, didn't he?"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceDirection(ACTOR_KARST, FACING_WEST, 0);
  Actor_TurnToFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Actor_TurnToFaceEachOther(ACTOR_PIERS, ACTOR_KRADEN, 0);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  // "Why didn't I see it before? You are Felix!"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_FELIX, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_KARST, 0);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
  Actor_FaceDirectionAndRefreshById(ACTOR_KARST, FACING_EAST);
  Event_Wait(20);
  // "Then Menardi should be somewhere around here, too... And yet..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Event_Wait(20);
  // "You won't see Menardi or Saturos ever again. Isaac killed them..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Sheba! Quiet!"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Hmph! I heard that, you wretched little thing!"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceActorById(ACTOR_KARST, ACTOR_FELIX, 0);
  Event_Wait(20);
  // "Can it be true? Is my sister dead?" Yes: "I can't believe it... No one
  // has the power..." No: "No? Then where is she!? Show me my sister!"
  FieldMessage_ShowAtActor(ACTOR_KARST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
    // "No? Then where is she!? Show me my sister!"
    Event_ShowMessageAndWait(ACTOR_KARST, 0);
    FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_Wait(40);
    Madra_SetAnimation(ACTOR_KARST, ANIM_NOD);
    Event_Wait(20);
    Event_Wait(20);
    // "What is the matter? Is it so very hard? You were traveling with her..."
    Event_ShowMessageAndWait(ACTOR_KARST, 0);
    Actor_TurnToFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
    Event_Wait(40);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
    Event_Wait(40);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    Madra_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    Event_Wait(33);
    Event_Wait(20);
    FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "She would never have left you behind...and yet you are alone... So..."
    Event_ShowMessageAndWait(ACTOR_KARST, 0);
  } else {
    Event_SetMessageId((s32)MsgMadraICantBelieveItNo);
    // "I can't believe it... No one has the power to defeat Saturos and my..."
    Event_ShowMessageAndWait(ACTOR_KARST, 0);
    Actor_TurnToFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
    Event_Wait(40);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
    Event_Wait(40);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    Madra_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    Event_Wait(33);
    Event_Wait(20);
    FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Yet you all avoid my gaze... And that look on your faces... It...is..."
    Event_ShowMessageAndWait(ACTOR_KARST, 0);
  }
  Event_SetMessageId((s32)MsgMadraWhoIsThisIsaac);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  // "Who is this Isaac?"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Madra_SetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Madra_SetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Madra_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_KARST, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "What? You feign ignorance? Why? Would you protect him from my fury!?"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_Wait(60);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Never mind. I believe I've heard some rumors of a traveler named..."
  Event_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Madra_SetAnimation(ACTOR_KARST, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "Is this Isaac the same one they've been speaking of? Is he pursuing you?"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceDirectionById(ACTOR_KARST, FACING_EAST, 0);
  Event_Wait(20);
  // "If that is the case, then this Isaac should show up here sooner or..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "How do you know it wasn't Felix who defeated your sister?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  // "What do you take me for? I'm a warrior, like my sister... I can..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Madra_SetAnimation(ACTOR_KARST, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  Event_Wait(20);
  // "Even if you doubled Felix's power, he would still be no match for my..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Actor_FaceDirection(ACTOR_KARST, FACING_WEST, 0);
  Event_Wait(20);
  // "And besides, I doubt Felix could find it in himself to betray my sister."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "He knows that the lives of those he holds dear would hang in the..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_KARST, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "But I must congratulate you on your successes at both the Mercury..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Madra_SetAnimation(ACTOR_KARST, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  Event_Wait(20);
  // "Although your success there now prevents us from returning to the..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  // "You would have failed, of course, had we not researched how to climb..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Oh, didn't Menardi tell you?"
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KARST, FACING_EAST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
  Event_Wait(20);
  Actor_FaceDirectionAndRefreshById(ACTOR_KARST, FACING_EAST);
  Event_Wait(20);
  // "Regardless, Isaac won't be getting in my way."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceDirectionById(ACTOR_KARST, FACING_WEST, 0);
  Event_Wait(20);
  // "You will have to find your own way to the Western Sea."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  // "And if you make the journey, you'll have to light Jupiter on your..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Madra_SetAnimation(ACTOR_KARST, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "We have our own work to do, and number one on the list is taking..." Yes:
  // "Aren't you an obedient boy... If you're worried..." No: "What's the
  // matter? Afraid you can't perform..."
  FieldMessage_ShowAtActor(ACTOR_KARST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Aren't you an obedient boy... If you're worried you can't get it..."
    Event_ShowMessageThenWait(ACTOR_KARST, 0, 0);
    (gFieldState->message)++;
  } else {
    FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    (gFieldState->message)++;
    // "What's the matter? Afraid you can't perform your duties? We could..."
    Event_ShowMessageThenWait(ACTOR_KARST, 0, 0);
  }
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_Wait(33);
  // "No, thank you. I believe we'll just find Jupiter Lighthouse on our..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Madra_SetAnimation(ACTOR_KARST, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "I see. Just don't let us down."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_RunRepeatedMotion(ACTOR_KARST, 2);
  Event_Wait(20);
  // "We're going to find that Isaac..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Actor_FaceDirectionById(ACTOR_KARST, FACING_EAST, 0);
  Event_Wait(20);
  // "I'm going to find this Isaac... And I swear that the last thing he..."
  Event_ShowMessageAndWait(ACTOR_KARST, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_WalkByAndWaitById(ACTOR_KARST, 24, 24);
  Actor_WalkByAndWaitById(ACTOR_KARST, 132, 0);
  Actor_SetPosition(ACTOR_KARST, 0, 0);
  Scene_PlayMusic();
  FieldCamera_FollowActor(ACTOR_FELIX, 1);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Wow. And I thought Saturos and Menardi had issues."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "We shouldn't take Karst too lightly..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_FELIX, 0);
  Event_Wait(20);
  // "Wait a moment... Karst did say "we," didn't she?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(10);
  // "Yes. Saturos and Menardi traveled as a pair. Perhaps she has a..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(20);
  // "We ought to warn Isaac that Karst is after him. Maybe we should try..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Event_Wait(20);
  // "Believe me, I'd love to see Isaac again, but we just don't have time..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(20);
  // "Even if we did find them, I think there is a good chance we'd end up..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_FELIX, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Event_Wait(20);
  // "Why?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_PIERS, 0);
  Event_Wait(20);
  // "What we are trying to achieve, they are trying to prevent... And..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Event_Wait(20);
  // "But, Jenna... Aren't you and Isaac an item? Couldn't you, you know..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_JENNA), SPRITE_COLOR_RED_TINT);
  // "A... An item? No! I mean... It's not like that! Not...really..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(20);
  // "...Stupid Sheba..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_TurnToFaceEachOther(ACTOR_FELIX, ACTOR_PIERS, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH);
  // "Ah! Well! Er, yes. For now, we should press onward to Jupiter..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_JENNA), SPRITE_COLOR_NORMAL);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Event_Wait(20);
  // "But...what about Garet and the others?"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Don't you mean...Isaac?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_JENNA), SPRITE_COLOR_RED_TINT);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Event_Wait(20);
  // "Sheba! Be quiet! Sheesh..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  // "*Ahem!* Are you ready to listen yet?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_JENNA), SPRITE_COLOR_NORMAL);
  Actor_FaceActorById(ACTOR_FELIX, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
  Event_Wait(40);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "They were strong enough to defeat Saturos and Menardi. They won't be..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Madra_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Madra_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Madra_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  // "We should continue on ahead, just as Saturos and Menardi would have..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Sheba? Piers? Are you ready?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Madra_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Madra_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  // "Then we should be going, right, Felix?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Madra_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Madra_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Madra_SetAnimationThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Madra_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_GetById(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
                             actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Madra_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestinationById(ACTOR_JENNA, actor->x.part[0].integer,
                             actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Madra_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestinationById(ACTOR_SHEBA, actor->x.part[0].integer,
                             actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Madra_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  actor = Actor_GetById(ACTOR_FELIX);
  if (actor != NULL)
    Actor_SetDestinationById(ACTOR_PIERS, actor->x.part[0].integer,
                             actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  GameFlag_Set(FLAG_MENARDIS_SHIP_SEEN);
  GameFlag_Set(FLAG_MADRA_KARST_MET);
  Event_End();
}
#undef Madra_SetAnimation
#undef Madra_SetAnimationThenWait

/// Nothing visible happens when Felix walks south out through the gate (cells
/// (29, 25) and (30, 25)) without having been inspected; the game marks the
/// inspection done, so that the guards do not stop him on the cells south of
/// it.
void Madra_SkipGateInspection(void) { GameFlag_Set(FLAG_MADRA_GATE_INSPECTED); }

/// Nothing visible happens when Felix walks south out through the gate (cells
/// (29, 25) and (30, 25)) after the Kibombo's raid without having been
/// questioned; the game marks the questioning done, so that the guards do not
/// stop him on the cells south of it.
void Madra_SkipRaidQuestioning(void) {
  GameFlag_Set(FLAG_MADRA_RAID_QUESTIONING_DONE);
}

/// The last of the ladder tops' step triggers that lead down to the
/// catacombs' east ladders; those above it lead to the west ones. No cell of
/// Madra's map has one.
enum { CELL_BEHAVIOR_MADRA_EAST_LADDERS_LAST = 34 };

/// Felix, already beside a ladder top, turns north, steps onto the rungs and
/// climbs down out of sight to the catacombs' east or west ladders, by the
/// ladder top's step trigger. Only the unused routines below call it.
void Madra_ClimbDownLadder(void) {
  s32 next;

  next = MADRA_TO_MADRA_CATACOMBS_LADDERS_EAST;
  if (gFieldState->stepTrigger > CELL_BEHAVIOR_MADRA_EAST_LADDERS_LAST)
    next = MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST;
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 4);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 16);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
  Event_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(next);
  Event_End();
}

/// Unused: Felix steps up to a ladder top from its west side and climbs down;
/// no trigger runs it.
UNUSED void Madra_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Madra_ClimbDownLadder();
}

/// Unused: Felix steps up to a ladder top from its east side and climbs down;
/// no trigger runs it.
UNUSED void Madra_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, -2, -6);
  Actor_WalkByAndWait(ACTOR_FELIX, -14, -8);
  Madra_ClimbDownLadder();
}

/// Unused: Felix turns north on the spot and climbs down; no trigger runs it.
UNUSED void Madra_TurnAndClimbDownLadder(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Madra_ClimbDownLadder();
}

#include "shared/scene_entrances_default.c"

// clang-format off
u32 gSceneExits[] = {
    SCENE_EXITS(Madra),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, MadraRoomsCavernHomes, MADRA_ROOMS_CAVERN_HOMES_FROM_MADRA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_NORTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_NORTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_WEST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, MadraRoomsCavernHomes, MADRA_ROOMS_CAVERN_HOMES_FROM_MADRA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MADRA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_CATACOMBS_LADDERS_EAST, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_EAST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The town until the party returns with Piers (enum MadraActorId).
struct ActorPlacement gMadra_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 392, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 440, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 384, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 274, 0, 510, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 264, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 259, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 248, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 112, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 88, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 272, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, IF_FLAG_SET(FLAG_BRIGGS_CAUGHT), MOVEMENT_TYPE_STAND, 280, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 280, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The town once the party returns with Piers (enum MadraActorId, then
/// MadraOnReturnActorId).
struct ActorPlacement gMadra_PlacementsOnReturn[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 150, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 392, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 440, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 392, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 274, 0, 510, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 296, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 259, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 440, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 272, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 276, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 112, 0, 393, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// A door opening: the one cell animation step of most of the town's doors.
u16 gMadra_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(1, 39, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The door opening of the left cavern home's two doors, from other cells.
u16 gMadra_CavernHomeDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(3, 39, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
// clang-format on

/// The town's doors that open with a cell animation as the party walks through,
/// by the exits they lead by.
struct MapGridCellAnimation gMadra_Doors[] = {
    {gMadra_CavernHomeDoorOpening, 20, 3},
    {gMadra_DoorOpening, 13, 23},
    {gMadra_DoorOpening, 17, 17},
    {gMadra_DoorOpening, 23, 18},
    {gMadra_DoorOpening, 28, 10},
    {gMadra_DoorOpening, 28, 6},
    {gMadra_DoorOpening, 9, 6},
    {gMadra_DoorOpening, 19, 24},
    {gMadra_DoorOpening, 0, 0}, // the jail's doorway has no door
    {gMadra_CavernHomeDoorOpening, 17, 3},
};

/// The town's doors, gate, talk and Mind Read before the Kibombo raid.
struct Trigger gMadra_TriggersBeforeRaid[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, MADRA_TO_MADRA_ROOMS_HOUSES_WEST, CONDITION_ALWAYS,
     MADRA_TO_MADRA_ROOMS_HOUSES_WEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, MADRA_TO_WORLD_MAP, CONDITION_ALWAYS, MADRA_TO_WORLD_MAP},
    {TRIGGER_EXIT, MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, CONDITION_ALWAYS,
     MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_CHAMPAS_HOLE, CONDITION_ALWAYS,
     EVENT(Madra_WorkerBlocksHole)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_INSIDE, FLAG_MADRA_GATE_INSPECTED,
     EVENT(Madra_SkipGateInspection)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_OUTSIDE, FLAG_MADRA_GATE_INSPECTED,
     EVENT(Madra_InspectPartyAtGate)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgMadraTownElderTookOff)},
    {TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS, CONDITION_ALWAYS,
     MSGNO(MsgMadraYouPickedWrongTime)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WAVE, CONDITION_ALWAYS,
     MSGNO(MsgMadraThatBigWaveMovedWhole)},
    {TRIGGER_TALK, ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraLetMeGuessYouWant)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraMaybeWellLearnBriggssWhereabouts)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_AIRS_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgMadraIfICouldWalk)},
    {TRIGGER_TALK, ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
     EVENT(Madra_AskAboutBriggs)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_SUPERCONTINENT, CONDITION_ALWAYS,
     MSGNO(MsgMadraItsLikeAllThreeContinents)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraChampaWereSwarmingOverVillage)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE, CONDITION_ALWAYS,
     MSGNO(MsgMadraThoseChampaCursDugHole)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_PLAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraIDoubtChampaCouldHave)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_BIG_HOLE, CONDITION_ALWAYS,
     MSGNO(MsgMadraIGotSomeoneToShow)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER, CONDITION_ALWAYS,
     MSGNO(MsgMadraIDontCareWhatElder)},
    {TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_PRISONER, CONDITION_ALWAYS,
     MSGNO(MsgMadraWeTookThatUnconsciousGuy)},
    {TRIGGER_TALK, ACTOR_GUARD_ON_HIS_FRIEND, CONDITION_ALWAYS,
     MSGNO(MsgMadraMyFriendWasOnGuard)},
    {TRIGGER_TALK, ACTOR_GUARD_ON_HIS_PARTNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraMyPartnerWasGuardingPrison)},
    {TRIGGER_TALK, ACTOR_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraTheresNoWayThosePirates)},
    {TRIGGER_TALK, ACTOR_SECOND_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraAllOfIndraShifted)},
    {TRIGGER_TALK, ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
     EVENT(Madra_AskComingThroughHole)},
    {TRIGGER_TALK, ACTOR_ICE_SHAFT_WITNESS, CONDITION_ALWAYS,
     EVENT(Madra_AskAboutIceShaft)},
    {TRIGGER_TALK, ACTOR_FRIGHTENED_WITNESS, CONDITION_ALWAYS,
     MSGNO(MsgMadraEvenIfWeTried)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToItemMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANTS_SIBLING, CONDITION_ALWAYS,
     EVENT(Madra_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_ELDER,
     CONDITION_ALWAYS, MSGNO(MsgMadraEveryoneThinksBriggsFled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
     CONDITION_ALWAYS, MSGNO(MsgMadraTownElderIsGettingPretty)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WAVE,
     CONDITION_ALWAYS, MSGNO(MsgMadraIfBigWaveCanMove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraItsBitOfProblem)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraThatChampaInCellLooks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgMadraIveAlwaysWantedToSee)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_BRIGGS_FAN | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_MADRA_BOY_HAS_ASKED,
     EVENT(Madra_MindReadBriggsFan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraWhoWasItWhoTold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SUPERCONTINENT,
     CONDITION_ALWAYS, MSGNO(MsgMadraNowMadraIsPinnedBetween)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraWhenISawThoseChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
     CONDITION_ALWAYS, MSGNO(MsgMadraChampaMustHaveKnownExactly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
     CONDITION_ALWAYS, MSGNO(MsgMadraItsNotLikeChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_BIG_HOLE,
     CONDITION_ALWAYS, MSGNO(MsgMadraImNotSureChampaActually)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
     CONDITION_ALWAYS, MSGNO(MsgMadraIllNeverForgiveThoseDogs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
     CONDITION_ALWAYS, MSGNO(MsgMadraILookedIntoThatMans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_FRIEND,
     CONDITION_ALWAYS, MSGNO(MsgMadraIWonderHowMany)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_PARTNER,
     CONDITION_ALWAYS, MSGNO(MsgMadraIMissMyPartner)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraItsSoWeird)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GATE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgMadraTheseAreRoughTimes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgMadraChampaDugThisHoleWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ICE_SHAFT_WITNESS,
     CONDITION_ALWAYS, MSGNO(MsgMadraItWasAllJustDream)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FRIGHTENED_WITNESS,
     CONDITION_ALWAYS, MSGNO(MsgMadraMySoreBottomTellsMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraItemVendorGotWounded)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraThoseLousyChampaNextTime)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraBriggsAndHisMen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_SIBLING,
     CONDITION_ALWAYS, MSGNO(MsgMadraBriggsAndHisMen)},
    {TRIGGER_TABLE_END},
};

/// The town's doors, gate, talk and Mind Read once the Kibombo have raided
/// Madra.
struct Trigger gMadra_TriggersAfterRaid[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, MADRA_TO_MADRA_ROOMS_HOUSES_WEST, CONDITION_ALWAYS,
     MADRA_TO_MADRA_ROOMS_HOUSES_WEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, MADRA_TO_WORLD_MAP, CONDITION_ALWAYS, MADRA_TO_WORLD_MAP},
    {TRIGGER_EXIT, MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, CONDITION_ALWAYS,
     MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_CHAMPAS_HOLE, CONDITION_ALWAYS,
     EVENT(Madra_WorkerBlocksSealedHole)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_INSIDE,
     FLAG_MADRA_RAID_QUESTIONING_DONE, EVENT(Madra_SkipRaidQuestioning)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_OUTSIDE,
     FLAG_MADRA_RAID_QUESTIONING_DONE, EVENT(Madra_StopPartyAfterRaid)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgMadraWeThinkKibomboCameInto)},
    {TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS, CONDITION_ALWAYS,
     MSGNO(MsgMadraSoKibomboGotIntoMadra)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WAVE, CONDITION_ALWAYS,
     MSGNO(MsgMadraKibomboCameInThroughBroken)},
    {TRIGGER_TALK, ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraIHeardThatElderGot)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraIGuessThatGuy)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_AIRS_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgMadraGondowanSoundsLikeFrighteningPlace)},
    {TRIGGER_TALK, ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
     EVENT(Madra_AskAboutBriggs)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_SUPERCONTINENT, CONDITION_ALWAYS,
     MSGNO(MsgMadraICantBelieveSomeoneStomped)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraNowThatWereSealingOff)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE, CONDITION_ALWAYS,
     MSGNO(MsgMadraThereAreStillPlenty)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_PLAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraBriggsDidntHaveNearly)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_BIG_HOLE, CONDITION_ALWAYS,
     MSGNO(MsgMadraTheseCavesWereMySecret)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER, CONDITION_ALWAYS,
     MSGNO(MsgThatGuyWasn)},
    {TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_PRISONER, CONDITION_ALWAYS,
     MSGNO(MsgOhWeReleased)},
    {TRIGGER_TALK, ACTOR_GUARD_ON_HIS_FRIEND, CONDITION_ALWAYS,
     MSGNO(MsgMadraEveryoneYelledAtMe)},
    {TRIGGER_TALK, ACTOR_GUARD_ON_HIS_PARTNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraYouShouldaSeenMyPartner)},
    {TRIGGER_TALK, ACTOR_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraFirstPiratesRaidOurTown)},
    {TRIGGER_TALK, ACTOR_SECOND_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraMyGuessIsKibomboHave)},
    {TRIGGER_TALK, ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
     EVENT(Madra_AskComingThroughSealedHole)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_ELDER,
     CONDITION_ALWAYS, MSGNO(MsgMadraIfItWerent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
     CONDITION_ALWAYS, MSGNO(MsgMadraIfGondowanIsReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WAVE,
     CONDITION_ALWAYS, MSGNO(MsgMadraItsStrangeItsAlmost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraMayorIsSupposed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraHeToldUsHe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgMadraInGondowanTheyHaveEvil)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraIHeardGuysWhoCaptured)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SUPERCONTINENT,
     CONDITION_ALWAYS, MSGNO(MsgMadraIfTheyWereStrongEnough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraItWasSoConvenientBeing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
     CONDITION_ALWAYS, MSGNO(MsgMadraIWonderWhoDugThose)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
     CONDITION_ALWAYS, MSGNO(MsgMadraWaitIfBriggsOnlyHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_BIG_HOLE,
     CONDITION_ALWAYS, MSGNO(MsgMadraIveNeverSeenAnyMonsters)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
     CONDITION_ALWAYS, MSGNO(MsgDidnTSeem)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
     CONDITION_ALWAYS, MSGNO(MsgWasSoHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_FRIEND,
     CONDITION_ALWAYS, MSGNO(MsgMadraIDidntHaveChance)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_PARTNER,
     CONDITION_ALWAYS, MSGNO(MsgMadraICantBelieveNobodySeems)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraWayThingsHaveBeenGoing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GATE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgMadraIveBeenStanding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgMadraWeSealedUpHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraChampaWeaponsWeFoundBefore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraGroupThatAttackedFromEast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraThatShinIsAlwaysBugging)},
    {TRIGGER_TABLE_END},
};

/// The town's doors, talk and Mind Read once the party returns with Piers, and
/// Karst's visit.
struct Trigger gMadra_TriggersOnReturn[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, MADRA_TO_MADRA_ROOMS_HOUSES_WEST, CONDITION_ALWAYS,
     MADRA_TO_MADRA_ROOMS_HOUSES_WEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, CONDITION_ALWAYS,
     EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, MADRA_TO_WORLD_MAP, CONDITION_ALWAYS, MADRA_TO_WORLD_MAP},
    {TRIGGER_EXIT, MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, CONDITION_ALWAYS,
     MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_PATH_PAST_KARST, FLAG_MADRA_KARST_MET,
     EVENT(Madra_KarstCallsForMenardi)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgMadraIDontThinkMayorEver)},
    {TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS, CONDITION_ALWAYS,
     MSGNO(MsgMadraWeGoToAll)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WAVE, CONDITION_ALWAYS,
     MSGNO(MsgMadraIHeardBoatMayorGot)},
    {TRIGGER_TALK, ACTOR_BOAT_OWNER, FLAG_MADRA_TOLD_OF_JENNA,
     EVENT(Madra_AskAboutJenna)},
    {TRIGGER_TALK, ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraThoseTravelersLookedWorriedAbout)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraIWonderIfLegends)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_AIRS_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgIDonTKnowWhatWill)},
    {TRIGGER_TALK, ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraGroupOfSharpLookingTravelers)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_SUPERCONTINENT, CONDITION_ALWAYS,
     MSGNO(MsgMadraIfYouHaveBigEnough)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraIWasAtMayorsHouse)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE, CONDITION_ALWAYS,
     MSGNO(MsgMadraItsNotThatIDont)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_PLAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraMayorAskedMeToGo)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_BIG_HOLE, CONDITION_ALWAYS,
     MSGNO(MsgMadraItSeemsNoOne)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER, CONDITION_ALWAYS,
     MSGNO(MsgSayThereWas)},
    {TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_PRISONER, CONDITION_ALWAYS,
     MSGNO(MsgMadraIThoughtShinAndOthers)},
    {TRIGGER_TALK, ACTOR_GUARD_ON_HIS_FRIEND, CONDITION_ALWAYS,
     MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, ACTOR_GUARD_ON_HIS_PARTNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraUhhhnnnUhhhnnn)},
    {TRIGGER_TALK, ACTOR_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraMayorFinallyReturnedFromAlhafra)},
    {TRIGGER_TALK, ACTOR_SECOND_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraHeyItsYouGuys)},
    {TRIGGER_TALK, ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgMadraYeahWellWereWorkin)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(Madra_TalkToItemMerchant)},
    {TRIGGER_TALK, ACTOR_KARST, CONDITION_ALWAYS,
     EVENT(Madra_KarstAsksAboutMenardi)},
    {TRIGGER_TALK, ACTOR_MAN_AFRAID_OF_THE_KIBOMBO, CONDITION_ALWAYS,
     EVENT(Madra_AskAfraidOfKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_ELDER,
     CONDITION_ALWAYS, MSGNO(MsgMadraWhyWouldAnyoneBeHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
     CONDITION_ALWAYS, MSGNO(MsgMadraTheyveBeenHavingSoMany)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WAVE,
     CONDITION_ALWAYS, MSGNO(MsgMadraEveryoneGetsUrgeToExplore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
     MSGNO(MsgMadraIWonderWhyThoseTravelers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraWhatWasThatIslandAgain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgMadraPeopleOfMadraKnowOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
     MSGNO(MsgMadraThatIslandThatGroup)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SUPERCONTINENT,
     CONDITION_ALWAYS, MSGNO(MsgMadraHowBigWouldShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraTheseNewBoatsApparentlyUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
     CONDITION_ALWAYS, MSGNO(MsgMadraIfBuildingHasBrokenColumns)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
     CONDITION_ALWAYS, MSGNO(MsgMadraSailorsAreSupposedToUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_BIG_HOLE,
     CONDITION_ALWAYS, MSGNO(MsgMadraOurMayorsDoneSoLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
     CONDITION_ALWAYS, MSGNO(MsgMadraIsTurningWaterToIce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
     CONDITION_ALWAYS, MSGNO(MsgMadraIsAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_FRIEND,
     CONDITION_ALWAYS, MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_PARTNER,
     CONDITION_ALWAYS, MSGNO(MsgMadraIKnewIShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraHeWasGoneSoLong)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GATE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgMadraHowMuchLongerDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraPiratesAndKibomboMustHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraThatNightChampaCame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgMadraShinSeemsTerrifiedOfPiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KARST, CONDITION_ALWAYS,
     EVENT(Madra_MindReadKarst)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AFRAID_OF_THE_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgMadraWereStillRecoveringFromKibombo)},
    {TRIGGER_TABLE_END},
};
