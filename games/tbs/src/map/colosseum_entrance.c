/// @file
/// Tolbi's colosseum, where Colosso is fought, in three maps: the Colosseum
/// Entrance outside its gates, the Colosseum Tunnel from Babi's Palace, and the
/// Colosseum itself, whose stands, waiting room and passages lead to the sites
/// of the three finals matches.
///
/// Colosso has two parts, and the code tells them apart by FLAG_BABI_SAVED.
/// While warriors fight their trials, Babi, the ruler of Tolbi, is missing.
/// Once the party has saved his life in Altmiller Cave, the trials are over and
/// Babi has picked Isaac for the finals himself: every map here then plays
/// the finals day. FLAG_COLOSSO_OVER, set after the third finals match, closes
/// the colosseum until next year.
///
/// - Colosseum Entrance (MapId_ColosseumEntrance), before the gates,
///   reached from Tolbi. During the trials the crowd cannot get in: its people
///   talk of the heats and of the Lucky Guess, a bet on which warrior will win
///   the finals, and Mind Read hears what they hope for. On the finals day
///   ColosseumEntrance_GetObjectEventTemplates places a second crowd, with
///   Isaac's companions and four of the colosseum's soldiers, and the map
///   events' flag conditions give everyone new lines: the scalpers sell seats,
///   and the Lucky Guess man recognizes Isaac and will not let a finalist play
///   (FLAG_LUCKY_GUESS_REFUSED_ISAAC remembers that he said so). Stepping
///   toward the gates (EventScript_CrowdRecognizesIsaac) plays the soldiers'
///   quarrel over the boy Babi picked; they lead the party in, and if Isaac
///   will not go, they and his companions talk him round, Garet leading the way
///   if he still refuses. The party arrives at the Colosseum's
///   COLOSSEUM_FROM_COLOSSEUM_ENTRANCE. Once Colosso is over,
///   ColosseumEntrance_UpdateEntranceCells changes the layout's cells before
///   the gates, and two people linger to say the colosseum is closed until next
///   year. COLOSSEUM_ENTRANCE_TO_TOLBI leads back to Tolbi.
/// - Colosseum Tunnel (MapId_ColosseumTunnel), the passage from Babi's
///   Palace (BABI_PALACE_TO_COLOSSEUM_TUNNEL) into the Colosseum, whose end
///   of the tunnel (COLOSSEUM_TUNNEL_FROM_COLOSSEUM and
///   COLOSSEUM_FROM_COLOSSEUM_TUNNEL) takes the same area name. It has only its
///   two warps.
/// - Colosseum (MapId_Colosseum), one map that holds the stands, the trial
///   grounds, the finals' waiting room and the passages to the finals stages,
///   each a warp destination's camera box on it
///   (https://alchemy.pascalpixel.com/places/colosseum/camera-boxes.png).
///   Every arrival is a warp destination of its own, and
///   ColosseumEntrance_ApplyArrivalState plays the cutscene that warp
///   destination stands for. During the trials the stands' warp destinations
///   (COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST,
///   COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS) alternate with the two trial
///   cutscenes (COLOSSEUM_FAILED_TRIAL_FROM_STANDS,
///   COLOSSEUM_CLEARED_TRIAL_FROM_STANDS), which FLAG_COLOSSO_FAILED_TRIAL_SEEN
///   and FLAG_COLOSSO_CLEARED_TRIAL_SEEN record. On the finals day
///   COLOSSEUM_FROM_COLOSSEUM_ENTRANCE keeps the party's items
///   (InventorySnapshot_Take) for Babi's Palace to give back, and the waiting
///   room's line asks whether Isaac is ready (EventScript_FinalsStart). Each
///   finals stage returns to its own warp destination here
///   (COLOSSEUM_FROM_COLOSSEUM_*_MATCH_*), and FLAG_COLOSSO_FIRST_MATCH_WON and
///   FLAG_COLOSSO_SECOND_MATCH_WON choose which stage the long passages lead to
///   next; the stages themselves are colosseum_azart_match.c,
///   colosseum_satrage_match.c and colosseum_navampa_match.c.
///
/// The three maps share one warp table, a run of warps for each map, and
/// the Colosseum's warps are how its arrivals chain into one another and into
/// the finals stages.
///
/// Open questions:
/// - XXX (no evidence): what the Colosseum's warp destinations 3 and 8 were
/// made
///   for; no warp or code enters by them, only the debug map selector can, and
///   ColosseumEntrance_GetObjectEventTemplates gives warp destination 8 the
///   soldiers' table, as it gives the arrivals of the finals day.

#include "game_state.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/colosseum_entrance.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/psynergy_effects.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "text/msg_ids.h"
#include "object_event.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

static void ColosseumEntrance_ApplyArrivalState(void);
static void ColosseumEntrance_ShowMessageAndPause(s32 speaker);
static void ColosseumEntrance_FaceDirection(s32 localId, s32 facing);
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

static inline void ColosseumEntrance_Turn(s32 localId, s32 facing) {
  /* FAKEMATCH(wrapper): EventScript_CrowdRecognizesIsaac's one turn goes
   * through this helper; as a direct ColosseumEntrance_FaceDirection call, 7 of
   * 643 instructions of EventScript_CrowdRecognizesIsaac differ in tbs-en. */
  ColosseumEntrance_FaceDirection(localId, facing);
}

/// The Colosseum Entrance's people, numbered by the rows of
/// gColosseumEntranceObjectEventTemplates;
/// gColosseumEntranceObjectEventTemplatesFinalsDay places the same people in
/// its first nine rows after Isaac's companions, who take their own object
/// event slots, and gColosseumEntranceObjectEventTemplatesAfterColosso keeps
/// the first two. They are named by what they say during the trials, except
/// where the finals day says more about who they are.
enum ColosseumEntranceLocalId {
  /// A young man with brown hair in a purple shirt who talks of the heats.
  /// "They're holding the preliminary Colosso heats right now."
  /// ★ Named from the game: "They're holding the preliminary Colosso heats
  /// right now." (MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats)
  LOCALID_VILLAGER_PRELIMINARY_HEATS = LOCALID_FIRST_PLACED,
  /// A woman with fair hair who cannot get into the packed colosseum. "Sigh...
  /// The colosseum is packed with people. I'll never get in there."
  /// ★ Named from the game: "I'll never get in there."
  /// (MsgColosseumEntranceSighTheColosseumIsPackedWith)
  LOCALID_WOMAN_CANNOT_GET_IN,
  /// A burly grey-haired man who asks whether the party hopes to get in
  /// (EventScript_DiscussSeating). "Are you hoping to get in to see Colosso
  /// now?"
  /// ★ Named from the game: "Are you hoping to get in to see Colosso now?"
  /// (MsgColosseumHopingGetInSeeColosso)
  LOCALID_VILLAGER_SEATING,
  /// A woman in a green dress who talks of winning at the Lucky Guess. "If you
  /// hit it big in the Lucky Guess, you can win a pretty penny."
  /// ★ Named from the game: "you can win a pretty penny."
  /// (MsgColosseumEntranceIfYouHitItBigIn)
  LOCALID_WOMAN_PRETTY_PENNY,
  /// An old man with a cane who has bet on the Lucky Guess for years. "I've
  /// been betting on the Lucky Guess for many years, but I've never won."
  /// ★ Named from the game: "I've been betting on the Lucky Guess for many
  /// years" (MsgColosseumEntranceIveBeenBettingOnTheLucky)
  LOCALID_OLD_MAN_BETTING_FOR_YEARS,
  /// An old woman who walks about before the finals day. "Colosso's Lucky Guess
  /// makes the battles in the colosseum better."; on the finals day, "I want to
  /// watch the finals from a good spot, but it costs too much!"
  /// ★ Named from the game: "I want to watch the finals from a good spot"
  /// (MsgColosseumWantWatchFinalsFromGood)
  LOCALID_OLD_WOMAN_GOOD_SPOT,
  /// A strongman, one of the scalpers. During the trials he bets: "I want to
  /// win big, so I put it all on a warrior no one else has picked."; on the
  /// finals day he is busy with a customer, and Mind Read hears "I'm going to
  /// be in trouble if I get stuck with any unsold seats..."
  /// ★ Named from the game: "I'm going to be in trouble if I get stuck with any
  /// unsold seats..." (MsgColosseumEntranceImGoingToBeInTrouble)
  LOCALID_SCALPER,
  /// The Lucky Guess man, in a green cap and carrying a pack. "Do you know what
  /// the Lucky Guess is?"
  /// ★ Named from the game: "Do you know what the Lucky Guess is?"
  /// (MsgColosseumDoKnowLuckyGuess)
  LOCALID_LUCKY_GUESS_MAN = LOCALID_FIRST_PLACED + 8,
  /// The four soldiers at the gates on the finals day (SPRITE_SOLDIER, as in
  /// the waiting room), gColosseumEntranceObjectEventTemplatesFinalsDay only.
  /// The
  /// first greets Isaac ("Hey, you're Isaac!") and leads the party in.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear,
  /// SPRITE_SOLDIER (its render)
  LOCALID_FIRST_GATE_SOLDIER
};

