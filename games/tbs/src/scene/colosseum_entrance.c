/// @file
/// Tolbi's colosseum, where Colosso is fought, in three scenes: the Colosseum
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
///   ColosseumEntrance_GetActorTemplates places a second crowd, with
///   Isaac's companions and four of the colosseum's soldiers, and the
///   triggers' flag conditions give everyone new lines: the scalpers sell
///   seats, and the Lucky Guess man recognizes Isaac and will not let a
///   finalist play (FLAG_LUCKY_GUESS_REFUSED_ISAAC remembers that he said so).
///   Stepping toward the gates (ColosseumEntrance_CrowdRecognizesIsaac) plays
///   the soldiers' quarrel over the boy Babi picked; they lead the party in,
///   and if Isaac will not go, they and his companions talk him round, Garet
///   leading the way if he still refuses. The party arrives at the Colosseum's
///   COLOSSEUM_FROM_COLOSSEUM_ENTRANCE. Once Colosso is over,
///   ColosseumEntrance_UpdateEntranceCells changes the layout's cells before
///   the gates, and two people linger to say the colosseum is closed until next
///   year. COLOSSEUM_ENTRANCE_TO_TOLBI leads back to Tolbi.
/// - Colosseum Tunnel (SceneId_ColosseumTunnel), the passage from Babi's
///   Palace (BABI_PALACE_TO_COLOSSEUM_TUNNEL) into the Colosseum, whose end
///   of the tunnel (COLOSSEUM_TUNNEL_FROM_COLOSSEUM and
///   COLOSSEUM_FROM_COLOSSEUM_TUNNEL) takes the same area name. It has only its
///   two exits.
/// - Colosseum (SceneId_Colosseum), one scene that holds the stands, the trial
///   grounds, the finals' waiting room and the passages to the finals stages,
///   each an entrance's camera box on it.
///   Every arrival is an entrance of its own, and
///   ColosseumEntrance_ApplyArrivalState plays the event that
///   entrance stands for. During the trials the stands' entrances
///   (COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST,
///   COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS) alternate with the two trial
///   events (COLOSSEUM_FAILED_TRIAL_FROM_STANDS,
///   COLOSSEUM_CLEARED_TRIAL_FROM_STANDS), which FLAG_COLOSSO_FAILED_TRIAL_SEEN
///   and FLAG_COLOSSO_CLEARED_TRIAL_SEEN record. On the finals day
///   COLOSSEUM_FROM_COLOSSEUM_ENTRANCE keeps the party's items
///   (InventorySnapshot_Save) for Babi's Palace to give back, and the waiting
///   room's line asks whether Isaac is ready (ColosseumEntrance_FinalsStart).
///   Each finals stage returns to its own entrance here
///   (COLOSSEUM_FROM_COLOSSEUM_*_MATCH_*), and FLAG_COLOSSO_FIRST_MATCH_WON and
///   FLAG_COLOSSO_SECOND_MATCH_WON choose which stage the long passages lead to
///   next; the stages themselves are colosseum_azart_match.c,
///   colosseum_satrage_match.c and colosseum_navampa_match.c.
///
/// The three scenes share one exit table, a run of exits for each scene, and
/// the Colosseum's exits are how its arrivals chain into one another and into
/// the finals stages.

#include "persistent_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/colosseum_entrance.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "io_reg.h"
#include "audio_engine_symbols.h"

static void ColosseumEntrance_ApplyArrivalState(void);
static void ColosseumEntrance_ShowMessageAndPause(s32 speaker);
static void ColosseumEntrance_FaceDirection(s32 actorId, s32 facing);
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

static inline void ColosseumEntrance_Turn(s32 actorId, s32 facing) {
  /* FAKEMATCH(wrapper): ColosseumEntrance_CrowdRecognizesIsaac's one turn goes
   * through this helper; as a direct ColosseumEntrance_FaceDirection call, 7 of
   * 643 instructions of ColosseumEntrance_CrowdRecognizesIsaac differ in
   * tbs-en. */
  ColosseumEntrance_FaceDirection(actorId, facing);
}

/// The Colosseum Entrance's people, numbered by the rows of
/// gColosseumEntranceActorTemplates;
/// gColosseumEntranceActorTemplatesFinalsDay places the same people in
/// its first nine rows after Isaac's companions, who take their own
/// actor slots, and gColosseumEntranceActorTemplatesAfterColosso keeps
/// the first two. They are named by what they say during the trials, except
/// where the finals day says more about who they are.
enum ColosseumEntranceActorId {
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
  /// (ColosseumEntrance_DiscussSeating). "Are you hoping to get in to see
  /// Colosso
  /// now?"
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
  /// the waiting room), gColosseumEntranceActorTemplatesFinalsDay only.
  /// The
  /// first greets Isaac ("Hey, you're Isaac!") and leads the party in.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear,
  /// SPRITE_SOLDIER (its render)
  ACTOR_FIRST_GATE_SOLDIER
};

