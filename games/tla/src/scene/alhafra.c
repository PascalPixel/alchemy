/// @file
/// Alhafra, the port town of Osenia north of the Yampi Desert, where the
/// tidal wave wrecked every ship but one sailboat, whose mast it broke.
///
/// As Felix comes into town, the mayor and elder of Madra and their soldiers
/// arrive behind him, hunting Briggs, and ask the way to Alhafra's mayor
/// (Alhafra_MadransArrive); one of Briggs's pirates, watching the road by a
/// wall, sees the mayor of Madra and runs off to warn his crew
/// (Alhafra_PirateSeesMadrasMayor). Once Briggs is caught, the hungry boy
/// too weak to move eats the party's Large Bread (Alhafra_GiveBreadToHungryBoy)
/// and his father, at the back way into the cave by the mayor's home, lets
/// the party pass (Alhafra_FatherLetsPartyToCave). Frost freezes the puddles
/// of seawater, Reveal finds the Psy Crystal chest among the gravestones,
/// and the townsfolk change once the mast is cleared and again once Briggs
/// has stolen the ship (FLAG_ALHAFRA_SHIP_GONE).

#include "system/main_routine_state.h"
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
#include "field/field_sequence.h"
#include "fixed_math.h"

#include "text/msg_ids.h"

/// How the pirate's scene starts: the leader stepping past him, or reading his
/// mind, which lets the party question him before he runs.
enum AlhafraPirateScene { PIRATE_STEPPED_PAST, PIRATE_READ_BY_MIND_READ };

