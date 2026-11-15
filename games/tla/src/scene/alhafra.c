/// @file
/// Alhafra, the port town of Osenia across the Yampi Desert, where the tidal
/// wave broke the mast of the town's only sailboat. Its events follow the
/// story: Madra's mayor and his party arrive hunting Briggs
/// (Alhafra_MadransArrive) while a Champa lookout runs to warn the crew
/// (Alhafra_LookoutSeesMadrasMayor); once Briggs is caught, bread for a hungry
/// boy (Alhafra_GiveBreadToHungryBoy) gets his father to let the party into the
/// cave under the mayor's manor (Alhafra_FatherLetsPartyToCave), and the
/// getters pick later tables once the mast is cleared and once the ship has
/// gone (FLAG_ALHAFRA_SHIP_GONE). Frost freezes the puddles of seawater
/// (Alhafra_FreezePuddle), and the doors open as the leader walks into them
/// (Alhafra_EnterDoor).

#include "system/main_routine_work.h"
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
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "shared/tables.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/actor_script.h"
#include "fixed_math.h"

#include "text/msg_ids.h"

void Alhafra_LookoutSeesMadrasMayor(s32 a0);
void Alhafra_TalkToHungryBoy(s32 a);
void Alhafra_TakeLeadersLayer();
s32 Actor_TakeLeaderLayer();
void Alhafra_ReadLookoutsMind(void);
void Alhafra_DrawLookoutBehind(void);
void Alhafra_DrawLookoutInFront(void);
void Alhafra_SailorToldToWork(void);
void Alhafra_RevealTreasureSpot(void);
void Alhafra_HideTreasureSpot(void);
extern u32 gAlhafra_LookAroundScript[];
extern s16 gAlhafra_NoDoorEdit[];
extern s16 gAlhafra_DoorEdit[];
extern s16 gAlhafra_LeftDoorEdit[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The town's own story, which no other scene reads.
#define FLAG_TOLD_INDRA_MOVED 0x8b0 ///< told the fisherman Indra moved
#define FLAG_LOOKOUT_LEFT 0x8b4     ///< the Champa lookout ran to his crew
#define FLAG_HUNGRY_BOY_FED 0x8b5   ///< the boy at the inn ate the bread
#define FLAG_BOY_TOLD_OF_CAVE 0x8b6 ///< he told of the cave under the manor
#define FLAG_GOLD_COINS_FOUND 0x8b7 ///< his mother found five gold coins
#define FLAG_LET_INTO_CAVE 0x8b9    ///< his father let the party pass

/// Actors the events move; the placement tables give each id its own person.
enum AlhafraActorId {
  /// A Champa sailor watching the road before Briggs is caught.
  ACTOR_LOOKOUT = ACTOR_FIRST_PLACED + 17,
  /// The boy too hungry to move, once Briggs is caught.
  ACTOR_HUNGRY_BOY = ACTOR_FIRST_PLACED + 19
};

/// The Alhafrans the events move once Briggs is caught
/// (gAlhafra_ActorPlacementsOnceBriggsIsCaught).
enum AlhafraOnceBriggsIsCaughtActorId {
  /// "Hey! You there! Sailor boy! Get back to work!"
  ACTOR_MAN_SCOLDING_THE_SAILOR = ACTOR_FIRST_PLACED + 11,
  /// "What does a sailor like me know about building houses, anyway?"
  ACTOR_SAILOR_BUILDING_HOUSES = ACTOR_FIRST_PLACED + 16,
  /// The family on the step where the mother finds five gold coins.
  ///@{
  ACTOR_GOLD_COIN_FINDERS_HUSBAND = ACTOR_FIRST_PLACED + 20,
  ACTOR_GOLD_COIN_FINDER,
  ACTOR_GOLD_COIN_FINDERS_CHILD
  ///@}
};

/// Madra's mayor and his party, who arrive hunting Briggs
/// (Alhafra_MadransArrive), on the last rows of gAlhafra_ActorPlacements.
enum AlhafraMadransActorId {
  /// "Uh... The mayor? Just keep going down this road."
  ACTOR_ALHAFRAN_GIVING_DIRECTIONS = ACTOR_FIRST_PLACED + 9,
  /// Madra's elder, the mayor's father (his sprite, as in the Yampi Desert,
  /// where he calls the mayor his son): "We're here to bring in Briggs the
  /// Champa."
  ACTOR_MADRAS_ELDER = ACTOR_FIRST_PLACED + 21,
  /// Madra's mayor (his sprite, as Madra's ACTOR_MAYOR): "Madra's a pretty
  /// big place, but this town is huge!"
  ACTOR_MADRAS_MAYOR,
  /// Madra's soldiers (the drawbridge guard's sprite).
  ///@{
  ACTOR_FIRST_MADRAN_SOLDIER,
  ACTOR_SECOND_MADRAN_SOLDIER,
  ///@}
  /// "We-We're here?" ... "He's coming now..."
  ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
  /// A weary Madran who arrives before the elder: "Haaa hoo hooo... Are
  /// we...here?"
  ACTOR_WEARY_MADRAN,
  /// Who walks in beside the Madran waiting for the elder, in the sprite of
  /// the Yampi Desert's parched traveler.
  ACTOR_PARCHED_MADRAN
};

extern struct ActorPlacement gAlhafra_ActorPlacements[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceBriggsIsCaught[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceTheShipIsStolen[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceTheMastIsCleared[];
extern struct DoorStep gAlhafra_Doors[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE) == 0)
      return gAlhafra_ActorPlacementsOnceTheMastIsCleared;
    return gAlhafra_ActorPlacementsOnceTheShipIsStolen;
  }
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT))
    return gAlhafra_ActorPlacementsOnceBriggsIsCaught;
  return gAlhafra_ActorPlacements;
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void Alhafra_OpenDoorAndEnter(struct DoorStep *step, s32 countdown,
                              s32 unused) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(step->script, step->x, step->z);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Alhafra_EnterDoor(s32 index) {
  struct DoorStep *table = gAlhafra_Doors;

  Alhafra_OpenDoorAndEnter(&table[index], index, 0);
}

s32 Shop_Run();