/// The Colosseum's stands, numbered by the rows of
/// gColosseumStandsActorTemplates.
enum ColosseumStandsActorId {
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
/// gColosseumTrialGroundsActorTemplates.
enum ColosseumTrialActorId {
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
  FLAG_BEARER_ANIMATION_RED_FLAG = 5, ///< raises a red flag: the warrior failed
  FLAG_BEARER_ANIMATION_WHITE_FLAG = 6 ///< raises a white flag: he cleared it
};

/// The four soldiers of gColosseumSoldiersActorTemplates
/// (SPRITE_SOLDIER), who brief the party at COLOSSEUM_FROM_COLOSSEUM_ENTRANCE,
/// congratulate Isaac after his wins and lead him through the passages; Isaac's
/// companions take their own actor slots.
enum ColosseumSoldierActorId {
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
/// gColosseumActorTemplatesFromColosseumAzartMatchWest: four soldiers and
/// the seven warriors who cleared the trials, as they introduce themselves.
enum ColosseumWaitingRoomActorId {
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

/// The metatile behaviors the Colosseum Entrance's triggers answer.
enum ColosseumEntranceMetatileBehavior {
  /// Before the gates (ColosseumEntrance_CrowdRecognizesIsaac).
  MB_COLOSSEUM_ENTRANCE_BEFORE_GATES = 10
};

/// The metatile behaviors the Colosseum's triggers answer.
enum ColosseumMetatileBehavior {
  /// The line before the waiting room's east passage
  /// (ColosseumEntrance_FinalsStart).
  MB_COLOSSEUM_FINALS_LINE = 12,
  /// The cells behind it (ColosseumEntrance_StepBackFromFinalsLine).
  MB_COLOSSEUM_BEHIND_FINALS_LINE = 13
};

/// The temporary flags the colosseum's scenes keep.
enum {
  /// The Lucky Guess man has refused Isaac as a finalist
  /// (ColosseumEntrance_OfferLuckyGuess).
  /// ★ Named from the game: "Colosso's Lucky Guess makes the battles in the
  /// colosseum better." (MsgColosseumColossosLuckyGuessMakesBattles)
  FLAG_LUCKY_GUESS_REFUSED_ISAAC = FLAG_AREA_TEMP_C0,
  /// Navampa has explained that Dekka will not answer
  /// (ColosseumEntrance_TalkToDekka).
  /// ★ Named from the game: "Dekka must win the finals."
  /// (MsgColosseumDekkaMustWinFinalsDekka)
  FLAG_NAVAMPA_SPOKE_FOR_DEKKA = FLAG_AREA_TEMP_C1,
  /// The leader has crossed the cells behind the finals line, so stepping onto
  /// it asks whether Isaac is ready.
  /// ★ Named from the game: "If you win there, you advance to the finals."
  /// (MsgTolbiTheTrialsAreStartingAtThe)
  FLAG_FINALS_LINE_ARMED = FLAG_SCENE_TEMP_34,
  /// The leader has stepped onto the line since; its step trigger is off.
  /// ★ Named from the game: "If you win there, you advance to the finals."
  /// (MsgTolbiTheTrialsAreStartingAtThe)
  FLAG_FINALS_LINE_REACHED = FLAG_SCENE_TEMP_35
};

const struct SceneEntrance *ColosseumEntrance_GetEntrances(void) {
  s16 sceneId = gPersistentState.sceneId;

  if (sceneId == (s32)&SceneId_Colosseum) {
    return gColosseumEntrances;
  }
  if (sceneId == (s32)&SceneId_ColosseumEntrance) {
    return gColosseumEntranceEntrances;
  }
  return gColosseumEntranceEntrancesOther;
}

const struct SceneExitRegion *ColosseumEntrance_GetRegions(void) { return 0; }

/// Returns the shared colosseum exits.
const u32 *ColosseumEntrance_GetExits(void) { return gColosseumEntranceExits; }

/// Selects the actors: in the Colosseum by the entrance the
/// party arrives at
/// (the stands, the trial grounds, the passages, the waiting room or the
/// soldiers' briefing), at the Colosseum Entrance by how far Colosso has gone.
const struct ActorTemplate *ColosseumEntrance_GetActorTemplates(void) {
  const struct ActorTemplate *table;

  if (gPersistentState.sceneId == (s32)&SceneId_Colosseum) {
    switch (gPersistentState.entrance) {
    case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
    case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
      table = gColosseumStandsActorTemplates;
      break;
    case COLOSSEUM_FAILED_TRIAL_FROM_STANDS:
    case COLOSSEUM_CLEARED_TRIAL_FROM_STANDS:
      table = gColosseumTrialGroundsActorTemplates;
      break;
    // XXX (no evidence): what entrances 3 and 8 were made for; no exit
    // or code enters by them, only the debug scene selector can, and 8 takes
    // the soldiers' table, as the arrivals of the finals day do.
    case 8:
    case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
    case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH:
    case COLOSSEUM_FROM_COLOSSEUM_ENTRANCE:
    case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
    case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST:
      table = gColosseumSoldiersActorTemplates;
      break;
    case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
      table = gColosseumActorTemplatesFromColosseumAzartMatchWest;
      break;
    case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
    case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH:
      table = gColosseumActorTemplatesFromColosseumNavampaMatch;
      break;
    default:
      table = gColosseumActorTemplates;
      break;
    }
  } else if (gPersistentState.sceneId == (s32)&SceneId_ColosseumEntrance) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      table = gColosseumEntranceActorTemplatesAfterColosso;
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      table = gColosseumEntranceActorTemplatesFinalsDay;
    } else {
      table = gColosseumEntranceActorTemplates;
    }
  } else {
    table = gColosseumEntranceActorTemplatesOther;
  }
  return table;
}

const struct Trigger *ColosseumEntrance_GetTriggers(void) {
  s16 sceneId = gPersistentState.sceneId;

  if (sceneId == (s32)&SceneId_ColosseumTunnel) {
    return gColosseumTunnelTriggers;
  }
  if (sceneId == (s32)&SceneId_Colosseum) {
    if (gPersistentState.entrance ==
        COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST) {
      return gColosseumTriggersFromColosseumAzartMatchWest;
    }
    return gColosseumTriggers;
  }
  if (sceneId == (s32)&SceneId_ColosseumEntrance) {
    return gColosseumEntranceTriggers;
  }
  return gColosseumEntranceTriggersOther;
}

/// At the Colosseum Entrance: during the trials, a burly grey-haired man asks
/// whether the party hopes to get in to see Colosso; on the finals day
/// (FLAG_BABI_SAVED) the colosseum is already full, and only the scalpers have
/// seats.
static void ColosseumEntrance_DiscussSeating(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_BABI_SAVED)) {
    Event_SetMessageId(MsgColosseumColosseumAlreadyFullIfReally);
    Event_ShowMessageAndWait(ACTOR_VILLAGER_SEATING, 0);
  } else {
    Event_SetMessageId(MsgColosseumHopingGetInSeeColosso);
    Event_AskYesNoAndShowResponse(ACTOR_VILLAGER_SEATING, 0);
  }
  Event_End();
}

/// At the Colosseum Entrance, an old woman: the Lucky Guess makes the battles
/// better; on the finals day, a good spot costs too much.
static void ColosseumEntrance_DiscussLuckyGuessSeats(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_BABI_SAVED)) {
    FieldEffect_SpawnAttached(ACTOR_OLD_WOMAN_GOOD_SPOT,
                              EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Event_SetMessageId(MsgColosseumWantWatchFinalsFromGood);
    Event_ShowMessageAndWait(ACTOR_OLD_WOMAN_GOOD_SPOT, 0);
  } else {
    Event_SetMessageId(MsgColosseumColossosLuckyGuessMakesBattles);
    Event_ShowMessageAndWait(ACTOR_OLD_WOMAN_GOOD_SPOT, 0);
  }
  Event_End();
}

/// At the Colosseum Entrance: a strongman, betting, put it all on a warrior no
/// one picked; on the finals day a scalper, busy with a customer, asks if the
/// party wants seats.
static void ColosseumEntrance_DiscussLuckyGuessWinnings(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    Actor_RunRepeatedMotion(ACTOR_SCALPER, 2);
    Event_SetMessageId(MsgColosseumHoldOnSecond);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_SCALPER);
    Actor_LinkPair(ACTOR_SCALPER, ACTOR_PARTY_LEADER, 0);
    Event_WaitTicks(20);
    Event_AskYesNoAndShowResponse(ACTOR_SCALPER, 0);
    ColosseumEntrance_FaceDirection(ACTOR_SCALPER, FACING_EAST);
  } else {
    Event_SetMessageId(MsgColosseumWantWinBigPutOn);
    Event_ShowMessageAndWait(ACTOR_SCALPER, 0);
  }
  Event_End();
}

/// The Lucky Guess man at the Colosseum Entrance explains the game; on the
/// finals day he recognizes Isaac as Babi's pick and refuses him, as finalists
/// may not play (FLAG_LUCKY_GUESS_REFUSED_ISAAC: said once).
static void ColosseumEntrance_OfferLuckyGuess(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    if (Event_GetFlag(FLAG_LUCKY_GUESS_REFUSED_ISAAC) != 0) {
      Event_SetMessageId(MsgColosseumLookImSorryButWe);
      Event_ShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
    } else {
      Event_SetMessageId(MsgColosseumWantTryOutLuckyGuess);
      Event_OpenMessage(ACTOR_LUCKY_GUESS_MAN, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
        gFieldWork->message++;
        Actor_ShowEmote(ACTOR_LUCKY_GUESS_MAN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        Event_OpenMessage(ACTOR_LUCKY_GUESS_MAN, 0);
        if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
          gFieldWork->message++;
        }
        Event_WaitTicks(40);
        Event_ShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
        GameFlag_Set(FLAG_LUCKY_GUESS_REFUSED_ISAAC);
      } else {
        Event_ShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
      }
    }
  } else {
    Event_SetMessageId(MsgColosseumDoKnowLuckyGuess);
    Event_AskYesNo(ACTOR_LUCKY_GUESS_MAN, 0);
  }
  Event_End();
}

/// Azart won't lose to someone who skipped the trials. He waits in the
/// Colosseum's waiting room as the second-best, and goes back to pacing after
/// he has said so.
static void ColosseumEntrance_TalkToAzart(void) {
  struct Actor *contestant;

  contestant = Actor_GetById(ACTOR_AZART);
  Event_Start();
  Actor_Stop(ACTOR_AZART);
  Actor_LinkPair(ACTOR_AZART, ACTOR_PARTY_LEADER, 20);
  Event_SetMessageId(MsgColosseumImRatedAsSecondBest);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_AZART);
  Actor_RunRepeatedMotion(ACTOR_AZART, 1);
  Event_ShowMessageAndWait(ACTOR_AZART, 0);
  // Give his wander back its home, where he is placed: MOVEMENT_TYPE_WANDER
  // reads timer and work as homeX and homeZ (Actor.homeX and homeZ), in
  // pixels.
  {
    u16 *timer = (u16 *)&contestant->timer;
    u16 duration = 720;

    *timer = duration;
  }
  {
    u16 *work = &contestant->userValue;
    u16 value = 112;

    *work = value;
  }
  Actor_SetMovementTypeById(ACTOR_AZART, MOVEMENT_TYPE_WANDER);
  Event_End();
}

