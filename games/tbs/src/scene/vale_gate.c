/// @file
/// Vale (SceneId_ValeGate): the part of the village with the Psynergy Stone and
/// the town gate, Vale's way out to the world. The scene sees the prologue end
/// and the journey begin, and ValeGate_GetPlacements and ValeGate_GetEvents
/// pick its people and events by the time of the story. In story order:
///
/// - The stormy night of the prologue (FLAG_VALE_STORM), when the villagers
///   have gathered in the plaza here. Isaac and Garet come from the river
///   (Vale, SceneId_ValeStorm, by VALE_STORM_TO_VALE_GATE), where Felix has
///   fallen in. Walking up to the villagers (cell code 20) plays
///   Event_SentToAidElders: Garet's grandfather, the mayor, sends two strongmen
///   to aid the elders holding the Boulder, then sends Isaac and Garet back to
///   show Jenna and a strongman whose Psynergy the Psynergy Stone has restored
///   the way to the river (FLAG_VALE_STORM_SENT_TO_AID_ELDERS). Others have
///   spent their Psynergy, and one lifts a small rock to show he has a little
///   left (Event_DemonstrateLevitation). Later, when the Boulder has swept
///   Kyle, Jenna's parents and Felix away and Garet has gone after Isaac
///   (FLAG_VALE_STORM_GARET_LEFT), arriving here plays
///   Event_MeetSurvivingTravelers: Isaac overhears two strangers saying only
///   the two of them survived Sol Sanctum's fury, Garet catches up, and the
///   strangers fight the boys to make them forget (ValeGate_FightTravelers).
///   Win or lose, the battle returns to VALE_GATE_AFTER_TRAVELERS_BATTLE, where
///   the strangers leave the boys lying in the storm, the storm ends
///   (FLAG_VALE_STORM is cleared) and VALE_GATE_TO_TITLE leads to the title
///   shown after the prologue, which opens on Vale three years later
///   (menu/title.c). The game names the two only three years later, as Saturos
///   and Menardi (vale_kraden.c); their sprites here are the same.
/// - Three years later, the day the story begins. The villagers talk of
///   meditating on Mt. Aleph, of the travelers who stopped by, and of the
///   storm, in which Isaac lost his father and Jenna her whole family; one
///   shows off his Psynergy, shaking the ground (Event_DemonstratePsynergy).
/// - The farewell (VALE_GATE_FROM_SANCTUM), once Isaac and Garet have
///   accepted the Wise One's quest in the Vale Sanctum (sanctum.c).
///   Event_DepartForQuest restores the party's HP and PP and plays the village
///   seeing them off at the town gate: Garet's sister gives a Herb, and his
///   brother the Catch Beads that Aunty Dora sent, for Dora herself does not
///   come. VALE_GATE_TO_WORLD_MAP leads out to the world map.
/// - After the party has left (FLAG_PARTY_LEFT_VALE), the villagers point the
///   way to Vault and talk of the eruption, whose sparkles they take for
///   crushed Psynergy Stones; a large Psynergy Stone has fallen on the hut,
///   whose roof ValeGate_Initialize redraws. Only now does the town gate lead
///   out (Event_LeaveThroughTownGate).
/// - Back from Kalay (FLAG_REACHED_KALAY), the villagers welcome Isaac home,
///   the hut's owner finds the stone gone, leaving a hole in his roof
///   (Event_PsynergyStoneIsGone), and a door in the north-east opens.
///
/// The Psynergy Stone (search 100) restores the party's PP at every time but
/// the farewell. The ivy in three places (ACTOR_FIRST_IVY to ACTOR_THIRD_IVY)
/// can be blown away with Whirlwind, each one's flag keeping it gone. The doors
/// lead into the houses (SceneId_ValeInteriorsShops) and into another part of
/// Vale (SceneId_ValeInteriorsInn), the path west to the village (SceneId_Vale)
/// or, on the stormy night, back to the storm's map (SceneId_ValeStorm), and
/// the exit row of cell code 11 to VALE_GATE_TO_GOMA_CAVE_STREAM, which the
/// exit table sends to Goma Cave (SceneId_GomaCaveStream,
/// GOMA_CAVE_STREAM_FROM_VALE_GATE).
///
/// Three debug entrances are in no entrance table and no exit leads to them:
/// VALE_GATE_DEBUG_BEFORE_TRAVELERS_LEAVE replays the strangers' departure,
/// VALE_GATE_DEBUG_CHILDHOOD sets FLAG_CHILDHOOD and takes
/// VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST into Vale
/// (SceneId_ValeInteriorsInn), and VALE_GATE_DEBUG_STORY_FLAGS sets a preset
/// of story flags and takes VALE_GATE_TO_BILIBIN
/// (ValeGate_SetProgressFlagsAndExit).

#include "game_state.h"
#include "flags.h"
#include "character_ids.h"
#include "version.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/scene/vale_gate.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "global_cells.h"
#include "vale_gate.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_service.h"
#include "types.h"
#include "animspr.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "facing_object.h"
#include "field_obj.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "songs.h"
#include "item_ids.h"

// Vale's place flags, kept while the party stays in Vale's scenes.
/// Back from Kalay, a young man has welcomed Isaac home.
/// ★ Named from the game: "You came back home, huh?" (MsgValeYouCameBackHome)
#define FLAG_VALE_WELCOMED_HOME 0x302
/// Back from Kalay, the old man searching for the Psynergy Stone has told
/// Isaac it is gone.
/// ★ Named from the game: "Where'd that Psynergy Stone go? It's gone!"
/// (MsgValeThePsynergyStoneIsGone)
#define FLAG_VALE_PSYNERGY_STONE_GONE_TOLD 0x306

/// Each placement table numbers its own rows from ACTOR_FIRST_PLACED, Garet
/// keeping his own party slot, and the same people stand in several tables
/// under other numbers. The three ivy actors come first in every table.
enum ValeGateActor {
  /// Ivy, drawn half as tall again: the sprite of the ivy that holds the stump
  /// in goma_cave_entrance.c ("It looks like it's trapped in the ivy.").
  /// ★ Named from what the player sees: a clump of green ivy against the cliff
  /// (its render)
  ACTOR_FIRST_IVY = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a clump of green ivy against the cliff
  /// (its render)
  ACTOR_SECOND_IVY,
  /// ★ Named from what the player sees: a clump of green ivy against the cliff
  /// (its render)
  ACTOR_THIRD_IVY
};

/// By day (gValeGate_Placements, before and after the party leaves). The
/// notes quote the day the story begins.
enum ValeGateDayActor {
  /// An old man with a cane: "The Valeans are a chosen people." On the stormy
  /// night, after the party
  /// has left and back from Kalay, a question for Isaac instead
  /// (Event_AskAboutJourney); after the party has left, "You saw the Wise
  /// One?"
  /// ★ Named from the game: "The Valeans are a chosen people."
  /// (MsgValeChosenPeople)
  ACTOR_CURIOUS_OLD_MAN = ACTOR_FIRST_PLACED + 3,
  /// In the old woman's sprite, who on the stormy night asks "You expect an old
  /// woman like me to use Psynergy?" By day: "Long ago, my grandfather was a
  /// powerful Adept... I'm so proud of him."
  /// ★ Named from the game: "You expect an old woman like me to use
  /// Psynergy?" (MsgValeOldWomanCannotUsePsynergy)
  ACTOR_OLD_WOMAN,
  /// A young man with brown hair: "Well, if it isn't Isaac. It's just another
  /// quiet
  /// day in Vale."
  /// ★ Named from the game: "It's just another quiet day in Vale."
  /// (MsgValeQuietDay)
  ACTOR_YOUNG_MAN_QUIET_DAY,
  /// A woman with fair hair: "Kraden had visitors when I dropped by his place
  /// this morning."
  /// ★ Named from the game: "Kraden had visitors when I dropped by his place
  /// this morning." (MsgValeKradenHadVisitors)
  ACTOR_WOMAN_ON_KRADENS_VISITORS,
  /// Two children, a small boy with fair hair and a little girl with orange
  /// pigtails; back from Kalay the boy thinks "I wonder if we can play
  /// with Isaac and his pals again, like we used to." By day: "Did the
  /// travelers that stopped by today go to meet the mayor?"
  /// ★ Named from what the player sees: a small boy with fair hair in a blue
  /// shirt (its render)
  ACTOR_FIRST_CHILD,
  /// The little girl: "With Psynergy, the more you practice, the more you
  /// learn."
  /// ★ Named from what the player sees: a little girl with orange pigtails in
  /// a green dress (its render)
  ACTOR_SECOND_CHILD,
  /// A strongman who shakes the ground with his Psynergy
  /// (Event_DemonstratePsynergy). "Hey,
  /// Isaac, have I shown you my new ability?"
  /// ★ Named from the game: "have I shown you my new ability?"
  /// (MsgValeShownNewAbility)
  ACTOR_STRONGMAN_SHOWING_PSYNERGY,
  /// A second strongman: "If your Psynergy runs low, just walk around. You'll
  /// recover it slowly."
  /// ★ Named from the game: "If your Psynergy runs low, just walk around."
  /// (MsgValeWalkToRecoverPsynergy)
  ACTOR_STRONGMAN_WALK_TO_RECOVER,
  /// An invisible actor (as in gondowan_cave.c), out of sight until the party
  /// has left; then it stands on the large Psynergy Stone that fell on the hut,
  /// for the hut's owner to turn to.
  /// ★ Named from the game: "That large Psynergy Stone fell right on top of the
  /// hut." (MsgValeTheStoneFellOnThe)
  ACTOR_FALLEN_STONE_SPOT = ACTOR_FIRST_PLACED + 13,
  /// A burly grey-haired man: "Isaac! Have you remembered to meditate on Mt.
  /// Aleph daily?"
  /// ★ Named from the game: "Have you remembered to meditate on Mt. Aleph
  /// daily?" (MsgValeMeditateOnMtAlephDaily)
  ACTOR_MAN_ASKING_ABOUT_MEDITATION = ACTOR_FIRST_PLACED + 15,
  /// A woman in a green dress: "You had a very difficult time three years ago,
  /// didn't you?"
  /// ★ Named from the game: "You had a very difficult time three years ago"
  /// (MsgValeADifficultTimeThreeYears)
  ACTOR_WOMAN_RECALLING_STORM,
  /// The hut's owner, an old man with a cane, who back from Kalay finds "a big
  /// hole in my roof". "Since
  /// food is stored in this hut, it isn't scary at all..."
  /// ★ Named from the game: "Since food is stored in this hut"
  /// (MsgValeFoodStoredForWinter)
  ACTOR_HUT_OWNER = ACTOR_FIRST_PLACED + 18
};