/// Alhafra's shops, as Shop_Run numbers them.
enum { SHOP_ALHAFRA_WEAPONS = 7, SHOP_ALHAFRA_ARMOR = 8 };

void Alhafra_OpenWeaponShop(s32 a0) { Shop_Run(SHOP_ALHAFRA_WEAPONS, a0); }

void Alhafra_OpenArmorShop(s32 a0) { Shop_Run(SHOP_ALHAFRA_ARMOR, a0); }

void Alhafra_AskWhetherIndraMoved(s32 actor) {
  s32 msg;

  if (Event_GetFlag(FLAG_TOLD_INDRA_MOVED) == 0) {
    msg = (s32)&MsgAlhafra03;
    Event_SetMessageId(msg);
    // "I hear the tidal wave knocked Indra into Osenia. Is that true?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      GameFlag_Set(FLAG_TOLD_INDRA_MOVED);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else {
    Event_SetMessageId((s32)&MsgAlhafra04);
    // "I guess what those men told me was true... I'd figured they were..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Alhafra_CanoeMakerBacklogged(s32 actor) {
  s32 msg = (s32)&MsgAlhafra16;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Alhafra_MayorInAMeeting(s32 actor) {
  s32 msg = (s32)&MsgAlhafra33;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Alhafra_ReadLookoutsMind(void) { Alhafra_LookoutSeesMadrasMayor(1); }

void Alhafra_LookoutRunsToWarnTheCrew(void) {
  Alhafra_LookoutSeesMadrasMayor(0);
}

void Alhafra_DrawLookoutBehind(void) {
  FieldActor_SetSpritePriority(ACTOR_LOOKOUT, 2);
}

void Alhafra_DrawLookoutInFront(void) {
  FieldActor_SetSpritePriority(ACTOR_LOOKOUT, 3);
}

s32 FlagBlock_ApplyFlags();
extern s16 gAlhafra_Puddles[];

void Alhafra_FreezePuddle(s32 a0, s32 a1) {

  FlagBlock_ApplyFlags((s32)gAlhafra_Puddles);
  Actor_GetById(a1)->priorityFlags = 0;
}

void Alhafra_SailorToldToWork(void) {
  Event_SetMessageId((s32)MsgAlhafra50);
  // "What does a sailor like me know about building houses, anyway?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 10);
  Actor_FaceDirectionById(ACTOR_MAN_SCOLDING_THE_SAILOR, -256, 0);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_MAN_SCOLDING_THE_SAILOR,
                      EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Hey! You there! Sailor boy! Get back to work!"
  FieldEvent_ShowMessageThenWait(ACTOR_MAN_SCOLDING_THE_SAILOR, 0, 10);
  FieldActor_StartRepeatedMotion(24, 3);
  FieldEffect_SetAttached(ACTOR_SAILOR_BUILDING_HOUSES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_MAN_SCOLDING_THE_SAILOR, FACING_WEST, 0);
  // "Hey! Give me a break! I'm on it!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 0);
}

void Alhafra_GiveBreadToHungryBoy(void) {
  GameFlag_Set(FLAG_HUNGRY_BOY_FED);
  PartyInventory_Remove(ITEM_LARGE_BREAD);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra69);
  Event_Wait(10);
  Actor_FaceActorById(ACTOR_HUNGRY_BOY, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(10);
  // "Thanks, mister."
  Event_ShowMessageThenWait(ACTOR_HUNGRY_BOY, 0, 10);
  // "The kid ate the bread."
  Event_ShowMessageThenWait(-1, 0, 10);
  Actor_SetAnimationById(ACTOR_HUNGRY_BOY, ANIM_STAND);
  Actor_Jump(ACTOR_HUNGRY_BOY, 6, 15);
  Actor_Jump(ACTOR_HUNGRY_BOY, 6, 23);
  // "Oh, wow, I feel so much better! Look at how much energy I've got now!"
  Event_ShowMessageThenWait(ACTOR_HUNGRY_BOY, 0, 0);
  Alhafra_TalkToHungryBoy(ACTOR_HUNGRY_BOY);
  Actor_FaceDirection(ACTOR_HUNGRY_BOY, FACING_SOUTH, 0);
  Event_End();
}

void Alhafra_TalkToHungryBoy(s32 id) {
  if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED)) {
    gFieldWork->fieldMenu = 1;
    Event_SetMessageId(MsgAlhafra65);
    // "I'm so hungry I can't even move..."
    Event_ShowMessageAndWait(id, 0);
  } else if (Event_GetFlag(FLAG_BOY_TOLD_OF_CAVE)) {
    Event_SetMessageId(MsgAlhafra71);
    // "I'm feeling a lot better now thanks to you, mister. If you get any..."
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId(MsgAlhafra70);
    Event_Wait(10);
    Actor_SetAnimationAndWait(id, 3);
    // "Geez, mister, I don't know how I can repay you..."
    Event_ShowMessageThenWait(id, 0, 10);
    Actor_RunRepeatedMotion(id, 2);
    // "Wait, I've got it! Do you like...adventures?"
    FieldMessage_ShowAtActor(id, 0);
    Event_Wait(10);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      GameFlag_Set(FLAG_BOY_TOLD_OF_CAVE);
      Event_Wait(20);
      // "Then you totally have to go check out the caves under the mayor's..."
      Event_ShowMessageThenWait(id, 0, 10);
      gFieldWork->message++;
    } else {
      Event_Wait(30);
      gFieldWork->message++;
      // "Really? Wow... You're so boring. Well, if you change your mind..."
      Event_ShowMessageThenWait(id, 0, 10);
    }
  }
}

