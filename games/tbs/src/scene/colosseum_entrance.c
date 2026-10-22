/// @file
/// Tolbi's colosseum, where Colosso is fought, in three places: the Colosseum
/// Entrance outside its gates, the Colosseum Tunnel from Babi's Palace, and the
/// Colosseum itself, whose stands, waiting room and passages lead to the sites
/// of the three finals matches.
///
/// Colosso has two parts, and the code tells them apart by FLAG_BABI_SAVED.
/// While warriors fight their trials, Babi, the ruler of Tolbi, is missing.
/// Once the party has saved his life in Altmiller Cave, the trials are over and
/// Babi has picked Isaac for the finals himself: every scene here then plays
/// the finals day. FLAG_COLOSSO_OVER, set after the third finals match, closes
/// the colosseum until next year.
///
/// - Colosseum Entrance (SceneId_ColosseumEntrance), before the gates,
///   reached from Tolbi. During the trials the crowd cannot get in: its people
///   talk of the heats and of the Lucky Guess, a bet on which warrior will win
///   the finals, and Mind Read hears what they hope for. On the finals day
///   ColosseumEntrance_GetPlacements places a second crowd, with Isaac's
///   companions and four of the colosseum's soldiers, and the event table's
///   flag conditions give everyone new lines: the scalpers sell seats, and the
///   Lucky Guess man recognizes Isaac and will not let a finalist play
///   (FLAG_LUCKY_GUESS_REFUSED_ISAAC remembers that he said so). Stepping
///   toward the gates (Event_CrowdRecognizesIsaac) plays the soldiers' quarrel
///   over the boy Babi picked; they lead the party in, and if Isaac will not
///   go, they and his companions talk him round, Garet leading the way if he
///   still refuses. The party arrives at the Colosseum's
///   COLOSSEUM_FROM_COLOSSEUM_ENTRANCE. Once Colosso is over,
///   ColosseumEntrance_UpdateEntranceCells changes the map's cells before the
///   gates, and two people linger to say the colosseum is closed until next
///   year. COLOSSEUM_ENTRANCE_TO_TOLBI leads back to Tolbi.
/// - Colosseum Tunnel (SceneId_ColosseumTunnel), the passage from Babi's
///   Palace (BABI_PALACE_TO_COLOSSEUM_TUNNEL) into the Colosseum, whose end
///   of the tunnel (COLOSSEUM_TUNNEL_FROM_COLOSSEUM and
///   COLOSSEUM_FROM_COLOSSEUM_TUNNEL) takes the same area name. It has only its
///   two exits.
/// - Colosseum (SceneId_Colosseum), one map that holds the stands, the trial
///   grounds, the finals' waiting room and the passages to the finals stages,
///   each an entrance's camera box on it
///   (https://alchemy.pascalpixel.com/places/colosseum/camera-boxes.png).
///   Every arrival is an entrance of its own, and
///   ColosseumEntrance_ApplyArrivalState plays the cutscene that entrance
///   stands for. During the trials the stands' entrances
///   (COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST,
///   COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS) alternate with the two trial
///   cutscenes (COLOSSEUM_FAILED_TRIAL_FROM_STANDS,
///   COLOSSEUM_CLEARED_TRIAL_FROM_STANDS), which FLAG_COLOSSO_FAILED_TRIAL_SEEN
///   and FLAG_COLOSSO_CLEARED_TRIAL_SEEN record. On the finals day
///   COLOSSEUM_FROM_COLOSSEUM_ENTRANCE keeps the party's items
///   (InventorySnapshot_Take) for Babi's Palace to give back, and the waiting
///   room's line asks whether Isaac is ready (Event_FinalsBegin). Each finals
///   stage returns to its own entrance here
///   (COLOSSEUM_FROM_COLOSSEUM_*_MATCH_*), and FLAG_COLOSSO_FIRST_MATCH_WON and
///   FLAG_COLOSSO_SECOND_MATCH_WON choose which stage the long passages lead to
///   next; the stages themselves are colosseum_azart_match.c,
///   colosseum_satrage_match.c and colosseum_navampa_match.c.
///
/// The three scenes share one exit table, a run of exits for each scene, and
/// the Colosseum's exits are how its arrivals chain into one another and into
/// the finals stages.
///
/// Open questions:
/// - XXX (no evidence): what the Colosseum's entrances 3 and 8 were made for;
///   no exit or code enters by them, only the debug scene selector can, and
///   ColosseumEntrance_GetPlacements gives entrance 8 the soldiers' table, as
///   it gives the arrivals of the finals day.

#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/colosseum_entrance.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "field_obj.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"

static void ColosseumEntrance_ApplyArrivalState(void);
static void ColosseumEntrance_ShowMessageAndPause(s32 speaker);
static void ColosseumEntrance_FaceDirection(s32 actor, s32 facing);
static void ColosseumEntrance_UpdateEntranceCells(void);
static void ColosseumEntrance_ExplainPartySupport(void);
static void ColosseumEntrance_EscortToEastExit(void);
static void ColosseumEntrance_AnnounceFinals(void);
static void ColosseumEntrance_WalkToEastExit(void);
static void ColosseumEntrance_CelebrateAdvancement(void);
static void ColosseumEntrance_EscortToWestExit(void);
static void ColosseumEntrance_WalkToWestExit(void);
static void ColosseumEntrance_CelebrateBothWins(void);
static void ColosseumEntrance_InitializeContestants(void);

static inline void ColosseumEntrance_Turn(s32 actor, s32 facing) {
  /* FAKEMATCH(wrapper): Event_CrowdRecognizesIsaac's one turn goes through
   * this helper; as a direct ColosseumEntrance_FaceDirection call, 7 of 643
   * instructions of Event_CrowdRecognizesIsaac differ in tbs-en. */
  ColosseumEntrance_FaceDirection(actor, facing);
}

/// The Colosseum Entrance's people, numbered by the rows of
/// gColosseumEntrancePlacements; gColosseumEntrancePlacementsFinalsDay places
/// the same people in its first nine rows after Isaac's companions, who take
/// their own actor slots, and gColosseumEntrancePlacementsAfterColosso keeps
/// the first two. They are named by what they say during the trials, except
/// where the finals day says more about who they are.
enum ColosseumEntranceActor {
  /// A young man with brown hair in a purple shirt who talks of the heats.
  /// "They're holding the preliminary Colosso heats right now."
  /// ★ Named from the game: "They're holding the preliminary Colosso heats
  /// right now." (MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats)
  ACTOR_VILLAGER_PRELIMINARY_HEATS = ACTOR_FIRST_PLACED,
  /// A woman with fair hair who cannot get into the packed colosseum. "Sigh...
  /// The colosseum is packed with people. I'll never get in there."
  /// ★ Named from the game: "I'll never get in there."
  /// (MsgColosseumEntranceSighTheColosseumIsPackedWith)
  ACTOR_WOMAN_CANNOT_GET_IN,
  /// A burly grey-haired man who asks whether the party hopes to get in
  /// (Event_DiscussSeating). "Are you hoping to get in to see Colosso now?"
  /// ★ Named from the game: "Are you hoping to get in to see Colosso now?"
  /// (MsgColosseumHopingGetInSeeColosso)
  ACTOR_VILLAGER_SEATING,
  /// A woman in a green dress who talks of winning at the Lucky Guess. "If you
  /// hit it big in the Lucky Guess, you can win a pretty penny."
  /// ★ Named from the game: "you can win a pretty penny."
  /// (MsgColosseumEntranceIfYouHitItBigIn)
  ACTOR_WOMAN_PRETTY_PENNY,
  /// An old man with a cane who has bet on the Lucky Guess for years. "I've
  /// been betting on the Lucky Guess for many years, but I've never won."
  /// ★ Named from the game: "I've been betting on the Lucky Guess for many
  /// years" (MsgColosseumEntranceIveBeenBettingOnTheLucky)
  ACTOR_OLD_MAN_BETTING_FOR_YEARS,
  /// An old woman who walks about before the finals day. "Colosso's Lucky Guess
  /// makes the battles in the colosseum better."; on the finals day, "I want to
  /// watch the finals from a good spot, but it costs too much!"
  /// ★ Named from the game: "I want to watch the finals from a good spot"
  /// (MsgColosseumWantWatchFinalsFromGood)
  ACTOR_OLD_WOMAN_GOOD_SPOT,
  /// A strongman, one of the scalpers. During the trials he bets: "I want to
  /// win big, so I put it all on a warrior no one else has picked."; on the
  /// finals day he is busy with a customer, and Mind Read hears "I'm going to
  /// be in trouble if I get stuck with any unsold seats..."
  /// ★ Named from the game: "I'm going to be in trouble if I get stuck with any
  /// unsold seats..." (MsgColosseumEntranceImGoingToBeInTrouble)
  ACTOR_SCALPER,
  /// The Lucky Guess man, in a green cap and carrying a pack. "Do you know what
  /// the Lucky Guess is?"
  /// ★ Named from the game: "Do you know what the Lucky Guess is?"
  /// (MsgColosseumDoKnowLuckyGuess)
  ACTOR_LUCKY_GUESS_MAN = ACTOR_FIRST_PLACED + 8,
  /// The four soldiers at the gates on the finals day (SPRITE_SOLDIER, as in
  /// the waiting room), gColosseumEntrancePlacementsFinalsDay only. The
  /// first greets Isaac ("Hey, you're Isaac!") and leads the party in.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear,
  /// SPRITE_SOLDIER (its render)
  ACTOR_FIRST_GATE_SOLDIER
};