/// On the stormy night (gValeGate_StormPlacements).
enum ValeGateStormActor {
  /// The same old man as ACTOR_CURIOUS_OLD_MAN: "Huh? Are you...asking me
  /// if I can use Psynergy, Isaac?"
  /// ★ Named from the game: "Did you see anything interesting on your trip"
  /// (MsgValeAnythingInterestingOnYourTrip)
  ACTOR_STORM_CURIOUS_OLD_MAN = ACTOR_FIRST_PLACED + 3,
  /// The old woman: "You expect an old woman like me to use Psynergy?"
  /// ★ Named from the game: "You expect an old woman like me to use
  /// Psynergy?" (MsgValeOldWomanCannotUsePsynergy)
  ACTOR_STORM_OLD_WOMAN,
  /// A young man with brown hair: "I'm exhausted. I used up all my Psynergy
  /// deflecting falling rocks..."
  /// ★ Named from the game: "I used up all my Psynergy deflecting falling
  /// rocks..." (MsgValeExhaustedFromFallingRocks)
  ACTOR_STORM_ROCK_DEFLECTING_YOUNG_MAN,
  /// A woman with fair hair: "Oh! Little Felix has fallen into the river? That
  /// must be why Jenna looked so upset."
  /// ★ Named from the game: "has fallen into the river?"
  /// (MsgValeFelixFellInRiver)
  ACTOR_STORM_WOMAN_HEARING_OF_FELIX,
  /// A small boy with fair hair who lifts ACTOR_STORM_SMALL_ROCK
  /// (Event_DemonstrateLevitation). "I have some Psynergy left."
  /// ★ Named from what the player sees: a small boy with fair hair holds a
  /// small rock up in the air with his Psynergy, "See?" (its render)
  ACTOR_STORM_ROCK_LIFTING_BOY,
  /// A little girl with orange pigtails: "Jenna went off that way!"
  /// ★ Named from the game: "went off that way!" (MsgValeWentOffWay)
  ACTOR_STORM_GIRL_WHO_SAW_JENNA,
  /// The two strongmen Garet's grandfather sends to aid the elders. "We can't
  /// let the
  /// Mt. Aleph Boulder destroy our village."
  /// ★ Named from the game: "destroy our village" (MsgValeGoAidElders)
  ACTOR_STORM_FIRST_AIDE,
  /// ★ Named from the game: "help the elders" (MsgValeGoAidElders)
  ACTOR_STORM_SECOND_AIDE, ///< "Let's go help the elders."
  /// Garet's grandfather, the mayor, an old man with white hair and a cane
  /// ("Grandpa!", Garet calls him; his sprite is vale_interiors_shops.c's). "Go
  /// aid the elders!"
  /// ★ Named from the game: "aid the elders!" (MsgValeGoAidElders)
  ACTOR_STORM_GARETS_GRANDFATHER,
  /// The storm's rock sprite at three tenths of its size.
  /// ★ Named from what the player sees: a small rock floating beside the
  /// boy who lifts it (its render)
  ACTOR_STORM_SMALL_ROCK,
  /// An invisible actor on ACTOR_STORM_SMALL_ROCK's spot at three fifths of its
  /// size. XXX: nothing here moves or reads it after the setup; what it marks
  /// or blocks would say what it is for.
  ACTOR_STORM_ROCK_SPOT,
  /// Rested at the Psynergy Stone, the strongman vale_storm.c calls
  /// ACTOR_EXHAUSTED_STRONGMAN. "Great! I'm all charged up!"
  /// ★ Named from the game: "all charged up" (MsgValeGoAidElders)
  ACTOR_STORM_CHARGED_UP_STRONGMAN,
  /// The two travelers who survived Sol Sanctum, Saturos and Menardi by their
  /// sprites (as in vale_kraden.c). "Only the two of us survived..."
  /// ★ Named from the game: "Only the two of us survived"
  /// (MsgValeOnlyTwoSurvived)
  ACTOR_FIRST_TRAVELER,
  /// ★ Named from the game: "Only the two of us survived"
  /// (MsgValeOnlyTwoSurvived)
  ACTOR_SECOND_TRAVELER,
  /// Garet as a child, who follows Isaac through the night. "Isaac! Wait!"
  /// ★ Named from the game: "Garet (child)" (MsgDebugGlyphName)
  ACTOR_STORM_GARET,
  /// ★ Named from the game: "Jenna" (MsgCharacterName)
  ACTOR_STORM_JENNA ///< "Garet, Isaac... You came to help me?"
};

/// At the farewell (gValeGate_DeparturePlacements).
enum ValeGateFarewellActor {
  /// The small boy of ACTOR_FIRST_CHILD, at the top of the path. "What,
  /// didn't you hear? Isaac and Garet are leaving Vale on a journey."
  /// ★ Named from the game: "are leaving Vale on a journey"
  /// (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_FIRST_CHILD = ACTOR_FIRST_PLACED + 3,
  /// The little girl of ACTOR_SECOND_CHILD: "Why's everyone hanging around
  /// the town gate?"
  /// ★ Named from the game: "Why's everyone hanging around the town gate?"
  /// (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_SECOND_CHILD,
  /// The Great Healer, by his sprite (as in vale_eruption.c and sanctum.c). "We
  /// are counting on you, Isaac. Oh, and you, too, Garet."
  /// ★ Named from the game: "What is it, Great Healer?"
  /// (MsgSanctumWeHadIdeaTrueSanctum), said to the same sprite in the Sanctum
  ACTOR_FAREWELL_GREAT_HEALER,
  /// The man with long brown hair and a beard in a long robe of vale.c's
  /// ACTOR_MAN_GUARDING_MT_ALEPH: "Hmm... Dora is very late..."
  /// ★ Named from the game: "Dora is very late" (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_MAN_AWAITING_DORA,
  /// Garet's grandfather, the old man with white hair and a cane. "They can't
  /// keep standing around here like this, or
  /// they'll never leave."
  /// ★ Named from the game: "keep standing around here"
  /// (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_GARETS_GRANDFATHER,
  /// Garet's sister, the young woman with an orange ponytail, who gives the
  /// Herb. "I know not to drink stagnant water,
  /// Sis!", Garet answers her.
  /// ★ Named from the game: "stagnant water" (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_GARETS_SISTER,
  /// Garet's brother, the small boy with orange hair of vale.c's
  /// ACTOR_GARETS_BROTHER, who
  /// brings the Catch Beads: "Aunty Dora asked me to give this to you, Isaac."
  /// Of Garet: "He's a pig! He'll eat anything that's not tied down!"
  /// ★ Named from the game: "Aunty Dora asked me" (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_GARETS_BROTHER,
  /// A woman with orange hair under a green headscarf: "As Garet's mother, I
  /// know how Dora must be feeling."
  /// ★ Named from the game: "I know how Dora must be feeling"
  /// (MsgValeWhysEveryoneHanging)
  ACTOR_FAREWELL_GARETS_MOTHER,
  /// Garet's father, a man with brown hair in a teal shirt, by his sprite
  /// (vale_interiors_shops.c's).
  /// ★ Named from the game: "My own son" (MsgValeImProudOfYouGaretMy), what the
  /// same sprite says in vale_interiors_shops.c
  ACTOR_FAREWELL_GARETS_FATHER,
  /// The rest of the crowd at the gate, ten who only nod and jump: an old man
  /// with a cane, an old woman, a young man with brown hair, a woman with fair
  /// hair,
  /// three strongmen, a burly grey-haired man, a woman in a green dress and a
  /// stout man.
  /// ★ Named from what the player sees: men, women and a young man of Vale in
  /// their
  /// sprites (its render)
  ACTOR_FAREWELL_FIRST_CROWD_VILLAGER,
  /// The sprite of the dog of vault.c ("Woof! Woof!"), which runs past the
  /// children and later after ACTOR_FAREWELL_CAT.
  /// ★ Named from the game: "Woof! Woof!" (MsgValeRrruffRrrruff)
  ACTOR_FAREWELL_DOG = ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 10,
  /// A small cat set down where the dog stops, which runs off with the dog
  /// after it. No other scene places its sprite.
  /// ★ Named from what the player sees: a small white and grey cat (its
  /// render)
  ACTOR_FAREWELL_CAT
};

/// Back from Kalay (gValeGate_PlacementsAfterKalay).
enum ValeGateAfterKalayActor {
  /// A young man with brown hair: "Hey, if it isn't Isaac! You came back home,
  /// huh?"
  /// ★ Named from the game: "You came back home, huh?" (MsgValeYouCameBackHome)
  ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED + 3,
  /// A woman with fair hair: "Oh, Isaac! You came back sooner than I thought!"
  /// ★ Named from the game: "You came back sooner than I thought!"
  /// (MsgValeCameBackSooner)
  ACTOR_AFTER_KALAY_WOMAN_BACK_SOONER,
  /// A burly grey-haired man: "It looks like you made some friends in your
  /// travels, Isaac."
  /// ★ Named from the game: "made some friends in your travels"
  /// (MsgValeMadeFriendsOnJourney)
  ACTOR_AFTER_KALAY_MAN_ON_NEW_FRIENDS,
  /// A woman in a green dress: "The river in town grew so hot after Mt. Aleph
  /// erupted."
  /// ★ Named from the game: "The river in town grew so hot after Mt. Aleph
  /// erupted." (MsgValeRiverHeatedAndCooled)
  ACTOR_AFTER_KALAY_WOMAN_ON_HOT_RIVER,
  /// The same old man as ACTOR_CURIOUS_OLD_MAN: "Did you see anything
  /// interesting on your trip, Isaac?"
  /// ★ Named from the game: "Did you see anything interesting on your trip"
  /// (MsgValeAnythingInterestingOnYourTrip)
  ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN,
  /// ★ Named from the game: "How have you been!" (MsgValeReturnHowHaveYouBeen),
  /// the woman of ACTOR_STORM_OLD_WOMAN
  ACTOR_AFTER_KALAY_OLD_WOMAN, ///< "Oh, my! Isaac! How have you been!"
  /// The small boy of ACTOR_FIRST_CHILD: "Welcome back, Isaac! It's good to
  /// have you home!"
  /// ★ Named from the game: "It's good to have you home!" (MsgValeWelcomeHome)
  ACTOR_AFTER_KALAY_FIRST_CHILD,
  /// The little girl of ACTOR_SECOND_CHILD: "I see you made many friends on
  /// your journey."
  /// ★ Named from the game: "I see you made many friends on your journey."
  /// (MsgValeWantToMakeFriends)
  ACTOR_AFTER_KALAY_SECOND_CHILD,
  /// A strongman: "The eruption created a terrible mess on the other side of
  /// Mt. Aleph."
  /// ★ Named from the game: "The eruption created a terrible mess"
  /// (MsgValeEruptionMadeMess)
  ACTOR_AFTER_KALAY_STRONGMAN_ON_ERUPTION,
  /// A second strongman: "The eruption caused a crack to open on Mt. Aleph's
  /// northwest side."
  /// ★ Named from the game: "The eruption caused a crack to open"
  /// (MsgValeNorthwestCrack)
  ACTOR_AFTER_KALAY_STRONGMAN_ON_CRACK,
  /// The hut's owner, the old man with a cane of ACTOR_HUT_OWNER, searching the
  /// ground: "Hey! Where'd
  /// that Psynergy Stone go?"
  /// ★ Named from the game: "Where'd that Psynergy Stone go"
  /// (MsgValeThePsynergyStoneIsGone)
  ACTOR_AFTER_KALAY_HUT_OWNER
};

// Forward declarations.
static void Event_MeetSurvivingTravelers(void);
static void ValeGate_FightTravelers(void);
static void Event_TravelersLeaveAfterBattle(void);
static void Event_DepartForQuest(void);
static void Event_SayThenWait(s32 speaker, s32 frames);
static void ValeGate_FaceDirectionAndWait(s32 actorId, s32 facing, s32 frames);
static void ValeGate_FlashPsynergyPalette(struct FieldActor *actor);
static void ValeGate_SpawnEffectPair(struct FieldActor *parent);
static void ValeGate_UpdateRockLiftingBoyPsynergy(void);
static void ValeGate_UpdateShowingStrongmanPsynergy(void);
static void ValeGate_UpdateSmallRockPalette(void);
static void ValeGate_SetProgressFlagsAndExit(void);

#ifdef UBFIX
#define VALE_GATE_TURN_AND_WAIT ValeGate_FaceDirectionAndWait
#else
static inline s32 ValeGate_TurnAndWaitValue(s32 actor, s32 facing, s32 frames) {
  /* FAKEMATCH(wrapper): a forwarding inline that calls
   * ValeGate_FaceDirectionAndWait as returning a value; as a direct call in
   * Event_SentToAidElders, 15 of 455 instructions differ in tbs-en. */
  return ((s32 (*)())ValeGate_FaceDirectionAndWait)(actor, facing, frames);
}
#define VALE_GATE_TURN_AND_WAIT ValeGate_TurnAndWaitValue
#endif

#include "common/object_facing.h"

const struct SceneEntrance *ValeGate_GetEntrances(void) {
  return gValeGate_Entrances;
}

const struct SceneRegion *ValeGate_GetRegions(void) { return NULL; }

/// Returns the stormy night's exits while FLAG_VALE_STORM is set, else the
/// day's.
const u32 *ValeGate_GetExits(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeGate_StormExits;
  }
  return gValeGate_Exits;
}

/// Selects the placements: the stormy night's, the farewell's
/// (VALE_GATE_FROM_SANCTUM), after Kalay, else the day's.
const struct ScenePlacement *ValeGate_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeGate_StormPlacements;
  }
  if (gGameState.entrance == VALE_GATE_FROM_SANCTUM) {
    return gValeGate_DeparturePlacements;
  }
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    return gValeGate_PlacementsAfterKalay;
  }
  return gValeGate_Placements;
}

/// In Vale, searching the Psynergy Stone restores the party's PP.
///
/// Reached by searching cell code 100 in every table but the farewell's.
/// Clears FLAG_NOTHING_HAPPENS.
static void Event_UsePsynergyStone(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeCheckedThePsynergyStone, 1);
  Engine_AudioPlayCue(SE_RECOVERY);
  Party_AddPp(999, 0);
  Engine_EventWait(10);
  Engine_MessageShowCentered(MsgValePartyPpRestored, 1);
  UiWork_FinalizePendingCore();
  Engine_GameFlagClear(FLAG_NOTHING_HAPPENS);
  Engine_EventEnd();
}

/// Selects the events: after Kalay, after the party has left, the farewell's
/// (VALE_GATE_FROM_SANCTUM), the stormy night's, else the day's.
const struct SceneEvent *ValeGate_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    return gValeGate_EventsAfterKalay;
  }
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    return gValeGate_EventsAfterLeaving;
  }
  if (gGameState.entrance == VALE_GATE_FROM_SANCTUM) {
    return gValeGate_DepartureEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeGate_StormEvents;
  }
  return gValeGate_Events;
}