/// In the Colosseum's waiting room, Galahad, the third seed, jumps.
/// "Waah! Don't frighten me!"
static void ColosseumEntrance_TalkToGalahad(void) {
  Event_Start();
  Actor_SetAttachedEffect(ACTOR_GALAHAD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_GALAHAD, 2);
  Event_SetMessageId(MsgColosseumWaahDontFrightenMe);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_GALAHAD);
  Actor_ShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_GALAHAD, 0);
  Event_End();
}

/// In the Colosseum's waiting room, Satrage, the fourth-ranked, opens his eyes
/// and names himself.
static void ColosseumEntrance_TalkToSatrage(void) {
  Event_Start();
  Event_SetMessageId(MsgColosseumWhoHaveComeQuestionMe);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
  Actor_LinkPair(ACTOR_SATRAGE, ACTOR_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_SATRAGE, 0);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
  ColosseumEntrance_FaceDirection(ACTOR_SATRAGE, FACING_SOUTH + FACING_STEP);
  Event_End();
}

/// In the Colosseum's waiting room, Dekka psyches himself up; the first time,
/// Navampa explains he will not answer (FLAG_NAVAMPA_SPOKE_FOR_DEKKA).
static void ColosseumEntrance_TalkToDekka(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_DEKKA, 2);
  Event_SetMessageId(MsgColosseumDekkaMustWinFinalsDekka);
  FieldEvent_ShowMessageThenWait(ACTOR_DEKKA, 0, 20);
  if (Event_GetFlag(FLAG_NAVAMPA_SPOKE_FOR_DEKKA) != 0) {
    Event_WaitTicks(20);
  } else {
    ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_EAST);
    Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 1);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
    Actor_LinkPair(ACTOR_NAVAMPA, ACTOR_PARTY_LEADER, 20);
    Import_ActorSetAnimation(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
    Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
    ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH + FACING_STEP);
    GameFlag_Set(FLAG_NAVAMPA_SPOKE_FOR_DEKKA);
  }
  Event_End();
}

/// In the Colosseum's waiting room, Navampa of Gondowan, the sixth-ranked,
/// introduces himself.
static void ColosseumEntrance_TalkToNavampa(void) {
  Event_Start();
  Actor_LinkPair(ACTOR_NAVAMPA, ACTOR_PARTY_LEADER, 20);
  Event_SetMessageId(MsgColosseumAmNavampaGondowanSixthRanked);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
  Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_NAVAMPA);
  ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH + FACING_STEP);
  Event_End();
}

/// In the Colosseum's waiting room, Buford, the seventh seed: anyone could win
/// today.
static void ColosseumEntrance_TalkToBuford(void) {
  Event_Start();
  Actor_LinkPair(ACTOR_BUFORD, ACTOR_PARTY_LEADER, 20);
  Event_SetMessageId(MsgColosseumImBufordSeventhSeed);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_BUFORD);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_WEST, 40);
  Actor_LinkPair(ACTOR_BUFORD, ACTOR_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_BUFORD);
  Actor_SetAnimationAndWait(ACTOR_BUFORD, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_BUFORD);
  ColosseumEntrance_FaceDirection(ACTOR_BUFORD, FACING_SOUTH + FACING_STEP);
  Event_End();
}

/// In the Colosseum's waiting room, the soldier asks whether Isaac is ready
/// for the finals.
static void ColosseumEntrance_AskIfReadyForFinals(void) {
  Event_Start();
  Actor_LinkPair(ACTOR_FINALS_SOLDIER, ACTOR_PARTY_LEADER, 20);
  Event_SetMessageId(MsgColosseumYouReadyForFinals);
  FieldMessage_ShowAtActor(ACTOR_FINALS_SOLDIER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
    gFieldWork->message++;
  Event_ShowMessageAndWait(ACTOR_FINALS_SOLDIER, 0);
  Event_End();
}

/// Stepping toward the gates at the Colosseum Entrance
/// (MB_COLOSSEUM_ENTRANCE_BEFORE_GATES) on the finals day: four soldiers
/// recognize Isaac as Babi's pick and quarrel over a boy in the finals, then
/// one takes the party in to the Colosseum's COLOSSEUM_FROM_COLOSSEUM_ENTRANCE.
/// If Isaac will not go, the soldiers insist and Ivan, Mia and Garet talk him
/// round; if he still refuses, Garet leads the way in. Before Babi is saved it
/// does nothing.
static void ColosseumEntrance_CrowdRecognizesIsaac(void) {
  // The native sequence reuses this word for actor handles and the
  // reply.
  s32 actorOrAnswer;

  if (GameFlag_Get(FLAG_BABI_SAVED) == 0) {
    return;
  }

  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
  Actor_FaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER, FACING_EAST, 60);
  Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Event_SetMessageId(MsgColosseumHeyYouRe);
  Actor_StartRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 2);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 1,
                    FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 2,
                    FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 3,
                    FACING_SOUTHEAST + FACING_STEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(172), 1);
  FieldCamera_WaitForArrival();
  Event_WaitTicks(20);
  Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 1, EMOTE_IN_FRONT | EMOTE_WINCE,
                  40);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 1);
  Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
  Actor_SetAttachedEffect(ACTOR_FIRST_GATE_SOLDIER + 2,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitTicks(40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER + 2, FACING_EAST);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER + 2);
  Actor_ShowEmote(ACTOR_FIRST_GATE_SOLDIER + 3,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3, FACING_EAST);
  Actor_StartRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 3, 2);
  Event_ShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER + 3, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
  Event_WaitTicks(10);
  Actor_FaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_NORTHWEST + FACING_STEP,
                      20);
  Event_ShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 1, 1);
  Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 1, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  // Gather Isaac and his companions for the invitation.
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_WEST);
  Event_ShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 2, 1);
  Event_WaitTicks(40);
  Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 2, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER + 3, 2);
  Event_WaitTicks(40);
  Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, ANIM_NOD);
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 1, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 2, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, ANIM_NOD);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTHEAST + FACING_STEP,
                    0);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 1,
                    FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 2,
                    FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_GATE_SOLDIER + 3,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_FIRST_GATE_SOLDIER, 258, 172);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 188);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  actorOrAnswer = (s32)Actor_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Import_ActorSetPosition(ACTOR_GARET,
                            ((struct Actor *)actorOrAnswer)->x.fixed,
                            ((struct Actor *)actorOrAnswer)->z.fixed);
  }
  actorOrAnswer = (s32)Actor_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Import_ActorSetPosition(ACTOR_IVAN,
                            ((struct Actor *)actorOrAnswer)->x.fixed,
                            ((struct Actor *)actorOrAnswer)->z.fixed);
  }
  actorOrAnswer = (s32)Actor_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Import_ActorSetPosition(ACTOR_MIA, ((struct Actor *)actorOrAnswer)->x.fixed,
                            ((struct Actor *)actorOrAnswer)->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_ResetAndSetPositionInMode2(ACTOR_GARET, 246, 200);
  Actor_WalkTo(ACTOR_IVAN, 262, 200);
  Actor_WalkToAndWait(ACTOR_MIA, 278, 200);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
  Event_OpenMessage(ACTOR_FIRST_GATE_SOLDIER, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);

  // Either leave with the guide or let the companions persuade Isaac.
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
    Actor_SetScript(ACTOR_GARET, gColosseumEntrance_GatherIntoIsaacScript);
    Actor_SetScript(ACTOR_IVAN, gColosseumEntrance_GatherIntoIsaacScript);
    Actor_RunScriptAndWait(ACTOR_MIA, gColosseumEntrance_GatherIntoIsaacScript);
    FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
    FieldCamera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(100), 1);
    Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
    Actor_SetScript(ACTOR_FIRST_GATE_SOLDIER,
                    gColosseumEntrance_EnterGatesScript);
    Event_WaitTicks(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumEntrance_EnterGatesScript);
    Event_WaitTicks(80);
    gFieldWork->transitionFrames = 40;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
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
    Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_SHAKE_HEAD);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_WaitTicks(20);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_FIRST_GATE_SOLDIER,
                        FACING_NORTHWEST + FACING_STEP, 0);
    Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 2, FACING_WEST, 0);
    Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 3, FACING_EAST, 60);
    Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTHEAST + FACING_STEP,
                      0);
    Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 2,
                      FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_ArmCallback(ACTOR_FIRST_GATE_SOLDIER + 3,
                      FACING_SOUTHEAST + FACING_STEP, 20);
    Import_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 1, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER + 2, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER + 3, ANIM_NOD);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    ColosseumEntrance_Turn(ACTOR_MIA, FACING_NORTHWEST);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_MIA);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_WaitTicks(20);
    Event_OpenMessage(ACTOR_GARET, 0);

    actorOrAnswer = Event_AskYesNoWithActorAnimation(0, 0);
    if (actorOrAnswer == 0) {
      Actor_SetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
      Actor_SetScript(ACTOR_GARET, gColosseumEntrance_GatherIntoIsaacScript);
      Actor_SetScript(ACTOR_IVAN, gColosseumEntrance_GatherIntoIsaacScript);
      Actor_RunScriptAndWait(ACTOR_MIA,
                             gColosseumEntrance_GatherIntoIsaacScript);
      FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
      FieldCamera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(100), 1);
      Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetScript(ACTOR_FIRST_GATE_SOLDIER,
                      gColosseumEntrance_EnterGatesScript);
      Event_WaitTicks(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumEntrance_EnterGatesScript);
      Event_WaitTicks(80);
      gFieldWork->transitionFrames = 40;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
    } else {
      Event_SkipMessages(1);
      Event_WaitTicks(20);
      Actor_StartRepeatedMotion(ACTOR_GARET, 2);
      Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
      FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
      ColosseumEntrance_FaceDirection(ACTOR_MIA, FACING_WEST);
      Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_MIA);
      Actor_SetScript(ACTOR_IVAN, gColosseumEntrance_GatherIntoGaretScript);
      Actor_RunScriptAndWait(ACTOR_MIA,
                             gColosseumEntrance_GatherIntoGaretScript);
      Event_WaitTicks(20);
      Actor_RunScriptAndWait(ACTOR_PARTY_LEADER,
                             gColosseumEntrance_GatherIntoGaretScript);
      Actor_WalkToAndWait(ACTOR_GARET, 262, 188);
      ColosseumEntrance_FaceDirection(ACTOR_GARET, FACING_NORTH);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_GARET);
      FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
      FieldCamera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(100), 1);
      Actor_SetSpeed(ACTOR_FIRST_GATE_SOLDIER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetScript(ACTOR_FIRST_GATE_SOLDIER,
                      gColosseumEntrance_EnterGatesScript);
      Event_WaitTicks(10);
      Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
      Actor_SetScript(ACTOR_GARET, gColosseumEntrance_EnterGatesScript);
      Event_WaitTicks(80);
      gFieldWork->transitionFrames = 40;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
    }
  }

  Import_EventSetExitRequest(COLOSSEUM_ENTRANCE_TO_COLOSSEUM);
  GameFlag_Set(FLAG_COLOSSEUM_FINALS_ENTERED);
  Event_End();
}