/// The Colosseum's stands, numbered by the rows of
/// gColosseumStandsObjectEventTemplates.
enum ColosseumStandsLocalId {
  /// A torch (SPRITE_TORCH).
  /// ★ Named from what the player sees: a torch on its stand (its render)
  LOCALID_STANDS_WEST_TORCH = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: the other torch on its stand (its
  /// render)
  LOCALID_STANDS_EAST_TORCH,
  /// A young man with brown hair in a purple shirt who cheers on a small
  /// warrior. "Go for it! That's it! I'm cheering for you, little guy!"
  /// ★ Named from the game: "I'm cheering for you, little guy!"
  /// (MsgColosseumEntranceGoForItThatsItIm)
  LOCALID_SPECTATOR_CHEERING = LOCALID_FIRST_PLACED + 5,
  /// A woman with fair hair, excited by the trials. "Watching the trials makes
  /// me really excited. The battles are incredible!"
  /// ★ Named from the game: "Watching the trials makes me really excited."
  /// (MsgColosseumEntranceWatchingTheTrialsMakesMeReally)
  LOCALID_SPECTATOR_EXCITED,
  /// A burly grey-haired man, taken with the warriors. "Watching such proud,
  /// skilled warriors in battle is so enchanting."
  /// ★ Named from the game: "Watching such proud, skilled warriors in battle is
  /// so enchanting." (MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn)
  LOCALID_SPECTATOR_ENCHANTED,
  /// A woman in a green dress who has waited a year for Colosso. "I've waited a
  /// whole year to see Colosso. I can't wait for the finals!"
  /// ★ Named from the game: "I've waited a whole year to see Colosso."
  /// (MsgColosseumEntranceIveWaitedAWholeYearTo)
  LOCALID_SPECTATOR_WAITED_A_YEAR,
  /// The attendant by the trials, a bald, muscular man, who shows the party two
  /// warriors' trials.
  /// ★ Named from the game: "That warrior cleared this trial"
  /// (MsgColosseumHavePerfectTechniquesLikeThose)
  LOCALID_TRIAL_ATTENDANT
};

/// The Colosseum's trial grounds, numbered by the rows of
/// gColosseumTrialGroundsObjectEventTemplates.
enum ColosseumTrialLocalId {
  /// Six torches (SPRITE_TORCH) around the trial.
  /// ★ Named from what the player sees: six lit torches on stands around the
  /// trial ground (its render)
  LOCALID_GROUNDS_FIRST_TORCH = LOCALID_FIRST_PLACED,
  /// Five warriors, 14 to 18, in the finalists' sprite
  /// (SPRITE_WARRIOR_IN_GOLDEN_HELMET); two of them take the trials the party
  /// is shown.
  /// ★ Named from the game: "That warrior failed his trial"
  /// (MsgColosseumHaveMakeThroughCountlessMatches)
  LOCALID_FIRST_TRIAL_WARRIOR = LOCALID_FIRST_PLACED + 6,
  /// The warrior the party first watches, of whom the attendant then says
  /// "That warrior failed his trial, so he's disqualified here."
  /// ★ Named from the game: "That warrior failed his trial"
  /// (MsgColosseumHaveMakeThroughCountlessMatches)
  LOCALID_FAILING_WARRIOR = LOCALID_FIRST_TRIAL_WARRIOR + 2,
  /// The warrior the party watches second, of whom the attendant then says
  /// "That warrior cleared this trial, but I doubt he'll make the finals."
  /// ★ Named from the game: "That warrior cleared this trial"
  /// (MsgColosseumHavePerfectTechniquesLikeThose)
  LOCALID_CLEARING_WARRIOR,
  /// Two young men with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, drawn
  /// mirrored) who judge the trial with flags: a red flag when the warrior
  /// fails and a white one when he clears it (enum FlagBearerAnimation). No
  /// text names them.
  /// ★ Named from what the player sees: two young men with fair hair in white
  /// tunics beside the trial, who raise a flag at its end (its render)
  LOCALID_FIRST_FLAG_BEARER = LOCALID_FIRST_PLACED + 11,
  /// A capped pillar (SPRITE_CAPPED_PILLAR) the clearing warrior heaves up and
  /// drops.
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap (its render)
  LOCALID_TRIAL_PILLAR = LOCALID_FIRST_PLACED + 13
};

/// The flag bearers' animations (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, spr_165.tsv).
enum FlagBearerAnimation {
  FLAG_BEARER_ANIMATION_RED_FLAG = 5, ///< raises a red flag: the warrior failed
  FLAG_BEARER_ANIMATION_WHITE_FLAG = 6 ///< raises a white flag: he cleared it
};

/// The four soldiers of gColosseumSoldiersObjectEventTemplates
/// (SPRITE_SOLDIER), who brief the party at COLOSSEUM_FROM_COLOSSEUM_ENTRANCE,
/// congratulate Isaac after his wins and lead him through the passages; Isaac's
/// companions take their own object event slots.
enum ColosseumSoldierLocalId {
  /// ★ Named from what the player sees: soldiers in helmets with spears
  /// (its render)
  LOCALID_FIRST_SOLDIER = LOCALID_FIRST_PLACED,
  /// The last, who leads Isaac on to each finals stage.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  LOCALID_ESCORTING_SOLDIER = LOCALID_FIRST_PLACED + 3
};

/// The Colosseum's waiting room (Colosseum,
/// COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST), numbered by the rows of
/// gColosseumObjectEventTemplatesFromColosseumAzartMatchWest: four soldiers and
/// the seven warriors who cleared the trials, as they introduce themselves.
enum ColosseumWaitingRoomLocalId {
  /// Asks whether Isaac is ready for the finals.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  LOCALID_FINALS_SOLDIER = LOCALID_FIRST_PLACED,
  /// Will not let Isaac leave. "You cannot leave until the finals are over."
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  LOCALID_EXIT_SOLDIER,
  /// Has waited long for the finals. "I feel we've been waiting forever for
  /// the finals to begin."
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  LOCALID_WAITING_SOLDIER,
  /// Tells of the gong. "When the gong sounds, the finals will begin."
  /// ★ Named from the game: "When the gong sounds, the finals will begin."
  /// (MsgColosseumEntranceWhenTheGongSoundsTheFinals)
  LOCALID_GONG_SOLDIER,
  /// The highest-ranked.
  /// ★ Named from the game: "I am Morgan."
  /// (MsgColosseumEntranceIAmMorganIAmThe)
  LOCALID_MORGAN,
  /// The second-best.
  /// ★ Named from the game: "The name's Azart."
  /// (MsgColosseumImRatedAsSecondBest)
  LOCALID_AZART,
  /// ★ Named from the game: "I'm Galahad, the third-seeded fighter."
  /// (MsgColosseumWaahDontFrightenMe)
  LOCALID_GALAHAD,
  /// ★ Named from the game: "My name is Satrage."
  /// (MsgColosseumWhoHaveComeQuestionMe)
  LOCALID_SATRAGE,
  /// ★ Named from the game: "Dekka must win the finals."
  /// (MsgColosseumDekkaMustWinFinalsDekka)
  LOCALID_DEKKA,
  /// ★ Named from the game: "I am Navampa of Gondowan"
  /// (MsgColosseumAmNavampaGondowanSixthRanked)
  LOCALID_NAVAMPA,
  /// ★ Named from the game: "I'm Buford, the seventh seed."
  /// (MsgColosseumImBufordSeventhSeed)
  LOCALID_BUFORD
};

/// The coord event triggers the map event tables answer, as the map's
/// metatile behaviors.
enum ColosseumEntranceTrigger {
  /// Before the gates at the Colosseum Entrance
  /// (EventScript_CrowdRecognizesIsaac).
  TRIGGER_BEFORE_GATES = 10,
  /// The line before the waiting room's east passage (EventScript_FinalsStart).
  TRIGGER_FINALS_LINE = 12,
  /// The cells behind it (EventScript_RearmFinalsStart).
  TRIGGER_BEHIND_FINALS_LINE = 13
};

/// The temporary flags the colosseum's maps keep.
enum {
  /// The Lucky Guess man has refused Isaac as a finalist
  /// (EventScript_OfferLuckyGuess).
  /// ★ Named from the game: "Colosso's Lucky Guess makes the battles in the
  /// colosseum better." (MsgColosseumColossosLuckyGuessMakesBattles)
  FLAG_LUCKY_GUESS_REFUSED_ISAAC = FLAG_MAPSEC_TEMP_C0,
  /// Navampa has explained that Dekka will not answer
  /// (EventScript_TalkToDekka).
  /// ★ Named from the game: "Dekka must win the finals."
  /// (MsgColosseumDekkaMustWinFinalsDekka)
  FLAG_NAVAMPA_SPOKE_FOR_DEKKA = FLAG_MAPSEC_TEMP_C1,
  /// The leader has crossed the cells behind the finals line, so stepping onto
  /// it asks whether Isaac is ready.
  /// ★ Named from the game: "If you win there, you advance to the finals."
  /// (MsgTolbiTheTrialsAreStartingAtThe)
  FLAG_FINALS_LINE_ARMED = FLAG_MAP_TEMP_34,
  /// The leader has stepped onto the line since; its coord event is off.
  /// ★ Named from the game: "If you win there, you advance to the finals."
  /// (MsgTolbiTheTrialsAreStartingAtThe)
  FLAG_FINALS_LINE_REACHED = FLAG_MAP_TEMP_35
};

const struct MapWarpDest *ColosseumEntrance_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Colosseum) {
    return gColosseumWarpDests;
  }
  if (mapId == (s32)&MapId_ColosseumEntrance) {
    return gColosseumEntranceWarpDests;
  }
  return gColosseumEntranceWarpDestsOther;
}

const struct MapWarpRegion *ColosseumEntrance_GetRegions(void) { return 0; }

/// Returns the shared colosseum warps.
const u32 *ColosseumEntrance_GetWarps(void) { return gColosseumEntranceWarps; }

/// Selects the object events: in the Colosseum by the warp destination the
/// party arrives at
/// (the stands, the trial grounds, the passages, the waiting room or the
/// soldiers' briefing), at the Colosseum Entrance by how far Colosso has gone.
const struct ObjectEventTemplate *
ColosseumEntrance_GetObjectEventTemplates(void) {
  const struct ObjectEventTemplate *table;

  if (gGameState.mapId == (s32)&MapId_Colosseum) {
    switch (gGameState.warpDest) {
    case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
    case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
      table = gColosseumStandsObjectEventTemplates;
      break;
    case COLOSSEUM_FAILED_TRIAL_FROM_STANDS:
    case COLOSSEUM_CLEARED_TRIAL_FROM_STANDS:
      table = gColosseumTrialGroundsObjectEventTemplates;
      break;
    // Warp destination 8 has a table row but no warp leads to it (see the
    // open questions).
    case 8:
    case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
    case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH:
    case COLOSSEUM_FROM_COLOSSEUM_ENTRANCE:
    case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
    case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST:
      table = gColosseumSoldiersObjectEventTemplates;
      break;
    case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
      table = gColosseumObjectEventTemplatesFromColosseumAzartMatchWest;
      break;
    case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
    case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH:
      table = gColosseumObjectEventTemplatesFromColosseumNavampaMatch;
      break;
    default:
      table = gColosseumObjectEventTemplates;
      break;
    }
  } else if (gGameState.mapId == (s32)&MapId_ColosseumEntrance) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      table = gColosseumEntranceObjectEventTemplatesAfterColosso;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      table = gColosseumEntranceObjectEventTemplatesFinalsDay;
    } else {
      table = gColosseumEntranceObjectEventTemplates;
    }
  } else {
    table = gColosseumEntranceObjectEventTemplatesOther;
  }
  return table;
}

const struct MapEvent *ColosseumEntrance_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_ColosseumTunnel) {
    return gColosseumTunnelEvents;
  }
  if (mapId == (s32)&MapId_Colosseum) {
    if (gGameState.warpDest == COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST) {
      return gColosseumEventsFromColosseumAzartMatchWest;
    }
    return gColosseumEvents;
  }
  if (mapId == (s32)&MapId_ColosseumEntrance) {
    return gColosseumEntranceEvents;
  }
  return gColosseumEntranceEventsOther;
}