void Alhafra_PirateSeesMadrasMayor(s32 how);
void Alhafra_TalkToHungryBoy(s32 id);
void Alhafra_TakeLeadersLayer(void);
s32 Actor_TakeLeaderLayer();
void Alhafra_ReadPiratesMind(void);
void Alhafra_DrawPirateBehind(void);
void Alhafra_DrawPirateInFront(void);
void Alhafra_SailorToldToWork(void);
void Alhafra_RevealTreasureSpot(void);
void Alhafra_HideTreasureSpot(void);
extern u32 gAlhafra_LookAroundScript[];
extern u16 gAlhafra_NoDoor[];
extern u16 gAlhafra_DoorOpening[];
extern u16 gAlhafra_SecondDoorOpening[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The town's own story, which no other scene reads.
#define FLAG_TOLD_INDRA_MOVED 0x8b0 ///< told the man asking that Indra moved
#define FLAG_PIRATE_LEFT 0x8b4      ///< the pirate ran to warn his crew
#define FLAG_HUNGRY_BOY_FED 0x8b5   ///< the boy at the inn ate the bread
#define FLAG_BOY_TOLD_OF_CAVE 0x8b6 ///< he told of the cave under the manor
#define FLAG_GOLD_COINS_FOUND 0x8b7 ///< his mother found five gold coins
#define FLAG_LET_INTO_CAVE 0x8b9    ///< his father let the party pass

/// Set while Frost holds each of the three puddles frozen into a pillar.
#define FLAG_PUDDLE_A_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_PUDDLE_B_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_PUDDLE_C_FROZEN FLAG_SCENE_TEMP_02

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
  /// "I hear the tidal wave knocked Indra into Osenia. Is that true?"
  ACTOR_MAN_ASKING_ABOUT_INDRA,
  /// "Right after my boat got crushed, a group of men came by..."
  ACTOR_BOY_WITH_A_CRUSHED_BOAT,
  /// "Uh... The mayor? Just keep going down this road."
  ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
  /// "Those people that just came through looked totally exhausted."
  ACTOR_OLD_WOMAN_ON_THE_TRAVELERS,
  /// "We've had a lot of strangers coming through here lately"; once Briggs
  /// is caught, "Hey! You there! Sailor boy! Get back to work!"
  ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK,
  /// "Our port was ruined by the tidal wave, so kids aren't allowed there..."
  ACTOR_GIRL_KEPT_FROM_THE_PORT,
  /// "I wonder if my partner has finished up that canoe yet..."
  ACTOR_PARTNER_WAITING_FOR_THE_CANOE,
  /// "Let me guess... You're here because you want to buy a canoe, right?"
  ACTOR_MAN_FINISHING_A_CANOE,
  /// "That boat at the wharf uses a sail..."
  ACTOR_SAILOR_ON_THE_SAILBOAT,
  /// "Why would the mayor go and sell our boat to a bunch of foreigners?";
  /// once Briggs is caught, "What does a sailor like me know about building
  /// houses, anyway?"
  ACTOR_SAILOR_BUILDING_HOUSES,
  /// One of Briggs's pirates, watching the road before Briggs is caught.
  ACTOR_PIRATE,
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
  /// A bald, bearded Madran who walks in beside the one waiting for the elder
  /// and never speaks.
  ACTOR_SILENT_MADRAN
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
  /// The family on the step where the mother finds five gold coins: "What a
  /// stroke of luck!"
  ///@{
  ACTOR_LUCKY_MOTHERS_HUSBAND,
  ACTOR_LUCKY_MOTHER,
  ACTOR_LUCKY_MOTHERS_CHILD,
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
  ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP,
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

/// A door of the town opens as Felix walks into it, he walks
/// through, and the screen wipes to the room behind it (AlhafraRooms, the
/// jail, the cave or the manor, by the exit given).
void Alhafra_OpenDoorAndEnter(struct MapGridCellAnimation *step, s32 exit,
                              s32 unused) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(step->steps, step->x, step->y);
  Actor_GetById(gMainRoutineState.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix walks north into one of the town's doors: the houses' and the inn's,
/// the jail's, the back way into the cave by the mayor's home, or the
/// manor's. The door opens, he walks in, and the screen wipes to the room
/// inside (Alhafra_OpenDoorAndEnter).
///
/// Each door's cell behaviour is the number of the exit it leads by, and of
/// its row of gAlhafra_Doors.
void Alhafra_EnterDoor(s32 index) {
  struct MapGridCellAnimation *table = gAlhafra_Doors;

  Alhafra_OpenDoorAndEnter(&table[index], index, 0);
}

s32 Shop_Run();

/// Alhafra's shops, as Shop_Run numbers them.
enum { SHOP_ALHAFRA_WEAPONS = 7, SHOP_ALHAFRA_ARMOR = 8 };

/// The cell behaviors of the town's step triggers: the pirate drawn behind or
/// in front of the wall as the leader passes, the Madrans arriving (from the
/// first Felix stands in their way and steps aside, from the second he only
/// turns to them), and the pirate running off to warn the crew.
enum AlhafraCellBehavior {
  CELL_BEHAVIOR_PIRATE_BEHIND = 18,
  CELL_BEHAVIOR_PIRATE_IN_FRONT,
  CELL_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX,
  CELL_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX,
  CELL_BEHAVIOR_PIRATE_RUNS_OFF
};

/// Felix talks to the weapon merchant across his counter, and the shop
/// opens.
void Alhafra_OpenWeaponShop(s32 keeperId) {
  Shop_Run(SHOP_ALHAFRA_WEAPONS, keeperId);
}

/// Felix talks to the armor merchant across his counter, and the shop
/// opens.
void Alhafra_OpenArmorShop(s32 keeperId) {
  Shop_Run(SHOP_ALHAFRA_ARMOR, keeperId);
}

/// A townsman asks whether the tidal wave really knocked Indra into
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

/// The man finishing a canoe guesses the party wants to buy one; yes or no, he
/// is too backlogged with preorders to make it.
void Alhafra_BackloggedWithCanoes(s32 actor) {
  s32 msg = (s32)&MsgAlhafraLetMeGuessYoureHere;

  Event_SetMessageId(msg);
  // "Let me guess... You're here because you want to buy a canoe, right?"
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    // Yes: "Sorry, but I'm totally backlogged with preorders..."
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    // No: "That's good, because I've got a ton of work to do already..."
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

/// The manor's guard asks whether the party wants to see the mayor, who is in
/// a meeting.
void Alhafra_MayorInAMeeting(s32 actor) {
  s32 msg = (s32)&MsgAlhafraDidYouWantToSee;

  Event_SetMessageId(msg);
  // "Did you want to see the mayor?"
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    // Yes: "Sorry, but he's meeting with some visitors from Madra right now..."
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    // No: "The mayor is very open to meeting visitors, but he's in an..."
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

/// Mind Read on the pirate: he sees the mayor of Madra, and the party
/// can question him before he runs to warn the crew.
void Alhafra_ReadPiratesMind(void) {
  Alhafra_PirateSeesMadrasMayor(PIRATE_READ_BY_MIND_READ);
}

/// Stepping past the pirate: he sees the mayor of Madra and runs to warn the
/// crew.
void Alhafra_PirateRunsToWarnTheCrew(void) {
  Alhafra_PirateSeesMadrasMayor(PIRATE_STEPPED_PAST);
}

/// Nothing visible happens as Felix walks past the wall where Briggs's pirate
/// keeps watch (cells (10, 44) to (11, 45)); the game sets the pirate to be
/// drawn behind the wall from where Felix now stands.
void Alhafra_DrawPirateBehind(void) {
  FieldActor_SetSpritePriority(ACTOR_PIRATE, 2);
}

/// Nothing visible happens as Felix walks back past the pirate's wall, south
/// of it (cells (12, 46) and (12, 47)); the game sets the pirate to be drawn
/// in front of the wall again.
void Alhafra_DrawPirateInFront(void) {
  FieldActor_SetSpritePriority(ACTOR_PIRATE, 3);
}

s32 FlagBlock_ApplyFlags();
extern s16 gAlhafra_Puddles[];

/// Felix casts Frost on one of the three puddles of seawater the tidal wave
/// left in town, and it freezes into a pillar of ice.
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
  // She turns to the sailor, just north of east.
  Actor_FaceDirectionById(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK,
                          FACING_EAST - TRIG_TURN / 256, 0);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK,
                      EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Hey! You there! Sailor boy! Get back to work!"
  FieldEvent_ShowMessageThenWait(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_SAILOR_BUILDING_HOUSES, 3);
  FieldEffect_SetAttached(ACTOR_SAILOR_BUILDING_HOUSES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, FACING_WEST, 0);
  // "Hey! Give me a break! I'm on it!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 0);
}

/// Giving the hungry boy Large Bread: he eats it, jumps up full of energy and
/// talks to the party.
void Alhafra_GiveBreadToHungryBoy(void) {
  GameFlag_Set(FLAG_HUNGRY_BOY_FED);
  PartyInventory_Remove(ITEM_LARGE_BREAD);
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgAlhafraThanksMister);
  Event_Wait(10);
  Actor_FaceActorById(ACTOR_HUNGRY_BOY, gMainRoutineState.leaderActorId, 0);
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
    gFieldState->fieldMenu = 1;
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
      gFieldState->message++;
    } else {
      Event_Wait(30);
      gFieldState->message++;
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
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgAlhafraWhyLookAtThis);
  FieldCamera_MoveTo(560, CAMERA_KEEP, 496, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_LUCKY_MOTHER, 2);
  Event_Wait(10);
  // "Why, look at this!"
  FieldEvent_ShowMessageThenWait(ACTOR_LUCKY_MOTHER, 0, 10);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHERS_HUSBAND, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BOY_HUNTING_FOR_COINS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHERS_CHILD, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_LUCKY_MOTHER, 5);
  Sound_Play(MUS_LEVEL_UP);
  gMainRoutineState.leaderActorId = ACTOR_LUCKY_MOTHER;
  ItemDisplay_ShowItem(0, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
  gMainRoutineState.leaderActorId = ACTOR_FELIX;
  Event_Wait(50);
  FieldActor_Jump(ACTOR_LUCKY_MOTHERS_HUSBAND, 6, 15);
  FieldActor_Jump(ACTOR_LUCKY_MOTHERS_HUSBAND, 6, 23);
  // "Wow, honey! You found some gold coins!"
  FieldEvent_ShowMessageThenWait(ACTOR_LUCKY_MOTHERS_HUSBAND, 0, 10);
  Actor_FaceDirectionById(ACTOR_LUCKY_MOTHER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUCKY_MOTHER, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_LUCKY_MOTHERS_CHILD, EMOTE_IN_FRONT | EMOTE_SMILE,
                      30);
  // "Way to go, Mom! So... How much did you find?"
  FieldEvent_ShowMessageThenWait(ACTOR_LUCKY_MOTHERS_CHILD, 0, 10);
  Actor_ShowEmoteById(ACTOR_LUCKY_MOTHER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHER, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_LUCKY_MOTHER, 2);
  Event_Wait(10);
  // "Oh! Well, it's not too much... It's only 5 coins."
  FieldEvent_ShowMessageThenWait(ACTOR_LUCKY_MOTHER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LUCKY_MOTHERS_CHILD, 2);
  Event_Wait(10);
  // "That's great, though! We can use it to buy some food!"
  FieldEvent_ShowMessageThenWait(ACTOR_LUCKY_MOTHERS_CHILD, 0, 10);
  Actor_FaceDirectionById(ACTOR_LUCKY_MOTHER, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUCKY_MOTHER, ANIM_NOD);
  Event_Wait(20);
  FieldActor_Jump(ACTOR_BOY_HUNTING_FOR_COINS, 6, 15);
  FieldActor_Jump(ACTOR_BOY_HUNTING_FOR_COINS, 6, 23);
  // "I'm going to start looking, too!"
  FieldEvent_ShowMessageThenWait(ACTOR_BOY_HUNTING_FOR_COINS, 0, 10);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHERS_CHILD, FACING_EAST, 0);
  Actor_SetAnimationAndWait(ACTOR_LUCKY_MOTHERS_CHILD, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BOY_HUNTING_FOR_COINS, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_BOY_HUNTING_FOR_COINS, 6);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHERS_CHILD, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_LUCKY_MOTHERS_CHILD, 6);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_LUCKY_MOTHERS_HUSBAND, FACING_NORTH, 0);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
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
    // "I've heard that the deeper you go into the cave, the more dangerous..."
    FieldMessage_ShowAtActor(actor, 0);
  } else {
    Event_SetMessageId(message);
    // "You're the one who gave my son some food, aren't you? Did you want to
    // go see the cave?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_Wait(20);
      Event_SetMessageId(message + 1);
      // No: "If you were up for a little spelunking, I'd be glad to let you..."
      Event_ShowMessageAndWait(actor, 0);
    } else {
      GameFlag_Set(FLAG_LET_INTO_CAVE);
      Event_Wait(20);
      Event_SetMessageId(message + 2);
      // Yes: "I'm only letting you pass because you did good by my son..."
      Event_ShowMessage(actor, 0);
      Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 72, 344);
      Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
      Event_Wait(10);
      Actor_SetSpeed(actor, 1);
      Actor_WalkToAndWait(actor, 56, 328);
      Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 0);
      Actor_FaceDirectionById(actor, FACING_SOUTHEAST, 0);
      Event_Wait(20);
      Actor_SetAnimationAndWait(actor, 3);
      Event_Wait(10);
      // "I'm only letting you by so you can explore a little. Make sure you
      // don't lay a hand on any of the mayor's treasure."
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

