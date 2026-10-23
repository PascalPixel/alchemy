/// @file
/// Babi's Palace in Tolbi, two maps under the area name Babi's Palace that
/// share this overlay and one warp table:
///
/// - MapId_BabiPalaceEntrance, the court between the town and the palace
///   door, with two guards and a well. Its door opens by a cell animation
///   (gBabiPalace_DoorCells, BabiPalace_LeaveThroughAnimatedDoor).
/// - MapId_BabiPalace, the palace's rooms on one map, joined by warps into
///   the same map and by doors that open by the map's tileset animations
///   (enum BabiPalaceDoorAnimation, BabiPalace_LeaveThroughDoor). One door
///   leads to Babi's scholars (BABI_PALACE_TO_VALE_INTERIORS_INN, held by
///   vale_interiors_inn.c) and one into the Colosseum Tunnel
///   (BABI_PALACE_TO_COLOSSEUM_TUNNEL).
///
/// Each map has three object event template tables and three map event tables,
/// and Map_GetObjectEventTemplates and Map_GetEvents choose between them by two
/// story flags: while Babi is missing, during the finals (FLAG_BABI_SAVED, set
/// when the party saves him in Altmiller Cave) and after Colosso
/// (FLAG_COLOSSO_OVER). The people inside stand in all three under other local
/// ids, so each table has its own local id enum.
///
/// While Babi is missing, the attendant shows the party four beds
/// (BabiPalace_ShowBeds), which a search then sleeps in
/// (BabiPalace_RestInPalaceBeds, with a talk the first night,
/// FLAG_BABI_PALACE_FIRST_NIGHT). Stepping beside Iodem
/// (MB_BABI_PALACE_BESIDE_IODEM) plays BabiPalace_HearSoldiersReport once
/// (FLAG_IODEM_HEARD_SOLDIERS, whose rows for Iodem and his soldiers place no
/// one once it is set), and Sheba notices the first Mind Read on her
/// (BabiPalace_ReadShebasMind, FLAG_SHEBA_NOTICED_MIND_READ).
///
/// During the finals the guards outside pass on Babi's word once
/// (BabiPalace_PassOnBabisWord, FLAG_BABI_PALACE_GUARD_GAVE_WORD) and the
/// attendant offers a rest (BabiPalace_OfferRest). The finals send the party
/// back here by two warp destinations, which BabiPalace_ApplyEntryState answers
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
/// Ball on show is an object event drawn from the item's icon until the party
/// takes it (BabiPalace_OfferCloakBall, FLAG_BABI_PALACE_CLOAK_BALL).
///
/// Open questions:
/// - XXX (no evidence): what BABI_PALACE_AFTER_BABI_SAVED and
///   BABI_PALACE_AFTER_COLOSSO were made for; only the debug map selector,
///   which enters any map by warp destination number, reaches them, and nothing
///   in the ROM says why they set the story flags they set.

#include "game_state.h"
#include "heap.h"
#include "version.h"
#include "keystate.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/babi_palace.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
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
#include "field_service.h"
#include "map_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "script_layer.h"
#include "item_ids.h"
#include "objcmd.h"
#include "heap_slots.h"
#include "display_transition.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// The palace's object events are placed by one of three tables, as the story
/// goes on: before Babi is rescued
/// (gBabiPalace_ObjectEventTemplatesBabiMissing), during the finals
/// (gBabiPalace_ObjectEventTemplatesFinals) and after Colosso
/// (gBabiPalace_ObjectEventTemplatesAfterColosso). Each numbers its rows from
/// LOCALID_FIRST_PLACED, with Garet, Ivan, Mia and Sheba in their own party
/// slots, and most of the people stand in all three under other numbers. The
/// palace's residents, three old men with canes and a young man with brown
/// hair, stand in all three: one sent Babi's soldiers to search the town, and
/// another tells warriors who are not entering Colosso that they may not stay.
/// The game names none of them.
///
/// Before the rescue: the attendant who shows the party its beds, Iodem and the
/// two soldiers he sent to Altmiller Cave; Sheba is held here until Colosso is
/// over.
enum BabiPalaceLocalId {
  /// An old man with a cane, in the same room and at the same number in every
  /// table, and the one who sent Babi's soldiers to search the town: "Did you
  /// run into any of Babi's soldiers in Town?", and if not, "Hmph! I told them
  /// to search every nook and cranny of this town..."
  /// ★ Named from the game: "Did you run into any of Babi's soldiers in Town?"
  /// (MsgTolbiRunBabisSoldiers)
  LOCALID_ASKING_ABOUT_BABIS_SOLDIERS = LOCALID_FIRST_PLACED,
  /// The attendant, a woman with fair hair, whose number is the same after
  /// Colosso: "No one's going to sleep in this big bedroom until the next
  /// Colosso!"
  /// ★ Named from the game: "We have just enough extra beds in this room for
  /// everybody." (MsgTolbiWeHaveJustEnoughExtra)
  LOCALID_BED_ATTENDANT,
  /// A warrior in a golden helmet. "So, did you guys get injured, too?"
  /// Thoughts: "I hurt my back during my training, and now I can't move at
  /// all."
  /// ★ Named from the game: "I hurt my back during my training"
  /// (MsgBabiPalaceIGetSoAngrySeeingAll)
  LOCALID_INJURED_WARRIOR,
  /// A warrior in a golden helmet. "I can't fight because of a little
  /// indigestion? That's pathetic!"
  /// ★ Named from the game: "Maybe I shouldn't have eaten quite so much
  /// yesterday..." (MsgBabiPalaceMaybeIShouldntHaveEatenQuite)
  LOCALID_WARRIOR_WITH_INDIGESTION,
  /// A young man with brown hair in a purple shirt. "If you're entering
  /// Colosso, hurry along to the colosseum." Thoughts: "Warriors are not
  /// allowed to stay in the palace if they don't enter Colosso!"
  /// ★ Named from the game: "If you're entering Colosso, hurry along to the
  /// colosseum." (MsgBabiPalaceIfYoureEnteringColossoHurryAlong)
  LOCALID_HURRYING_WARRIORS_ALONG,
  /// Four of Babi's soldiers, in one room; the first says "Only warriors who
  /// fight well in Colosso's finals can become soldiers here."
  /// ★ Named from the game: "Only warriors who fight well in Colosso's finals
  /// can become soldiers here." (MsgBabiPalaceOnlyWarriorsWhoFightWellIn)
  LOCALID_FIRST_PALACE_SOLDIER,
  /// An old man with a cane beside Babi's bed. "It looks like Lord Babi didn't
  /// use his bed this morning."
  /// ★ Named from what the player sees: an old man with a cane beside Babi's
  /// bed (its render)
  LOCALID_BY_BABIS_BED = LOCALID_FIRST_PALACE_SOLDIER + 4,
  /// An old man with a cane. "Sheba has been staying as Lord Babi's guest for
  /// some time." Thoughts: "It is heartbreaking to see her tears and hear her
  /// pleas to be released."
  /// ★ Named from the game: "It is heartbreaking to see her tears"
  /// (MsgBabiPalaceActorWasTakenHostageToForce)
  LOCALID_SORRY_FOR_SHEBA,
  /// In Sheba's room, in SPRITE_OLD_WOMAN, as in vale_gate.c. Thoughts: "Sheba
  /// is not permitted to leave this room."
  /// ★ Named from what the player sees: a white-haired old woman in Sheba's
  /// room (its render)
  LOCALID_OLD_WOMAN_IN_SHEBAS_ROOM,
  /// ★ Named from the game: "Iodem, Iodem!" (MsgTolbiIodemIodem)
  LOCALID_IODEM,
  /// ★ Named from what the player sees: a soldier in a helmet with a spear,
  /// SPRITE_SOLDIER (its render)
  LOCALID_FIRST_SOLDIER,
  /// ★ Named from what the player sees: the second soldier, SPRITE_SOLDIER
  /// (its render)
  LOCALID_SECOND_SOLDIER
};