/// At the Colosseum Entrance: during the trials, a burly grey-haired man asks
/// whether the party hopes to get in to see Colosso; on the finals day
/// (FLAG_BABI_SAVED) the colosseum is already full, and only the scalpers have
/// seats.
static void EventScript_DiscussSeating(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
    Engine_ScriptSetMessage(MsgColosseumColosseumAlreadyFullIfReally);
    Engine_ScriptShowMessage(LOCALID_VILLAGER_SEATING, 0);
  } else {
    Engine_ScriptSetMessage(MsgColosseumHopingGetInSeeColosso);
    Engine_ScriptAskYesNo(LOCALID_VILLAGER_SEATING, 0);
  }
  Engine_ScriptEnd();
}

/// At the Colosseum Entrance, an old woman: the Lucky Guess makes the battles
/// better; on the finals day, a good spot costs too much.
static void EventScript_DiscussLuckyGuessSeats(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
    Engine_ObjectEventShowEmote(LOCALID_OLD_WOMAN_GOOD_SPOT,
                                EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Engine_ScriptSetMessage(MsgColosseumWantWatchFinalsFromGood);
    Engine_ScriptShowMessage(LOCALID_OLD_WOMAN_GOOD_SPOT, 0);
  } else {
    Engine_ScriptSetMessage(MsgColosseumColossosLuckyGuessMakesBattles);
    Engine_ScriptShowMessage(LOCALID_OLD_WOMAN_GOOD_SPOT, 0);
  }
  Engine_ScriptEnd();
}

/// At the Colosseum Entrance: a strongman, betting, put it all on a warrior no
/// one picked; on the finals day a scalper, busy with a customer, asks if the
/// party wants seats.
static void EventScript_DiscussLuckyGuessWinnings(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_ObjectEventRunRepeatedMotion(LOCALID_SCALPER, 2);
    Engine_ScriptSetMessage(MsgColosseumHoldOnSecond);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_SCALPER);
    Engine_ObjectEventFaceEachOther(LOCALID_SCALPER, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Engine_ScriptAskYesNo(LOCALID_SCALPER, 0);
    ColosseumEntrance_FaceDirection(LOCALID_SCALPER, FACING_EAST);
  } else {
    Engine_ScriptSetMessage(MsgColosseumWantWinBigPutOn);
    Engine_ScriptShowMessage(LOCALID_SCALPER, 0);
  }
  Engine_ScriptEnd();
}

/// The Lucky Guess man at the Colosseum Entrance explains the game; on the
/// finals day he recognizes Isaac as Babi's pick and refuses him, as finalists
/// may not play (FLAG_LUCKY_GUESS_REFUSED_ISAAC: said once).
static void EventScript_OfferLuckyGuess(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_BABI_SAVED) != 0) {
    if (GameFlag_IsSet(FLAG_LUCKY_GUESS_REFUSED_ISAAC) != 0) {
      Engine_ScriptSetMessage(MsgColosseumLookImSorryButWe);
      Script_ShowMessage(LOCALID_LUCKY_GUESS_MAN, 0);
    } else {
      Engine_ScriptSetMessage(MsgColosseumWantTryOutLuckyGuess);
      Script_OpenMessage(LOCALID_LUCKY_GUESS_MAN, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 0) {
        gFieldWork->message++;
        ObjectEvent_ShowEmote(LOCALID_LUCKY_GUESS_MAN,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        Script_OpenMessage(LOCALID_LUCKY_GUESS_MAN, 0);
        if (Engine_ScriptChooseYesNo(0, 0) == 0) {
          gFieldWork->message++;
        }
        Engine_ScriptWait(40);
        Script_ShowMessage(LOCALID_LUCKY_GUESS_MAN, 0);
        GameFlag_Set(FLAG_LUCKY_GUESS_REFUSED_ISAAC);
      } else {
        Script_ShowMessage(LOCALID_LUCKY_GUESS_MAN, 0);
      }
    }
  } else {
    Engine_ScriptSetMessage(MsgColosseumDoKnowLuckyGuess);
    Script_AskYesNo(LOCALID_LUCKY_GUESS_MAN, 0);
  }
  Engine_ScriptEnd();
}

/// Azart won't lose to someone who skipped the trials. He waits in the
/// Colosseum's waiting room as the second-best, and goes back to pacing after
/// he has said so.
static void EventScript_TalkToAzart(void) {
  struct ObjectEvent *contestant;

  contestant = ObjectEvent_GetById(LOCALID_AZART);
  Engine_ScriptStart();
  Engine_ObjectEventStop(LOCALID_AZART);
  Engine_ObjectEventFaceEachOther(LOCALID_AZART, LOCALID_PARTY_LEADER, 20);
  Engine_ScriptSetMessage(MsgColosseumImRatedAsSecondBest);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_AZART);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_AZART, 1);
  Engine_ScriptShowMessage(LOCALID_AZART, 0);
  // Give his wander back its home, where he is placed: MOVEMENT_TYPE_WANDER
  // reads timer and work as homeX and homeZ (ScriptObjectRuntime), in pixels.
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
  ObjectEvent_SetBuiltinAction(LOCALID_AZART, MOVEMENT_TYPE_WANDER);
  Engine_ScriptEnd();
}

/// In the Colosseum's waiting room, Galahad, the third seed, jumps.
/// "Waah! Don't frighten me!"
static void EventScript_TalkToGalahad(void) {
  Engine_ScriptStart();
  ObjectEvent_SetAttachedEffect(LOCALID_GALAHAD, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GALAHAD, 2);
  Engine_ScriptSetMessage(MsgColosseumWaahDontFrightenMe);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_GALAHAD);
  ObjectEvent_ShowEmote(LOCALID_GALAHAD, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_GALAHAD, 0);
  Engine_ScriptEnd();
}

/// In the Colosseum's waiting room, Satrage, the fourth-ranked, opens his eyes
/// and names himself.
static void EventScript_TalkToSatrage(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgColosseumWhoHaveComeQuestionMe);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_SATRAGE);
  Engine_ObjectEventFaceEachOther(LOCALID_SATRAGE, LOCALID_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_SATRAGE);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_SATRAGE, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_SATRAGE, 0);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_SATRAGE);
  ColosseumEntrance_FaceDirection(LOCALID_SATRAGE, FACING_SOUTH + FACING_STEP);
  Engine_ScriptEnd();
}

/// In the Colosseum's waiting room, Dekka psyches himself up; the first time,
/// Navampa explains he will not answer (FLAG_NAVAMPA_SPOKE_FOR_DEKKA).
static void EventScript_TalkToDekka(void) {
  Engine_ScriptStart();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DEKKA, 2);
  Engine_ScriptSetMessage(MsgColosseumDekkaMustWinFinalsDekka);
  Script_ShowMessageAndWait(LOCALID_DEKKA, 0, 20);
  if (GameFlag_IsSet(FLAG_NAVAMPA_SPOKE_FOR_DEKKA) != 0) {
    Engine_ScriptWait(20);
  } else {
    ColosseumEntrance_FaceDirection(LOCALID_NAVAMPA, FACING_EAST);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_NAVAMPA, 1);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_NAVAMPA);
    Engine_ObjectEventFaceEachOther(LOCALID_NAVAMPA, LOCALID_PARTY_LEADER, 20);
    Engine_ObjectEventSetAnimation(LOCALID_NAVAMPA, ANIMATION_SHAKE_HEAD);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_NAVAMPA);
    ObjectEvent_ShowEmote(LOCALID_NAVAMPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_NAVAMPA);
    ColosseumEntrance_FaceDirection(LOCALID_NAVAMPA,
                                    FACING_SOUTH + FACING_STEP);
    GameFlag_Set(FLAG_NAVAMPA_SPOKE_FOR_DEKKA);
  }
  Engine_ScriptEnd();
}

/// In the Colosseum's waiting room, Navampa of Gondowan, the sixth-ranked,
/// introduces himself.
static void EventScript_TalkToNavampa(void) {
  Engine_ScriptStart();
  Engine_ObjectEventFaceEachOther(LOCALID_NAVAMPA, LOCALID_PARTY_LEADER, 20);
  Engine_ScriptSetMessage(MsgColosseumAmNavampaGondowanSixthRanked);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_NAVAMPA);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_NAVAMPA, ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_NAVAMPA);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_NAVAMPA, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_NAVAMPA);
  ColosseumEntrance_FaceDirection(LOCALID_NAVAMPA, FACING_SOUTH + FACING_STEP);
  Engine_ScriptEnd();
}

/// In the Colosseum's waiting room, Buford, the seventh seed: anyone could win
/// today.
static void EventScript_TalkToBuford(void) {
  Engine_ScriptStart();
  Engine_ObjectEventFaceEachOther(LOCALID_BUFORD, LOCALID_PARTY_LEADER, 20);
  Engine_ScriptSetMessage(MsgColosseumImBufordSeventhSeed);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_BUFORD);
  ObjectEvent_FaceDirection(LOCALID_BUFORD, FACING_NORTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_BUFORD, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_BUFORD, FACING_WEST, 40);
  Engine_ObjectEventFaceEachOther(LOCALID_BUFORD, LOCALID_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_BUFORD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_BUFORD, ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_BUFORD);
  ColosseumEntrance_FaceDirection(LOCALID_BUFORD, FACING_SOUTH + FACING_STEP);
  Engine_ScriptEnd();
}

/// In the Colosseum's waiting room, the soldier asks whether Isaac is ready
/// for the finals.
static void EventScript_AskIfReadyForFinals(void) {
  Engine_ScriptStart();
  Engine_ObjectEventFaceEachOther(LOCALID_FINALS_SOLDIER, LOCALID_PARTY_LEADER,
                                  20);
  Engine_ScriptSetMessage(MsgColosseumYouReadyForFinals);
  Engine_ScriptOpenMessage(LOCALID_FINALS_SOLDIER, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0)
    gFieldWork->message++;
  Engine_ScriptShowMessage(LOCALID_FINALS_SOLDIER, 0);
  Engine_ScriptEnd();
}