/// Felix casts Reveal in town: in the grey world Reveal shows, a red
/// treasure chest stands in the middle of the six gravestones by the
/// cliff, the hidden Psy Crystal chest, which can be opened while it
/// lasts.
void Alhafra_RevealTreasureSpot(void) {
  TreasureSpot_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, toFIXED(184), toFIXED(552));
}

/// Nothing visible: as Reveal fades, the Psy Crystal chest's spot is hidden
/// again.
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
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (!GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE)) {
      Actor_GetById(ACTOR_OLD_WOMAN_ON_FISHING_BOATS)->update =
          (ObjectUpdate)Alhafra_TakeLeadersLayer;
      Actor_GetById(ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY)->update =
          (ObjectUpdate)Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(ACTOR_OLD_MAN_ON_SAILS, 2);
      Actor_SetSpritePriority(ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP, 3);
      Actor_SetSpritePriority(ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, 3);
      Actor_SetSpritePriority(ACTOR_WOMAN_ADMIRING_THE_MAST, 1);
    } else {
      Actor_Get(ACTOR_WOMAN_ON_THE_LOST_SHIPS)->update =
          (ObjectUpdate)Alhafra_TakeLeadersLayer;
      Actor_GetById(ACTOR_GIRL_KEPT_FROM_THE_PORT)->update =
          (ObjectUpdate)Alhafra_TakeLeadersLayer;
      Actor_GetById(ACTOR_MAN_FINISHING_A_CANOE)->update =
          (ObjectUpdate)Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
      Actor_SetSpritePriority(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, 3);
      Actor_SetSpritePriority(ACTOR_SAILOR_ON_THE_SAILBOAT, 2);
      Actor_SetSpritePriority(ACTOR_SAILOR_BUILDING_HOUSES, 3);
      Actor_SetSpritePriority(ACTOR_HUNGRY_BOY, 2);
      Actor_SetSpritePriority(ACTOR_LUCKY_MOTHERS_HUSBAND, 3);
      Actor_SetSpritePriority(ACTOR_LUCKY_MOTHER, 1);
      Actor_SetSpritePriority(ACTOR_LUCKY_MOTHERS_CHILD, 3);
      Actor_SetSpritePriority(ACTOR_BOY_HUNTING_FOR_COINS, 3);
      Actor_SetSpritePriority(ACTOR_HUNGRY_BOYS_FATHER, 3);
      Actor_SetSpritePriority(ACTOR_MANOR_GUARD, 3);
    }
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_GetById(ACTOR_WOMAN_ON_THE_LOST_SHIPS)->update =
        (ObjectUpdate)Alhafra_TakeLeadersLayer;
    Actor_GetById(ACTOR_GIRL_KEPT_FROM_THE_PORT)->update =
        (ObjectUpdate)Alhafra_TakeLeadersLayer;
    Actor_SetSpritePriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
    Actor_SetSpritePriority(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, 3);
    Actor_SetSpritePriority(ACTOR_SAILOR_ON_THE_SAILBOAT, 3);
    Actor_SetSpritePriority(ACTOR_SAILOR_BUILDING_HOUSES, 3);
    Actor_SetSpritePriority(ACTOR_MAN_FINISHING_A_CANOE, 2);
    Actor_SetSpritePriority(ACTOR_HUNGRY_BOY, 2);
    Actor_SetSpritePriority(ACTOR_LUCKY_MOTHERS_HUSBAND, 3);
    Actor_SetSpritePriority(ACTOR_LUCKY_MOTHER, 1);
    Actor_SetSpritePriority(ACTOR_LUCKY_MOTHERS_CHILD, 3);
    Actor_SetSpritePriority(ACTOR_BOY_HUNTING_FOR_COINS, 3);
    Actor_SetSpritePriority(ACTOR_HUNGRY_BOYS_FATHER, 3);
    Actor_SetMovementType(Actor_GetById(ACTOR_WOMAN_WAITING_AT_THE_INN),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED))
      Actor_SetAnimationById(ACTOR_HUNGRY_BOY, 5);
    if (GameFlag_Get(FLAG_GOLD_COINS_FOUND)) {
      Actor_SetAnimationById(ACTOR_LUCKY_MOTHERS_CHILD, 6);
      Actor_SetAnimationById(ACTOR_BOY_HUNTING_FOR_COINS, 6);
    }
    if (GameFlag_Get(FLAG_LET_INTO_CAVE)) {
      Actor_SetPosition(ACTOR_HUNGRY_BOYS_FATHER, 56, 328);
      Actor_FaceDirectionById(ACTOR_HUNGRY_BOYS_FATHER, FACING_EAST, 0);
    }
  } else {
    Actor_SetSpritePriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
    Actor_SetSpritePriority(ACTOR_SAILOR_ON_THE_SAILBOAT, 2);
    Actor_SetSpritePriority(ACTOR_MANOR_GUARD, 3);
    Actor_GetById(ACTOR_SAILOR_BUILDING_HOUSES)->update =
        (ObjectUpdate)Alhafra_TakeLeadersLayer;
    if (GameFlag_Get(FLAG_ALHAFRA_MADRANS_ARRIVED))
      Actor_SetPosition(ACTOR_WOMAN_ON_THE_LOST_SHIPS, 536, 760);
    if (GameFlag_Get(FLAG_PIRATE_LEFT))
      Actor_SetPosition(ACTOR_PIRATE, 0, 0);
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

/// The pool of seawater the tidal wave left by the east road: its four colours
/// in their palette, and the palette holding the dry ground's.
enum {
  SEAWATER_POOL_PALETTE = 7,
  DRY_GROUND_PALETTE = 8,
  SEAWATER_POOL_COLOR = 11
};

/// Once Briggs is caught the pool has dried up: its colours take the dry
/// ground's.
s32 Scene_PrepareMap(void) {
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    u16 *src = (u16 *)&BG_PLTT_COLOR(DRY_GROUND_PALETTE, SEAWATER_POOL_COLOR);

    Io_Write16(*src,
               &BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR));
    src++;
    Io_Write16(*src,
               &BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR + 1));
    src++;
    Io_Write16(*src,
               &BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR + 2));
    Io_Write16(src[1],
               &BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR + 3));
    MapGrid_CopyCellAttributes(37, 18, 2, 2, 37, 33);
    MapGrid_CopyCellAttributes(37, 18, 2, 3, 39, 31);
    MapGrid_CopyCellAttributes(50, 80, 7, 5, 35, 95);
  }
  return 0;
}

