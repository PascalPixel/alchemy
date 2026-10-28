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
/// Each scene has three actor placement tables and three trigger tables,
/// and Scene_GetActorPlacements and Scene_GetTriggers choose between them by
/// two story flags: while Babi is missing, during the finals (FLAG_BABI_SAVED,
/// set when the party saves him in Altmiller Cave) and after Colosso
/// (FLAG_COLOSSO_OVER). The people inside stand in all three under other
/// actor ids, so each table has its own actor id enum.
///
/// While Babi is missing, the attendant shows the party four beds
/// (BabiPalace_ShowBeds), which a search then sleeps in
/// (BabiPalace_RestInPalaceBeds, with a talk the first night,
/// FLAG_BABI_PALACE_FIRST_NIGHT). Stepping beside Iodem
/// (CELL_BEHAVIOR_BABI_PALACE_BESIDE_IODEM) plays BabiPalace_HearSoldiersReport
/// once (FLAG_IODEM_HEARD_SOLDIERS, whose rows for Iodem and his soldiers place
/// no one once it is set), and Sheba notices the first Mind Read on her
/// (BabiPalace_ReadShebasMind, FLAG_SHEBA_NOTICED_MIND_READ).
///
/// During the finals the guards outside pass on Babi's word once
/// (BabiPalace_PassOnBabisWord, FLAG_BABI_PALACE_GUARD_GAVE_WORD) and the
/// attendant offers a rest (BabiPalace_OfferRest). The finals send the party
/// back here by two entrances, which BabiPalace_ApplyEntryState answers
/// by restoring the party and its inventory
/// (BabiPalace_RestorePartyAndInventory) and playing a waking in the big
/// bedroom: a lost first or second match by
/// BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH (BabiPalace_WakeFromFinalsDream),
/// the third match by BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH
/// (BabiPalace_WakeAfterFinals).
///
/// After Colosso, entering the throne room by
/// BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM the first time plays Babi's
/// audience (BabiPalace_MeetBabiAfterColosso, FLAG_BABI_SPOKE_AFTER_COLOSSO),
/// which gives the victor's prize when FLAG_WON_COLOSSO is set and ends with
/// Babi sending Iodem with the party, to meet at Gondowan Passage. The Cloak
/// Ball on show is an actor drawn from the item's icon until the party
/// takes it (BabiPalace_OfferCloakBall, FLAG_BABI_PALACE_CLOAK_BALL).

#include "adventure.h"
#include "heap.h"
#include "version.h"
#include "keystate.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/babi_palace.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
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
#include "minigame.h"
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
#include "songs.h"
#include "audio_engine_symbols.h"

