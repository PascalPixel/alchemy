/// @file
/// Babi's Palace in Tolbi, two scenes under the area name Babi's Palace that
/// share this overlay and one exit table:
///
/// - SceneId_BabiPalaceEntrance, the court between the town and the palace
///   door, with two guards and a well. Its door opens by a cell animation
///   (gBabiPalace_DoorCells, BabiPalace_LeaveThroughAnimatedDoor).
/// - SceneId_BabiPalace, the palace's rooms on one map, joined by exits into
///   the same map and by doors that open by the map's tileset animations
///   (enum BabiPalaceDoorAnimation, BabiPalace_LeaveThroughDoor). One door
///   leads to Babi's scholars (BABI_PALACE_TO_COTTAGE_ROOMS_B, held by
///   cottage_rooms_b.c) and one into the Colosseum Tunnel
///   (BABI_PALACE_TO_COLOSSEUM_TUNNEL).
///
/// Each scene has three actor placement tables and three trigger tables.
/// Scene_GetActorPlacements and Scene_GetTriggers choose between them by two
/// story flags. The three times are while Babi is missing, during the finals
/// (FLAG_BABI_SAVED, set when the party saves him in Altmiller Cave) and after
/// Colosso (FLAG_COLOSSO_OVER).
///
/// The people inside stand in all three under other actor ids, so each table
/// has its own actor id enum.
///
/// While Babi is missing, the attendant shows the party four beds
/// (BabiPalace_ShowBeds). A search then sleeps in them
/// (BabiPalace_RestInPalaceBeds), with a talk the first night
/// (FLAG_BABI_PALACE_FIRST_NIGHT).
///
/// Stepping beside Iodem (CELL_BEHAVIOR_BABI_PALACE_BESIDE_IODEM) plays
/// BabiPalace_HearSoldiersReport once. Its flag is FLAG_IODEM_HEARD_SOLDIERS,
/// and the rows for Iodem and his soldiers place no one once it is set.
///
/// Sheba notices the first Mind Read on her (BabiPalace_ReadShebasMind,
/// FLAG_SHEBA_NOTICED_MIND_READ).
///
/// During the finals the guards outside pass on Babi's word once
/// (BabiPalace_PassOnBabisWord, FLAG_BABI_PALACE_GUARD_GAVE_WORD) and the
/// attendant offers a rest (BabiPalace_OfferRest).
///
/// The finals send the party back here by two entrances.
/// BabiPalace_ApplyEntryState answers them by restoring the party and its
/// inventory (BabiPalace_RestorePartyAndInventory) and playing a waking in the
/// big bedroom. A lost first or second match comes in by
/// BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH (BabiPalace_WakeFromFinalsDream). The
/// third match comes in by BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH
/// (BabiPalace_WakeAfterFinals).
///
/// After Colosso, entering the throne room by
/// BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM the first time plays Babi's
/// audience (BabiPalace_MeetBabiAfterColosso, FLAG_BABI_SPOKE_AFTER_COLOSSO).
/// It gives the victor's prize when FLAG_WON_COLOSSO is set, and ends with Babi
/// sending Iodem with the party, to meet at Gondowan Passage.
///
/// The Cloak Ball on show is an actor drawn from the item's icon until the
/// party takes it (BabiPalace_OfferCloakBall, FLAG_BABI_PALACE_CLOAK_BALL).

#include "main_routine_work.h"
#include "heap.h"
#include "version.h"
#include "input.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/babi_palace.h"
#include "module/field/field_sequence.h"
#include "module/system/main_routine.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/shop/shop.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "event.h"
#include "item_ids.h"
#include "actor_script.h"
#include "heap_slots.h"
#include "display_transition.h"
#include "flags.h"
#include "sounds.h"
#include "sound_driver_symbols.h"

/// The palace's actors are placed by one of three tables, as the story goes on.
///
/// The three are before Babi is rescued
/// (gBabiPalace_ActorPlacementsBabiMissing), during the finals
/// (gBabiPalace_ActorPlacementsFinals) and after Colosso
/// (gBabiPalace_ActorPlacementsAfterColosso).
///
/// Each numbers its rows from ACTOR_FIRST_PLACED, with Garet, Ivan, Mia and
/// Sheba in their own party slots. Most of the people stand in all three under
/// other numbers.
///
/// The palace's residents, three old men with canes and a young man with brown
/// hair, stand in all three. One sent Babi's soldiers to search the town, and
/// another tells warriors who are not entering Colosso that they may not stay.
/// The game names none of them.
///
/// Before the rescue there are the attendant who shows the party its beds,
/// Iodem and the two soldiers he sent to Altmiller Cave. Sheba is held here
/// until Colosso is over.
enum BabiPalaceActorId {
  /// An old man with a cane, in the same room and at the same number in every
  /// table. He is the one who sent Babi's soldiers to search the town.
  ///
  /// He asks "Did you run into any of Babi's soldiers in Town?", and if not,
  /// says "Hmph! I told them to search every nook and cranny of this town..."
  /// ★ Named from the game: "Did you run into any of Babi's soldiers in Town?"
  /// (MsgTolbiRunBabisSoldiers)
  ACTOR_ASKING_ABOUT_BABIS_SOLDIERS = ACTOR_FIRST_PLACED,
  /// The attendant, a woman with fair hair, whose number is the same after
  /// Colosso: "No one's going to sleep in this big bedroom until the next
  /// Colosso!"
  /// ★ Named from the game: "We have just enough extra beds in this room for
  /// everybody." (MsgTolbiWeHaveJustEnoughExtra)
  ACTOR_BED_ATTENDANT,
  /// A warrior in a golden helmet. "So, did you guys get injured, too?"
  /// Thoughts: "I hurt my back during my training, and now I can't move at
  /// all."
  /// ★ Named from the game: "I hurt my back during my training"
  /// (MsgBabiPalaceIGetSoAngrySeeingAll)
  ACTOR_INJURED_WARRIOR,
  /// A warrior in a golden helmet. "I can't fight because of a little
  /// indigestion? That's pathetic!"
  /// ★ Named from the game: "Maybe I shouldn't have eaten quite so much
  /// yesterday..." (MsgBabiPalaceMaybeIShouldntHaveEatenQuite)
  ACTOR_WARRIOR_WITH_INDIGESTION,
  /// A young man with brown hair in a purple shirt. "If you're entering
  /// Colosso, hurry along to the colosseum." Thoughts: "Warriors are not
  /// allowed to stay in the palace if they don't enter Colosso!"
  /// ★ Named from the game: "If you're entering Colosso, hurry along to the
  /// colosseum." (MsgBabiPalaceIfYoureEnteringColossoHurryAlong)
  ACTOR_HURRYING_WARRIORS_ALONG,
  /// Four of Babi's soldiers, in one room. The first says "Only warriors who
  /// fight well in Colosso's finals can become soldiers here."
  /// ★ Named from the game: "Only warriors who fight well in Colosso's finals
  /// can become soldiers here." (MsgBabiPalaceOnlyWarriorsWhoFightWellIn)
  ACTOR_FIRST_PALACE_SOLDIER,
  /// An old man with a cane beside Babi's bed. "It looks like Lord Babi didn't
  /// use his bed this morning."
  /// ★ Named from what the player sees: an old man with a cane beside Babi's
  /// bed (its render)
  ACTOR_BY_BABIS_BED = ACTOR_FIRST_PALACE_SOLDIER + 4,
  /// An old man with a cane. "Sheba has been staying as Lord Babi's guest for
  /// some time." Thoughts: "It is heartbreaking to see her tears and hear her
  /// pleas to be released."
  /// ★ Named from the game: "It is heartbreaking to see her tears"
  /// (MsgBabiPalaceActorWasTakenHostageToForce)
  ACTOR_SORRY_FOR_SHEBA,
  /// In Sheba's room, in SPRITE_OLD_WOMAN, as in vale_plaza.c. Thoughts: "Sheba
  /// is not permitted to leave this room."
  /// ★ Named from what the player sees: a white-haired old woman in Sheba's
  /// room (its render)
  ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM,
  /// ★ Named from the game: "Iodem, Iodem!" (MsgTolbiIodemIodem)
  ACTOR_IODEM,
  /// ★ Named from what the player sees: a soldier in a helmet with a spear,
  /// SPRITE_SOLDIER (its render)
  ACTOR_FIRST_SOLDIER,
  /// ★ Named from what the player sees: the second soldier, SPRITE_SOLDIER
  /// (its render)
  ACTOR_SECOND_SOLDIER
};