void Alhafra_FindGoldCoins(void) {
  GameFlag_Set(FLAG_GOLD_COINS_FOUND);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra74);
  FieldCamera_MoveTo(560, CAMERA_KEEP, 496, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GOLD_COIN_FINDER, 2);
  Event_Wait(10);
  // "Why, look at this!"
  FieldEvent_ShowMessageThenWait(ACTOR_GOLD_COIN_FINDER, 0, 10);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_HUSBAND, FACING_EAST, 0);
  Actor_FaceDirection(31, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_CHILD, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_GOLD_COIN_FINDER, 5);
  Sound_Play(MUS_LEVEL_UP);
  gMainRoutineWork.leaderActorId = 0x1d;
  ItemDisplay_ShowItem(0, 3);
  gMainRoutineWork.leaderActorId = 4;
  Event_Wait(50);
  FieldActor_Jump(ACTOR_GOLD_COIN_FINDERS_HUSBAND, 6, 15);
  FieldActor_Jump(ACTOR_GOLD_COIN_FINDERS_HUSBAND, 6, 23);
  // "Wow, honey! You found some gold coins!"
  FieldEvent_ShowMessageThenWait(ACTOR_GOLD_COIN_FINDERS_HUSBAND, 0, 10);
  Actor_FaceDirectionById(ACTOR_GOLD_COIN_FINDER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GOLD_COIN_FINDER, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_GOLD_COIN_FINDERS_CHILD,
                      EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Way to go, Mom! So... How much did you find?"
  FieldEvent_ShowMessageThenWait(ACTOR_GOLD_COIN_FINDERS_CHILD, 0, 10);
  Actor_ShowEmoteById(ACTOR_GOLD_COIN_FINDER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDER, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GOLD_COIN_FINDER, 2);
  Event_Wait(10);
  // "Oh! Well, it's not too much... It's only 5 coins."
  FieldEvent_ShowMessageThenWait(ACTOR_GOLD_COIN_FINDER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GOLD_COIN_FINDERS_CHILD, 2);
  Event_Wait(10);
  // "That's great, though! We can use it to buy some food!"
  FieldEvent_ShowMessageThenWait(ACTOR_GOLD_COIN_FINDERS_CHILD, 0, 10);
  Actor_FaceDirectionById(ACTOR_GOLD_COIN_FINDER, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GOLD_COIN_FINDER, ANIM_NOD);
  Event_Wait(20);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  // "I'm going to start looking, too!"
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_CHILD, FACING_EAST, 0);
  Actor_SetAnimationAndWait(ACTOR_GOLD_COIN_FINDERS_CHILD, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(31, FACING_SOUTH, 0);
  Actor_SetAnimation(31, 6);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_CHILD, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_GOLD_COIN_FINDERS_CHILD, 6);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_HUSBAND, FACING_NORTH, 0);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The hungry boy's father, by the way into the cave under the mayor's manor:
/// once the boy is fed he asks whether the party wants to see the cave, and a
/// yes has him step aside for good.
void Alhafra_FatherLetsPartyToCave(s32 actor) {
  s32 message = (s32)MsgAlhafra92;

  if (Event_GetFlag(FLAG_LET_INTO_CAVE) != 0) {
    Event_SetMessageId(message + 4);
    FieldMessage_ShowAtActor(actor, 0);
  } else {
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_Wait(20);
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      GameFlag_Set(FLAG_LET_INTO_CAVE);
      Event_Wait(20);
      Event_SetMessageId(message + 2);
      Event_ShowMessage(actor, 0);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 344);
      Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
      Event_Wait(10);
      Actor_SetSpeed(actor, 1);
      Actor_WalkToAndWait(actor, 56, 328);
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
      Actor_FaceDirectionById(actor, FACING_SOUTHEAST, 0);
      Event_Wait(20);
      Actor_SetAnimationAndWait(actor, 3);
      Event_Wait(10);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

void Alhafra_RevealTreasureSpot(void) {
  TreasureSpot_SetPosition(0x40, 0xb80000, 0x2280000);
}

void Alhafra_HideTreasureSpot(void) { TreasureSpot_SetPosition(0x40, -1, -1); }

extern struct Trigger gAlhafra_Triggers[];
extern struct Trigger gAlhafra_TriggersOnceBriggsIsCaught[];
extern struct Trigger gAlhafra_TriggersOnceTheShipIsStolen[];
extern struct Trigger gAlhafra_TriggersOnceTheMastIsCleared[];

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE) == 0)
      return gAlhafra_TriggersOnceTheMastIsCleared;
    return gAlhafra_TriggersOnceTheShipIsStolen;
  }
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT))
    return gAlhafra_TriggersOnceBriggsIsCaught;
  return gAlhafra_Triggers;
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x100;
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (!GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE)) {
      Actor_GetById(24)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(19)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(20, 2);
      Actor_SetSpritePriority(14, 3);
      Actor_SetSpritePriority(16, 3);
      Actor_SetSpritePriority(10, 1);
    } else {
      Actor_Get(14)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(20)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(22)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(13, 2);
      Actor_SetSpritePriority(19, 3);
      Actor_SetSpritePriority(23, 2);
      Actor_SetSpritePriority(24, 3);
      Actor_SetSpritePriority(27, 2);
      Actor_SetSpritePriority(28, 3);
      Actor_SetSpritePriority(29, 1);
      Actor_SetSpritePriority(30, 3);
      Actor_SetSpritePriority(31, 3);
      Actor_SetSpritePriority(12, 3);
      Actor_SetSpritePriority(10, 3);
    }
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_GetById(14)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
    Actor_GetById(20)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
    Actor_SetSpritePriority(13, 2);
    Actor_SetSpritePriority(19, 3);
    Actor_SetSpritePriority(23, 3);
    Actor_SetSpritePriority(24, 3);
    Actor_SetSpritePriority(22, 2);
    Actor_SetSpritePriority(27, 2);
    Actor_SetSpritePriority(28, 3);
    Actor_SetSpritePriority(29, 1);
    Actor_SetSpritePriority(30, 3);
    Actor_SetSpritePriority(31, 3);
    Actor_SetSpritePriority(12, 3);
    Actor_SetMovementType(Actor_GetById(26), 9);
    if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED))
      Actor_SetAnimationById(27, 5);
    if (GameFlag_Get(FLAG_GOLD_COINS_FOUND)) {
      Actor_SetAnimationById(30, 6);
      Actor_SetAnimationById(31, 6);
    }
    if (GameFlag_Get(FLAG_LET_INTO_CAVE)) {
      Actor_SetPosition(12, 56, 328);
      Actor_FaceDirectionById(12, FACING_EAST, 0);
    }
  } else {
    Actor_SetSpritePriority(13, 2);
    Actor_SetSpritePriority(23, 2);
    Actor_SetSpritePriority(10, 3);
    Actor_GetById(24)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
    if (GameFlag_Get(FLAG_ALHAFRA_MADRANS_ARRIVED))
      Actor_SetPosition(14, 536, 760);
    if (GameFlag_Get(FLAG_LOOKOUT_LEFT))
      Actor_SetPosition(25, 0, 0);
    FlagBlock_PlaceAllMarkingEdges((s32)&gAlhafra_Puddles);
    Actor_SetMovementType(Actor_GetById(26), 9);
    Actor_SetMovementType(Actor_GetById(27), 9);
    Actor_SetMovementType(Actor_GetById(28), 9);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    u16 *src = (u16 *)&BG_PLTT_COLOR(8, 11);

    u32 c;

    /* FAKEMATCH(dead): the four copies in a once-only block. */
    do {
      c = *src;
      BG_PLTT_COLOR(7, 11) = c;
      src++;
      c = *src;
      BG_PLTT_COLOR(7, 12) = c;
      src++;
      c = *src;
      BG_PLTT_COLOR(7, 13) = c;
      {
        u32 last = src[1];

        BG_PLTT_COLOR(7, 14) = last;
      }
    } while (0);
    MapGrid_CopyCellAttributes(37, 18, 2, 2, 37, 33);
    MapGrid_CopyCellAttributes(37, 18, 2, 3, 39, 31);
    MapGrid_CopyCellAttributes(50, 80, 7, 5, 35, 95);
  }
  return 0;
}