/// The Colosseum's stands, numbered by the rows of gColosseumStandsPlacements.
enum ColosseumStandsActor {
  /// A torch (SPRITE_TORCH).
  /// ★ Named from what the player sees: a torch on its stand (its render)
  ACTOR_STANDS_WEST_TORCH = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the other torch on its stand (its
  /// render)
  ACTOR_STANDS_EAST_TORCH,
  /// A young man with brown hair in a purple shirt who cheers on a small
  /// warrior. "Go for it! That's it! I'm cheering for you, little guy!"
  /// ★ Named from the game: "I'm cheering for you, little guy!"
  /// (MsgColosseumEntranceGoForItThatsItIm)
  ACTOR_SPECTATOR_CHEERING = ACTOR_FIRST_PLACED + 5,
  /// A woman with fair hair, excited by the trials. "Watching the trials makes
  /// me really excited. The battles are incredible!"
  /// ★ Named from the game: "Watching the trials makes me really excited."
  /// (MsgColosseumEntranceWatchingTheTrialsMakesMeReally)
  ACTOR_SPECTATOR_EXCITED,
  /// A burly grey-haired man, taken with the warriors. "Watching such proud,
  /// skilled warriors in battle is so enchanting."
  /// ★ Named from the game: "Watching such proud, skilled warriors in battle is
  /// so enchanting." (MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn)
  ACTOR_SPECTATOR_ENCHANTED,
  /// A woman in a green dress who has waited a year for Colosso. "I've waited a
  /// whole year to see Colosso. I can't wait for the finals!"
  /// ★ Named from the game: "I've waited a whole year to see Colosso."
  /// (MsgColosseumEntranceIveWaitedAWholeYearTo)
  ACTOR_SPECTATOR_WAITED_A_YEAR,
  /// The attendant by the trials, a bald, muscular man, who shows the party two
  /// warriors' trials.
  /// ★ Named from the game: "That warrior cleared this trial"
  /// (MsgColosseumHavePerfectTechniquesLikeThose)
  ACTOR_TRIAL_ATTENDANT
};

/// The Colosseum's trial grounds, numbered by the rows of
/// gColosseumTrialGroundsPlacements.
enum ColosseumTrialActor {
  /// Six torches (SPRITE_TORCH) around the trial.
  /// ★ Named from what the player sees: six lit torches on stands around the
  /// trial ground (its render)
  ACTOR_GROUNDS_FIRST_TORCH = ACTOR_FIRST_PLACED,
  /// Five warriors, 14 to 18, in the finalists' sprite
  /// (SPRITE_WARRIOR_IN_GOLDEN_HELMET); two of them take the trials the party
  /// is shown.
  /// ★ Named from the game: "That warrior failed his trial"
  /// (MsgColosseumHaveMakeThroughCountlessMatches)
  ACTOR_FIRST_TRIAL_WARRIOR = ACTOR_FIRST_PLACED + 6,
  /// The warrior the party first watches, of whom the attendant then says
  /// "That warrior failed his trial, so he's disqualified here."
  /// ★ Named from the game: "That warrior failed his trial"
  /// (MsgColosseumHaveMakeThroughCountlessMatches)
  ACTOR_FAILING_WARRIOR = ACTOR_FIRST_TRIAL_WARRIOR + 2,
  /// The warrior the party watches second, of whom the attendant then says
  /// "That warrior cleared this trial, but I doubt he'll make the finals."
  /// ★ Named from the game: "That warrior cleared this trial"
  /// (MsgColosseumHavePerfectTechniquesLikeThose)
  ACTOR_CLEARING_WARRIOR,
  /// Two young men with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, drawn
  /// mirrored) who judge the trial with flags: a red flag when the warrior
  /// fails and a white one when he clears it (enum FlagBearerAnimation). No
  /// text names them.
  /// ★ Named from what the player sees: two young men with fair hair in white
  /// tunics beside the trial, who raise a flag at its end (its render)
  ACTOR_FIRST_FLAG_BEARER = ACTOR_FIRST_PLACED + 11,
  /// A capped pillar (SPRITE_CAPPED_PILLAR) the clearing warrior heaves up and
  /// drops.
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap (its render)
  ACTOR_TRIAL_PILLAR = ACTOR_FIRST_PLACED + 13
};

/// The flag bearers' animations (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, spr_165.tsv).
enum FlagBearerAnimation {
  /// ★ Named from what the player sees: a young man raising a red flag (its
  /// render)
  FLAG_BEARER_ANIM_RED_FLAG = 5, ///< raises a red flag: the warrior failed
  /// ★ Named from what the player sees: a young man raising a white flag (its
  /// render)
  FLAG_BEARER_ANIM_WHITE_FLAG = 6 ///< raises a white flag: he cleared it
};

/// The four soldiers of gColosseumSoldiersPlacements (SPRITE_SOLDIER),
/// who brief the party at COLOSSEUM_FROM_COLOSSEUM_ENTRANCE, congratulate Isaac
/// after his wins and lead him through the passages; Isaac's companions take
/// their own actor slots.
enum ColosseumSoldierActor {
  /// ★ Named from what the player sees: soldiers in helmets with spears
  /// (its render)
  ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
  /// The last, who leads Isaac on to each finals stage.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  ACTOR_ESCORTING_SOLDIER = ACTOR_FIRST_PLACED + 3
};

/// The Colosseum's waiting room (Colosseum,
/// COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST), numbered by the rows of
/// gColosseumPlacementsFromColosseumAzartMatchWest: four soldiers and
/// the seven warriors who cleared the trials, as they introduce themselves.
enum ColosseumWaitingRoomActor {
  /// Asks whether Isaac is ready for the finals.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  ACTOR_FINALS_SOLDIER = ACTOR_FIRST_PLACED,
  /// Will not let Isaac leave. "You cannot leave until the finals are over."
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  ACTOR_EXIT_SOLDIER,
  /// Has waited long for the finals. "I feel we've been waiting forever for
  /// the finals to begin."
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  ACTOR_WAITING_SOLDIER,
  /// Tells of the gong. "When the gong sounds, the finals will begin."
  /// ★ Named from the game: "When the gong sounds, the finals will begin."
  /// (MsgColosseumEntranceWhenTheGongSoundsTheFinals)
  ACTOR_GONG_SOLDIER,
  /// The highest-ranked.
  /// ★ Named from the game: "I am Morgan."
  /// (MsgColosseumEntranceIAmMorganIAmThe)
  ACTOR_MORGAN,
  /// The second-best.
  /// ★ Named from the game: "The name's Azart."
  /// (MsgColosseumImRatedAsSecondBest)
  ACTOR_AZART,
  /// ★ Named from the game: "I'm Galahad, the third-seeded fighter."
  /// (MsgColosseumWaahDontFrightenMe)
  ACTOR_GALAHAD,
  /// ★ Named from the game: "My name is Satrage."
  /// (MsgColosseumWhoHaveComeQuestionMe)
  ACTOR_SATRAGE,
  /// ★ Named from the game: "Dekka must win the finals."
  /// (MsgColosseumDekkaMustWinFinalsDekka)
  ACTOR_DEKKA,
  /// ★ Named from the game: "I am Navampa of Gondowan"
  /// (MsgColosseumAmNavampaGondowanSixthRanked)
  ACTOR_NAVAMPA,
  /// ★ Named from the game: "I'm Buford, the seventh seed."
  /// (MsgColosseumImBufordSeventhSeed)
  ACTOR_BUFORD
};

/// The touch triggers the event tables answer, as the map's collision codes.
enum ColosseumEntranceTrigger {
  /// Before the gates at the Colosseum Entrance (Event_CrowdRecognizesIsaac).
  TRIGGER_BEFORE_GATES = 10,
  /// The line before the waiting room's east passage (Event_FinalsBegin).
  TRIGGER_FINALS_LINE = 12,
  /// The cells behind it (Event_RearmFinalsStart).
  TRIGGER_BEHIND_FINALS_LINE = 13
};

/// The temporary flags the colosseum's scenes keep.
enum {
  /// The Lucky Guess man has refused Isaac as a finalist
  /// (Event_OfferLuckyGuess).
  /// ★ Named from the game: "Colosso's Lucky Guess makes the battles in the
  /// colosseum better." (MsgColosseumColossosLuckyGuessMakesBattles)
  FLAG_LUCKY_GUESS_REFUSED_ISAAC = FLAG_PLACE_TEMP_C0,
  /// Navampa has explained that Dekka will not answer (Event_TalkToDekka).
  /// ★ Named from the game: "Dekka must win the finals."
  /// (MsgColosseumDekkaMustWinFinalsDekka)
  FLAG_NAVAMPA_SPOKE_FOR_DEKKA = FLAG_PLACE_TEMP_C1,
  /// The leader has crossed the cells behind the finals line, so stepping onto
  /// it asks whether Isaac is ready.
  /// ★ Named from the game: "If you win there, you advance to the finals."
  /// (MsgTolbiTheTrialsAreStartingAtThe)
  FLAG_FINALS_LINE_ARMED = FLAG_SCENE_TEMP_34,
  /// The leader has stepped onto the line since; its touch is off.
  /// ★ Named from the game: "If you win there, you advance to the finals."
  /// (MsgTolbiTheTrialsAreStartingAtThe)
  FLAG_FINALS_LINE_REACHED = FLAG_SCENE_TEMP_35
};

const struct SceneEntrance *ColosseumEntrance_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Colosseum) {
    return gColosseumEntrances;
  }
  if (scene == (s32)&SceneId_ColosseumEntrance) {
    return gColosseumEntranceEntrances;
  }
  return gColosseumEntranceEntrancesOther;
}

const struct SceneRegion *ColosseumEntrance_GetRegions(void) { return 0; }

/// Returns the shared colosseum exits.
const u32 *ColosseumEntrance_GetExits(void) { return gColosseumEntranceExits; }