/// Stepping to the line in the Colosseum's waiting room
/// (MB_COLOSSEUM_FINALS_LINE, while FLAG_FINALS_LINE_ARMED is set and
/// FLAG_FINALS_LINE_REACHED clear): asked whether he is ready, Isaac can still
/// wait; if he is, the warriors grumble, line up by rank and march out, and
/// Isaac follows them out by the east passage
/// (COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST).
static void ColosseumEntrance_FinalsStart(void) {
  struct Actor *actor;
  const s32 *script;

  if (GameFlag_Get(FLAG_FINALS_LINE_ARMED) != 0) {
    GameFlag_Set(FLAG_FINALS_LINE_REACHED);
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 872, 120);
    ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
    FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    FieldCamera_MoveTo(PIXELS(816), -1, PIXELS(112), 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirection(ACTOR_FINALS_SOLDIER, FACING_SOUTHEAST + FACING_STEP,
                        20);
    Actor_RunRepeatedMotion(ACTOR_FINALS_SOLDIER, 1);
    Event_SetMessageId(MsgColosseumYouReadyForFinals);
    Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_FINALS_SOLDIER, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_WaitTicks(20);
      ColosseumEntrance_FaceDirection(ACTOR_MORGAN, FACING_NORTH + FACING_STEP);
      Actor_RunRepeatedMotion(ACTOR_MORGAN, 1);
      Event_SetMessageId(MsgColosseumDayTiredWaiting);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            ACTOR_MORGAN);
      ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_EAST);
      ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST);
      Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Import_ActorSetAnimation(ACTOR_SATRAGE, ANIM_STAND);
      Actor_ArmCallback(ACTOR_SATRAGE, FACING_NORTH + FACING_STEP, 20);
      Actor_RunRepeatedMotion(ACTOR_SATRAGE, 1);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_SATRAGE);
      ColosseumEntrance_FaceDirection(ACTOR_DEKKA, FACING_WEST);
      Actor_SetAnimationAndWait(ACTOR_DEKKA, ANIM_NOD);
      Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 2);
      ColosseumEntrance_FaceDirection(ACTOR_NAVAMPA, FACING_NORTHWEST);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            ACTOR_NAVAMPA);
      ColosseumEntrance_FaceDirection(ACTOR_BUFORD,
                                      FACING_NORTHWEST + FACING_STEP);
      Actor_StartRepeatedMotion(ACTOR_BUFORD, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE |
                                            ACTOR_BUFORD);
      ColosseumEntrance_FaceDirection(ACTOR_GONG_SOLDIER,
                                      FACING_SOUTHEAST + FACING_STEP);
      FieldEffect_SetAttached(ACTOR_GONG_SOLDIER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_WaitTicks(60);
      Actor_StartRepeatedMotion(ACTOR_GONG_SOLDIER, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_BELOW |
                                            ACTOR_GONG_SOLDIER);
      Actor_Stop(ACTOR_AZART);
      WaitTicks(1);
      Actor_StartRepeatedMotion(ACTOR_AZART, 2);
      Actor_StartRepeatedMotion(ACTOR_GALAHAD, 2);
      Actor_RunRepeatedMotion(ACTOR_DEKKA, 2);
      Event_WaitTicks(20);
      Actor_ArmCallback(ACTOR_AZART, FACING_EAST, 0);
      Actor_ArmCallback(ACTOR_GALAHAD, FACING_WEST, 0);
      Actor_ArmCallback(ACTOR_DEKKA, FACING_NORTHWEST + FACING_STEP, 40);
      Import_ActorSetAnimation(ACTOR_AZART, ANIM_NOD);
      Import_ActorSetAnimation(ACTOR_GALAHAD, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_DEKKA, ANIM_NOD);
      Actor_SetSpeed(ACTOR_AZART, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_SetSpeed(ACTOR_GALAHAD, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_SetSpeed(ACTOR_DEKKA, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_SetScript(ACTOR_AZART,
                      gColosseumEntrance_ApproachFinalsGroupScript);
      Actor_SetScript(ACTOR_DEKKA,
                      gColosseumEntrance_JoinFinalsFromSouthScript);
      Event_WaitTicks(20);
      Actor_ArmCallback(ACTOR_SATRAGE, FACING_NORTH + FACING_STEP, 0);
      Actor_ArmCallback(ACTOR_NAVAMPA, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
      Actor_ArmCallback(ACTOR_MORGAN, FACING_NORTH + FACING_STEP, 0);
      Actor_ArmCallback(ACTOR_BUFORD, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_RunScriptAndWait(ACTOR_GALAHAD,
                             gColosseumEntrance_JoinFinalsFromEastScript);
      Event_WaitTicks(20);
      Actor_ArmCallback(ACTOR_FINALS_SOLDIER, FACING_EAST, 0);
      Actor_ArmCallback(ACTOR_GONG_SOLDIER, FACING_WEST, 40);
      Import_ActorSetAnimation(ACTOR_FINALS_SOLDIER, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_GONG_SOLDIER, ANIM_NOD);
      FieldCamera_MoveWithinMapBounds(PIXELS(808), -1, PIXELS(86), 1);
      FieldCamera_WaitForArrival();
      Actor_RunRepeatedMotion(ACTOR_FINALS_SOLDIER, 2);
      ColosseumEntrance_ShowMessageAndPause(ACTOR_FINALS_SOLDIER);
      Actor_SetSpeedParameters(ACTOR_FINALS_SOLDIER, PIXELS(1), PIXELS(1) / 2);
      Actor_GetById(ACTOR_FINALS_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_SetPositionAndReset(ACTOR_FINALS_SOLDIER, 792, 72);
      Event_WaitTicks(1);
      Actor_GetById(ACTOR_FINALS_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
      ColosseumEntrance_FaceDirection(ACTOR_FINALS_SOLDIER, FACING_EAST);
      // Azart stops pacing before the procession.
      Actor_Stop(ACTOR_AZART);
      actor = Actor_GetById(ACTOR_AZART);
      actor->update = NULL;
      actor->timer = 0;
      actor->userValue = 0;
      SET_VELOCITY(actor, 0, 0, 0);
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      WaitTicks(1);
      // Send contestants through the gate at staggered intervals.
      script = gColosseumEntrance_EnterFinalsScript;
      Actor_SetScript(ACTOR_SATRAGE, script);
      Event_WaitTicks(20);
      Actor_SetScript(ACTOR_AZART, script);
      Event_WaitTicks(20);
      Actor_SetScript(ACTOR_NAVAMPA, script);
      Event_WaitTicks(20);
      Actor_SetScript(ACTOR_GALAHAD, script);
      Event_WaitTicks(20);
      Actor_SetScript(ACTOR_DEKKA, script);
      Event_WaitTicks(20);
      Actor_SetScript(ACTOR_MORGAN, script);
      Event_WaitTicks(20);
      Actor_SetScript(ACTOR_BUFORD, script);
      Event_WaitTicks(60);
      Actor_SetScript(ACTOR_PARTY_LEADER, script);
      Event_WaitTicks(80);
      Import_EventSetExitRequest(
          COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST);
    } else {
      gFieldWork->message++;
      Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_FINALS_SOLDIER, 0);
    }
    Event_End();
  }
}

/// Stepping back from the line in the Colosseum's waiting room
/// (MB_COLOSSEUM_BEHIND_FINALS_LINE) arms the question again
/// (FLAG_FINALS_LINE_ARMED and FLAG_FINALS_LINE_REACHED).
static void ColosseumEntrance_StepBackFromFinalsLine(void) {
  GameFlag_Clear(FLAG_FINALS_LINE_REACHED);
  GameFlag_Set(FLAG_FINALS_LINE_ARMED);
}

/// The trial attendant in the Colosseum's stands: until both trials have been
/// watched, the screen goes to the next warrior's trial (the failed one first,
/// by FLAG_COLOSSO_FAILED_TRIAL_SEEN); afterwards he talks about the
/// techniques.
static void ColosseumEntrance_TalkToTrialAttendant(void) {
  s32 clearedTrialSeen;

  Event_Start();
  clearedTrialSeen = GameFlag_Get(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  if (clearedTrialSeen != 0) {
    Actor_LinkPair(ACTOR_TRIAL_ATTENDANT, ACTOR_PARTY_LEADER, 40);
    Event_SetMessageId(MsgColosseumHavePerfectTechniquesLikeThose);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_TRIAL_ATTENDANT);
    Actor_FaceDirection(ACTOR_TRIAL_ATTENDANT, FACING_SOUTHEAST + FACING_STEP,
                        20);
  } else {
    Actor_StartRepeatedMotion(ACTOR_TRIAL_ATTENDANT, 2);
    Event_SetMessageId(MsgColosseumSilence);
    Event_ShowMessage(ACTOR_TRIAL_ATTENDANT, 0);
    FieldCamera_GetActor()->motionFlags = clearedTrialSeen;
    WaitTicks(1);
    FieldCamera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
    FieldCamera_MoveTo(PIXELS(540), -1, PIXELS(208), 1);
    FieldCamera_WaitForArrival();
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gFieldWork->transitionFrames = 32;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    if (GameFlag_Get(FLAG_COLOSSO_FAILED_TRIAL_SEEN) != 0) {
      Import_EventSetExitRequest(COLOSSEUM_STANDS_TO_CLEARED_TRIAL);
    } else {
      Import_EventSetExitRequest(COLOSSEUM_STANDS_TO_FAILED_TRIAL);
    }
  }
  Event_End();
}

/// The Colosseum's trial grounds, arriving by
/// COLOSSEUM_FAILED_TRIAL_FROM_STANDS: a warrior strains at the trial, gives up
/// and runs, and the screen goes back to the stands.
static void ColosseumEntrance_ShowFailedTrial(void) {
  struct Actor *actor;
  s32 facing;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySong(SE_COLOSSO_LETTERING);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 1, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 2, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 3, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 4, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 5, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, 0);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 0);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 4, 0);
  actor = Actor_GetById(ACTOR_TRIAL_PILLAR);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_FIRST_FLAG_BEARER);
  actor->scaleX = -Q16_ONE;
  actor = Actor_GetById(ACTOR_FIRST_FLAG_BEARER + 1);
  actor->scaleX = -Q16_ONE;
  WaitTicks(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  WaitTicks(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_FAILING_WARRIOR, 1);
  Event_WaitTicks(20);
  Actor_SetSpeed(ACTOR_FAILING_WARRIOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 164, 904);
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 9);
  Event_WaitTicks(40);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 10);
  Event_WaitTicks(60);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, ANIM_STAND);
  Event_WaitTicks(20);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 164, 920);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 185, 920);
  Actor_FaceDirection(ACTOR_FAILING_WARRIOR, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 185, 916);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 11);
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_FAILING_WARRIOR, 1);
  Event_WaitTicks(60);
  Actor_RunRepeatedMotion(ACTOR_FAILING_WARRIOR, 3);
  Event_WaitTicks(40);
  Actor_SetScript(ACTOR_FAILING_WARRIOR, gColosseumEntrance_PulseScaleScript);
  Event_WaitTicks(80);
  Actor_SetAttachedEffect(ACTOR_FAILING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitTicks(60);
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
  Event_WaitTicks(60);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 1,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      0);
  facing = FACING_SOUTHEAST + FACING_STEP;
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, facing);
  Actor_Stop(ACTOR_FAILING_WARRIOR);
  actor = Actor_GetById(ACTOR_FAILING_WARRIOR);
  actor->facing = FACING_NORTH + FACING_STEP;
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 0);
  Event_WaitTicks(40);
  Import_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER,
                           FLAG_BEARER_ANIMATION_RED_FLAG);
  Import_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1,
                           FLAG_BEARER_ANIMATION_RED_FLAG);
  Event_WaitTicks(60);
  Actor_FaceDirection(ACTOR_FAILING_WARRIOR, facing, 20);
  Import_ActorSetAnimation(ACTOR_FAILING_WARRIOR, 8);
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_FIRST_TRIAL_WARRIOR + 4, ANIM_SHAKE_HEAD);
  Event_WaitTicks(40);
  Actor_SetAnimationAndWait(ACTOR_FAILING_WARRIOR, ANIM_SHAKE_HEAD);
  Event_WaitTicks(10);
  Actor_SetSpeed(ACTOR_FAILING_WARRIOR, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 162, 916);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 162, 890);
  Import_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1, ANIM_STAND);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 184, 863);
  Actor_WalkToAndWait(ACTOR_FAILING_WARRIOR, 184, 796);
  Actor_SetPosition(ACTOR_FAILING_WARRIOR, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Import_EventSetExitRequest(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
  Event_End();
}

/// Back in the Colosseum's stands from a trial
/// (COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS): the attendant says the warrior
/// failed, or after the second trial that he cleared it.
static void ColosseumEntrance_ExplainTrial(void) {
  Event_Start();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);

  Event_StartScreenTransition();
  Event_WaitTransitionFrames();

  Event_WaitTicks(20);
  ColosseumEntrance_FaceDirection(ACTOR_TRIAL_ATTENDANT,
                                  FACING_SOUTH + FACING_STEP);
  Event_SetMessageId(MsgColosseumHaveMakeThroughCountlessMatches);

  if (GameFlag_Get(FLAG_COLOSSO_CLEARED_TRIAL_SEEN) != 0) {
    gFieldWork->message = (gFieldWork->message + 1);
  }

  ColosseumEntrance_ShowMessageAndPause(ACTOR_TRIAL_ATTENDANT);
  ColosseumEntrance_FaceDirection(ACTOR_TRIAL_ATTENDANT,
                                  FACING_SOUTHEAST + FACING_STEP);
  GameFlag_Set(FLAG_COLOSSO_FAILED_TRIAL_SEEN);

  Event_End();
}