void Alhafra_TakeLeadersLayer(void) { Actor_TakeLeaderLayer(); }

extern s32 gAlhafra_OfficialStepsBackScript[];
extern u32 gAlhafra_PasserbyWalksOffScript[];
extern u32 gAlhafra_PasserbyLeadsFromCell21Script[];
extern u32 gAlhafra_OfficialFollowsFromCell21Script[];
extern u32 gAlhafra_FirstMadranFollowsFromCell21Script[];
extern u32 gAlhafra_MayorFollowsFromCell21Script[];
extern u32 gAlhafra_SecondSoldierFollowsFromCell21Script[];
extern u32 gAlhafra_FirstSoldierFollowsFromCell21Script[];
extern u32 gAlhafra_ElderFollowsFromCell21Script[];
extern u32 gAlhafra_SecondMadranFollowsFromCell21Script[];
extern s32 gAlhafra_PasserbyLeadsFromCell20Script[];
extern u32 gAlhafra_OfficialFollowsFromCell20Script[];
extern u32 gAlhafra_FirstMadranFollowsFromCell20Script[];
extern u32 gAlhafra_MayorFollowsFromCell20Script[];
extern u32 gAlhafra_ElderFollowsFromCell20Script[];
extern u32 gAlhafra_FirstSoldierFollowsFromCell20Script[];
extern s32 gAlhafra_SecondMadranFollowsFromCell20Script[];
extern u32 gAlhafra_SecondSoldierFollowsFromCell20Script[];