/// During the finals: LOCALID_ASKING_ABOUT_BABIS_SOLDIERS keeps its number, and
/// the others stand under new ones.
enum BabiPalaceFinalsLocalId {
  /// The same resident as LOCALID_HURRYING_WARRIORS_ALONG: "What are you doing?
  /// Hurry over to the colosseum!"
  /// ★ Named from the game: "Hurry over to the colosseum!"
  /// (MsgBabiPalaceWhatAreYouDoingHurryOver)
  LOCALID_FINALS_HURRYING_WARRIORS_ALONG = LOCALID_FIRST_PLACED + 1,
  /// The same attendant, a number later.
  /// ★ Named from the game: "Would you like to sleep right now?"
  /// (MsgTolbiCameRestBefore)
  LOCALID_FINALS_ATTENDANT,
  /// The same resident as LOCALID_BY_BABIS_BED: "Babi has returned and the
  /// finals are set to begin!"
  /// ★ Named from the game: "Babi has returned and the finals are set to
  /// begin!" (MsgBabiPalaceBabiHasReturnedAndTheFinals)
  LOCALID_FINALS_BY_BABIS_BED,
  /// The same resident as LOCALID_SORRY_FOR_SHEBA: "What's going on? Sheba is
  /// looking much happier."
  /// ★ Named from the game: "I wonder what happened to make that girl smile."
  /// (MsgBabiPalaceWhatsGoingOnActorIsLooking)
  LOCALID_FINALS_SORRY_FOR_SHEBA,
  /// The same woman as LOCALID_OLD_WOMAN_IN_SHEBAS_ROOM: "The soldiers came to
  /// see Sheba. She's been in a great mood ever since."
  /// ★ Named from the game: "She's been in a great mood ever since."
  /// (MsgBabiPalaceTheSoldiersCameToSeeActor)
  LOCALID_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM
};

/// After Colosso: Babi, and the Cloak Ball he keeps on show;
/// LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, LOCALID_BED_ATTENDANT and Iodem keep
/// their numbers.
enum BabiPalaceAfterColossoLocalId {
  /// One of Babi's soldiers.
  /// ★ Named from the game: "We're relieved everything went so well."
  /// (MsgBabiPalaceBabiWasFoundSafeAndSound)
  LOCALID_RELIEVED_SOLDIER = LOCALID_FIRST_PLACED + 2,
  /// The same resident as LOCALID_HURRYING_WARRIORS_ALONG: "So, you're all from
  /// the eastern shores of the Karagol Sea, huh?"
  /// ★ Named from the game: "So, you're all from the eastern shores of the
  /// Karagol Sea, huh?" (MsgTolbiEasternShoresKaragol)
  LOCALID_ASKING_ABOUT_THE_EASTERN_SHORES,
  /// One of Babi's soldiers.
  /// ★ Named from the game: "Now that Colosso is over, I'm going to Gondowan to
  /// build Babi Lighthouse!" (MsgBabiPalaceNowThatColossoIsOverIm)
  LOCALID_GONDOWAN_BOUND_SOLDIER,
  /// "The southern continent is a dangerous place... you'll know why I prefer
  /// Angara."
  /// ★ Named from the game: "you'll know why I prefer Angara."
  /// (MsgBabiPalaceTheSouthernContinentIsADangerous)
  LOCALID_SOLDIER_PREFERRING_ANGARA,
  /// ★ Named from the game: "Babi's face is very pale"
  /// (MsgBabiPalaceBabisFaceIsVeryPaleLike)
  LOCALID_BABI,
  /// The same resident as LOCALID_BY_BABIS_BED: "Babi's face is very pale,
  /// like that of a dead man."
  /// ★ Named from the game: "Babi's face is very pale"
  /// (MsgBabiPalaceBabisFaceIsVeryPaleLike)
  LOCALID_AFTER_COLOSSO_BY_BABIS_BED,
  /// ★ Named from the game: "You found the Cloak Ball."
  /// (MsgTolbiFoundCloakBall)
  LOCALID_CLOAK_BALL,
  /// The same resident as LOCALID_SORRY_FOR_SHEBA: "The girl from Lalivero is
  /// no longer here."
  /// ★ Named from the game: "The girl from Lalivero is no longer here."
  /// (MsgBabiPalaceTheGirlFromLaliveroIsNo)
  LOCALID_AFTER_COLOSSO_SORRY_FOR_SHEBA,
  /// The same woman as LOCALID_OLD_WOMAN_IN_SHEBAS_ROOM: "I am surprised Sheba
  /// was allowed to return to Lalivero like that."
  /// ★ Named from the game: "At least she won't be homesick any longer."
  /// (MsgBabiPalaceIAmSurprisedActorWasAllowed)
  LOCALID_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM
};

/// The two guards outside the palace (BabiPalaceEntrance).
enum {
  /// ★ Named from the game: "Normally, no one's allowed into Babi's Palace"
  /// (MsgBabiPalaceNormallyNoOnesAllowedIntoBabis)
  LOCALID_FIRST_GUARD = 8,
  /// ★ Named from the game: "Some of the rooms in the palace are available
  /// to Colosso entrants." (MsgBabiPalaceSomeOfTheRoomsInThe)
  LOCALID_SECOND_GUARD = 9
};

/// The attendant has led the party to the beds; talking again sends the
/// attendant back.
/// ★ Named from the game: "We have just enough extra beds in this room for
/// everybody." (MsgTolbiWeHaveJustEnoughExtra)
enum { FLAG_ATTENDANT_SHOWED_BEDS = FLAG_MAP_TEMP_00 };

/// The palace map's tileset animations that open its doors, for
/// TilesetAnim_Resume (play) and TilesetAnim_Pause (pause).
/// Each map entry pauses them, so every door is drawn shut; the recording of
/// BabiPalace_LeaveThroughDoor shows one swing open
/// (https://alchemy.pascalpixel.com/places/babi-palace/events/BabiPalace_LeaveThroughDoor.gif).
enum BabiPalaceDoorAnimation {
  BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION = 1, ///< the throne room's door
  BABI_PALACE_OTHER_DOORS_ANIMATION = 2,       ///< every door without its own
  BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION = 4     ///< the oven room's door
};

/// The leader's animations in the big bedroom's bed, after the four shared
/// ones (enum ObjectEventAnimation). The nod and the shake of the head answer
/// yes and no in BabiPalace_WakeAfterFinals.
enum BabiPalaceBedAnimation {
  ANIM_IN_BED_LYING = 31,
  /// Sitting up: the leader is raised by 3 pixels as it starts.
  ANIM_IN_BED_SITTING_UP = 32,
  ANIM_IN_BED_NOD = 33,
  ANIM_IN_BED_SHAKE_HEAD = 34
};

/// Metatile behaviors the map events answer besides the warps, whose metatile
/// behaviors are their warps' numbers.
enum BabiPalaceMetatileBehavior {
  /// The cells beside Iodem, while Babi is missing
  /// (BabiPalace_HearSoldiersReport).
  MB_BABI_PALACE_BESIDE_IODEM = 60,
  MB_BABI_PALACE_BEDS = 70, ///< the big bedroom's four beds, searched to sleep
  /// The well outside the palace.
  /// ★ Named from the game: "peered into the well" (MsgFieldPeeredWell)
  MB_BABI_PALACE_WELL = 90
};

/// A companion walks into the leader and is taken off the map, as the party
/// closes up into one walking sprite at the end of a scene.
static inline void BabiPalace_WalkIntoLeader(s32 member) {
  struct ObjectEvent *leader;

  ObjectEvent_SetAnimation(member, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectMotion_ResetAndSetPosition(member, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectEvent_WaitForMove(member);
  ObjectEvent_SetPosition(member, 0, 0);
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

const struct MapWarpDest *Map_GetWarpDests(void) {
  if (gGameState.mapId == (s32)&MapId_BabiPalaceEntrance) {
    return gBabiPalace_OutsideWarpDests;
  }
  return gBabiPalace_WarpDests;
}

/// The palace has no warp regions; the table is empty.
const struct MapWarpRegion *BabiPalace_GetRegions(void) {
  return gBabiPalace_Regions;
}

/// Both maps share one warp table.
const u32 *BabiPalace_GetWarps(void) { return gBabiPalace_Warps; }

/// Returns the object event templates for the map and the story: before Babi's
/// rescue, during the finals or after Colosso.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (gGameState.mapId == (s32)&MapId_BabiPalaceEntrance) {
    return gBabiPalace_OutsideObjectEventTemplates;
  }
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gBabiPalace_ObjectEventTemplatesAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gBabiPalace_ObjectEventTemplatesFinals;
  }
  return gBabiPalace_ObjectEventTemplatesBabiMissing;
}