/// During the finals, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS keeps its number, and
/// the others stand under new ones.
enum BabiPalaceFinalsActorId {
  /// The same resident as ACTOR_HURRYING_WARRIORS_ALONG: "What are you doing?
  /// Hurry over to the colosseum!"
  /// ★ Named from the game: "Hurry over to the colosseum!"
  /// (MsgBabiPalaceWhatAreYouDoingHurryOver)
  ACTOR_FINALS_HURRYING_WARRIORS_ALONG = ACTOR_FIRST_PLACED + 1,
  /// The same attendant, a number later.
  /// ★ Named from the game: "Would you like to sleep right now?"
  /// (MsgTolbiCameRestBefore)
  ACTOR_FINALS_ATTENDANT,
  /// The same resident as ACTOR_BY_BABIS_BED: "Babi has returned and the
  /// finals are set to begin!"
  /// ★ Named from the game: "Babi has returned and the finals are set to
  /// begin!" (MsgBabiPalaceBabiHasReturnedAndTheFinals)
  ACTOR_FINALS_BY_BABIS_BED,
  /// The same resident as ACTOR_SORRY_FOR_SHEBA: "What's going on? Sheba is
  /// looking much happier."
  /// ★ Named from the game: "I wonder what happened to make that girl smile."
  /// (MsgBabiPalaceWhatsGoingOnActorIsLooking)
  ACTOR_FINALS_SORRY_FOR_SHEBA,
  /// The same woman as ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM: "The soldiers came to
  /// see Sheba. She's been in a great mood ever since."
  /// ★ Named from the game: "She's been in a great mood ever since."
  /// (MsgBabiPalaceTheSoldiersCameToSeeActor)
  ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM
};

/// After Colosso there are Babi and the Cloak Ball he keeps on show.
/// ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, ACTOR_BED_ATTENDANT and Iodem keep their
/// numbers.
enum BabiPalaceAfterColossoActorId {
  /// One of Babi's soldiers.
  /// ★ Named from the game: "We're relieved everything went so well."
  /// (MsgBabiPalaceBabiWasFoundSafeAndSound)
  ACTOR_RELIEVED_SOLDIER = ACTOR_FIRST_PLACED + 2,
  /// The same resident as ACTOR_HURRYING_WARRIORS_ALONG: "So, you're all from
  /// the eastern shores of the Karagol Sea, huh?"
  /// ★ Named from the game: "So, you're all from the eastern shores of the
  /// Karagol Sea, huh?" (MsgTolbiEasternShoresKaragol)
  ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES,
  /// One of Babi's soldiers.
  /// ★ Named from the game: "Now that Colosso is over, I'm going to Gondowan to
  /// build Babi Lighthouse!" (MsgBabiPalaceNowThatColossoIsOverIm)
  ACTOR_GONDOWAN_BOUND_SOLDIER,
  /// "The southern continent is a dangerous place... you'll know why I prefer
  /// Angara."
  /// ★ Named from the game: "you'll know why I prefer Angara."
  /// (MsgBabiPalaceTheSouthernContinentIsADangerous)
  ACTOR_SOLDIER_PREFERRING_ANGARA,
  /// ★ Named from the game: "Babi's face is very pale"
  /// (MsgBabiPalaceBabisFaceIsVeryPaleLike)
  ACTOR_BABI,
  /// The same resident as ACTOR_BY_BABIS_BED: "Babi's face is very pale,
  /// like that of a dead man."
  /// ★ Named from the game: "Babi's face is very pale"
  /// (MsgBabiPalaceBabisFaceIsVeryPaleLike)
  ACTOR_AFTER_COLOSSO_BY_BABIS_BED,
  /// ★ Named from the game: "You found the Cloak Ball."
  /// (MsgTolbiFoundCloakBall)
  ACTOR_CLOAK_BALL,
  /// The same resident as ACTOR_SORRY_FOR_SHEBA: "The girl from Lalivero is
  /// no longer here."
  /// ★ Named from the game: "The girl from Lalivero is no longer here."
  /// (MsgBabiPalaceTheGirlFromLaliveroIsNo)
  ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA,
  /// The same woman as ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM: "I am surprised Sheba
  /// was allowed to return to Lalivero like that."
  /// ★ Named from the game: "At least she won't be homesick any longer."
  /// (MsgBabiPalaceIAmSurprisedActorWasAllowed)
  ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM
};

/// The two guards outside the palace (BabiPalaceEntrance).
enum {
  /// ★ Named from the game: "Normally, no one's allowed into Babi's Palace"
  /// (MsgBabiPalaceNormallyNoOnesAllowedIntoBabis)
  ACTOR_FIRST_GUARD = 8,
  /// ★ Named from the game: "Some of the rooms in the palace are available
  /// to Colosso entrants." (MsgBabiPalaceSomeOfTheRoomsInThe)
  ACTOR_SECOND_GUARD = 9
};

/// The attendant has led the party to the beds. Talking again sends the
/// attendant back.
/// ★ Named from the game: "We have just enough extra beds in this room for
/// everybody." (MsgTolbiWeHaveJustEnoughExtra)
enum { FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS = FLAG_SCENE_TEMP_00 };

/// The palace map's tileset animations that open its doors, for
/// TilesetAnim_Resume (play) and TilesetAnim_Pause (pause).
///
/// Each scene entry pauses them, so every door is drawn shut. The recording of
/// BabiPalace_LeaveThroughDoor shows one swing open
/// (https://alchemy.pascalpixel.com/places/babi-palace/events/BabiPalace_LeaveThroughDoor.gif).
enum BabiPalaceDoorAnimation {
  BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION = 1, ///< the throne room's door
  BABI_PALACE_OTHER_DOORS_ANIMATION = 2,       ///< every door without its own
  BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION = 4     ///< the oven room's door
};

/// The leader's animations in the big bedroom's bed, after the four shared ones
/// (enum ActorAnimation).
///
/// The nod and the shake of the head answer yes and no in
/// BabiPalace_WakeAfterFinals.
enum BabiPalaceBedAnimation {
  ANIM_IN_BED_LYING = 31,
  /// Sitting up. The leader is raised by 3 pixels as it starts.
  ANIM_IN_BED_SITTING_UP = 32,
  ANIM_IN_BED_NOD = 33,
  ANIM_IN_BED_SHAKE_HEAD = 34
};

/// Cell behaviors the triggers answer besides the exits, whose cell
/// behaviors are their exits' numbers.
enum BabiPalaceCellBehavior {
  /// The cells beside Iodem, while Babi is missing
  /// (BabiPalace_HearSoldiersReport).
  CELL_BEHAVIOR_BABI_PALACE_BESIDE_IODEM = 60,
  CELL_BEHAVIOR_BABI_PALACE_BEDS =
      70, ///< the big bedroom's four beds, searched to sleep
  /// The well outside the palace.
  /// ★ Named from the game: "peered into the well" (MsgFieldPeeredWell)
  CELL_BEHAVIOR_BABI_PALACE_WELL = 90
};

#include "shared/walk_into_leader.h"

static inline void BabiPalace_CloseDoors(void) {
  TilesetAnim_Pause(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
  TilesetAnim_Pause(BABI_PALACE_OTHER_DOORS_ANIMATION);
  TilesetAnim_Pause(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
}

/// After a night in the palace's beds, the party's HP and PP are full, Garet,
/// Ivan and Mia rejoin, and the inventory Colosso held is returned.
static __inline__ void BabiPalace_RestorePartyAndInventory(void) {
  s16 members[8];
  struct Unit *state;
  s32 memberCount;
  s32 i;

  memberCount = FieldParty_ListMembers(members);
  for (i = 0; i < memberCount; i++) {
    state = Unit_Get(members[i]);
    state->hp = state->hpMax;
    state->pp = state->ppMax;
    Unit_RecalculateGauges(members[i]);
  }
  Party_AddMember(GARET);
  Party_AddMember(IVAN);
  Party_AddMember(MIA);
  InventorySnapshot_Restore();
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_BabiPalaceEntrance) {
    return gBabiPalace_OutsideEntrances;
  }
  return gBabiPalace_Entrances;
}

/// The palace has no exit regions, so the table is empty.
const struct SceneExitRegion *BabiPalace_GetRegions(void) {
  return gBabiPalace_Regions;
}

/// Both scenes share one exit table.
const u32 *BabiPalace_GetExits(void) { return gBabiPalace_Exits; }

/// Returns the actor placements for the scene and the story, which is before
/// Babi's rescue, during the finals or after Colosso.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_BabiPalaceEntrance) {
    return gBabiPalace_OutsideActorPlacements;
  }
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gBabiPalace_ActorPlacementsAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gBabiPalace_ActorPlacementsFinals;
  }
  return gBabiPalace_ActorPlacementsBabiMissing;
}