void Alhafra_MadransArrive(s32 a0) {
  GameFlag_Set(FLAG_ALHAFRA_MADRANS_ARRIVED);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra10);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 1);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 1);
  Actor_SetSpeed(ACTOR_WEARY_MADRAN, 1);
  Actor_SetSpeed(ACTOR_PARCHED_MADRAN, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0.5);
  Actor_SetSpeed(14, 0.5);
  if (a0 == 0x14) {
    Actor_WalkByAndWait(ACTOR_FELIX, 0, -8);
  }
  Actor_SetPosition(ACTOR_FIRST_MADRAN_SOLDIER, 408, 872);
  Actor_SetPosition(ACTOR_SECOND_MADRAN_SOLDIER, 392, 888);
  // "Finally! We made it!"
  Event_ShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
  if (a0 == 0x14) {
    Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  } else {
    Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  }
  Event_Wait(20);
  FieldCamera_MoveTo(400, CAMERA_KEEP, 808, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkBy(ACTOR_FIRST_MADRAN_SOLDIER, 0, -96);
  Actor_WalkByAndWait(ACTOR_SECOND_MADRAN_SOLDIER, 0, -96);
  Actor_SetAnimationById(ACTOR_FIRST_MADRAN_SOLDIER, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_SetPosition(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 392, 872);
  Actor_SetPosition(ACTOR_PARCHED_MADRAN, 408, 872);
  // "We-We're here?"
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_PARCHED_MADRAN, 0, -48);
  Actor_WalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, -48);
  Actor_WaitForMoveById(ACTOR_PARCHED_MADRAN);
  Actor_FaceDirectionById(ACTOR_PARCHED_MADRAN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  // "Where's the Elder?"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                      FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_NORTH, 0);
  Event_Wait(20);
  // "He's coming now..."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_FIRST_MADRAN_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_MADRAN_SOLDIER, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkBy(ACTOR_PARCHED_MADRAN, 32, -48);
  Actor_WalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, -48);
  Actor_WaitForMoveById(ACTOR_PARCHED_MADRAN);
  Actor_FaceDirection(ACTOR_PARCHED_MADRAN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_SOUTH, 0);
  Actor_SetSpritePriority(ACTOR_PARCHED_MADRAN, 3);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_WEARY_MADRAN, 400, 872);
  // "Haaa hoo hooo... Are we...here? I feel like I just walked 500 miles."
  FieldEvent_ShowMessageThenWait(ACTOR_WEARY_MADRAN, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_WEARY_MADRAN, 0, -32);
  Event_Wait(1);
  Actor_SetAnimationById(ACTOR_WEARY_MADRAN, 5);
  Event_Wait(90);
  Actor_RunRepeatedMotion(ACTOR_WEARY_MADRAN, 2);
  Event_Wait(20);
  // "I'm fine... I feel like I could walk 500 more!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEARY_MADRAN, 0, 10);
  Actor_WalkByAndWaitById(ACTOR_WEARY_MADRAN, 0, -16);
  Event_Wait(1);
  Actor_SetAnimationById(ACTOR_WEARY_MADRAN, 5);
  Event_Wait(60);
  Actor_WalkByAndWait(ACTOR_WEARY_MADRAN, 8, -32);
  Actor_FaceDirection(ACTOR_WEARY_MADRAN, FACING_SOUTH, 0);
  Event_Wait(1);
  Actor_SetAnimationById(ACTOR_WEARY_MADRAN, 5);
  Event_Wait(30);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 392, 872);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 408, 872);
  Actor_WalkBy(ACTOR_MADRAS_MAYOR, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, 0, -32);
  Actor_SetAnimationById(ACTOR_MADRAS_MAYOR, ANIM_STAND);
  Event_Wait(1);
  Actor_SetAnimation(ACTOR_MADRAS_ELDER, 5);
  FieldActor_WatchAndSetScript(ACTOR_FIRST_MADRAN_SOLDIER, 30);
  FieldActor_WatchAndSetScript(ACTOR_SECOND_MADRAN_SOLDIER, 30);
  FieldActor_WatchAndSetScript(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 30);
  FieldActor_WatchAndSetScript(ACTOR_WEARY_MADRAN, 30);
  Actor_WatchAndSetScript(ACTOR_PARCHED_MADRAN, 30);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(10);
  FieldActor_Jump(ACTOR_MADRAS_MAYOR, 6, 15);
  FieldActor_Jump(ACTOR_MADRAS_MAYOR, 6, 23);
  Actor_SetAnimation(ACTOR_WEARY_MADRAN, ANIM_STAND);
  // "So...is this Alhafra?"
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetMovementTypeById(ACTOR_MADRAS_ELDER,
                            (s32)gAlhafra_OfficialStepsBackScript);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 0, -32);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, -32, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "Madra's a pretty big place, but this town is huge!"
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_SHAKE_HEAD);
  // "We didn't come to Alhafra to shop, Mister Mayor."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(20);
  // "We're here to bring in Briggs the Champa."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  Actor_SetAnimation(ACTOR_MADRAS_ELDER, 5);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 64, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHEAST, 0);
  Event_Wait(20);
  // "I know that!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_MAYOR, 0,
                                 10);
  Actor_SetAnimationById(ACTOR_MADRAS_ELDER, ANIM_STAND);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  // "Then we should go straight to speak with Alhafra's mayor."
  Event_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  Actor_Stop(ACTOR_FIRST_MADRAN_SOLDIER);
  Actor_Stop(ACTOR_SECOND_MADRAN_SOLDIER);
  Actor_Stop(ACTOR_MADRAN_WAITING_FOR_THE_ELDER);
  Actor_Stop(ACTOR_WEARY_MADRAN);
  Actor_Stop(ACTOR_PARCHED_MADRAN);
  Actor_SetAnimationById(ACTOR_MADRAS_ELDER, 5);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_SetMovementTypeById(14, (s32)gAlhafra_PasserbyWalksOffScript);
  FieldActor_Jump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 15);
  FieldActor_Jump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 23);
  // "We're in no shape to go present ourselves to the mayor right now!"
  Event_ShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_SECOND_MADRAN_SOLDIER, FACING_EAST_SOUTHEAST,
                          0);
  Event_Wait(20);
  // "Look, Briggs is obviously not here, and besides, we already know..."
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_MADRAN_SOLDIER, 0, 10);
  Actor_SetAnimationById(ACTOR_MADRAS_ELDER, ANIM_STAND);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_WEST, 0);
  Event_Wait(20);
  // "Silence!"
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  EventCamera_FollowActor(ACTOR_MADRAS_ELDER, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkToAndWait(ACTOR_MADRAS_ELDER, 488, 808);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WEARY_MADRAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARCHED_MADRAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_SHAKE_HEAD);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(648, CAMERA_KEEP, 536, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  // "It seems Alhafra has felt the effects of the tidal wave as well..."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 808, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, 36);
  Actor_WalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, 0);
  Actor_WalkToAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 504, 808);
  Actor_FaceDirectionById(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                          FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  FieldActor_StartRepeatedMotion(33, 3);
  FieldEffect_SetAttached(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, ANIM_NOD);
  Event_Wait(10);
  // "It looks like they suffered a good deal of damage..."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_ELDER, FACING_EAST, 0);
  Event_Wait(20);
  // "That would explain why Alhafra doesn't seem to have any boats for..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_ShowEmoteById(ACTOR_WEARY_MADRAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  // "You know, just looking around, I get the impression they only had..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEARY_MADRAN, 0, 10);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Event_Wait(10);
  // "So if you really needed a boat, the only way to get one would be to..."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "...Which means the pirates must still be here, trying to get a boat!"
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
  Event_Wait(30);
  FieldActor_Jump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 15);
  FieldActor_Jump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 23);
  // "So, you think we've actually caught up with Briggs?"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_MADRAN_SOLDIER, ANIM_NOD);
  Event_Wait(10);
  // "No doubt about it!"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_MADRAN_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTHEAST, 0);
  // "That's why I want to meet the mayor of Alhafra before the Champa..."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(10);
  // "Oh yeah... I get it now. In that case, let's go see Alhafra's mayor..."
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetPosition(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 408, 696);
  Actor_WalkToAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 408, 760);
  Actor_WalkToAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 448, 792);
  Actor_WalkToAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 448, 808);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 8, 0);
  // "Um... Say, excuse me..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_MAYOR, 0,
                                 10);
  Actor_ShowEmoteById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, FACING_WEST, 0);
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Where would we find the mayor of Alhafra?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_MAYOR, 0,
                                 10);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 2);
  Event_Wait(10);
  gFieldWork->fastForward = 0;
  // "Uh... The mayor?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 0.5);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 0.5);
  Actor_SetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 0.5);
  Actor_SetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 0.5);
  Actor_SetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0.5);
  Actor_SetSpeed(ACTOR_WEARY_MADRAN, 0.5);
  Actor_SetSpeed(ACTOR_PARCHED_MADRAN, 0.5);
  Actor_SetSpeed(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0.5);
  EventCamera_FollowActor(ACTOR_MADRAS_MAYOR, 1);
  FieldCamera_WaitForArrival();
  if (a0 == 0x15) {
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 2);
    FieldActor_WatchAndSetScript(ACTOR_FIRST_MADRAN_SOLDIER, 29);
    FieldActor_WatchAndSetScript(ACTOR_SECOND_MADRAN_SOLDIER, 29);
    FieldActor_WatchAndSetScript(ACTOR_WEARY_MADRAN, 29);
    Actor_SetMovementTypeById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
                              (s32)gAlhafra_PasserbyLeadsFromCell21Script);
    Actor_SetMovementTypeById(ACTOR_MADRAS_ELDER,
                              (s32)gAlhafra_OfficialFollowsFromCell21Script);
    Actor_SetMovementTypeById(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                              (s32)gAlhafra_FirstMadranFollowsFromCell21Script);
    Event_Wait(80);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 3);
    FieldActor_SetSpritePriority(ACTOR_FIRST_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_SECOND_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_WEARY_MADRAN, 3);
    FieldActor_SetSpritePriority(ACTOR_PARCHED_MADRAN, 3);
    FieldActor_SetSpritePriority(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 3);
    FieldActor_WatchAndSetScript(ACTOR_PARCHED_MADRAN, 29);
    Actor_SetMovementTypeById(ACTOR_MADRAS_MAYOR,
                              (s32)gAlhafra_MayorFollowsFromCell21Script);
    Event_Wait(170);
    Actor_SetMovementTypeById(
        32, (s32)gAlhafra_SecondSoldierFollowsFromCell21Script);
    Event_Wait(10);
    Actor_SetMovementTypeById(
        31, (s32)gAlhafra_FirstSoldierFollowsFromCell21Script);
    Actor_SetMovementTypeById(ACTOR_WEARY_MADRAN,
                              (s32)gAlhafra_ElderFollowsFromCell21Script);
    Event_Wait(60);
    Actor_SetMovementTypeById(
        35, (s32)gAlhafra_SecondMadranFollowsFromCell21Script);
  } else {
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 2);
    FieldActor_WatchAndSetScript(ACTOR_MADRAS_MAYOR, 17);
    FieldActor_WatchAndSetScript(ACTOR_FIRST_MADRAN_SOLDIER, 30);
    FieldActor_WatchAndSetScript(ACTOR_SECOND_MADRAN_SOLDIER, 30);
    FieldActor_WatchAndSetScript(ACTOR_WEARY_MADRAN, 30);
    Actor_SetMovementTypeById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
                              (s32)gAlhafra_PasserbyLeadsFromCell20Script);
    Event_Wait(60);
    Actor_SetMovementTypeById(ACTOR_MADRAS_ELDER,
                              (s32)gAlhafra_OfficialFollowsFromCell20Script);
    Actor_SetMovementTypeById(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                              (s32)gAlhafra_FirstMadranFollowsFromCell20Script);
    Event_Wait(70);
    FieldActor_WatchAndSetScript(ACTOR_PARCHED_MADRAN, 29);
    Actor_SetMovementTypeById(ACTOR_MADRAS_MAYOR,
                              (s32)gAlhafra_MayorFollowsFromCell20Script);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 3);
    FieldActor_SetSpritePriority(ACTOR_FIRST_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_SECOND_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_WEARY_MADRAN, 3);
    FieldActor_SetSpritePriority(ACTOR_PARCHED_MADRAN, 3);
    Actor_SetSpritePriority(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 3);
    Event_Wait(120);
    Actor_SetMovementTypeById(ACTOR_WEARY_MADRAN,
                              (s32)gAlhafra_ElderFollowsFromCell20Script);
    Event_Wait(40);
    Actor_SetMovementTypeById(
        31, (s32)gAlhafra_FirstSoldierFollowsFromCell20Script);
    Actor_SetMovementTypeById(
        35, (s32)gAlhafra_SecondMadranFollowsFromCell20Script);
    Event_Wait(30);
    Actor_SetMovementTypeById(
        32, (s32)gAlhafra_SecondSoldierFollowsFromCell20Script);
  }
  Event_Wait(300);
  Actor_WaitForScriptEndById(ACTOR_WEARY_MADRAN);
  Event_Wait(50);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 664, 1);
  FieldCamera_WaitForArrival();
  // "Just keep going down this road. It's the last house. You can't miss it."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0, 10);
  Actor_FaceDirectionById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
                          FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, -16, 0);
  Actor_FaceDirectionById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, FACING_EAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_WEARY_MADRAN, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_MADRAS_MAYOR, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MADRAS_ELDER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FIRST_MADRAN_SOLDIER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SECOND_MADRAN_SOLDIER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PARCHED_MADRAN, 3);
  Actor_SetAnimationAndWait(ACTOR_WEARY_MADRAN, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_WEARY_MADRAN, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 1.9);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 1.9);
  Actor_SetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 1.9);
  Actor_SetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 1.9);
  Actor_SetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 1.9);
  Actor_SetSpeed(ACTOR_WEARY_MADRAN, 1.9);
  Actor_SetSpeed(ACTOR_PARCHED_MADRAN, 1.9);
  Actor_WalkBy(ACTOR_MADRAS_ELDER, 0, -150);
  Actor_WalkBy(ACTOR_MADRAS_MAYOR, 0, -150);
  Actor_WalkBy(ACTOR_FIRST_MADRAN_SOLDIER, 0, -150);
  Actor_WalkBy(ACTOR_SECOND_MADRAN_SOLDIER, 0, -150);
  Actor_WalkBy(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, -150);
  Actor_WalkBy(ACTOR_PARCHED_MADRAN, 0, -150);
  Actor_WalkByAndWait(ACTOR_WEARY_MADRAN, 0, -150);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 0, 0);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_MADRAN_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_MADRAN_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 0);
  Actor_SetPosition(ACTOR_WEARY_MADRAN, 0, 0);
  Actor_SetPosition(ACTOR_PARCHED_MADRAN, 0, 0);
  Event_Wait(15);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