/// A burly grey-haired man asks whether Isaac meditates each day. He means on
/// Mt. Aleph, and he stands in Vale on the day the story begins.
///
/// Reached by talking to ACTOR_MAN_ASKING_ABOUT_MEDITATION on the day the
/// story begins.
static void Event_AskAboutMeditation(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeMeditateOnMtAlephDaily);
  Engine_ActorFaceEachOther(ACTOR_MAN_ASKING_ABOUT_MEDITATION,
                            ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(ACTOR_MAN_ASKING_ABOUT_MEDITATION, 0);
  Engine_EventEnd();
}

/// A woman in a green dress recalls the difficult time three years ago. She
/// asks Isaac about it, in Vale on the day the story begins.
///
/// Reached by talking to ACTOR_WOMAN_RECALLING_STORM on the day the story
/// begins.
static void Event_RecallThreeYearsAgo(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeADifficultTimeThreeYears);
  Engine_ActorFaceEachOther(ACTOR_WOMAN_RECALLING_STORM, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(ACTOR_WOMAN_RECALLING_STORM, 0);
  Engine_EventEnd();
}

/// A small boy asks whether the travelers went to meet the mayor. He has fair
/// hair, and asks it in Vale on the day the story begins, about the travelers
/// who stopped by.
///
/// Reached by talking to ACTOR_FIRST_CHILD on the day the story begins.
static void Event_AskAboutTheTravelers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDidTheTravelersMeetThe);
  Engine_ActorFaceEachOther(ACTOR_FIRST_CHILD, ACTOR_PARTY_LEADER, 2);
  Engine_EventAskYesNo(ACTOR_FIRST_CHILD, 0);
  Engine_EventEnd();
}

/// In Vale before the party sets out, a strongman shows Isaac his Psynergy and
/// shakes the ground; later he asks whether they worry about crossing the land
/// so few.
///
/// Reached by talking to ACTOR_STRONGMAN_SHOWING_PSYNERGY on the day the story
/// begins and after the party has left. The display adds and removes the
/// callback ValeGate_UpdateShowingStrongmanPsynergy and shakes the map for
/// three seconds.
static void Event_DemonstratePsynergy(void) {
  u32 i;
  s32 message;
  s32 shakes;
  s32 *origin;

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    message = MsgValeArentWorriedCrossing;
    Engine_EventSetMessage(message);
    // "...with just the two of you?", three or four: the flag numbered for a
    // character is set once that character has joined.
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0) {
      gEventWork->message++;
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      gEventWork->message++;
    }
    Engine_EventOpenMessage(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventSetMessage(message + 3);
    } else {
      Engine_EventSetMessage(message + 4);
    }
    Engine_EventShowMessage(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
  } else {
    origin = ((struct MapScrollWork *)gMapWork[0])->origin;
    Engine_EventSetMessage(MsgValeShownNewAbility);
    Engine_ActorFaceEachOther(ACTOR_STRONGMAN_SHOWING_PSYNERGY,
                              ACTOR_PARTY_LEADER, 0);
    Engine_EventAskYesNo(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 2);
    Engine_EventWait(15);
    // Flash the caster before starting the palette cycle and camera shake.
    Scene_BeginCutscenePsynergy();
    shakes = 0;
    for (i = 0; i < 40; i++) {
      ValeGate_FlashPsynergyPalette(
          Object_GetById(ACTOR_STRONGMAN_SHOWING_PSYNERGY));
      Engine_TaskWait(1);
    }
    FieldScheduler_AddCallback(ValeGate_UpdateShowingStrongmanPsynergy,
                               TASK_PRIORITY_SCENE);
    Engine_AudioPlayCue(SE_107);
    for (i = 0; i != 180; i++) {
      if (Math_RemainderUnsigned(i, 10) == 0) {
        if ((1 & shakes) != 0) {
          *origin -= PIXELS(1);
        } else {
          *origin += PIXELS(1);
        }
        shakes++;
      }
      Engine_EventWait(1);
    }
    // End the display and restore the strongman's ordinary appearance.
    Engine_AudioPlayCue(SE_289);
    Scheduler_RemoveCallback(ValeGate_UpdateShowingStrongmanPsynergy);
    Engine_TaskWait(1);
    Scene_EndCutscenePsynergy();
    Engine_ActorSetChildValue(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
    Engine_EventWait(40);
    Engine_EventSetMessage(MsgValeBeholdPowerPsynergy);
    Engine_EventShowMessage(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
  }
  Engine_EventEnd();
}

/// An old man with a cane asks Isaac a question that changes with the story.
/// In Vale he answers Isaac about Psynergy on the stormy night, asks about the
/// Wise One after the party has left, and about the journey after Kalay.
///
/// Reached by talking to ACTOR_STORM_CURIOUS_OLD_MAN on the stormy night,
/// ACTOR_CURIOUS_OLD_MAN after the party has left and
/// ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN after Kalay; the story state picks the
/// question.
static void Event_AskAboutJourney(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    Engine_EventSetMessage(MsgValeAnythingInterestingOnYourTrip);
    Engine_EventOpenMessage(ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
      Engine_EventShowMessage(ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
    } else {
      struct EventWork *work = gEventWork;

      work->message++;
      Engine_EventAskYesNo(ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_EventSetMessage(MsgValeYouSawTheWiseOne);
    Engine_EventAskYesNo(ACTOR_CURIOUS_OLD_MAN, 0);
  } else {
    Engine_EventSetMessage(MsgValeCanIUsePsynergy);
    Engine_EventAskYesNo(ACTOR_STORM_CURIOUS_OLD_MAN, 0);
  }
  Engine_EventEnd();
}

/// An old man shows Isaac the Psynergy Stone that fell on his hut. In Vale
/// after the party has left, the old man whose hut it is points out the large
/// stone, and the camera looks at it.
///
/// Reached by talking to ACTOR_HUT_OWNER after the party has left Vale.
static void Event_TalkAboutFallenStone(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_HUT_OWNER, ANIM_STAND);
  Engine_ActorFaceActor(ACTOR_HUT_OWNER, ACTOR_PARTY_LEADER, 20);
  Engine_ActorFaceActor(ACTOR_HUT_OWNER, ACTOR_FALLEN_STONE_SPOT, 40);
  Engine_EventSetMessage(MsgValeTheStoneFellOnThe);
  Event_SayThenWait(ACTOR_HUT_OWNER, 20);
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Engine_CameraMoveTo(PIXELS(337), -1, PIXELS(272), 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_HUT_OWNER, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_HUT_OWNER, ACTOR_PARTY_LEADER, 10);
  Event_SayThenWait(ACTOR_HUT_OWNER, 40);
  Actor_SetBuiltinAction(ACTOR_HUT_OWNER, 2);
  Engine_EventEnd();
}

/// In Vale on the stormy night, a small boy with some Psynergy left lifts
/// a small rock (ACTOR_STORM_SMALL_ROCK) into the air.
///
/// Reached by talking to ACTOR_STORM_ROCK_LIFTING_BOY on the stormy night.
/// Adds and removes the callbacks ValeGate_UpdateRockLiftingBoyPsynergy and
/// ValeGate_UpdateSmallRockPalette.
static void Event_DemonstrateLevitation(void) {
  u32 i;
  s32 savedMotionFlags;
  struct FieldActor *levitatingActor;

  Engine_EventBegin();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 760);
  Engine_ActorFaceEachOther(ACTOR_STORM_ROCK_LIFTING_BOY, ACTOR_PARTY_LEADER,
                            30);
  Engine_EventSetMessage(MsgValeIHaveSomePsynergyLeft);
  Event_SayThenWait(ACTOR_STORM_ROCK_LIFTING_BOY, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_STORM_ROCK_LIFTING_BOY, FACING_NORTHWEST,
                                20);
  Actor_SetAttachedEffect(ACTOR_STORM_ROCK_LIFTING_BOY, 0x102);
  Engine_EventWait(20);
  // Charge the caster's display, then start both palette updates.
  Scene_BeginCutscenePsynergy();
  for (i = 0; i < 40; i++) {
    ValeGate_FlashPsynergyPalette(Object_GetById(ACTOR_STORM_ROCK_LIFTING_BOY));
    Engine_TaskWait(1);
  }
  FieldScheduler_AddOrUpdateCallback(ValeGate_UpdateRockLiftingBoyPsynergy,
                                     TASK_PRIORITY_SCENE);
  FieldScheduler_AddCallback(ValeGate_UpdateSmallRockPalette,
                             TASK_PRIORITY_SCENE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  // Suspend ordinary movement while lifting the target a little each frame.
  levitatingActor = Object_GetById(ACTOR_STORM_SMALL_ROCK);
  savedMotionFlags = levitatingActor->motionFlags;
  levitatingActor->motionFlags = 0;
  for (i = 0; i < 40; i++) {
    levitatingActor->y.fixed += PIXELS(3) / 32;
    Engine_TaskWait(1);
  }
  levitatingActor->motionFlags = savedMotionFlags;
  Scheduler_RemoveCallback(ValeGate_UpdateRockLiftingBoyPsynergy);
  Scheduler_RemoveCallback(ValeGate_UpdateSmallRockPalette);
  Engine_TaskWait(1);
  Event_PlayCue(SE_161);
  Actor_SetChildValue(ACTOR_STORM_ROCK_LIFTING_BOY, 0);
  Actor_SetChildValue(ACTOR_STORM_SMALL_ROCK, 0);
  Engine_EventWait(40);
  Scene_EndCutscenePsynergy();
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STORM_ROCK_LIFTING_BOY,
                            30);
  Event_ShowMessage(ACTOR_STORM_ROCK_LIFTING_BOY, 0);
  Engine_EventEnd();
}

/// A little girl tells Isaac which way Jenna went. In Vale on the stormy night,
/// once he has been sent back to the river, she tells him to help Felix
/// instead.
///
/// Reached by talking to ACTOR_STORM_GIRL_WHO_SAW_JENNA on the stormy
/// night.
static void Event_TalkAboutHelping(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_STORM_GIRL_WHO_SAW_JENNA, ACTOR_PARTY_LEADER,
                            10);
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
    Engine_EventSetMessage(MsgValeSureHelp);
    Engine_EventShowMessage(ACTOR_STORM_GIRL_WHO_SAW_JENNA, 0);
  } else {
    Engine_EventSetMessage(MsgValeWentOffWay);
    Engine_EventShowMessage(ACTOR_STORM_GIRL_WHO_SAW_JENNA, 0);
  }
  Engine_EventEnd();
}

/// In Vale back from Kalay, a young man welcomes Isaac home; later he only says
/// this is Vale.
///
/// Reached by talking to ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN after Kalay.
/// Sets FLAG_VALE_WELCOMED_HOME.
static void Event_WelcomeIsaacHome(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_VALE_WELCOMED_HOME) != 0) {
    Engine_EventSetMessage(MsgValeThisIsVale);
  } else {
    Engine_EventSetMessage(MsgValeYouCameBackHome);
    GameFlag_Set(FLAG_VALE_WELCOMED_HOME);
  }
  Event_ShowMessage(ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN, 0);
  Engine_EventEnd();
}

/// The old man searching the ground says the Psynergy Stone is gone. In Vale
/// back from Kalay, he stops and tells Isaac the stone that fell on his roof
/// has gone.
///
/// Reached after Kalay by talking to ACTOR_AFTER_KALAY_HUT_OWNER, or by Mind
/// Read on it until FLAG_VALE_PSYNERGY_STONE_GONE_TOLD is set; sets it and
/// restarts the search.
static void Event_PsynergyStoneIsGone(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_AFTER_KALAY_HUT_OWNER);
  Engine_EventBegin();
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  Engine_ActorSetAnimation(ACTOR_AFTER_KALAY_HUT_OWNER, ANIM_STAND);
  Engine_ActorStop(ACTOR_AFTER_KALAY_HUT_OWNER);
  Engine_ActorShowEmote(ACTOR_AFTER_KALAY_HUT_OWNER, EMOTE_IN_FRONT, 40);
  actor->facing = FACING_NORTHWEST + FACING_STEP;
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_AFTER_KALAY_HUT_OWNER, 2);
  Engine_EventSetMessage(MsgValeThePsynergyStoneIsGone);
  Engine_EventShowMessageAndWait(ACTOR_AFTER_KALAY_HUT_OWNER, 0, 40);
  Engine_ActorFaceActor(ACTOR_AFTER_KALAY_HUT_OWNER, ACTOR_PARTY_LEADER, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_AFTER_KALAY_HUT_OWNER, 2);
  Engine_EventShowMessage(ACTOR_AFTER_KALAY_HUT_OWNER, 0);
  Engine_GameFlagSet(FLAG_VALE_PSYNERGY_STONE_GONE_TOLD);
  Engine_ActorStop(ACTOR_AFTER_KALAY_HUT_OWNER);
  Engine_TaskWait(1);
  Actor_SetActionScript(ACTOR_AFTER_KALAY_HUT_OWNER,
                        gValeGate_HutOwnerSearchActions);
  Engine_EventEnd();
}