/// Asks a yes or no question; the answers are the two messages after it.
static inline void BabiPalace_ShowQuestionAnswer(s32 localId, s32 question) {
  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtObject(localId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0)
    FieldScript_SetMessageId(question + 1);
  else
    FieldScript_SetMessageId(question + 2);
  FieldScript_ShowMessageAndWait(localId, 0);
}

/// An old man with a cane asks whether the party ran into Babi's soldiers. He's
/// one of the palace's residents, and asks while Babi is missing whether the
/// party ran into them in town.
static void BabiPalace_AskAboutSoldiers(s32 localId) {
  BabiPalace_ShowQuestionAnswer(localId, MsgTolbiRunBabisSoldiers);
}

/// Talking to the attendant before Babi's rescue: the attendant leads the party
/// to the beds. "We have just enough extra beds in this room for everybody.
/// Please follow me." Talking again, the attendant asks whether the party will
/// sleep right away and goes back.
static void BabiPalace_ShowBeds(s32 localId) {

  struct ObjectEvent *leader;
  s16 roundedFacing;
  s32 facingOrMessage;
  s32 facingBits;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & 0xc000;
  Script_Start();
  Field_EndBusyAndReveal();
  if (Script_GetFlag(FLAG_ATTENDANT_SHOWED_BEDS) == 0) {
    GameFlag_Set(FLAG_ATTENDANT_SHOWED_BEDS);
    GameFlag_Clear(FLAG_BABI_PALACE_BEDS_OPEN);
    Script_SetMessage(MsgTolbiWeHaveJustEnoughExtra);
    Script_ShowMessage(localId, 0);
    Script_Wait(10);
    facingBits = roundedFacing << 16;
    facingOrMessage = FACING_SOUTH;
    if (facingBits == (FACING_SOUTH << 16)) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 40, 104);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    }
    ObjectEvent_SetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(localId, 0, -48);
    ObjectEvent_WalkByAndWait(localId, 64, 0);
    ObjectEvent_FaceDirection(localId, facingOrMessage, 0);
  } else {
    GameFlag_Clear(FLAG_ATTENDANT_SHOWED_BEDS);
    GameFlag_Set(FLAG_BABI_PALACE_BEDS_OPEN);
    ObjectEvent_FaceDirection(localId, FACING_SOUTH, 0);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 120, 96);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    Script_Wait(20);
    facingOrMessage = MsgTolbiUseFourBeds;
    Script_SetMessage(facingOrMessage);
    Script_OpenMessage(localId, 0);
    if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      Script_SetMessage(facingOrMessage + 1);
      Script_ShowMessage(localId, 0);
    } else {
      Script_SetMessage(facingOrMessage + 2);
      Script_ShowMessage(localId, 0);
    }
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(localId, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_WalkByAndWait(localId, -64, 0);
    ObjectEvent_WalkByAndWait(localId, 0, 48);
#if TBS_PATCH_3
    ObjectEvent_SetPosition(localId, 56 << 16, 120 << 16);
#endif
  }
  Script_End();
}

/// Talking to the attendant during the finals: the first time, the attendant
/// asks whether the party came to rest before the match and steps aside;
/// afterwards, the attendant offers rest. "if you're tired, please feel free to
/// get some rest."
static void BabiPalace_OfferRest(s32 localId) {
  s32 question;

  Script_Start();
  Field_EndBusyAndReveal();
  if (Script_GetFlag(FLAG_BABI_PALACE_FINALS_REST_OFFERED) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
    GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
    ObjectEvent_FaceDirection(localId, FACING_SOUTH, 0);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 120, 96);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    Script_Wait(20);
    question = MsgTolbiCameRestBefore;
    Script_SetMessage(question);
    Script_OpenMessage(localId, 0);
    if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      Script_Wait(10);
      Script_SetMessage(question + 1);
    } else {
      Script_SetMessage(question + 2);
    }
    Script_ShowMessage(localId, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(localId, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_SetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(localId, -64, 0);
    ObjectEvent_WalkByAndWait(localId, 0, 48);
  } else {
    Script_SetMessage(MsgTolbiTiredFeelFreeRest);
    Script_OpenMessage(localId, 0);
  }
  Script_End();
}

/// A warrior asks whether the party is entering Colosso. The warrior is in the
/// palace while Babi is missing, and either hurries the party to the trials or
/// asks whether it got hurt too.
static void BabiPalace_AskAboutColosso(s32 localId) {
  s32 question = MsgTolbiPlanningEnterColosso;

  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtObject(localId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    FieldEffect_SpawnLinked(localId, EMOTE_IN_FRONT | EMOTE_WINCE, 0x28);
    FieldScript_SetMessageId(question + 1);
  } else {
    FieldScript_WaitFrames(10);
    FieldEffect_SpawnLinked(localId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x28);
    FieldScript_SetMessageId(question + 2);
  }
  FieldScript_ShowMessageAndWait(localId, 0);
}