/// An actor's update: it takes the party leader's display layer, so it is drawn
/// over or under what the leader is.
void Alhafra_TakeLeadersLayer(void) { Actor_TakeLeaderLayer(); }

extern s32 gAlhafra_ElderStepsBackScript[];
extern u32 gAlhafra_WomanWalksOffScript[];
extern u32 gAlhafra_AlhafranLeadsBesideFelixScript[];
extern u32 gAlhafra_ElderFollowsBesideFelixScript[];
extern u32 gAlhafra_WaitingMadranFollowsBesideFelixScript[];
extern u32 gAlhafra_MayorFollowsBesideFelixScript[];
extern u32 gAlhafra_SecondSoldierFollowsBesideFelixScript[];
extern u32 gAlhafra_FirstSoldierFollowsBesideFelixScript[];
extern u32 gAlhafra_WearyMadranFollowsBesideFelixScript[];
extern u32 gAlhafra_SilentMadranFollowsBesideFelixScript[];
extern s32 gAlhafra_AlhafranLeadsBehindFelixScript[];
extern u32 gAlhafra_ElderFollowsBehindFelixScript[];
extern u32 gAlhafra_WaitingMadranFollowsBehindFelixScript[];
extern u32 gAlhafra_MayorFollowsBehindFelixScript[];
extern u32 gAlhafra_WearyMadranFollowsBehindFelixScript[];
extern u32 gAlhafra_FirstSoldierFollowsBehindFelixScript[];
extern s32 gAlhafra_SilentMadranFollowsBehindFelixScript[];
extern u32 gAlhafra_SecondSoldierFollowsBehindFelixScript[];