/// Asks a yes or no question. The answers are the two messages after it.
static inline void BabiPalace_ShowQuestionAnswer(s32 actorId, s32 question) {
  Event_SetMessageId(question);
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    Event_SetMessageId(question + 1);
  else
    Event_SetMessageId(question + 2);
  Event_ShowMessage(actorId, 0);
}

/// An old man with a cane asks whether the party ran into Babi's soldiers.
///
/// He's one of the palace's residents, and asks while Babi is missing whether
/// the party ran into them in town.
static void BabiPalace_AskAboutSoldiers(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiRunBabisSoldiers);
}

/// The attendant leads the party to the beds when spoken to before Babi's
/// rescue. "We have just enough extra beds in this room for everybody. Please
/// follow me."
///
/// Talking again, the attendant asks whether the party will sleep right away
/// and goes back.
static void BabiPalace_ShowBeds(s32 actorId) {

  struct Actor *leader;
  s16 roundedFacing;
  s32 facingOrMessage;
  s32 facingBits;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + TRIG_TURN / 8) & 0xc000;
  Event_Start();
  Field_EndBusyAndReveal();
  if (Event_GetFlag(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
    GameFlag_Clear(FLAG_BABI_PALACE_BEDS_OPEN);
    Event_SetMessageId(MsgTolbiWeHaveJustEnoughExtra);
    // "We have just enough extra beds in this room for everybody...."
    Event_ShowMessage(actorId, 0);
    Event_Wait(10);
    facingBits = roundedFacing << 16;
    facingOrMessage = FACING_SOUTH;
    if (facingBits == (FACING_SOUTH << 16)) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    }
    Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(actorId, 0, -48);
    Actor_WalkByAndWait(actorId, 64, 0);
    Actor_FaceDirection(actorId, facingOrMessage, 0);
  } else {
    GameFlag_Clear(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
    GameFlag_Set(FLAG_BABI_PALACE_BEDS_OPEN);
    Actor_FaceDirection(actorId, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(20);
    facingOrMessage = MsgTolbiUseFourBeds;
    Event_SetMessageId(facingOrMessage);
    // "You may use these four beds. Will you be going right to sleep?"
    Event_OpenMessage(actorId, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_SetMessageId(facingOrMessage + 1);
      // "Then, enjoy your rest."
      Event_ShowMessage(actorId, 0);
    } else {
      Event_SetMessageId(facingOrMessage + 2);
      // "Well, if you do get tired, feel free to sleep here anytime..."
      Event_ShowMessage(actorId, 0);
    }
    Event_Wait(10);
    Actor_SetAnimationAndWait(actorId, ANIM_NOD);
    Event_Wait(20);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
#if TBS_PATCH_3
    Actor_SetPosition(actorId, 56 << 16, 120 << 16);
#endif
  }
  Event_End();
}

/// The attendant offers the party a rest when spoken to during the finals.
///
/// The first time, the attendant asks whether the party came to rest before the
/// match and steps aside. Afterwards, the attendant offers rest. "if you're
/// tired, please feel free to get some rest."
static void BabiPalace_OfferRest(s32 actorId) {
  s32 question;

  Event_Start();
  Field_EndBusyAndReveal();
  if (Event_GetFlag(FLAG_BABI_PALACE_FINALS_REST_OFFERED) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
    GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
    Actor_FaceDirection(actorId, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(20);
    question = MsgTolbiCameRestBefore;
    Event_SetMessageId(question);
    // "You all came to rest up before the match, didn't you? Would..."
    Event_OpenMessage(actorId, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(question + 1);
    } else {
      Event_SetMessageId(question + 2);
    }
    Event_ShowMessage(actorId, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(actorId, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
  } else {
    Event_SetMessageId(MsgTolbiTiredFeelFreeRest);
    // "Well, if you're tired, please feel free to get some rest."
    Event_OpenMessage(actorId, 0);
  }
  Event_End();
}

/// A warrior asks whether the party is entering Colosso.
///
/// The warrior is in the palace while Babi is missing, and either hurries the
/// party to the trials or asks whether it got hurt too.
static void BabiPalace_AskAboutColosso(s32 actorId) {
  s32 question = MsgTolbiPlanningEnterColosso;

  Event_SetMessageId(question);
  // "Are you planning to enter Colosso?"
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Event_SetMessageId(question + 1);
  } else {
    Event_Wait(10);
    Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessage(actorId, 0);
}

/// A warrior with indigestion gives up on entering Colosso.
///
/// The warrior is in the palace while Babi is missing, tries to shrug it off,
/// and doubles up.
static void BabiPalace_TalkToWarriorWithIndigestion(s32 actorId) {
  struct Actor *actor;
  const s32 *indigestionScript;

  actor = Actor_Get(actorId);
  Event_Start();
  indigestionScript = gBabiPalace_IndigestionScript;
  Actor_SetScript(actorId, indigestionScript);
  Event_SetMessageId(MsgTolbiCantFightBecauseLittleIndigestion);
  // "I can't fight because of a little indigestion? That's pathetic!"
  Event_ShowMessage(actorId, 0);
  Actor_Stop(actorId);
  actor->scaleY = Q16_ONE;
  actor->scaleX = Q16_ONE;
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(60);
  // "Maybe I can shrug off this pain in my gut... I have to enter..."
  Event_ShowMessage(actorId, 0);
  Event_Wait(20);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_SetScript(actorId, indigestionScript);
  // "It's no use! My stomach really hurts!"
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirection(actorId, FACING_NORTHEAST, 0);
  Event_Wait(10);
  actor->scaleY = Q16_ONE;
  actor->scaleX = Q16_ONE;
  Actor_SetScript(actorId, indigestionScript);
  Event_End();
}

/// Mind Read on Sheba in the palace, while Babi is missing, reads that she
/// cannot escape to Lalivero alone.
///
/// The first time, she feels it, ends the Psynergy and asks about it. "Did you
/// just..."
static void BabiPalace_ReadShebasMind(s32 casterId, s32 actorId) {
  Event_Start();
  Event_SetMessageId(MsgTolbiEvenIfEscapedBabiPalace);
  // "Even if I escaped Babi Palace, my power could not get me to..."
  Event_ShowMessage(actorId, 0);
  if (Event_GetFlag(FLAG_SHEBA_NOTICED_MIND_READ) == 0) {
    GameFlag_Set(FLAG_SHEBA_NOTICED_MIND_READ);
    Psynergy_Cancel();
    Event_Wait(50);
    Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    Actor_FaceActor(actorId, ACTOR_PARTY_LEADER, 40);
    // "Did you just..."
    Event_ShowMessage(actorId, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "No, it is nothing."
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(actorId, FACING_WEST, 0);
  }
  Event_End();
}

/// The same old man, after Colosso, asks whether the party has met with Babi.
static void BabiPalace_AskAboutMeetingBabi(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiMeetBabi);
}

/// A young man asks whether the party is going back east already.
///
/// He has brown hair and is one of the palace's residents. He asks after
/// Colosso whether the party wants to go back already to the eastern shores of
/// the Karagol Sea.
static void BabiPalace_AskAboutEasternShores(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiEasternShoresKaragol);
}

/// When the party takes the Cloak Ball on show after Colosso, Babi asks whether
/// it wants it, and lends it for the quest if it does.
static void BabiPalace_OfferCloakBall(void) {
  s32 msg;

  Event_Start();
  Field_EndBusyAndReveal();
  msg = MsgTolbiFoundCloakBall;
  Event_SetMessageId(msg);
  // "You found the Cloak Ball."
  Event_ShowMessage(-1, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BABI, 30);
  // "Isaac! Is that Cloak Ball what you desire?"
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Event_SetMessageId(msg + 2);
    // "Then please leave the Cloak Ball alone. Do not worry about..."
    Event_ShowMessage(ACTOR_BABI, 0);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 3);
    // "That Cloak Ball is very precious to me. I can't just give it..."
    Event_ShowMessage(ACTOR_BABI, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_SetPosition(ACTOR_CLOAK_BALL, 0, 0);
    Item_ShowFound(ITEM_CLOAK_BALL, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Party_GiveItem(ITEM_CLOAK_BALL, 0);
    GameFlag_Set(FLAG_BABI_PALACE_CLOAK_BALL);
  }
}

/// Waking after resting before the finals, the leader has dreamt of fighting in
/// them. The others tease, and the party sets off for the colosseum.
static void BabiPalace_WakeFromFinalsDream(void) {
  struct Actor *leader;

  Event_Start();
  Event_SetMessageId(MsgTolbiGetUp);
  Actor_Get(ACTOR_PARTY_LEADER)->spriteStorageKind = 0;
  Actor_Get(ACTOR_FINALS_ATTENDANT)->spriteStorageKind = 0;
  wait(1);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  // "Isaac! Get up, Isaac!"
  Event_ShowMessage(-1, 0);
  REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  Actor_Get(ACTOR_PARTY_LEADER)->spriteStorageKind = 1;
  Actor_Get(ACTOR_FINALS_ATTENDANT)->spriteStorageKind = 1;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_LYING);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(120), PIXELS(104));
  Actor_SetPosition(ACTOR_MIA, PIXELS(104), PIXELS(80));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(120), PIXELS(120));
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  gFieldWork->transitionFrames = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  gFieldWork->transitionFrames = 24;
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_MIA, 16, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(30);
  // "Are you okay? You were having nightmares for quite a while..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->z.fixed += -PIXELS(3);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->targetZ += -PIXELS(3);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
  Event_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  // "What's the matter, Isaac?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "What? You entered and fought in the finals?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "How could you? You haven't even been to the colosseum yet,..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Sounds like you've been dreaming, Isaac."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(45);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  // "You're so exhausted, it's giving you nightmares. Are you..."
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(-1, 0) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "You think you can manage? I understand, but don't overdo it,..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "Isaac's awake now, so let's head over to the colosseum."
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message += 1;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    gFieldWork->message += 2;
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "Okay! We're off to the colosseum!"
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_GARET, -16, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_Wait(35);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_MIA);
  Scene_WalkIntoLeader(ACTOR_IVAN);
  Event_Wait(10);
  Event_End();
}