/// The Colosseum's trial grounds, arriving by
/// COLOSSEUM_CLEARED_TRIAL_FROM_STANDS: a warrior heaves the trial object up in
/// jolts and drops it, shaking the ground: he clears the trial
/// (FLAG_COLOSSO_CLEARED_TRIAL_SEEN).
static void ColosseumEntrance_ShowClearedTrial(void) {
  u32 counter;
  struct Actor *trialEffect;
  struct Actor *actor;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySong(SE_COLOSSO_LETTERING);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 1, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 2, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 3, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 4, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 5, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, 0);
  Actor_SetPosition(ACTOR_FAILING_WARRIOR, 0, 0);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 4, 0);
  actor = Actor_GetById(ACTOR_TRIAL_PILLAR);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_FIRST_FLAG_BEARER);
  actor->scaleX = -Q16_ONE;
  actor = Actor_GetById(ACTOR_FIRST_FLAG_BEARER + 1);
  actor->scaleX = -Q16_ONE;
  WaitTicks(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  WaitTicks(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 1);
  Event_WaitTicks(20);
  Actor_SetSpeed(ACTOR_CLEARING_WARRIOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 164, 904);
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 9);
  Event_WaitTicks(40);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 10);
  Event_WaitTicks(60);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STAND);
  Event_WaitTicks(20);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 164, 920);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 185, 920);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 185, 916);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 11);
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 1);
  Event_WaitTicks(60);
  Actor_RunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 3);
  Event_WaitTicks(40);
  Actor_SetScript(ACTOR_CLEARING_WARRIOR, gColosseumEntrance_PulseScaleScript);
  Event_WaitTicks(80);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitTicks(60);
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
  Event_WaitTicks(60);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 1,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // The object rises a little with every heave.
  trialEffect = Actor_GetById(ACTOR_TRIAL_PILLAR);
  trialEffect->motionFlags = 0;
  for (counter = 0; counter < 20; counter++) {
    trialEffect->y.fixed += PIXELS(3) / 5;
    WaitTicks(4);
    trialEffect->y.fixed += -PIXELS(3) / 10;
    WaitTicks(4);
  }
  Import_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER,
                           FLAG_BEARER_ANIMATION_WHITE_FLAG);
  Import_ActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1,
                           FLAG_BEARER_ANIMATION_WHITE_FLAG);
  Event_WaitTicks(60);
  Actor_SetAttachedEffect(ACTOR_CLEARING_WARRIOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_Stop(ACTOR_CLEARING_WARRIOR);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STAND);
  actor = Actor_GetById(ACTOR_CLEARING_WARRIOR);
  actor->facing = FACING_NORTH + FACING_STEP;
  trialEffect->motionFlags = 3;
  trialEffect->scaleX = Q16_ONE;
  trialEffect->scaleY = Q16_ONE;
  Event_WaitTicks(10);
  Event_PlaySong(SE_TREMOR);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_WaitTicks(10);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Actor_SetSpeed(ACTOR_CLEARING_WARRIOR, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_CLEARING_WARRIOR, 208, 928);
  Event_PlaySong(MUS_FANFARE_MEDIUM);
  Actor_FaceDirection(ACTOR_CLEARING_WARRIOR, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 9);
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 10);
  Event_WaitTicks(40);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 9);
  Event_WaitTicks(20);
  Import_ActorSetAnimation(ACTOR_CLEARING_WARRIOR, 10);
  Event_WaitTicks(80);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  Import_EventSetExitRequest(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
  Event_End();
}