/// Stepping toward the gates at the Colosseum Entrance (TRIGGER_BEFORE_GATES)
/// on the finals day: four soldiers recognize Isaac as Babi's pick and quarrel
/// over a boy in the finals, then one takes the party in to the Colosseum's
/// COLOSSEUM_FROM_COLOSSEUM_ENTRANCE. If Isaac will not go, the soldiers
/// insist and Ivan, Mia and Garet talk him round; if he still refuses, Garet
/// leads the way in. Before Babi is saved it does nothing.
static void EventScript_CrowdRecognizesIsaac(void) {
  // The native sequence reuses this word for object event handles and the
  // reply.
  s32 objectEventOrAnswer;

  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) == 0) {
    return;
  }

  Engine_ScriptStart();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER, 1);
  ObjectEvent_FaceDirection(LOCALID_FIRST_GATE_SOLDIER,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER, FACING_EAST, 60);
  ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_GATE_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_ScriptSetMessage(MsgColosseumHeyYouRe);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER, 2);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 2,
                                  FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 3,
                                  FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptGetViewCenter()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(172), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER + 1,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER + 1);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER,
                                        ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_GATE_SOLDIER + 2,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_GATE_SOLDIER + 2, FACING_EAST);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER + 2);
  ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER + 3,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_GATE_SOLDIER + 3, FACING_EAST);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER + 3, 2);
  Engine_ScriptShowMessageAndWait(LOCALID_FIRST_GATE_SOLDIER + 3, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER, 1);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FIRST_GATE_SOLDIER,
                            FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ScriptShowMessageAndWait(LOCALID_FIRST_GATE_SOLDIER, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER + 1, 1);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER + 1,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  // Gather Isaac and his companions for the invitation.
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_GATE_SOLDIER, FACING_WEST);
  Engine_ScriptShowMessageAndWait(LOCALID_FIRST_GATE_SOLDIER, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER + 2, 1);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER + 2,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptShowMessageAndWait(LOCALID_FIRST_GATE_SOLDIER, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER + 3, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER + 3,
                                        ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_GATE_SOLDIER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_GATE_SOLDIER + 1, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_GATE_SOLDIER + 2, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER + 3,
                                        ANIMATION_NOD);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 2,
                                  FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_GATE_SOLDIER + 3,
                                  FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_SetSpeed(LOCALID_FIRST_GATE_SOLDIER, PIXELS(3) / 5,
                       PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_FIRST_GATE_SOLDIER, 258, 172);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 262, 188);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  objectEventOrAnswer = (s32)ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEventOrAnswer != 0) {
    Engine_ObjectEventSetPosition(
        GARET, ((struct ObjectEvent *)objectEventOrAnswer)->x.fixed,
        ((struct ObjectEvent *)objectEventOrAnswer)->z.fixed);
  }
  objectEventOrAnswer = (s32)ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEventOrAnswer != 0) {
    Engine_ObjectEventSetPosition(
        IVAN, ((struct ObjectEvent *)objectEventOrAnswer)->x.fixed,
        ((struct ObjectEvent *)objectEventOrAnswer)->z.fixed);
  }
  objectEventOrAnswer = (s32)ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEventOrAnswer != 0) {
    Engine_ObjectEventSetPosition(
        MIA, ((struct ObjectEvent *)objectEventOrAnswer)->x.fixed,
        ((struct ObjectEvent *)objectEventOrAnswer)->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ObjectEventWalkTo(GARET, 246, 200);
  ObjectEvent_WalkTo(IVAN, 262, 200);
  ObjectEvent_WalkToAndWait(MIA, 278, 200);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_GATE_SOLDIER, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER,
                                        ANIMATION_NOD);
  Script_OpenMessage(LOCALID_FIRST_GATE_SOLDIER, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);

  // Either leave with the guide or let the companions persuade Isaac.
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER,
                                          ANIMATION_NOD);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
    ObjectEvent_SetActionScript(GARET,
                                gColosseumEntrance_GatherIntoIsaacScript);
    ObjectEvent_SetActionScript(IVAN, gColosseumEntrance_GatherIntoIsaacScript);
    ObjectEvent_RunActionScriptAndWait(
        MIA, gColosseumEntrance_GatherIntoIsaacScript);
    Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
    ObjectEvent_SetSpeed(LOCALID_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetActionScript(LOCALID_FIRST_GATE_SOLDIER,
                                gColosseumEntrance_EnterGatesScript);
    Engine_ScriptWait(10);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                                gColosseumEntrance_EnterGatesScript);
    Engine_ScriptWait(80);
    gFieldWork->transitionFrames = 40;
    Engine_ScriptCloseScreen();
    Engine_ScriptWaitForScreen();
  } else {
    Script_SkipMessages(1);
    ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
    ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER + 1,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER + 1);
    ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER + 2,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER + 2);
    ObjectEvent_ShowEmote(LOCALID_FIRST_GATE_SOLDIER + 3,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER + 2);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER,
                                          ANIMATION_SHAKE_HEAD);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
    Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
    Engine_ScriptWait(20);
    ColosseumEntrance_ShowMessageAndPause(IVAN);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(MIA);
    ObjectEvent_FaceDirection(LOCALID_FIRST_GATE_SOLDIER,
                              FACING_NORTHWEST + FACING_STEP, 0);
    Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 2, FACING_WEST,
                                    0);
    Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 3, FACING_EAST,
                                    60);
    Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER,
                                    FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 2,
                                    FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ObjectEventFaceDirection(LOCALID_FIRST_GATE_SOLDIER + 3,
                                    FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_GATE_SOLDIER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_GATE_SOLDIER + 1,
                                   ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_GATE_SOLDIER + 2,
                                   ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER + 3,
                                          ANIMATION_NOD);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    ColosseumEntrance_Turn(MIA, FACING_NORTHWEST);
    ColosseumEntrance_ShowMessageAndPause(MIA);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    Script_OpenMessage(GARET, 0);

    objectEventOrAnswer = Engine_ScriptChooseYesNo(0, 0);
    if (objectEventOrAnswer == 0) {
      Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_GATE_SOLDIER,
                                            ANIMATION_NOD);
      ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_GATE_SOLDIER);
      ObjectEvent_SetActionScript(GARET,
                                  gColosseumEntrance_GatherIntoIsaacScript);
      ObjectEvent_SetActionScript(IVAN,
                                  gColosseumEntrance_GatherIntoIsaacScript);
      ObjectEvent_RunActionScriptAndWait(
          MIA, gColosseumEntrance_GatherIntoIsaacScript);
      Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
      ObjectEvent_SetSpeed(LOCALID_FIRST_GATE_SOLDIER, PIXELS(1),
                           PIXELS(1) / 2);
      ObjectEvent_SetActionScript(LOCALID_FIRST_GATE_SOLDIER,
                                  gColosseumEntrance_EnterGatesScript);
      Engine_ScriptWait(10);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                                  gColosseumEntrance_EnterGatesScript);
      Engine_ScriptWait(80);
      gFieldWork->transitionFrames = 40;
      Engine_ScriptCloseScreen();
      Engine_ScriptWaitForScreen();
    } else {
      Script_SkipMessages(1);
      Engine_ScriptWait(20);
      Engine_ObjectEventStartRepeatedMotion(GARET, 2);
      Engine_ScriptShowMessageAndWait(GARET, 0, 20);
      Engine_ObjectEventShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      ColosseumEntrance_ShowMessageAndPause(IVAN);
      ColosseumEntrance_FaceDirection(MIA, FACING_WEST);
      Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
      ColosseumEntrance_ShowMessageAndPause(MIA);
      ObjectEvent_SetActionScript(IVAN,
                                  gColosseumEntrance_GatherIntoGaretScript);
      ObjectEvent_RunActionScriptAndWait(
          MIA, gColosseumEntrance_GatherIntoGaretScript);
      Engine_ScriptWait(20);
      ObjectEvent_RunActionScriptAndWait(
          LOCALID_PARTY_LEADER, gColosseumEntrance_GatherIntoGaretScript);
      ObjectEvent_WalkToAndWait(GARET, 262, 188);
      ColosseumEntrance_FaceDirection(GARET, FACING_NORTH);
      Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
      ColosseumEntrance_ShowMessageAndPause(GARET);
      Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
      ObjectEvent_SetSpeed(LOCALID_FIRST_GATE_SOLDIER, PIXELS(1),
                           PIXELS(1) / 2);
      ObjectEvent_SetActionScript(LOCALID_FIRST_GATE_SOLDIER,
                                  gColosseumEntrance_EnterGatesScript);
      Engine_ScriptWait(10);
      ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_SetActionScript(GARET, gColosseumEntrance_EnterGatesScript);
      Engine_ScriptWait(80);
      gFieldWork->transitionFrames = 40;
      Engine_ScriptCloseScreen();
      Engine_ScriptWaitForScreen();
    }
  }

  Engine_ScriptRequestWarp(COLOSSEUM_ENTRANCE_TO_COLOSSEUM);
  Engine_GameFlagSet(FLAG_COLOSSEUM_FINALS_ENTERED);
  Engine_ScriptEnd();
}

