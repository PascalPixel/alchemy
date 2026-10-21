/// @file
/// Babi's Palace in Tolbi, the palace of Babi, the ruler of Tolbi, in two
/// scenes that share this overlay and the area name Babi's Palace:
/// SceneId_BabiPalaceEntrance, outside, between the town and the palace
/// door, with two guards and a well, and SceneId_BabiPalace, the palace
/// itself, a set of rooms joined by doors (BabiPalace_LeaveThroughDoor). From
/// the palace one door leads into a room of Babi's scholars
/// (BABI_PALACE_TO_VALE_INTERIORS_INN, which vale_interiors_inn.c holds) and
/// another into the Colosseum Tunnel (BABI_PALACE_TO_COLOSSEUM_TUNNEL).
/// Each scene has three sets of placements and events, chosen by the story
/// (Scene_GetPlacements, Scene_GetEvents): before Babi is saved, during the
/// finals (FLAG_BABI_SAVED), and after Colosso (FLAG_COLOSSO_OVER).
/// BabiPalace_ApplyEntryState plays the scenes that wait for the party on
/// arrival, by the entrance it comes in by.
///
/// The party first comes here on the day Colosso should begin, with Babi
/// missing. During Colosso visitors may enter, the guards outside say, and
/// Babi opens rooms to the warriors who fight in it. Inside, an attendant
/// leads the party to four beds (BabiPalace_ShowBeds), and searching them puts
/// it to bed, talking it over the first night (BabiPalace_RestInPalaceBeds,
/// FLAG_BABI_PALACE_FIRST_NIGHT); warriors nurse injuries before the trials,
/// one doubled up with indigestion (gBabiPalace_IndigestionScript). Walking
/// past Iodem (touch 60) plays BabiPalace_HearSoldiersReport: the soldiers he
/// sent to search Altmiller Cave come back without Babi, Iodem tells how he
/// has seen Babi vanish into that cave before, though this time his bed was
/// not slept in, and leaves for the colosseum in Babi's stead
/// (FLAG_IODEM_HEARD_SOLDIERS, after which he and the soldiers are gone). In
/// another room is Sheba, a girl from the town of Lalivero on the Gondowan
/// continent, held as Babi's guest; she says herself that he holds her
/// hostage to complete his lighthouse in Lalivero. The first Mind Read on her
/// she feels (BabiPalace_ReadShebasMind, FLAG_SHEBA_NOTICED_MIND_READ).
/// Babi's diary reads `This Colosso may be my last...`. The story goes on in
/// Altmiller Cave, where the party finds Babi and saves him
/// (altmiller_cave.c).
///
/// During the finals the guards outside pass on Babi's word that he is
/// waiting at the colosseum (BabiPalace_PassOnBabisWord), and one thanks the
/// party once for saving him (BabiPalace_ThankForSavingBabi). Inside, the
/// attendant offers a rest before the match (BabiPalace_OfferRest). A lost
/// match brings the party back here by BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH
/// (colosseum_azart_match.c, colosseum_satrage_match.c): its HP and PP are made
/// full, Garet, Ivan and Mia rejoin and the inventory Colosso held is returned
/// (BabiPalace_RestorePartyAndInventory), and the leader wakes from what his
/// friends call a dream of fighting in the finals
/// (BabiPalace_WakeFromFinalsDream), free to go back and try again.
///
/// The third match, won or lost, ends Colosso and brings the party back by
/// BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH (colosseum_navampa_match.c): the
/// leader wakes long after the finals, and Babi wants to see him
/// (BabiPalace_WakeAfterFinals). Coming into the throne room
/// (BABI_PALACE_THRONE_ROOM_FROM_CENTRAL_HALL) the first time plays
/// BabiPalace_MeetBabiAfterColosso (FLAG_BABI_SPOKE_AFTER_COLOSSO): Babi
/// praises the party's fighting, and has Iodem bring the victor's prize if it
/// won (FLAG_WON_COLOSSO). He saw its Psynergy through his Cloak Ball, and
/// tells of Lemuria, the city of an ancient people far out at sea, where he
/// went at the time of the great flood, a hundred years ago, and whose draught
/// has kept him alive since and is running out. He built Babi Lighthouse to
/// find Lemuria again, and asks the party to sail his Lemurian ship, which only
/// an Adept can; learning that it must beat Felix and the others to Venus
/// Lighthouse, he sends Iodem with it, to meet at Gondowan Passage. The Cloak
/// Ball on show (ACTOR_CLOAK_BALL, drawn from its item icon) Babi lends for the
/// quest (BabiPalace_OfferCloakBall, FLAG_BABI_PALACE_CLOAK_BALL), and Sheba
/// has left for Lalivero with the soldiers going to Babi Lighthouse. Arriving
/// by BABI_PALACE_AFTER_BABI_SAVED sets FLAG_BABI_SAVED, and by
/// BABI_PALACE_AFTER_COLOSSO FLAG_COLOSSO_OVER as well
/// (XXX: no exit table in the scenes leads to either; what does would say what
/// they are for).

#include "game_state.h"
#include "heap_state.h"
#include "version.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/babi_palace.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_event.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"
#include "display_transition.h"
#include "flags.h"
#include "songs.h"

/// The palace's actors are placed by one of three tables, as the story goes
/// on: before Babi is rescued (gBabiPalace_PlacementsOther), during the
/// finals (gBabiPalace_PlacementsColosso) and after Colosso
/// (gBabiPalace_PlacementsAfterColosso). Each numbers its rows from
/// ACTOR_FIRST_PLACED, with Garet, Ivan, Mia and Sheba in their own party
/// slots, and most of the people stand in all three under other numbers.
/// The palace's residents stand in all three: one sent Babi's soldiers to
/// search the town, and another tells warriors who are not entering Colosso
/// that they may not stay. The game names none of them.
///
/// Before the rescue: the attendant who shows the party its beds, Iodem and
/// the two soldiers he sent to Altmiller Cave; Sheba is held here until
/// Colosso is over.
enum BabiPalaceActor {
  /// In the same room and at the same number in every table, and the one who
  /// sent Babi's soldiers to search the town: "Did you run into any of Babi's
  /// soldiers in Town?", and if not, "Hmph! I told them to search every nook
  /// and cranny of this town..."
  ACTOR_FIRST_RESIDENT = ACTOR_FIRST_PLACED,
  /// The attendant, whose number is the same after Colosso: "No one's going
  /// to sleep in this big bedroom until the next Colosso!"
  ACTOR_BED_ATTENDANT,
  /// "So, did you guys get injured, too?" Thoughts: "I hurt my back during my
  /// training, and now I can't move at all."
  ACTOR_INJURED_WARRIOR,
  /// "I can't fight because of a little indigestion? That's pathetic!"
  ACTOR_WARRIOR_WITH_INDIGESTION,
  /// "If you're entering Colosso, hurry along to the colosseum." Thoughts:
  /// "Warriors are not allowed to stay in the palace if they don't enter
  /// Colosso!"
  ACTOR_SECOND_RESIDENT,
  /// Four of Babi's soldiers, in one room; the first says "Only warriors who
  /// fight well in Colosso's finals can become soldiers here."
  ACTOR_FIRST_PALACE_SOLDIER,
  /// Beside Babi's bed. "It looks like Lord Babi didn't use his bed this
  /// morning."
  ACTOR_BEDSIDE_RESIDENT = ACTOR_FIRST_PALACE_SOLDIER + 4,
  /// "Sheba has been staying as Lord Babi's guest for some time." Thoughts:
  /// "It is heartbreaking to see her tears and hear her pleas to be
  /// released."
  ACTOR_PITYING_RESIDENT,
  /// In Sheba's room, in the old woman's sprite (0x6f, as in vale_gate.c).
  /// Thoughts: "Sheba is not permitted to leave this room."
  ACTOR_SHEBAS_MINDER,
  ACTOR_IODEM,
  ACTOR_FIRST_SOLDIER,
  ACTOR_SECOND_SOLDIER
};