/// Sets up the Colosseum or its entrance as the party arrives. The Colosseum is
/// set up by the entrance the party arrives at, and the Colosseum
/// Entrance by how far Colosso has gone.
s32 ColosseumEntrance_Initialize(void) {
  s32 sceneId = gPersistentState.sceneId;

  if (sceneId == (s32)&SceneId_Colosseum) {
    ColosseumEntrance_ApplyArrivalState();
  } else if (sceneId == (s32)&SceneId_ColosseumEntrance) {
    ColosseumEntrance_UpdateEntranceCells();
  }
  return 0;
}

/// Each arrival in the Colosseum has its own event: the trials (the stands and
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

  WaitTicks(1);
  entrance = gPersistentState.entrance;
  switch (entrance) {
  case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
    Import_ActorSetAnimation(ACTOR_STANDS_WEST_TORCH, ANIM_WALK);
    Import_ActorSetAnimation(ACTOR_STANDS_EAST_TORCH, ANIM_WALK);
    break;
  case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
    Import_ActorSetAnimation(ACTOR_STANDS_WEST_TORCH, ANIM_WALK);
    Import_ActorSetAnimation(ACTOR_STANDS_EAST_TORCH, ANIM_WALK);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0)
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
    InventorySnapshot_Save();
    break;
  case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
    ColosseumEntrance_EscortToEastExit();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
    ColosseumEntrance_WalkToEastExit();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    ColosseumEntrance_InitializeContestants();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0)
      break;
    ColosseumEntrance_AnnounceFinals();
    break;
  case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
    Party_AddMember(GARET);
    Party_AddMember(IVAN);
    Party_AddMember(MIA);
    GameFlag_Set(FLAG_COLOSSO_FIRST_MATCH_WON);
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
    GameFlag_Set(FLAG_COLOSSO_SECOND_MATCH_WON);
    ColosseumEntrance_CelebrateBothWins();
    break;
  default:
    break;
  }
}

static void ColosseumEntrance_ShowMessageAndPause(s32 speaker) {
  Event_ShowMessageAndWait(speaker, 0);
  Event_WaitTicks(10);
}

/// Turns an actor to the given facing over ten frames.
static void ColosseumEntrance_FaceDirection(s32 actorId, s32 facing) {
  Actor_ArmCallback(actorId, (u16)facing, 10);
}

/// The Colosseum Entrance's setup: once Colosso is over, the tiles and cells by
/// the gates change; before that, the Lucky Guess man (ACTOR_LUCKY_GUESS_MAN)
/// takes child value 2, and on the finals day a cell pair before the gates
/// changes.
static void ColosseumEntrance_UpdateEntranceCells(void) {
  s32 column, row;

  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    MapGrid_CopyCellsTo(64, 0, 48, 5, 2, 2);
    column = 16;
    row = 8;
    MapGrid_CopyCellAttributes(14, 8, 2, 1, column, row);
  } else {
    Actor_ConfigureSpriteColor(ACTOR_LUCKY_GUESS_MAN, SPRITE_COLOR_RBG);
    if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      column = 14;
      row = 11;
      MapGrid_CopyCellAttributes(30, 22, 1, 2, column, row);
    }
  }
}