/// A warrior with indigestion gives up on entering Colosso. The warrior is in
/// the palace while Babi is missing, tries to shrug it off, and doubles up.
static void BabiPalace_TalkToWarriorWithIndigestion(s32 localId) {
  struct ObjectEvent *objectEvent;
  const s32 *indigestionScript;

  objectEvent = ObjectEvent_Get(localId);
  Script_Start();
  indigestionScript = gBabiPalace_IndigestionScript;
  ObjectEvent_SetActionScript(localId, indigestionScript);
  Script_SetMessage(MsgTolbiCantFightBecauseLittleIndigestion);
  Script_ShowMessage(localId, 0);
  ObjectEvent_Stop(localId);
  objectEvent->scaleY = 0x10000;
  objectEvent->scaleX = 0x10000;
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(localId, 2);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(localId, 2);
  Script_Wait(60);
  Script_ShowMessage(localId, 0);
  Script_Wait(20);
  ObjectEvent_ShowEmote(localId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_RunRepeatedMotion(localId, 2);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(localId, 2);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(localId, 2);
  Script_Wait(30);
  ObjectEvent_SetActionScript(localId, indigestionScript);
  Script_ShowMessage(localId, 0);
  ObjectEvent_FaceDirection(localId, FACING_NORTHEAST, 0);
  Script_Wait(10);
  objectEvent->scaleY = 0x10000;
  objectEvent->scaleX = 0x10000;
  ObjectEvent_SetActionScript(localId, indigestionScript);
  Script_End();
}

/// Mind Read on Sheba in the palace, while Babi is missing: she cannot escape
/// to Lalivero alone; the first time, she feels it, ends the Psynergy and
/// asks about it. "Did you just..."
static void BabiPalace_ReadShebasMind(s32 casterId, s32 localId) {
  Script_Start();
  Script_SetMessage(MsgTolbiEvenIfEscapedBabiPalace);
  Script_ShowMessage(localId, 0);
  if (Script_GetFlag(FLAG_SHEBA_NOTICED_MIND_READ) == 0) {
    GameFlag_Set(FLAG_SHEBA_NOTICED_MIND_READ);
    Psynergy_Cancel();
    Script_Wait(50);
    ObjectEvent_ShowEmote(localId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    ObjectEvent_FaceObjectEvent(localId, LOCALID_PARTY_LEADER, 40);
    Script_ShowMessage(localId, 0);
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(localId, ANIM_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(localId, 0);
    ObjectEvent_FaceDirection(localId, FACING_WEST, 0);
  }
  Script_End();
}

/// The same old man, after Colosso, asks whether the party has met with Babi.
static void BabiPalace_AskAboutMeetingBabi(s32 localId) {
  BabiPalace_ShowQuestionAnswer(localId, MsgTolbiMeetBabi);
}

/// A young man asks whether the party is going back east already. He has brown
/// hair and is one of the palace's residents, and asks after Colosso whether
/// the party wants to go back already to the eastern shores of the Karagol Sea.
static void BabiPalace_AskAboutEasternShores(s32 localId) {
  BabiPalace_ShowQuestionAnswer(localId, MsgTolbiEasternShoresKaragol);
}

/// Taking the Cloak Ball on show after Colosso: Babi asks whether the party
/// wants it, and lends it for the quest if it does.
static void BabiPalace_OfferCloakBall(void) {
  s32 msg;

  FieldScript_Start();
  Field_EndBusyAndReveal();
  msg = MsgTolbiFoundCloakBall;
  FieldScript_SetMessageId(msg);
  FieldScript_ShowMessageAndWait(-1, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_BABI, 30);
  FieldMessage_ShowAtObject(LOCALID_BABI, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) !=
      0) {
    FieldScript_SetMessageId(msg + 2);
    FieldScript_ShowMessageAndWait(LOCALID_BABI, 0);
  } else {
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(msg + 3);
    FieldScript_ShowMessageAndWait(LOCALID_BABI, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(30);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    FieldScript_WaitFrames(30);
    Engine_ObjectEventSetPosition(LOCALID_CLOAK_BALL, 0, 0);
    FieldEffect_StartEffectObject22(ITEM_CLOAK_BALL, 3);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
    PartyInventory_GiveItem(ITEM_CLOAK_BALL, 0);
    GameFlag_Set(FLAG_BABI_PALACE_CLOAK_BALL);
  }
}

/// Waking after resting before the finals: the leader dreamt of fighting in
/// them, the others tease, and the party sets off for the colosseum.
static void BabiPalace_WakeFromFinalsDream(void) {
  struct ObjectEvent *leader;

  Script_Start();
  Script_SetMessage(MsgTolbiGetUp);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->spriteStorageKind = 0;
  ObjectEvent_Get(LOCALID_FINALS_ATTENDANT)->spriteStorageKind = 0;
  Task_Wait(1);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  Script_ShowMessage(-1, 0);
  REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->spriteStorageKind = 1;
  ObjectEvent_Get(LOCALID_FINALS_ATTENDANT)->spriteStorageKind = 1;
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_IN_BED_LYING);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(leader, 0);
  ObjectEvent_SetPosition(GARET, PIXELS(120), PIXELS(104));
  ObjectEvent_SetPosition(MIA, PIXELS(104), PIXELS(80));
  ObjectEvent_SetPosition(IVAN, PIXELS(120), PIXELS(120));
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  gFieldWork->transitionFrames = 60;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  Script_Wait(20);
  gFieldWork->transitionFrames = 24;
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(MIA, 16, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Script_Wait(20);
  ObjectEvent_RunRepeatedMotion(MIA, 2);
  Script_Wait(30);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leader->z.fixed += -0x30000;
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leader->targetZ += -0x30000;
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
  Script_Wait(40);
  ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
  Script_Wait(30);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_IN_BED_NOD);
  Script_Wait(50);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(30);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Script_Wait(20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(45);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Script_Wait(30);
  Script_OpenMessage(GARET, 0);
  if (Script_ChooseYesNo(-1, 0) != 0) {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER,
                                    ANIM_IN_BED_SHAKE_HEAD);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(GARET, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_IN_BED_NOD);
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(GARET, 0);
    gFieldWork->message += 1;
  } else {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_IN_BED_NOD);
    gFieldWork->message += 2;
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(GARET, 0);
  }
  Script_Wait(10);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(GARET, -16, 0);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  Script_Wait(35);
  ObjectEvent_Jump(LOCALID_PARTY_LEADER, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, -32, 0);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(40);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_SetAnimation(IVAN, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  BabiPalace_WalkIntoLeader(GARET);
  BabiPalace_WalkIntoLeader(MIA);
  BabiPalace_WalkIntoLeader(IVAN);
  Script_Wait(10);
  Script_End();
}

/// Searching the beds: the party goes to bed, talking it over the first night.
/// "Would you like to sleep now?"
static void BabiPalace_RestInPalaceBeds(void) {
  struct ObjectEvent *leader;
  s16 roundedFacing;
  u16 facing;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & -0x4000;
  Script_Start();
  Field_EndBusyAndReveal();
  Script_SetMessage(MsgTolbiLikeSleep);
  Script_OpenMessage(-1, 0);
  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) != 0) {
  } else {
    if (Script_GetFlag(FLAG_BABI_PALACE_FIRST_NIGHT) != 0) {
      Script_Wait(20);
      Inn_PlaySleep(0);
      return;
    }
    Script_Wait(20);
    facing = roundedFacing;
    if (facing == FACING_EAST) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 128, 120);
    }
    if (facing == FACING_WEST) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 240, 120);
    }
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 184, 120);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    Script_Wait(10);
    FieldMotion_LaunchFromFocusedObject(GARET, 16, 0, 0x8000);
    ObjectEvent_RefreshSelectorById(GARET);
    Script_Wait(10);
    Script_ShowMessage(GARET, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(GARET, 40, 0);
    ObjectEvent_WalkByAndWait(GARET, 0, -32);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Script_Wait(20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
    Script_Wait(20);
    FieldMotion_LaunchFromFocusedObject(IVAN, -16, 0, 0);
    ObjectEvent_RefreshSelectorById(IVAN);
    Script_Wait(10);
    Script_ShowMessage(IVAN, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(IVAN, -40, 0);
    ObjectEvent_WalkByAndWait(IVAN, 0, 40);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Script_Wait(20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    Script_Wait(20);
    FieldMotion_LaunchFromFocusedObject(MIA, 16, 0, 0x8000);
    ObjectEvent_RefreshSelectorById(MIA);
    Script_Wait(10);
    Script_ShowMessage(MIA, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(MIA, 40, 0);
    ObjectEvent_WalkByAndWait(MIA, 0, 40);
    ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
    Script_Wait(20);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, -56, 0);
    ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, -32);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
    Script_Wait(10);
    Script_ShowMessage(MIA, 0);
    ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
    Script_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Script_ShowMessage(GARET, 0);
    } else {
      gFieldWork->message++;
    }
    ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
    Script_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Script_ShowMessage(IVAN, 0);
    } else {
      gFieldWork->message++;
    }
    Script_Wait(20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
    ObjectEvent_FaceDirection(MIA, FACING_WEST, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
    Inn_PlaySleep(0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
    Script_Wait(10);
    Script_ShowMessage(GARET, 0);
    ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
    Script_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Script_ShowMessage(IVAN, 0);
    } else {
      gFieldWork->message++;
    }
    ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
    Script_Wait(10);
    if (GameFlag_Get(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Script_ShowMessage(MIA, 0);
    } else {
      gFieldWork->message++;
    }
    ObjectEvent_WalkByAndWait(GARET, 0, 32);
    ObjectEvent_WalkBy(GARET, -112, 0);
    ObjectEvent_WalkByAndWait(MIA, 0, -40);
    ObjectEvent_WalkBy(MIA, -112, 0);
    Script_Wait(50);
    ObjectEvent_WalkByAndWait(IVAN, 0, -24);
    ObjectEvent_WaitForMove(GARET);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectEvent_WalkByAndWait(GARET, 0, -16);
    ObjectEvent_WaitForMove(MIA);
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
    Script_Wait(20);
    Script_ShowMessage(GARET, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
    BabiPalace_WalkIntoLeader(GARET);
    BabiPalace_WalkIntoLeader(MIA);
    BabiPalace_WalkIntoLeader(IVAN);
    GameFlag_Set(FLAG_BABI_PALACE_FIRST_NIGHT);
  }
  Script_End();
}

/// Walking past Iodem while Babi is missing: his two soldiers report back
/// from Altmiller Cave without Babi, and Iodem tells how Babi has vanished
/// into the cave before; then they leave.
static void BabiPalace_HearSoldiersReport(void) {
  GameFlag_Set(FLAG_IODEM_HEARD_SOLDIERS);
  Script_Start();
  Field_EndBusyAndReveal();
  Script_SetMessage(MsgTolbiIodemIodem);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 520, 424);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  FieldCamera_MoveTo(PIXELS(560), -1, PIXELS(368), 1);
  FieldCamera_WaitForMove();
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 40, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 584, 360);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  ObjectEvent_SetSpeed(LOCALID_FIRST_SOLDIER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_SECOND_SOLDIER, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkTo(LOCALID_FIRST_SOLDIER, 528, 352);
  ObjectEvent_WalkToAndWait(LOCALID_SECOND_SOLDIER, 528, 368);
  Task_Wait(3);
  ObjectEvent_SetAnimation(LOCALID_FIRST_SOLDIER, ANIM_STAND);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, -16, 0);
  Script_Wait(10);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimation(LOCALID_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  ObjectEvent_SetSpeed(LOCALID_FIRST_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_FIRST_SOLDIER, 8, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  ObjectEvent_SetSpeed(LOCALID_SECOND_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_SECOND_SOLDIER, 8, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimation(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_FIRST_SOLDIER, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_SECOND_SOLDIER, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_FIRST_SOLDIER, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(LOCALID_FIRST_SOLDIER, LOCALID_SECOND_SOLDIER, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_FIRST_SOLDIER, LOCALID_IODEM, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SECOND_SOLDIER, LOCALID_IODEM, 0);
  Script_Wait(20);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAttachedEffect(LOCALID_IODEM, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(LOCALID_FIRST_SOLDIER, LOCALID_SECOND_SOLDIER, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_FIRST_SOLDIER, LOCALID_IODEM, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SECOND_SOLDIER, LOCALID_IODEM, 0);
  Script_Wait(20);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_FIRST_SOLDIER, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_SECOND_SOLDIER, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_FIRST_SOLDIER, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(LOCALID_FIRST_SOLDIER, LOCALID_SECOND_SOLDIER, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_FIRST_SOLDIER, LOCALID_IODEM, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SECOND_SOLDIER, LOCALID_IODEM, 0);
  Script_Wait(20);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAttachedEffect(
      LOCALID_FIRST_SOLDIER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(
      LOCALID_SECOND_SOLDIER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  ObjectEvent_StartRepeatedMotion(LOCALID_FIRST_SOLDIER, 2);
  ObjectEvent_RunRepeatedMotion(LOCALID_SECOND_SOLDIER, 2);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  Script_Wait(20);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(40);
  ObjectEvent_FaceEachOther(LOCALID_FIRST_SOLDIER, LOCALID_SECOND_SOLDIER, 60);
  ObjectEvent_SetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_SOUTH, 0);
  Script_Wait(40);
  ObjectEvent_SetSpeed(LOCALID_FIRST_SOLDIER, PIXELS(13) / 10, PIXELS(13) / 20);
  ObjectEvent_SetSpeed(LOCALID_SECOND_SOLDIER, PIXELS(13) / 10,
                       PIXELS(13) / 20);
  ObjectEvent_WalkBy(LOCALID_FIRST_SOLDIER, 0, 120);
  ObjectEvent_WalkByAndWait(LOCALID_SECOND_SOLDIER, 0, 120);
  ObjectEvent_SetPosition(LOCALID_FIRST_SOLDIER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_SOLDIER, 0, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(40);
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, -16, 0);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(30);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  Script_Wait(30);
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 120, 0);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 60, 0);
  ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
  Script_End();
}

/// Waking in the palace after the finals: the leader slept long after
/// fighting to the end; Babi wants to see the party.
static void BabiPalace_WakeAfterFinals(void) {
  struct ObjectEvent *objectEvent;
  s32 step;
  s32 pick;
  struct FieldWork *work;
  struct ScreenTransitionState *state;

  Script_Start();
  Script_SetMessage(MsgTolbiCallsName);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_IN_BED_LYING);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_SetPosition(GARET, PIXELS(104), PIXELS(104));
  ObjectEvent_SetPosition(MIA, PIXELS(88), PIXELS(120));
  ObjectEvent_SetPosition(IVAN, PIXELS(120), PIXELS(120));
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 0);
  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 12;
  Task_Wait(1);
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
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  for (step = 1; step <= 5; step++) {
    Task_Wait(3);
    state->value = step;
  }
  Script_Wait(40);
  ObjectEvent_RunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Script_Wait(30);
  for (step = 5; step <= 31; step++) {
    Task_Wait(3);
    state->value = step;
  }
  state->active = 31;
  work = gFieldWork;
  work->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  work->transitionFrames = 24;
  Script_Wait(20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 40);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Script_Wait(30);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  Script_ShowMessage(MIA, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(MIA, 2);
  Script_Wait(30);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(MIA, 0, -40);
  ObjectEvent_WalkByAndWait(MIA, 32, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Script_Wait(10);
  Script_ShowMessage(MIA, 0);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  objectEvent->z.fixed -= 0x30000;
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  objectEvent->targetZ -= 0x30000;
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
  Script_Wait(40);
  ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
  Script_Wait(30);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_IN_BED_NOD);
  Script_Wait(40);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Script_OpenMessage(IVAN, 0);
  if (Script_ChooseYesNo(-1, 0) == 0) {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_IN_BED_NOD);
    Script_Wait(20);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    ObjectEvent_Jump(GARET, 4, 13);
    ObjectEvent_Jump(GARET, 4, 30);
    Script_ShowMessage(GARET, 0);
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER,
                                    ANIM_IN_BED_SHAKE_HEAD);
    Script_Wait(20);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    ObjectEvent_Jump(GARET, 4, 13);
    ObjectEvent_Jump(GARET, 4, 30);
    gFieldWork->message++;
    Script_ShowMessage(GARET, 0);
  }
  Script_Wait(10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(40);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Script_Wait(50);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Script_Wait(35);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(IVAN, 2);
  Script_Wait(20);
  Script_OpenMessage(IVAN, 0);
  if (Script_ChooseYesNo(-1, 0) == 0) {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_IN_BED_NOD);
    Script_Wait(20);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    Script_ShowMessage(GARET, 0);
    pick = 0;
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER,
                                    ANIM_IN_BED_SHAKE_HEAD);
    Script_Wait(20);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    gFieldWork->message++;
    pick = 1;
    Script_ShowMessage(GARET, 0);
  }
  Script_Wait(10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Script_Wait(30);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(40);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Script_Wait(50);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Script_Wait(35);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(MIA, 0);
  if (pick == 0) {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
    Script_Wait(30);
    Script_ShowMessage(IVAN, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(GARET, 2);
    Script_Wait(30);
    Script_ShowMessage(GARET, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(IVAN, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(GARET, 2);
    Script_Wait(20);
    Script_ShowMessage(GARET, 0);
  }
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(GARET, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(IVAN, MIA, 40);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Script_Wait(30);
  // Wait for the player to leave the bed before the companions rejoin.
  while ((gKeysHeld & KEYS_DPAD) == 0) {
    Task_Wait(1);
  }
  ObjectEvent_Jump(LOCALID_PARTY_LEADER, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, -32, -8);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(20);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(MIA, 2);
  Script_Wait(20);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(20);
  Script_ShowMessage(GARET, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Script_Wait(30);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  BabiPalace_WalkIntoLeader(GARET);
  BabiPalace_WalkIntoLeader(MIA);
  BabiPalace_WalkIntoLeader(IVAN);
  Script_Wait(10);
  Script_End();
}

/// Arriving after Colosso: Iodem and Babi have been waiting for the party.
/// Babi praises its fighting, saw its Psynergy through his Cloak Ball, and
/// tells of Lemuria and the draught that has kept him alive; it is running
/// out, and he asks the party to sail his Lemurian ship to find Lemuria.
/// Learning that the party is bound for Venus Lighthouse, he sends Iodem
/// with it, to meet at Gondowan Passage.
static void BabiPalace_MeetBabiAfterColosso(void) {
  Script_PlaySong(MUS_030);
  Script_Start();
  Script_SetMessage(MsgTolbiWaitingCompanions);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldCamera_WaitForMove();
  Script_Wait(20);
  Script_Wait(10);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetSpeed(LOCALID_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, 16);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  FieldCamera_WaitForMove();
  Script_Wait(40);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 208, 0x2f8);
  Script_Wait(10);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, 0xc000);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 24, 0xc000);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, 0xc000);
  ObjectEvent_WaitForMove(GARET);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);

  // If the leader won the finals, Iodem brings the victor's prize, the Lure
  // Cap; otherwise Babi only praises how far the party came.
  if (GameFlag_Get(FLAG_WON_COLOSSO) != 0) {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_FaceDirection(LOCALID_BABI, FACING_NORTHWEST, 0);
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_BABI, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(20);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -16);
    Script_Wait(40);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(40);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
    Script_Wait(20);
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 32);
    ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
    Script_Wait(10);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
    Script_Wait(20);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_Get(LOCALID_IODEM)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -16);
    ObjectEvent_Get(LOCALID_IODEM)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
    ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
    Script_Wait(40);
    ObjectEvent_SetSpeed(LOCALID_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, 16);
    Script_Wait(40);
    MessageArgument_Push(ITEM_LURE_CAP, 2);
    Script_ShowMessage(-1, 0);
    Item_ShowFound(ITEM_LURE_CAP, 3);
    Party_GiveItem(ITEM_LURE_CAP, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    Script_Wait(30);
    ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, -16);
    ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
    Script_Wait(30);
    ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
    Script_Wait(20);
    Script_OpenMessage(LOCALID_BABI, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 5;
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectEvent_FaceDirection(LOCALID_BABI, FACING_NORTHWEST, 0);
    Script_Wait(40);
    ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(30);
    ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
    Script_Wait(30);
    ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
    Script_Wait(20);
    Script_OpenMessage(LOCALID_BABI, 0);
  }

  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_Wait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_IODEM, 0);
  }

  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_NORTHWEST, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_Jump(GARET, 4, 13);
  ObjectEvent_Jump(GARET, 4, 30);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  ObjectEvent_SetSpeed(LOCALID_BABI, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 16);
  Task_Wait(2);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Script_Wait(10);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(30);
  ObjectEvent_FaceObjectEvent(LOCALID_BABI, LOCALID_IODEM, 30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(LOCALID_BABI, GARET, 0);
  Script_Wait(40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(30);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(IVAN, 2);
  Script_Wait(20);
  ObjectEvent_FaceObjectEvent(IVAN, GARET, 30);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceEachOther(MIA, IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_OpenMessage(LOCALID_BABI, 0);
  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
    Script_Wait(30);
    Script_ShowMessage(LOCALID_BABI, 0);
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
    Script_Wait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_BABI, 0);
  }

  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimation(MIA, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_Wait(50);
  ObjectEvent_FaceObjectEvent(LOCALID_BABI, LOCALID_IODEM, 60);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_BABI, 50);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceEachOther(MIA, IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(50);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(20);
  Script_ShowMessage(GARET, 0);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  Script_Wait(40);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(LOCALID_BABI, IVAN, 40);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceEachOther(MIA, IVAN, 60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(50);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 40);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(20);
  Script_OpenMessage(GARET, 0);
  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Script_Wait(20);
    Script_ShowMessage(GARET, 0);
    gFieldWork->message += 3;
  } else {
    Script_Wait(10);
    gFieldWork->message++;
    Script_OpenMessage(GARET, 0);
    if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      Script_Wait(20);
      Script_ShowMessage(GARET, 0);
      gFieldWork->message++;
    } else {
      gFieldWork->message++;
      Script_ShowMessage(GARET, 0);
    }
  }

  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Script_Wait(40);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(20);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(IVAN, 2);
  Script_Wait(20);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_BABI, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(GARET, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(40);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_FaceObjectEvent(MIA, IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, IVAN, 0);
  Script_Wait(50);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(IVAN, 2);
  Script_Wait(20);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(MIA, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_BABI, 30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_StartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_StartRepeatedMotion(GARET, 2);
  ObjectEvent_StartRepeatedMotion(MIA, 2);
  ObjectEvent_RunRepeatedMotion(IVAN, 2);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceEachOther(MIA, IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(40);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(30);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(LOCALID_BABI, 20, 40);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAttachedEffect(LOCALID_IODEM, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 24);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IODEM, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_IODEM, 0);
  ObjectEvent_FaceObjectEvent(MIA, LOCALID_IODEM, 0);
  ObjectEvent_FaceObjectEvent(IVAN, LOCALID_IODEM, 0);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_BABI, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(30);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -24);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(20);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceEachOther(MIA, IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(LOCALID_BABI, 20, 30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(GARET, 2);
  Script_Wait(20);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(MIA, 2);
  Script_Wait(20);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimation(MIA, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(20);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(GARET, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_OpenMessage(IVAN, 0);
  if (Script_ChooseYesNo(LOCALID_BABI, 0) == 0) {
    Script_Wait(10);
    Script_ShowMessage(IVAN, 0);
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    gFieldWork->message++;
    Script_ShowMessage(IVAN, 0);
  }

  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceObjectEvent(LOCALID_BABI, LOCALID_IODEM, 40);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 40);
  Script_ShowMessage(GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Script_Wait(30);
  ObjectEvent_FaceEachOther(MIA, IVAN, 60);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  Script_ShowMessage(MIA, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(IVAN, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(LOCALID_IODEM, 14, 60);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(30);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_OpenMessage(LOCALID_IODEM, 0);
  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Script_Wait(20);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Script_ShowMessage(GARET, 0);
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    gFieldWork->message++;
    Script_ShowMessage(GARET, 0);
  }

  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(20);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimation(MIA, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_BABI, 40);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceObjectEvent(LOCALID_BABI, LOCALID_IODEM, 40);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(20);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        80);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_FaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceEachOther(MIA, IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Script_OpenMessage(LOCALID_BABI, 0);
  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Script_Wait(20);
    ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Script_ShowMessage(LOCALID_BABI, 0);
    gFieldWork->message++;
  } else {
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_BABI, 0);
  }

  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceObjectEvent(LOCALID_BABI, LOCALID_IODEM, 40);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_BABI, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_BABI, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(10);
  ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Script_Wait(20);
  // The party tracks each departing speaker before gathering back together.
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_PARTY_LEADER, 20);
  ObjectEvent_LinkObjectAndSetCallback(GARET, 20);
  ObjectEvent_LinkObjectAndSetCallback(MIA, 20);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, 20);
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 32);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Script_Wait(10);
  ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_WalkByAndWait(GARET, 16, 0);
  ObjectEvent_WalkBy(LOCALID_IODEM, 0, 80);
  Script_Wait(40);
  ObjectEvent_WalkByAndWait(GARET, -16, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 0);
  ObjectEvent_WaitForMove(LOCALID_IODEM);
  Script_Wait(80);
  ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_Wait(30);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_PARTY_LEADER, 14);
  ObjectEvent_LinkObjectAndSetCallback(GARET, 14);
  ObjectEvent_LinkObjectAndSetCallback(MIA, 14);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, 14);
  Script_Wait(30);
  ObjectEvent_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  Script_Wait(30);
  ObjectEvent_SetSpeed(LOCALID_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, 24);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, -80, 0);
  Script_Wait(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_EAST, 0);
  Script_Wait(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  Script_Wait(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  Script_Wait(20);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, 48);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, -64, 0);
  ObjectEvent_Stop(LOCALID_PARTY_LEADER);
  ObjectEvent_Stop(GARET);
  ObjectEvent_Stop(MIA);
  ObjectEvent_Stop(IVAN);
  ObjectEvent_SetPosition(LOCALID_BABI, 0, 0);
  Script_Wait(20);
  ObjectEvent_FaceEachOther(LOCALID_PARTY_LEADER, MIA, 0);
  ObjectEvent_FaceEachOther(GARET, IVAN, 0);
  Script_Wait(30);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimation(GARET, ANIM_NOD);
  ObjectEvent_SetAnimation(MIA, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_Wait(30);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  BabiPalace_WalkIntoLeader(GARET);
  BabiPalace_WalkIntoLeader(IVAN);
  BabiPalace_WalkIntoLeader(MIA);
  Script_Wait(10);
  Map_PlayMusic();
  Script_End();
}