/// Selects the actors: in the Colosseum by the entrance the party arrives at
/// (the stands, the trial grounds, the passages, the waiting room or the
/// soldiers' briefing), at the Colosseum Entrance by how far Colosso has gone.
const struct ScenePlacement *ColosseumEntrance_GetPlacements(void) {
  const struct ScenePlacement *table;

  if (gGameState.scene == (s32)&SceneId_Colosseum) {
    switch (gGameState.entrance) {
    case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
    case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
      table = gColosseumStandsPlacements;
      break;
    case COLOSSEUM_FAILED_TRIAL_FROM_STANDS:
    case COLOSSEUM_CLEARED_TRIAL_FROM_STANDS:
      table = gColosseumTrialGroundsPlacements;
      break;
    // Entrance 8 has a table row but no exit leads to it (see the open
    // questions).
    case 8:
    case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
    case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH:
    case COLOSSEUM_FROM_COLOSSEUM_ENTRANCE:
    case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
    case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST:
      table = gColosseumSoldiersPlacements;
      break;
    case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
      table = gColosseumPlacementsFromColosseumAzartMatchWest;
      break;
    case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
    case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH:
      table = gColosseumPlacementsFromColosseumNavampaMatch;
      break;
    default:
      table = gColosseumPlacements;
      break;
    }
  } else if (gGameState.scene == (s32)&SceneId_ColosseumEntrance) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      table = gColosseumEntrancePlacementsAfterColosso;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      table = gColosseumEntrancePlacementsFinalsDay;
    } else {
      table = gColosseumEntrancePlacements;
    }
  } else {
    table = gColosseumEntrancePlacementsOther;
  }
  return table;
}

const struct SceneEvent *ColosseumEntrance_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ColosseumTunnel) {
    return gColosseumTunnelEvents;
  }
  if (scene == (s32)&SceneId_Colosseum) {
    if (gGameState.entrance == COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST) {
      return gColosseumEventsFromColosseumAzartMatchWest;
    }
    return gColosseumEvents;
  }
  if (scene == (s32)&SceneId_ColosseumEntrance) {
    return gColosseumEntranceEvents;
  }
  return gColosseumEntranceEventsOther;
}

/// At the Colosseum Entrance: during the trials, a burly grey-haired man asks
/// whether the party hopes to get in to see Colosso; on the finals day
/// (FLAG_BABI_SAVED) the colosseum is already full, and only the scalpers have
/// seats.
static void Event_DiscussSeating(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
    Engine_EventSetMessage(MsgColosseumColosseumAlreadyFullIfReally);
    Engine_EventShowMessage(ACTOR_VILLAGER_SEATING, 0);
  } else {
    Engine_EventSetMessage(MsgColosseumHopingGetInSeeColosso);
    Engine_EventAskYesNo(ACTOR_VILLAGER_SEATING, 0);
  }
  Engine_EventEnd();
}

/// At the Colosseum Entrance, an old woman: the Lucky Guess makes the battles
/// better; on the finals day, a good spot costs too much.
static void Event_DiscussLuckyGuessSeats(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
    Engine_ActorShowEmote(ACTOR_OLD_WOMAN_GOOD_SPOT,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Engine_EventSetMessage(MsgColosseumWantWatchFinalsFromGood);
    Engine_EventShowMessage(ACTOR_OLD_WOMAN_GOOD_SPOT, 0);
  } else {
    Engine_EventSetMessage(MsgColosseumColossosLuckyGuessMakesBattles);
    Engine_EventShowMessage(ACTOR_OLD_WOMAN_GOOD_SPOT, 0);
  }
  Engine_EventEnd();
}

/// At the Colosseum Entrance: a strongman, betting, put it all on a warrior no
/// one picked; on the finals day a scalper, busy with a customer, asks if the
/// party wants seats.
static void Event_DiscussLuckyGuessWinnings(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_ActorRunRepeatedMotion(ACTOR_SCALPER, 2);
    Engine_EventSetMessage(MsgColosseumHoldOnSecond);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_SCALPER);
    Engine_ActorFaceEachOther(ACTOR_SCALPER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventAskYesNo(ACTOR_SCALPER, 0);
    ColosseumEntrance_FaceDirection(ACTOR_SCALPER, FACING_EAST);
  } else {
    Engine_EventSetMessage(MsgColosseumWantWinBigPutOn);
    Engine_EventShowMessage(ACTOR_SCALPER, 0);
  }
  Engine_EventEnd();
}

/// The Lucky Guess man at the Colosseum Entrance explains the game; on the
/// finals day he recognizes Isaac as Babi's pick and refuses him, as finalists
/// may not play (FLAG_LUCKY_GUESS_REFUSED_ISAAC: said once).
static void Event_OfferLuckyGuess(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_BABI_SAVED) != 0) {
    if (GameFlag_IsSet(FLAG_LUCKY_GUESS_REFUSED_ISAAC) != 0) {
      Engine_EventSetMessage(MsgColosseumLookImSorryButWe);
      Event_ShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
    } else {
      Engine_EventSetMessage(MsgColosseumWantTryOutLuckyGuess);
      Event_OpenMessage(ACTOR_LUCKY_GUESS_MAN, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        gEventWork->message++;
        Actor_ShowEmote(ACTOR_LUCKY_GUESS_MAN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        Event_OpenMessage(ACTOR_LUCKY_GUESS_MAN, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
          gEventWork->message++;
        }
        Engine_EventWait(40);
        Event_ShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
        GameFlag_Set(FLAG_LUCKY_GUESS_REFUSED_ISAAC);
      } else {
        Event_ShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
      }
    }
  } else {
    Engine_EventSetMessage(MsgColosseumDoKnowLuckyGuess);
    Event_AskYesNo(ACTOR_LUCKY_GUESS_MAN, 0);
  }
  Engine_EventEnd();
}

/// Azart won't lose to someone who skipped the trials. He waits in the
/// Colosseum's waiting room as the second-best, and goes back to pacing after
/// he has said so.
static void Event_TalkToAzart(void) {
  struct FieldActor *contestant;

  contestant = Object_GetById(ACTOR_AZART);
  Engine_EventBegin();
  Engine_ActorStop(ACTOR_AZART);
  Engine_ActorFaceEachOther(ACTOR_AZART, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumImRatedAsSecondBest);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_AZART);
  Engine_ActorRunRepeatedMotion(ACTOR_AZART, 1);
  Engine_EventShowMessage(ACTOR_AZART, 0);
  // Give his wander back its home, where he is placed: ACTOR_WANDER reads
  // timer and work as homeX and homeZ (ScriptObjectRuntime), in pixels.
  {
    u16 *timer = (u16 *)&contestant->timer;
    u16 duration = 720;

    *timer = duration;
  }
  {
    u16 *work = &contestant->work;
    u16 value = 112;

    *work = value;
  }
  Actor_SetBuiltinAction(ACTOR_AZART, ACTOR_WANDER);
  Engine_EventEnd();
}

/// In the Colosseum's waiting room, Galahad, the third seed, jumps.
/// "Waah! Don't frighten me!"
static void Event_TalkToGalahad(void) {
  Engine_EventBegin();
  Actor_SetAttachedEffect(ACTOR_GALAHAD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorRunRepeatedMotion(ACTOR_GALAHAD, 2);
  Engine_EventSetMessage(MsgColosseumWaahDontFrightenMe);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_GALAHAD);
  Actor_ShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_GALAHAD, 0);
  Engine_EventEnd();
}

/// In the Colosseum's waiting room, Satrage, the fourth-ranked, opens his eyes
/// and names himself.
static void Event_TalkToSatrage(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgColosseumWhoHaveComeQuestionMe);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
  Engine_ActorFaceEachOther(ACTOR_SATRAGE, ACTOR_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
  Engine_ActorSetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_SATRAGE, 0);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
  ColosseumEntrance_FaceDirection(ACTOR_SATRAGE, FACING_SOUTH + FACING_STEP);
  Engine_EventEnd();
}

/// In the Colosseum's waiting room, Dekka psyches himself up; the first time,
/// Navampa explains he will not answer (FLAG_NAVAMPA_SPOKE_FOR_DEKKA).
static void Event_TalkToDekka(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_DEKKA, 2);
  Engine_EventSetMessage(MsgColosseumDekkaMustWinFinalsDekka);
  Event_ShowMessageAndWait(ACTOR_DEKKA, 0, 20);
  if (GameFlag_IsSet(FLAG_NAVAMPA_SPOKE_FOR_DEKKA) != 0) {
    Engine_EventWait(20);
  } else {
    ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_EAST);
    Engine_ActorRunRepeatedMotion(ACTOR_NAVAMPA, 1);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
    Engine_ActorFaceEachOther(ACTOR_NAVAMPA, ACTOR_PARTY_LEADER, 20);
    Engine_ActorSetAnimation(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
    Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
    ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH + FACING_STEP);
    GameFlag_Set(FLAG_NAVAMPA_SPOKE_FOR_DEKKA);
  }
  Engine_EventEnd();
}

/// In the Colosseum's waiting room, Navampa of Gondowan, the sixth-ranked,
/// introduces himself.
static void Event_TalkToNavampa(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_NAVAMPA, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumAmNavampaGondowanSixthRanked);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
  Engine_ActorRunRepeatedMotion(ACTOR_NAVAMPA, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
  ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH + FACING_STEP);
  Engine_EventEnd();
}

/// In the Colosseum's waiting room, Buford, the seventh seed: anyone could win
/// today.
static void Event_TalkToBuford(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_BUFORD, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumImBufordSeventhSeed);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_BUFORD);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_WEST, 40);
  Engine_ActorFaceEachOther(ACTOR_BUFORD, ACTOR_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_BUFORD);
  Engine_ActorSetAnimationAndWait(ACTOR_BUFORD, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_BUFORD);
  ColosseumEntrance_FaceDirection(ACTOR_BUFORD, FACING_SOUTH + FACING_STEP);
  Engine_EventEnd();
}