/// Searching the beds sends the party to bed, talking it over the first night.
/// "Would you like to sleep now?"
static void BabiPalace_RestInPalaceBeds(void) {
  struct Actor *leader;
  s16 roundedFacing;
  u16 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + TRIG_TURN / 8) & -0x4000;
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgTolbiLikeSleep);
  // "Would you like to sleep now?"
  Event_OpenMessage(-1, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
  } else {
    if (Event_GetFlag(FLAG_BABI_PALACE_FIRST_NIGHT) != 0) {
      Event_Wait(20);
      Inn_PlaySleep(0);
      return;
    }
    Event_Wait(20);
    facing = roundedFacing;
    if (facing == FACING_EAST) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
    }
    if (facing == FACING_WEST) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 240, 120);
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 120);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(10);
    FieldActor_LaunchFromLeader(ACTOR_GARET, 16, 0, FACING_WEST);
    Actor_WaitForScriptEndById(ACTOR_GARET);
    Event_Wait(10);
    // "Ohh, I'm so tired..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_GARET, 40, 0);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -32);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_Wait(20);
    FieldActor_LaunchFromLeader(ACTOR_IVAN, -16, 0, FACING_EAST);
    Actor_WaitForScriptEndById(ACTOR_IVAN);
    Event_Wait(10);
    // "I didn't think we'd ever find a place to stay. What a relief!"
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_IVAN, -40, 0);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, 40);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(20);
    FieldActor_LaunchFromLeader(ACTOR_MIA, 16, 0, FACING_WEST);
    Actor_WaitForScriptEndById(ACTOR_MIA);
    Event_Wait(10);
    // "Let's rest up for tomorrow."
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_MIA, 40, 0);
    Actor_WalkByAndWait(ACTOR_MIA, 0, 40);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    Event_Wait(20);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -56, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -32);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(10);
    // "Good night."
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      // "G'night."
      Event_ShowMessage(ACTOR_GARET, 0);
    } else {
      gFieldWork->message++;
    }
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      // "Sleep well."
      Event_ShowMessage(ACTOR_IVAN, 0);
    } else {
      gFieldWork->message++;
    }
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
    Inn_PlaySleep(0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    // "Morning."
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      // "Good morning."
      Event_ShowMessage(ACTOR_IVAN, 0);
    } else {
      gFieldWork->message++;
    }
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      // "Good morning."
      Event_ShowMessage(ACTOR_MIA, 0);
    } else {
      gFieldWork->message++;
    }
    Actor_WalkByAndWait(ACTOR_GARET, 0, 32);
    Actor_WalkBy(ACTOR_GARET, -112, 0);
    Actor_WalkByAndWait(ACTOR_MIA, 0, -40);
    Actor_WalkBy(ACTOR_MIA, -112, 0);
    Event_Wait(50);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, -24);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -16);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Event_Wait(20);
    // "All right, Isaac! We've got another hard day of work ahead..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
    Scene_WalkIntoLeader(ACTOR_GARET);
    Scene_WalkIntoLeader(ACTOR_MIA);
    Scene_WalkIntoLeader(ACTOR_IVAN);
    GameFlag_Set(FLAG_BABI_PALACE_FIRST_NIGHT);
  }
  Event_End();
}

/// Walking past Iodem while Babi is missing, the party hears his soldiers
/// report.
///
/// His two soldiers report back from Altmiller Cave without Babi, and Iodem
/// tells how Babi has vanished into the cave before. Then they leave.
static void BabiPalace_HearSoldiersReport(void) {
  GameFlag_Set(FLAG_IODEM_HEARD_SOLDIERS);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgTolbiIodemIodem);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 520, 424);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  FieldCamera_MoveTo(PIXELS(560), -1, PIXELS(368), 1);
  FieldCamera_WaitForMove();
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IODEM, 40, 0);
  Actor_WalkToAndWait(ACTOR_IODEM, 584, 360);
  // "Iodem, Iodem!"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_FIRST_SOLDIER, 528, 352);
  Actor_WalkToAndWait(ACTOR_SECOND_SOLDIER, 528, 368);
  wait(3);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  // "We went to Altmiller Cave, as you commanded."
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Event_Wait(10);
  // "And what have you? What news is there of Babi?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "This is futile..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 8, 0);
  Event_Wait(20);
  // "Iodem, why did you send us to look for Babi in a place like..."
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 8, 0);
  Event_Wait(20);
  // "Why would Lord Babi enter such a dangerous cave alone? It's..."
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Inconceivable? Is that what you were going to say?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  // "Altmiller Cave is an evil place."
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "If Babi went there alone, a monster might have..."
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I'm aware of Altmiller Cave's...eccentricities."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "You knew? Then why would you send us in there, Iodem?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Lord Babi has left in secret before..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(20);
  // "You mean, this isn't the first time Lord Babi has..."
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  // "Do you know where he went before?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "I followed Lord Babi once..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "I was certain he had gone into Altmiller Cave..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Did you ever investigate the cave, Iodem?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  // "What was he doing in the cave?"
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(30);
  // "Once he entered the cave, Lord Babi vanished before my very..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Vanished?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "You mean you lost sight of Lord Babi. Is that it?"
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "After he entered the mouth of the cave, I followed him......"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I tried following him several times after that. Each time,..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  // "And you think that's where Lord Babi went this morning, Iodem?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  // "But if he always returned from Altmiller Cave before..."
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(20);
  // "Then he should be back soon, right?"
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "No. This time is different. Something puzzles me..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  // "What do you mean?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "It is certainly strange that Lord Babi should go right..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "What is it?"
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Lord Babi's bedding was not disturbed this morning."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What's unusual about that?"
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Last night, Lord Babi did not use his bed. That is the problem."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Lord Babi has, in fact, been missing since last night."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_FIRST_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SECOND_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(20);
  // "And that is why I am so worried. He has never done this before."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "We didn't know he'd been missing so long..."
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "You don't think Lord Babi's been ki-"
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_Wait(20);
  // "Speak no such foolishness! I am sure Lord Babi is alive and..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  // "Ah, yes, of course."
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I will go to the colosseum in Lord Babi's stead. You must..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  // "We will all conduct another round of searches."
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(13) / 10, PIXELS(13) / 20);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(13) / 10, PIXELS(13) / 20);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 120);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 0, 120);
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, 0, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Event_Wait(30);
  // "Lord Babi, where have you gone?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IODEM, 120, 0);
  Actor_WalkByAndWait(ACTOR_IODEM, 60, 0);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_End();
}