/// Places the three companions behind Isaac for the soldiers' briefing.
static inline void ColosseumEntrance_PositionPartyForBriefing(void) {
  Actor_SetPosition(ACTOR_GARET, PIXELS(792), PIXELS(136));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(824), PIXELS(136));
  Actor_SetPosition(ACTOR_MIA, PIXELS(808), PIXELS(152));
}

/// The Colosseum's briefing, the party's arrival from the gates
/// (COLOSSEUM_FROM_COLOSSEUM_ENTRANCE): only Isaac may enter the finals, so the
/// soldiers let his companions cheer him on from the stage, and at Ivan's
/// asking they agree to show Isaac the finals stages before the first match. An
/// soldier then leads the party on
/// (COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST).
static void ColosseumEntrance_ExplainPartySupport(void) {
  s32 flag;

  Event_Start();
  ColosseumEntrance_PositionPartyForBriefing();
  WaitTicks(1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  PaletteFade_SetSource(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  WaitTicks(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  PaletteFade_SetSource(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  Event_WaitTicks(80);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  Event_WaitTicks(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitTicks(40);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_WaitTicks(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  Event_WaitTicks(80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitTicks(20);
  // The soldiers explain Isaac's selection and the companions' role.
  Event_SetMessageId(MsgColosseumIsaacOnlyOneEnteringFinals);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER + 1, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTH + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER + 1, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_GARET);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER + 2, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Actor_SetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  ColosseumEntrance_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ColosseumEntrance_FaceDirection(ACTOR_GARET, FACING_EAST);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_GARET);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  ColosseumEntrance_FaceDirection(ACTOR_IVAN, FACING_WEST);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_MIA);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  ColosseumEntrance_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_GARET);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  ColosseumEntrance_FaceDirection(ACTOR_IVAN, FACING_NORTH);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  ColosseumEntrance_FaceDirection(ACTOR_MIA, FACING_NORTH);
  Actor_SetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  ColosseumEntrance_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_ESCORTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  flag = 0;
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
    flag = 1;
  } else {
    gFieldWork->message++;
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    ColosseumEntrance_FaceDirection(ACTOR_IVAN, FACING_NORTH);
    ColosseumEntrance_ShowMessageAndPause(ACTOR_IVAN);
  }
  if (flag != 0) {
    gFieldWork->message++;
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
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER,
                                  FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER + 1,
                                  FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_FaceDirection(ACTOR_FIRST_SOLDIER + 2,
                                  FACING_SOUTH + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  ColosseumEntrance_FaceDirection(ACTOR_ESCORTING_SOLDIER, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  // Rejoin the companions before the soldier leads Isaac onward.
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetScript(ACTOR_GARET, gColosseumEntrance_RejoinPartyScript);
  Actor_SetScript(ACTOR_IVAN, gColosseumEntrance_RejoinPartyScript);
  Actor_RunScriptAndWait(ACTOR_MIA, gColosseumEntrance_RejoinPartyScript);
  Event_WaitTicks(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
  Event_WaitTicks(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Event_WaitTicks(60);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 40;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/// The Colosseum's east passage
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST): a soldier leads Isaac
/// to the next finals match's tour, Colosseum Final 1 or, after the second win,
/// Colosseum Final 3.
static void ColosseumEntrance_EscortToEastExit(void) {
  struct Actor *escort;

  if (GameFlag_Get(JENNA) != 0) {
    GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
    Party_RemoveMember(JENNA);
    Party_AddMember(MIA);
  }
  Event_Start();
  Actor_SetPosition(ACTOR_ESCORTING_SOLDIER, PIXELS(712), PIXELS(588));
  WaitTicks(1);
  FieldCamera_FollowActor(ACTOR_ESCORTING_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = Actor_GetById(ACTOR_ESCORTING_SOLDIER);
  escort->facing = FACING_EAST;
  Event_StartScreenTransition();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 780, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 812, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 880, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 912, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 980, 588);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 1012, 588);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  if (GameFlag_Get(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Import_EventSetExitRequest(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH);
  } else {
    Import_EventSetExitRequest(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH);
  }
}

/// Arriving in the Colosseum's waiting room from Colosseum Final 1's tour: the
/// soldier tells the warriors to prepare.
static void ColosseumEntrance_AnnounceFinals(void) {
  struct Actor *contestant;

  contestant = Actor_GetById(ACTOR_AZART);
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_FINALS_SOLDIER, 2);
  Actor_Stop(ACTOR_AZART);
  WaitTicks(1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Import_ActorSetAnimation(ACTOR_AZART, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MORGAN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_AZART, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GALAHAD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SATRAGE, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DEKKA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_NORTHWEST + FACING_STEP, 0);
  Event_SetMessageId(MsgColosseumPrepareYourselvesContestantsFinalsWill);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FINALS_SOLDIER);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // Azart's wander home, as in ColosseumEntrance_TalkToAzart.
  {
    u16 *timer = (u16 *)&contestant->timer;
    u16 duration = 720;

    *timer = duration;
  }
  {
    u16 *work = &contestant->userValue;
    u16 value = 112;

    *work = value;
  }
  Actor_SetMovementTypeById(ACTOR_AZART, MOVEMENT_TYPE_WANDER);
  Actor_FaceDirection(ACTOR_MORGAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GALAHAD, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATRAGE, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DEKKA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_BUFORD, FACING_SOUTH + FACING_STEP, 0);
  Event_End();
}

/// The Colosseum's east passage (COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH): Isaac
/// walks alone into Colosseum Final 1 or, after the second win, Colosseum Final
/// 3, where his friends take their turns to lend a hand
/// (COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST or
/// COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH).
static void ColosseumEntrance_WalkToEastExit(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Event_StartScreenTransition();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 780, 428);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 880, 428);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 980, 428);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  if (GameFlag_Get(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Import_EventSetExitRequest(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH);
  } else {
    Import_EventSetExitRequest(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST);
  }
}

/// Back in the Colosseum after the first win
/// (COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST): the soldiers congratulate
/// Isaac and lead him on to Colosseum Final 2.
static void ColosseumEntrance_CelebrateAdvancement(void) {
  const s32 *actions;

  Event_Start();
  ColosseumEntrance_PositionPartyForBriefing();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_SetMessageId(MsgColosseumSeeYouveMadeThroughYour);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Actor_RunRepeatedMotion(ACTOR_ESCORTING_SOLDIER, 1);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  Actor_SetScript(ACTOR_GARET, actions);
  Actor_SetScript(ACTOR_IVAN, actions);
  Actor_RunScriptAndWait(ACTOR_MIA, actions);
  Event_WaitTicks(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
  Event_WaitTicks(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Event_WaitTicks(60);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST);
}

/// The Colosseum's west passage
/// (COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST): a soldier leads Isaac
/// to Colosseum Final 2's tour.
static void ColosseumEntrance_EscortToWestExit(void) {
  struct Actor *escort;

  if (GameFlag_Get(JENNA) != 0) {
    GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
    Party_RemoveMember(JENNA);
    Party_AddMember(MIA);
  }
  Event_Start();
  Actor_SetPosition(ACTOR_ESCORTING_SOLDIER, PIXELS(868), PIXELS(588));
  WaitTicks(1);
  FieldCamera_FollowActor(ACTOR_ESCORTING_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = Actor_GetById(ACTOR_ESCORTING_SOLDIER);
  escort->facing = FACING_WEST;
  Event_StartScreenTransition();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 800, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 768, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 700, 588);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 668, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 600, 588);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 568, 588);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST);
}

/// The Colosseum's west passage (COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH):
/// Isaac walks alone into Colosseum Final 2, where his friends take their turns
/// to lend a hand (COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST).
static void ColosseumEntrance_WalkToWestExit(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Event_StartScreenTransition();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 800, 428);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 700, 428);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 600, 428);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST);
}