/// In the Colosseum's waiting room, the soldier asks whether Isaac is ready
/// for the finals.
static void Event_AskIfReadyForFinals(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_FINALS_SOLDIER, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumYouReadyForFinals);
  Engine_EventOpenMessage(ACTOR_FINALS_SOLDIER, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0)
    gEventWork->message++;
  Engine_EventShowMessage(ACTOR_FINALS_SOLDIER, 0);
  Engine_EventEnd();
}

/// Stepping toward the gates at the Colosseum Entrance (TRIGGER_BEFORE_GATES)
/// on the finals day: four soldiers recognize Isaac as Babi's pick and quarrel
/// over a boy in the finals, then one takes the party in to the Colosseum's
/// COLOSSEUM_FROM_COLOSSEUM_ENTRANCE. If Isaac will not go, the soldiers
/// insist and Ivan, Mia and Garet talk him round; if he still refuses, Garet
/// leads the way in. Before Babi is saved it does nothing.
static void Event_CrowdRecognizesIsaac(void) {
  // The native sequence reuses this word for actor handles and the reply.
  s32 actorOrAnswer;

  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) == 0) {
    return;
  }

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
  Actor_FaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_EAST, 60);
  Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_EventSetMessage(MsgColosseumHeyYouRe);
  Engine_ActorStartRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 2);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 1,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 2,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(256), -1, PIXELS(172), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 1, EMOTE_IN_FRONT | EMOTE_WINCE,
                  40);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 1);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
  Actor_SetAttachedEffect(ACTOR_FIRST_GATE_SOLDIER + 2,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER + 2, FACING_EAST);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 2);
  Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 3,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3, FACING_EAST);
  Engine_ActorStartRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 3, 2);
  Engine_EventShowMessageAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_NORTHWEST + FACING_STEP,
                      20);
  Engine_EventShowMessageAndWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 1, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 1,
                                  ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  // Gather Isaac and his companions for the invitation.
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_WEST);
  Engine_EventShowMessageAndWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 2, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 2,
                                  ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 3, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 1, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 2, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, ANIM_NOD);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 1,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 2,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_FIRST_GATE_SOLDIER, 258, 172);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 188);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  actorOrAnswer = (s32)Object_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Engine_ActorSetPosition(GARET,
                            ((struct FieldActor *)actorOrAnswer)->x.fixed,
                            ((struct FieldActor *)actorOrAnswer)->z.fixed);
  }
  actorOrAnswer = (s32)Object_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Engine_ActorSetPosition(IVAN, ((struct FieldActor *)actorOrAnswer)->x.fixed,
                            ((struct FieldActor *)actorOrAnswer)->z.fixed);
  }
  actorOrAnswer = (s32)Object_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Engine_ActorSetPosition(MIA, ((struct FieldActor *)actorOrAnswer)->x.fixed,
                            ((struct FieldActor *)actorOrAnswer)->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorWalkTo(GARET, 246, 200);
  Actor_WalkTo(IVAN, 262, 200);
  Actor_WalkToAndWait(MIA, 278, 200);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
  Event_OpenMessage(ACTOR_FIRST_GATE_SOLDIER, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);

  // Either leave with the guide or let the companions persuade Isaac.
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
    Actor_SetActionScript(GARET, gColosseumEntrance_GatherIntoIsaacScript);
    Actor_SetActionScript(IVAN, gColosseumEntrance_GatherIntoIsaacScript);
    Actor_RunActionScriptAndWait(MIA, gColosseumEntrance_GatherIntoIsaacScript);
    Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
    Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
    Actor_SetActionScript(ACTOR_FIRST_GATE_SOLDIER,
                          gColosseumEntrance_EnterGatesScript);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_SetActionScript(ACTOR_PARTY_LEADER,
                          gColosseumEntrance_EnterGatesScript);
    Engine_EventWait(80);
    gEventWork->transitionFrames = 40;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  } else {
    Event_SkipMessages(1);
    Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
    Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 1, EMOTE_IN_FRONT | EMOTE_WINCE,
                    40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 1);
    Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 2, EMOTE_IN_FRONT | EMOTE_WINCE,
                    40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 2);
    Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 3, EMOTE_IN_FRONT | EMOTE_WINCE,
                    40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 2);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_SHAKE_HEAD);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(20);
    ColosseumEntrance_ShowMessageAndPause(IVAN);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(MIA);
    Actor_FaceDirection(ACTOR_FIRST_GATE_SOLDIER,
                        FACING_NORTHWEST + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 2, FACING_WEST, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3, FACING_EAST, 60);
    Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 2,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3,
                              FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 1, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 2, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, ANIM_NOD);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    ColosseumEntrance_Turn(MIA, FACING_NORTHWEST);
    ColosseumEntrance_ShowMessageAndPause(MIA);
    Engine_ActorSetAnimation(GARET, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_OpenMessage(GARET, 0);

    actorOrAnswer = Engine_EventChooseYesNo(0, 0);
    if (actorOrAnswer == 0) {
      Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
      Actor_SetActionScript(GARET, gColosseumEntrance_GatherIntoIsaacScript);
      Actor_SetActionScript(IVAN, gColosseumEntrance_GatherIntoIsaacScript);
      Actor_RunActionScriptAndWait(MIA,
                                   gColosseumEntrance_GatherIntoIsaacScript);
      Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
      Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(ACTOR_FIRST_GATE_SOLDIER,
                            gColosseumEntrance_EnterGatesScript);
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(ACTOR_PARTY_LEADER,
                            gColosseumEntrance_EnterGatesScript);
      Engine_EventWait(80);
      gEventWork->transitionFrames = 40;
      Engine_EventCloseScreen();
      Engine_EventWaitForScreen();
    } else {
      Event_SkipMessages(1);
      Engine_EventWait(20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      ColosseumEntrance_ShowMessageAndPause(IVAN);
      ColosseumEntrance_FaceDirection(MIA, FACING_WEST);
      Engine_ActorSetAnimation(MIA, ANIM_NOD);
      ColosseumEntrance_ShowMessageAndPause(MIA);
      Actor_SetActionScript(IVAN, gColosseumEntrance_GatherIntoGaretScript);
      Actor_RunActionScriptAndWait(MIA,
                                   gColosseumEntrance_GatherIntoGaretScript);
      Engine_EventWait(20);
      Actor_RunActionScriptAndWait(ACTOR_PARTY_LEADER,
                                   gColosseumEntrance_GatherIntoGaretScript);
      Actor_WalkToAndWait(GARET, 262, 188);
      ColosseumEntrance_FaceDirection(GARET, FACING_NORTH);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
      ColosseumEntrance_ShowMessageAndPause(GARET);
      Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
      Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(ACTOR_FIRST_GATE_SOLDIER,
                            gColosseumEntrance_EnterGatesScript);
      Engine_EventWait(10);
      Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(GARET, gColosseumEntrance_EnterGatesScript);
      Engine_EventWait(80);
      gEventWork->transitionFrames = 40;
      Engine_EventCloseScreen();
      Engine_EventWaitForScreen();
    }
  }

  Engine_EventRequestExit(COLOSSEUM_ENTRANCE_TO_COLOSSEUM);
  Engine_GameFlagSet(FLAG_COLOSSEUM_FINALS_ENTERED);
  Engine_EventEnd();
}