/// Stepping to the line in the Colosseum's waiting room (TRIGGER_FINALS_LINE,
/// while FLAG_FINALS_LINE_ARMED is set and FLAG_FINALS_LINE_REACHED clear):
/// asked whether he is ready, Isaac can still wait; if he is, the warriors
/// grumble, line up by rank and march out, and Isaac follows them out by the
/// east passage (COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST).
static void EventScript_FinalsStart(void) {
  struct ObjectEvent *objectEvent;
  const s32 *script;

  if (Engine_GameFlagIsSet(FLAG_FINALS_LINE_ARMED) != 0) {
    Engine_GameFlagSet(FLAG_FINALS_LINE_REACHED);
    Engine_ScriptStart();
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 872, 120);
    ColosseumEntrance_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST);
    FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    FieldCamera_MoveTo(PIXELS(816), -1, PIXELS(112), 1);
    Engine_CameraWaitForMove();
    ObjectEvent_FaceDirection(LOCALID_FINALS_SOLDIER,
                              FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_FINALS_SOLDIER, 1);
    Engine_ScriptSetMessage(MsgColosseumYouReadyForFinals);
    Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_FINALS_SOLDIER, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Engine_ScriptWait(20);
      ColosseumEntrance_FaceDirection(LOCALID_MORGAN,
                                      FACING_NORTH + FACING_STEP);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_MORGAN, 1);
      Engine_ScriptSetMessage(MsgColosseumDayTiredWaiting);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            LOCALID_MORGAN);
      ColosseumEntrance_FaceDirection(LOCALID_NAVAMPA, FACING_EAST);
      ColosseumEntrance_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_NAVAMPA, ANIMATION_NOD);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER,
                                            ANIMATION_NOD);
      Engine_ObjectEventSetAnimation(LOCALID_SATRAGE, ANIMATION_STAND);
      Engine_ObjectEventFaceDirection(LOCALID_SATRAGE,
                                      FACING_NORTH + FACING_STEP, 20);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_SATRAGE, 1);
      ColosseumEntrance_ShowMessageAndPause(LOCALID_SATRAGE);
      ColosseumEntrance_FaceDirection(LOCALID_DEKKA, FACING_WEST);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_DEKKA, ANIMATION_NOD);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_NAVAMPA, 2);
      ColosseumEntrance_FaceDirection(LOCALID_NAVAMPA, FACING_NORTHWEST);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            LOCALID_NAVAMPA);
      ColosseumEntrance_FaceDirection(LOCALID_BUFORD,
                                      FACING_NORTHWEST + FACING_STEP);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_BUFORD, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            LOCALID_BUFORD);
      ColosseumEntrance_FaceDirection(LOCALID_GONG_SOLDIER,
                                      FACING_SOUTHEAST + FACING_STEP);
      Engine_ObjectEventSetAttachedEffect(LOCALID_GONG_SOLDIER,
                                          ATTACHED_EFFECT_IN_FRONT |
                                              ATTACHED_EFFECT_ONCE);
      Engine_ScriptWait(60);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_GONG_SOLDIER, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_BELOW |
                                            LOCALID_GONG_SOLDIER);
      Engine_ObjectEventStop(LOCALID_AZART);
      Engine_TaskWait(1);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_AZART, 2);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_GALAHAD, 2);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_DEKKA, 2);
      Engine_ScriptWait(20);
      Engine_ObjectEventFaceDirection(LOCALID_AZART, FACING_EAST, 0);
      Engine_ObjectEventFaceDirection(LOCALID_GALAHAD, FACING_WEST, 0);
      Engine_ObjectEventFaceDirection(LOCALID_DEKKA,
                                      FACING_NORTHWEST + FACING_STEP, 40);
      Engine_ObjectEventSetAnimation(LOCALID_AZART, ANIMATION_NOD);
      Engine_ObjectEventSetAnimation(LOCALID_GALAHAD, ANIMATION_NOD);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_DEKKA, ANIMATION_NOD);
      ObjectEvent_SetSpeed(LOCALID_AZART, PIXELS(8) / 5, PIXELS(4) / 5);
      ObjectEvent_SetSpeed(LOCALID_GALAHAD, PIXELS(8) / 5, PIXELS(4) / 5);
      ObjectEvent_SetSpeed(LOCALID_DEKKA, PIXELS(8) / 5, PIXELS(4) / 5);
      ObjectEvent_SetActionScript(LOCALID_AZART,
                                  gColosseumEntrance_ApproachFinalsGroupScript);
      ObjectEvent_SetActionScript(LOCALID_DEKKA,
                                  gColosseumEntrance_JoinFinalsFromSouthScript);
      Engine_ScriptWait(20);
      Engine_ObjectEventFaceDirection(LOCALID_SATRAGE,
                                      FACING_NORTH + FACING_STEP, 0);
      Engine_ObjectEventFaceDirection(LOCALID_NAVAMPA,
                                      FACING_NORTHWEST + FACING_STEP, 0);
      Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST,
                                      0);
      Engine_ObjectEventFaceDirection(LOCALID_MORGAN,
                                      FACING_NORTH + FACING_STEP, 0);
      Engine_ObjectEventFaceDirection(LOCALID_BUFORD,
                                      FACING_NORTHWEST + FACING_STEP, 0);
      ObjectEvent_RunActionScriptAndWait(
          LOCALID_GALAHAD, gColosseumEntrance_JoinFinalsFromEastScript);
      Engine_ScriptWait(20);
      Engine_ObjectEventFaceDirection(LOCALID_FINALS_SOLDIER, FACING_EAST, 0);
      Engine_ObjectEventFaceDirection(LOCALID_GONG_SOLDIER, FACING_WEST, 40);
      Engine_ObjectEventSetAnimation(LOCALID_FINALS_SOLDIER, ANIMATION_NOD);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_GONG_SOLDIER,
                                            ANIMATION_NOD);
      Engine_CameraMoveTo(PIXELS(808), -1, PIXELS(86), 1);
      Engine_CameraWaitForMove();
      Engine_ObjectEventRunRepeatedMotion(LOCALID_FINALS_SOLDIER, 2);
      ColosseumEntrance_ShowMessageAndPause(LOCALID_FINALS_SOLDIER);
      Engine_ObjectEventSetSpeed(LOCALID_FINALS_SOLDIER, PIXELS(1),
                                 PIXELS(1) / 2);
      ObjectEvent_GetById(LOCALID_FINALS_SOLDIER)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      Engine_ObjectEventWalkToAndWait(LOCALID_FINALS_SOLDIER, 792, 72);
      Engine_ScriptWait(1);
      ObjectEvent_GetById(LOCALID_FINALS_SOLDIER)->steeringFlags |=
          OBJ_EVENT_FACES_MOTION;
      ColosseumEntrance_FaceDirection(LOCALID_FINALS_SOLDIER, FACING_EAST);
      // Azart stops pacing before the procession.
      Engine_ObjectEventStop(LOCALID_AZART);
      objectEvent = ObjectEvent_GetById(LOCALID_AZART);
      objectEvent->update = NULL;
      objectEvent->timer = 0;
      objectEvent->work = 0;
      SET_VELOCITY(objectEvent, 0, 0, 0);
      objectEvent->targetX = OBJ_EVENT_NO_TARGET;
      objectEvent->targetY = OBJ_EVENT_NO_TARGET;
      objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
      Engine_TaskWait(1);
      // Send contestants through the gate at staggered intervals.
      script = gColosseumEntrance_EnterFinalsScript;
      ObjectEvent_SetActionScript(LOCALID_SATRAGE, script);
      Engine_ScriptWait(20);
      ObjectEvent_SetActionScript(LOCALID_AZART, script);
      Engine_ScriptWait(20);
      ObjectEvent_SetActionScript(LOCALID_NAVAMPA, script);
      Engine_ScriptWait(20);
      ObjectEvent_SetActionScript(LOCALID_GALAHAD, script);
      Engine_ScriptWait(20);
      ObjectEvent_SetActionScript(LOCALID_DEKKA, script);
      Engine_ScriptWait(20);
      ObjectEvent_SetActionScript(LOCALID_MORGAN, script);
      Engine_ScriptWait(20);
      ObjectEvent_SetActionScript(LOCALID_BUFORD, script);
      Engine_ScriptWait(60);
      ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, script);
      Engine_ScriptWait(80);
      Engine_ScriptRequestWarp(COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST);
    } else {
      gFieldWork->message++;
      Engine_ScriptShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_FINALS_SOLDIER,
                               0);
    }
    Engine_ScriptEnd();
  }
}

/// Stepping back from the line in the Colosseum's waiting room
/// (TRIGGER_BEHIND_FINALS_LINE) arms the question again (FLAG_FINALS_LINE_ARMED
/// and FLAG_FINALS_LINE_REACHED).
static void EventScript_RearmFinalsStart(void) {
  Engine_GameFlagClear(FLAG_FINALS_LINE_REACHED);
  Engine_GameFlagSet(FLAG_FINALS_LINE_ARMED);
}

/// The trial attendant in the Colosseum's stands: until both trials have been
/// watched, the screen goes to the next warrior's trial (the failed one first,
/// by FLAG_COLOSSO_FAILED_TRIAL_SEEN); afterwards he talks about the
/// techniques.
static void EventScript_TalkToTrialAttendant(void) {
  s32 clearedTrialSeen;

  Engine_ScriptStart();
  clearedTrialSeen = GameFlag_IsSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  if (clearedTrialSeen != 0) {
    Engine_ObjectEventFaceEachOther(LOCALID_TRIAL_ATTENDANT,
                                    LOCALID_PARTY_LEADER, 40);
    Engine_ScriptSetMessage(MsgColosseumHavePerfectTechniquesLikeThose);
    ColosseumEntrance_ShowMessageAndPause(LOCALID_TRIAL_ATTENDANT);
    ObjectEvent_FaceDirection(LOCALID_TRIAL_ATTENDANT,
                              FACING_SOUTHEAST + FACING_STEP, 20);
  } else {
    Engine_ObjectEventStartRepeatedMotion(LOCALID_TRIAL_ATTENDANT, 2);
    Engine_ScriptSetMessage(MsgColosseumSilence);
    Script_ShowMessage(LOCALID_TRIAL_ATTENDANT, 0);
    Engine_ScriptGetViewCenter()->motionFlags = clearedTrialSeen;
    Engine_TaskWait(1);
    FieldCamera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
    FieldCamera_MoveTo(PIXELS(540), -1, PIXELS(208), 1);
    Engine_CameraWaitForMove();
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 0);
    gFieldWork->transitionFrames = 32;
    Engine_ScriptCloseScreen();
    Engine_ScriptWaitForScreen();
    if (GameFlag_IsSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN) != 0) {
      Engine_ScriptRequestWarp(COLOSSEUM_STANDS_TO_CLEARED_TRIAL);
    } else {
      Engine_ScriptRequestWarp(COLOSSEUM_STANDS_TO_FAILED_TRIAL);
    }
  }
  Engine_ScriptEnd();
}

/// The Colosseum's trial grounds, arriving by
/// COLOSSEUM_FAILED_TRIAL_FROM_STANDS: a warrior strains at the trial, gives up
/// and runs, and the screen goes back to the stands.
static void ColosseumEntrance_ShowFailedTrial(void) {
  struct ObjectEvent *objectEvent;
  s32 facing;

  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Script_PlaySong(SE_247);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH, ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 1,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 2,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 3,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 4,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 5,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR + 1, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, 0);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR + 4, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_TRIAL_PILLAR);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_FLAG_BEARER);
  objectEvent->scaleX = -Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_FLAG_BEARER + 1);
  objectEvent->scaleX = -Q16_ONE;
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 32;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FAILING_WARRIOR, 1);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_FAILING_WARRIOR, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 164, 904);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, 9);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, 10);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, ANIMATION_STAND);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 164, 920);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 185, 920);
  ObjectEvent_FaceDirection(LOCALID_FAILING_WARRIOR, FACING_NORTH, 20);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 185, 916);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, 11);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FAILING_WARRIOR, 1);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FAILING_WARRIOR, 3);
  Engine_ScriptWait(40);
  ObjectEvent_SetActionScript(LOCALID_FAILING_WARRIOR,
                              gColosseumEntrance_PulseScaleScript);
  Engine_ScriptWait(80);
  ObjectEvent_SetAttachedEffect(
      LOCALID_FAILING_WARRIOR, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                            FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_CLEARING_WARRIOR, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 4, FACING_WEST, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TRIAL_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_CLEARING_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TRIAL_WARRIOR + 4,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_CLEARING_WARRIOR,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  facing = FACING_SOUTHEAST + FACING_STEP;
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 4, facing);
  Engine_ObjectEventStop(LOCALID_FAILING_WARRIOR);
  objectEvent = ObjectEvent_GetById(LOCALID_FAILING_WARRIOR);
  objectEvent->facing = FACING_NORTH + FACING_STEP;
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_FLAG_BEARER,
                                 FLAG_BEARER_ANIMATION_RED_FLAG);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_FLAG_BEARER + 1,
                                 FLAG_BEARER_ANIMATION_RED_FLAG);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_FAILING_WARRIOR, facing, 20);
  Engine_ObjectEventSetAnimation(LOCALID_FAILING_WARRIOR, 8);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR,
                                 ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                                 ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR,
                                 ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_TRIAL_WARRIOR + 4,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FAILING_WARRIOR,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_FAILING_WARRIOR, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 162, 916);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 162, 890);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_FLAG_BEARER, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_FLAG_BEARER + 1,
                                 ANIMATION_STAND);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 184, 863);
  ObjectEvent_WalkToAndWait(LOCALID_FAILING_WARRIOR, 184, 796);
  ObjectEvent_SetPosition(LOCALID_FAILING_WARRIOR, 0, 0);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ScriptRequestWarp(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
  Engine_ScriptEnd();
}