/// Back in the Colosseum after the second win
/// (COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH): one more win and Isaac is
/// the champion; the soldiers lead him back east
/// (COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST), now to Colosseum
/// Final 3.
static void ColosseumEntrance_CelebrateBothWins(void) {
  const s32 *actions;

  Event_Start();
  ColosseumEntrance_PositionPartyForBriefing();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitTicks(40);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_SetMessageId(MsgColosseumWonBothMatches);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 1, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 1);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER + 2, 1);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_FIRST_SOLDIER + 2);
  Actor_RunRepeatedMotion(ACTOR_ESCORTING_SOLDIER, 1);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  ColosseumEntrance_ShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  Actor_SetScript(ACTOR_GARET, actions);
  Actor_SetScript(ACTOR_IVAN, actions);
  Actor_RunScriptAndWait(ACTOR_MIA, actions);
  Event_WaitTicks(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ESCORTING_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
  Actor_MoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
  Actor_SetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
  Event_WaitTicks(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Event_WaitTicks(60);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/// The warriors' child values and their waiting animations in the Colosseum's
/// waiting room.
static void ColosseumEntrance_InitializeContestants(void) {
  Actor_ConfigureSpriteColor(ACTOR_MORGAN, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_AZART, SPRITE_COLOR_NORMAL);
  Actor_ConfigureSpriteColor(ACTOR_GALAHAD, SPRITE_COLOR_GBR);
  Actor_ConfigureSpriteColor(ACTOR_SATRAGE, SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_DEKKA, SPRITE_COLOR_BRG);
  Actor_ConfigureSpriteColor(ACTOR_NAVAMPA, SPRITE_COLOR_RBG);
  Actor_ConfigureSpriteColor(ACTOR_BUFORD, SPRITE_COLOR_GREY);
  Actor_SetAnimationSpeedById(ACTOR_AZART, 10);
  Actor_SetAnimationSpeedById(ACTOR_GALAHAD, 20);
  Import_ActorSetAnimation(ACTOR_SATRAGE, 0);
  Actor_SetAnimationSpeedById(ACTOR_DEKKA, 40);
  Actor_SetAnimationSpeedById(ACTOR_NAVAMPA, 50);
  Actor_SetAnimationSpeedById(ACTOR_BUFORD, 60);
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
struct ActorTemplate gColosseumEntranceActorTemplatesOther[] = {
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumStandsActorTemplates[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(592), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
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
        PIXELS(444), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(428), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(240), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(192), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(608), 0, PIXELS(104), FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumTrialGroundsActorTemplates[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(250), 0, PIXELS(846), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(298), 0, PIXELS(912), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(250), 0, PIXELS(974), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(121), 0, PIXELS(974), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(912), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(121), 0, PIXELS(846), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
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
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumActorTemplates[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(72), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(72), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumSoldiersActorTemplates[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(782), 0, PIXELS(96), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(832), 0, PIXELS(96), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumActorTemplatesFromColosseumAzartMatchWest[] = {
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
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumActorTemplatesFromColosseumNavampaMatch[] = {
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
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumEntranceActorTemplates[] = {
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
        PIXELS(264), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(80), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(56), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(140), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(64), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(72), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumEntranceActorTemplatesFinalsDay[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
        PIXELS(278), 0, PIXELS(70), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(88), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(106), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(124), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(142), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(160), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gColosseumEntranceActorTemplatesAfterColosso[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(286), 0, PIXELS(164), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gColosseumEntranceTriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gColosseumTunnelTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_BABI_PALACE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_COLOSSEUM },
    { TRIGGERS_END },
};

struct Trigger gColosseumTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, COLOSSEUM_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH },
    { TRIGGER_TALK, ACTOR_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceGoForItThatsItIm) },
    { TRIGGER_TALK, ACTOR_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingTheTrialsMakesMeReally) },
    { TRIGGER_TALK, ACTOR_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn) },
    { TRIGGER_TALK, ACTOR_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIveWaitedAWholeYearTo) },
    { TRIGGER_TALK, ACTOR_TRIAL_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumEntrance_TalkToTrialAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceEvenTheSmallerWarriorsCanWin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheseTrialsAreBasedOnPersonal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatFantasticPhysiquesWhyIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColossoDidntThinkBattles) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRIAL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceNoMatterHowGoodAWarrior) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianItIsLocked)) },
    { TRIGGERS_END },
};

struct Trigger gColosseumEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_TOLBI },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_COLOSSEUM },
    { TRIGGER_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats) },
    { TRIGGER_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheFinalsAreFinallyHereI) },
    { TRIGGER_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceColossoIsOverAndTheColosseum) },
    { TRIGGER_TALK, ACTOR_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceSighTheColosseumIsPackedWith) },
    { TRIGGER_TALK, ACTOR_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceIHeardBabiHasPickedSome) },
    { TRIGGER_TALK, ACTOR_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheColosseumIsClosedUntilNext) },
    { TRIGGER_TALK, ACTOR_VILLAGER_SEATING, CONDITION_ALWAYS, EVENT(ColosseumEntrance_DiscussSeating) },
    { TRIGGER_TALK, ACTOR_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIfYouHitItBigIn) },
    { TRIGGER_TALK, ACTOR_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAWarriorNamedIsaacWasSelected) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIveBeenBettingOnTheLucky) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceHavingAWarriorWhoWasntIn) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, EVENT(ColosseumEntrance_DiscussLuckyGuessSeats) },
    { TRIGGER_TALK, ACTOR_SCALPER, CONDITION_ALWAYS, EVENT(ColosseumEntrance_DiscussLuckyGuessWinnings) },
    { TRIGGER_TALK, ACTOR_LUCKY_GUESS_MAN, CONDITION_ALWAYS, EVENT(ColosseumEntrance_OfferLuckyGuess) },
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
    { TRIGGER_STEP, MB_COLOSSEUM_ENTRANCE_BEFORE_GATES, FLAG_COLOSSO_OVER, EVENT(ColosseumEntrance_CrowdRecognizesIsaac) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gColosseumTriggersFromColosseumAzartMatchWest[] = {
    {TRIGGER_TALK, ACTOR_MORGAN, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIAmMorganIAmThe)},
    {TRIGGER_TALK, ACTOR_AZART, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_TalkToAzart)},
    {TRIGGER_TALK, ACTOR_GALAHAD, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_TalkToGalahad)},
    {TRIGGER_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_TalkToSatrage)},
    {TRIGGER_TALK, ACTOR_DEKKA, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_TalkToDekka)},
    {TRIGGER_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_TalkToNavampa)},
    {TRIGGER_TALK, ACTOR_BUFORD, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_TalkToBuford)},
    {TRIGGER_TALK, ACTOR_FINALS_SOLDIER, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_AskIfReadyForFinals)},
    {TRIGGER_TALK, ACTOR_EXIT_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceYouCannotLeaveUntilTheFinals)},
    {TRIGGER_TALK, ACTOR_WAITING_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIFeelWeveBeenWaitingForever)},
    {TRIGGER_TALK, ACTOR_GONG_SOLDIER, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceWhenTheGongSoundsTheFinals)},
    {TRIGGER_STEP, MB_COLOSSEUM_FINALS_LINE, FLAG_FINALS_LINE_REACHED,
     EVENT(ColosseumEntrance_FinalsStart)},
    {TRIGGER_STEP, MB_COLOSSEUM_BEHIND_FINALS_LINE, CONDITION_ALWAYS,
     EVENT(ColosseumEntrance_StepBackFromFinalsLine)},
    {TRIGGERS_END},
};