#include "common/vale.h"

/// Opens a door's map cells and sends the leader through its threshold.
static inline void Event_OpenDoorAndWalkThrough(s32 cellX, s32 cellZ, s32 x,
                                                s32 z) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeGate_DoorOpens, cellX, cellZ);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, z);
}

// Vale's doors here: walking north into cell code n opens door n and takes exit
// n (Vale_RequestBackdropExit), into the houses (SceneId_ValeInteriorsShops) or
// another part of Vale (SceneId_ValeInteriorsInn).

/// In Vale, opens the northwest door, into a house (SceneId_ValeInteriorsShops,
/// VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST).
static void Event_OpenNorthwestDoor(void) {
  Event_OpenDoorAndWalkThrough(44, 7, 248, 279);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_SHOPS_EAST);
}

/// In Vale back from Kalay, opens the northeast doorway in two stages and walks
/// inside (SceneId_ValeInteriorsInn,
/// VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHEAST).
///
/// Reached by walking north into cell code 2 after Kalay; the other tables have
/// no such door.
static void Event_OpenNortheastDoor(void) {
  Event_PlayCue(SE_188);
  Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
  Engine_TaskWait(10);
  Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
  Engine_TaskWait(10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_INN_NORTH);
}

/// In Vale, enters through the western door of the middle row of houses
/// (SceneId_ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH).
static void Event_OpenCentralWestDoor(void) {
  Event_OpenDoorAndWalkThrough(43, 15, 230, 407);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_SHOPS_NORTH);
}

/// In Vale, enters through the eastern door of the middle row of houses
/// (SceneId_ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH).
static void Event_OpenCentralEastDoor(void) {
  Event_OpenDoorAndWalkThrough(52, 18, 374, 419);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_SHOPS_SOUTH);
}

/// In Vale, opens the southwest door, into Vale's shops
/// (SceneId_ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST).
static void Event_OpenSouthwestDoor(void) {
  Event_OpenDoorAndWalkThrough(41, 32, 200, 546);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_SHOPS_WEST);
}

/// In Vale, opens the door at the west edge, into another part of Vale
/// (SceneId_ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHWEST).
static void Event_OpenWestEdgeDoor(void) {
  Event_OpenDoorAndWalkThrough(35, 36, 102, 611);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_INN_NORTHWEST);
}

/// In Vale, opens the southeast door, into another part of Vale
/// (SceneId_ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_WEST).
static void Event_OpenSoutheastDoor(void) {
  Event_OpenDoorAndWalkThrough(51, 39, 358, 670);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE_INTERIORS_INN_SOUTH);
}

/// In Vale on the stormy night, leaves by the west passage, back to the
/// storm's map: stepping on cell code 9 takes VALE_GATE_TO_VALE, to the storm's
/// VALE_STORM_FROM_VALE_GATE.
static void Event_LeaveThroughWestPassage(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vale_RequestBackdropExit(VALE_GATE_TO_VALE);
}

/// In Vale, leaves by the town gate for the world map, once the party has
/// begun its journey.
///
/// Reached by stepping on cell code 8; takes VALE_GATE_TO_WORLD_MAP once
/// FLAG_PARTY_LEFT_VALE is set, else does nothing. The farewell
/// (Event_DepartForQuest) shows the villagers gathered at the gate and leaves
/// by the same exit.
static void Event_LeaveThroughTownGate(void) {
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Vale_RequestBackdropExit(VALE_GATE_TO_WORLD_MAP);
  }
}

/// In Vale, Whirlwind blows ACTOR_FIRST_IVY away.
///
/// Reached by Whirlwind on it by day, until it is gone; sets
/// FLAG_VALE_FIRST_IVY_BLOWN_AWAY.
static void Event_BlowAwayFirstIvy(void) {
  Engine_GameFlagSet((FLAG_VALE_FIRST_IVY_BLOWN_AWAY));
}

/// In Vale, Whirlwind blows ACTOR_SECOND_IVY away.
///
/// Reached by Whirlwind on it by day, until it is gone; sets
/// FLAG_VALE_SECOND_IVY_BLOWN_AWAY.
static void Event_BlowAwaySecondIvy(void) {
  Engine_GameFlagSet((FLAG_VALE_SECOND_IVY_BLOWN_AWAY));
}

/// In Vale, Whirlwind blows ACTOR_THIRD_IVY away.
///
/// Reached by Whirlwind on it by day, until it is gone; sets
/// FLAG_VALE_THIRD_IVY_BLOWN_AWAY.
static void Event_BlowAwayThirdIvy(void) {
  Engine_GameFlagSet((FLAG_VALE_THIRD_IVY_BLOWN_AWAY));
}

/// Arriving in this part of Vale: hides the ivy Whirlwind has blown away, runs
/// the three debug entrances (the strangers' departure and two presets), and
/// sets up the storm, the farewell or the village after Kalay.
///
/// The scene's initialize entry. VALE_GATE_DEBUG_CHILDHOOD sets FLAG_CHILDHOOD
/// and takes VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST; VALE_GATE_FROM_SANCTUM
/// runs Event_DepartForQuest; VALE_GATE_AFTER_TRAVELERS_BATTLE (and
/// VALE_GATE_DEBUG_BEFORE_TRAVELERS_LEAVE) Event_TravelersLeaveAfterBattle; on
/// the stormy night, once Garet has gone after Isaac
/// (FLAG_VALE_STORM_GARET_LEFT), Event_MeetSurvivingTravelers. Once the boys
/// have been sent back to the river, it removes the two strongmen who left to
/// aid the elders, Jenna and the man who went with them.
s32 ValeGate_Initialize(void) {
  struct FieldActor *actor;
  s32 flag;
  s32 verticalScale;

  if (Engine_GameFlagIsSet((FLAG_VALE_FIRST_IVY_BLOWN_AWAY)) != 0) {
    Engine_ActorSetPosition(ACTOR_FIRST_IVY, 0, 0);
  }
  if (Engine_GameFlagIsSet((FLAG_VALE_SECOND_IVY_BLOWN_AWAY)) != 0) {
    Engine_ActorSetPosition(ACTOR_SECOND_IVY, 0, 0);
  }
  if (Engine_GameFlagIsSet((FLAG_VALE_THIRD_IVY_BLOWN_AWAY)) != 0) {
    Engine_ActorSetPosition(ACTOR_THIRD_IVY, 0, 0);
  }
  switch (gGameState.entrance) {
  case VALE_GATE_DEBUG_CHILDHOOD:
    Engine_GameFlagSet(FLAG_CHILDHOOD);
    Engine_EventRequestExit(VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST);
    return 0;
  case VALE_GATE_DEBUG_STORY_FLAGS:
    ValeGate_SetProgressFlagsAndExit();
    return 0;
  case VALE_GATE_DEBUG_BEFORE_TRAVELERS_LEAVE:
    Event_TravelersLeaveAfterBattle();
    return 0;
  }
  verticalScale = 3 * Q16_ONE / 2;
  actor = Object_GetById(ACTOR_FIRST_IVY);
  actor->scaleY = verticalScale;
  actor = Object_GetById(ACTOR_SECOND_IVY);
  actor->scaleY = verticalScale;
  actor = Object_GetById(ACTOR_THIRD_IVY);
  actor->scaleY = verticalScale;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    Map_CopyCellsTo(97, 2, 80, 5, 2, 2);
    Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
      FieldFx_StartGroundParticles();
      FieldFx_StartLightning();
      Map_CopyCellAttributes(21, 38, 1, 1, 18, 41);
      flag = Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
      if (flag != 0) {
        Engine_ActorSetPosition(ACTOR_STORM_FIRST_AIDE, 0, 0);
        Engine_ActorSetPosition(ACTOR_STORM_SECOND_AIDE, 0, 0);
        FieldObject_SetTargetAndCallback(ACTOR_STORM_GARETS_GRANDFATHER,
                                         ACTOR_LINK_KEEP_MOTION |
                                             ACTOR_PARTY_LEADER,
                                         gValeGate_FaceTargetActions);
      }
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Actor_SetPosition(ACTOR_SECOND_CHILD, PIXELS(180), PIXELS(568));
        Map_CopyCellsTo(92, 2, 80, 5, 2, 2);
        Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
      }
    }
    if (gGameState.entrance == VALE_GATE_FROM_SANCTUM) {
      Event_DepartForQuest();
    } else {
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
        actor = Object_GetById(ACTOR_STORM_SMALL_ROCK);
        actor->scaleX = 3 * Q16_ONE / 10;
        actor->scaleY = 3 * Q16_ONE / 10;
        actor = Object_GetById(ACTOR_STORM_SMALL_ROCK);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor = Object_GetById(ACTOR_STORM_ROCK_SPOT);
        actor->scaleX = 3 * Q16_ONE / 5;
        actor->scaleY = 3 * Q16_ONE / 5;
        Engine_ActorSetAnimation(ACTOR_STORM_ROCK_DEFLECTING_YOUNG_MAN, 5);
      } else {
        if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
          Actor_SetPosition(ACTOR_FALLEN_STONE_SPOT, PIXELS(331), PIXELS(249));
          actor = Object_GetById(ACTOR_FALLEN_STONE_SPOT);
          Engine_ActorSetSpriteFlags(actor, 0);
        }
      }
      // XXX: by day these numbers are ACTOR_HUT_OWNER and a strongman out of
      // sight; whether the flag is still set then would say if he is hidden.
      if (Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
        Engine_ActorSetPosition(ACTOR_STORM_JENNA, 0, 0);
        Engine_ActorSetPosition(ACTOR_STORM_CHARGED_UP_STRONGMAN, 0, 0);
      }
      if (gGameState.entrance == VALE_GATE_AFTER_TRAVELERS_BATTLE) {
        Event_TravelersLeaveAfterBattle();
      } else {
        if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0 &&
            (flag = Engine_GameFlagIsSet(FLAG_VALE_STORM_GARET_LEFT)) != 0) {
          Event_MeetSurvivingTravelers();
        } else {
          if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            FieldFx_ResumeLightning();
          }
        }
      }
    }
  }
  return 0;
}

/// Walks a separate scene actor back to the leader, then hides it.
static inline void ValeGate_RejoinParty(s32 actorId) {
  struct FieldActor *leader;

  Engine_ActorSetAnimation(actorId, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(actorId, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(actorId);
  Engine_ActorSetPosition(actorId, 0, 0);
}

/// The mayor sends help to the elders and the boys back to the river. In Vale's
/// plaza on the stormy night, Garet's grandfather, the mayor, sends two
/// strongmen to aid the elders holding back the Boulder, then sends Isaac and
/// Garet back to the river to show Jenna and a man whose Psynergy is restored
/// the way. "Go aid the elders!"
///
/// Reached by stepping on cell code 20 on the stormy night, until
/// FLAG_VALE_STORM_SENT_TO_AID_ELDERS is set; sets it. The flag is named for
/// the mayor's first order, but what it records is the boys being sent back:
/// the return to the river (vale_storm.c) waits for it.
static void Event_SentToAidElders(void) {
  struct FieldActor *leader;
  s32 skipReply = 0;
  s32 speechFlags;

  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0 &&
      Engine_GameFlagIsSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
    // The camera goes to the villagers by the stone. Garet's grandfather: go
    // aid the elders, the Boulder is too heavy for them.
    Engine_EventBegin();
    Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    Engine_CameraMoveTo(PIXELS(197), -1, PIXELS(768), 1);
    Camera_WaitForArrival();
    Engine_EventSetMessage(MsgValeGoAidElders);
    Engine_ActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
    Event_ShowMessageAndWait(
        SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0, 10);
    // Isaac walks over and Garet steps out of him.
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_STORM_GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 179, 789);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Engine_ActorSetPosition(ACTOR_STORM_GARET, leader->x.fixed,
                              leader->z.fixed);
    }
    Actor_WalkToAndWait(ACTOR_STORM_GARET, 179, 804);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STORM_GARET, 40);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_STORM_GARET, FACING_EAST, 0);
    // The two aides will not let the Boulder destroy the village and go to
    // help; Garet's grandfather counts on them.
    Engine_ActorSetAnimation(ACTOR_STORM_FIRST_AIDE, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_SECOND_AIDE, ANIM_NOD);
    Engine_ActorFaceEachOther(ACTOR_STORM_FIRST_AIDE, ACTOR_STORM_SECOND_AIDE,
                              0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_STORM_FIRST_AIDE, 1);
    Engine_EventShowMessageAndWait(
        SPEAKER_WINDOW_ABOVE | ACTOR_STORM_FIRST_AIDE, 0, 10);
    Engine_ActorSetAnimation(ACTOR_STORM_SECOND_AIDE, ANIM_NOD);
    Engine_EventShowMessageAndWait(ACTOR_STORM_SECOND_AIDE, 0, 10);
    Engine_ActorFaceDirection(ACTOR_STORM_FIRST_AIDE, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_STORM_SECOND_AIDE, FACING_NORTHEAST + FACING_STEP,
                        10);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_NOD);
    Event_ShowMessageAndWait(
        SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0, 10);
