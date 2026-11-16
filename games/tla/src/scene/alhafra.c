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
#include "system/io_write.h"
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

/// How the lookout's scene starts: the leader stepping past him, or reading his
/// mind, which lets the party question him before he runs.
enum AlhafraLookoutScene { LOOKOUT_STEPPED_PAST, LOOKOUT_READ_BY_MIND_READ };

void Alhafra_LookoutSeesMadrasMayor(s32 how);
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

/// The chest Reveal shows among the six gravestones, holding a Psy Crystal.
#define ACTOR_PSY_CRYSTAL_CHEST 64

/// The Alhafrans of gAlhafra_ActorPlacements, who keep their rows, sprites and
/// places to ACTOR_SAILOR_BUILDING_HOUSES once Briggs is caught and once the
/// ship is stolen; what each says changes with the story.
enum AlhafraActorId {
  ACTOR_WEAPON_MERCHANT = ACTOR_FIRST_PLACED,
  ACTOR_ARMOR_MERCHANT,
  /// The soldiers at the mayor's manor: "The mayor is in a meeting..."
  ///@{
  ACTOR_MANOR_GUARD,
  ACTOR_SECOND_MANOR_GUARD,
  ///@}
  /// The man at the back way into the cave under the manor, the hungry boy's
  /// father: "No one is allowed beyond this point."
  ACTOR_HUNGRY_BOYS_FATHER,
  /// "Welcome to Alhafra. This is the largest town in western Osenia..."
  ACTOR_WELCOMING_YOUNG_MAN,
  /// "A massive tidal wave hit us and destroyed every ship in Alhafra."
  ACTOR_WOMAN_ON_THE_LOST_SHIPS,
  /// The fisherman who asks whether Indra moved.
  ACTOR_FISHERMAN,
  /// "Right after my boat got crushed, a group of men came by..."
  ACTOR_BOY_WITH_A_CRUSHED_BOAT,
  /// "Uh... The mayor? Just keep going down this road."
  ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
  /// "Those people that just came through looked totally exhausted."
  ACTOR_OLD_WOMAN_ON_THE_TRAVELERS,
  /// "We've had a lot of strangers coming through here lately"; once Briggs
  /// is caught, "Hey! You there! Sailor boy! Get back to work!"
  ACTOR_WOMAN_SCOLDING_THE_SAILOR,
  /// "Our port was ruined by the tidal wave, so kids aren't allowed there..."
  ACTOR_GIRL_KEPT_FROM_THE_PORT,
  /// "I wonder if my partner has finished up that canoe yet..."
  ACTOR_CANOE_MAKERS_PARTNER,
  /// The canoe maker, too busy to make one for the party.
  ACTOR_CANOE_MAKER,
  /// "That boat at the wharf uses a sail..."
  ACTOR_SAILOR_IN_A_RED_BANDANA,
  /// "Why would the mayor go and sell our boat to a bunch of foreigners?";
  /// once Briggs is caught, "What does a sailor like me know about building
  /// houses, anyway?"
  ACTOR_SAILOR_BUILDING_HOUSES,
  /// A Champa sailor watching the road before Briggs is caught.
  ACTOR_LOOKOUT,
  /// The three puddles of seawater Frost freezes.
  ///@{
  ACTOR_PUDDLE_A,
  ACTOR_PUDDLE_B,
  ACTOR_PUDDLE_C,
  ///@}
  /// Madra's elder, the mayor's father (his sprite, as in the Yampi Desert,
  /// where he calls the mayor his son): "We're here to bring in Briggs the
  /// Champa."
  ACTOR_MADRAS_ELDER,
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

/// The families whose houses are being rebuilt, after the townsfolk once
/// Briggs is caught and again once the ship is stolen.
enum AlhafraOnceBriggsIsCaughtActorId {
  /// "Once they fix the old folks' home, it'll be our turn..."
  ACTOR_YOUNG_MAN_WAITING_AT_THE_INN = ACTOR_SAILOR_BUILDING_HOUSES + 1,
  /// "I was afraid my house would never get repaired..."
  ACTOR_WOMAN_WAITING_AT_THE_INN,
  /// The boy too hungry to move.
  ACTOR_HUNGRY_BOY,
  /// The family on the step where the mother finds five gold coins.
  ///@{
  ACTOR_GOLD_COIN_FINDERS_HUSBAND,
  ACTOR_GOLD_COIN_FINDER,
  ACTOR_GOLD_COIN_FINDERS_CHILD,
  ///@}
  /// "I wonder if there's any more money hidden around here..."
  ACTOR_BOY_HUNTING_FOR_COINS
};

/// The Alhafrans the town's setup moves once the mast is cleared, until the
/// ship is stolen (gAlhafra_ActorPlacementsOnceTheMastIsCleared).
enum AlhafraOnceTheMastIsClearedActorId {
  /// "The whole town is grateful to you..."
  ACTOR_GRATEFUL_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// "I'm just standing guard, in case Briggs decides to..."
  ACTOR_YOUNG_MAN_STANDING_GUARD,
  /// "Our sailboat looks fabulous with its mast repaired!"
  ACTOR_WOMAN_ADMIRING_THE_MAST,
  /// "I wonder where Briggs and his crew went..."
  ACTOR_WOMAN_WONDERING_WHERE_BRIGGS_WENT,
  /// "I heard you made it so we could fix our sailboat!"
  ACTOR_WOMAN_GLAD_OF_THE_SAILBOAT,
  /// "Ever since we heard Briggs broke out of jail..."
  ACTOR_MAN_WARY_OF_BRIGGS,
  /// "I don't know why everyone's so excited about fixing the ship..."
  ACTOR_MAN_UNMOVED_BY_THE_SHIP,
  /// "Oh! It's you! Where have you been?"
  ACTOR_MAN_GLAD_TO_SEE_FELIX,
  /// "With that ship, Alhafra's going to become a big, important town..."
  ACTOR_WOMAN_ON_ALHAFRAS_FUTURE,
  /// "Once that stone was taken care of, it was really quite..."
  ACTOR_WOMAN_ON_THE_STONE,
  /// "If Briggs is smart, he's probably long gone..."
  ACTOR_WOMAN_SURE_BRIGGS_IS_GONE,
  /// "I've been waiting for this day for years!"
  ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY,
  /// "The secret to harnessing the wind's power is building a small..."
  ACTOR_OLD_MAN_ON_SAILS,
  /// "I'm sure it's just a coincidence, but whenever you show up..."
  ACTOR_OLD_MAN_ON_COINCIDENCES,
  /// "The mayor was actually singing and skipping..."
  ACTOR_OLD_WOMAN_ON_THE_SINGING_MAYOR,
  /// "Weird... Have you ever had the feeling that someone just..."
  ACTOR_OLD_WOMAN_FEELING_WATCHED,
  /// "Huge boats carry more, but we need smaller ships to catch the fish..."
  ACTOR_OLD_WOMAN_ON_FISHING_BOATS
};

extern struct ActorPlacement gAlhafra_ActorPlacements[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceBriggsIsCaught[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceTheShipIsStolen[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceTheMastIsCleared[];
extern struct MapGridCellAnimation gAlhafra_Doors[];

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
void Alhafra_OpenDoorAndEnter(struct MapGridCellAnimation *step, s32 countdown,
                              s32 unused) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(step->steps, step->x, step->y);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Walking into one of the town's doors: it opens and the leader goes
/// through to the exit of the same number.
void Alhafra_EnterDoor(s32 index) {
  struct MapGridCellAnimation *table = gAlhafra_Doors;

  Alhafra_OpenDoorAndEnter(&table[index], index, 0);
}

s32 Shop_Run();

/// Alhafra's shops, as Shop_Run numbers them.
enum { SHOP_ALHAFRA_WEAPONS = 7, SHOP_ALHAFRA_ARMOR = 8 };

/// The cell behaviors of the town's step triggers: the lookout drawn behind or
/// in front of the wall as the leader passes, the Madrans arriving (from the
/// first Felix stands in their way and steps aside, from the second he only
/// turns to them), and the lookout running off to warn the crew.
enum AlhafraCellBehavior {
  CELL_BEHAVIOR_LOOKOUT_BEHIND = 18,
  CELL_BEHAVIOR_LOOKOUT_IN_FRONT,
  CELL_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX,
  CELL_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX,
  CELL_BEHAVIOR_LOOKOUT_RUNS_OFF
};

/// Talking to the weapon merchant across his counter opens his shop.
void Alhafra_OpenWeaponShop(s32 keeperId) {
  Shop_Run(SHOP_ALHAFRA_WEAPONS, keeperId);
}

/// Talking to the armor merchant across his counter opens his shop.
void Alhafra_OpenArmorShop(s32 keeperId) {
  Shop_Run(SHOP_ALHAFRA_ARMOR, keeperId);
}

/// The fisherman asks whether the tidal wave really knocked Indra into
/// Osenia; a yes is remembered, and after it he only marvels that it is true.
void Alhafra_AskWhetherIndraMoved(s32 actor) {
  s32 msg;

  if (Event_GetFlag(FLAG_TOLD_INDRA_MOVED) == 0) {
    msg = (s32)&MsgAlhafraIHearTheTidalWave;
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
    Event_SetMessageId((s32)&MsgAlhafraIGuessWhatThoseMen);
    // "I guess what those men told me was true... I'd figured they were..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The canoe maker guesses the party wants to buy a canoe; yes or no, he is too
/// backlogged to make one.
void Alhafra_CanoeMakerBacklogged(s32 actor) {
  s32 msg = (s32)&MsgAlhafraLetMeGuessYoureHere;

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

/// The manor's guard asks whether the party wants to see the mayor, who is in
/// a meeting.
void Alhafra_MayorInAMeeting(s32 actor) {
  s32 msg = (s32)&MsgAlhafraDidYouWantToSee;

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

/// Mind Read on the Champa lookout: he sees the mayor of Madra, and the party
/// can question him before he runs to warn the crew.
void Alhafra_ReadLookoutsMind(void) {
  Alhafra_LookoutSeesMadrasMayor(LOOKOUT_READ_BY_MIND_READ);
}

/// Stepping past the lookout: he sees the mayor of Madra and runs to warn the
/// crew.
void Alhafra_LookoutRunsToWarnTheCrew(void) {
  Alhafra_LookoutSeesMadrasMayor(LOOKOUT_STEPPED_PAST);
}

/// Passing the lookout's wall: he is drawn behind it.
void Alhafra_DrawLookoutBehind(void) {
  FieldActor_SetSpritePriority(ACTOR_LOOKOUT, 2);
}

/// Coming back past it: he is drawn in front of it again.
void Alhafra_DrawLookoutInFront(void) {
  FieldActor_SetSpritePriority(ACTOR_LOOKOUT, 3);
}

s32 FlagBlock_ApplyFlags();
extern s16 gAlhafra_Puddles[];

/// Frost on one of the town's puddles freezes it into a pillar.
void Alhafra_FreezePuddle(s32 unused, s32 actorId) {
  FlagBlock_ApplyFlags((s32)gAlhafra_Puddles);
  Actor_GetById(actorId)->priorityFlags = 0;
}

/// The sailor building houses grumbles, and the woman beside him tells him to
/// get back to work.
void Alhafra_SailorToldToWork(void) {
  Event_SetMessageId((s32)MsgAlhafraWhatDoesASailorLike);
  // "What does a sailor like me know about building houses, anyway?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 10);
  Actor_FaceDirectionById(ACTOR_WOMAN_SCOLDING_THE_SAILOR, -256, 0);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_WOMAN_SCOLDING_THE_SAILOR,
                      EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Hey! You there! Sailor boy! Get back to work!"
  FieldEvent_ShowMessageThenWait(ACTOR_WOMAN_SCOLDING_THE_SAILOR, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_SAILOR_BUILDING_HOUSES, 3);
  FieldEffect_SetAttached(ACTOR_SAILOR_BUILDING_HOUSES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_WOMAN_SCOLDING_THE_SAILOR, FACING_WEST, 0);
  // "Hey! Give me a break! I'm on it!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 0);
}

/// Giving the hungry boy Large Bread: he eats it, jumps up full of energy and
/// talks to the party.
void Alhafra_GiveBreadToHungryBoy(void) {
  GameFlag_Set(FLAG_HUNGRY_BOY_FED);
  PartyInventory_Remove(ITEM_LARGE_BREAD);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafraThanksMister);
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

/// The hungry boy: too hungry to move until fed, then he tells the party of
/// the caves under the mayor's manor if they like adventures.
void Alhafra_TalkToHungryBoy(s32 id) {
  if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED)) {
    gFieldWork->fieldMenu = 1;
    Event_SetMessageId(MsgAlhafraImSoHungryICant);
    // "I'm so hungry I can't even move..."
    Event_ShowMessageAndWait(id, 0);
  } else if (Event_GetFlag(FLAG_BOY_TOLD_OF_CAVE)) {
    Event_SetMessageId(MsgAlhafraImFeelingALotBetter);
    // "I'm feeling a lot better now thanks to you, mister. If you get any..."
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId(MsgAlhafraGeezMisterIDontKnow);
    Event_Wait(10);
    Actor_SetAnimationAndWait(id, 3);
    // "Geez, mister, I don't know how I can repay you..."
    Event_ShowMessageThenWait(id, 0, 10);
    Actor_RunRepeatedMotion(id, 2);
    // "Wait, I've got it! Do you like...adventures?"
    FieldMessage_ShowAtActor(id, 0);
    Event_Wait(10);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
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

/// Stepping by the family on the step: the mother finds five gold coins, and
/// her husband and child cheer.
void Alhafra_FindGoldCoins(void) {
  GameFlag_Set(FLAG_GOLD_COINS_FOUND);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafraWhyLookAtThis);
  FieldCamera_MoveTo(560, CAMERA_KEEP, 496, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GOLD_COIN_FINDER, 2);
  Event_Wait(10);
  // "Why, look at this!"
  FieldEvent_ShowMessageThenWait(ACTOR_GOLD_COIN_FINDER, 0, 10);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_HUSBAND, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BOY_HUNTING_FOR_COINS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_CHILD, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_GOLD_COIN_FINDER, 5);
  Sound_Play(MUS_LEVEL_UP);
  gMainRoutineWork.leaderActorId = ACTOR_GOLD_COIN_FINDER;
  ItemDisplay_ShowItem(0, 3);
  gMainRoutineWork.leaderActorId = ACTOR_FELIX;
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
  FieldActor_Jump(ACTOR_BOY_HUNTING_FOR_COINS, 6, 15);
  FieldActor_Jump(ACTOR_BOY_HUNTING_FOR_COINS, 6, 23);
  // "I'm going to start looking, too!"
  FieldEvent_ShowMessageThenWait(ACTOR_BOY_HUNTING_FOR_COINS, 0, 10);
  Actor_FaceDirection(ACTOR_GOLD_COIN_FINDERS_CHILD, FACING_EAST, 0);
  Actor_SetAnimationAndWait(ACTOR_GOLD_COIN_FINDERS_CHILD, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BOY_HUNTING_FOR_COINS, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_BOY_HUNTING_FOR_COINS, 6);
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
  s32 message = (s32)MsgAlhafraYoureTheOneWhoGave;

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

/// Reveal shows the Psy Crystal chest's hidden spot.
void Alhafra_RevealTreasureSpot(void) {
  TreasureSpot_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, toFIXED(184), toFIXED(552));
}

/// As Reveal fades, the spot hides again.
void Alhafra_HideTreasureSpot(void) {
  TreasureSpot_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, -1, -1);
}

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
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (!GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE)) {
      Actor_GetById(ACTOR_OLD_WOMAN_ON_FISHING_BOATS)->update =
          (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY)->update =
          (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(ACTOR_OLD_MAN_ON_SAILS, 2);
      Actor_SetSpritePriority(ACTOR_MAN_UNMOVED_BY_THE_SHIP, 3);
      Actor_SetSpritePriority(ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, 3);
      Actor_SetSpritePriority(ACTOR_WOMAN_ADMIRING_THE_MAST, 1);
    } else {
      Actor_Get(ACTOR_WOMAN_ON_THE_LOST_SHIPS)->update =
          (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(ACTOR_GIRL_KEPT_FROM_THE_PORT)->update =
          (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(ACTOR_CANOE_MAKER)->update =
          (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
      Actor_SetSpritePriority(ACTOR_WOMAN_SCOLDING_THE_SAILOR, 3);
      Actor_SetSpritePriority(ACTOR_SAILOR_IN_A_RED_BANDANA, 2);
      Actor_SetSpritePriority(ACTOR_SAILOR_BUILDING_HOUSES, 3);
      Actor_SetSpritePriority(ACTOR_HUNGRY_BOY, 2);
      Actor_SetSpritePriority(ACTOR_GOLD_COIN_FINDERS_HUSBAND, 3);
      Actor_SetSpritePriority(ACTOR_GOLD_COIN_FINDER, 1);
      Actor_SetSpritePriority(ACTOR_GOLD_COIN_FINDERS_CHILD, 3);
      Actor_SetSpritePriority(ACTOR_BOY_HUNTING_FOR_COINS, 3);
      Actor_SetSpritePriority(ACTOR_HUNGRY_BOYS_FATHER, 3);
      Actor_SetSpritePriority(ACTOR_MANOR_GUARD, 3);
    }
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_GetById(ACTOR_WOMAN_ON_THE_LOST_SHIPS)->update =
        (void (*)(void))Alhafra_TakeLeadersLayer;
    Actor_GetById(ACTOR_GIRL_KEPT_FROM_THE_PORT)->update =
        (void (*)(void))Alhafra_TakeLeadersLayer;
    Actor_SetSpritePriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
    Actor_SetSpritePriority(ACTOR_WOMAN_SCOLDING_THE_SAILOR, 3);
    Actor_SetSpritePriority(ACTOR_SAILOR_IN_A_RED_BANDANA, 3);
    Actor_SetSpritePriority(ACTOR_SAILOR_BUILDING_HOUSES, 3);
    Actor_SetSpritePriority(ACTOR_CANOE_MAKER, 2);
    Actor_SetSpritePriority(ACTOR_HUNGRY_BOY, 2);
    Actor_SetSpritePriority(ACTOR_GOLD_COIN_FINDERS_HUSBAND, 3);
    Actor_SetSpritePriority(ACTOR_GOLD_COIN_FINDER, 1);
    Actor_SetSpritePriority(ACTOR_GOLD_COIN_FINDERS_CHILD, 3);
    Actor_SetSpritePriority(ACTOR_BOY_HUNTING_FOR_COINS, 3);
    Actor_SetSpritePriority(ACTOR_HUNGRY_BOYS_FATHER, 3);
    Actor_SetMovementType(Actor_GetById(ACTOR_WOMAN_WAITING_AT_THE_INN),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED))
      Actor_SetAnimationById(ACTOR_HUNGRY_BOY, 5);
    if (GameFlag_Get(FLAG_GOLD_COINS_FOUND)) {
      Actor_SetAnimationById(ACTOR_GOLD_COIN_FINDERS_CHILD, 6);
      Actor_SetAnimationById(ACTOR_BOY_HUNTING_FOR_COINS, 6);
    }
    if (GameFlag_Get(FLAG_LET_INTO_CAVE)) {
      Actor_SetPosition(ACTOR_HUNGRY_BOYS_FATHER, 56, 328);
      Actor_FaceDirectionById(ACTOR_HUNGRY_BOYS_FATHER, FACING_EAST, 0);
    }
  } else {
    Actor_SetSpritePriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
    Actor_SetSpritePriority(ACTOR_SAILOR_IN_A_RED_BANDANA, 2);
    Actor_SetSpritePriority(ACTOR_MANOR_GUARD, 3);
    Actor_GetById(ACTOR_SAILOR_BUILDING_HOUSES)->update =
        (void (*)(void))Alhafra_TakeLeadersLayer;
    if (GameFlag_Get(FLAG_ALHAFRA_MADRANS_ARRIVED))
      Actor_SetPosition(ACTOR_WOMAN_ON_THE_LOST_SHIPS, 536, 760);
    if (GameFlag_Get(FLAG_LOOKOUT_LEFT))
      Actor_SetPosition(ACTOR_LOOKOUT, 0, 0);
    FlagBlock_PlaceAllMarkingEdges((s32)&gAlhafra_Puddles);
    Actor_SetMovementType(Actor_GetById(ACTOR_PUDDLE_A),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    Actor_SetMovementType(Actor_GetById(ACTOR_PUDDLE_B),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    Actor_SetMovementType(Actor_GetById(ACTOR_PUDDLE_C),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    u16 *src = (u16 *)&BG_PLTT_COLOR(8, 11);

    Io_Write16(*src, &BG_PLTT_COLOR(7, 11));
    src++;
    Io_Write16(*src, &BG_PLTT_COLOR(7, 12));
    src++;
    Io_Write16(*src, &BG_PLTT_COLOR(7, 13));
    Io_Write16(src[1], &BG_PLTT_COLOR(7, 14));
    MapGrid_CopyCellAttributes(37, 18, 2, 2, 37, 33);
    MapGrid_CopyCellAttributes(37, 18, 2, 3, 39, 31);
    MapGrid_CopyCellAttributes(50, 80, 7, 5, 35, 95);
  }
  return 0;
}

/// An actor's update: it takes the party leader's display layer, so it is drawn
/// over or under what the leader is.
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

/// The mayor and elder of Madra arrive with their soldiers and townsfolk,
/// ask the way to Alhafra's mayor and follow an Alhafran to the manor.
void Alhafra_MadransArrive(s32 cellBehavior) {
  GameFlag_Set(FLAG_ALHAFRA_MADRANS_ARRIVED);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafraFinallyWeMadeIt);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 1);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 1);
  Actor_SetSpeed(ACTOR_WEARY_MADRAN, 1);
  Actor_SetSpeed(ACTOR_PARCHED_MADRAN, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0.5);
  Actor_SetSpeed(ACTOR_WOMAN_ON_THE_LOST_SHIPS, 0.5);
  if (cellBehavior == CELL_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX) {
    Actor_WalkByAndWait(ACTOR_FELIX, 0, -8);
  }
  Actor_SetPosition(ACTOR_FIRST_MADRAN_SOLDIER, 408, 872);
  Actor_SetPosition(ACTOR_SECOND_MADRAN_SOLDIER, 392, 888);
  // "Finally! We made it!"
  Event_ShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
  if (cellBehavior == CELL_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX) {
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
  FieldActor_WatchAndSetScript(ACTOR_FIRST_MADRAN_SOLDIER, ACTOR_MADRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_SECOND_MADRAN_SOLDIER, ACTOR_MADRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                               ACTOR_MADRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_WEARY_MADRAN, ACTOR_MADRAS_MAYOR);
  Actor_WatchAndSetScript(ACTOR_PARCHED_MADRAN, ACTOR_MADRAS_MAYOR);
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
  Actor_SetMovementTypeById(ACTOR_WOMAN_ON_THE_LOST_SHIPS,
                            (s32)gAlhafra_PasserbyWalksOffScript);
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
  FieldActor_StartRepeatedMotion(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
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
  if (cellBehavior == CELL_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX) {
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 2);
    FieldActor_WatchAndSetScript(ACTOR_FIRST_MADRAN_SOLDIER,
                                 ACTOR_MADRAS_ELDER);
    FieldActor_WatchAndSetScript(ACTOR_SECOND_MADRAN_SOLDIER,
                                 ACTOR_MADRAS_ELDER);
    FieldActor_WatchAndSetScript(ACTOR_WEARY_MADRAN, ACTOR_MADRAS_ELDER);
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
    FieldActor_WatchAndSetScript(ACTOR_PARCHED_MADRAN, ACTOR_MADRAS_ELDER);
    Actor_SetMovementTypeById(ACTOR_MADRAS_MAYOR,
                              (s32)gAlhafra_MayorFollowsFromCell21Script);
    Event_Wait(170);
    Actor_SetMovementTypeById(
        ACTOR_SECOND_MADRAN_SOLDIER,
        (s32)gAlhafra_SecondSoldierFollowsFromCell21Script);
    Event_Wait(10);
    Actor_SetMovementTypeById(
        ACTOR_FIRST_MADRAN_SOLDIER,
        (s32)gAlhafra_FirstSoldierFollowsFromCell21Script);
    Actor_SetMovementTypeById(ACTOR_WEARY_MADRAN,
                              (s32)gAlhafra_ElderFollowsFromCell21Script);
    Event_Wait(60);
    Actor_SetMovementTypeById(
        ACTOR_PARCHED_MADRAN,
        (s32)gAlhafra_SecondMadranFollowsFromCell21Script);
  } else {
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 2);
    FieldActor_WatchAndSetScript(ACTOR_MADRAS_MAYOR,
                                 ACTOR_ALHAFRAN_GIVING_DIRECTIONS);
    FieldActor_WatchAndSetScript(ACTOR_FIRST_MADRAN_SOLDIER,
                                 ACTOR_MADRAS_MAYOR);
    FieldActor_WatchAndSetScript(ACTOR_SECOND_MADRAN_SOLDIER,
                                 ACTOR_MADRAS_MAYOR);
    FieldActor_WatchAndSetScript(ACTOR_WEARY_MADRAN, ACTOR_MADRAS_MAYOR);
    Actor_SetMovementTypeById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
                              (s32)gAlhafra_PasserbyLeadsFromCell20Script);
    Event_Wait(60);
    Actor_SetMovementTypeById(ACTOR_MADRAS_ELDER,
                              (s32)gAlhafra_OfficialFollowsFromCell20Script);
    Actor_SetMovementTypeById(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
                              (s32)gAlhafra_FirstMadranFollowsFromCell20Script);
    Event_Wait(70);
    FieldActor_WatchAndSetScript(ACTOR_PARCHED_MADRAN, ACTOR_MADRAS_ELDER);
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
        ACTOR_FIRST_MADRAN_SOLDIER,
        (s32)gAlhafra_FirstSoldierFollowsFromCell20Script);
    Actor_SetMovementTypeById(
        ACTOR_PARCHED_MADRAN,
        (s32)gAlhafra_SecondMadranFollowsFromCell20Script);
    Event_Wait(30);
    Actor_SetMovementTypeById(
        ACTOR_SECOND_MADRAN_SOLDIER,
        (s32)gAlhafra_SecondSoldierFollowsFromCell20Script);
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

/// The Champa lookout recognizes the mayor of Madra and runs to warn Briggs's
/// crew, after answering the party if they read his mind.
void Alhafra_LookoutSeesMadrasMayor(s32 how) {
  Event_SetFlag(FLAG_LOOKOUT_LEFT);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraOooThisAintGood);
  if (how == LOOKOUT_READ_BY_MIND_READ) {
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
  if (how == LOOKOUT_READ_BY_MIND_READ) {
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, -8, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 3);
    FieldEffect_SetAttached(ACTOR_LOOKOUT,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_GetById(ACTOR_LOOKOUT)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWaitById(ACTOR_LOOKOUT, 192, 736);
    Event_Wait(1);
    Actor_GetById(ACTOR_LOOKOUT)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_Wait(10);
    Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
    Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    Event_OpenMessage(ACTOR_LOOKOUT, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
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
    Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_FELIX, 152, 752);
    Event_Wait(1);
    Actor_GetById(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
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
  if (how == LOOKOUT_STEPPED_PAST) {
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
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 664, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 520, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 560, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 648, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 736, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 584, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceBriggsIsCaught[] = {
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 792, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 248, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 520, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 536, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 600, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 504, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 504, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 576, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 616, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 488, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 456, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceTheShipIsStolen[] = {
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 328, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 792, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 248, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 520, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 536, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 440, 0, 800, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 656, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 568, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 488, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 456, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 600, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 544, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 648, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 600, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 624, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 592, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 576, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, (s32)gAlhafra_LookAroundScript, 648, 0, 504, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 608, FACING_EAST, TALK_FACING_PARTY, 0),
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

struct MapGridCellAnimation gAlhafra_Doors[] = {
    {NULL, 0, 0},
    {gAlhafra_DoorEdit, 27, 37},
    {gAlhafra_DoorEdit, 27, 43},
    {NULL, 0, 0},
    {NULL, 0, 0},
    {gAlhafra_DoorEdit, 21, 36},
    {gAlhafra_DoorEdit, 38, 46},
    {gAlhafra_NoDoorEdit, 0, 0},
    {gAlhafra_LeftDoorEdit, 11, 33},
    {NULL, 0, 0},
    {NULL, 0, 0},
    {NULL, 0, 0},
    {gAlhafra_LeftDoorEdit, 5, 9},
    {NULL, 0, 0},
    {gAlhafra_DoorEdit, 16, 9},
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
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOOKOUT_BEHIND, CONDITION_ALWAYS, EVENT(Alhafra_DrawLookoutBehind)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOOKOUT_IN_FRONT, CONDITION_ALWAYS, EVENT(Alhafra_DrawLookoutInFront)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOOKOUT_RUNS_OFF, FLAG_LOOKOUT_LEFT, EVENT(Alhafra_LookoutRunsToWarnTheCrew)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeWereHitByA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingAllMyMostExpensive)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatTidalWaveMayWell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItDoesntMatterHowWell)},
    {TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWelcomeToAlhafraThisIs)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraAMassiveTidalWaveHit)},
    {TRIGGER_TALK, ACTOR_FISHERMAN, CONDITION_ALWAYS, EVENT(Alhafra_AskWhetherIndraMoved)},
    {TRIGGER_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraRightAfterMyBoatGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisGuysProbablyWonderingAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingOurBoatsWasAwful)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIJustCantBelieveA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThoseGuysLookingForA)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraThoseMenDidntLookLike)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraThosePeopleThatJustCame)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCOLDING_THE_SAILOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeveHadALotOf)},
    {TRIGGER_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraOurPortWasRuinedBy)},
    {TRIGGER_TALK, ACTOR_CANOE_MAKERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheOnlyBoatSparedBy)},
    {TRIGGER_TALK, ACTOR_CANOE_MAKER, CONDITION_ALWAYS, EVENT(Alhafra_CanoeMakerBacklogged)},
    {TRIGGER_TALK, ACTOR_SAILOR_IN_A_RED_BANDANA, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatBoatAtTheWharf)},
    {TRIGGER_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyWouldTheMayorGo)},
    {TRIGGER_TALK, ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgAlhafraSilence)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfThisGuysLookingTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhoWouldComeToAlhafra)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCOLDING_THE_SAILOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisTidalWavesActuallyBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhyThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CANOE_MAKERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfMyPartner)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CANOE_MAKER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeCantRaiseTheMast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_IN_A_RED_BANDANA, CONDITION_ALWAYS, MSGNO(MsgAlhafraIStillCantBelieveThose)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraItMayBeNewfangledBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS, EVENT(Alhafra_ReadLookoutsMind)},
    {TRIGGER_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, EVENT(Alhafra_MayorInAMeeting)},
    {TRIGGER_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraListenYouLookLikeAn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHateMyJobI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraMyPartnersAlwaysInSuch)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraNoOneIsAllowedBeyond)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSaidNoOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_SCENE_TEMP_00, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_SCENE_TEMP_01, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_C, FLAG_SCENE_TEMP_02, EVENT(Alhafra_FreezePuddle)},
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
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_STEP, 25, FLAG_GOLD_COINS_FOUND, EVENT(Alhafra_FindGoldCoins)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraItLooksLikeTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraIGuessOurMayorDidnt)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraIHearTheMayorOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraHeSeemsLikeAnHonest)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatBriggsWasTheLeader)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsWasVeryGenerousHe)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatGroupFromMadraWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraGettingABoatForNothing)},
    {TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWereAllGoingToWork)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceThingsAreBackTo)},
    {TRIGGER_TALK, ACTOR_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraSoTheGuysWhoBought)},
    {TRIGGER_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryoneIsTalkingAboutHow)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorIsTheOnly)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfMadraApparently)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCOLDING_THE_SAILOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraINeverThoughtAFine)},
    {TRIGGER_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraICanHelpBuildHouses)},
    {TRIGGER_TALK, ACTOR_CANOE_MAKERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWevePutOffFixingThe)},
    {TRIGGER_TALK, ACTOR_CANOE_MAKER, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceRepairsAreDoneHere)},
    {TRIGGER_TALK, ACTOR_SAILOR_IN_A_RED_BANDANA, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheyToldMeTheydHelp)},
    {TRIGGER_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, EVENT(Alhafra_SailorToldToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsSoNiceOfEveryone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryoneInTownIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowPiratesWhoPay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThosePiratesHaveTheCoolest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraICantBelieveTheyLeft)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfOurMayorThinksIm)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCOLDING_THE_SAILOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowSittingHereFixing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraHelpingPeopleOutFeelsReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CANOE_MAKERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheFactThatMadraTook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CANOE_MAKER, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfTheCrew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_IN_A_RED_BANDANA, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsStrangeTheQuartersDown)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraWorkingOnDeckWithThe)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceTheyFixTheOld)},
    {TRIGGER_TALK, ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWasAfraidMyHouse)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, EVENT(Alhafra_TalkToHungryBoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfWeDontGetOut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowSinceWereRebuilding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, FLAG_HUNGRY_BOY_FED, MSGNO(MsgAlhafraIWishSomeoneWouldJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfIShould)},
    {USE_ITEM_ON_ACTOR(ITEM_LARGE_BREAD), 27, CONDITION_ALWAYS, EVENT(Alhafra_GiveBreadToHungryBoy)},
    {TRIGGER_TALK, ACTOR_GOLD_COIN_FINDERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraAllMyNeighborsAreGoing)},
    {TRIGGER_TALK, ACTOR_GOLD_COIN_FINDER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatAStrokeOfLuck)},
    {TRIGGER_TALK, ACTOR_GOLD_COIN_FINDERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraMyDadLostTheDraw)},
    {TRIGGER_TALK, ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfTheresAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GOLD_COIN_FINDERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingTheLotteryAndBeing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GOLD_COIN_FINDER, CONDITION_ALWAYS, MSGNO(MsgAlhafraIThoughtTheHardPart)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GOLD_COIN_FINDERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraAllBecauseOfMyDads)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIReallyShouldntBePeeking)},
    {TRIGGER_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraAhFelixWelcomeWelcomeListen)},
    {TRIGGER_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraOhFelixGoOnIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfIdKnownBriggsWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorWasReallyImpressed)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOYS_FATHER, FLAG_BOY_TOLD_OF_CAVE, MSGNO(MsgAlhafraNoOneIsAllowedBeyondThis)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, EVENT(Alhafra_FatherLetsPartyToCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, FLAG_LET_INTO_CAVE, MSGNO(MsgAlhafraTheMayorSaidNoOneGoes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraNotThatItMattersThe)},
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
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryonesSayingThatBriggsProbably)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraSupposedlyTheCaveUnderThe)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraSoBriggsStoleMadrasShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowINever)},
    {TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraAlhafraLostItsShipNow)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEvenThoughWeLostThe)},
    {TRIGGER_TALK, ACTOR_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraAfterWeDidAllThat)},
    {TRIGGER_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItWasGreatToSee)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhenTheyFindThatTheir)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsNotLikeTheBoat)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCOLDING_THE_SAILOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheRepairsAreComingAlong)},
    {TRIGGER_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhenIsThisHouseGoing)},
    {TRIGGER_TALK, ACTOR_CANOE_MAKERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgAlhafraItFeltSoGreatWhen)},
    {TRIGGER_TALK, ACTOR_CANOE_MAKER, CONDITION_ALWAYS, MSGNO(MsgAlhafraBackBeforeBriggsDestroyedThe)},
    {TRIGGER_TALK, ACTOR_SAILOR_IN_A_RED_BANDANA, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeFixedTheMastSo)},
    {TRIGGER_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfTheyCaughtAGood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfOnlyWeHadThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHopeTradeDoesntMake)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraItWasMadrasBoatIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSeemedSoHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfMadraSeems)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheresNoPointInWorrying)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCOLDING_THE_SAILOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraWereAllPullingTogetherTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfMomWouldStopMaking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CANOE_MAKERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgAlhafraIKnowItsTimeTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CANOE_MAKER, CONDITION_ALWAYS, MSGNO(MsgAlhafraFirstHeStealsAShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_IN_A_RED_BANDANA, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyWorryAboutBriggsIts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfBriggsHadntStolenThat)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatGuysHouseIsntFixed)},
    {TRIGGER_TALK, ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorsBeenHandingOut)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeHaveToStoreWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeCouldUseSomeHelp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhatWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIveSeenFatherTryingTo)},
    {TRIGGER_TALK, ACTOR_GOLD_COIN_FINDERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraEvenIfWeFixIt)},
    {TRIGGER_TALK, ACTOR_GOLD_COIN_FINDER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithEveryonesHelpWeWere)},
    {TRIGGER_TALK, ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraMomWasHappyToHear)},
    {TRIGGER_TALK, ACTOR_GOLD_COIN_FINDERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraThereAreALotOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GOLD_COIN_FINDERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisIsJustTooMuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GOLD_COIN_FINDER, CONDITION_ALWAYS, MSGNO(MsgAlhafraHesNeverGoingToGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheresNoWayDadCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GOLD_COIN_FINDERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraCarryingTheseThingsAllOver)},
    {TRIGGER_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfAlhafraDoes)},
    {TRIGGER_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouReallyMadeAMess)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorWasSoMad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraOneThingsForSureWere)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsCameThroughHereAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraThePoorGuardWasBlindfolded)},
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
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TRIGGER_TALK, ACTOR_GRATEFUL_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheWholeTownIsGrateful)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_STANDING_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraImJustStandingGuardIn)},
    {TRIGGER_TALK, ACTOR_WOMAN_ADMIRING_THE_MAST, CONDITION_ALWAYS, MSGNO(MsgAlhafraOurSailboatLooksFabulousWith)},
    {TRIGGER_TALK, ACTOR_WOMAN_WONDERING_WHERE_BRIGGS_WENT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderWhereBriggsAnd)},
    {TRIGGER_TALK, ACTOR_WOMAN_GLAD_OF_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHeardYouMadeIt)},
    {TRIGGER_TALK, ACTOR_MAN_WARY_OF_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEverSinceWeHeardBriggs)},
    {TRIGGER_TALK, ACTOR_MAN_UNMOVED_BY_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhyEveryones)},
    {TRIGGER_TALK, ACTOR_MAN_GLAD_TO_SEE_FELIX, CONDITION_ALWAYS, MSGNO(MsgAlhafraOhItsYouWhereHave)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithThatShipAlhafrasGoing)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_STONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceThatStoneWasTaken)},
    {TRIGGER_TALK, ACTOR_WOMAN_SURE_BRIGGS_IS_GONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfBriggsIsSmartHes)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIveBeenWaitingForThis)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_SAILS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheSecretToHarnessingThe)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_COINCIDENCES, CONDITION_ALWAYS, MSGNO(MsgAlhafraImSureItsJustA)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_SINGING_MAYOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorWasActuallySinging)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_FEELING_WATCHED, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeirdHaveYouEverHad)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_FISHING_BOATS, CONDITION_ALWAYS, MSGNO(MsgAlhafraHugeBoatsCarryMoreBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRATEFUL_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraNowThatTheBoatIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_STANDING_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraImSupposedToBeGuarding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ADMIRING_THE_MAST, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhoCaresIfTheBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WONDERING_WHERE_BRIGGS_WENT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyDoIHaveTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_GLAD_OF_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHateToTellFelix)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARY_OF_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIdBetThatBriggsFled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_UNMOVED_BY_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsPaidALotFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GLAD_TO_SEE_FELIX, CONDITION_ALWAYS, MSGNO(MsgAlhafraFelixMustHaveFoundHimself)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, CONDITION_ALWAYS, MSGNO(MsgAlhafraImCertainTheOnlyPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_STONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraNowThatAllTheDebris)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SURE_BRIGGS_IS_GONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSaidIHad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY, CONDITION_ALWAYS, MSGNO(MsgAlhafraImProbablyTooOldFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_SAILS, CONDITION_ALWAYS, MSGNO(MsgAlhafraSmallerShipsUseTheirSails)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_COINCIDENCES, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsBeenSoBusyAround)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_SINGING_MAYOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOnlyHadOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_FEELING_WATCHED, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatIfThePiratesWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_FISHING_BOATS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithoutSmallerShipsHowCan)},
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