/// Waking in the palace after the finals, the leader has slept long after
/// fighting to the end. Babi wants to see the party.
static void BabiPalace_WakeAfterFinals(void) {
  struct Actor *actor;
  s32 step;
  s32 pick;
  struct FieldWork *work;
  struct ScreenTransitionState *state;

  Event_Start();
  Event_SetMessageId(MsgTolbiCallsName);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_LYING);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(104), PIXELS(104));
  Actor_SetPosition(ACTOR_MIA, PIXELS(88), PIXELS(120));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(120), PIXELS(120));
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 12;
  wait(1);
  ScreenTransition_InitializeBattleEffectState(WINDOW_TRANSITION_LEADER);
  // UB: HEAP_SLOT_SCREEN_TRANSITION lies past scalar gFieldWork. Its named load
  // adds four bytes in the native English trial (2,092 to 2,096 bytes).
#ifdef UBFIX
  state = gScreenTransitionWork;
#else
  state = (struct ScreenTransitionState *)((u8 **)&gFieldWork)[4];
#endif
  state->value = 0;
  state->mask = 0x1f1f;
  state->active = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  for (step = 1; step <= 5; step++) {
    wait(3);
    state->value = step;
  }
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(30);
  for (step = 5; step <= 31; step++) {
    wait(3);
    state->value = step;
  }
  state->active = 31;
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  work->transitionFrames = 24;
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_MIA, 0, -40);
  Actor_WalkByAndWait(ACTOR_MIA, 32, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_MIA, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->z.fixed -= PIXELS(3);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->targetZ -= PIXELS(3);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(-1, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_Jump(ACTOR_GARET, 4, 13);
    Actor_Jump(ACTOR_GARET, 4, 30);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_Jump(ACTOR_GARET, 4, 13);
    Actor_Jump(ACTOR_GARET, 4, 30);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(-1, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    Event_ShowMessage(ACTOR_GARET, 0);
    pick = 0;
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    gFieldWork->message++;
    pick = 1;
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  if (pick == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_MIA, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  // Wait for the player to leave the bed before the companions rejoin.
  while ((gKeysHeld & KEYS_DPAD) == 0) {
    wait(1);
  }
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_MIA);
  Scene_WalkIntoLeader(ACTOR_IVAN);
  Event_Wait(10);
  Event_End();
}

/// Arriving after Colosso, the party finds Iodem and Babi have been waiting for
/// it.
///
/// - Babi praises its fighting, and saw its Psynergy through his Cloak Ball.
/// - He tells of Lemuria and the draught that has kept him alive.
/// - The draught is running out, and he asks the party to sail his Lemurian
///   ship to find Lemuria.
/// - Learning that the party is bound for Venus Lighthouse, he sends Iodem with
///   it, to meet at Gondowan Passage.
static void BabiPalace_MeetBabiAfterColosso(void) {
  Sound_Play(MUS_BABI);
  Event_Start();
  Event_SetMessageId(MsgTolbiWaitingCompanions);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Event_Wait(10);
  // "I've been waiting for you and your companions, Isaac!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 16);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I've been waiting here because I wanted to speak with you."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  // "It's hard to talk when you're so far away... Come over here."
  Event_ShowMessage(ACTOR_IODEM, 0);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 760);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "The battles at Colosso were spectacular!"
  Event_ShowMessage(ACTOR_BABI, 0);

  // If the leader won the finals, Iodem brings the victor's prize, the Lure
  // Cap; otherwise Babi only praises how far the party came.
  if (GameFlag_Get(FLAG_WON_COLOSSO) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    // "It was most rewarding to see Babi's favored warrior steal..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_Wait(20);
    // "Iodem, bring him the victor's prize."
    Event_ShowMessage(ACTOR_BABI, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    // "Yes, sir, right away."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, 32);
    Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Actor_Get(ACTOR_IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkByAndWait(ACTOR_BABI, 0, 16);
    Event_Wait(40);
    MessageArgument_Push(ITEM_LURE_CAP, MESSAGE_ARGUMENT_ITEM);
    // "Isaac received ...."
    Event_ShowMessage(-1, 0);
    Item_ShowFound(ITEM_LURE_CAP, 3);
    Party_GiveItem(ITEM_LURE_CAP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_WalkByAndWait(ACTOR_BABI, 0, -16);
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_BABI, 2);
    Event_Wait(20);
    // "Incidentally, Isaac, are you feeling better now?"
    Event_OpenMessage(ACTOR_BABI, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 5;
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    // "It was wonderful to see Babi's handpicked warrior make it so..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_BABI, 2);
    Event_Wait(20);
    // "Incidentally, Isaac, are you feeling better now?"
    Event_OpenMessage(ACTOR_BABI, 0);
  }

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    // "Ah, the recuperative powers of youth...and after such fierce..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    gFieldWork->message++;
    // "You must be exhausted. I'm amazed to see you on your feet..."
    Event_ShowMessage(ACTOR_IODEM, 0);
  }

  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "Who could expect less from the warriors who rescued me?"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_Jump(ACTOR_GARET, 4, 13);
  Actor_Jump(ACTOR_GARET, 4, 30);
  // "Enough with the flattery. Let's get to the point: why were..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  wait(2);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  // "Such insolence! Who do you think you are, to speak to Babi so?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 30);
  // "Let it be, Iodem. There are other ways to judge a man."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_GARET, 0);
  Event_Wait(40);
  // "What is it you said you were called?"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
  // "He's talking to you, Garet."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "That's right, Garet. We shall explain all."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "We intended to thank you and your companions for saving..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "But it would seem that gratitude is not enough..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I have disclosed everything to Iodem."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What do you mean by everything?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "You do wish to know more about Lemuria, correct?"
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_Wait(30);
    // "Yes, indeed... Your desire is as plain as day."
    Event_ShowMessage(ACTOR_BABI, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldWork->message++;
    // "You can't hide your interest from me. It is written on your..."
    Event_ShowMessage(ACTOR_BABI, 0);
  }

  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "I have just seen Psynergy for the first time... It is an..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 60);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  // "I used my Cloak Ball to show him Psynergy."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "No one else could see it, but I knew Isaac's tournament was..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I thought it unlikely that you would fight using such power..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "No need to be ashamed, Isaac. Psynergy is also a warrior's..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "When I first learned of your power, I was shocked. I thought..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "You have mentioned Lemuria many times, Babi. What is it?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 50);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "It is the city of an ancient people, lying far out at sea."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "Many years ago, about the time of the great flood, Babi went..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "I see..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "The great flood struck while I was traveling with Lunpa."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Lunpa... You mean Lunpa the thief?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_IVAN, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I know it is hard to believe... It happened a hundred years ago."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "You mean...those rumors about you are...true?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "Kraden once mentioned the Stone of Sages. Do you think..."
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    // "So the stone that grants immortality came from Lemuria?"
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message += 3;
  } else {
    Event_Wait(10);
    gFieldWork->message++;
    // "Huh? Don't tell me you forgot about the Stone of Sages..."
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      // "It rules all, can change anything into gold, grants..."
      Event_ShowMessage(ACTOR_GARET, 0);
      gFieldWork->message++;
    } else {
      gFieldWork->message++;
      // "I can't think of anything else that would let him live for..."
      Event_ShowMessage(ACTOR_GARET, 0);
    }
  }

  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "So you've even heard of the Stone of Sages... I'm afraid I..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "It is the draught of Lemuria that has allowed me to live for..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "The draught of Lemuria?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "It is a magical draught once taken by those who lived in..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "And just drinking stops you from aging?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Is that what it does?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  // "Hmmm... It would be more accurate to say it slows the aging..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "There is death in Lemuria..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "But it only comes to those whose senses have failed from..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  // "Do the people of Lemuria use Psynergy, like we do?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "You have that draught, Babi... That's why you've lived so long."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_IVAN, 0);
  Event_Wait(50);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "And become wealthy enough to rule Tolbi."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What are you getting at?"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "What is your purpose in making everyone study Alchemy?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(20);
  // "I'm starting to wonder myself. What do you hope to gain, Babi?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "Ah. If I had attained true immortality... There would be no..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "I'm not sure I understand."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I had precious little draught, and now I am running out."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "You all saw it. There was but little left in the cave..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "How long will it last? And what will happen when it runs out?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "It will run out any day now. And when it does, my life is at..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Why didn't you tell us sooner? We must hurry to Lemuria!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 24);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_IODEM, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  // "It is pointless..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "What on earth do you mean, pointless?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "We cannot find Lemuria."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -24);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(20);
  // "But you just said that it lies far out at sea..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "If we search the seas, we're sure to find it!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I've sent so many ships in the past in search of Lemuria."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "This must have been long before I joined you... But still..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I needed more Lemurian draught, but I couldn't find Lemuria."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "Why did you collapse in the cave if you still had more draught?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I have been rationing it, but this time, I waited too long..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "You were trying to make it last longer..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I overexerted myself trying to find Lemuria before it ran out."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  // "And your reason for calling us here has to do with Lemuria?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "Exactly. I want you to find Lemuria."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "I have not been able to locate Lemuria by sea, but perhaps..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I can hide it no longer... I have built Babi Lighthouse to..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What? That's why you're building the lighthouse?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_IODEM, 30);
  // "No, there's more to it than that."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "We've spotted something in the ocean southeast of the..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "Something dangerous, no doubt. What, monsters? We can handle..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  // "That may be... However, something else troubles me more."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "What is it, my lord?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  // "No sailor has been able to keep his bearings at sea."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "You're kidding! You mean they can't even keep a straight course?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "Is there something that distorts distance or direction or..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "The currents shift there, subtly changing a ship's direction."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "So the lighthouse will help the ships keep their bearings..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "So, how exactly did you get away from Lemuria after the..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "That is the thing, Garet!"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I crossed the sea in a Lemurian ship. I am hoping you can..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "But why us, when you have so many powerful soldiers at your..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "None of them have the ability to use Psynergy."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "One must have Psynergy to sail this ship!"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Then that would mean that Babi is an Adept, wouldn't it?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_BABI, 0) == 0) {
    Event_Wait(10);
    // "That must be it. No normal person could use that Cloak Ball."
    Event_ShowMessage(ACTOR_IVAN, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    gFieldWork->message++;
    // "But, Isaac, no normal person could master the Cloak Ball!"
    Event_ShowMessage(ACTOR_IVAN, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "I learned to use a little Psynergy during my stay in Lemuria."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "With what little I'd learned, I was able to guide the ship."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(20);
  // "In that case, perhaps... I could sail the ship."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  // "No. It must be an Adept."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
  // "What should we do, Isaac? Will you do what he wants?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  // "If you do, I will grant you whatever you wish."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  // "You shouldn't make vows you will later find you cannot keep..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I haven't even considered my own wishes lately."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_IODEM, ACTOR_BABI, 60);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "In that case, I could provide you with a wealth of items you..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Babi's offer is very generous... You will consider going,..."
  Event_OpenMessage(ACTOR_IODEM, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Isaac, we have to beat Felix and the others to the lighthouse."
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    gFieldWork->message++;
    // "Not this time. We have to beat Felix and the others to the..."
    Event_ShowMessage(ACTOR_GARET, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "The lighthouse!?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Do you mean Venus Lighthouse?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Well then, our paths are intertwined. I, too, must go to..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "But we cannot get to the top of Venus Lighthouse, can we?"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "These Adepts may be able to solve the riddle that blocks us."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Ah, I see... The thought hadn't occurred to me."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "Isaac! You shall go to Venus Lighthouse with Iodem."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Do not worry... You can take care of your own business first."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  // "I see... If they can solve the riddle of the lighthouse......"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  // "So, we can search for Lemuria after they complete their own..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Is that acceptable?"
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "Ah, you will undertake this quest! I am most grateful!"
    Event_ShowMessage(ACTOR_BABI, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    gFieldWork->message++;
    // "If you need time to make up your mind, then please do so."
    Event_ShowMessage(ACTOR_BABI, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "In any case, you will need my assistance to reach the..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // "I'll make preparations so that I am ready to leave at any time."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  // "I take my leave now to make preparations."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  // "Then you can all meet at Gondowan Passage..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "That's a brilliant idea!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  // "One more thing. I'd like you to think about a suitable reward."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Yes. I will take care of that immediately, as well."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // The party tracks each departing speaker before gathering back together.
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_IODEM);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IODEM);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IODEM);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_IODEM);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 32);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  // "I shall be waiting for you at the passage."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Actor_WalkByAndWait(ACTOR_GARET, 16, 0);
  Actor_WalkBy(ACTOR_IODEM, 0, 80);
  Event_Wait(40);
  Actor_WalkByAndWait(ACTOR_GARET, -16, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_IODEM);
  Event_Wait(80);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 24);
  Actor_WalkByAndWait(ACTOR_BABI, -80, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(20);
  // "Your first task shall be the riddles of Venus Lighthouse. I..."
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -64, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_IVAN);
  Scene_WalkIntoLeader(ACTOR_MIA);
  Event_Wait(10);
  Scene_PlayMusic();
  Event_End();
}