#if TBS_PATCH_4
    Engine_ActorSetSpritePriority(ACTOR_STORM_SECOND_AIDE, 1);
#endif
    Actor_SetSpeed(ACTOR_STORM_FIRST_AIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetSpeed(ACTOR_STORM_SECOND_AIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    // The two run off; Isaac and Garet step up to the group.
    Actor_SetActionScript(ACTOR_STORM_FIRST_AIDE, gValeGate_AideRunActions);
    Engine_EventWait(20);
#if TBS_PATCH_4
    Object_GetById(ACTOR_STORM_SECOND_AIDE)->priorityFlags |= 1;
#endif
    Actor_SetActionScript(ACTOR_STORM_SECOND_AIDE, gValeGate_AideRunActions);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_STORM_GARET, FACING_NORTH, 60);
    Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeGate_LeaderStepActions);
    Actor_RunActionScriptAndWait(ACTOR_STORM_GARET,
                                 gValeGate_StormGaretStepActions);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_STORM_GARET, FACING_EAST, 10);
    // Garet calls out to Grandpa and Jenna; Jenna asks whether they came to
    // help her.
    Engine_EventShowMessage(ACTOR_STORM_GARET, 0);
    Actor_FaceDirection(ACTOR_STORM_GARETS_GRANDFATHER, FACING_WEST, 0);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_JENNA, FACING_SOUTHWEST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_STORM_JENNA, 2);
    Event_SayThenWait(ACTOR_STORM_JENNA, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_GARET, ANIM_NOD);
    Engine_EventWait(20);
    // Garet's grandfather asks whether Jenna told them about Felix. Yes: good
    // boys, Jenna could use a hand; no: help her regardless.
    Engine_ActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
    Engine_EventOpenMessage(
        SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
      skipReply = 1;
      Engine_ActorSetAnimation(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
    } else {
      Engine_ActorSetAnimation(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_NOD);
      gEventWork->message++;
    }
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0);
    if (skipReply != 0) {
      gEventWork->message++;
    }
    // The rested strongman is all charged up; everyone turns to him.
    speechFlags = SPEAKER_WINDOW_ABOVE;
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_CHARGED_UP_STRONGMAN,
                                  FACING_SOUTH, 30);
    Engine_EventShowMessage(ACTOR_STORM_CHARGED_UP_STRONGMAN, 0);
    Actor_ShowEmote(ACTOR_STORM_GARETS_GRANDFATHER, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(ACTOR_STORM_JENNA, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 0);
    Actor_ShowEmote(ACTOR_STORM_GARET, EMOTE_IN_FRONT | 0, 40);
    Actor_FaceDirection(ACTOR_STORM_GARETS_GRANDFATHER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_STORM_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_GARET, FACING_NORTHEAST, 10);
    Engine_CameraSetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
    Camera_MoveTo(PIXELS(215), -1, PIXELS(758), 1);
    Camera_WaitForArrival();
    Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    Camera_MoveTo(PIXELS(205), -1, PIXELS(778), 1);
    // He hops down to them; asked about his Psynergy, he has enough to save
    // Felix, thanks to the Psynergy Stone.
    Actor_SetActionScript(ACTOR_STORM_CHARGED_UP_STRONGMAN,
                          gValeGate_ChargedUpStrongmanActions);
    Object_RefreshSelectorById(ACTOR_STORM_CHARGED_UP_STRONGMAN);
    VALE_GATE_TURN_AND_WAIT(ACTOR_STORM_CHARGED_UP_STRONGMAN, FACING_SOUTHEAST,
                            60);
    Engine_ActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
    Event_SayThenWait(ACTOR_STORM_GARETS_GRANDFATHER, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_CHARGED_UP_STRONGMAN, ANIM_NOD);
    Event_SayThenWait(ACTOR_STORM_CHARGED_UP_STRONGMAN, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_NOD);
    Engine_EventWait(10);
    // Garet's grandfather sends Jenna back with the rested strongman, and Isaac
    // and Garet to show the way past the fallen rocks; Jenna thanks Isaac.
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_GARETS_GRANDFATHER, FACING_SOUTH,
                                  30);
    Event_SayThenWait(speechFlags + ACTOR_STORM_GARETS_GRANDFATHER, 10);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_JENNA, FACING_NORTHEAST, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_JENNA, ANIM_NOD);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_GARETS_GRANDFATHER, FACING_WEST,
                                  30);
    Engine_ActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
    Event_SayThenWait(speechFlags + ACTOR_STORM_GARETS_GRANDFATHER, 10);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STORM_GARET, 40);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_GARET, FACING_EAST, 20);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_JENNA, FACING_WEST, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_JENNA, ANIM_NOD);
    Event_SayThenWait(ACTOR_STORM_JENNA, 30);
    ValeGate_FaceDirectionAndWait(ACTOR_STORM_JENNA, FACING_NORTH, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_JENNA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_STORM_CHARGED_UP_STRONGMAN, ANIM_NOD);
    // Garet, Jenna and the rested strongman walk into Isaac; Garet's
    // grandfather keeps turning to watch him.
    ValeGate_RejoinParty(ACTOR_STORM_GARET);
    ValeGate_RejoinParty(ACTOR_STORM_JENNA);
    ValeGate_RejoinParty(ACTOR_STORM_CHARGED_UP_STRONGMAN);
    FieldObject_SetTargetAndCallback(ACTOR_STORM_GARETS_GRANDFATHER,
                                     ACTOR_LINK_KEEP_MOTION |
                                         ACTOR_PARTY_LEADER,
                                     gValeGate_FaceTargetActions);
    Engine_GameFlagSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
    Engine_EventEnd();
  }
}

/// In Vale on the stormy night, after the river, Isaac overhears two travelers
/// who survived Sol Sanctum's fury, the game's first sight of Saturos and
/// Menardi. "Only the two of us survived..."
///
/// Called by ValeGate_Initialize on the stormy night once
/// FLAG_VALE_STORM_GARET_LEFT is set; goes on into ValeGate_FightTravelers.
static void Event_MeetSurvivingTravelers(void) {
  struct FieldActor *camera;

  camera = Camera_GetEventObject();
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  // The screen opens on the storm at the west path, where Isaac comes in from
  // the river; the village music plays as he walks up and hears the first
  // traveler.
  Engine_CameraMoveTo(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Event_PrepareObjectAndApplyValue(1, 0);
  Engine_EventOpenScreen();
  Engine_AudioPlayCue(MUS_017);
  FieldFx_ResumeLightning();
  Engine_ActorSetPosition(ACTOR_FIRST_TRAVELER, PIXELS(105), PIXELS(267));
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 93, 343);
  Engine_EventSetMessage(MsgValeOnlyTwoSurvived);
  Engine_EventShowMessage(ACTOR_FIRST_TRAVELER, 0);
  // The travelers' theme starts; the camera rises to them and the second
  // traveler walks in.
  Engine_AudioPlayCue(MUS_061);
  camera->motionFlags = 0;
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Engine_CameraMoveTo(PIXELS(109), PIXELS(176), PIXELS(281), 1);
  Camera_WaitForArrival();
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_SECOND_TRAVELER, PIXELS(135), PIXELS(177));
  Actor_SetSpeed(ACTOR_SECOND_TRAVELER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_SECOND_TRAVELER, 126, 258);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_NORTH + FACING_STEP,
                            0);
  Engine_ActorWaitForMove(ACTOR_SECOND_TRAVELER);
  Engine_ActorSetAnimation(ACTOR_SECOND_TRAVELER, ANIM_STAND);
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER,
                                FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  // They talk of Sol Sanctum's fury and its trap switch, of the power of
  // Alchemy, and of their next attempt on the sanctum.
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
  Event_SayThenWait(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 30);
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER,
                                FACING_NORTHWEST + FACING_STEP, 20);
  Event_SayThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 10);
  ValeGate_FaceDirectionAndWait(ACTOR_FIRST_TRAVELER,
                                FACING_NORTHWEST + FACING_STEP, 40);
  Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_FIRST_TRAVELER,
                                FACING_NORTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
  Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
  Engine_EventWait(20);
  Event_SayThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 20);
  ValeGate_FightTravelers();
  Engine_EventEnd();
}