/// The palace's actors are placed by one of three tables, as the story
/// goes on: before Babi is rescued
/// (gBabiPalace_ActorPlacementsBabiMissing), during the finals
/// (gBabiPalace_ActorPlacementsFinals) and after Colosso
/// (gBabiPalace_ActorPlacementsAfterColosso). Each numbers its rows from
/// ACTOR_FIRST_PLACED, with Garet, Ivan, Mia and Sheba in their own party
/// slots, and most of the people stand in all three under other numbers. The
/// palace's residents, three old men with canes and a young man with brown
/// hair, stand in all three: one sent Babi's soldiers to search the town, and
/// another tells warriors who are not entering Colosso that they may not stay.
/// The game names none of them.
///
/// Before the rescue: the attendant who shows the party its beds, Iodem and the
/// two soldiers he sent to Altmiller Cave; Sheba is held here until Colosso is
/// over.
enum BabiPalaceActorId {
  /// An old man with a cane, in the same room and at the same number in every
  /// table, and the one who sent Babi's soldiers to search the town: "Did you
  /// run into any of Babi's soldiers in Town?", and if not, "Hmph! I told them
  /// to search every nook and cranny of this town..."
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
  /// Four of Babi's soldiers, in one room; the first says "Only warriors who
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

/// During the finals: ACTOR_ASKING_ABOUT_BABIS_SOLDIERS keeps its number, and
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

/// After Colosso: Babi, and the Cloak Ball he keeps on show;
/// ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, ACTOR_BED_ATTENDANT and Iodem keep
/// their numbers.
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

/// The attendant has led the party to the beds; talking again sends the
/// attendant back.
/// ★ Named from the game: "We have just enough extra beds in this room for
/// everybody." (MsgTolbiWeHaveJustEnoughExtra)
enum { FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS = FLAG_SCENE_TEMP_00 };

/// The palace map's tileset animations that open its doors, for
/// TilesetAnim_Resume (play) and TilesetAnim_Pause (pause).
/// Each scene entry pauses them, so every door is drawn shut; the recording of
/// BabiPalace_LeaveThroughDoor shows one swing open
/// (https://alchemy.pascalpixel.com/places/babi-palace/events/BabiPalace_LeaveThroughDoor.gif).
enum BabiPalaceDoorAnimation {
  BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION = 1, ///< the throne room's door
  BABI_PALACE_OTHER_DOORS_ANIMATION = 2,       ///< every door without its own
  BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION = 4     ///< the oven room's door
};

/// The leader's animations in the big bedroom's bed, after the four shared
/// ones (enum ActorAnimation). The nod and the shake of the head answer
/// yes and no in BabiPalace_WakeAfterFinals.
enum BabiPalaceBedAnimation {
  ANIM_IN_BED_LYING = 31,
  /// Sitting up: the leader is raised by 3 pixels as it starts.
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

/// A companion walks into the leader and is taken off the map, as the party
/// closes up into one walking sprite at the end of an event.
static inline void BabiPalace_WalkIntoLeader(s32 member) {
  struct Actor *leader;

  Actor_SetAnimation(member, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_ResetAndSetPosition(member, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(member);
  Actor_SetPosition(member, 0, 0);
}

static inline void BabiPalace_CloseDoors(void) {
  TilesetAnim_Pause(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
  TilesetAnim_Pause(BABI_PALACE_OTHER_DOORS_ANIMATION);
  TilesetAnim_Pause(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
}

/// After a night in the palace's beds: the party's HP and PP are full,
/// Garet, Ivan and Mia rejoin, and the inventory Colosso held is returned.
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
  if (gAdventure.sceneId == (s32)&SceneId_BabiPalaceEntrance) {
    return gBabiPalace_OutsideEntrances;
  }
  return gBabiPalace_Entrances;
}

/// The palace has no exit regions; the table is empty.
const struct SceneExitRegion *BabiPalace_GetRegions(void) {
  return gBabiPalace_Regions;
}

/// Both scenes share one exit table.
const u32 *BabiPalace_GetExits(void) { return gBabiPalace_Exits; }

/// Returns the actor placements for the scene and the story: before Babi's
/// rescue, during the finals or after Colosso.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gAdventure.sceneId == (s32)&SceneId_BabiPalaceEntrance) {
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

/// Asks a yes or no question; the answers are the two messages after it.
static inline void BabiPalace_ShowQuestionAnswer(s32 actorId, s32 question) {
  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    Event_SetMessageId(question + 1);
  else
    Event_SetMessageId(question + 2);
  Event_ShowMessageAndWait(actorId, 0);
}

/// An old man with a cane asks whether the party ran into Babi's soldiers. He's
/// one of the palace's residents, and asks while Babi is missing whether the
/// party ran into them in town.
static void BabiPalace_AskAboutSoldiers(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiRunBabisSoldiers);
}

/// Talking to the attendant before Babi's rescue: the attendant leads the party
/// to the beds. "We have just enough extra beds in this room for everybody.
/// Please follow me." Talking again, the attendant asks whether the party will
/// sleep right away and goes back.
static void BabiPalace_ShowBeds(s32 actorId) {

  struct Actor *leader;
  s16 roundedFacing;
  s32 facingOrMessage;
  s32 facingBits;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & 0xc000;
  Event_Start();
  Field_EndBusyAndReveal();
  if (Event_GetFlag(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
    GameFlag_Clear(FLAG_BABI_PALACE_BEDS_OPEN);
    Event_SetMessageId(MsgTolbiWeHaveJustEnoughExtra);
    Event_ShowMessage(actorId, 0);
    Event_WaitFrames(10);
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
    Event_WaitFrames(20);
    facingOrMessage = MsgTolbiUseFourBeds;
    Event_SetMessageId(facingOrMessage);
    Event_OpenMessage(actorId, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_SetMessageId(facingOrMessage + 1);
      Event_ShowMessage(actorId, 0);
    } else {
      Event_SetMessageId(facingOrMessage + 2);
      Event_ShowMessage(actorId, 0);
    }
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(actorId, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
#if TBS_PATCH_3
    Actor_SetPosition(actorId, 56 << 16, 120 << 16);
#endif
  }
  Event_End();
}

/// Talking to the attendant during the finals: the first time, the attendant
/// asks whether the party came to rest before the match and steps aside;
/// afterwards, the attendant offers rest. "if you're tired, please feel free to
/// get some rest."
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
    Event_WaitFrames(20);
    question = MsgTolbiCameRestBefore;
    Event_SetMessageId(question);
    Event_OpenMessage(actorId, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_WaitFrames(10);
      Event_SetMessageId(question + 1);
    } else {
      Event_SetMessageId(question + 2);
    }
    Event_ShowMessage(actorId, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(actorId, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
  } else {
    Event_SetMessageId(MsgTolbiTiredFeelFreeRest);
    Event_OpenMessage(actorId, 0);
  }
  Event_End();
}

/// A warrior asks whether the party is entering Colosso. The warrior is in the
/// palace while Babi is missing, and either hurries the party to the trials or
/// asks whether it got hurt too.
static void BabiPalace_AskAboutColosso(s32 actorId) {
  s32 question = MsgTolbiPlanningEnterColosso;

  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(10);
    FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 0x28);
    Event_SetMessageId(question + 1);
  } else {
    Event_WaitFrames(10);
    FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x28);
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// A warrior with indigestion gives up on entering Colosso. The warrior is in
/// the palace while Babi is missing, tries to shrug it off, and doubles up.
static void BabiPalace_TalkToWarriorWithIndigestion(s32 actorId) {
  struct Actor *actor;
  const s32 *indigestionScript;

  actor = Actor_Get(actorId);
  Event_Start();
  indigestionScript = gBabiPalace_IndigestionScript;
  Actor_SetScript(actorId, indigestionScript);
  Event_SetMessageId(MsgTolbiCantFightBecauseLittleIndigestion);
  Event_ShowMessage(actorId, 0);
  Actor_Stop(actorId);
  actor->scaleY = 0x10000;
  actor->scaleX = 0x10000;
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_WaitFrames(60);
  Event_ShowMessage(actorId, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_WaitFrames(30);
  Actor_SetScript(actorId, indigestionScript);
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirection(actorId, FACING_NORTHEAST, 0);
  Event_WaitFrames(10);
  actor->scaleY = 0x10000;
  actor->scaleX = 0x10000;
  Actor_SetScript(actorId, indigestionScript);
  Event_End();
}

/// Mind Read on Sheba in the palace, while Babi is missing: she cannot escape
/// to Lalivero alone; the first time, she feels it, ends the Psynergy and
/// asks about it. "Did you just..."
static void BabiPalace_ReadShebasMind(s32 casterId, s32 actorId) {
  Event_Start();
  Event_SetMessageId(MsgTolbiEvenIfEscapedBabiPalace);
  Event_ShowMessage(actorId, 0);
  if (Event_GetFlag(FLAG_SHEBA_NOTICED_MIND_READ) == 0) {
    GameFlag_Set(FLAG_SHEBA_NOTICED_MIND_READ);
    Psynergy_Cancel();
    Event_WaitFrames(50);
    Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    Actor_FaceActor(actorId, ACTOR_PARTY_LEADER, 40);
    Event_ShowMessage(actorId, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(actorId, FACING_WEST, 0);
  }
  Event_End();
}

/// The same old man, after Colosso, asks whether the party has met with Babi.
static void BabiPalace_AskAboutMeetingBabi(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiMeetBabi);
}

/// A young man asks whether the party is going back east already. He has brown
/// hair and is one of the palace's residents, and asks after Colosso whether
/// the party wants to go back already to the eastern shores of the Karagol Sea.
static void BabiPalace_AskAboutEasternShores(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiEasternShoresKaragol);
}

/// Taking the Cloak Ball on show after Colosso: Babi asks whether the party
/// wants it, and lends it for the quest if it does.
static void BabiPalace_OfferCloakBall(void) {
  s32 msg;

  Event_Start();
  Field_EndBusyAndReveal();
  msg = MsgTolbiFoundCloakBall;
  Event_SetMessageId(msg);
  Event_ShowMessageAndWait(-1, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(30);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_BABI, 30);
  FieldMessage_ShowAtActor(ACTOR_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Event_SetMessageId(msg + 2);
    Event_ShowMessageAndWait(ACTOR_BABI, 0);
  } else {
    Event_WaitFrames(20);
    Event_SetMessageId(msg + 3);
    Event_ShowMessageAndWait(ACTOR_BABI, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(30);
    Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Event_WaitFrames(30);
    Import_ActorSetPosition(ACTOR_CLOAK_BALL, 0, 0);
    ItemDisplay_ShowItem(ITEM_CLOAK_BALL, 3);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    PartyInventory_GiveItem(ITEM_CLOAK_BALL, 0);
    GameFlag_Set(FLAG_BABI_PALACE_CLOAK_BALL);
  }
}

/// Waking after resting before the finals: the leader dreamt of fighting in
/// them, the others tease, and the party sets off for the colosseum.
static void BabiPalace_WakeFromFinalsDream(void) {
  struct Actor *leader;

  Event_Start();
  Event_SetMessageId(MsgTolbiGetUp);
  Actor_Get(ACTOR_PARTY_LEADER)->spriteStorageKind = 0;
  Actor_Get(ACTOR_FINALS_ATTENDANT)->spriteStorageKind = 0;
  WaitFrames(1);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
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
  Event_WaitFrames(20);
  gFieldWork->transitionFrames = 24;
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_MIA, 16, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->z.fixed += -0x30000;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->targetZ += -0x30000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
  Event_WaitFrames(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
  Event_WaitFrames(50);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(45);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(-1, 0) != 0) {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message += 1;
  } else {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    gFieldWork->message += 2;
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_GARET, -16, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_WaitFrames(35);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  BabiPalace_WalkIntoLeader(ACTOR_GARET);
  BabiPalace_WalkIntoLeader(ACTOR_MIA);
  BabiPalace_WalkIntoLeader(ACTOR_IVAN);
  Event_WaitFrames(10);
  Event_End();
}

/// Searching the beds: the party goes to bed, talking it over the first night.
/// "Would you like to sleep now?"
static void BabiPalace_RestInPalaceBeds(void) {
  struct Actor *leader;
  s16 roundedFacing;
  u16 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & -0x4000;
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgTolbiLikeSleep);
  Event_OpenMessage(-1, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
  } else {
    if (Event_GetFlag(FLAG_BABI_PALACE_FIRST_NIGHT) != 0) {
      Event_WaitFrames(20);
      Inn_PlaySleep(0);
      return;
    }
    Event_WaitFrames(20);
    facing = roundedFacing;
    if (facing == FACING_EAST) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
    }
    if (facing == FACING_WEST) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 240, 120);
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 120);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_WaitFrames(10);
    FieldActor_LaunchFromLeader(ACTOR_GARET, 16, 0, FACING_WEST);
    Actor_RefreshSelectorById(ACTOR_GARET);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_GARET, 40, 0);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -32);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_WaitFrames(20);
    FieldActor_LaunchFromLeader(ACTOR_IVAN, -16, 0, FACING_EAST);
    Actor_RefreshSelectorById(ACTOR_IVAN);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_IVAN, -40, 0);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, 40);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_WaitFrames(20);
    FieldActor_LaunchFromLeader(ACTOR_MIA, 16, 0, FACING_WEST);
    Actor_RefreshSelectorById(ACTOR_MIA);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_MIA, 40, 0);
    Actor_WalkByAndWait(ACTOR_MIA, 0, 40);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    Event_WaitFrames(20);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -56, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -32);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_WaitFrames(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(ACTOR_GARET, 0);
    } else {
      gFieldWork->message++;
    }
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_WaitFrames(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(ACTOR_IVAN, 0);
    } else {
      gFieldWork->message++;
    }
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
    Inn_PlaySleep(0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_WaitFrames(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(ACTOR_IVAN, 0);
    } else {
      gFieldWork->message++;
    }
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_WaitFrames(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(ACTOR_MIA, 0);
    } else {
      gFieldWork->message++;
    }
    Actor_WalkByAndWait(ACTOR_GARET, 0, 32);
    Actor_WalkBy(ACTOR_GARET, -112, 0);
    Actor_WalkByAndWait(ACTOR_MIA, 0, -40);
    Actor_WalkBy(ACTOR_MIA, -112, 0);
    Event_WaitFrames(50);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, -24);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -16);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
    BabiPalace_WalkIntoLeader(ACTOR_GARET);
    BabiPalace_WalkIntoLeader(ACTOR_MIA);
    BabiPalace_WalkIntoLeader(ACTOR_IVAN);
    GameFlag_Set(FLAG_BABI_PALACE_FIRST_NIGHT);
  }
  Event_End();
}