/// Stepping to the line in the Colosseum's waiting room (TRIGGER_FINALS_LINE,
/// while FLAG_FINALS_LINE_ARMED is set and FLAG_FINALS_LINE_REACHED clear):
/// asked whether he is ready, Isaac can still wait; if he is, the warriors
/// grumble, line up by rank and march out, and Isaac follows them out by the
/// east passage (COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST).
static void Event_FinalsBegin(void) {
  struct FieldActor *actor;
  const s32 *script;

  if (Engine_GameFlagIsSet(FLAG_FINALS_LINE_ARMED) != 0) {
    Engine_GameFlagSet(FLAG_FINALS_LINE_REACHED);
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 872, 120);
    ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
    Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    Camera_MoveTo(PIXELS(816), -1, PIXELS(112), 1);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(ACTOR_FINALS_SOLDIER, FACING_SOUTHEAST + FACING_STEP,
                        20);
    Engine_ActorRunRepeatedMotion(ACTOR_FINALS_SOLDIER, 1);
    Engine_EventSetMessage(MsgColosseumYouReadyForFinals);
    Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_FINALS_SOLDIER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      ColosseumEntrance_FaceDirection(ACTOR_MORGAN, FACING_NORTH + FACING_STEP);
      Engine_ActorRunRepeatedMotion(ACTOR_MORGAN, 1);
      Engine_EventSetMessage(MsgColosseumDayTiredWaiting);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            ACTOR_MORGAN);
      ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_EAST);
      ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST);
      Engine_ActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_ActorSetAnimation(ACTOR_SATRAGE, ANIM_STAND);
      Engine_ActorFaceDirection(ACTOR_SATRAGE, FACING_NORTH + FACING_STEP, 20);
      Engine_ActorRunRepeatedMotion(ACTOR_SATRAGE, 1);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
      ColosseumEntrance_FaceDirection(ACTOR_DEKKA, FACING_WEST);
      Engine_ActorSetAnimationAndWait(ACTOR_DEKKA, ANIM_NOD);
      Engine_ActorRunRepeatedMotion(ACTOR_NAVAMPA, 2);
      ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_NORTHWEST);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            ACTOR_NAVAMPA);
      ColosseumEntrance_FaceDirection(ACTOR_BUFORD,
                                      FACING_NORTHWEST + FACING_STEP);
      Engine_ActorStartRepeatedMotion(ACTOR_BUFORD, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            ACTOR_BUFORD);
      ColosseumEntrance_FaceDirection(ACTOR_GONG_SOLDIER,
                                      FACING_SOUTHEAST + FACING_STEP);
      Engine_ActorSetAttachedEffect(
          ACTOR_GONG_SOLDIER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Engine_EventWait(60);
      Engine_ActorStartRepeatedMotion(ACTOR_GONG_SOLDIER, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_BELOW |
                                            ACTOR_GONG_SOLDIER);
      Engine_ActorStop(ACTOR_AZART);
      Engine_TaskWait(1);
      Engine_ActorStartRepeatedMotion(ACTOR_AZART, 2);
      Engine_ActorStartRepeatedMotion(ACTOR_GALAHAD, 2);
      Engine_ActorRunRepeatedMotion(ACTOR_DEKKA, 2);
      Engine_EventWait(20);
      Engine_ActorFaceDirection(ACTOR_AZART, FACING_EAST, 0);
      Engine_ActorFaceDirection(ACTOR_GALAHAD, FACING_WEST, 0);
      Engine_ActorFaceDirection(ACTOR_DEKKA, FACING_NORTHWEST + FACING_STEP,
                                40);
      Engine_ActorSetAnimation(ACTOR_AZART, ANIM_NOD);
      Engine_ActorSetAnimation(ACTOR_GALAHAD, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(ACTOR_DEKKA, ANIM_NOD);
      Actor_SetSpeed(ACTOR_AZART, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_SetSpeed(ACTOR_GALAHAD, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_SetSpeed(ACTOR_DEKKA, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_SetActionScript(ACTOR_AZART,
                            gColosseumEntrance_ApproachFinalsGroupScript);
      Actor_SetActionScript(ACTOR_DEKKA,
                            gColosseumEntrance_JoinFinalsFromSouthScript);
      Engine_EventWait(20);
      Engine_ActorFaceDirection(ACTOR_SATRAGE, FACING_NORTH + FACING_STEP, 0);
      Engine_ActorFaceDirection(ACTOR_NAVAMPA, FACING_NORTHWEST + FACING_STEP,
                                0);
      Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
      Engine_ActorFaceDirection(ACTOR_MORGAN, FACING_NORTH + FACING_STEP, 0);
      Engine_ActorFaceDirection(ACTOR_BUFORD, FACING_NORTHWEST + FACING_STEP,
                                0);
      Actor_RunActionScriptAndWait(ACTOR_GALAHAD,
                                   gColosseumEntrance_JoinFinalsFromEastScript);
      Engine_EventWait(20);
      Engine_ActorFaceDirection(ACTOR_FINALS_SOLDIER, FACING_EAST, 0);
      Engine_ActorFaceDirection(ACTOR_GONG_SOLDIER, FACING_WEST, 40);
      Engine_ActorSetAnimation(ACTOR_FINALS_SOLDIER, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(ACTOR_GONG_SOLDIER, ANIM_NOD);
      Engine_CameraMoveTo(PIXELS(808), -1, PIXELS(86), 1);
      Engine_CameraWaitForMove();
      Engine_ActorRunRepeatedMotion(ACTOR_FINALS_SOLDIER, 2);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_FINALS_SOLDIER);
      Engine_ActorSetSpeed(ACTOR_FINALS_SOLDIER, PIXELS(1), PIXELS(1) / 2);
      Object_GetById(ACTOR_FINALS_SOLDIER)->steeringFlags &=
          ~ACTOR_FACES_MOTION;
      Engine_ActorWalkToAndWait(ACTOR_FINALS_SOLDIER, 792, 72);
      Engine_EventWait(1);
      Object_GetById(ACTOR_FINALS_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
      ColosseumEntrance_FaceDirection(ACTOR_FINALS_SOLDIER, FACING_EAST);
      // Azart stops pacing before the procession.
      Engine_ActorStop(ACTOR_AZART);
      actor = Object_GetById(ACTOR_AZART);
      actor->update = NULL;
      actor->timer = 0;
      actor->work = 0;
      SET_VELOCITY(actor, 0, 0, 0);
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      Engine_TaskWait(1);
      // Send contestants through the gate at staggered intervals.
      script = gColosseumEntrance_EnterFinalsScript;
      Actor_SetActionScript(ACTOR_SATRAGE, script);
      Engine_EventWait(20);
      Actor_SetActionScript(ACTOR_AZART, script);
      Engine_EventWait(20);
      Actor_SetActionScript(ACTOR_NAVAMPA, script);
      Engine_EventWait(20);
      Actor_SetActionScript(ACTOR_GALAHAD, script);
      Engine_EventWait(20);
      Actor_SetActionScript(ACTOR_DEKKA, script);
      Engine_EventWait(20);
      Actor_SetActionScript(ACTOR_MORGAN, script);
      Engine_EventWait(20);
      Actor_SetActionScript(ACTOR_BUFORD, script);
      Engine_EventWait(60);
      Actor_SetActionScript(ACTOR_PARTY_LEADER, script);
      Engine_EventWait(80);
      Engine_EventRequestExit(COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST);
    } else {
      gEventWork->message++;
      Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FINALS_SOLDIER, 0);
    }
    Engine_EventEnd();
  }
}

/// Stepping back from the line in the Colosseum's waiting room
/// (TRIGGER_BEHIND_FINALS_LINE) arms the question again (FLAG_FINALS_LINE_ARMED
/// and FLAG_FINALS_LINE_REACHED).
static void Event_RearmFinalsStart(void) {
  Engine_GameFlagClear(FLAG_FINALS_LINE_REACHED);
  Engine_GameFlagSet(FLAG_FINALS_LINE_ARMED);
}

/// The trial attendant in the Colosseum's stands: until both trials have been
/// watched, the screen goes to the next warrior's trial (the failed one first,
/// by FLAG_COLOSSO_FAILED_TRIAL_SEEN); afterwards he talks about the
/// techniques.
static void Event_TalkToTrialAttendant(void) {
  s32 clearedTrialSeen;

  Engine_EventBegin();
  clearedTrialSeen = GameFlag_IsSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  if (clearedTrialSeen != 0) {
    Engine_ActorFaceEachOther(ACTOR_TRIAL_ATTENDANT, ACTOR_PARTY_LEADER, 40);
    Engine_EventSetMessage(MsgColosseumHavePerfectTechniquesLikeThose);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_TRIAL_ATTENDANT);
    Actor_FaceDirection(ACTOR_TRIAL_ATTENDANT, FACING_SOUTHEAST + FACING_STEP,
                        20);
  } else {
    Engine_ActorStartRepeatedMotion(ACTOR_TRIAL_ATTENDANT, 2);
    Engine_EventSetMessage(MsgColosseumSilence);
    Event_ShowMessage(ACTOR_TRIAL_ATTENDANT, 0);
    Engine_EventGetViewCenter()->motionFlags = clearedTrialSeen;
    Engine_TaskWait(1);
    Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
    Camera_MoveTo(PIXELS(540), -1, PIXELS(208), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transitionFrames = 32;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    if (GameFlag_IsSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN) != 0) {
      Engine_EventRequestExit(COLOSSEUM_STANDS_TO_CLEARED_TRIAL);
    } else {
      Engine_EventRequestExit(COLOSSEUM_STANDS_TO_FAILED_TRIAL);
    }
  }
  Engine_EventEnd();
}

/// The Colosseum's trial grounds, arriving by
/// COLOSSEUM_FAILED_TRIAL_FROM_STANDS: a warrior strains at the trial, gives up
/// and runs, and the screen goes back to the stands.
static void ColosseumEntrance_ShowFailedTrial(void) {
  struct FieldActor *actor;
  s32 facing;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Event_PlayCue(SE_247);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 1, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 2, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 3, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 4, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 5, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, 0);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 0);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 4, 0);
  actor = Object_GetById(ACTOR_TRIAL_PILLAR);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FIRST_FLAG_BEARER);
  actor->scaleX = -Q16_ONE;
  actor = Object_GetById(ACTOR_FIRST_FLAG_BEARER + 1);
  actor->scaleX = -Q16_ONE;
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FAILING_WARRIOR, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_FAILING_WARRIOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 164, 904);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 9);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 10);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, ANIM_STAND);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 164, 920);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 185, 920);
  Actor_FaceDirection(ACTOR_FAILING_WARRIOR, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 185, 916);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 11);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FAILING_WARRIOR, 1);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_FAILING_WARRIOR, 3);
  Engine_EventWait(40);
  Actor_SetActionScript(ACTOR_FAILING_WARRIOR,
                        gColosseumEntrance_PulseScaleScript);
  Engine_EventWait(80);
  Actor_SetAttachedEffect(ACTOR_FAILING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 1, FACING_SOUTH + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, FACING_WEST, 20);
  Actor_SetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 1,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 4,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 1,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      0);
  facing = FACING_SOUTHEAST + FACING_STEP;
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, facing);
  Engine_ActorStop(ACTOR_FAILING_WARRIOR);
  actor = Object_GetById(ACTOR_FAILING_WARRIOR);
  actor->facing = FACING_NORTH + FACING_STEP;
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER, FLAG_BEARER_ANIM_RED_FLAG);
  Engine_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1,
                           FLAG_BEARER_ANIM_RED_FLAG);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_FAILING_WARRIOR, facing, 20);
  Engine_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 8);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRIAL_WARRIOR + 4,
                                  ANIM_SHAKE_HEAD);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_FAILING_WARRIOR, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_FAILING_WARRIOR, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 162, 916);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 162, 890);
  Engine_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1, ANIM_STAND);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 184, 863);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 184, 796);
  Actor_SetPosition(ACTOR_FAILING_WARRIOR, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventRequestExit(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
  Engine_EventEnd();
}

/// Back in the Colosseum's stands from a trial
/// (COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS): the attendant says the warrior
/// failed, or after the second trial that he cleared it.
static void ColosseumEntrance_ExplainTrial(void) {
  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);

  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();

  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_TRIAL_ATTENDANT,
                                  FACING_SOUTH + FACING_STEP);
  Engine_EventSetMessage(MsgColosseumHaveMakeThroughCountlessMatches);

  if (Engine_GameFlagIsSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN) != 0) {
    gEventWork->message = (gEventWork->message + 1);
  }

  ColosseumEntrance_ShowMessageAndPause(ACTOR_TRIAL_ATTENDANT);
  ColosseumEntrance_FaceDirection(ACTOR_TRIAL_ATTENDANT,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_GameFlagSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN);

  Engine_EventEnd();
}