void Alhafra_LookoutSeesMadrasMayor(s32 a0) {

  Event_SetFlag(FLAG_LOOKOUT_LEFT);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafra28);
  if (a0 == 1) {
    FieldEvent_Wait(10);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 168, 736);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  }
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Ooo... This ain't good!"
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LOOKOUT, ANIM_NOD);
  FieldEvent_Wait(10);
  // "That guy looked like the mayor of Madra..."
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LOOKOUT, ANIM_SHAKE_HEAD);
  // "He must have come here looking for Briggs."
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 2);
  FieldEvent_Wait(20);
  // "I'd better go tell the others!"
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
  if (a0 != 1) {
  } else {
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, -8, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 3);
    FieldEffect_SetAttached(ACTOR_LOOKOUT,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_GetById(ACTOR_LOOKOUT)->steeringFlags &= 0xfe;
    Actor_WalkToAndWaitById(ACTOR_LOOKOUT, 192, 736);
    Event_Wait(1);
    Actor_GetById(ACTOR_LOOKOUT)->steeringFlags |= 1;
    Event_Wait(10);
    Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
    Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    Event_OpenMessage(ACTOR_LOOKOUT, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(30);
      Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 2);
      Event_Wait(20);
      Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
      gFieldWork->message = (gFieldWork->message + 1);
    } else {
      Event_Wait(30);
      gFieldWork->message = (gFieldWork->message + 1);
      Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
    }
    Actor_GetById(ACTOR_FELIX)->steeringFlags &= 0xfe;
    Actor_WalkToAndWait(ACTOR_FELIX, 152, 752);
    Event_Wait(1);
    Actor_GetById(ACTOR_FELIX)->steeringFlags |= 1;
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_LOOKOUT, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirectionById(ACTOR_LOOKOUT, FACING_NORTHEAST, 0);
    Event_Wait(70);
    Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
    Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
  }
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 184, 728);
  Actor_WalkByAndWait(ACTOR_LOOKOUT, 0, -110);
  if (a0 == 0) {
    Event_Wait(50);
    Actor_SetPosition(ACTOR_LOOKOUT, 264, 648);
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, 0, 32);
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, 150, 0);
  }
  Actor_SetPosition(ACTOR_LOOKOUT, 0, 0);
  Event_End();
}