/// Back in the Colosseum's stands from a trial
/// (COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS): the attendant says the warrior
/// failed, or after the second trial that he cleared it.
static void ColosseumEntrance_ExplainTrial(void) {
  Engine_ScriptStart();
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 1);

  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();

  Engine_ScriptWait(20);
  ColosseumEntrance_FaceDirection(LOCALID_TRIAL_ATTENDANT,
                                  FACING_SOUTH + FACING_STEP);
  Engine_ScriptSetMessage(MsgColosseumHaveMakeThroughCountlessMatches);

  if (Engine_GameFlagIsSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN) != 0) {
    gFieldWork->message = (gFieldWork->message + 1);
  }

  ColosseumEntrance_ShowMessageAndPause(LOCALID_TRIAL_ATTENDANT);
  ColosseumEntrance_FaceDirection(LOCALID_TRIAL_ATTENDANT,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_GameFlagSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN);

  Engine_ScriptEnd();
}

/// The Colosseum's trial grounds, arriving by
/// COLOSSEUM_CLEARED_TRIAL_FROM_STANDS: a warrior heaves the trial object up in
/// jolts and drops it, shaking the ground: he clears the trial
/// (FLAG_COLOSSO_CLEARED_TRIAL_SEEN).
static void ColosseumEntrance_ShowClearedTrial(void) {
  u32 counter;
  struct ObjectEvent *trialEffect;
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Script_PlaySong(SE_247);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH, ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 1,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 2,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 3,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 4,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_GROUNDS_FIRST_TORCH + 5,
                                 ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR + 1, 0);
  ObjectEvent_SetPosition(LOCALID_FAILING_WARRIOR, 0, 0);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_TRIAL_WARRIOR + 4, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_TRIAL_PILLAR);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_FLAG_BEARER);
  objectEvent->scaleX = -Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_FLAG_BEARER + 1);
  objectEvent->scaleX = -Q16_ONE;
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 32;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CLEARING_WARRIOR, 1);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_CLEARING_WARRIOR, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CLEARING_WARRIOR, 164, 904);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 9);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 10);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, ANIMATION_STAND);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_CLEARING_WARRIOR, 164, 920);
  ObjectEvent_WalkToAndWait(LOCALID_CLEARING_WARRIOR, 185, 920);
  ObjectEvent_FaceDirection(LOCALID_CLEARING_WARRIOR, FACING_NORTH, 20);
  ObjectEvent_WalkToAndWait(LOCALID_CLEARING_WARRIOR, 185, 916);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 11);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CLEARING_WARRIOR, 1);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CLEARING_WARRIOR, 3);
  Engine_ScriptWait(40);
  ObjectEvent_SetActionScript(LOCALID_CLEARING_WARRIOR,
                              gColosseumEntrance_PulseScaleScript);
  Engine_ScriptWait(80);
  ObjectEvent_SetAttachedEffect(LOCALID_CLEARING_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR,
                            FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                            FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 4, FACING_WEST, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TRIAL_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_CLEARING_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TRIAL_WARRIOR + 4,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 1,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_TRIAL_WARRIOR + 4,
                                  FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_SetAttachedEffect(LOCALID_CLEARING_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ATTACH);
  // The object rises a little with every heave.
  trialEffect = ObjectEvent_GetById(LOCALID_TRIAL_PILLAR);
  trialEffect->motionFlags = 0;
  for (counter = 0; counter < 20; counter++) {
    trialEffect->y.fixed += PIXELS(3) / 5;
    Engine_TaskWait(4);
    trialEffect->y.fixed += -PIXELS(3) / 10;
    Engine_TaskWait(4);
  }
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_FLAG_BEARER,
                                 FLAG_BEARER_ANIMATION_WHITE_FLAG);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_FLAG_BEARER + 1,
                                 FLAG_BEARER_ANIMATION_WHITE_FLAG);
  Engine_ScriptWait(60);
  ObjectEvent_SetAttachedEffect(LOCALID_CLEARING_WARRIOR,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_REMOVE);
  Engine_ObjectEventStop(LOCALID_CLEARING_WARRIOR);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, ANIMATION_STAND);
  objectEvent = ObjectEvent_GetById(LOCALID_CLEARING_WARRIOR);
  objectEvent->facing = FACING_NORTH + FACING_STEP;
  trialEffect->motionFlags = 3;
  trialEffect->scaleX = Q16_ONE;
  trialEffect->scaleY = Q16_ONE;
  Engine_ScriptWait(10);
  Script_PlaySong(SE_107);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_ScriptWait(10);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  ObjectEvent_SetSpeed(LOCALID_CLEARING_WARRIOR, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CLEARING_WARRIOR, 208, 928);
  Script_PlaySong(MUS_FANFARE_MEDIUM);
  ObjectEvent_FaceDirection(LOCALID_CLEARING_WARRIOR,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 9);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 10);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 9);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_CLEARING_WARRIOR, 10);
  Engine_ScriptWait(80);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  GameFlag_Set(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  Engine_ScriptRequestWarp(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
  Engine_ScriptEnd();
}

/// Sets up the Colosseum or its entrance as the party arrives. The Colosseum is
/// set up by the warp destination the party arrives at, and the Colosseum
/// Entrance by how far Colosso has gone.
s32 ColosseumEntrance_Initialize(void) {
  s32 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Colosseum) {
    ColosseumEntrance_ApplyArrivalState();
  } else if (mapId == (s32)&MapId_ColosseumEntrance) {
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
  s32 warpDest;

  Engine_TaskWait(1);
  warpDest = gGameState.warpDest;
  switch (warpDest) {
  case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
    Engine_ObjectEventSetAnimation(LOCALID_STANDS_WEST_TORCH, ANIMATION_WALK);
    Engine_ObjectEventSetAnimation(LOCALID_STANDS_EAST_TORCH, ANIMATION_WALK);
    break;
  case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
    Engine_ObjectEventSetAnimation(LOCALID_STANDS_WEST_TORCH, ANIMATION_WALK);
    Engine_ObjectEventSetAnimation(LOCALID_STANDS_EAST_TORCH, ANIMATION_WALK);
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
    Party_AddMember(GARET);
    Party_AddMember(IVAN);
    Party_AddMember(MIA);
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
    Party_AddMember(GARET);
    Party_AddMember(IVAN);
    Party_AddMember(MIA);
    Engine_GameFlagSet(FLAG_COLOSSO_SECOND_MATCH_WON);
    ColosseumEntrance_CelebrateBothWins();
    break;
  default:
    break;
  }
}

static void ColosseumEntrance_ShowMessageAndPause(s32 speaker) {
  Engine_ScriptShowMessage(speaker, 0);
  Engine_ScriptWait(10);
}

/// Turns an object event to the given facing over ten frames.
static void ColosseumEntrance_FaceDirection(s32 localId, s32 facing) {
  Engine_ObjectEventFaceDirection(localId, (u16)facing, 10);
}

/// The Colosseum Entrance's setup: once Colosso is over, the tiles and cells by
/// the gates change; before that, the Lucky Guess man (LOCALID_LUCKY_GUESS_MAN)
/// takes child value 2, and on the finals day a cell pair before the gates
/// changes.
static void ColosseumEntrance_UpdateEntranceCells(void) {
  s32 column, row;

  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    MapGrid_CopyCellsTo(64, 0, 48, 5, 2, 2);
    column = 16;
    row = 8;
    MapGrid_CopyCellAttributes(14, 8, 2, 1, column, row);
  } else {
    Engine_ObjectEventSetChildValue(LOCALID_LUCKY_GUESS_MAN, 2);
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      column = 14;
      row = 11;
      MapGrid_CopyCellAttributes(30, 22, 1, 2, column, row);
    }
  }
}

/// Places the three companions behind Isaac for the soldiers' briefing.
static inline void ColosseumEntrance_PositionPartyForBriefing(void) {
  ObjectEvent_SetPosition(GARET, PIXELS(792), PIXELS(136));
  ObjectEvent_SetPosition(IVAN, PIXELS(824), PIXELS(136));
  ObjectEvent_SetPosition(MIA, PIXELS(808), PIXELS(152));
}