/// During the finals: ACTOR_FIRST_RESIDENT keeps its number, and the others
/// stand under new ones.
enum BabiPalaceFinalsActor {
  /// The same resident as ACTOR_SECOND_RESIDENT: "What are you doing? Hurry
  /// over to the colosseum!"
  ACTOR_FINALS_SECOND_RESIDENT = ACTOR_FIRST_PLACED + 1,
  ACTOR_FINALS_ATTENDANT, ///< the same attendant, a number later
  /// The same resident as ACTOR_BEDSIDE_RESIDENT: "Babi has returned and the
  /// finals are set to begin!"
  ACTOR_FINALS_BEDSIDE_RESIDENT,
  /// The same resident as ACTOR_PITYING_RESIDENT: "What's going on? Sheba is
  /// looking much happier."
  ACTOR_FINALS_PITYING_RESIDENT,
  /// The same woman as ACTOR_SHEBAS_MINDER: "The soldiers came to see Sheba.
  /// She's been in a great mood ever since."
  ACTOR_FINALS_SHEBAS_MINDER
};

/// After Colosso: Babi, and the Cloak Ball he keeps on show;
/// ACTOR_FIRST_RESIDENT, ACTOR_BED_ATTENDANT and Iodem keep their numbers.
enum BabiPalaceAfterColossoActor {
  /// "Babi was found safe and sound, and Colosso ended without incident."
  ACTOR_RELIEVED_SOLDIER = ACTOR_FIRST_PLACED + 2,
  /// The same resident as ACTOR_SECOND_RESIDENT: "So, you're all from the
  /// eastern shores of the Karagol Sea, huh?"
  ACTOR_AFTER_COLOSSO_SECOND_RESIDENT,
  /// "Now that Colosso is over, I'm going to Gondowan to build Babi
  /// Lighthouse!"
  ACTOR_GONDOWAN_BOUND_SOLDIER,
  /// "The southern continent is a dangerous place... you'll know why I prefer
  /// Angara."
  ACTOR_SOLDIER_PREFERRING_ANGARA,
  ACTOR_BABI,
  /// The same resident as ACTOR_BEDSIDE_RESIDENT: "Babi's face is very pale,
  /// like that of a dead man."
  ACTOR_AFTER_COLOSSO_BEDSIDE_RESIDENT,
  ACTOR_CLOAK_BALL,
  /// The same resident as ACTOR_PITYING_RESIDENT: "The girl from Lalivero is
  /// no longer here."
  ACTOR_AFTER_COLOSSO_PITYING_RESIDENT,
  /// The same woman as ACTOR_SHEBAS_MINDER: "I am surprised Sheba was allowed
  /// to return to Lalivero like that."
  ACTOR_AFTER_COLOSSO_SHEBAS_MINDER
};

/// The two guards outside the palace (BabiPalaceEntrance).
enum { ACTOR_FIRST_GUARD = 8, ACTOR_SECOND_GUARD = 9 };

/// The attendant has led the party to the beds; talking again sends the
/// attendant back.
enum { FLAG_ATTENDANT_SHOWED_BEDS = FLAG_SCENE_TEMP_00 };

/// After a night in the palace's beds: the party's HP and PP are full,
/// Garet, Ivan and Mia rejoin, and the inventory Colosso held is returned.
static __inline__ void BabiPalace_RestorePartyAndInventory(void) {
  s16 owners[8];
  struct BattleUnit *state;
  s32 ownerCount;
  s32 i;

  ownerCount = FieldParty_ListActiveOwners(owners);
  for (i = 0; i < ownerCount; i++) {
    state = UnitGet(owners[i]);
    state->hp = state->maxHp;
    state->pp = state->maxPp;
    Engine_OwnerRecalculateRatios(owners[i]);
  }
  Party_AddActiveOwner(GARET);
  Party_AddActiveOwner(IVAN);
  Party_AddActiveOwner(MIA);
  Engine_InventorySnapshotRestore();
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_BabiPalaceEntrance) {
    return gBabiPalace_Entrances2;
  }
  return gBabiPalace_EntrancesOther;
}

/// The palace has no regions; the table is empty.
const struct SceneRegion *BabiPalace_GetRegions(void) {
  return gBabiPalace_Regions;
}

/// Both scenes share one exit table.
const u32 *BabiPalace_GetExits(void) { return gBabiPalace_Exits; }

/// Returns the actor placements for the scene and the story: before Babi's
/// rescue, during the finals or after Colosso.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_BabiPalaceEntrance) {
    return gBabiPalace_Placements2;
  }
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gBabiPalace_PlacementsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    return gBabiPalace_PlacementsColosso;
  }
  return gBabiPalace_PlacementsOther;
}

/// Asks a yes or no question; the answers are the two messages after it.
static inline void BabiPalace_ShowQuestionAnswer(s32 actorId, s32 question) {
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
    Engine_EventSetMessage(question + 1);
  else
    Engine_EventSetMessage(question + 2);
  Engine_EventShowMessage(actorId, 0);
}

/// A resident of the palace, while Babi is missing, asks whether the party ran
/// into Babi's soldiers in town.
static void BabiPalace_AskAboutSoldiers(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiRunBabisSoldiers);
}