// clang-format off
s16 gAlhafra_Puddles[] = {
    26, 512, 27, 513, 28, 514, -1, 0,
};

u32 gAlhafra_LookAroundScript[] = {
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(60),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Alhafra),
    SCENE_EXIT_UNLESS(1, AlhafraInteriors, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AlhafraInteriors, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AlhafraInteriors, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AlhafraInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AlhafraInteriors, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AlhafraJail, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, WorldMap, 20, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, EasternAlhafra, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AlhafranCave, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AlhafraManor, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAlhafra_ActorPlacements[] = {
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 664, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 520, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 560, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(215, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(224, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(225, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 648, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 736, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 584, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceBriggsIsCaught[] = {
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 792, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 248, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 520, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 536, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(215, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 600, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(224, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 504, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(225, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 504, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 576, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 616, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 488, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 456, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceTheShipIsStolen[] = {
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 328, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 792, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 248, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 520, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 536, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(215, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 440, 0, 800, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(224, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(225, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 656, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 568, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 488, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 456, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 600, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 544, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 648, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 600, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 624, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 592, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 576, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, (s32)gAlhafra_LookAroundScript, 648, 0, 504, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 608, FACING_EAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

s16 gAlhafra_NoDoorEdit[] = {
    -1,
};

s16 gAlhafra_DoorEdit[] = {
    63, 2, 1, 2, 6, -1,
};

s16 gAlhafra_LeftDoorEdit[] = {
    62, 2, 1, 2, 6, -1, 0,
};

struct DoorStep gAlhafra_Doors[] = {
    {0, 0, 0},
    {(s32)gAlhafra_DoorEdit, 27, 37},
    {(s32)gAlhafra_DoorEdit, 27, 43},
    {0, 0, 0},
    {0, 0, 0},
    {(s32)gAlhafra_DoorEdit, 21, 36},
    {(s32)gAlhafra_DoorEdit, 38, 46},
    {(s32)gAlhafra_NoDoorEdit, 0, 0},
    {(s32)gAlhafra_LeftDoorEdit, 11, 33},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {(s32)gAlhafra_LeftDoorEdit, 5, 9},
    {0, 0, 0},
    {(s32)gAlhafra_DoorEdit, 16, 9},
};

struct Trigger gAlhafra_Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafra180))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_STEP, 18, CONDITION_ALWAYS, EVENT(Alhafra_DrawLookoutBehind)},
    {TRIGGER_STEP, 19, CONDITION_ALWAYS, EVENT(Alhafra_DrawLookoutInFront)},
    {TRIGGER_STEP, 20, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, 21, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, 22, FLAG_LOOKOUT_LEFT, EVENT(Alhafra_LookoutRunsToWarnTheCrew)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra29)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra30)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra31)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra32)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra01)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra02)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Alhafra_AskWhetherIndraMoved)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra09)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra11)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra12)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra13)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra14)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra15)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Alhafra_CanoeMakerBacklogged)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra17)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra18)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra19)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25 | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS, EVENT(Alhafra_ReadLookoutsMind)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Alhafra_MayorInAMeeting)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra36)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra38)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 26, FLAG_SCENE_TEMP_00, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 27, FLAG_SCENE_TEMP_01, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 28, FLAG_SCENE_TEMP_02, EVENT(Alhafra_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafra_TriggersOnceBriggsIsCaught[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafra180))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_STEP, 25, FLAG_GOLD_COINS_FOUND, EVENT(Alhafra_FindGoldCoins)},
    {TRIGGER_TALK, 18, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafra94)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafra95)},
    {TRIGGER_TALK, 17, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafra96)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafra97)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra82)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra83)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra85)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra39)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra40)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra41)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra42)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra43)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra44)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra45)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra46)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra47)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra48)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra49)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Alhafra_SailorToldToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra51)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra52)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra53)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra54)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra55)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra56)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra57)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra58)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra60)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra61)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra62)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra63)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra64)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, EVENT(Alhafra_TalkToHungryBoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra66)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra67)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, FLAG_HUNGRY_BOY_FED, MSGNO(MsgAlhafra68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgAlhafra72)},
    {USE_ITEM_ON_ACTOR(ITEM_LARGE_BREAD), 27, CONDITION_ALWAYS, EVENT(Alhafra_GiveBreadToHungryBoy)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra73)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra75)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra77)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra76)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra78)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra79)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra81)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra80)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra86)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra87)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra88)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra89)},
    {TRIGGER_TALK, 12, FLAG_BOY_TOLD_OF_CAVE, MSGNO(MsgAlhafra90)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Alhafra_FatherLetsPartyToCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_LET_INTO_CAVE, MSGNO(MsgAlhafra91)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra93)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafra_TriggersOnceTheShipIsStolen[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafra180))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra170)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra171)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra172)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra173)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra132)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra133)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra134)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra135)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra136)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra137)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra138)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra139)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra140)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra141)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra142)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra143)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra144)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra145)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra146)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra147)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra148)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra155)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra149)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra150)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra151)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra152)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra153)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra154)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra156)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra157)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgAlhafra158)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra159)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra160)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgAlhafra161)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra162)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra163)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra164)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra165)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra166)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra167)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra168)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra169)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra174)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra175)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra176)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra177)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra178)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra179)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafra_TriggersOnceTheMastIsCleared[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafra180))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra98)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra99)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra100)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra101)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra102)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra103)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra104)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra105)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra106)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra107)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra108)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra109)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra110)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra111)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra112)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra113)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra114)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra115)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra116)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra117)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra118)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra119)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra120)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra121)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra122)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra123)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra125)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra126)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra127)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra128)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra129)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra130)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra131)},
    {TRIGGER_TABLE_END},
};