/// Walking north into one of the palace's doors: it opens (SE_DOOR_OPEN) and
/// the leader walks through. The door's coord event is the number of the warp
/// it leads to, so the map events name each by its warp and the trigger is
/// passed on as the warp. The throne room's door walks the leader straight
/// north; the others first center the leader on the door.
static void BabiPalace_LeaveThroughDoor(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Script_Start();
  Script_PlaySong(SE_DOOR_OPEN);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  if (work->coordEvent == BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM) {
    TilesetAnim_Resume(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
    Script_Wait(10);
    ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -16);
  } else {
    if (work->coordEvent == BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM) {
      TilesetAnim_Resume(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
      Script_Wait(10);
      ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 3, -16);
    } else {
      TilesetAnim_Resume(BABI_PALACE_OTHER_DOORS_ANIMATION);
      Script_Wait(10);
      ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 3, -16);
    }
  }
  Script_Wait(16);
  Script_RequestWarp(work->coordEvent);
  BabiPalace_CloseDoors();
  Script_End();
}

/// Walking north into the palace's door from outside: it opens and the leader
/// goes in.
static void BabiPalace_LeaveThroughAnimatedDoor(void) {
  Script_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gBabiPalace_DoorCells, 36, 10);
  ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -16);
  Script_Wait(16);
  Script_RequestWarp(BABI_PALACE_ENTRANCE_TO_BABI_PALACE);
  Script_End();
}