/// Talking to the attendant before Babi's rescue: the attendant leads the party
/// to the beds. "We have just enough extra beds in this room for everybody.
/// Please follow me." Talking again, the attendant asks whether the party will
/// sleep right away and goes back.
static void BabiPalace_ShowBeds(s32 actorId) {

  struct FieldActor *leader;
  s16 roundedFacing;
  s32 facingOrMessage;
  s32 facingBits;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & 0xc000;
  Event_Begin();
  Battle_ResetEffectCounter();
  if (GameFlag_IsSet(FLAG_ATTENDANT_SHOWED_BEDS) == 0) {
    GameFlag_Set(FLAG_ATTENDANT_SHOWED_BEDS);
    GameFlag_Clear(FLAG_BABI_PALACE_BEDS_OPEN);
    Event_SetMessage(MsgTolbiWeHaveJustEnoughExtra);
    Event_ShowMessage(actorId, 0);
    Event_Wait(10);
    facingBits = roundedFacing << 16;
    facingOrMessage = 0x4000;
    if (facingBits == (0x4000 << 16)) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    }
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
    Actor_WalkByAndWait(actorId, 0, -48);
    Actor_WalkByAndWait(actorId, 64, 0);
    Actor_FaceDirection(actorId, facingOrMessage, 0);
  } else {
    GameFlag_Clear(FLAG_ATTENDANT_SHOWED_BEDS);
    GameFlag_Set(FLAG_BABI_PALACE_BEDS_OPEN);
    Actor_FaceDirection(actorId, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(20);
    facingOrMessage = MsgTolbiUseFourBeds;
    Event_SetMessage(facingOrMessage);
    Event_OpenMessage(actorId, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_SetMessage(facingOrMessage + 1);
      Event_ShowMessage(actorId, 0);
    } else {
      Event_SetMessage(facingOrMessage + 2);
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

/// Talking to the attendant during the finals: the first time, the attendant
/// asks whether the party came to rest before the match and steps aside;
/// afterwards, the attendant offers rest. "if you're tired, please feel free to
/// get some rest."
static void BabiPalace_OfferRest(s32 actorId) {
  s32 question;

  Event_Begin();
  Battle_ResetEffectCounter();
  if (GameFlag_IsSet(FLAG_BABI_PALACE_FINALS_REST_OFFERED) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
    GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
    Actor_FaceDirection(actorId, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(20);
    question = MsgTolbiCameRestBefore;
    Event_SetMessage(question);
    Event_OpenMessage(actorId, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(10);
      Event_SetMessage(question + 1);
    } else {
      Event_SetMessage(question + 2);
    }
    Event_ShowMessage(actorId, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(actorId, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
  } else {
    Event_SetMessage(MsgTolbiTiredFeelFreeRest);
    Event_OpenMessage(actorId, 0);
  }
  Event_End();
}

/// A warrior in the palace, while Babi is missing, asks whether the party is
/// entering Colosso, and either hurries it to the trials or asks whether it
/// got hurt too.
static void BabiPalace_AskAboutColosso(s32 actorId) {
  s32 question = MsgTolbiPlanningEnterColosso;

  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorShowEmote(actorId, EMOTE_IN_FRONT | 2, 0x28);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventWait(10);
    Engine_ActorShowEmote(actorId, EMOTE_IN_FRONT | 5, 0x28);
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

/// The warrior in the palace with indigestion, while Babi is missing, tries to
/// shrug it off, doubles up, and gives up on entering Colosso.
static void BabiPalace_TalkToWarriorWithIndigestion(s32 actorId) {
  struct FieldActor *actor;
  const s32 *indigestionScript;

  actor = Actor_Get(actorId);
  Event_Begin();
  indigestionScript = gBabiPalace_IndigestionScript;
  Actor_SetActionScript(actorId, indigestionScript);
  Event_SetMessage(MsgTolbiCantFightBecauseLittleIndigestion);
  Event_ShowMessage(actorId, 0);
  Actor_Stop(actorId);
  actor->scaleY = 0x10000;
  actor->scaleX = 0x10000;
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(60);
  Event_ShowMessage(actorId, 0);
  Event_Wait(20);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | 2, 60);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_SetActionScript(actorId, indigestionScript);
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirection(actorId, FACING_NORTHEAST, 0);
  Event_Wait(10);
  actor->scaleY = 0x10000;
  actor->scaleX = 0x10000;
  Actor_SetActionScript(actorId, indigestionScript);
  Event_End();
}

/// Mind Read on Sheba in the palace, while Babi is missing: she cannot escape
/// to Lalivero alone; the first time, she feels it, ends the Psynergy and
/// asks about it. "Did you just..."
static void BabiPalace_ReadShebasMind(s32 unused0, s32 actorId) {
  Event_Begin();
  Event_SetMessage(MsgTolbiEvenIfEscapedBabiPalace);
  Event_ShowMessage(actorId, 0);
  if (GameFlag_IsSet(FLAG_SHEBA_NOTICED_MIND_READ) == 0) {
    GameFlag_Set(FLAG_SHEBA_NOTICED_MIND_READ);
    Psynergy_Cancel();
    Event_Wait(50);
    Actor_ShowEmote(actorId, EMOTE_IN_FRONT, 70);
    Actor_FaceActor(actorId, ACTOR_PARTY_LEADER, 40);
    Event_ShowMessage(actorId, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(actorId, FACING_WEST, 0);
  }
  Event_End();
}

/// A resident of the palace, after Colosso, asks whether the party has met with
/// Babi.
static void BabiPalace_AskAboutMeetingBabi(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiMeetBabi);
}

/// A resident of the palace, after Colosso, asks whether the party wants to go
/// back already to the eastern shores of the Karagol Sea.
static void BabiPalace_AskAboutEasternShores(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiEasternShoresKaragol);
}

/// Taking the Cloak Ball on show after Colosso: Babi asks whether the party
/// wants it, and lends it for the quest if it does.
static void BabiPalace_OfferCloakBall(void) {
  s32 msg;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  msg = MsgTolbiFoundCloakBall;
  Engine_EventSetMessage(msg);
  Engine_EventShowMessage(-1, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BABI, 30);
  Engine_EventOpenMessage(ACTOR_BABI, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
    Engine_EventSetMessage(msg + 2);
    Engine_EventShowMessage(ACTOR_BABI, 0);
  } else {
    Engine_EventWait(20);
    Engine_EventSetMessage(msg + 3);
    Engine_EventShowMessage(ACTOR_BABI, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetPosition(ACTOR_CLOAK_BALL, 0, 0);
    Engine_ItemShowFound(ITEM_CLOAK_BALL, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_PartyGiveItem(ITEM_CLOAK_BALL, 0);
    Engine_GameFlagSet(FLAG_BABI_PALACE_CLOAK_BALL);
  }
}

/// Waking after resting before the finals: the leader dreamt of fighting in
/// them, the others tease, and the party sets off for the colosseum.
static void BabiPalace_WakeFromFinalsDream(void) {
  struct FieldActor *leader;

  Event_Begin();
  Event_SetMessage(MsgTolbiGetUp);
  Actor_Get(ACTOR_PARTY_LEADER)->spriteStorageKind = 0;
  Actor_Get(ACTOR_FINALS_ATTENDANT)->spriteStorageKind = 0;
  Task_Wait(1);
  REG_DISPCNT = 0x1140;
  Event_ShowMessage(-1, 0);
  REG_DISPCNT = 0x140;
  Actor_Get(ACTOR_PARTY_LEADER)->spriteStorageKind = 1;
  Actor_Get(ACTOR_FINALS_ATTENDANT)->spriteStorageKind = 1;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 31);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 0);
  Actor_SetPosition(GARET, PIXELS(120), PIXELS(104));
  Actor_SetPosition(MIA, PIXELS(104), PIXELS(80));
  Actor_SetPosition(IVAN, PIXELS(120), PIXELS(120));
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  gEventWork->transitionFrames = 60;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Event_Wait(20);
  gEventWork->transitionFrames = 24;
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkByAndWait(MIA, 16, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->z.fixed += -0x30000;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->targetZ += -0x30000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 32);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 33);
  Event_Wait(50);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
  Event_Wait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 6, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(45);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Event_OpenMessage(GARET, 0);
  if (Event_ChooseYesNo(-1, 0) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(20);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    Event_Wait(30);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    gEventWork->message += 1;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    gEventWork->message += 2;
    Event_Wait(30);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkByAndWait(GARET, -16, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Event_Wait(35);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimation(IVAN, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Actor_WaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetAnimation(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Actor_SetAnimation(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Actor_WaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Event_Wait(10);
  Event_End();
}

/// Searching the beds: the party goes to bed, talking it over the first night.
/// "Would you like to sleep now?"
static void BabiPalace_RestInPalaceBeds(void) {
  struct FieldActor *leader;
  s16 roundedFacing;
  u16 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & -0x4000;
  Event_Begin();
  Battle_ResetEffectCounter();
  Event_SetMessage(MsgTolbiLikeSleep);
  Event_OpenMessage(-1, 0);
  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
  } else {
    if (GameFlag_IsSet(FLAG_BABI_PALACE_FIRST_NIGHT) != 0) {
      Event_Wait(20);
      Inn_PlaySleep(0);
      return;
    }
    Event_Wait(20);
    facing = roundedFacing;
    if (facing == 0) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
    }
    if (facing == 0x8000) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 240, 120);
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 120);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(10);
    FieldMotion_LaunchFromFocusedObject(GARET, 16, 0, 0x8000);
    Object_RefreshSelectorById(GARET);
    Event_Wait(10);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(GARET, 0x10000, 0x8000);
    Actor_WalkByAndWait(GARET, 40, 0);
    Actor_WalkByAndWait(GARET, 0, -32);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_Wait(20);
    FieldMotion_LaunchFromFocusedObject(IVAN, -16, 0, 0);
    Object_RefreshSelectorById(IVAN);
    Event_Wait(10);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(IVAN, 0x10000, 0x8000);
    Actor_WalkByAndWait(IVAN, -40, 0);
    Actor_WalkByAndWait(IVAN, 0, 40);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(20);
    FieldMotion_LaunchFromFocusedObject(MIA, 16, 0, 0x8000);
    Object_RefreshSelectorById(MIA);
    Event_Wait(10);
    Event_ShowMessage(MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(MIA, 0x10000, 0x8000);
    Actor_WalkByAndWait(MIA, 40, 0);
    Actor_WalkByAndWait(MIA, 0, 40);
    Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
    Event_Wait(20);
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -56, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -32);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(MIA, ANIM_NOD);
    Event_Wait(10);
    Event_ShowMessage(MIA, 0);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_IsSet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(GARET, 0);
    } else {
      gEventWork->message++;
    }
    Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_IsSet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(IVAN, 0);
    } else {
      gEventWork->message++;
    }
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Actor_FaceDirection(MIA, FACING_WEST, 0);
    Actor_FaceDirection(IVAN, FACING_EAST, 0);
    Inn_PlaySleep(0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(10);
    Event_ShowMessage(GARET, 0);
    Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_IsSet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(IVAN, 0);
    } else {
      gEventWork->message++;
    }
    Actor_SetAnimationAndWait(MIA, ANIM_NOD);
    Event_Wait(10);
    if (GameFlag_IsSet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
      Event_ShowMessage(MIA, 0);
    } else {
      gEventWork->message++;
    }
    Actor_WalkByAndWait(GARET, 0, 32);
    Actor_WalkBy(GARET, -112, 0);
    Actor_WalkByAndWait(MIA, 0, -40);
    Actor_WalkBy(MIA, -112, 0);
    Event_Wait(50);
    Actor_WalkByAndWait(IVAN, 0, -24);
    Actor_WaitForMove(GARET);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_WalkByAndWait(GARET, 0, -16);
    Actor_WaitForMove(MIA);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(GARET, 0x13333, 0x9999);
    Actor_SetSpeed(IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(MIA, 0x13333, 0x9999);
    Actor_SetAnimation(GARET, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Actor_WaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
    Actor_SetAnimation(MIA, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(MIA, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Actor_WaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
    Actor_SetAnimation(IVAN, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Actor_WaitForMove(IVAN);
    Actor_SetPosition(IVAN, 0, 0);
    GameFlag_Set(FLAG_BABI_PALACE_FIRST_NIGHT);
  }
  Event_End();
}

/// Walking past Iodem while Babi is missing: his two soldiers report back
/// from Altmiller Cave without Babi, and Iodem tells how Babi has vanished
/// into the cave before; then they leave.
static void BabiPalace_HearSoldiersReport(void) {
  GameFlag_Set(FLAG_IODEM_HEARD_SOLDIERS);
  Event_Begin();
  Battle_ResetEffectCounter();
  Event_SetMessage(MsgTolbiIodemIodem);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 520, 424);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Camera_MoveTo(PIXELS(560), -1, PIXELS(368), 1);
  Camera_WaitForMove();
  Actor_SetSpeed(ACTOR_IODEM, 65536, 32768);
  Actor_WalkByAndWait(ACTOR_IODEM, 40, 0);
  Actor_WalkToAndWait(ACTOR_IODEM, 584, 360);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(ACTOR_IODEM, 32768, 0);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, 131072, 65536);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, 131072, 65536);
  Actor_WalkTo(ACTOR_FIRST_SOLDIER, 528, 352);
  Actor_WalkToAndWait(ACTOR_SECOND_SOLDIER, 528, 368);
  Task_Wait(3);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | 2, 40);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, 65536, 32768);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 8, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 80);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | 2, 40);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, 65536, 32768);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 8, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, 22, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, 22, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, 22, 60);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
  Actor_FaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
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
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_IODEM, 32768, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, 22, 60);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, 16384, 0);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, 85196, 42598);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, 85196, 42598);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 120);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 0, 120);
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, 0, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_IODEM, 52428, 26214);
  Actor_WalkByAndWait(ACTOR_IODEM, -16, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_IODEM, 65536, 32768);
  Actor_WalkByAndWait(ACTOR_IODEM, 120, 0);
  Actor_WalkByAndWait(ACTOR_IODEM, 60, 0);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Event_End();
}