/// The mayor and elder of Madra arrive with their soldiers and townsfolk,
/// ask the way to Alhafra's mayor and follow an Alhafran to the manor.
void Alhafra_MadransArrive(s32 cellBehavior) {
  GameFlag_Set(FLAG_ALHAFRA_MADRANS_ARRIVED);
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgAlhafraFinallyWeMadeIt);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 1);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 1);
  Actor_SetSpeed(ACTOR_WEARY_MADRAN, 1);
  Actor_SetSpeed(ACTOR_SILENT_MADRAN, 1);
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
  Actor_SetPosition(ACTOR_SILENT_MADRAN, 408, 872);
  // "We-We're here?"
  FieldEvent_ShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_SILENT_MADRAN, 0, -48);
  Actor_WalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, -48);
  Actor_WaitForMoveById(ACTOR_SILENT_MADRAN);
  Actor_FaceDirectionById(ACTOR_SILENT_MADRAN, FACING_NORTH_NORTHWEST, 0);
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
  Actor_WalkBy(ACTOR_SILENT_MADRAN, 32, -48);
  Actor_WalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, -48);
  Actor_WaitForMoveById(ACTOR_SILENT_MADRAN);
  Actor_FaceDirection(ACTOR_SILENT_MADRAN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_SOUTH, 0);
  Actor_SetSpritePriority(ACTOR_SILENT_MADRAN, 3);
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
  Actor_WatchAndSetScript(ACTOR_SILENT_MADRAN, ACTOR_MADRAS_MAYOR);
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
                            (s32)gAlhafra_ElderStepsBackScript);
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
  Actor_Stop(ACTOR_SILENT_MADRAN);
  Actor_SetAnimationById(ACTOR_MADRAS_ELDER, 5);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_SetMovementTypeById(ACTOR_WOMAN_ON_THE_LOST_SHIPS,
                            (s32)gAlhafra_WomanWalksOffScript);
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
  Actor_FaceDirection(ACTOR_SILENT_MADRAN, FACING_SOUTH_SOUTHEAST, 0);
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
  gFieldState->fastForward = 0;
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
  Actor_SetSpeed(ACTOR_SILENT_MADRAN, 0.5);
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
                              (s32)gAlhafra_AlhafranLeadsBesideFelixScript);
    Actor_SetMovementTypeById(ACTOR_MADRAS_ELDER,
                              (s32)gAlhafra_ElderFollowsBesideFelixScript);
    Actor_SetMovementTypeById(
        ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
        (s32)gAlhafra_WaitingMadranFollowsBesideFelixScript);
    Event_Wait(80);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 3);
    FieldActor_SetSpritePriority(ACTOR_FIRST_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_SECOND_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_WEARY_MADRAN, 3);
    FieldActor_SetSpritePriority(ACTOR_SILENT_MADRAN, 3);
    FieldActor_SetSpritePriority(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 3);
    FieldActor_WatchAndSetScript(ACTOR_SILENT_MADRAN, ACTOR_MADRAS_ELDER);
    Actor_SetMovementTypeById(ACTOR_MADRAS_MAYOR,
                              (s32)gAlhafra_MayorFollowsBesideFelixScript);
    Event_Wait(170);
    Actor_SetMovementTypeById(
        ACTOR_SECOND_MADRAN_SOLDIER,
        (s32)gAlhafra_SecondSoldierFollowsBesideFelixScript);
    Event_Wait(10);
    Actor_SetMovementTypeById(
        ACTOR_FIRST_MADRAN_SOLDIER,
        (s32)gAlhafra_FirstSoldierFollowsBesideFelixScript);
    Actor_SetMovementTypeById(
        ACTOR_WEARY_MADRAN, (s32)gAlhafra_WearyMadranFollowsBesideFelixScript);
    Event_Wait(60);
    Actor_SetMovementTypeById(
        ACTOR_SILENT_MADRAN,
        (s32)gAlhafra_SilentMadranFollowsBesideFelixScript);
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
                              (s32)gAlhafra_AlhafranLeadsBehindFelixScript);
    Event_Wait(60);
    Actor_SetMovementTypeById(ACTOR_MADRAS_ELDER,
                              (s32)gAlhafra_ElderFollowsBehindFelixScript);
    Actor_SetMovementTypeById(
        ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
        (s32)gAlhafra_WaitingMadranFollowsBehindFelixScript);
    Event_Wait(70);
    FieldActor_WatchAndSetScript(ACTOR_SILENT_MADRAN, ACTOR_MADRAS_ELDER);
    Actor_SetMovementTypeById(ACTOR_MADRAS_MAYOR,
                              (s32)gAlhafra_MayorFollowsBehindFelixScript);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAS_MAYOR, 3);
    FieldActor_SetSpritePriority(ACTOR_FIRST_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_SECOND_MADRAN_SOLDIER, 3);
    FieldActor_SetSpritePriority(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
    FieldActor_SetSpritePriority(ACTOR_WEARY_MADRAN, 3);
    FieldActor_SetSpritePriority(ACTOR_SILENT_MADRAN, 3);
    Actor_SetSpritePriority(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 3);
    Event_Wait(120);
    Actor_SetMovementTypeById(
        ACTOR_WEARY_MADRAN, (s32)gAlhafra_WearyMadranFollowsBehindFelixScript);
    Event_Wait(40);
    Actor_SetMovementTypeById(
        ACTOR_FIRST_MADRAN_SOLDIER,
        (s32)gAlhafra_FirstSoldierFollowsBehindFelixScript);
    Actor_SetMovementTypeById(
        ACTOR_SILENT_MADRAN,
        (s32)gAlhafra_SilentMadranFollowsBehindFelixScript);
    Event_Wait(30);
    Actor_SetMovementTypeById(
        ACTOR_SECOND_MADRAN_SOLDIER,
        (s32)gAlhafra_SecondSoldierFollowsBehindFelixScript);
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
  Actor_SetAnimation(ACTOR_SILENT_MADRAN, 3);
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
  Actor_SetSpeed(ACTOR_SILENT_MADRAN, 1.9);
  Actor_WalkBy(ACTOR_MADRAS_ELDER, 0, -150);
  Actor_WalkBy(ACTOR_MADRAS_MAYOR, 0, -150);
  Actor_WalkBy(ACTOR_FIRST_MADRAN_SOLDIER, 0, -150);
  Actor_WalkBy(ACTOR_SECOND_MADRAN_SOLDIER, 0, -150);
  Actor_WalkBy(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, -150);
  Actor_WalkBy(ACTOR_SILENT_MADRAN, 0, -150);
  Actor_WalkByAndWait(ACTOR_WEARY_MADRAN, 0, -150);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 0, 0);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_MADRAN_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_MADRAN_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 0);
  Actor_SetPosition(ACTOR_WEARY_MADRAN, 0, 0);
  Actor_SetPosition(ACTOR_SILENT_MADRAN, 0, 0);
  Event_Wait(15);
  FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The pirate recognizes the mayor of Madra and runs to warn Briggs's