/// Walking north into one of the palace's doors opens it (SE_DOOR_OPEN), and
/// the leader walks through.
///
/// The door's step trigger is the number of the exit it leads to. So the
/// triggers name each by its exit, and the subject is passed on as the exit.
///
/// The throne room's door walks the leader straight north. The others first
/// center the leader on the door.
static void BabiPalace_LeaveThroughDoor(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  Sound_Play(SE_DOOR_OPEN);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->stepTrigger == BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM) {
    TilesetAnim_Resume(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
    Event_Wait(10);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    if (work->stepTrigger == BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM) {
      TilesetAnim_Resume(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
      Event_Wait(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    } else {
      TilesetAnim_Resume(BABI_PALACE_OTHER_DOORS_ANIMATION);
      Event_Wait(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
  }
  Event_Wait(16);
  Import_EventSetExitRequest(work->stepTrigger);
  BabiPalace_CloseDoors();
  Event_End();
}

/// Walking north into the palace's door from outside opens it, and the leader
/// goes in.
static void BabiPalace_LeaveThroughAnimatedDoor(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gBabiPalace_DoorCells, 36, 10);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(BABI_PALACE_ENTRANCE_TO_BABI_PALACE);
  Event_End();
}

/// A guard outside the palace during the finals passes on Babi's word.
/// "You're Isaac, aren't you? Babi left word that he would be waiting for you
/// at the colosseum."
static void BabiPalace_PassOnBabisWord(void) {
  s32 question;
  GameFlag_Set(FLAG_BABI_PALACE_GUARD_GAVE_WORD);
  Event_Start();
  Field_EndBusyAndReveal();
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(20);
  question = MsgTolbiArent;
  Event_SetMessageId(question);
  // "You're Isaac, aren't you?"
  Event_OpenMessage(ACTOR_FIRST_GUARD, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Event_SetMessageId(question + 1);
    // "Babi left word that he would be waiting for you at the..."
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  } else {
    Event_Wait(20);
    Event_SetMessageId(question + 2);
    // "Oh, I guess I was mistaken..."
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 60);
    Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
    Event_Wait(30);
    Actor_FaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 30);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "No, I'm certain it was you."
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
    Event_Wait(20);
    // "Babi left word that he would be waiting for you at the..."
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  }
  Event_End();
}

/// The other guard thanks the party for saving Babi once, then sends it to
/// the colosseum.
static void BabiPalace_ThankForSavingBabi(void) {
  if (Event_GetFlag(FLAG_BABI_PALACE_GUARD_THANKED_PARTY) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_GUARD_THANKED_PARTY);
    Event_SetMessageId(MsgTolbiIsaacIdReallyLikeThank);
    // "Isaac, I'd really like to thank you for saving Babi. He is..."
    Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  } else {
    Event_SetMessageId(MsgTolbiBabiWaitingForAtColosseum);
    // "Babi is waiting for you at the colosseum. He wants you to go..."
    Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  }
}

/// A guard asks whether the party is leaving like the other warriors.
///
/// The guard stands outside the palace after Colosso, and means the other
/// warriors who stayed in the palace.
static void BabiPalace_AskAboutRemainingWarriors(s32 actorId) {
  s32 question = MsgTolbiWarriorsWhoStayed;

  Event_SetMessageId(question);
  // "All the warriors who stayed in the palace left when Colosso..."
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(question + 1);
    // "Oh, that's too bad. I bet we could have had a good time..."
    Event_ShowMessage(actorId, 0);
  } else {
    Event_SetMessageId(question + 2);
    // "Huh? I thought for sure you'd be going, too. I guess not..."
    Event_ShowMessage(actorId, 0);
  }
}

/// Searching the well outside the palace. "It's filled with fresh, clean
/// water."
static void BabiPalace_ExamineWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgTolbiItsFilledWithFreshClean, 1);
  Event_End();
}