/// Waking in the palace after the finals: the leader slept long after
/// fighting to the end; Babi wants to see the party.
static void BabiPalace_WakeAfterFinals(void) {
  struct FieldActor *actor;
  s32 step;
  s32 pick;
  struct EventWork *event;
  struct DisplayTransitionState *work;

  Event_Begin();
  Event_SetMessage(MsgTolbiCallsName);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 31);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetPosition(GARET, PIXELS(104), PIXELS(104));
  Actor_SetPosition(MIA, PIXELS(88), PIXELS(120));
  Actor_SetPosition(IVAN, PIXELS(120), PIXELS(120));
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  event = gEventWork;
  event->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  event->transitionFrames = 12;
  Task_Wait(1);
  DisplayTransition_InitializeBattleEffectState(WINDOW_TRANSITION_LEADER);
  // UB: HEAP_SLOT_TRANSITION lies past scalar gEventWork. Its named load adds
  // four bytes in the native English trial (0x82c to 0x830).
#ifdef UBFIX
  work = gDisplayTransition;
#else
  work = (struct DisplayTransitionState *)((u8 **)&gEventWork)[4];
#endif
  work->value = 0;
  work->mask = 0x1f1f;
  work->active = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  for (step = 1; step <= 5; step++) {
    Task_Wait(3);
    work->value = step;
  }
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(30);
  for (step = 5; step <= 31; step++) {
    Task_Wait(3);
    work->value = step;
  }
  work->active = 31;
  event = gEventWork;
  event->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  event->transitionFrames = 24;
  Event_Wait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 50);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Event_ShowMessage(MIA, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(30);
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkByAndWait(MIA, 0, -40);
  Actor_WalkByAndWait(MIA, 32, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Event_ShowMessage(MIA, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->z.fixed -= 0x30000;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->targetZ -= 0x30000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 32);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 33);
  Event_Wait(40);
  Actor_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 50);
  Event_OpenMessage(IVAN, 0);
  if (Event_ChooseYesNo(-1, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
    Actor_Jump(GARET, 4, 13);
    Actor_Jump(GARET, 4, 30);
    Event_ShowMessage(GARET, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
    Actor_Jump(GARET, 4, 13);
    Actor_Jump(GARET, 4, 30);
    gEventWork->message++;
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(40);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 8, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Event_OpenMessage(IVAN, 0);
  if (Event_ChooseYesNo(-1, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 40);
    Event_ShowMessage(GARET, 0);
    pick = 0;
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 40);
    gEventWork->message++;
    pick = 1;
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(40);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 8, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  if (pick == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(30);
    Event_ShowMessage(GARET, 0);
    gEventWork->message += 2;
  } else {
    gEventWork->message += 2;
    Event_Wait(10);
    Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Event_Wait(10);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_FaceEachOther(IVAN, MIA, 40);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  // Wait for the player to leave the bed before the companions rejoin.
  while ((gKeysHeld & KEYS_DPAD) == 0) {
    Task_Wait(1);
  }
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetAnimation(GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Engine_ActorSetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetAnimation(MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Engine_ActorSetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Actor_SetAnimation(IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Engine_ActorSetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Event_Wait(10);
  Event_End();
}

/// Arriving after Colosso: Iodem and Babi have been waiting for the party.
/// Babi praises its fighting, saw its Psynergy through his Cloak Ball, and
/// tells of Lemuria and the draught that has kept him alive; it is running
/// out, and he asks the party to sail his Lemurian ship to find Lemuria.
/// Learning that the party is bound for Venus Lighthouse, he sends Iodem
/// with it, to meet at Gondowan Passage.
static void BabiPalace_MeetBabiAfterColosso(void) {
  struct FieldActor *position;

  Event_PlayCue(MUS_030);
  Event_Begin();
  Event_SetMessage(MsgTolbiWaitingCompanions);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  Camera_WaitForMove();
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_BABI, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 16);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Camera_WaitForMove();
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 0x2f8);
  Event_Wait(10);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(736), 1);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, 0xc000);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 24, 0xc000);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, 0xc000);
  Actor_WaitForMove(GARET);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);

  // If the leader won the finals, Iodem brings the victor's prize, the Lure
  // Cap; otherwise Babi only praises how far the party came.
  if (GameFlag_IsSet(FLAG_WON_COLOSSO) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_BABI, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
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
    Actor_SetSpeed(ACTOR_IODEM, 0xcccc, 0x6666);
    Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Actor_Get(ACTOR_IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_BABI, 0xcccc, 0x6666);
    Actor_WalkByAndWait(ACTOR_BABI, 0, 16);
    Event_Wait(40);
    Engine_UiTextDrawQuantity(ITEM_LURE_CAP, 2);
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
    Event_OpenMessage(ACTOR_BABI, 0);
    gEventWork->message += 2;
  } else {
    gEventWork->message += 5;
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
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
    Event_OpenMessage(ACTOR_BABI, 0);
  }

  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }

  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_Jump(GARET, 4, 13);
  Actor_Jump(GARET, 4, 30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_BABI, 0x19999, 0xcccc);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Task_Wait(2);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_BABI, GARET, 0);
  Event_Wait(40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(30);
  Actor_FaceActor(GARET, IVAN, 30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Actor_FaceActor(IVAN, GARET, 30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 6, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_BABI, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_BABI, 0);
  }

  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimation(MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 60);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 50);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 50);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_BABI, IVAN, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_OpenMessage(GARET, 0);
  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    gEventWork->message += 3;
  } else {
    Event_Wait(10);
    gEventWork->message++;
    Event_OpenMessage(GARET, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Event_ShowMessage(GARET, 0);
      gEventWork->message++;
    } else {
      gEventWork->message++;
      Event_ShowMessage(GARET, 0);
    }
  }

  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 5, 70);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceActor(MIA, IVAN, 0);
  Actor_FaceActor(ACTOR_IODEM, IVAN, 0);
  Event_Wait(50);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(GARET, 2);
  Actor_StartRepeatedMotion(MIA, 2);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_BABI, 20, 40);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_SetSpeed(ACTOR_IODEM, 0x19999, 0xcccc);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 24);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Actor_FaceActor(GARET, ACTOR_IODEM, 0);
  Actor_FaceActor(MIA, ACTOR_IODEM, 0);
  Actor_FaceActor(IVAN, ACTOR_IODEM, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 60);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 60);
  Actor_SetSpeed(ACTOR_IODEM, 0x13333, 0x9999);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -24);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_BABI, 20, 30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimation(MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 40);
  Event_OpenMessage(IVAN, 0);
  if (Event_ChooseYesNo(ACTOR_BABI, 0) == 0) {
    Event_Wait(10);
    Event_ShowMessage(IVAN, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    gEventWork->message++;
    Event_ShowMessage(IVAN, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 40);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_FaceEachOther(MIA, IVAN, 60);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_IODEM, 14, 60);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_IODEM, 0);
  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(GARET, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    gEventWork->message++;
    Event_ShowMessage(GARET, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimation(MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 80);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 2, 70);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | 6, 50);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 8, 40);
  Event_OpenMessage(ACTOR_BABI, 0);
  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(ACTOR_BABI, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
    gEventWork->message++;
    Event_ShowMessage(ACTOR_BABI, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // The party tracks each departing speaker before gathering back together.
  Object_LinkObjectAndSetCallback(ACTOR_PARTY_LEADER, 20);
  Object_LinkObjectAndSetCallback(GARET, 20);
  Object_LinkObjectAndSetCallback(MIA, 20);
  Object_LinkObjectAndSetCallback(IVAN, 20);
  Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 32);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Actor_WalkByAndWait(GARET, 16, 0);
  Actor_WalkBy(ACTOR_IODEM, 0, 80);
  Event_Wait(40);
  Actor_WalkByAndWait(GARET, -16, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_WaitForMove(ACTOR_IODEM);
  Event_Wait(80);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Object_LinkObjectAndSetCallback(ACTOR_PARTY_LEADER, 14);
  Object_LinkObjectAndSetCallback(GARET, 14);
  Object_LinkObjectAndSetCallback(MIA, 14);
  Object_LinkObjectAndSetCallback(IVAN, 14);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_BABI, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 24);
  Actor_WalkByAndWait(ACTOR_BABI, -80, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -64, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(GARET);
  Actor_Stop(MIA);
  Actor_Stop(IVAN);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, MIA, 0);
  Actor_FaceEachOther(GARET, IVAN, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimation(MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_PlayCue(MUS_017);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetAnimation(GARET, ANIM_WALK);
  position = Actor_Get(ACTOR_PARTY_LEADER);
  if (position != 0)
    Engine_ActorSetDestination(GARET, position->x.part.pixel,
                               position->z.part.pixel);
  Actor_WaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetAnimation(IVAN, ANIM_WALK);
  position = Actor_Get(ACTOR_PARTY_LEADER);
  if (position != 0)
    Engine_ActorSetDestination(IVAN, position->x.part.pixel,
                               position->z.part.pixel);
  Actor_WaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetAnimation(MIA, ANIM_WALK);
  position = Actor_Get(ACTOR_PARTY_LEADER);
  if (position != 0)
    Engine_ActorSetDestination(MIA, position->x.part.pixel,
                               position->z.part.pixel);
  Actor_WaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Event_Wait(10);
  Audio_PlayCueFromEventWork();
  Event_End();
}

/// Walking north into one of the palace's doors: it opens (SE_DOOR_OPEN) and
/// the leader walks through.
static void BabiPalace_LeaveThroughDoor(void) {
  struct EventWork *work;

  work = gEventWork;
  Event_Begin();
  Event_PlayCue(SE_DOOR_OPEN);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->touchedTrigger == 32) {
    Map_ClearLayerEntryFlag(1);
    Event_Wait(10);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    if (work->touchedTrigger == 30) {
      Map_ClearLayerEntryFlag(4);
      Event_Wait(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    } else {
      Map_ClearLayerEntryFlag(2);
      Event_Wait(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
  }
  Event_Wait(16);
  Event_RequestExit(work->touchedTrigger);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(4);
  Event_End();
}

/// Walking north into the palace's door from outside: it opens and the leader
/// goes in.
static void BabiPalace_LeaveThroughAnimatedDoor(void) {
  Event_Begin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gBabiPalace_DoorCells, 36, 10);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_Wait(16);
  Event_RequestExit(BABI_PALACE_ENTRANCE_TO_BABI_PALACE);
  Event_End();
}

/// A guard outside the palace during the finals passes on Babi's word.
/// "You're Isaac, aren't you? Babi left word that he would be waiting for you
/// at the colosseum."
static void BabiPalace_PassOnBabisWord(void) {
  s32 question;
  GameFlag_Set(FLAG_BABI_PALACE_GUARD_GAVE_WORD);
  Event_Begin();
  Battle_ResetEffectCounter();
  Actor_FaceDirection(ACTOR_FIRST_GUARD, 20480, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, 12288, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
  Event_Wait(20);
  question = MsgTolbiArent;
  Event_SetMessage(question);
  Event_OpenMessage(ACTOR_FIRST_GUARD, 0);
  if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Event_SetMessage(question + 1);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  } else {
    Event_Wait(20);
    Event_SetMessage(question + 2);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 60);
    Actor_FaceDirection(ACTOR_SECOND_GUARD, 12288, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
    Event_Wait(30);
    Actor_FaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 30);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | 2, 50);
    Actor_FaceDirection(ACTOR_FIRST_GUARD, 20480, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUARD, 12288, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  }
  Event_End();
}

/// The other guard thanks the party for saving Babi once, then sends it to
/// the colosseum.
static void BabiPalace_ThankForSavingBabi(void) {
  if (GameFlag_IsSet(FLAG_BABI_PALACE_GUARD_THANKED_PARTY) == 0) {
    GameFlag_Set(FLAG_BABI_PALACE_GUARD_THANKED_PARTY);
    Event_SetMessage(MsgTolbiIsaacIdReallyLikeThank);
    Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  } else {
    Event_SetMessage(MsgTolbiBabiWaitingForAtColosseum);
    Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  }
}

/// A guard outside the palace, after Colosso, asks whether the party is
/// leaving like the other warriors who stayed in the palace.
static void BabiPalace_AskAboutRemainingWarriors(s32 actorId) {
  s32 question = MsgTolbiWarriorsWhoStayed;

  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventSetMessage(question + 1);
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(question + 2);
    Engine_EventShowMessage(actorId, 0);
  }
}

/// Searching the well outside the palace. "It's filled with fresh, clean
/// water."
static void BabiPalace_ExamineWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgTolbiItsFilledWithFreshClean, 1);
  Engine_EventEnd();
}