/// Walking past Iodem while Babi is missing: his two soldiers report back
/// from Altmiller Cave without Babi, and Iodem tells how Babi has vanished
/// into the cave before; then they leave.
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
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_FIRST_SOLDIER, 528, 352);
  Actor_WalkToAndWait(ACTOR_SECOND_SOLDIER, 528, 368);
  WaitFrames(3);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Event_WaitFrames(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 8, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 8, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAttachedEffect(ACTOR_FIRST_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SECOND_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(40);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTH, 0);
  Event_WaitFrames(40);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(13) / 10, PIXELS(13) / 20);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(13) / 10, PIXELS(13) / 20);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 120);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 0, 120);
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, 0, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(40);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IODEM, 120, 0);
  Actor_WalkByAndWait(ACTOR_IODEM, 60, 0);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_End();
}

/// Waking in the palace after the finals: the leader slept long after
/// fighting to the end; Babi wants to see the party.
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
  WaitFrames(1);
  ScreenTransition_InitializeBattleEffectState(WINDOW_TRANSITION_LEADER);
  // UB: HEAP_SLOT_SCREEN_TRANSITION lies past scalar gFieldWork. Its named load
  // adds four bytes in the native English trial (0x82c to 0x830).
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
    WaitFrames(3);
    state->value = step;
  }
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(30);
  for (step = 5; step <= 31; step++) {
    WaitFrames(3);
    state->value = step;
  }
  state->active = 31;
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  work->transitionFrames = 24;
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_MIA, 0, -40);
  Actor_WalkByAndWait(ACTOR_MIA, 32, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_WaitFrames(10);
  Event_ShowMessage(ACTOR_MIA, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->z.fixed -= 0x30000;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->targetZ -= 0x30000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
  Event_WaitFrames(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(-1, 0) == 0) {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_Jump(ACTOR_GARET, 4, 13);
    Actor_Jump(ACTOR_GARET, 4, 30);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Actor_Jump(ACTOR_GARET, 4, 13);
    Actor_Jump(ACTOR_GARET, 4, 30);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Event_WaitFrames(50);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_WaitFrames(35);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(-1, 0) == 0) {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    Event_ShowMessage(ACTOR_GARET, 0);
    pick = 0;
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    gFieldWork->message++;
    pick = 1;
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Event_WaitFrames(50);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_WaitFrames(35);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  if (pick == 0) {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_MIA, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  // Wait for the player to leave the bed before the companions rejoin.
  while ((gKeysHeld & KEYS_DPAD) == 0) {
    WaitFrames(1);
  }
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  BabiPalace_WalkIntoLeader(ACTOR_GARET);
  BabiPalace_WalkIntoLeader(ACTOR_MIA);
  BabiPalace_WalkIntoLeader(ACTOR_IVAN);
  Event_WaitFrames(10);
  Event_End();
}

/// Arriving after Colosso: Iodem and Babi have been waiting for the party.
/// Babi praises its fighting, saw its Psynergy through his Cloak Ball, and
/// tells of Lemuria and the draught that has kept him alive; it is running
/// out, and he asks the party to sail his Lemurian ship to find Lemuria.
/// Learning that the party is bound for Venus Lighthouse, he sends Iodem
/// with it, to meet at Gondowan Passage.
static void BabiPalace_MeetBabiAfterColosso(void) {
  Event_PlaySong(MUS_BABI);
  Event_Start();
  Event_SetMessageId(MsgTolbiWaitingCompanions);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldCamera_WaitForMove();
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 16);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForMove();
  Event_WaitFrames(40);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 0x2f8);
  Event_WaitFrames(10);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);

  // If the leader won the finals, Iodem brings the victor's prize, the Lure
  // Cap; otherwise Babi only praises how far the party came.
  if (GameFlag_Get(FLAG_WON_COLOSSO) != 0) {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_BABI, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Event_WaitFrames(40);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(40);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    Event_WaitFrames(20);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, 32);
    Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Actor_Get(ACTOR_IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_WaitFrames(40);
    Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkByAndWait(ACTOR_BABI, 0, 16);
    Event_WaitFrames(40);
    MessageArgument_Push(ITEM_LURE_CAP, 2);
    Event_ShowMessage(-1, 0);
    Item_ShowFound(ITEM_LURE_CAP, 3);
    Party_GiveItem(ITEM_LURE_CAP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_WaitFrames(30);
    Actor_WalkByAndWait(ACTOR_BABI, 0, -16);
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_BABI, 2);
    Event_WaitFrames(20);
    Event_OpenMessage(ACTOR_BABI, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 5;
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
    Event_WaitFrames(40);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_BABI, 2);
    Event_WaitFrames(20);
    Event_OpenMessage(ACTOR_BABI, 0);
  }

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }

  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_Jump(ACTOR_GARET, 4, 13);
  Actor_Jump(ACTOR_GARET, 4, 30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  WaitFrames(2);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(30);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_GARET, 0);
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(30);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_BABI, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_BABI, 0);
  }

  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(50);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 60);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 50);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(50);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_IVAN, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(50);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message += 3;
  } else {
    Event_WaitFrames(10);
    gFieldWork->message++;
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_WaitFrames(20);
      Event_ShowMessage(ACTOR_GARET, 0);
      gFieldWork->message++;
    } else {
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_GARET, 0);
    }
  }

  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_IVAN, 0);
  Event_WaitFrames(50);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 24);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_IODEM, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -24);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_BABI, ACTOR_IODEM, 30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_BABI, 0) == 0) {
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_IVAN, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_IVAN, 0);
  }

  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_IODEM, ACTOR_BABI, 60);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_IODEM, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_GARET, 0);
  }

  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Event_ShowMessage(ACTOR_BABI, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(10);
    Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_BABI, 0);
  }

  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  // The party tracks each departing speaker before gathering back together.
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_IODEM);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IODEM);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IODEM);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_IODEM);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 32);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_WalkByAndWait(ACTOR_GARET, 16, 0);
  Actor_WalkBy(ACTOR_IODEM, 0, 80);
  Event_WaitFrames(40);
  Actor_WalkByAndWait(ACTOR_GARET, -16, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_IODEM);
  Event_WaitFrames(80);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 24);
  Actor_WalkByAndWait(ACTOR_BABI, -80, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -64, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Event_WaitFrames(20);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_WaitFrames(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  BabiPalace_WalkIntoLeader(ACTOR_GARET);
  BabiPalace_WalkIntoLeader(ACTOR_IVAN);
  BabiPalace_WalkIntoLeader(ACTOR_MIA);
  Event_WaitFrames(10);
  Scene_PlayMusic();
  Event_End();
}

/// Walking north into one of the palace's doors: it opens (SE_DOOR_OPEN) and
/// the leader walks through. The door's step trigger is the number of the exit
/// it leads to, so the triggers name each by its exit and the subject is
/// passed on as the exit. The throne room's door walks the leader straight
/// north; the others first center the leader on the door.
static void BabiPalace_LeaveThroughDoor(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  Event_PlaySong(SE_DOOR_OPEN);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->stepTrigger == BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM) {
    TilesetAnim_Resume(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
    Event_WaitFrames(10);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    if (work->stepTrigger == BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM) {
      TilesetAnim_Resume(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
      Event_WaitFrames(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    } else {
      TilesetAnim_Resume(BABI_PALACE_OTHER_DOORS_ANIMATION);
      Event_WaitFrames(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
  }
  Event_WaitFrames(16);
  Import_EventSetExitRequest(work->stepTrigger);
  BabiPalace_CloseDoors();
  Event_End();
}

/// Walking north into the palace's door from outside: it opens and the leader
/// goes in.
static void BabiPalace_LeaveThroughAnimatedDoor(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Event_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gBabiPalace_DoorCells, 36, 10);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_WaitFrames(16);
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
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_WaitFrames(20);
  question = MsgTolbiArent;
  Event_SetMessageId(question);
  Event_OpenMessage(ACTOR_FIRST_GUARD, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Event_SetMessageId(question + 1);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  } else {
    Event_WaitFrames(20);
    Event_SetMessageId(question + 2);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
    Event_WaitFrames(20);
    Actor_FaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 60);
    Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_WaitFrames(40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
    Event_WaitFrames(30);
    Actor_FaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 30);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
    Event_WaitFrames(20);
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
    Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  } else {
    Event_SetMessageId(MsgTolbiBabiWaitingForAtColosseum);
    Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  }
}

/// A guard asks whether the party is leaving like the other warriors. The guard
/// stands outside the palace after Colosso, and means the other warriors who
/// stayed in the palace.
static void BabiPalace_AskAboutRemainingWarriors(s32 actorId) {
  s32 question = MsgTolbiWarriorsWhoStayed;

  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(question + 1);
    Event_ShowMessageAndWait(actorId, 0);
  } else {
    Event_SetMessageId(question + 2);
    Event_ShowMessageAndWait(actorId, 0);
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
  if (gAdventure.sceneId == (s32)&SceneId_BabiPalaceEntrance) {
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

/// Sets up the scene being entered: the doors, the Cloak Ball on show, and
/// the events that play on arrival (Babi after Colosso, the attendant during
/// the finals, waking after a night's rest). Returns 0.
s32 BabiPalace_ApplyEntryState(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  u8 *buf;
  s32 set;

  BabiPalace_CloseDoors();
  // XXX (no evidence): what BABI_PALACE_AFTER_BABI_SAVED and
  // BABI_PALACE_AFTER_COLOSSO were made for; only the debug scene selector
  // reaches them, and nothing in the ROM says why they set these story flags.
  if (gAdventure.entrance == BABI_PALACE_AFTER_BABI_SAVED) {
    GameFlag_Set(FLAG_BABI_SAVED);
  }
  if (gAdventure.entrance == BABI_PALACE_AFTER_COLOSSO) {
    GameFlag_Set(FLAG_BABI_SAVED);
    GameFlag_Set(FLAG_COLOSSO_OVER);
  }
  if (gAdventure.sceneId != (s32)&SceneId_BabiPalaceEntrance) {
    if (gAdventure.entrance == BABI_PALACE_FROM_COTTAGE_ROOMS_B) {
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
        // The icon is drawn 0x400 bytes into the glyph buffer, as the 128 bytes
        // of a 16 by 16 sprite.
        buf = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
        Item_LoadIcon(ITEM_CLOAK_BALL);
        Vram_Load(sprite->slot, 128, buf + 0x400);
        HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      }
      if (gAdventure.entrance == BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM &&
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
    if (gAdventure.entrance == BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      BabiPalace_WakeAfterFinals();
      // Only this entrance chooses the waking, so rewriting it makes a
      // later load of the scene from the adventure, as when a game saved
      // here is continued, an arrival from the statue room, not a second
      // waking.
      gAdventure.entrance = BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
    }
    if (gAdventure.entrance == BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
      Actor_SetPosition(ACTOR_FINALS_ATTENDANT, PIXELS(56), PIXELS(120));
      Actor_FaceDirection(ACTOR_FINALS_ATTENDANT,
                          FACING_NORTHEAST + FACING_STEP, 0);
      BabiPalace_WakeFromFinalsDream();
      gAdventure.entrance = BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
    }
  }
  return 0;
}

// clang-format off
u16 gBabiPalace_DoorCells[] = {
    MAPGRID_ANIMATION_STEP(25, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(26, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(27, 28, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

s32 gBabiPalace_IndigestionScript[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
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
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gBabiPalace_ActorPlacementsBabiMissing[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(96), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, (s32)gBabiPalace_IndigestionScript,
        PIXELS(472), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(888), 0, PIXELS(344), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(984), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(728), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_IODEM, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(352), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(368), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gBabiPalace_ActorPlacementsFinals[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gBabiPalace_ActorPlacementsAfterColosso[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(392), 0, PIXELS(112), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(888), 0, PIXELS(553), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(532), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(712), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IODEM, FLAG_BABI_SPOKE_AFTER_COLOSSO, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(704), FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(BabiPalace_PassOnBabisWord) },
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