/// A guard outside the palace during the finals passes on Babi's word.
/// "You're Isaac, aren't you? Babi left word that he would be waiting for you
/// at the colosseum."
static void BabiPalace_PassOnBabisWord(void) {
  s32 question;
  GameFlag_Set(FLAG_BABI_PALACE_GUARD_GAVE_WORD);
  Script_Start();
  Field_EndBusyAndReveal();
  ObjectEvent_FaceDirection(LOCALID_FIRST_GUARD, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 200, 272);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Script_Wait(20);
  question = MsgTolbiArent;
  Script_SetMessage(question);
  Script_OpenMessage(LOCALID_FIRST_GUARD, 0);
  if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Script_Wait(20);
    Script_SetMessage(question + 1);
    Script_ShowMessage(LOCALID_FIRST_GUARD, 0);
  } else {
    Script_Wait(20);
    Script_SetMessage(question + 2);
    Script_ShowMessage(LOCALID_FIRST_GUARD, 0);
    Script_Wait(20);
    ObjectEvent_FaceEachOther(LOCALID_FIRST_GUARD, LOCALID_SECOND_GUARD, 60);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUARD,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    Script_Wait(40);
    ObjectEvent_RunRepeatedMotion(LOCALID_SECOND_GUARD, 2);
    Script_Wait(30);
    ObjectEvent_FaceEachOther(LOCALID_FIRST_GUARD, LOCALID_SECOND_GUARD, 30);
    ObjectEvent_SetAnimationAndWait(LOCALID_SECOND_GUARD, ANIM_NOD);
    Script_Wait(30);
    ObjectEvent_ShowEmote(LOCALID_FIRST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE,
                          50);
    ObjectEvent_FaceDirection(LOCALID_FIRST_GUARD, FACING_SOUTH + FACING_STEP,
                              0);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUARD,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_FIRST_GUARD, ANIM_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_FIRST_GUARD, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_FIRST_GUARD, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_FIRST_GUARD, 0);
  }
  Script_End();
}