/// Returns the event table for the scene and the story, as the placements.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_BabiPalaceEntrance) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gBabiPalace_Events2AfterColosso;
    }
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      return gBabiPalace_Events2Colosso;
    }
    return gBabiPalace_Events2;
  }
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gBabiPalace_EventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    return gBabiPalace_EventsColosso;
  }
  return gBabiPalace_EventsOther;
}

/// Sets up the scene being entered: the doors, the Cloak Ball on show, and
/// the scenes that play on arrival (Babi after Colosso, the attendant during
/// the finals, waking after a night's rest). Returns 0.
s32 BabiPalace_ApplyEntryState(void) {
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  u8 *buf;
  s32 set;

  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(4);
  if (gGameState.entrance == BABI_PALACE_AFTER_BABI_SAVED) {
    GameFlag_Set(FLAG_BABI_SAVED);
  }
  if (gGameState.entrance == BABI_PALACE_AFTER_COLOSSO) {
    GameFlag_Set(FLAG_BABI_SAVED);
    GameFlag_Set(FLAG_COLOSSO_OVER);
  }
  if (gGameState.scene != (s32)&SceneId_BabiPalaceEntrance) {
    if (gGameState.entrance == BABI_PALACE_FROM_VALE_INTERIORS_INN) {
      GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    }
    if (GameFlag_IsSet(FLAG_COLOSSO_OVER)) {
      set = GameFlag_IsSet(FLAG_BABI_PALACE_CLOAK_BALL);
      if (set) {
        Actor_SetPosition(ACTOR_CLOAK_BALL, 0, 0);
      } else {
        actor = Actor_Get(ACTOR_CLOAK_BALL);
        actor->retainSpriteCache = 1;
        actor->motionFlags = set;
        sprite = actor->sprite;
        actor->y.fixed = 0x40000;
        sprite->count = set;
        sprite->image.fullColor = 0;
        sprite->image.palette = 0;
        buf = Heap_Allocate(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
        Item_LoadIcon(ITEM_CLOAK_BALL);
        Vram_Load(sprite->slot, 128, buf + 0x400);
        Heap_Release(HEAP_SLOT_GLYPH_TRANSFER);
      }
      if (gGameState.entrance == BABI_PALACE_THRONE_ROOM_FROM_CENTRAL_HALL &&
          !GameFlag_IsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
        GameFlag_Set(FLAG_BABI_SPOKE_AFTER_COLOSSO);
        Actor_SetPosition(ACTOR_BABI, PIXELS(208), PIXELS(704));
        BabiPalace_MeetBabiAfterColosso();
      }
      Actor_SetAnimation(ACTOR_BABI, 5);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_BABI), 0);
    } else if (GameFlag_IsSet(FLAG_BABI_SAVED) &&
               !GameFlag_IsSet(FLAG_BABI_PALACE_FINALS_REST_OFFERED)) {
      Actor_SetPosition(ACTOR_FINALS_ATTENDANT, PIXELS(120), PIXELS(72));
    }
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    // The attendant before the rescue and after Colosso; during the finals
    // ACTOR_FINALS_SECOND_RESIDENT has this number.
    Actor_Get(ACTOR_BED_ATTENDANT)->collisionFlags |= 4;
    if (gGameState.entrance == BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      BabiPalace_WakeAfterFinals();
      gGameState.entrance = 8;
    }
    if (gGameState.entrance == BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH) {
      BabiPalace_RestorePartyAndInventory();
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
      GameFlag_Set(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
      Actor_SetPosition(ACTOR_FINALS_ATTENDANT, PIXELS(56), PIXELS(120));
      Actor_FaceDirection(ACTOR_FINALS_ATTENDANT, 0xf000, 0);
      BabiPalace_WakeFromFinalsDream();
      gGameState.entrance = 8;
    }
  }
  return 0;
}