s32 gAlhafra_OfficialStepsBackScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, -8),
    SC_SYNC,
    SC_MOVE_BY(32, 0, -32),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_ANIMATION(5),
    SC_END,
};

u32 gAlhafra_PasserbyWalksOffScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(552, 0, 808),
    SC_SYNC,
    SC_MOVE(536, 0, 760),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gAlhafra_PasserbyLeadsFromCell21Script[] = {                                                               
      39,        2, 4,  0x1b80000, 0,   0x3200000, 1,  4,         0x1980000, 0, 0x2f80000,     1,  4,      
      0x1980000, 0, 0x2f80000, 1,  4,   0x1980000, 0,  0x2a80000, 1,  4, 0xf80000,     0,  0x2a80000,     
      1,         4, 0xf80000, 0,  0x2780000, 1,  39, 1,         22, 3, 0xd000, 17,         
  };

u32 gAlhafra_MayorFollowsFromCell21Script[] = {                                                               
      39, 2,  4, 0x1a80000, 0, 0x3180000, 1,        4, 0x1980000, 0, 0x2f80000, 1, 4,  0x1980000, 0,      0x2a80000, 1,  
      4,  0xf80000, 0, 0x2a80000, 1, 4,  0xf80000, 0, 0x2980000, 1, 39, 1, 22, 3,  0xd000, 17,     
  };

u32 gAlhafra_OfficialFollowsFromCell21Script[] = {                                                               
      39,        2, 4,  0x1b80000, 0,   0x3280000, 1,  4,         0x1b80000, 0, 0x3200000,     1,  4,      
      0x1980000, 0, 0x2f80000, 1,  4,   0x1980000, 0,  0x2a80000, 1,  4, 0x1080000,     0,  0x2a80000,     
      1,         4, 0x1080000, 0,  0x2980000, 1,  39, 1,         22, 3, 0xd000, 17,         
  };

u32 gAlhafra_FirstMadranFollowsFromCell21Script[] = {                                                               
      39, 2,  4, 0x1b80000, 0, 0x3280000, 1,        4, 0x1b80000, 0, 0x3200000, 1, 4,  0x1980000, 0,      0x2f80000, 1,  
      4,  0x1980000, 0, 0x2a80000, 1, 4,  0xf80000, 0, 0x2a80000, 1, 39, 1, 22, 3,  0xd000, 17,     
  };

u32 gAlhafra_FirstSoldierFollowsFromCell21Script[] = {
    SC_ANIMATION(2),
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(264, 0, 680),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_ElderFollowsFromCell21Script[] = {                                                               
      39, 2,  4, 0x1980000, 0, 0x2f80000, 1,        4, 0x1980000, 0, 0x2a80000, 1, 4,  0x1180000, 0,      0x2a80000, 1,  
      4,  0x1180000, 0, 0x2b80000, 1, 4,  0xf80000, 0, 0x2b80000, 1, 39, 1, 22, 3,  0xd000, 17,     
  };

u32 gAlhafra_SecondSoldierFollowsFromCell21Script[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(280, 0, 680),
    SC_SYNC,
    SC_MOVE(280, 0, 696),
    SC_SYNC,
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_SecondMadranFollowsFromCell21Script[] = {                                                               
      39,        2, 4,  0x1980000, 0,   0x2f80000, 1,  4,         0x1980000, 0, 0x2f80000,     1,  4,      
      0x1980000, 0, 0x2a80000, 1,  4,   0x1180000, 0,  0x2a80000, 1,  4, 0x1180000,     0,  0x2b80000,     
      1,         4, 0x1000000, 0,  0x2c80000, 1,  39, 1,         22, 3, 0xd000, 17,         
  };

s32 gAlhafra_PasserbyLeadsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(-16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(-16, 0, 16),
    SC_SYNC,
    SC_MOVE(248, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 632),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_MayorFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 664),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_OfficialFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(264, 0, 808),
    SC_SYNC,
    SC_MOVE(264, 0, 664),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_FirstMadranFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 680),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_ElderFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 808),
    SC_SYNC,
    SC_MOVE(248, 0, 696),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_SecondSoldierFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(264, 0, 808),
    SC_SYNC,
    SC_MOVE(264, 0, 680),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_FirstSoldierFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(264, 0, 808),
    SC_SYNC,
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

s32 gAlhafra_SecondMadranFollowsFromCell20Script[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-20, 0, 32),
    SC_SYNC,
    SC_MOVE(328, 0, 808),
    SC_SYNC,
    SC_MOVE(256, 0, 808),
    SC_SYNC,
    SC_MOVE(256, 0, 712),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};
// clang-format on