/// The Colosseum's trial grounds, arriving by
/// COLOSSEUM_CLEARED_TRIAL_FROM_STANDS: a warrior heaves the trial object up in
/// jolts and drops it, shaking the ground: he clears the trial
/// (FLAG_COLOSSO_CLEARED_TRIAL_SEEN).
static void ColosseumEntrance_ShowClearedTrial(void) {
  u32 counter;
  struct FieldActor *trialEffect;
  struct FieldActor *actor;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Event_PlayCue(SE_247);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 1, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 2, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 3, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 4, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 5, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, 0);
  Actor_SetPosition(ACTOR_FAILING_WARRIOR, 0, 0);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 4, 0);
  actor = Object_GetById(ACTOR_TRIAL_PILLAR);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FIRST_FLAG_BEARER);
  actor->scaleX = -Q16_ONE;
  actor = Object_GetById(ACTOR_FIRST_FLAG_BEARER + 1);
  actor->scaleX = -Q16_ONE;
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_CLEARING_WARRIOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 164, 904);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 9);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 10);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STAND);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 164, 920);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 185, 920);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 185, 916);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 11);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 1);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 3);
  Engine_EventWait(40);
  Actor_SetActionScript(ACTOR_CLEARING_WARRIOR,
                        gColosseumEntrance_PulseScaleScript);
  Engine_EventWait(80);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 1, FACING_SOUTH + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, FACING_WEST, 20);
  Actor_SetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 1,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 4,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 1,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // The object rises a little with every heave.
  trialEffect = Object_GetById(ACTOR_TRIAL_PILLAR);
  trialEffect->motionFlags = 0;
  for (counter = 0; counter < 20; counter++) {
    trialEffect->y.fixed += PIXELS(3) / 5;
    Engine_TaskWait(4);
    trialEffect->y.fixed += -PIXELS(3) / 10;
    Engine_TaskWait(4);
  }
  Engine_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER,
                           FLAG_BEARER_ANIM_WHITE_FLAG);
  Engine_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1,
                           FLAG_BEARER_ANIM_WHITE_FLAG);
  Engine_EventWait(60);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Engine_ActorStop(ACTOR_CLEARING_WARRIOR);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STAND);
  actor = Object_GetById(ACTOR_CLEARING_WARRIOR);
  actor->facing = FACING_NORTH + FACING_STEP;
  trialEffect->motionFlags = 3;
  trialEffect->scaleX = Q16_ONE;
  trialEffect->scaleY = Q16_ONE;
  Engine_EventWait(10);
  Event_PlayCue(SE_107);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(10);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_SetSpeed(ACTOR_CLEARING_WARRIOR, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 208, 928);
  Event_PlayCue(MUS_092);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 9);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 10);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 9);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 10);
  Engine_EventWait(80);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  Engine_EventRequestExit(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
  Engine_EventEnd();
}

/// Sets up the Colosseum or its entrance as the party arrives. The Colosseum is
/// set up by the entrance the party arrives at, and the Colosseum Entrance by
/// how far Colosso has gone.
s32 ColosseumEntrance_Initialize(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Colosseum) {
    ColosseumEntrance_ApplyArrivalState();
  } else if (scene == (s32)&SceneId_ColosseumEntrance) {
    ColosseumEntrance_UpdateEntranceCells();
  }
  return 0;
}

/// Each arrival in the Colosseum has its own scene: the trials (the stands and
/// the trial grounds), the briefing before the finals
/// (COLOSSEUM_FROM_COLOSSEUM_ENTRANCE), the passages to the finals stages
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST,
/// COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH,
/// COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST and
/// COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH), the waiting room
/// (COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST) and the congratulations after
/// the first and second wins (COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST and
/// COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH).
static void ColosseumEntrance_ApplyArrivalState(void) {
  s32 entrance;

  Engine_TaskWait(1);
  entrance = gGameState.entrance;
  switch (entrance) {
  case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
    Engine_ActorSetAnimation(ACTOR_STANDS_WEST_TORCH, ANIM_WALK);
    Engine_ActorSetAnimation(ACTOR_STANDS_EAST_TORCH, ANIM_WALK);
    break;
  case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
    Engine_ActorSetAnimation(ACTOR_STANDS_WEST_TORCH, ANIM_WALK);
    Engine_ActorSetAnimation(ACTOR_STANDS_EAST_TORCH, ANIM_WALK);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
      break;
    ColosseumEntrance_ExplainTrial();
    break;
  case COLOSSEUM_FAILED_TRIAL_FROM_STANDS:
    ColosseumEntrance_ShowFailedTrial();
    break;
  case COLOSSEUM_CLEARED_TRIAL_FROM_STANDS:
    ColosseumEntrance_ShowClearedTrial();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_ENTRANCE:
    ColosseumEntrance_ExplainPartySupport();
    InventorySnapshot_Take();
    break;
  case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
    ColosseumEntrance_EscortToEastExit();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
    ColosseumEntrance_WalkToEastExit();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    ColosseumEntrance_InitializeContestants();
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
      break;
    ColosseumEntrance_AnnounceFinals();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
    Party_AddActiveOwner(GARET);
    Party_AddActiveOwner(IVAN);
    Party_AddActiveOwner(MIA);
    Engine_GameFlagSet(FLAG_COLOSSO_FIRST_MATCH_WON);
    ColosseumEntrance_CelebrateAdvancement();
    break;
  case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST:
    ColosseumEntrance_EscortToWestExit();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH:
    ColosseumEntrance_WalkToWestExit();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH:
    Party_AddActiveOwner(GARET);
    Party_AddActiveOwner(IVAN);
    Party_AddActiveOwner(MIA);
    Engine_GameFlagSet(FLAG_COLOSSO_SECOND_MATCH_WON);
    ColosseumEntrance_CelebrateBothWins();
    break;
  default:
    break;
  }
}

static void ColosseumEntrance_ShowMessageAndPause(s32 speaker) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(10);
}

/// Turns an actor to the given facing over ten frames.
static void ColosseumEntrance_FaceDirection(s32 actor, s32 facing) {
  Engine_ActorFaceDirection(actor, (u16)facing, 10);
}

/// The Colosseum Entrance's setup: once Colosso is over, the tiles and cells by
/// the gates change; before that, the Lucky Guess man (ACTOR_LUCKY_GUESS_MAN)
/// takes child value 2, and on the finals day a cell pair before the gates
/// changes.
static void ColosseumEntrance_UpdateEntranceCells(void) {
  s32 column, row;

  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    Map_CopyCellsTo(64, 0, 48, 5, 2, 2);
    column = 16;
    row = 8;
    Map_CopyCellAttributes(14, 8, 2, 1, column, row);
  } else {
    Engine_ActorSetChildValue(ACTOR_LUCKY_GUESS_MAN, 2);
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      column = 14;
      row = 11;
      Map_CopyCellAttributes(30, 22, 1, 2, column, row);
    }
  }
}

/// Places the three companions behind Isaac for the soldiers' briefing.
static inline void ColosseumEntrance_PositionPartyForBriefing(void) {
  Actor_SetPosition(GARET, PIXELS(792), PIXELS(136));
  Actor_SetPosition(IVAN, PIXELS(824), PIXELS(136));
  Actor_SetPosition(MIA, PIXELS(808), PIXELS(152));
}