/// crew, after answering the party if they read his mind.
void Alhafra_PirateSeesMadrasMayor(s32 how) {
  Event_SetFlag(FLAG_PIRATE_LEFT);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraOooThisAintGood);
  if (how == PIRATE_READ_BY_MIND_READ) {
    FieldEvent_Wait(10);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 168, 736);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  }
  Actor_ShowEmote(ACTOR_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Ooo... This ain't good!"
  FieldEvent_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIRATE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "That guy looked like the mayor of Madra..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIRATE, ANIM_SHAKE_HEAD);
  // "He must have come here looking for Briggs."
  FieldEvent_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIRATE, 2);
  FieldEvent_Wait(20);
  // "I'd better go tell the others!"
  FieldEvent_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
  if (how == PIRATE_READ_BY_MIND_READ) {
    Actor_WalkByAndWaitById(ACTOR_PIRATE, -8, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(ACTOR_PIRATE, 3);
    FieldEffect_SetAttached(ACTOR_PIRATE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_GetById(ACTOR_PIRATE)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWaitById(ACTOR_PIRATE, 192, 736);
    Event_Wait(1);
    Actor_GetById(ACTOR_PIRATE)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_Wait(10);
    // "What's your problem? What are you, some kind of freak?"
    Event_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
    Actor_ShowEmote(ACTOR_PIRATE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Well? What!? You got a problem with me?"
    Event_OpenMessage(ACTOR_PIRATE, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      Event_Wait(30);
      Actor_RunRepeatedMotion(ACTOR_PIRATE, 2);
      Event_Wait(20);
      // Yes: "Ah, who cares what you think? I've got stuff to do, so scram,
      // kid!"
      Event_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
      gFieldState->message = (gFieldState->message + 1);
    } else {
      Event_Wait(30);
      gFieldState->message = (gFieldState->message + 1);
      // No: "So where do you get off invading my personal space, huh?"
      Event_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
    }
    Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_FELIX, 152, 752);
    Event_Wait(1);
    Actor_GetById(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_PIRATE, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirectionById(ACTOR_PIRATE, FACING_NORTHEAST, 0);
    Event_Wait(70);
    Actor_ShowEmote(ACTOR_PIRATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
    // "Where was I? Oh yeah, I was going to tell them about the mayor of
    // Madra!"
    Event_ShowMessageThenWait(ACTOR_PIRATE, 0, 10);
  }
  Actor_WalkToAndWait(ACTOR_PIRATE, 184, 728);
  Actor_WalkByAndWait(ACTOR_PIRATE, 0, -110);
  if (how == PIRATE_STEPPED_PAST) {
    Event_Wait(50);
    Actor_SetPosition(ACTOR_PIRATE, 264, 648);
    Actor_WalkByAndWaitById(ACTOR_PIRATE, 0, 32);
    Actor_WalkByAndWaitById(ACTOR_PIRATE, 150, 0);
  }
  Actor_SetPosition(ACTOR_PIRATE, 0, 0);
  Event_End();
}

// clang-format off
/// The three puddles Frost freezes into pillars, each with the flag set while
/// it stands (FlagBlock_PlaceAll).
s16 gAlhafra_Puddles[] = {
    ACTOR_PUDDLE_A, FLAG_PUDDLE_A_FROZEN, ACTOR_PUDDLE_B, FLAG_PUDDLE_B_FROZEN,
    ACTOR_PUDDLE_C, FLAG_PUDDLE_C_FROZEN, -1, 0,
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

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(Alhafra),
    SCENE_EXIT_UNLESS(1, AlhafraRooms, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AlhafraRooms, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AlhafraRooms, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AlhafraRooms, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AlhafraRooms, 7, EXIT_FLAG_NONE),
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
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 736, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
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

/// A doorway with no door: nothing to open.
u16 gAlhafra_NoDoor[] = {
    MAPGRID_ANIMATION_STEPS_END,
};

/// A door opening: one step of cells copied over it.
u16 gAlhafra_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(63, 2, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The other shape of door's opening, from the cells beside the first's.
u16 gAlhafra_SecondDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(62, 2, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The doors Felix opens by walking into them, a row for each exit number
/// (Alhafra_EnterDoor): the door's opening and its cell; a row with none opens
/// nothing.
struct MapGridCellAnimation gAlhafra_Doors[] = {
    {NULL, 0, 0},
    {gAlhafra_DoorOpening, 27, 37},
    {gAlhafra_DoorOpening, 27, 43},
    {NULL, 0, 0},
    {NULL, 0, 0},
    {gAlhafra_DoorOpening, 21, 36},
    {gAlhafra_DoorOpening, 38, 46},
    {gAlhafra_NoDoor, 0, 0},
    {gAlhafra_SecondDoorOpening, 11, 33},
    {NULL, 0, 0},
    {NULL, 0, 0},
    {NULL, 0, 0},
    {gAlhafra_SecondDoorOpening, 5, 9},
    {NULL, 0, 0},
    {gAlhafra_DoorOpening, 16, 9},
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
    {TRIGGER_STEP, CELL_BEHAVIOR_PIRATE_BEHIND, CONDITION_ALWAYS, EVENT(Alhafra_DrawPirateBehind)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PIRATE_IN_FRONT, CONDITION_ALWAYS, EVENT(Alhafra_DrawPirateInFront)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PIRATE_RUNS_OFF, FLAG_PIRATE_LEFT, EVENT(Alhafra_PirateRunsToWarnTheCrew)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeWereHitByA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingAllMyMostExpensive)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatTidalWaveMayWell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItDoesntMatterHowWell)},
    {TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWelcomeToAlhafraThisIs)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraAMassiveTidalWaveHit)},
    {TRIGGER_TALK, ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, EVENT(Alhafra_AskWhetherIndraMoved)},
    {TRIGGER_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraRightAfterMyBoatGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisGuysProbablyWonderingAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingOurBoatsWasAwful)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraIJustCantBelieveA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThoseGuysLookingForA)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraThoseMenDidntLookLike)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraThosePeopleThatJustCame)},
    {TRIGGER_TALK, ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeveHadALotOf)},
    {TRIGGER_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraOurPortWasRuinedBy)},
    {TRIGGER_TALK, ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheOnlyBoatSparedBy)},
    {TRIGGER_TALK, ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, EVENT(Alhafra_BackloggedWithCanoes)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatBoatAtTheWharf)},
    {TRIGGER_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyWouldTheMayorGo)},
    {TRIGGER_TALK, ACTOR_PIRATE, CONDITION_ALWAYS, MSGNO(MsgAlhafraSilence)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfThisGuysLookingTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhoWouldComeToAlhafra)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisTidalWavesActuallyBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhyThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfMyPartner)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeCantRaiseTheMast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIStillCantBelieveThose)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraItMayBeNewfangledBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS, EVENT(Alhafra_ReadPiratesMind)},
    {TRIGGER_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, EVENT(Alhafra_MayorInAMeeting)},
    {TRIGGER_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraListenYouLookLikeAn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHateMyJobI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraMyPartnersAlwaysInSuch)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraNoOneIsAllowedBeyond)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSaidNoOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_PUDDLE_A_FROZEN, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_PUDDLE_B_FROZEN, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_C, FLAG_PUDDLE_C_FROZEN, EVENT(Alhafra_FreezePuddle)},
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
    {TRIGGER_TALK, ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraSoTheGuysWhoBought)},
    {TRIGGER_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryoneIsTalkingAboutHow)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorIsTheOnly)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfMadraApparently)},
    {TRIGGER_TALK, ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraINeverThoughtAFine)},
    {TRIGGER_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraICanHelpBuildHouses)},
    {TRIGGER_TALK, ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraWevePutOffFixingThe)},
    {TRIGGER_TALK, ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceRepairsAreDoneHere)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheyToldMeTheydHelp)},
    {TRIGGER_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, EVENT(Alhafra_SailorToldToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsSoNiceOfEveryone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryoneInTownIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowPiratesWhoPay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThosePiratesHaveTheCoolest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraICantBelieveTheyLeft)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfOurMayorThinksIm)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowSittingHereFixing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraHelpingPeopleOutFeelsReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheFactThatMadraTook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfTheCrew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsStrangeTheQuartersDown)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraWorkingOnDeckWithThe)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceTheyFixTheOld)},
    {TRIGGER_TALK, ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWasAfraidMyHouse)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, EVENT(Alhafra_TalkToHungryBoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfWeDontGetOut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowSinceWereRebuilding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, FLAG_HUNGRY_BOY_FED, MSGNO(MsgAlhafraIWishSomeoneWouldJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfIShould)},
    {USE_ITEM_ON_ACTOR(ITEM_LARGE_BREAD), 27, CONDITION_ALWAYS, EVENT(Alhafra_GiveBreadToHungryBoy)},
    {TRIGGER_TALK, ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraAllMyNeighborsAreGoing)},
    {TRIGGER_TALK, ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatAStrokeOfLuck)},
    {TRIGGER_TALK, ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraMyDadLostTheDraw)},
    {TRIGGER_TALK, ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfTheresAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingTheLotteryAndBeing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraIThoughtTheHardPart)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraAllBecauseOfMyDads)},
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
    {TRIGGER_TALK, ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraAfterWeDidAllThat)},
    {TRIGGER_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItWasGreatToSee)},
    {TRIGGER_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhenTheyFindThatTheir)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsNotLikeTheBoat)},
    {TRIGGER_TALK, ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheRepairsAreComingAlong)},
    {TRIGGER_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhenIsThisHouseGoing)},
    {TRIGGER_TALK, ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraItFeltSoGreatWhen)},
    {TRIGGER_TALK, ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraBackBeforeBriggsDestroyedThe)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeFixedTheMastSo)},
    {TRIGGER_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfTheyCaughtAGood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfOnlyWeHadThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHopeTradeDoesntMake)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraItWasMadrasBoatIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSeemedSoHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfMadraSeems)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheresNoPointInWorrying)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraWereAllPullingTogetherTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfMomWouldStopMaking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIKnowItsTimeTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraFirstHeStealsAShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyWorryAboutBriggsIts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfBriggsHadntStolenThat)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatGuysHouseIsntFixed)},
    {TRIGGER_TALK, ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorsBeenHandingOut)},
    {TRIGGER_TALK, ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeHaveToStoreWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeCouldUseSomeHelp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhatWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIveSeenFatherTryingTo)},
    {TRIGGER_TALK, ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraEvenIfWeFixIt)},
    {TRIGGER_TALK, ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithEveryonesHelpWeWere)},
    {TRIGGER_TALK, ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraMomWasHappyToHear)},
    {TRIGGER_TALK, ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraThereAreALotOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisIsJustTooMuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraHesNeverGoingToGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheresNoWayDadCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraCarryingTheseThingsAllOver)},
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
    {TRIGGER_TALK, ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhyEveryones)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsPaidALotFor)},
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