/// The other guard thanks the party for saving Babi once, then sends it to
/// the colosseum.
static void BabiPalace_ThankForSavingBabi(void) {
  if (Script_GetFlag(FLAG_BABI_PALACE_GUARD_THANKED_PARTY) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_GUARD_THANKED_PARTY);
    Script_SetMessage(MsgTolbiIsaacIdReallyLikeThank);
    Script_ShowMessage(LOCALID_SECOND_GUARD, 0);
  } else {
    Script_SetMessage(MsgTolbiBabiWaitingForAtColosseum);
    Script_ShowMessage(LOCALID_SECOND_GUARD, 0);
  }
}

/// A guard asks whether the party is leaving like the other warriors. The guard
/// stands outside the palace after Colosso, and means the other warriors who
/// stayed in the palace.
static void BabiPalace_AskAboutRemainingWarriors(s32 localId) {
  s32 question = MsgTolbiWarriorsWhoStayed;

  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtObject(localId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_SetMessageId(question + 1);
    FieldScript_ShowMessageAndWait(localId, 0);
  } else {
    FieldScript_SetMessageId(question + 2);
    FieldScript_ShowMessageAndWait(localId, 0);
  }
}

/// Searching the well outside the palace. "It's filled with fresh, clean
/// water."
static void BabiPalace_ExamineWell(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgTolbiItsFilledWithFreshClean, 1);
  FieldScript_End();
}

/// Returns the map events for the map and the story, as for the object event
/// templates.
const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_BabiPalaceEntrance) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gBabiPalace_OutsideEventsAfterColosso;
    }
    if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      return gBabiPalace_OutsideEventsFinals;
    }
    return gBabiPalace_OutsideEventsBabiMissing;
  }
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gBabiPalace_EventsAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gBabiPalace_EventsFinals;
  }
  return gBabiPalace_EventsBabiMissing;
}