/// The Colosseum's briefing, the party's arrival from the gates
/// (COLOSSEUM_FROM_COLOSSEUM_ENTRANCE): only Isaac may enter the finals, so the
/// soldiers let his companions cheer him on from the stage, and at Ivan's
/// asking they agree to show Isaac the finals stages before the first match. An
/// soldier then leads the party on
/// (COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST).
static void ColosseumEntrance_ExplainPartySupport(void) {
  s32 flag;

  Engine_EventBegin();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_TaskWait(1);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ColorBufferApplySource(0, 0);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ColorBufferApplySource(0, 0);
  ColorBuffer_ApplyTarget(0x10002, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Engine_EventWait(20);
  ColorBuffer_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(80);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  // The soldiers explain Isaac's selection and the companions' role.
  Engine_EventSetMessage(MsgColosseumIsaacOnlyOneEnteringFinals);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER + 1, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTH + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER + 1, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER + 2, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTHWEST);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ColosseumEntrance_FaceDirection(GARET, FACING_EAST);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_WEST);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ActorSetAnimation(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  ColosseumEntrance_FaceDirection(GARET, FACING_SOUTHEAST);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTH);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  ColosseumEntrance_FaceDirection(MIA, FACING_NORTH);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTHEAST);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_ESCORTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  flag = 0;
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    ColosseumEntrance_ShowMessageAndPause(IVAN);
    flag = 1;
  } else {
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    ColosseumEntrance_FaceDirection(IVAN, FACING_NORTH);
    ColosseumEntrance_ShowMessageAndPause(IVAN);
  }
  if (flag != 0) {
    gEventWork->message++;
  }
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER + 1, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER + 2, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_ESCORTING_SOLDIER, FACING_NORTHWEST + FACING_STEP,
                      20);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER + 1, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER + 2, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_ESCORTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER + 2,
                                  FACING_SOUTH + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  ColosseumEntrance_FaceDirection(ACTOR_ESCORTING_SOLDIER, FACING_WEST);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  // Rejoin the companions before the soldier leads Isaac onward.
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(GARET, gColosseumEntrance_RejoinPartyScript);
  Actor_SetActionScript(IVAN, gColosseumEntrance_RejoinPartyScript);
  Actor_RunActionScriptAndWait(MIA, gColosseumEntrance_RejoinPartyScript);
  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Engine_EventWait(60);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 40;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/// The Colosseum's east passage
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST): a soldier leads Isaac
/// to the next finals match's tour, Colosseum Final 1 or, after the second win,
/// Colosseum Final 3.
static void ColosseumEntrance_EscortToEastExit(void) {
  struct FieldActor *escort;

  if (GameFlag_IsSet(JENNA) != 0) {
    GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
    Party_RemoveActiveOwner(JENNA);
    Party_AddActiveOwner(MIA);
  }
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_ESCORTING_SOLDIER, PIXELS(712), PIXELS(588));
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_ESCORTING_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = Object_GetById(ACTOR_ESCORTING_SOLDIER);
  escort->facing = FACING_EAST;
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 780, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 812, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 880, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 912, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 980, 588);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 1012, 588);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  if (GameFlag_IsSet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Engine_EventRequestExit(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH);
  } else {
    Engine_EventRequestExit(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH);
  }
}

/// Arriving in the Colosseum's waiting room from Colosseum Final 1's tour: the
/// soldier tells the warriors to prepare.
static void ColosseumEntrance_AnnounceFinals(void) {
  struct FieldActor *contestant;

  contestant = Object_GetById(ACTOR_AZART);
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FINALS_SOLDIER, 2);
  Engine_ActorStop(ACTOR_AZART);
  Engine_TaskWait(1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Engine_ActorSetAnimation(ACTOR_AZART, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MORGAN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_AZART, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GALAHAD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SATRAGE, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DEKKA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventSetMessage(MsgColosseumPrepareYourselvesContestantsFinalsWill);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FINALS_SOLDIER);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // Azart's wander home, as in Event_TalkToAzart.
  {
    u16 *timer = (u16 *)&contestant->timer;
    u16 duration = 720;

    *timer = duration;
  }
  {
    u16 *work = &contestant->work;
    u16 value = 112;

    *work = value;
  }
  Actor_SetBuiltinAction(ACTOR_AZART, ACTOR_WANDER);
  Actor_FaceDirection(ACTOR_MORGAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GALAHAD, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATRAGE, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DEKKA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventEnd();
}

/// The Colosseum's east passage (COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH): Isaac
/// walks alone into Colosseum Final 1 or, after the second win, Colosseum Final
/// 3, where his friends take their turns to lend a hand
/// (COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST or
/// COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH).
static void ColosseumEntrance_WalkToEastExit(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 780, 428);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 880, 428);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 980, 428);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  if (GameFlag_IsSet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Engine_EventRequestExit(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH);
  } else {
    Engine_EventRequestExit(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST);
  }
}

/// Back in the Colosseum after the first win
/// (COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST): the soldiers congratulate
/// Isaac and lead him on to Colosseum Final 2.
static void ColosseumEntrance_CelebrateAdvancement(void) {
  const s32 *actions;

  Engine_EventBegin();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  Engine_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_EventSetMessage(MsgColosseumSeeYouveMadeThroughYour);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORTING_SOLDIER, 1);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  Actor_SetActionScript(GARET, actions);
  Actor_SetActionScript(IVAN, actions);
  Actor_RunActionScriptAndWait(MIA, actions);
  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Engine_EventWait(60);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST);
}

/// The Colosseum's west passage
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST): a soldier leads Isaac
/// to Colosseum Final 2's tour.
static void ColosseumEntrance_EscortToWestExit(void) {
  struct FieldActor *escort;

  if (GameFlag_IsSet(JENNA) != 0) {
    GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
    Party_RemoveActiveOwner(JENNA);
    Party_AddActiveOwner(MIA);
  }
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_ESCORTING_SOLDIER, PIXELS(868), PIXELS(588));
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_ESCORTING_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = Object_GetById(ACTOR_ESCORTING_SOLDIER);
  escort->facing = FACING_WEST;
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 800, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 768, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 700, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 668, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 600, 588);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 568, 588);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST);
}

/// The Colosseum's west passage (COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH):
/// Isaac walks alone into Colosseum Final 2, where his friends take their turns
/// to lend a hand (COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST).
static void ColosseumEntrance_WalkToWestExit(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 800, 428);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 700, 428);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 600, 428);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST);
}

/// Back in the Colosseum after the second win
/// (COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH): one more win and Isaac is
/// the champion; the soldiers lead him back east
/// (COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST), now to Colosseum
/// Final 3.
static void ColosseumEntrance_CelebrateBothWins(void) {
  const s32 *actions;

  Engine_EventBegin();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  Engine_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_EventSetMessage(MsgColosseumWonBothMatches);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORTING_SOLDIER, 1);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  Actor_SetActionScript(GARET, actions);
  Actor_SetActionScript(IVAN, actions);
  Actor_RunActionScriptAndWait(MIA, actions);
  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Engine_EventWait(60);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/// The warriors' child values and their waiting actions in the Colosseum's
/// waiting room.
static void ColosseumEntrance_InitializeContestants(void) {
  Engine_ActorSetChildValue(ACTOR_MORGAN, 3);
  Engine_ActorSetChildValue(ACTOR_AZART, 0);
  Engine_ActorSetChildValue(ACTOR_GALAHAD, 4);
  Engine_ActorSetChildValue(ACTOR_SATRAGE, 1);
  Engine_ActorSetChildValue(ACTOR_DEKKA, 5);
  Engine_ActorSetChildValue(ACTOR_NAVAMPA, 2);
  Engine_ActorSetChildValue(ACTOR_BUFORD, 6);
  Object_SetActionById(ACTOR_AZART, 10);
  Object_SetActionById(ACTOR_GALAHAD, 20);
  Engine_ActorSetAnimation(ACTOR_SATRAGE, 0);
  Object_SetActionById(ACTOR_DEKKA, 40);
  Object_SetActionById(ACTOR_NAVAMPA, 50);
  Object_SetActionById(ACTOR_BUFORD, 60);
}