/// Returns the triggers for the scene and the story, as for the actor
/// placements.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_BabiPalaceEntrance) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gBabiPalace_OutsideTriggersAfterColosso;
    }
    if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      return gBabiPalace_OutsideTriggersFinals;
    }
    return gBabiPalace_OutsideTriggersBabiMissing;
  }
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gBabiPalace_TriggersAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gBabiPalace_TriggersFinals;
  }
  return gBabiPalace_TriggersBabiMissing;
}

/// Sets up the scene being entered. Returns 0.
///
/// It sets up the doors, the Cloak Ball on show, and the events that play on
/// arrival: Babi after Colosso, the attendant during the finals, and waking
/// after a night's rest.
s32 BabiPalace_ApplyEntryState(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  u8 *buf;
  s32 set;

  BabiPalace_CloseDoors();
  // XXX (no evidence): what BABI_PALACE_AFTER_BABI_SAVED and
  // BABI_PALACE_AFTER_COLOSSO were made for; only the debug scene selector
  // reaches them, and nothing in the ROM says why they set these story flags.
  if (gMainRoutineWork.entrance == BABI_PALACE_AFTER_BABI_SAVED) {
    GameFlag_Set(FLAG_BABI_SAVED);
  }
  if (gMainRoutineWork.entrance == BABI_PALACE_AFTER_COLOSSO) {
    GameFlag_Set(FLAG_BABI_SAVED);
    GameFlag_Set(FLAG_COLOSSO_OVER);
  }
  if (gMainRoutineWork.next != (s32)&SceneId_BabiPalaceEntrance) {
    if (gMainRoutineWork.entrance == BABI_PALACE_FROM_COTTAGE_ROOMS_B) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }
    if (GameFlag_Get(FLAG_COLOSSO_OVER)) {
      set = GameFlag_Get(FLAG_BABI_PALACE_CLOAK_BALL);
      if (set) {
        Actor_SetPosition(ACTOR_CLOAK_BALL, 0, 0);
      } else {
        actor = Actor_Get(ACTOR_CLOAK_BALL);
        actor->retainSpriteCache = 1;
        actor->motionFlags = set;
        sprite = actor->sprite;
        actor->y.fixed = PIXELS(4);
        sprite->count = set;
        sprite->image.fullColor = 0;
        sprite->image.palette = 0;
        // The icon is drawn 1,024 bytes into the glyph buffer, as the 128 bytes
        // of a 16 by 16 sprite.
        buf = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 1544);
        Item_LoadIcon(ITEM_CLOAK_BALL);
        Vram_Load(sprite->slot, 128, buf + 1024);
        HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      }
      if (gMainRoutineWork.entrance ==
              BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM &&
          !Event_GetFlag(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
        GameFlag_Set(FLAG_BABI_SPOKE_AFTER_COLOSSO);
        Actor_SetPosition(ACTOR_BABI, PIXELS(208), PIXELS(704));
        BabiPalace_MeetBabiAfterColosso();
      }
      // After Colosso Babi lies in his bed, with no shadow
      // (https://alchemy.pascalpixel.com/places/babi-palace_colosso-over/map-18.png).
      Actor_SetAnimation(ACTOR_BABI, BABI_ANIMATION_IN_BED);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_BABI), 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) &&
               !GameFlag_Get(FLAG_BABI_PALACE_FINALS_REST_OFFERED)) {
      Actor_SetPosition(ACTOR_FINALS_ATTENDANT, PIXELS(120), PIXELS(72));
    }
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    // The attendant before the rescue and after Colosso; during the finals
    // ACTOR_FINALS_HURRYING_WARRIORS_ALONG has this number.
    Actor_Get(ACTOR_BED_ATTENDANT)->collisionFlags |= ACTOR_NEAR_TALK;
    if (gMainRoutineWork.entrance == BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      BabiPalace_WakeAfterFinals();
      // Only this entrance chooses the waking, so rewriting it makes a
      // later load of the scene from the main routine's work, as when a game
      // saved here is continued, an arrival from the statue room, not a second
      // waking.
      gMainRoutineWork.entrance = BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
    }
    if (gMainRoutineWork.entrance == BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
      Actor_SetPosition(ACTOR_FINALS_ATTENDANT, PIXELS(56), PIXELS(120));
      Actor_FaceDirection(ACTOR_FINALS_ATTENDANT, FACING_EAST_NORTHEAST, 0);
      BabiPalace_WakeFromFinalsDream();
      gMainRoutineWork.entrance = BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
    }
  }
  return 0;
}