/// Sets up the map being entered: the doors, the Cloak Ball on show, and
/// the scenes that play on arrival (Babi after Colosso, the attendant during
/// the finals, waking after a night's rest). Returns 0.
s32 BabiPalace_ApplyEntryState(void) {
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  u8 *buf;
  s32 set;

  BabiPalace_CloseDoors();
  if (gGameState.warpDest == BABI_PALACE_AFTER_BABI_SAVED) {
    GameFlag_Set(FLAG_BABI_SAVED);
  }
  if (gGameState.warpDest == BABI_PALACE_AFTER_COLOSSO) {
    GameFlag_Set(FLAG_BABI_SAVED);
    GameFlag_Set(FLAG_COLOSSO_OVER);
  }
  if (gGameState.mapId != (s32)&MapId_BabiPalaceEntrance) {
    if (gGameState.warpDest == BABI_PALACE_FROM_VALE_INTERIORS_INN) {
      GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    }
    if (GameFlag_Get(FLAG_COLOSSO_OVER)) {
      set = GameFlag_Get(FLAG_BABI_PALACE_CLOAK_BALL);
      if (set) {
        ObjectEvent_SetPosition(LOCALID_CLOAK_BALL, 0, 0);
      } else {
        objectEvent = ObjectEvent_Get(LOCALID_CLOAK_BALL);
        objectEvent->retainSpriteCache = 1;
        objectEvent->motionFlags = set;
        sprite = objectEvent->sprite;
        objectEvent->y.fixed = PIXELS(4);
        sprite->count = set;
        sprite->image.fullColor = 0;
        sprite->image.palette = 0;
        // The icon is drawn 0x400 bytes into the glyph buffer, as the 128 bytes
        // of a 16 by 16 sprite.
        buf = Heap_Allocate(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
        Item_LoadIcon(ITEM_CLOAK_BALL);
        Vram_Load(sprite->slot, 128, buf + 0x400);
        Heap_Release(HEAP_SLOT_GLYPH_TRANSFER);
      }
      if (gGameState.warpDest == BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM &&
          !Script_GetFlag(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
        GameFlag_Set(FLAG_BABI_SPOKE_AFTER_COLOSSO);
        ObjectEvent_SetPosition(LOCALID_BABI, PIXELS(208), PIXELS(704));
        BabiPalace_MeetBabiAfterColosso();
      }
      // After Colosso Babi lies in his bed, with no shadow
      // (https://alchemy.pascalpixel.com/places/babi-palace_colosso-over/map-18.png).
      ObjectEvent_SetAnimation(LOCALID_BABI, BABI_ANIMATION_IN_BED);
      ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_BABI), 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) &&
               !GameFlag_Get(FLAG_BABI_PALACE_FINALS_REST_OFFERED)) {
      ObjectEvent_SetPosition(LOCALID_FINALS_ATTENDANT, PIXELS(120),
                              PIXELS(72));
    }
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    // The attendant before the rescue and after Colosso; during the finals
    // LOCALID_FINALS_HURRYING_WARRIORS_ALONG has this number.
    ObjectEvent_Get(LOCALID_BED_ATTENDANT)->collisionFlags |=
        OBJ_EVENT_NEAR_TALK;
    if (gGameState.warpDest == BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      BabiPalace_WakeAfterFinals();
      // XXX: likely so that reloading the map does not play the waking again,
      // because only this warp destination chooses it.
      gGameState.warpDest = BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
    }
    if (gGameState.warpDest == BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
      ObjectEvent_SetPosition(LOCALID_FINALS_ATTENDANT, PIXELS(56),
                              PIXELS(120));
      ObjectEvent_FaceDirection(LOCALID_FINALS_ATTENDANT,
                                FACING_NORTHEAST + FACING_STEP, 0);
      BabiPalace_WakeFromFinalsDream();
      gGameState.warpDest = BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
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

struct MapWarpDest gBabiPalace_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { BABI_PALACE_FROM_VALE_INTERIORS_INN, CONDITION_ALWAYS, 168, 0, 936, FACING_SOUTH, 0, 0, 864, 240, 1024, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gBabiPalace_OutsideWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 264, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 264, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_BABI_PALACE, CONDITION_ALWAYS, 200, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpRegion gBabiPalace_Regions[] = {
    { MAP_TABLE_END },
};

u32 gBabiPalace_Warps[] = {
    MAP_WARPS(BabiPalace),
    MAP_WARP(BABI_PALACE_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance, BABI_PALACE_ENTRANCE_FROM_BABI_PALACE),
    MAP_WARP(BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_WEST),
    MAP_WARP(BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_EAST),
    MAP_WARP(BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS, BabiPalace, BABI_PALACE_WEST_STEPS_FROM_STATUE_ROOM),
    MAP_WARP(BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS, BabiPalace, BABI_PALACE_EAST_STEPS_FROM_STATUE_ROOM),
    MAP_WARP(BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST),
    MAP_WARP(BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_EAST),
    MAP_WARP(BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_WEST),
    MAP_WARP(BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_EAST),
    MAP_WARP(BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SCHOLARS_LANDING),
    MAP_WARP(BABI_PALACE_TO_VALE_INTERIORS_INN, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_BABI_PALACE),
    MAP_WARP(BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_TUNNEL_LANDING),
    MAP_WARP(BABI_PALACE_TO_COLOSSEUM_TUNNEL, ColosseumTunnel, COLOSSEUM_TUNNEL_FROM_BABI_PALACE),
    MAP_WARP(BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM, BabiPalace, BABI_PALACE_MEETING_ROOM_FROM_BEDROOM_LANDING),
    MAP_WARP(BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, BabiPalace, BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING),
    MAP_WARP(BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM),
    MAP_WARP(BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_WEST_STEPS),
    MAP_WARP(BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM, BabiPalace, BABI_PALACE_WEST_ROUND_ROOM_FROM_WEST_STEPS),
    MAP_WARP(BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_EAST_STEPS),
    MAP_WARP(BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM, BabiPalace, BABI_PALACE_EAST_ROUND_ROOM_FROM_EAST_STEPS),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_WEST),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_EAST),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING, BabiPalace, BABI_PALACE_SCHOLARS_LANDING_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING, BabiPalace, BABI_PALACE_TUNNEL_LANDING_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM, BabiPalace, BABI_PALACE_NORTH_ROUND_ROOM_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, BabiPalace, BABI_PALACE_OVEN_ROOM_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, BabiPalace, BABI_PALACE_SOLDIERS_ROOM_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, BabiPalace, BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM),
    MAP_WARP(BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_MEETING_ROOM),
    MAP_WARP(BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_MEETING_ROOM),
    MAP_WARP(BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_OVEN_ROOM),
    MAP_WARP(BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SOLDIERS_ROOM),
    MAP_WARP(BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS, BabiPalace, BABI_PALACE_WEST_STEPS_FROM_WEST_ROUND_ROOM),
    MAP_WARP(BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS, BabiPalace, BABI_PALACE_EAST_STEPS_FROM_EAST_ROUND_ROOM),
    MAP_WARP(BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_NORTH_ROUND_ROOM),
    MAP_WARP(BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SHEBAS_LANDING),
    MAP_WARP(BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, BabiPalace, BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING),
    MAP_WARP(BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM),
    MAP_WARPS(BabiPalaceEntrance),
    MAP_WARP(BABI_PALACE_ENTRANCE_TO_TOLBI, Tolbi, TOLBI_FROM_BABI_PALACE_ENTRANCE),
    MAP_WARP(BABI_PALACE_ENTRANCE_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_BABI_PALACE_ENTRANCE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gBabiPalace_OutsideObjectEventTemplates[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesBabiMissing[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_IODEM, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(352), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(368), FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesFinals[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesAfterColosso[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { MAP_TABLE_END },
};

struct MapEvent gBabiPalace_EventsBabiMissing[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_VALE_INTERIORS_INN },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { MAP_EVENT_WARP, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { MAP_EVENT_WARP, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { MAP_EVENT_WARP, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { MAP_EVENT_WARP, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { MAP_EVENT_WARP, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { MAP_EVENT_WARP, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { MAP_EVENT_WARP, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { MAP_EVENT_WARP, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { MAP_EVENT_WARP, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { MAP_EVENT_WARP, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { MAP_EVENT_WARP, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { MAP_EVENT_COORD, MB_BABI_PALACE_BESIDE_IODEM, FLAG_IODEM_HEARD_SOLDIERS, EVENT_SCRIPT(BabiPalace_HearSoldiersReport) },
    { MAP_EVENT_TALK, LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutSoldiers) },
    { MAP_EVENT_TALK, LOCALID_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
    { MAP_EVENT_TALK, LOCALID_BED_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ShowBeds) },
    { MAP_EVENT_TALK, LOCALID_INJURED_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutColosso) },
    { MAP_EVENT_TALK, LOCALID_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_TalkToWarriorWithIndigestion) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INJURED_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
    { MAP_EVENT_TALK, LOCALID_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
    { MAP_EVENT_TALK, LOCALID_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
    { MAP_EVENT_TALK, LOCALID_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
    { MAP_EVENT_TALK, LOCALID_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
    { MAP_EVENT_TALK, LOCALID_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
    { MAP_EVENT_TALK, LOCALID_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
    { MAP_EVENT_TALK, SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SHEBA, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ReadShebasMind) },
    { BG_EVENT(BG_EVENT_SILENT), MB_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_BEDS_OPEN), EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { BG_EVENT(BG_EVENT_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { BG_EVENT(BG_EVENT_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { BG_EVENT(BG_EVENT_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

struct MapEvent gBabiPalace_EventsFinals[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_VALE_INTERIORS_INN },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { MAP_EVENT_WARP, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { MAP_EVENT_WARP, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { MAP_EVENT_WARP, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { MAP_EVENT_WARP, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { MAP_EVENT_WARP, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { MAP_EVENT_WARP, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { MAP_EVENT_WARP, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { MAP_EVENT_WARP, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { MAP_EVENT_WARP, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { MAP_EVENT_WARP, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { MAP_EVENT_WARP, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { MAP_EVENT_TALK, LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
    { MAP_EVENT_TALK, LOCALID_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
    { MAP_EVENT_TALK, LOCALID_FINALS_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferRest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FINALS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
    { MAP_EVENT_TALK, LOCALID_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
    { MAP_EVENT_TALK, LOCALID_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
    { MAP_EVENT_TALK, LOCALID_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
    { MAP_EVENT_TALK, SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
    { BG_EVENT(BG_EVENT_SILENT), MB_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_FINALS_BEDS_OPEN), EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { BG_EVENT(BG_EVENT_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { BG_EVENT(BG_EVENT_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { BG_EVENT(BG_EVENT_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

struct MapEvent gBabiPalace_EventsAfterColosso[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_VALE_INTERIORS_INN },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { MAP_EVENT_WARP, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { MAP_EVENT_WARP, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { MAP_EVENT_WARP, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { MAP_EVENT_WARP, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { MAP_EVENT_WARP, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { MAP_EVENT_WARP, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { MAP_EVENT_WARP, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { MAP_EVENT_WARP, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { MAP_EVENT_WARP, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { MAP_EVENT_WARP, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { MAP_EVENT_WARP, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { MAP_EVENT_WARP, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { MAP_EVENT_WARP, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { MAP_EVENT_TALK, LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutMeetingBabi) },
    { MAP_EVENT_TALK, LOCALID_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutEasternShores) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
    { MAP_EVENT_TALK, LOCALID_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
    { MAP_EVENT_TALK, LOCALID_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
    { MAP_EVENT_TALK, LOCALID_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
    { MAP_EVENT_TALK, LOCALID_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
    { MAP_EVENT_TALK, LOCALID_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
    { MAP_EVENT_TALK, LOCALID_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
    { MAP_EVENT_TALK, LOCALID_CLOAK_BALL, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferCloakBall) },
    { MAP_EVENT_TALK, LOCALID_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
    { MAP_EVENT_TALK, LOCALID_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
    { BG_EVENT(BG_EVENT_SILENT), MB_BABI_PALACE_BEDS, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { BG_EVENT(BG_EVENT_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { BG_EVENT(BG_EVENT_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { BG_EVENT(BG_EVENT_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

struct MapEvent gBabiPalace_OutsideEventsBabiMissing[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { MAP_EVENT_TALK, LOCALID_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
    { MAP_EVENT_TALK, LOCALID_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
    { BG_EVENT(BG_EVENT_SILENT), MB_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { MAP_EVENTS_END },
};

struct MapEvent gBabiPalace_OutsideEventsFinals[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { MAP_EVENT_TALK, LOCALID_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { MAP_EVENT_TALK, LOCALID_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { MAP_EVENT_TALK, LOCALID_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
    { MAP_EVENT_TALK, LOCALID_SECOND_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ThankForSavingBabi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
    { BG_EVENT(BG_EVENT_SILENT), MB_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { MAP_EVENTS_END },
};

struct MapEvent gBabiPalace_OutsideEventsAfterColosso[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { MAP_EVENT_TALK, LOCALID_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
    { MAP_EVENT_TALK, LOCALID_SECOND_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutRemainingWarriors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
    { BG_EVENT(BG_EVENT_SILENT), MB_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { MAP_EVENTS_END },
};
// clang-format on