// clang-format off
u16 gBabiPalace_DoorCells[] = {
    MAP_CELL_STEP(25, 28, 1, 3, 5), MAP_CELL_STEP(26, 28, 1, 3, 5), MAP_CELL_STEP(27, 28, 1, 3, 5), MAP_CELL_STEPS_END,
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

struct SceneEntrance gBabiPalace_EntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_FROM_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, 768, 0, 200, FACING_NORTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_HALL_FROM_CENTRAL_HALL_WEST, CONDITION_ALWAYS, 728, 0, 88, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_HALL_FROM_CENTRAL_HALL_EAST, CONDITION_ALWAYS, 808, 0, 88, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_HALL_FROM_WEST_STAIRWELL, CONDITION_ALWAYS, 600, 0, 152, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_HALL_FROM_EAST_STAIRWELL, CONDITION_ALWAYS, 936, 0, 152, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_HALL_FROM_BIG_BEDROOM_WEST, CONDITION_ALWAYS, 584, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_HALL_FROM_BIG_BEDROOM_EAST, CONDITION_ALWAYS, 952, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_BIG_BEDROOM_FROM_STATUE_HALL_WEST, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_BIG_BEDROOM_FROM_STATUE_HALL_EAST, CONDITION_ALWAYS, 440, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_SCHOLARS_LANDING_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 72, 0, 968, FACING_EAST, 0, 0, 864, 240, 1024, 0 },
    { BABI_PALACE_FROM_VALE_INTERIORS_INN, CONDITION_ALWAYS, 168, 0, 936, FACING_SOUTH, 0, 0, 864, 240, 1024, 0 },
    { BABI_PALACE_TUNNEL_LANDING_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 424, 0, 680, FACING_WEST, 0, 328, 600, 568, 760, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_TUNNEL, CONDITION_ALWAYS, 472, 0, 696, FACING_SOUTH, 0, 328, 600, 568, 760, 0 },
    { BABI_PALACE_BEDROOM_LANDING_FROM_THRONE_ROOM, CONDITION_ALWAYS, 408, 0, 824, FACING_EAST, 0, 320, 728, 560, 888, 0 },
    { BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM, CONDITION_ALWAYS, 456, 0, 808, FACING_SOUTH, 0, 320, 728, 560, 888, 0 },
    { BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING, CONDITION_ALWAYS, 824, 0, 648, FACING_NORTH, 0, 768, 480, 1024, 672, 0 },
    { BABI_PALACE_WEST_STAIRWELL_FROM_STATUE_HALL, CONDITION_ALWAYS, 872, 0, 936, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { BABI_PALACE_WEST_STAIRWELL_FROM_WEST_OCTAGONAL_ROOM, CONDITION_ALWAYS, 888, 0, 968, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { BABI_PALACE_EAST_STAIRWELL_FROM_STATUE_HALL, CONDITION_ALWAYS, 760, 0, 936, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { BABI_PALACE_EAST_STAIRWELL_FROM_EAST_OCTAGONAL_ROOM, CONDITION_ALWAYS, 744, 0, 968, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_STATUE_HALL_WEST, CONDITION_ALWAYS, 440, 0, 584, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_STATUE_HALL_EAST, CONDITION_ALWAYS, 616, 0, 584, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_SCHOLARS_LANDING, CONDITION_ALWAYS, 328, 0, 312, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_TUNNEL_LANDING, CONDITION_ALWAYS, 728, 0, 312, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_NORTH_OCTAGONAL_ROOM, CONDITION_ALWAYS, 312, 0, 296, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_SHEBAS_LANDING, CONDITION_ALWAYS, 744, 0, 296, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_STOVE_ROOM, CONDITION_ALWAYS, 472, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_SOLDIERS_ROOM, CONDITION_ALWAYS, 584, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CENTRAL_HALL_FROM_THRONE_ROOM, CONDITION_ALWAYS, 528, 0, 296, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_THRONE_ROOM_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 208, 0, 840, FACING_NORTH, 0, 0, 624, 336, 864, 0 },
    { BABI_PALACE_THRONE_ROOM_FROM_BEDROOM_LANDING, CONDITION_ALWAYS, 40, 0, 760, FACING_WEST, 0, 0, 624, 336, 864, 0 },
    { BABI_PALACE_STOVE_ROOM_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 200, 0, 360, FACING_NORTH, 0, 0, 240, 256, 480, 0 },
    { BABI_PALACE_SOLDIERS_ROOM_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 824, 0, 360, FACING_NORTH, 0, 768, 240, 1024, 480, 0 },
    { BABI_PALACE_WEST_OCTAGONAL_ROOM_FROM_WEST_STAIRWELL, CONDITION_ALWAYS, 376, 0, 936, FACING_EAST, 0, 232, 864, 472, 1024, 0 },
    { BABI_PALACE_EAST_OCTAGONAL_ROOM_FROM_EAST_STAIRWELL, CONDITION_ALWAYS, 520, 0, 936, FACING_WEST, 0, 424, 864, 664, 1024, 0 },
    { BABI_PALACE_NORTH_OCTAGONAL_ROOM_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 904, 0, 776, FACING_EAST, 0, 776, 704, 1016, 864, 0 },
    { BABI_PALACE_SHEBAS_LANDING_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 216, 0, 552, FACING_WEST, 0, 0, 472, 304, 632, 0 },
    { BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM, CONDITION_ALWAYS, 88, 0, 552, FACING_SOUTH, 0, 0, 472, 304, 632, 0 },
    { BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING, CONDITION_ALWAYS, 728, 0, 824, FACING_NORTH, 0, 552, 656, 792, 848, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gBabiPalace_Entrances2[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 264, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 264, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_BABI_PALACE, CONDITION_ALWAYS, 200, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneRegion gBabiPalace_Regions[] = {
    { SCENE_TABLE_END },
};

u32 gBabiPalace_Exits[] = {
    SCENE_EXITS(BabiPalace),
    SCENE_EXIT(BABI_PALACE_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance, BABI_PALACE_ENTRANCE_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_WEST, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_STATUE_HALL_WEST),
    SCENE_EXIT(BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_EAST, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_STATUE_HALL_EAST),
    SCENE_EXIT(BABI_PALACE_STATUE_HALL_TO_WEST_STAIRWELL, BabiPalace, BABI_PALACE_WEST_STAIRWELL_FROM_STATUE_HALL),
    SCENE_EXIT(BABI_PALACE_STATUE_HALL_TO_EAST_STAIRWELL, BabiPalace, BABI_PALACE_EAST_STAIRWELL_FROM_STATUE_HALL),
    SCENE_EXIT(BABI_PALACE_STATUE_HALL_TO_BIG_BEDROOM_WEST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_HALL_WEST),
    SCENE_EXIT(BABI_PALACE_STATUE_HALL_TO_BIG_BEDROOM_EAST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_HALL_EAST),
    SCENE_EXIT(BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_WEST, BabiPalace, BABI_PALACE_STATUE_HALL_FROM_BIG_BEDROOM_WEST),
    SCENE_EXIT(BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_EAST, BabiPalace, BABI_PALACE_STATUE_HALL_FROM_BIG_BEDROOM_EAST),
    SCENE_EXIT(BABI_PALACE_SCHOLARS_LANDING_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_SCHOLARS_LANDING),
    SCENE_EXIT(BABI_PALACE_TO_VALE_INTERIORS_INN, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_TUNNEL_LANDING_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_TUNNEL_LANDING),
    SCENE_EXIT(BABI_PALACE_TO_COLOSSEUM_TUNNEL, ColosseumTunnel, COLOSSEUM_TUNNEL_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_BEDROOM_LANDING_TO_THRONE_ROOM, BabiPalace, BABI_PALACE_THRONE_ROOM_FROM_BEDROOM_LANDING),
    SCENE_EXIT(BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, BabiPalace, BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING),
    SCENE_EXIT(BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM),
    SCENE_EXIT(BABI_PALACE_WEST_STAIRWELL_TO_STATUE_HALL, BabiPalace, BABI_PALACE_STATUE_HALL_FROM_WEST_STAIRWELL),
    SCENE_EXIT(BABI_PALACE_WEST_STAIRWELL_TO_WEST_OCTAGONAL_ROOM, BabiPalace, BABI_PALACE_WEST_OCTAGONAL_ROOM_FROM_WEST_STAIRWELL),
    SCENE_EXIT(BABI_PALACE_EAST_STAIRWELL_TO_STATUE_HALL, BabiPalace, BABI_PALACE_STATUE_HALL_FROM_EAST_STAIRWELL),
    SCENE_EXIT(BABI_PALACE_EAST_STAIRWELL_TO_EAST_OCTAGONAL_ROOM, BabiPalace, BABI_PALACE_EAST_OCTAGONAL_ROOM_FROM_EAST_STAIRWELL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_WEST, BabiPalace, BABI_PALACE_STATUE_HALL_FROM_CENTRAL_HALL_WEST),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_EAST, BabiPalace, BABI_PALACE_STATUE_HALL_FROM_CENTRAL_HALL_EAST),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_SCHOLARS_LANDING, BabiPalace, BABI_PALACE_SCHOLARS_LANDING_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_TUNNEL_LANDING, BabiPalace, BABI_PALACE_TUNNEL_LANDING_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_NORTH_OCTAGONAL_ROOM, BabiPalace, BABI_PALACE_NORTH_OCTAGONAL_ROOM_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_STOVE_ROOM, BabiPalace, BABI_PALACE_STOVE_ROOM_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_SOLDIERS_ROOM, BabiPalace, BABI_PALACE_SOLDIERS_ROOM_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_CENTRAL_HALL_TO_THRONE_ROOM, BabiPalace, BABI_PALACE_THRONE_ROOM_FROM_CENTRAL_HALL),
    SCENE_EXIT(BABI_PALACE_THRONE_ROOM_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_THRONE_ROOM),
    SCENE_EXIT(BABI_PALACE_THRONE_ROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_THRONE_ROOM),
    SCENE_EXIT(BABI_PALACE_STOVE_ROOM_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_STOVE_ROOM),
    SCENE_EXIT(BABI_PALACE_SOLDIERS_ROOM_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_SOLDIERS_ROOM),
    SCENE_EXIT(BABI_PALACE_WEST_OCTAGONAL_ROOM_TO_WEST_STAIRWELL, BabiPalace, BABI_PALACE_WEST_STAIRWELL_FROM_WEST_OCTAGONAL_ROOM),
    SCENE_EXIT(BABI_PALACE_EAST_OCTAGONAL_ROOM_TO_EAST_STAIRWELL, BabiPalace, BABI_PALACE_EAST_STAIRWELL_FROM_EAST_OCTAGONAL_ROOM),
    SCENE_EXIT(BABI_PALACE_NORTH_OCTAGONAL_ROOM_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_NORTH_OCTAGONAL_ROOM),
    SCENE_EXIT(BABI_PALACE_SHEBAS_LANDING_TO_CENTRAL_HALL, BabiPalace, BABI_PALACE_CENTRAL_HALL_FROM_SHEBAS_LANDING),
    SCENE_EXIT(BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, BabiPalace, BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING),
    SCENE_EXIT(BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM),
    SCENE_EXITS(BabiPalaceEntrance),
    SCENE_EXIT(BABI_PALACE_ENTRANCE_TO_TOLBI, Tolbi, TOLBI_FROM_BABI_PALACE_ENTRANCE),
    SCENE_EXIT(BABI_PALACE_ENTRANCE_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_BABI_PALACE_ENTRANCE),
    SCENE_EXITS_END,
};

struct ScenePlacement gBabiPalace_Placements2[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalace_PlacementsOther[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, (s32)gBabiPalace_IndigestionScript,
        PIXELS(472), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(344), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(728), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x39, FLAG_IODEM_HEARD_SOLDIERS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(352), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(368), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalace_PlacementsColosso[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalace_PlacementsAfterColosso[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x34, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(553), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(532), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x39, FLAG_BABI_SPOKE_AFTER_COLOSSO, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(704), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBabiPalace_EventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_WEST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_WEST_STAIRWELL },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_EAST_STAIRWELL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_WEST },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_EAST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CENTRAL_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 11, CONDITION_ALWAYS, BABI_PALACE_TO_VALE_INTERIORS_INN },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CENTRAL_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_THRONE_ROOM },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STAIRWELL_TO_STATUE_HALL },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STAIRWELL_TO_WEST_OCTAGONAL_ROOM },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STAIRWELL_TO_STATUE_HALL },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STAIRWELL_TO_EAST_OCTAGONAL_ROOM },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_WEST },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_EAST },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_SCHOLARS_LANDING },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_TUNNEL_LANDING },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_NORTH_OCTAGONAL_ROOM },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_SHEBAS_LANDING },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_THRONE_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_THRONE_ROOM_TO_BEDROOM_LANDING },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_STOVE_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_OCTAGONAL_ROOM_TO_WEST_STAIRWELL },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_OCTAGONAL_ROOM_TO_EAST_STAIRWELL },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_OCTAGONAL_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CENTRAL_HALL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { EVENT_TOUCH, 60, FLAG_IODEM_HEARD_SOLDIERS, EVENT_SCRIPT(BabiPalace_HearSoldiersReport) },
    { EVENT_TALK, ACTOR_FIRST_RESIDENT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutSoldiers) },
    { EVENT_TALK, ACTOR_SECOND_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
    { EVENT_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ShowBeds) },
    { EVENT_TALK, ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutColosso) },
    { EVENT_TALK, ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_TalkToWarriorWithIndigestion) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
    { EVENT_TALK, ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
    { EVENT_TALK, ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
    { EVENT_TALK, ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
    { EVENT_TALK, ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
    { EVENT_TALK, ACTOR_BEDSIDE_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEDSIDE_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
    { EVENT_TALK, ACTOR_PITYING_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
    { EVENT_TALK, ACTOR_SHEBAS_MINDER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
    { EVENT_TALK, SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PITYING_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBAS_MINDER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SHEBA, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ReadShebasMind) },
    { SEARCH(SEARCH_UNNAMED), 70, IF_FLAG_SET(FLAG_BABI_PALACE_BEDS_OPEN), EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalace_EventsColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_WEST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_WEST_STAIRWELL },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_EAST_STAIRWELL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_WEST },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_EAST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CENTRAL_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 11, CONDITION_ALWAYS, BABI_PALACE_TO_VALE_INTERIORS_INN },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CENTRAL_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_THRONE_ROOM },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STAIRWELL_TO_STATUE_HALL },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STAIRWELL_TO_WEST_OCTAGONAL_ROOM },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STAIRWELL_TO_STATUE_HALL },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STAIRWELL_TO_EAST_OCTAGONAL_ROOM },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_WEST },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_EAST },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_SCHOLARS_LANDING },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_TUNNEL_LANDING },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_NORTH_OCTAGONAL_ROOM },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_SHEBAS_LANDING },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_THRONE_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_THRONE_ROOM_TO_BEDROOM_LANDING },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_STOVE_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_OCTAGONAL_ROOM_TO_WEST_STAIRWELL },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_OCTAGONAL_ROOM_TO_EAST_STAIRWELL },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_OCTAGONAL_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CENTRAL_HALL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { EVENT_TALK, ACTOR_FIRST_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
    { EVENT_TALK, ACTOR_FINALS_SECOND_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_SECOND_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
    { EVENT_TALK, ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferRest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
    { EVENT_TALK, ACTOR_FINALS_BEDSIDE_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_BEDSIDE_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
    { EVENT_TALK, ACTOR_FINALS_PITYING_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
    { EVENT_TALK, ACTOR_FINALS_SHEBAS_MINDER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
    { EVENT_TALK, SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_PITYING_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_SHEBAS_MINDER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
    { SEARCH(SEARCH_UNNAMED), 70, IF_FLAG_SET(FLAG_BABI_PALACE_FINALS_BEDS_OPEN), EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalace_EventsAfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_WEST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_CENTRAL_HALL_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_WEST_STAIRWELL },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_HALL_TO_EAST_STAIRWELL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_WEST },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_HALL_EAST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CENTRAL_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 11, CONDITION_ALWAYS, BABI_PALACE_TO_VALE_INTERIORS_INN },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CENTRAL_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_THRONE_ROOM },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STAIRWELL_TO_STATUE_HALL },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STAIRWELL_TO_WEST_OCTAGONAL_ROOM },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STAIRWELL_TO_STATUE_HALL },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STAIRWELL_TO_EAST_OCTAGONAL_ROOM },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_WEST },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_STATUE_HALL_EAST },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_SCHOLARS_LANDING },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_TUNNEL_LANDING },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_NORTH_OCTAGONAL_ROOM },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CENTRAL_HALL_TO_SHEBAS_LANDING },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_THRONE_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_THRONE_ROOM_TO_BEDROOM_LANDING },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_STOVE_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_OCTAGONAL_ROOM_TO_WEST_STAIRWELL },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_OCTAGONAL_ROOM_TO_EAST_STAIRWELL },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_OCTAGONAL_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CENTRAL_HALL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { EVENT_TALK, ACTOR_FIRST_RESIDENT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutMeetingBabi) },
    { EVENT_TALK, ACTOR_AFTER_COLOSSO_SECOND_RESIDENT, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutEasternShores) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_SECOND_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
    { EVENT_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
    { EVENT_TALK, ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
    { EVENT_TALK, ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
    { EVENT_TALK, ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
    { EVENT_TALK, ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
    { EVENT_TALK, ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
    { EVENT_TALK, ACTOR_AFTER_COLOSSO_BEDSIDE_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_BEDSIDE_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
    { EVENT_TALK, ACTOR_CLOAK_BALL, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferCloakBall) },
    { EVENT_TALK, ACTOR_AFTER_COLOSSO_PITYING_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
    { EVENT_TALK, ACTOR_AFTER_COLOSSO_SHEBAS_MINDER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_PITYING_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_SHEBAS_MINDER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
    { SEARCH(SEARCH_UNNAMED), 70, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalace_Events2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { EVENT_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
    { EVENT_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalace_Events2Colosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { EVENT_TALK, ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { EVENT_TALK, ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT_SCRIPT(BabiPalace_PassOnBabisWord) },
    { EVENT_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
    { EVENT_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ThankForSavingBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalace_Events2AfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { EVENT_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
    { EVENT_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutRemainingWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { SCENE_EVENTS_END },
};
// clang-format on