/// The Colosseum's briefing, the party's arrival from the gates
/// (COLOSSEUM_FROM_COLOSSEUM_ENTRANCE): only Isaac may enter the finals, so the
/// soldiers let his companions cheer him on from the stage, and at Ivan's
/// asking they agree to show Isaac the finals stages before the first match. An
/// soldier then leads the party on
/// (COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST).
static void ColosseumEntrance_ExplainPartySupport(void) {
  s32 flag;

  Engine_ScriptStart();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_TaskWait(1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_ColorBufferApplySource(0, 0);
  PaletteFade_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 3);
  gFieldWork->transitionFrames = 1;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ColorBufferApplySource(0, 0);
  PaletteFade_ApplyTarget(0x10002, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_ScriptWait(80);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER, 1);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(40);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER, 2);
  Engine_ScriptWait(20);
  PaletteFade_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_ScriptWait(80);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  Engine_ScriptWait(20);
  // The soldiers explain Isaac's selection and the companions' role.
  Engine_ScriptSetMessage(MsgColosseumIsaacOnlyOneEnteringFinals);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER + 1, FACING_EAST, 40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_SOUTH + FACING_STEP,
                            0);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER + 1,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 1);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER + 2, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 2);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ESCORTING_SOLDIER,
                                        ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_ESCORTING_SOLDIER);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTHWEST);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ColosseumEntrance_FaceDirection(GARET, FACING_EAST);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  ColosseumEntrance_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_WEST);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  ColosseumEntrance_FaceDirection(GARET, FACING_SOUTHEAST);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTH);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ColosseumEntrance_FaceDirection(MIA, FACING_NORTH);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ESCORTING_SOLDIER,
                                        ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_ESCORTING_SOLDIER);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTHEAST);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 20);
  ObjectEvent_ShowEmote(LOCALID_ESCORTING_SOLDIER,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_OpenMessage(IVAN, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  flag = 0;
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    ColosseumEntrance_ShowMessageAndPause(IVAN);
    flag = 1;
  } else {
    gFieldWork->message++;
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    ColosseumEntrance_FaceDirection(IVAN, FACING_NORTH);
    ColosseumEntrance_ShowMessageAndPause(IVAN);
  }
  if (flag != 0) {
    gFieldWork->message++;
  }
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER + 1, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER + 2,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_ESCORTING_SOLDIER,
                            FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER + 1,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER + 2,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_ESCORTING_SOLDIER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER, 1);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 1);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_FaceDirection(LOCALID_FIRST_SOLDIER + 2,
                                  FACING_SOUTH + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 2);
  ColosseumEntrance_FaceDirection(LOCALID_ESCORTING_SOLDIER, FACING_WEST);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ESCORTING_SOLDIER,
                                        ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_ESCORTING_SOLDIER);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  // Rejoin the companions before the soldier leads Isaac onward.
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetActionScript(GARET, gColosseumEntrance_RejoinPartyScript);
  ObjectEvent_SetActionScript(IVAN, gColosseumEntrance_RejoinPartyScript);
  ObjectEvent_RunActionScriptAndWait(MIA, gColosseumEntrance_RejoinPartyScript);
  Engine_ScriptWait(20);
  ColosseumEntrance_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ESCORTING_SOLDIER,
                                        ANIMATION_NOD);
  ObjectEvent_SetSpeed(LOCALID_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 830, 152);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 808, 164);
  ObjectEvent_SetDestination(LOCALID_ESCORTING_SOLDIER, 808, 312);
  Engine_ScriptWait(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 808, 164);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 808, 312);
  Engine_ScriptWait(60);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 40;
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/// The Colosseum's east passage
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST): a soldier leads Isaac
/// to the next finals match's tour, Colosseum Final 1 or, after the second win,
/// Colosseum Final 3.
static void ColosseumEntrance_EscortToEastExit(void) {
  struct ObjectEvent *escort;

  if (GameFlag_IsSet(JENNA) != 0) {
    GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
    Party_RemoveMember(JENNA);
    Party_AddMember(MIA);
  }
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_ESCORTING_SOLDIER, PIXELS(712), PIXELS(588));
  Engine_TaskWait(1);
  Engine_CameraFollowObjectEvent(LOCALID_ESCORTING_SOLDIER, 1);
  ObjectEvent_SetSpeed(LOCALID_ESCORTING_SOLDIER, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = ObjectEvent_GetById(LOCALID_ESCORTING_SOLDIER);
  escort->facing = FACING_EAST;
  Engine_ScriptOpenScreen();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_WALK);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 780, 588);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 812, 588);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 880, 588);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 912, 588);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 980, 588);
  ObjectEvent_SetDestination(LOCALID_ESCORTING_SOLDIER, 1012, 588);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  if (GameFlag_IsSet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Engine_ScriptRequestWarp(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH);
  } else {
    Engine_ScriptRequestWarp(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH);
  }
}

/// Arriving in the Colosseum's waiting room from Colosseum Final 1's tour: the
/// soldier tells the warriors to prepare.
static void ColosseumEntrance_AnnounceFinals(void) {
  struct ObjectEvent *contestant;

  contestant = ObjectEvent_GetById(LOCALID_AZART);
  Engine_ScriptStart();
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FINALS_SOLDIER, 2);
  Engine_ObjectEventStop(LOCALID_AZART);
  Engine_TaskWait(1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  Engine_ObjectEventSetAnimation(LOCALID_AZART, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_MORGAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_AZART, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GALAHAD, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_SATRAGE, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DEKKA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_NAVAMPA, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_BUFORD, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptSetMessage(MsgColosseumPrepareYourselvesContestantsFinalsWill);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FINALS_SOLDIER);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  // Azart's wander home, as in EventScript_TalkToAzart.
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
  ObjectEvent_SetBuiltinAction(LOCALID_AZART, MOVEMENT_TYPE_WANDER);
  ObjectEvent_FaceDirection(LOCALID_MORGAN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GALAHAD, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATRAGE, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DEKKA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_NAVAMPA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_BUFORD, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptEnd();
}

/// The Colosseum's east passage (COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH): Isaac
/// walks alone into Colosseum Final 1 or, after the second win, Colosseum Final
/// 3, where his friends take their turns to lend a hand
/// (COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST or
/// COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH).
static void ColosseumEntrance_WalkToEastExit(void) {
  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ScriptOpenScreen();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 780, 428);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 880, 428);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 980, 428);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  if (GameFlag_IsSet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Engine_ScriptRequestWarp(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH);
  } else {
    Engine_ScriptRequestWarp(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST);
  }
}

/// Back in the Colosseum after the first win
/// (COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST): the soldiers congratulate
/// Isaac and lead him on to Colosseum Final 2.
static void ColosseumEntrance_CelebrateAdvancement(void) {
  const s32 *actions;

  Engine_ScriptStart();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER, 1);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_SOLDIER, ANIMATION_NOD);
  Engine_ScriptSetMessage(MsgColosseumSeeYouveMadeThroughYour);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 1);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ESCORTING_SOLDIER, 1);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_ESCORTING_SOLDIER);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  ObjectEvent_SetActionScript(GARET, actions);
  ObjectEvent_SetActionScript(IVAN, actions);
  ObjectEvent_RunActionScriptAndWait(MIA, actions);
  Engine_ScriptWait(20);
  ColosseumEntrance_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ESCORTING_SOLDIER,
                                        ANIMATION_NOD);
  ObjectEvent_SetSpeed(LOCALID_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 830, 152);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 808, 164);
  ObjectEvent_SetDestination(LOCALID_ESCORTING_SOLDIER, 808, 312);
  Engine_ScriptWait(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 808, 164);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 808, 312);
  Engine_ScriptWait(60);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST);
}

/// The Colosseum's west passage
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST): a soldier leads Isaac
/// to Colosseum Final 2's tour.
static void ColosseumEntrance_EscortToWestExit(void) {
  struct ObjectEvent *escort;

  if (GameFlag_IsSet(JENNA) != 0) {
    GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
    Party_RemoveMember(JENNA);
    Party_AddMember(MIA);
  }
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_ESCORTING_SOLDIER, PIXELS(868), PIXELS(588));
  Engine_TaskWait(1);
  Engine_CameraFollowObjectEvent(LOCALID_ESCORTING_SOLDIER, 1);
  ObjectEvent_SetSpeed(LOCALID_ESCORTING_SOLDIER, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = ObjectEvent_GetById(LOCALID_ESCORTING_SOLDIER);
  escort->facing = FACING_WEST;
  Engine_ScriptOpenScreen();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_WALK);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 800, 588);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 768, 588);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 700, 588);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 668, 588);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 600, 588);
  ObjectEvent_SetDestination(LOCALID_ESCORTING_SOLDIER, 568, 588);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST);
}

/// The Colosseum's west passage (COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH):
/// Isaac walks alone into Colosseum Final 2, where his friends take their turns
/// to lend a hand (COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST).
static void ColosseumEntrance_WalkToWestExit(void) {
  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ScriptOpenScreen();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 800, 428);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 700, 428);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 600, 428);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST);
}

/// Back in the Colosseum after the second win
/// (COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH): one more win and Isaac is
/// the champion; the soldiers lead him back east
/// (COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST), now to Colosseum
/// Final 3.
static void ColosseumEntrance_CelebrateBothWins(void) {
  const s32 *actions;

  Engine_ScriptStart();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER, 1);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_SOLDIER, ANIMATION_NOD);
  Engine_ScriptSetMessage(MsgColosseumWonBothMatches);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 1);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_FIRST_SOLDIER + 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ESCORTING_SOLDIER, 1);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_NOD);
  ColosseumEntrance_ShowMessageAndPause(LOCALID_ESCORTING_SOLDIER);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  ObjectEvent_SetActionScript(GARET, actions);
  ObjectEvent_SetActionScript(IVAN, actions);
  ObjectEvent_RunActionScriptAndWait(MIA, actions);
  Engine_ScriptWait(20);
  ColosseumEntrance_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ESCORTING_SOLDIER,
                                        ANIMATION_NOD);
  ObjectEvent_SetSpeed(LOCALID_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetAnimation(LOCALID_ESCORTING_SOLDIER, ANIMATION_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 830, 152);
  ObjectEvent_MoveToAndWait(LOCALID_ESCORTING_SOLDIER, 808, 164);
  ObjectEvent_SetDestination(LOCALID_ESCORTING_SOLDIER, 808, 312);
  Engine_ScriptWait(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 808, 164);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 808, 312);
  Engine_ScriptWait(60);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/// The warriors' child values and their waiting actions in the Colosseum's
/// waiting room.
static void ColosseumEntrance_InitializeContestants(void) {
  Engine_ObjectEventSetChildValue(LOCALID_MORGAN, 3);
  Engine_ObjectEventSetChildValue(LOCALID_AZART, 0);
  Engine_ObjectEventSetChildValue(LOCALID_GALAHAD, 4);
  Engine_ObjectEventSetChildValue(LOCALID_SATRAGE, 1);
  Engine_ObjectEventSetChildValue(LOCALID_DEKKA, 5);
  Engine_ObjectEventSetChildValue(LOCALID_NAVAMPA, 2);
  Engine_ObjectEventSetChildValue(LOCALID_BUFORD, 6);
  ObjectEvent_SetActionById(LOCALID_AZART, 10);
  ObjectEvent_SetActionById(LOCALID_GALAHAD, 20);
  Engine_ObjectEventSetAnimation(LOCALID_SATRAGE, 0);
  ObjectEvent_SetActionById(LOCALID_DEKKA, 40);
  ObjectEvent_SetActionById(LOCALID_NAVAMPA, 50);
  ObjectEvent_SetActionById(LOCALID_BUFORD, 60);
}