/// In Vale on the stormy night, the travelers catch Isaac eavesdropping and
/// fight; win or lose, the party returns by VALE_GATE_AFTER_TRAVELERS_BATTLE.
/// "You must forget everything you heard."
///
/// Called by Event_MeetSurvivingTravelers. Starts the fifth slot of the
/// encounter zone of the scripted battles of Vale, Vault and the world map,
/// with both outcomes leading back to VALE_GATE_AFTER_TRAVELERS_BATTLE, and
/// sets FLAG_KEEP_MUSIC.
static void ValeGate_FightTravelers(void) {
  struct FieldActor *actor;
  s32 southSouthwestFacing;
  s32 westFacing;
  s32 northNortheastFacing;

  // Garet, still out of sight, calls Isaac to wait; the travelers turn and
  // look down at Isaac.
  Engine_ActorSetChildValue(ACTOR_STORM_GARET, 15);
  actor = Object_GetById(ACTOR_STORM_GARET);
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_SetPosition(ACTOR_STORM_GARET, 0, PIXELS(331));
  Engine_TaskWait(1);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_STORM_GARET, 0);
  Actor_ShowEmote(ACTOR_FIRST_TRAVELER, EMOTE_IN_FRONT | 0, 0);
  Actor_ShowEmote(ACTOR_SECOND_TRAVELER, EMOTE_IN_FRONT | 0, 40);
  Engine_ActorSetPosition(ACTOR_STORM_GARET, 0, 0);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTH + FACING_STEP, 0);
  southSouthwestFacing = FACING_SOUTH + FACING_STEP;
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, southSouthwestFacing,
                                40);
  Engine_CameraSetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  Engine_CameraMoveTo(PIXELS(89), PIXELS(176), PIXELS(313), 1);
  Camera_WaitForArrival();
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_NORTHEAST, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER,
                                FACING_SOUTHWEST + FACING_STEP, 40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(100), PIXELS(144), PIXELS(333), 1);
  Actor_SetSpeed(ACTOR_FIRST_TRAVELER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_SECOND_TRAVELER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_FIRST_TRAVELER, 105, 329);
  Engine_EventWait(10);
  Engine_ActorWalkTo(ACTOR_SECOND_TRAVELER, 124, 329);
  Engine_ActorWaitForMove(ACTOR_FIRST_TRAVELER);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRAVELER, ANIM_STAND);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, southSouthwestFacing, 0);
  Engine_ActorWaitForMove(ACTOR_SECOND_TRAVELER);
  Engine_ActorSetAnimation(ACTOR_SECOND_TRAVELER, ANIM_STAND);
  Engine_ActorFaceDirection(ACTOR_SECOND_TRAVELER, southSouthwestFacing, 0);
  // Garet runs in from the west. The first traveler asks whether Isaac was
  // eavesdropping; whatever he answers, Garet thinks they look scary.
  Engine_ActorSetChildValue(ACTOR_STORM_GARET, 0);
  actor = Object_GetById(ACTOR_STORM_GARET);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetPosition(ACTOR_STORM_GARET, 0, PIXELS(331));
  Actor_SetSpeed(ACTOR_STORM_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkToAndWait(ACTOR_STORM_GARET, 37, 339);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_EventOpenMessage(ACTOR_FIRST_TRAVELER, 0);
  Actor_ShowEmote(ACTOR_STORM_GARET, EMOTE_IN_FRONT | 1, 0);
  northNortheastFacing = FACING_NORTH + FACING_STEP;
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, northNortheastFacing, 10);
  Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_EAST, 0);
  westFacing = FACING_WEST;
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, westFacing, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_STORM_GARET, 2);
  Event_SayThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_STORM_GARET, 10);
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_STORM_GARET,
                                   gValeGate_FaceTargetActions);
  Actor_WalkToAndWait(ACTOR_STORM_GARET, 93, 361);
  ValeGate_FaceDirectionAndWait(ACTOR_STORM_GARET, northNortheastFacing, 40);
  Event_SayThenWait(ACTOR_STORM_GARET, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_EAST, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, westFacing, 15);
  Engine_ActorSetAnimation(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
  Engine_ActorFaceDirection(ACTOR_FIRST_TRAVELER, southSouthwestFacing, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_SECOND_TRAVELER, southSouthwestFacing,
                                30);
  // The travelers: forget everything you heard; we'll help you forget.
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, northNortheastFacing, 30);
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Event_SayThenWait(ACTOR_FIRST_TRAVELER, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_ActorSetAttachedEffect(ACTOR_STORM_GARET, 0x102);
  Engine_EventWait(80);
  Actor_SetActionScript(ACTOR_SECOND_TRAVELER,
                        gValeGate_SecondTravelerLeapActions);
  Engine_EventWait(6);
  Actor_SetActionScript(ACTOR_FIRST_TRAVELER,
                        gValeGate_FirstTravelerLeapActions);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeGate_LeaderActions);
  Engine_EventWait(6);
  Actor_RunActionScriptAndWait(ACTOR_STORM_GARET,
                               gValeGate_StormGaretLeapActions);
  // The confrontation continues in battle; both outcomes return here.
  Battle_SetStart(BATTLE_START_NO_ESCAPE);
  {
    const void *message = &SceneId_ValeGate;

    Event_SetNextDestination((s32)message, VALE_GATE_AFTER_TRAVELERS_BATTLE);
    Event_SetDefeatDestination((s32)message, VALE_GATE_AFTER_TRAVELERS_BATTLE);
  }
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 4);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// In Vale after the battle, the travelers leave Isaac and Garet lying in the
/// storm, and the storm, and the prologue, end.
///
/// Called by ValeGate_Initialize from VALE_GATE_AFTER_TRAVELERS_BATTLE, after
/// the battle, or VALE_GATE_DEBUG_BEFORE_TRAVELERS_LEAVE. Clears
/// FLAG_VALE_STORM and takes VALE_GATE_TO_TITLE, to the title (SceneId_Title,
/// TITLE_FROM_VALE_GATE), which then opens on Vale three years later
/// (menu/title.c).
static void Event_TravelersLeaveAfterBattle(void) {
  struct FieldActor *actor;
  s32 gravityQ8;

  // The screen opens on Isaac and Garet lying beaten in the storm, the
  // travelers standing over them.
  Engine_AudioPlayCue(MUS_017);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_CameraMoveTo(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(48), PIXELS(346));
  Actor_SetPosition(ACTOR_STORM_GARET, PIXELS(78), PIXELS(358));
  Actor_SetPosition(ACTOR_FIRST_TRAVELER, PIXELS(103), PIXELS(342));
  Actor_SetPosition(ACTOR_SECOND_TRAVELER, PIXELS(112), PIXELS(360));
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_TRAVELER, FACING_WEST, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->scaleX = -Q16_ONE;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_STORM_GARET, 7);
  actor = Object_GetById(ACTOR_STORM_GARET);
  actor->sprite->rotation = 0x1555;
  actor = Object_GetById(ACTOR_STORM_GARET);
  Engine_ActorSetSpriteFlags(actor, 0);
  {
    struct EventWork *work = gEventWork;

    work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  }
  Engine_EventOpenScreen();
  FieldFx_ResumeLightning();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_TRAVELER, FACING_NORTH, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FIRST_TRAVELER, FACING_WEST, 10);
  Actor_FaceDirection(ACTOR_SECOND_TRAVELER, FACING_WEST, 10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(ACTOR_STORM_GARET, 3);
  Actor_SetSpeed(ACTOR_FIRST_TRAVELER, PIXELS(12) / 5, PIXELS(6) / 5);
  gravityQ8 = 128;
  actor = Object_GetById(ACTOR_FIRST_TRAVELER);
  actor->bounce = 0x28f;
  actor->gravity = gravityQ8 << 8;
  Actor_SetActionScript(ACTOR_FIRST_TRAVELER,
                        gValeGate_FirstTravelerBounceActions);
  Engine_EventWait(24);
  Engine_ActorSetSpeed(ACTOR_SECOND_TRAVELER, PIXELS(12) / 5, PIXELS(6) / 5);
  actor = Object_GetById(ACTOR_SECOND_TRAVELER);
  actor->bounce = 0x28f;
  actor->gravity = gravityQ8 << 8;
  Actor_RunActionScriptAndWait(ACTOR_SECOND_TRAVELER,
                               gValeGate_SecondTravelerBounceActions);
  Engine_EventWait(40);
  // Three last flashes of lightning, then the storm ends and the screen
  // fades slowly out.
  FieldFx_StopLightning();
  FieldFx_LastLightning();
  Engine_TaskWait(20);
  FieldFx_LastLightning();
  Engine_TaskWait(60);
  FieldFx_LastLightning();
  Engine_TaskWait(20);
  FieldFx_StopLightning();
  Engine_EventWait(40);
  gEventWork->transitionFrames = 120;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_GameFlagClear(FLAG_VALE_STORM);
  Engine_EventRequestExit(VALE_GATE_TO_TITLE);
  Engine_EventEnd();
}

/// In Vale, at the town gate, the villagers give their supplies and farewells
/// as Isaac and Garet set out on the Wise One's quest.
///
/// Called by ValeGate_Initialize from VALE_GATE_FROM_SANCTUM, the way out of
/// the Vale Sanctum once the quest is accepted. Restores the party's HP and PP,
/// gives a Herb (from Garet's sister) and the Catch Beads (from Dora), and
/// takes VALE_GATE_TO_WORLD_MAP.
static void Event_DepartForQuest(void) {
  struct FieldActor *actor;
  struct EventWork *work;
  s32 priorityFlags;
  const s32 *faceActions;
  const s32 *chaseActions;
  const s32 *departureActions;

  // The village gathers at the gate to see Isaac and Garet off; the camera
  // starts on the two children up the path.
  Party_RestoreHpPp(1);
  Engine_EventBegin();
  Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
  Camera_MoveTo(PIXELS(180), PIXELS(16), PIXELS(618), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  actor = Object_GetById(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 2);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 3);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 4);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 5);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 6);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 9);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetSpritePriority(GARET, 1);
  Engine_ActorSetSpritePriority(ACTOR_FAREWELL_GARETS_BROTHER, 1);
  Engine_ActorSetSpritePriority(ACTOR_FAREWELL_GARETS_SISTER, 1);
  Engine_ActorSetSpritePriority(ACTOR_FAREWELL_GARETS_GRANDFATHER, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(208), PIXELS(814));
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  // The second child asks why everyone is at the gate; the first says Isaac and
  // Garet are leaving on a journey.
  Actor_ShowEmote(ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | 1, 40);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHWEST + FACING_STEP, 20);
  Event_SetMessage(MSGNO(MsgValeWhysEveryoneHanging));
  Event_SayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 10);
  Actor_ShowEmote(ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | 2, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_EAST + FACING_STEP, 10);
  Event_SayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_FIRST_CHILD, 2);
  Event_SayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 10);
  // They walk to the top of the path: the second child wishes to go too, the
  // first has heard it will be dangerous, and everyone is down there to say
  // good-bye.
  Actor_ShowEmote(ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | 0, 40);
  Actor_SetSpeed(ACTOR_FAREWELL_SECOND_CHILD, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FAREWELL_SECOND_CHILD, 184, 618);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 60);
  Event_SayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 20);
  Actor_SetSpeed(ACTOR_FAREWELL_FIRST_CHILD, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_FAREWELL_FIRST_CHILD, 168, 618);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_NORTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CHILD, ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_SECOND_CHILD, 1);
  Engine_EventShowMessage(ACTOR_FAREWELL_SECOND_CHILD, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_FAREWELL_DOG, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetPosition(ACTOR_FAREWELL_DOG, PIXELS(110), PIXELS(744));
  Engine_TaskWait(2);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_DOG, ANIM_NOD);
  Actor_SetActionScript(ACTOR_FAREWELL_DOG, gValeGate_FarewellDogActions);
  Engine_EventWait(40);
  faceActions = gValeGate_FaceTargetActions;
  FieldObject_SetTargetAndCallback(ACTOR_FAREWELL_FIRST_CHILD,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_FAREWELL_DOG,
                                   faceActions);
  FieldObject_SetTargetAndCallback(ACTOR_FAREWELL_SECOND_CHILD,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_FAREWELL_DOG,
                                   faceActions);
  Object_RefreshSelectorById(ACTOR_FAREWELL_DOG);
  Engine_ActorStop(ACTOR_FAREWELL_FIRST_CHILD);
  Engine_ActorStop(ACTOR_FAREWELL_SECOND_CHILD);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | 5, 120);
  Engine_ActorFaceDirection(ACTOR_FAREWELL_FIRST_CHILD,
                            FACING_EAST + FACING_STEP, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHWEST + FACING_STEP, 80);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTH + FACING_STEP, 40);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
  Engine_EventWait(20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTH + FACING_STEP, 60);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 40);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTH + FACING_STEP, 60);
  // They step down to look over the crowd: Isaac's mother is not there to
  // see him off.
  Actor_ShowEmote(ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorFaceDirection(ACTOR_FAREWELL_FIRST_CHILD,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_FAREWELL_SECOND_CHILD, 184, 630);
  Engine_EventWait(20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTH + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | 1, 40);
  Actor_ShowEmote(ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | 1, 40);
  Actor_WalkToAndWait(ACTOR_FAREWELL_FIRST_CHILD, 168, 630);
  Engine_EventWait(20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 40);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTH + FACING_STEP, 40);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ShowEmote(ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | 1, 40);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_EAST + FACING_STEP, 10);
  Event_SayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
  Event_SayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 10);
  Actor_ShowEmote(ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | 0, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTH + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTH + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_SOUTH + FACING_STEP, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
  Event_SayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 10);
  {
    struct FieldActor *actor = Object_GetById(ACTOR_FAREWELL_DOG);

    if (actor != NULL) {
      Engine_ActorSetPosition(ACTOR_FAREWELL_CAT, actor->x.fixed,
                              actor->z.fixed);
    }
  }
  priorityFlags = 0xfe;
  Engine_TaskWait(2);
  Object_GetById(ACTOR_FAREWELL_DOG)->priorityFlags &= priorityFlags;
  Object_GetById(ACTOR_FAREWELL_CAT)->priorityFlags &= priorityFlags;
  Engine_ActorSetSpritePriority(ACTOR_FAREWELL_DOG, 2);
  Engine_ActorSetSpritePriority(ACTOR_FAREWELL_CAT, 2);
  Actor_SetSpeed(ACTOR_FAREWELL_CAT, PIXELS(18) / 5, PIXELS(9) / 5);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_CAT, ANIM_WALK);
  chaseActions = gValeGate_FarewellChaseActions;
  Actor_SetActionScript(ACTOR_FAREWELL_CAT, chaseActions);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_DOG, ANIM_NOD);
  Actor_SetSpeed(ACTOR_FAREWELL_DOG, PIXELS(24) / 5, PIXELS(12) / 5);
  Actor_RunActionScriptAndWait(ACTOR_FAREWELL_DOG, chaseActions);
  Engine_EventWait(60);
  // Should they say good-bye too? Sure: both run down, and the camera
  // follows them to the gate.
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_SECOND_CHILD, 2);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHWEST + FACING_STEP, 10);
  Event_SayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_FIRST_CHILD, 1);
  Engine_EventWait(20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_FIRST_CHILD,
                                FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
  Event_SayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 20);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_FAREWELL_FIRST_CHILD, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetSpeed(ACTOR_FAREWELL_SECOND_CHILD, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_SetActionScript(ACTOR_FAREWELL_FIRST_CHILD,
                        gValeGate_FarewellFirstChildActions);
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_MoveTo(PIXELS(215), PIXELS(16), PIXELS(801), 1);
  Engine_EventWait(10);
  Actor_SetActionScript(ACTOR_FAREWELL_SECOND_CHILD,
                        gValeGate_FarewellSecondChildActions);
  Object_RefreshSelectorById(ACTOR_FAREWELL_SECOND_CHILD);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_SECOND_CHILD,
                                FACING_SOUTHEAST + FACING_STEP, 120);
  // The Great Healer counts on Isaac, and on Garet too; the two nod.
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_GREAT_HEALER, 2);
  Engine_EventWait(20);
  Event_SayThenWait(ACTOR_FAREWELL_GREAT_HEALER, 20);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  ValeGate_FaceDirectionAndWait(GARET, FACING_WEST + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  ValeGate_FaceDirectionAndWait(GARET, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);
  // Garet's sister warns them off water that smells funny and hands over a
  // Herb.
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_GARETS_SISTER, 2);
  Engine_EventWait(20);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_SISTER, 10);
  Actor_SetSpeed(ACTOR_FAREWELL_GARETS_SISTER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_FAREWELL_GARETS_SISTER, 216, 800);
  Actor_FaceDirection(ACTOR_FAREWELL_GARETS_SISTER, FACING_SOUTH, 0);
  Engine_PartyGiveItem(ITEM_HERB, 0);
  Actor_WalkToAndWait(ACTOR_FAREWELL_GARETS_SISTER, 264, 800);
  Actor_FaceDirection(ACTOR_FAREWELL_GARETS_SISTER, FACING_SOUTHWEST, 0);
  // Garet protests he knows that, Sis; his brother says he will eat anything,
  // and Garet jumps.
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  ValeGate_FaceDirectionAndWait(GARET, FACING_NORTHEAST + FACING_STEP, 10);
  Event_SayThenWait(GARET, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_BROTHER,
                                  ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_BROTHER, 10);
  Engine_ActorFaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 20);
  ObjectMotion_Launch(GARET, 4, 60);
  // Dora is very late, the bearded man in a long robe says. Everyone wonders;
  // Garet's brother remembers.
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_MAN_AWAITING_DORA, 2);
  Engine_EventWait(20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_MAN_AWAITING_DORA,
                                FACING_NORTH + FACING_STEP, 10);
  Event_SayThenWait(ACTOR_FAREWELL_MAN_AWAITING_DORA, 60);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_FAREWELL_GARETS_SISTER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_FAREWELL_GARETS_BROTHER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_FAREWELL_GARETS_MOTHER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_FAREWELL_GARETS_FATHER, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(ACTOR_FAREWELL_GARETS_BROTHER, EMOTE_IN_FRONT | 0, 0);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_BROTHER, 60);
  // Garet's brother brings the Catch Beads Aunty Dora sent for Isaac; Isaac and
  // Garet turn to follow.
  Object_LinkObjectAndSetCallback(ACTOR_PARTY_LEADER,
                                  ACTOR_FAREWELL_GARETS_BROTHER);
  Object_LinkObjectAndSetCallback(GARET, ACTOR_FAREWELL_GARETS_BROTHER);
  Actor_WalkToAndWait(ACTOR_FAREWELL_GARETS_BROTHER, 216, 800);
  Actor_FaceDirection(ACTOR_FAREWELL_GARETS_BROTHER, FACING_SOUTH, 0);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_BROTHER, 60);
  Engine_PartyGiveItem(ITEM_CATCH_BEADS, 0);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Actor_WalkToAndWait(ACTOR_FAREWELL_GARETS_BROTHER, 272, 816);
  Actor_FaceDirection(ACTOR_FAREWELL_GARETS_BROTHER, FACING_WEST, 0);
  // Garet thinks they are all set; his sister says think again, it must be hard
  // for Dora.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  ValeGate_FaceDirectionAndWait(GARET, FACING_WEST + FACING_STEP, 10);
  Event_SayThenWait(GARET, 10);
  Engine_ActorFaceDirection(ACTOR_FAREWELL_MAN_AWAITING_DORA,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_EAST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_FAREWELL_GARETS_SISTER, 1);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_SISTER, 10);
  ValeGate_FaceDirectionAndWait(GARET, FACING_NORTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_SISTER,
                                  ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_SISTER, 10);
  // Garet's mother knows how Dora feels: she does not want Isaac to go, but
  // knows he must. The crowd nods.
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_NOD);
  Engine_EventShowMessage(ACTOR_FAREWELL_GARETS_MOTHER, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_SHAKE_HEAD);
  Engine_EventShowMessage(ACTOR_FAREWELL_GARETS_MOTHER, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_FAREWELL_GARETS_MOTHER, 3);
  Engine_EventShowMessage(ACTOR_FAREWELL_GARETS_MOTHER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_SISTER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_FATHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_BROTHER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 4, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 7, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 8, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 5, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 1,
                                  ANIM_NOD);
  // Garet's grandfather hops: they will never leave standing around like this,
  // so one final farewell.
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_GRANDFATHER, 2, 10);
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_GRANDFATHER, 4, 40);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_GRANDFATHER, 10);
  Engine_ActorFaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_GARETS_GRANDFATHER,
                                FACING_NORTH + FACING_STEP, 10);
  Event_SayThenWait(ACTOR_FAREWELL_GARETS_GRANDFATHER, 10);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_GARETS_GRANDFATHER,
                                FACING_WEST + FACING_STEP, 20);
  ValeGate_FaceDirectionAndWait(ACTOR_FAREWELL_GARETS_GRANDFATHER,
                                FACING_SOUTH + FACING_STEP, 10);
  // The crowd nods in three waves, then all jump together: "Farewell!"
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_MAN_AWAITING_DORA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_BROTHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 3, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 6, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 9, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 1, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 4, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 7, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GREAT_HEALER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_SISTER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_GARETS_FATHER, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 2, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 5, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 8,
                                  ANIM_NOD);
  Engine_EventWait(80);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CHILD, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_MAN_AWAITING_DORA, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_BROTHER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 3, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 6, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 9, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_SECOND_CHILD, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_GRANDFATHER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_MOTHER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 1, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 4, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 7, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_GREAT_HEALER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_SISTER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_GARETS_FATHER, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 2, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 5, 4, 0);
  ObjectMotion_Launch(ACTOR_FAREWELL_FIRST_CROWD_VILLAGER + 8, 4, 0);
  Engine_MessageShowCentered(MSGNO(MsgValeFarewell), 1);
  priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  Engine_EventWait(80);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= priorityFlags;
  {
    struct FieldActor *actor = Object_GetById(GARET);
    u8 value = (priorityFlags | actor->priorityFlags);

    actor->priorityFlags = value;
  }
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  ValeGate_FaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  ValeGate_FaceDirectionAndWait(GARET, FACING_SOUTH + FACING_STEP, 20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  departureActions = gValeGate_PartyDepartureActions;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, departureActions);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Engine_CameraMoveTo(PIXELS(216), PIXELS(16), PIXELS(905), 1);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetActionScript(GARET, departureActions);
  Engine_EventWait(60);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_EventRequestExit(VALE_GATE_TO_WORLD_MAP);
  Engine_EventEnd();
}