s32 gAlhafra_ElderStepsBackScript[] = {
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

u32 gAlhafra_WomanWalksOffScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(552, 0, 808),
    SC_SYNC,
    SC_MOVE(536, 0, 760),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gAlhafra_AlhafranLeadsBesideFelixScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(440, 0, 800),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(248, 0, 680),
    SC_SYNC,
    SC_MOVE(248, 0, 632),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_MayorFollowsBesideFelixScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(424, 0, 792),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(248, 0, 680),
    SC_SYNC,
    SC_MOVE(248, 0, 664),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_ElderFollowsBesideFelixScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(440, 0, 808),
    SC_SYNC,
    SC_MOVE(440, 0, 800),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(264, 0, 680),
    SC_SYNC,
    SC_MOVE(264, 0, 664),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_WaitingMadranFollowsBesideFelixScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(440, 0, 808),
    SC_SYNC,
    SC_MOVE(440, 0, 800),
    SC_SYNC,
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(248, 0, 680),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_FirstSoldierFollowsBesideFelixScript[] = {
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

u32 gAlhafra_WearyMadranFollowsBesideFelixScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(408, 0, 760),
    SC_SYNC,
    SC_MOVE(408, 0, 680),
    SC_SYNC,
    SC_MOVE(280, 0, 680),
    SC_SYNC,
    SC_MOVE(280, 0, 696),
    SC_SYNC,
    SC_MOVE(248, 0, 696),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gAlhafra_SecondSoldierFollowsBesideFelixScript[] = {
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

u32 gAlhafra_SilentMadranFollowsBesideFelixScript[] = {
    SC_ANIMATION(2),
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
    SC_MOVE(256, 0, 712),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

s32 gAlhafra_AlhafranLeadsBehindFelixScript[] = {
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

u32 gAlhafra_MayorFollowsBehindFelixScript[] = {
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

u32 gAlhafra_ElderFollowsBehindFelixScript[] = {
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

u32 gAlhafra_WaitingMadranFollowsBehindFelixScript[] = {
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

u32 gAlhafra_WearyMadranFollowsBehindFelixScript[] = {
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

u32 gAlhafra_SecondSoldierFollowsBehindFelixScript[] = {
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

u32 gAlhafra_FirstSoldierFollowsBehindFelixScript[] = {
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

s32 gAlhafra_SilentMadranFollowsBehindFelixScript[] = {
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