s32 gColosseumEntrance_GatherIntoIsaacScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(262), 0, PIXELS(188)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gColosseumEntrance_GatherIntoGaretScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(246), 0, PIXELS(200)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gColosseumEntrance_RejoinPartyScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(120)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gColosseumEntrance_EnterGatesScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(268), 0, PIXELS(146)),
    SC_SYNC,         SC_MOVE(PIXELS(262), 0, PIXELS(24)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gColosseumEntrance_ApproachFinalsGroupScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(776), 0, PIXELS(120)),          SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP), SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_JoinFinalsFromEastScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(894), 0, PIXELS(106)),
    SC_SYNC,
    SC_MOVE(PIXELS(882), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(792), 0, PIXELS(152)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_JoinFinalsFromSouthScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(824), 0, PIXELS(152)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_EnterFinalsScript[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(2) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(88)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(58)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
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

struct MapWarpDest gColosseumEntranceWarpDestsOther[] = {
    {0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {COLOSSEUM_TUNNEL_FROM_BABI_PALACE, CONDITION_ALWAYS, 72, 0, 72,
     FACING_SOUTH, 0, 16, 16, 528, 256, 0},
    {COLOSSEUM_TUNNEL_FROM_COLOSSEUM, CONDITION_ALWAYS, 488, 0, 88,
     FACING_SOUTH, 0, 16, 16, 528, 256, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gColosseumWarpDests[] = {
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
    {MAP_TABLE_END},
};

struct MapWarpDest gColosseumEntranceWarpDests[] = {
    {COLOSSEUM_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 96, 0, 288, FACING_NORTH,
     0, 0, 0, 512, 320, 0},
    {2, CONDITION_ALWAYS, 272, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 320, 0},
    {MAP_TABLE_END},
};

u32 gColosseumEntranceWarps[] = {
    MAP_WARPS(ColosseumTunnel),
    MAP_WARP(COLOSSEUM_TUNNEL_TO_BABI_PALACE, BabiPalace,
             BABI_PALACE_FROM_COLOSSEUM_TUNNEL),
    MAP_WARP(COLOSSEUM_TUNNEL_TO_COLOSSEUM, Colosseum,
             COLOSSEUM_FROM_COLOSSEUM_TUNNEL),
    MAP_WARPS(Colosseum),
    MAP_WARP(COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH, Colosseum,
             COLOSSEUM_STANDS_FROM_STONE_PASSAGE_NORTH),
    MAP_WARP(COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST, Colosseum,
             COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST),
    MAP_WARP(COLOSSEUM_STONE_PASSAGE_LOCKED_DOOR_TO_STONE_PASSAGE, Colosseum,
             COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_TUNNEL, ColosseumTunnel,
             COLOSSEUM_TUNNEL_FROM_COLOSSEUM),
    MAP_WARP(COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST, Colosseum,
             COLOSSEUM_STONE_PASSAGE_FROM_STANDS_WEST),
    MAP_WARP(COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH, Colosseum,
             COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH),
    MAP_WARP(COLOSSEUM_STANDS_TO_FAILED_TRIAL, Colosseum,
             COLOSSEUM_FAILED_TRIAL_FROM_STANDS),
    MAP_WARP(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS, Colosseum,
             COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS),
    MAP_WARP(COLOSSEUM_STANDS_TO_CLEARED_TRIAL, Colosseum,
             COLOSSEUM_CLEARED_TRIAL_FROM_STANDS),
    MAP_WARP(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST, Colosseum,
             COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH, ColosseumAzartMatch,
             COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM),
    MAP_WARP(COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST, Colosseum,
             COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
    MAP_WARP(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST, Colosseum,
             COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST, ColosseumAzartMatch,
             COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST, ColosseumSatrageMatch,
             COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST, ColosseumSatrageMatch,
             COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH, ColosseumNavampaMatch,
             COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH),
    MAP_WARP(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH, ColosseumNavampaMatch,
             COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH),
    MAP_WARPS(ColosseumEntrance),
    MAP_WARP(COLOSSEUM_ENTRANCE_TO_TOLBI, Tolbi, TOLBI_FROM_COLOSSEUM_ENTRANCE),
    MAP_WARP(COLOSSEUM_ENTRANCE_TO_COLOSSEUM, Colosseum,
             COLOSSEUM_FROM_COLOSSEUM_ENTRANCE),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gColosseumEntranceObjectEventTemplatesOther[] = {
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumStandsObjectEventTemplates[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(592), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(528), 0, PIXELS(88), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(432), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(476), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(454), 0, PIXELS(104), FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(208), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(444), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(428), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(240), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(192), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(608), 0, PIXELS(104), FACING_SOUTHEAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumTrialGroundsObjectEventTemplates[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(250), 0, PIXELS(846), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(298), 0, PIXELS(912), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(250), 0, PIXELS(974), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(121), 0, PIXELS(974), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(912), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(121), 0, PIXELS(846), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(96), 0, PIXELS(900), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(114), 0, PIXELS(884), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(148), 0, PIXELS(864), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(164), 0, PIXELS(860), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(212), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(902), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumObjectEventTemplates[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(72), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(72), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumSoldiersObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(782), 0, PIXELS(96), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(832), 0, PIXELS(96), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumObjectEventTemplatesFromColosseumAzartMatchWest[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(234), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(778), 0, PIXELS(72), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(760), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(720), 0, PIXELS(112), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, 3, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(920), 0, PIXELS(134), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumObjectEventTemplatesFromColosseumNavampaMatch[] = {
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(920), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1016), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumEntranceObjectEventTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(266), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(152), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(198), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(48), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(80), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(56), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(140), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(64), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(72), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumEntranceObjectEventTemplatesFinalsDay[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(182), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(294), 0, PIXELS(178), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(228), 0, PIXELS(192), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(438), 0, PIXELS(186), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(186), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(258), 0, PIXELS(161), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(244), 0, PIXELS(150), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(161), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(168), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(52), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(70), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(88), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(106), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(124), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(142), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(160), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gColosseumEntranceObjectEventTemplatesAfterColosso[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(286), 0, PIXELS(164), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gColosseumEntranceEventsOther[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gColosseumTunnelEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_BABI_PALACE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_COLOSSEUM },
    { MAP_EVENTS_END },
};

struct MapEvent gColosseumEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, COLOSSEUM_TO_COLOSSEUM_TUNNEL },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH },
    { MAP_EVENT_TALK, LOCALID_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceGoForItThatsItIm) },
    { MAP_EVENT_TALK, LOCALID_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingTheTrialsMakesMeReally) },
    { MAP_EVENT_TALK, LOCALID_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn) },
    { MAP_EVENT_TALK, LOCALID_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIveWaitedAWholeYearTo) },
    { MAP_EVENT_TALK, LOCALID_TRIAL_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToTrialAttendant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceEvenTheSmallerWarriorsCanWin) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheseTrialsAreBasedOnPersonal) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatFantasticPhysiquesWhyIfI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColossoDidntThinkBattles) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TRIAL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceNoMatterHowGoodAWarrior) },
    { BG_EVENT(BG_EVENT_SILENT), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianItIsLocked)) },
    { MAP_EVENTS_END },
};

struct MapEvent gColosseumEntranceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_TOLBI },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_COLOSSEUM },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheFinalsAreFinallyHereI) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceColossoIsOverAndTheColosseum) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceSighTheColosseumIsPackedWith) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceIHeardBabiHasPickedSome) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheColosseumIsClosedUntilNext) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_SEATING, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DiscussSeating) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIfYouHitItBigIn) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAWarriorNamedIsaacWasSelected) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIveBeenBettingOnTheLucky) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceHavingAWarriorWhoWasntIn) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DiscussLuckyGuessSeats) },
    { MAP_EVENT_TALK, LOCALID_SCALPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DiscussLuckyGuessWinnings) },
    { MAP_EVENT_TALK, LOCALID_LUCKY_GUESS_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OfferLuckyGuess) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIWonderWhichWarriorWillBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheRealBattlesAreAboutTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhenTheColosseumGatesFinallyShut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceHavingToArriveSoEarlyJust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheWarriorNamedIsaacIsSure) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheFightingAtThisYearsColosso) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SEATING, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceThereAreSoManyPeopleWho) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SEATING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceOfCourseItsTheRichWho) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImGoingToPickTheBiggest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceThereCanBeUpsetsInThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImFeelingConfidentThisTimeIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatKindOfWarriorIsThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_GOOD_SPOT, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryoneKeepsRiskingMoreAndMore) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceMaybeIWillBuyASeat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCALPER, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryYearIGoAfterA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCALPER, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceImGoingToBeInTrouble) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LUCKY_GUESS_MAN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceOnceTheTrialsAreOverIts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LUCKY_GUESS_MAN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAllOfTheProfitsFromThe) },
    { MAP_EVENT_COORD, TRIGGER_BEFORE_GATES, FLAG_COLOSSO_OVER, EVENT_SCRIPT(EventScript_CrowdRecognizesIsaac) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gColosseumEventsFromColosseumAzartMatchWest[] = {
    {MAP_EVENT_TALK, LOCALID_MORGAN, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIAmMorganIAmThe)},
    {MAP_EVENT_TALK, LOCALID_AZART, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToAzart)},
    {MAP_EVENT_TALK, LOCALID_GALAHAD, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToGalahad)},
    {MAP_EVENT_TALK, LOCALID_SATRAGE, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToSatrage)},
    {MAP_EVENT_TALK, LOCALID_DEKKA, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToDekka)},
    {MAP_EVENT_TALK, LOCALID_NAVAMPA, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToNavampa)},
    {MAP_EVENT_TALK, LOCALID_BUFORD, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToBuford)},
    {MAP_EVENT_TALK, LOCALID_FINALS_SOLDIER, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_AskIfReadyForFinals)},
    {MAP_EVENT_TALK, LOCALID_EXIT_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceYouCannotLeaveUntilTheFinals)},
    {MAP_EVENT_TALK, LOCALID_WAITING_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIFeelWeveBeenWaitingForever)},
    {MAP_EVENT_TALK, LOCALID_GONG_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceWhenTheGongSoundsTheFinals)},
    {MAP_EVENT_COORD, TRIGGER_FINALS_LINE, FLAG_FINALS_LINE_REACHED,
     EVENT_SCRIPT(EventScript_FinalsStart)},
    {MAP_EVENT_COORD, TRIGGER_BEHIND_FINALS_LINE, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_RearmFinalsStart)},
    {MAP_EVENTS_END},
};