static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

static void ValeGate_FaceDirectionAndWait(s32 actorId, s32 facing, s32 frames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(frames);
}

/// Flashes the caster's palette and emits paired effects every sixteen frames.
static void ValeGate_FlashPsynergyPalette(struct FieldActor *actor) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    ValeGate_SpawnEffectPair(actor);
  }
}

/// Cycles six palette banks and emits the caster's paired effects.
static void
ValeGate_CyclePsynergyPaletteAndSpawnEffects(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    Engine_ObjectSetPartPalettes(actor, palette);
  }
  if ((gFrameCount & 15) == 0) {
    ValeGate_SpawnEffectPair(actor);
  }
}

/// Cycles the levitated actor through six palette banks.
static void ValeGate_CyclePsynergyPalette(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    Engine_ObjectSetPartPalettes(actor, palette);
  }
}

#include "common/effect_vertical.h"

/// Creates the two vertical Psynergy effects using the shared sprite tiles.
static void ValeGate_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct FieldFxScene *work = gEffectWork;
  s32 i;

  Engine_AudioPlayCue(SE_131);
  for (i = 0; i < 2; ++i) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        AnimationObjects_SelectAnimation(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct FieldActor *effect = pair[0];
    struct AnimationObject *effectSprite = effect->sprite;

    effect->update = FieldFx_AnimateVerticalNegative;
    effectSprite->image.priority = 2;
  }
  {
    struct FieldActor *effect = pair[1];
    struct AnimationObject *effectSprite = effect->sprite;

    effectSprite->image.priority = 2;
    effect->update = FieldFx_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

/// Updates the rock-lifting boy's Psynergy display.
///
/// A callback Event_DemonstrateLevitation adds while the boy lifts.
static void ValeGate_UpdateRockLiftingBoyPsynergy(void) {
  ValeGate_CyclePsynergyPaletteAndSpawnEffects(
      Object_GetById(ACTOR_STORM_ROCK_LIFTING_BOY));
}

/// Updates the demonstrating strongman's Psynergy display.
///
/// A callback Event_DemonstratePsynergy adds while the ground shakes.
static void ValeGate_UpdateShowingStrongmanPsynergy(void) {
  ValeGate_CyclePsynergyPaletteAndSpawnEffects(
      Object_GetById(ACTOR_STRONGMAN_SHOWING_PSYNERGY));
}

/// Updates the small rock's palette while it is lifted.
///
/// A callback Event_DemonstrateLevitation adds while ACTOR_STORM_SMALL_ROCK
/// rises.
static void ValeGate_UpdateSmallRockPalette(void) {
  ValeGate_CyclePsynergyPalette(Object_GetById(ACTOR_STORM_SMALL_ROCK));
}

/// Arriving by VALE_GATE_DEBUG_STORY_FLAGS: sets a preset of story flags,
/// which differs by edition, and leaves by VALE_GATE_TO_BILIBIN.
///
/// Called by ValeGate_Initialize. Sets FLAG_NO_ENCOUNTERS and
/// FLAG_DEBUG_BATTLE_KEYS; from the third patch the whole story up to Kalay,
/// before it FLAG_TRET_DEFEATED and FLAG_KOLIMA_CURSE_LIFTED.
static void ValeGate_SetProgressFlagsAndExit(void) {
  Engine_GameFlagSet(FLAG_NO_ENCOUNTERS);
  Engine_GameFlagSet(FLAG_DEBUG_BATTLE_KEYS);
#if TBS_PATCH_3
  Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_THIRD_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
  Engine_GameFlagSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  Engine_GameFlagSet(FLAG_VALE_STORM_HEARD_GARET);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_JOINED);
  Engine_GameFlagSet(FLAG_VALE_BOULDER_FELL);
  Engine_GameFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
  Engine_GameFlagSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
  Engine_GameFlagSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
  Engine_GameFlagSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
  Engine_GameFlagSet(FLAG_VALE_STORM_GARET_LEFT);
  Engine_GameFlagSet(FLAG_VALE_ASKED_IF_GOING_TO_KRADEN);
  Engine_GameFlagSet(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
  Engine_GameFlagSet(FLAG_MET_SATUROS_AND_MENARDI);
  Engine_GameFlagSet(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
  Engine_GameFlagSet(FLAG_SANCTUM_VISIT_PLANNED);
  Engine_GameFlagSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_ENTERED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED);
  Engine_GameFlagSet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FOUND);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_ERUPTED);
  Engine_GameFlagSet(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
  Engine_GameFlagSet(FLAG_PARTY_LEFT_VALE);
  Engine_GameFlagSet(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  Engine_GameFlagSet(FLAG_GARET_CAME_HOME);
  Engine_GameFlagSet(FLAG_REACHED_KALAY);
#else
  Engine_GameFlagSet(FLAG_TRET_DEFEATED);
  Engine_GameFlagSet(FLAG_KOLIMA_CURSE_LIFTED);
#endif
  Engine_EventRequestExit(VALE_GATE_TO_BILIBIN);
}

// clang-format off
u32 gValeGate_ChargedUpStrongmanActionsHead[] = {
    0x1000000, 0x2020101, 0x3030302, 0x5040404, 0x6060505, 0x1000006, 0x3020201, 0x5040403,
};

s32 gValeGate_ChargedUpStrongmanActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(214), 0, PIXELS(721)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(40),
    SC_SE(153),
    SC_SET(SW_MOTION, 3),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_ARG(2),
    SC_MOVE(PIXELS(214), 0, PIXELS(763)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeGate_SecondTravelerLeapActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(12)),
    SC_MOVE(PIXELS(114), PIXELS(144), PIXELS(360)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeGate_FirstTravelerLeapActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(12)),
    SC_MOVE(PIXELS(114), PIXELS(144), PIXELS(343)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeGate_LeaderActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(82), PIXELS(144), PIXELS(343)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeGate_StormGaretLeapActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(82), PIXELS(144), PIXELS(360)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gValeGate_FirstTravelerBounceActions[] = {
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_MOVE(PIXELS(65), 0, PIXELS(328)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_MOVE(PIXELS(31), 0, PIXELS(338)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(0, 0, PIXELS(338)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeGate_SecondTravelerBounceActions[] = {
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_MOVE(PIXELS(63), 0, PIXELS(374)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SET(SW_VELOCITY_Y, PIXELS(7)),
    SC_MOVE(PIXELS(17), 0, PIXELS(338)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ARG(5),
    SC_WAIT(10),
    SC_ARG(6),
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(0, 0, PIXELS(338)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeGate_FaceTargetActions[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeGate_FarewellDogActions[] = {
    SC_MOVE(PIXELS(132), 0, PIXELS(636)),
    SC_SYNC,
    SC_MOVE(PIXELS(171), 0, PIXELS(629)),
    SC_SYNC,
    SC_MOVE(PIXELS(207), 0, PIXELS(627)),
    SC_SYNC,
    SC_MOVE(PIXELS(235), 0, PIXELS(625)),
    SC_SYNC,
    SC_MOVE(PIXELS(261), 0, PIXELS(574)),
    SC_SYNC,
    SC_MOVE(PIXELS(327), 0, PIXELS(547)),
    SC_SYNC,
    SC_END,
};

s32 gValeGate_FarewellChaseActions[] = {
    SC_MOVE(PIXELS(271), 0, PIXELS(571)),
    SC_SYNC,
    SC_MOVE(PIXELS(187), 0, PIXELS(577)),
    SC_SYNC,
    SC_MOVE(PIXELS(154), 0, PIXELS(580)),
    SC_SYNC,
    SC_MOVE(PIXELS(127), 0, PIXELS(543)),
    SC_SYNC,
    SC_MOVE(PIXELS(139), 0, PIXELS(498)),
    SC_SYNC,
    SC_END,
};

s32 gValeGate_FarewellFirstChildActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(137), 0, PIXELS(639)),
    SC_SYNC,
    SC_MOVE(PIXELS(122), 0, PIXELS(692)),
    SC_SYNC,
    SC_MOVE(PIXELS(141), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(6) / 5),
    SC_SET(SW_ACCEL, PIXELS(3) / 5),
    SC_MOVE(PIXELS(148), 0, PIXELS(770)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeGate_FarewellSecondChildActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(137), 0, PIXELS(639)),
    SC_SYNC,
    SC_MOVE(PIXELS(122), 0, PIXELS(692)),
    SC_SYNC,
    SC_MOVE(PIXELS(141), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(6) / 5),
    SC_SET(SW_ACCEL, PIXELS(3) / 5),
    SC_MOVE(PIXELS(164), 0, PIXELS(758)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeGate_PartyDepartureActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(854)),
    SC_SYNC,
    SC_MOVE(PIXELS(215), 0, PIXELS(921)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeGate_HutOwnerSearchActions[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 20),
    SC_SET(SW_COLLISION, 3),
    SC_LABEL(1),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gValeGate_AideRunActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(148), 0, PIXELS(758)),
    SC_SYNC,
    SC_MOVE(PIXELS(128), 0, PIXELS(709)),
    SC_SYNC,
    SC_MOVE(PIXELS(129), 0, PIXELS(647)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeGate_LeaderStepActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(774)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gValeGate_StormGaretStepActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(790)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

struct SceneEntrance gValeGate_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_SHOPS_NORTH, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_INN_NORTH, CONDITION_ALWAYS, 352, 0, 308, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_SHOPS_WEST, CONDITION_ALWAYS, 230, 0, 423, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_SHOPS_EAST, CONDITION_ALWAYS, 374, 0, 435, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_SHOPS_SOUTH, CONDITION_ALWAYS, 200, 0, 562, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_INN_WEST, CONDITION_ALWAYS, 102, 0, 627, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE_INTERIORS_INN_SOUTH, CONDITION_ALWAYS, 358, 0, 678, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_VALE, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_WORLD_MAP, CONDITION_ALWAYS, 217, 0, 843, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 186, 0, 474, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_FROM_SANCTUM, CONDITION_ALWAYS, 180, 0, 618, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_GOMA_CAVE_STREAM_UNUSED, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_GATE_AFTER_TRAVELERS_BATTLE, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gValeGate_Exits[] = {
    SCENE_EXITS(ValeGate),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_EAST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_NORTH, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHEAST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_NORTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_SOUTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_WEST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_NORTHWEST, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHWEST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_SOUTH, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_WEST),
    SCENE_EXIT(VALE_GATE_TO_VALE, Vale, VALE_FROM_VALE_GATE),
    SCENE_EXIT(VALE_GATE_TO_TITLE, Title, TITLE_FROM_VALE_GATE),
    SCENE_EXIT(VALE_GATE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_VALE_GATE),
    SCENE_EXIT(VALE_GATE_TO_GOMA_CAVE_STREAM, GomaCaveStream, GOMA_CAVE_STREAM_FROM_VALE_GATE),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST),
#if !TBS_PATCH_3
    SCENE_EXIT(VALE_GATE_TO_BILIBIN, Bilibin, BILIBIN_FROM_WORLD_MAP),
#else
    SCENE_EXIT(VALE_GATE_TO_BILIBIN, SolSanctumStatue, SOL_SANCTUM_STATUE_FROM_VALE_GATE),
#endif
    SCENE_EXITS_END,
};

u32 gValeGate_StormExits[] = {
    SCENE_EXITS(ValeGate),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_EAST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_NORTH, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHEAST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_NORTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_SOUTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_SHOPS_WEST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_NORTHWEST, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHWEST),
    SCENE_EXIT(VALE_GATE_TO_VALE_INTERIORS_INN_SOUTH, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_GATE_WEST),
    SCENE_EXIT(VALE_GATE_TO_VALE, ValeStorm, VALE_STORM_FROM_VALE_GATE),
    SCENE_EXIT(VALE_GATE_TO_TITLE, Title, TITLE_FROM_VALE_GATE),
    SCENE_EXITS_END,
};

struct ScenePlacement gValeGate_Placements[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, 3,
        PIXELS(392), 0, PIXELS(712), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(616), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(760), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(600), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(824), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(568), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHWEST, 3, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(760), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(680), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(370), 0, PIXELS(319), FACING_WEST, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA_CHILD, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeGate_StormPlacements[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, 3,
        PIXELS(249), 0, PIXELS(628), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(214), 0, PIXELS(635), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(294), 0, PIXELS(670), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, 3,
        PIXELS(136), 0, PIXELS(678), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(87), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(173), 0, PIXELS(568), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(213), 0, PIXELS(771), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
#if TBS_PATCH_4
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(198), 0, PIXELS(779), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
#else
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(204), 0, PIXELS(783), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
#endif
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(250), 0, PIXELS(773), FACING_SOUTHWEST, 3, 0 },
    { 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(729), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(729), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(215), 0, PIXELS(709), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET_CHILD, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA_CHILD, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(786), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeGate_DeparturePlacements[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(225), 0, PIXELS(814), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(607), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(607), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(199), 0, PIXELS(781), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(180), 0, PIXELS(790), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(778), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(799), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(270), 0, PIXELS(814), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(787), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(781), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(742), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(286), 0, PIXELS(761), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(271), 0, PIXELS(778), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(762), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(227), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(209), 0, PIXELS(760), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(288), 0, PIXELS(795), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(161), 0, PIXELS(790), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(171), 0, PIXELS(815), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(191), 0, PIXELS(758), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_CAT, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeGate_PlacementsAfterKalay[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(217), 0, PIXELS(804), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(107), 0, PIXELS(765), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(267), 0, PIXELS(744), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(681), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, 3,
        PIXELS(391), 0, PIXELS(716), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(617), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(166), 0, PIXELS(603), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(568), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(315), 0, PIXELS(823), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(287), 0, PIXELS(571), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(370), 0, PIXELS(319), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeGate_Events[] = {
    { EVENT_TALK, ACTOR_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeQuietDay) },
    { EVENT_TALK, ACTOR_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeKradenHadVisitors) },
    { EVENT_TALK, ACTOR_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutMeditation) },
    { EVENT_TALK, ACTOR_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RecallThreeYearsAgo) },
    { EVENT_TALK, ACTOR_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeChosenPeople) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeProudOfAdeptGrandfather) },
    { EVENT_TALK, ACTOR_FIRST_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutTheTravelers) },
    { EVENT_TALK, ACTOR_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValePracticePsynergy) },
    { EVENT_TALK, ACTOR_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DemonstratePsynergy) },
    { EVENT_TALK, ACTOR_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeWalkToRecoverPsynergy) },
    { EVENT_TALK, ACTOR_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeFoodStoredForWinter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughTownGate) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VALE_GATE_TO_VALE },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, VALE_GATE_TO_GOMA_CAVE_STREAM },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, (FLAG_VALE_FIRST_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwayFirstIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_IVY, (FLAG_VALE_SECOND_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwaySecondIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THIRD_IVY, (FLAG_VALE_THIRD_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwayThirdIvy) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeGate_StormEvents[] = {
    { EVENT_TALK, ACTOR_STORM_CURIOUS_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJourney) },
    { EVENT_TALK, ACTOR_STORM_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeOldWomanCannotUsePsynergy) },
    { EVENT_TALK, ACTOR_STORM_ROCK_DEFLECTING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgValeExhaustedFromFallingRocks) },
    { EVENT_TALK, ACTOR_STORM_WOMAN_HEARING_OF_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixFellInRiver) },
    { EVENT_TALK, ACTOR_STORM_ROCK_LIFTING_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DemonstrateLevitation) },
    { EVENT_TALK, ACTOR_STORM_GIRL_WHO_SAW_JENNA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutHelping) },
    { EVENT_TALK, ACTOR_STORM_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeRaceAgainstTime) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughWestPassage) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SentToAidElders) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeGate_DepartureEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeGate_EventsAfterLeaving[] = {
    { EVENT_TALK, ACTOR_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeTremorsSettled) },
    { EVENT_TALK, ACTOR_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeRoadToVault) },
    { EVENT_TALK, ACTOR_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, MSGNO(MsgValeRoadToGomaRange) },
    { EVENT_TALK, ACTOR_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, MSGNO(MsgValePsynergyStonePowder) },
    { EVENT_TALK, ACTOR_CURIOUS_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJourney) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeLunpaFoundedNorthernTown) },
    { EVENT_TALK, ACTOR_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeDoraHasBeenCrying) },
    { EVENT_TALK, ACTOR_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeStonesScatteredByEruption) },
    { EVENT_TALK, ACTOR_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DemonstratePsynergy) },
    { EVENT_TALK, ACTOR_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeReplenishPsynergyBeforeDungeon) },
    { EVENT_TALK, ACTOR_HUT_OWNER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutFallenStone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeWorriedAboutAnotherEruption) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeVaultUndergroundPath) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, MSGNO(MsgValeGomaWinterCold) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, MSGNO(MsgValeStonesPowerAleph) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeProofOfWiseOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeLunpaThievesChanged) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeDoraMustBeLonely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeLuckyToFindPsynergyStones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAlive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeRecoverPsynergyByWalking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeStonesReplenishPsynergy) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughTownGate) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VALE_GATE_TO_VALE },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, VALE_GATE_TO_GOMA_CAVE_STREAM },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, (FLAG_VALE_FIRST_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwayFirstIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_IVY, (FLAG_VALE_SECOND_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwaySecondIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THIRD_IVY, (FLAG_VALE_THIRD_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwayThirdIvy) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeGate_EventsAfterKalay[] = {
    { EVENT_TALK, ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WelcomeIsaacHome) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_WOMAN_BACK_SOONER, CONDITION_ALWAYS, MSGNO(MsgValeCameBackSooner) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_MAN_ON_NEW_FRIENDS, CONDITION_ALWAYS, MSGNO(MsgValeMadeFriendsOnJourney) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_WOMAN_ON_HOT_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeRiverHeatedAndCooled) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutJourney) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeReturnHowHaveYouBeen) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHome) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWantToMakeFriends) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_STRONGMAN_ON_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeEruptionMadeMess) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_STRONGMAN_ON_CRACK, CONDITION_ALWAYS, MSGNO(MsgValeNorthwestCrack) },
    { EVENT_TALK, ACTOR_AFTER_KALAY_HUT_OWNER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PsynergyStoneIsGone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgValeSurprisedByEarlyReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_WOMAN_BACK_SOONER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacHasGrown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_MAN_ON_NEW_FRIENDS, CONDITION_ALWAYS, MSGNO(MsgValeNewFriendsAreAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_WOMAN_ON_HOT_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeEruptionHeatsRiver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeStrangeEventsElsewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeWhenDidYouReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValePlayTogetherAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWantToJourney) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_STRONGMAN_ON_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeLavaBurntOtherSide) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_STRONGMAN_ON_CRACK, CONDITION_ALWAYS, MSGNO(MsgValeCrackWidening) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_AFTER_KALAY_HUT_OWNER, FLAG_VALE_PSYNERGY_STONE_GONE_TOLD, EVENT_SCRIPT(Event_PsynergyStoneIsGone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeEruptionTooMuchForPeople) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNorthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenNortheastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralWestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenCentralEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSouthwestDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestEdgeDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSoutheastDoor) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughTownGate) },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VALE_GATE_TO_VALE },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, VALE_GATE_TO_GOMA_CAVE_STREAM },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, (FLAG_VALE_FIRST_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwayFirstIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_IVY, (FLAG_VALE_SECOND_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwaySecondIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THIRD_IVY, (FLAG_VALE_THIRD_IVY_BLOWN_AWAY), EVENT_SCRIPT(Event_BlowAwayThirdIvy) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UsePsynergyStone) },
    { SCENE_EVENTS_END },
};

u16 gValeGate_DoorOpens[] = {
    MAP_CELL_STEP(0, 57, 2, 2, 1), MAP_CELL_STEP(0, 59, 2, 2, 1), MAP_CELL_STEP(2, 59, 2, 2, 1), MAP_CELL_STEPS_END,
};
// clang-format on