s32 gColosseumEntrance_GatherIntoIsaacScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(262), 0, PIXELS(188)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gColosseumEntrance_GatherIntoGaretScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(246), 0, PIXELS(200)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gColosseumEntrance_RejoinPartyScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(808), 0, PIXELS(120)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gColosseumEntrance_EnterGatesScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(268), 0, PIXELS(146)),
    SC_SYNC,   SC_MOVE(PIXELS(262), 0, PIXELS(24)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gColosseumEntrance_ApproachFinalsGroupScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(776), 0, PIXELS(120)),          SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP), SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_JoinFinalsFromEastScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(894), 0, PIXELS(106)),
    SC_SYNC,
    SC_MOVE(PIXELS(882), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(792), 0, PIXELS(152)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_JoinFinalsFromSouthScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(824), 0, PIXELS(152)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_EnterFinalsScript[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCEL, PIXELS(2) / 5),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(88)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(58)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gColosseumEntrance_PulseScaleScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gColosseumEntranceEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {COLOSSEUM_TUNNEL_FROM_BABI_PALACE, CONDITION_ALWAYS, 72, 0, 72,
     FACING_SOUTH, 0, 16, 16, 528, 256, 0},
    {COLOSSEUM_TUNNEL_FROM_COLOSSEUM, CONDITION_ALWAYS, 488, 0, 88,
     FACING_SOUTH, 0, 16, 16, 528, 256, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gColosseumEntrances[] = {
    {0, CONDITION_ALWAYS, 56, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH, CONDITION_ALWAYS, 184, 0, 72,
     FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {COLOSSEUM_STONE_PASSAGE_FROM_STANDS_WEST, CONDITION_ALWAYS, 56, 0, 120,
     FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {3, CONDITION_ALWAYS, 120, 0, 176, FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {COLOSSEUM_FROM_COLOSSEUM_TUNNEL, CONDITION_ALWAYS, 56, 0, 296,
     FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST, CONDITION_ALWAYS, 392, 0, 120,
     FACING_SOUTH, 0, 272, 0, 656, 304, 0},
    {COLOSSEUM_STANDS_FROM_STONE_PASSAGE_NORTH, CONDITION_ALWAYS, 520, 0, 72,
     FACING_SOUTH, 0, 272, 0, 656, 304, 0},
    {COLOSSEUM_FAILED_TRIAL_FROM_STANDS, CONDITION_ALWAYS, 184, 0, 912,
     FACING_SOUTH, 0, 0, 736, 368, 1024, 0},
    {8, CONDITION_ALWAYS, 808, 0, 296, FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST, CONDITION_ALWAYS, 784, 0, 108,
     FACING_SOUTHWEST, 0, 656, 0, 960, 320, 0},
    {COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST, CONDITION_ALWAYS, 808, 0, 120,
     FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH, CONDITION_ALWAYS, 808, 0,
     120, FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {COLOSSEUM_FROM_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, 808, 0, 120,
     FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST, CONDITION_ALWAYS, 680, 0,
     588, FACING_EAST, 0, 520, 490, 1020, 640, 0},
    {COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH, CONDITION_ALWAYS, 680, 0, 428,
     FACING_EAST, 0, 520, 330, 1020, 480, 0},
    {COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST, CONDITION_ALWAYS, 900, 0,
     588, FACING_WEST, 0, 520, 490, 1020, 640, 0},
    {COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH, CONDITION_ALWAYS, 900, 0,
     428, FACING_WEST, 0, 520, 330, 1020, 480, 0},
    {COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS, CONDITION_ALWAYS, 440, 0, 120,
     FACING_SOUTHEAST, 0, 272, 0, 656, 304, 0},
    {COLOSSEUM_CLEARED_TRIAL_FROM_STANDS, CONDITION_ALWAYS, 184, 0, 912,
     FACING_SOUTH, 0, 0, 736, 368, 1024, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gColosseumEntranceEntrances[] = {
    {COLOSSEUM_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 96, 0, 288, FACING_NORTH,
     0, 0, 0, 512, 320, 0},
    {2, CONDITION_ALWAYS, 272, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 320, 0},
    {SCENE_TABLE_END},
};

u32 gColosseumEntranceExits[] = {
    SCENE_EXITS(ColosseumTunnel),
    SCENE_EXIT(COLOSSEUM_TUNNEL_TO_BABI_PALACE, BabiPalace,
               BABI_PALACE_FROM_COLOSSEUM_TUNNEL),
    SCENE_EXIT(COLOSSEUM_TUNNEL_TO_COLOSSEUM, Colosseum,
               COLOSSEUM_FROM_COLOSSEUM_TUNNEL),
    SCENE_EXITS(Colosseum),
    SCENE_EXIT(COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH, Colosseum,
               COLOSSEUM_STANDS_FROM_STONE_PASSAGE_NORTH),
    SCENE_EXIT(COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST, Colosseum,
               COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST),
    SCENE_EXIT(COLOSSEUM_STONE_PASSAGE_LOCKED_DOOR_TO_STONE_PASSAGE, Colosseum,
               COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_TUNNEL, ColosseumTunnel,
               COLOSSEUM_TUNNEL_FROM_COLOSSEUM),
    SCENE_EXIT(COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST, Colosseum,
               COLOSSEUM_STONE_PASSAGE_FROM_STANDS_WEST),
    SCENE_EXIT(COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH, Colosseum,
               COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH),
    SCENE_EXIT(COLOSSEUM_STANDS_TO_FAILED_TRIAL, Colosseum,
               COLOSSEUM_FAILED_TRIAL_FROM_STANDS),
    SCENE_EXIT(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS, Colosseum,
               COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS),
    SCENE_EXIT(COLOSSEUM_STANDS_TO_CLEARED_TRIAL, Colosseum,
               COLOSSEUM_CLEARED_TRIAL_FROM_STANDS),
    SCENE_EXIT(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST, Colosseum,
               COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH, ColosseumAzartMatch,
               COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM),
    SCENE_EXIT(COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST, Colosseum,
               COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXIT(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST, Colosseum,
               COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST, ColosseumAzartMatch,
               COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST, ColosseumSatrageMatch,
               COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST, ColosseumSatrageMatch,
               COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH,
               ColosseumNavampaMatch,
               COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH),
    SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH,
               ColosseumNavampaMatch,
               COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH),
    SCENE_EXITS(ColosseumEntrance),
    SCENE_EXIT(COLOSSEUM_ENTRANCE_TO_TOLBI, Tolbi,
               TOLBI_FROM_COLOSSEUM_ENTRANCE),
    SCENE_EXIT(COLOSSEUM_ENTRANCE_TO_COLOSSEUM, Colosseum,
               COLOSSEUM_FROM_COLOSSEUM_ENTRANCE),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gColosseumEntrancePlacementsOther[] = {
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumStandsPlacements[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(592), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(528), 0, PIXELS(88), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(476), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(454), 0, PIXELS(104), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(208), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(444), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(428), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(240), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(192), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(608), 0, PIXELS(104), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumTrialGroundsPlacements[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(250), 0, PIXELS(846), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(298), 0, PIXELS(912), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(250), 0, PIXELS(974), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(121), 0, PIXELS(974), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(912), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(121), 0, PIXELS(846), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(96), 0, PIXELS(900), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(114), 0, PIXELS(884), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(148), 0, PIXELS(864), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(860), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(902), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumPlacements[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumSoldiersPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(782), 0, PIXELS(96), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(832), 0, PIXELS(96), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumPlacementsFromColosseumAzartMatchWest[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(234), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(778), 0, PIXELS(72), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(760), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(720), 0, PIXELS(112), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, 3, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(134), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumPlacementsFromColosseumNavampaMatch[] = {
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1016), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(266), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(152), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(198), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(48), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(80), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(56), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(140), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(64), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(72), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacementsFinalsDay[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(182), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(294), 0, PIXELS(178), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(228), 0, PIXELS(192), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(438), 0, PIXELS(186), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(186), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(258), 0, PIXELS(161), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(244), 0, PIXELS(150), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(161), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(168), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(52), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(70), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(88), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(106), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(124), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(142), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(160), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacementsAfterColosso[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(286), 0, PIXELS(164), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gColosseumEntranceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gColosseumTunnelEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 1, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_BABI_PALACE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 2, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_COLOSSEUM },
    { SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 1, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 2, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 4, CONDITION_ALWAYS, COLOSSEUM_TO_COLOSSEUM_TUNNEL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 5, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 6, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH },
    { EVENT_TALK, ACTOR_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceGoForItThatsItIm) },
    { EVENT_TALK, ACTOR_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingTheTrialsMakesMeReally) },
    { EVENT_TALK, ACTOR_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn) },
    { EVENT_TALK, ACTOR_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIveWaitedAWholeYearTo) },
    { EVENT_TALK, ACTOR_TRIAL_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTrialAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceEvenTheSmallerWarriorsCanWin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheseTrialsAreBasedOnPersonal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatFantasticPhysiquesWhyIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColossoDidntThinkBattles) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRIAL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceNoMatterHowGoodAWarrior) },
    { SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianItIsLocked)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_TOLBI },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_COLOSSEUM },
    { EVENT_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats) },
    { EVENT_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheFinalsAreFinallyHereI) },
    { EVENT_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceColossoIsOverAndTheColosseum) },
    { EVENT_TALK, ACTOR_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceSighTheColosseumIsPackedWith) },
    { EVENT_TALK, ACTOR_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceIHeardBabiHasPickedSome) },
    { EVENT_TALK, ACTOR_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheColosseumIsClosedUntilNext) },
    { EVENT_TALK, ACTOR_VILLAGER_SEATING, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DiscussSeating) },
    { EVENT_TALK, ACTOR_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIfYouHitItBigIn) },
    { EVENT_TALK, ACTOR_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAWarriorNamedIsaacWasSelected) },
    { EVENT_TALK, ACTOR_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIveBeenBettingOnTheLucky) },
    { EVENT_TALK, ACTOR_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceHavingAWarriorWhoWasntIn) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DiscussLuckyGuessSeats) },
    { EVENT_TALK, ACTOR_SCALPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DiscussLuckyGuessWinnings) },
    { EVENT_TALK, ACTOR_LUCKY_GUESS_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OfferLuckyGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIWonderWhichWarriorWillBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheRealBattlesAreAboutTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhenTheColosseumGatesFinallyShut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceHavingToArriveSoEarlyJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheWarriorNamedIsaacIsSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheFightingAtThisYearsColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SEATING, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceThereAreSoManyPeopleWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SEATING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceOfCourseItsTheRichWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImGoingToPickTheBiggest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceThereCanBeUpsetsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImFeelingConfidentThisTimeIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatKindOfWarriorIsThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_GOOD_SPOT, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryoneKeepsRiskingMoreAndMore) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceMaybeIWillBuyASeat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALPER, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryYearIGoAfterA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALPER, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceImGoingToBeInTrouble) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_GUESS_MAN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceOnceTheTrialsAreOverIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_GUESS_MAN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAllOfTheProfitsFromThe) },
    { EVENT_TOUCH, TRIGGER_BEFORE_GATES, FLAG_COLOSSO_OVER, EVENT_SCRIPT(Event_CrowdRecognizesIsaac) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gColosseumEventsFromColosseumAzartMatchWest[] = {
    {EVENT_TALK, ACTOR_MORGAN, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIAmMorganIAmThe)},
    {EVENT_TALK, ACTOR_AZART, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToAzart)},
    {EVENT_TALK, ACTOR_GALAHAD, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToGalahad)},
    {EVENT_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToSatrage)},
    {EVENT_TALK, ACTOR_DEKKA, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToDekka)},
    {EVENT_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToNavampa)},
    {EVENT_TALK, ACTOR_BUFORD, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToBuford)},
    {EVENT_TALK, ACTOR_FINALS_SOLDIER, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_AskIfReadyForFinals)},
    {EVENT_TALK, ACTOR_EXIT_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceYouCannotLeaveUntilTheFinals)},
    {EVENT_TALK, ACTOR_WAITING_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIFeelWeveBeenWaitingForever)},
    {EVENT_TALK, ACTOR_GONG_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceWhenTheGongSoundsTheFinals)},
    {EVENT_TOUCH, TRIGGER_FINALS_LINE, FLAG_FINALS_LINE_REACHED,
     EVENT_SCRIPT(Event_FinalsBegin)},
    {EVENT_TOUCH, TRIGGER_BEHIND_FINALS_LINE, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_RearmFinalsStart)},
    {SCENE_EVENTS_END},
};