// clang-format off
u16 gBabiPalace_DoorCells[] = {
    MAPGRID_ANIMATION_STEP(25, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(26, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(27, 28, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

s32 gBabiPalace_IndigestionScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gBabiPalace_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_FROM_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, 768, 0, 200, FACING_NORTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_WEST, CONDITION_ALWAYS, 728, 0, 88, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_EAST, CONDITION_ALWAYS, 808, 0, 88, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_WEST_STEPS, CONDITION_ALWAYS, 600, 0, 152, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_EAST_STEPS, CONDITION_ALWAYS, 936, 0, 152, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_WEST, CONDITION_ALWAYS, 584, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_EAST, CONDITION_ALWAYS, 952, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_EAST, CONDITION_ALWAYS, 440, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_SCHOLARS_LANDING_FROM_CARPET_ROOM, CONDITION_ALWAYS, 72, 0, 968, FACING_EAST, 0, 0, 864, 240, 1024, 0 },
    { BABI_PALACE_FROM_COTTAGE_ROOMS_B, CONDITION_ALWAYS, 168, 0, 936, FACING_SOUTH, 0, 0, 864, 240, 1024, 0 },
    { BABI_PALACE_TUNNEL_LANDING_FROM_CARPET_ROOM, CONDITION_ALWAYS, 424, 0, 680, FACING_WEST, 0, 328, 600, 568, 760, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_TUNNEL, CONDITION_ALWAYS, 472, 0, 696, FACING_SOUTH, 0, 328, 600, 568, 760, 0 },
    { BABI_PALACE_BEDROOM_LANDING_FROM_MEETING_ROOM, CONDITION_ALWAYS, 408, 0, 824, FACING_EAST, 0, 320, 728, 560, 888, 0 },
    { BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM, CONDITION_ALWAYS, 456, 0, 808, FACING_SOUTH, 0, 320, 728, 560, 888, 0 },
    { BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING, CONDITION_ALWAYS, 824, 0, 648, FACING_NORTH, 0, 768, 480, 1024, 672, 0 },
    { BABI_PALACE_WEST_STEPS_FROM_STATUE_ROOM, CONDITION_ALWAYS, 872, 0, 936, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { BABI_PALACE_WEST_STEPS_FROM_WEST_ROUND_ROOM, CONDITION_ALWAYS, 888, 0, 968, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { BABI_PALACE_EAST_STEPS_FROM_STATUE_ROOM, CONDITION_ALWAYS, 760, 0, 936, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { BABI_PALACE_EAST_STEPS_FROM_EAST_ROUND_ROOM, CONDITION_ALWAYS, 744, 0, 968, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_WEST, CONDITION_ALWAYS, 440, 0, 584, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_EAST, CONDITION_ALWAYS, 616, 0, 584, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_SCHOLARS_LANDING, CONDITION_ALWAYS, 328, 0, 312, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_TUNNEL_LANDING, CONDITION_ALWAYS, 728, 0, 312, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_NORTH_ROUND_ROOM, CONDITION_ALWAYS, 312, 0, 296, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_SHEBAS_LANDING, CONDITION_ALWAYS, 744, 0, 296, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_OVEN_ROOM, CONDITION_ALWAYS, 472, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_SOLDIERS_ROOM, CONDITION_ALWAYS, 584, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_MEETING_ROOM, CONDITION_ALWAYS, 528, 0, 296, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 208, 0, 840, FACING_NORTH, 0, 0, 624, 336, 864, 0 },
    { BABI_PALACE_MEETING_ROOM_FROM_BEDROOM_LANDING, CONDITION_ALWAYS, 40, 0, 760, FACING_WEST, 0, 0, 624, 336, 864, 0 },
    { BABI_PALACE_OVEN_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 200, 0, 360, FACING_NORTH, 0, 0, 240, 256, 480, 0 },
    { BABI_PALACE_SOLDIERS_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 824, 0, 360, FACING_NORTH, 0, 768, 240, 1024, 480, 0 },
    { BABI_PALACE_WEST_ROUND_ROOM_FROM_WEST_STEPS, CONDITION_ALWAYS, 376, 0, 936, FACING_EAST, 0, 232, 864, 472, 1024, 0 },
    { BABI_PALACE_EAST_ROUND_ROOM_FROM_EAST_STEPS, CONDITION_ALWAYS, 520, 0, 936, FACING_WEST, 0, 424, 864, 664, 1024, 0 },
    { BABI_PALACE_NORTH_ROUND_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 904, 0, 776, FACING_EAST, 0, 776, 704, 1016, 864, 0 },
    { BABI_PALACE_SHEBAS_LANDING_FROM_CARPET_ROOM, CONDITION_ALWAYS, 216, 0, 552, FACING_WEST, 0, 0, 472, 304, 632, 0 },
    { BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM, CONDITION_ALWAYS, 88, 0, 552, FACING_SOUTH, 0, 0, 472, 304, 632, 0 },
    { BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING, CONDITION_ALWAYS, 728, 0, 824, FACING_NORTH, 0, 552, 656, 792, 848, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gBabiPalace_OutsideEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 264, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 264, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_BABI_PALACE, CONDITION_ALWAYS, 200, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneExitRegion gBabiPalace_Regions[] = {
    { SCENE_TABLE_END },
};

u32 gBabiPalace_Exits[] = {
    SCENE_EXITS(BabiPalace),
    SCENE_EXIT(BABI_PALACE_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance, BABI_PALACE_ENTRANCE_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_WEST),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_EAST),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS, BabiPalace, BABI_PALACE_WEST_STEPS_FROM_STATUE_ROOM),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS, BabiPalace, BABI_PALACE_EAST_STEPS_FROM_STATUE_ROOM),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_EAST),
    SCENE_EXIT(BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_WEST),
    SCENE_EXIT(BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_EAST),
    SCENE_EXIT(BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SCHOLARS_LANDING),
    SCENE_EXIT(BABI_PALACE_TO_COTTAGE_ROOMS_B, CottageRoomsB, COTTAGE_ROOMS_B_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_TUNNEL_LANDING),
    SCENE_EXIT(BABI_PALACE_TO_COLOSSEUM_TUNNEL, ColosseumTunnel, COLOSSEUM_TUNNEL_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM, BabiPalace, BABI_PALACE_MEETING_ROOM_FROM_BEDROOM_LANDING),
    SCENE_EXIT(BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, BabiPalace, BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING),
    SCENE_EXIT(BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM),
    SCENE_EXIT(BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_WEST_STEPS),
    SCENE_EXIT(BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM, BabiPalace, BABI_PALACE_WEST_ROUND_ROOM_FROM_WEST_STEPS),
    SCENE_EXIT(BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_EAST_STEPS),
    SCENE_EXIT(BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM, BabiPalace, BABI_PALACE_EAST_ROUND_ROOM_FROM_EAST_STEPS),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_WEST),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_EAST),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING, BabiPalace, BABI_PALACE_SCHOLARS_LANDING_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING, BabiPalace, BABI_PALACE_TUNNEL_LANDING_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM, BabiPalace, BABI_PALACE_NORTH_ROUND_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, BabiPalace, BABI_PALACE_OVEN_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, BabiPalace, BABI_PALACE_SOLDIERS_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, BabiPalace, BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_MEETING_ROOM),
    SCENE_EXIT(BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_MEETING_ROOM),
    SCENE_EXIT(BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_OVEN_ROOM),
    SCENE_EXIT(BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SOLDIERS_ROOM),
    SCENE_EXIT(BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS, BabiPalace, BABI_PALACE_WEST_STEPS_FROM_WEST_ROUND_ROOM),
    SCENE_EXIT(BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS, BabiPalace, BABI_PALACE_EAST_STEPS_FROM_EAST_ROUND_ROOM),
    SCENE_EXIT(BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_NORTH_ROUND_ROOM),
    SCENE_EXIT(BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SHEBAS_LANDING),
    SCENE_EXIT(BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, BabiPalace, BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING),
    SCENE_EXIT(BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM),
    SCENE_EXITS(BabiPalaceEntrance),
    SCENE_EXIT(BABI_PALACE_ENTRANCE_TO_TOLBI, Tolbi, TOLBI_FROM_BABI_PALACE_ENTRANCE),
    SCENE_EXIT(BABI_PALACE_ENTRANCE_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_BABI_PALACE_ENTRANCE),
    SCENE_EXITS_END,
};

struct ActorPlacement gBabiPalace_OutsideActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBabiPalace_ActorPlacementsBabiMissing[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        752, 0, 136, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        56, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 96, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, (s32)gBabiPalace_IndigestionScript,
        472, 0, 136, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 296, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        888, 0, 344, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        944, 0, 384, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        920, 0, 560, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 728, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 744, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        392, 0, 360, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        392, 0, 352, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        392, 0, 368, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBabiPalace_ActorPlacementsFinals[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        712, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        56, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        920, 0, 560, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 744, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        600, 0, 744, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBabiPalace_ActorPlacementsAfterColosso[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        712, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        392, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        944, 0, 384, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        888, 0, 553, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        920, 0, 560, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 532, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        712, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, FLAG_BABI_SPOKE_AFTER_COLOSSO, MOVEMENT_TYPE_STAND,
        184, 0, 704, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gBabiPalace_TriggersBabiMissing[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { TRIGGER_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { TRIGGER_STEP, CELL_BEHAVIOR_BABI_PALACE_BESIDE_IODEM, FLAG_IODEM_HEARD_SOLDIERS, EVENT(BabiPalace_HearSoldiersReport) },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, EVENT(BabiPalace_AskAboutSoldiers) },
    { TRIGGER_TALK, ACTOR_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
    { TRIGGER_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, EVENT(BabiPalace_ShowBeds) },
    { TRIGGER_TALK, ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, EVENT(BabiPalace_AskAboutColosso) },
    { TRIGGER_TALK, ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, EVENT(BabiPalace_TalkToWarriorWithIndigestion) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
    { TRIGGER_TALK, ACTOR_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
    { TRIGGER_TALK, ACTOR_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
    { TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBA, CONDITION_ALWAYS, EVENT(BabiPalace_ReadShebasMind) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_BEDS_OPEN), EVENT(BabiPalace_RestInPalaceBeds) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

struct Trigger gBabiPalace_TriggersFinals[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { TRIGGER_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
    { TRIGGER_TALK, ACTOR_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
    { TRIGGER_TALK, ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, EVENT(BabiPalace_OfferRest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
    { TRIGGER_TALK, ACTOR_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
    { TRIGGER_TALK, ACTOR_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
    { TRIGGER_TALK, ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
    { TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_FINALS_BEDS_OPEN), EVENT(BabiPalace_RestInPalaceBeds) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

struct Trigger gBabiPalace_TriggersAfterColosso[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { TRIGGER_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughDoor) },
    { TRIGGER_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, EVENT(BabiPalace_AskAboutMeetingBabi) },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, EVENT(BabiPalace_AskAboutEasternShores) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
    { TRIGGER_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
    { TRIGGER_TALK, ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
    { TRIGGER_TALK, ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
    { TRIGGER_TALK, ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
    { TRIGGER_TALK, ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
    { TRIGGER_TALK, ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
    { TRIGGER_TALK, ACTOR_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
    { TRIGGER_TALK, ACTOR_CLOAK_BALL, CONDITION_ALWAYS, EVENT(BabiPalace_OfferCloakBall) },
    { TRIGGER_TALK, ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
    { TRIGGER_TALK, ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_BEDS, CONDITION_ALWAYS, EVENT(BabiPalace_RestInPalaceBeds) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGERS_END },
};

struct Trigger gBabiPalace_OutsideTriggersBabiMissing[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughAnimatedDoor) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT(BabiPalace_ExamineWell) },
    { TRIGGERS_END },
};

struct Trigger gBabiPalace_OutsideTriggersFinals[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughAnimatedDoor) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(BabiPalace_PassOnBabisWord) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(BabiPalace_PassOnBabisWord) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, EVENT(BabiPalace_ThankForSavingBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT(BabiPalace_ExamineWell) },
    { TRIGGERS_END },
};

struct Trigger gBabiPalace_OutsideTriggersAfterColosso[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT(BabiPalace_LeaveThroughAnimatedDoor) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, EVENT(BabiPalace_AskAboutRemainingWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT(BabiPalace_ExamineWell) },
    { TRIGGERS_END },
};
// clang-format on
