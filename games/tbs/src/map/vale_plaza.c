/// @file
/// Vale (MapId_ValePlaza): the part of the village with the Psynergy Stone and
/// the town gate, Vale's way out to the world. The map sees the prologue end
/// and the journey begin, and ValePlaza_GetObjectEventTemplates and
/// ValePlaza_GetEvents pick its people and events by the time of the story. In
/// story order:
///
/// - The stormy night of the prologue (FLAG_VALE_STORM), when the villagers
///   have gathered in the plaza here. Isaac and Garet come from the river
///   (Vale, MapId_ValeStormyNight, by VALE_STORMY_NIGHT_TO_VALE_PLAZA), where
///   Felix has fallen in. Walking up to the villagers (metatile behavior 20)
///   plays EventScript_SentToAidElders: Garet's grandfather, the mayor, sends
///   two strongmen to aid the elders holding the Boulder, then sends Isaac and
///   Garet back to show Jenna and a strongman whose Psynergy the Psynergy Stone
///   has restored the way to the river (FLAG_VALE_STORM_SENT_TO_AID_ELDERS).
///   Others have spent their Psynergy, and one lifts a small rock to show he
///   has a little left (EventScript_BoyLiftsSmallRock). Later, when the Boulder
///   has swept Kyle, Jenna's parents and Felix away and Garet has gone after
///   Isaac (FLAG_VALE_STORM_GARET_LEFT), arriving here plays
///   EventScript_MeetSurvivingTravelers: Isaac overhears two strangers saying
///   only the two of them survived Sol Sanctum's fury, Garet catches up, and
///   the strangers fight the boys to make them forget
///   (ValePlaza_FightTravelers). Win or lose, the battle returns to
///   VALE_PLAZA_AFTER_TRAVELERS_BATTLE, where the strangers leave the boys
///   lying in the storm, the storm ends (FLAG_VALE_STORM is cleared) and
///   VALE_PLAZA_TO_TITLE leads to the title shown after the prologue, which
///   opens on Vale three years later (menu/title.c). The game names the two
///   only three years later, as Saturos and Menardi (vale_kradens_cottage.c);
///   their sprites here are the same.
/// - Three years later, the day the story begins. The villagers talk of
///   meditating on Mt. Aleph, of the travelers who stopped by, and of the
///   storm, in which Isaac lost his father and Jenna her whole family; one
///   shows off his Psynergy, shaking the ground
///   (EventScript_DemonstratePsynergy).
/// - The farewell (VALE_PLAZA_FROM_SANCTUM), once Isaac and Garet have
///   accepted the Wise One's quest in the Vale Sanctum (sanctum.c).
///   EventScript_DepartForQuest restores the party's HP and PP and plays the
///   village seeing them off at the town gate: Garet's sister gives a Herb, and
///   his brother the Catch Beads that Aunty Dora sent, for Dora herself does
///   not come. VALE_PLAZA_TO_WORLD_MAP leads out to the world map.
/// - After the party has left (FLAG_VALE_PARTY_LEFT), the villagers point the
///   way to Vault and talk of the eruption, whose sparkles they take for
///   crushed Psynergy Stones; a large Psynergy Stone has fallen on the hut,
///   whose roof ValePlaza_Initialize redraws. Only now does the town gate lead
///   out (EventScript_LeaveThroughTownGate).
/// - Back from Kalay (FLAG_KALAY_REACHED), the villagers welcome Isaac home,
///   the hut's owner finds the stone gone, leaving a hole in his roof
///   (EventScript_PsynergyStoneIsGone), and a door in the north-east opens.
///
/// The Psynergy Stone (a bg event on metatile behavior 100) restores the
/// party's PP at every time but the farewell. The ivy in three places
/// (LOCALID_FIRST_IVY to LOCALID_THIRD_IVY) can be blown away with Whirlwind,
/// each one's flag keeping it gone. The doors lead into the houses
/// (MapId_ValeInteriors) and into the inn and the forge
/// (MapId_WoodenRoomsA), the path west to the village (MapId_Vale) or, on
/// the stormy night, back to the storm's map (MapId_ValeStormyNight), and the
/// warp row of metatile behavior 11 to VALE_PLAZA_TO_GOMA_CAVE_STREAM, which
/// the warp table sends to Goma Cave (MapId_GomaCaveStream,
/// GOMA_CAVE_STREAM_FROM_VALE_PLAZA). Behavior 11 lies on one cell, (11, 28),
/// in every state of the map, the cell north of where the party arrives from
/// Goma Cave (VALE_PLAZA_FROM_GOMA_CAVE_STREAM, at (186, 474)), so that way
/// back is the way in (render probe vale-gate).
///
/// Three debug warp destinations are in no warp destination table and no warp
/// leads to them: VALE_PLAZA_DEBUG_BEFORE_TRAVELERS_LEAVE replays the
/// strangers' departure, VALE_PLAZA_DEBUG_CHILDHOOD sets FLAG_CHILDHOOD and
/// takes VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTHEAST into Isaac's house
/// (MapId_WoodenRoomsA), and VALE_PLAZA_DEBUG_STORY_FLAGS sets a preset of
/// story flags and takes VALE_PLAZA_TO_BILIBIN
/// (ValePlaza_SetProgressFlagsAndExit).
///
/// The hut's owner (LOCALID_HUT_OWNER) is never seen by day after the prologue:
/// ValePlaza_Initialize hides the storm's Jenna by number without testing
/// FLAG_VALE_STORM, and by day that number is his (a BUG, fixed by BUGFIX).
///
/// A villager showing Psynergy (EventScript_DemonstratePsynergy by day,
/// EventScript_BoyLiftsSmallRock on the stormy night) flashes white
/// (ValePlaza_FlashPsynergyPalette), then cycles through his colour modes while
/// pairs of sparks rise and fall about him every 16 frames
/// (ValePlaza_CyclePsynergyPaletteAndSpawnEffects)
/// (https://alchemy.pascalpixel.com/places/vale-gate/psynergy-display.gif: the
/// strongman's display by day, the screen with him boxed beside the sprites
/// alone and the 16 sprite palettes).

#include "save_block.h"
#include "flags.h"
#include "character_ids.h"
#include "version.h"
#include "map_ids.h"
#include "vram_cache.h"
#include "unit/map/vale_plaza.h"
#include "unit/map/common/effect_psynergy.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
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
#include "heap_slots.h"
#include "vale_plaza.h"
#include "map_grid_scroll.h"
#include "text/msg_ids.h"
#include "script_layer.h"
#include "field_service.h"
#include "types.h"
#include "animspr.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "objcmd.h"
#include "object_event.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

// Vale's map section flags, kept while the party stays in Vale's maps.
/// Back from Kalay, a young man has welcomed Isaac home.
/// ★ Named from the game: "You came back home, huh?" (MsgValeYouCameBackHome)
#define FLAG_VALE_WELCOMED_HOME 0x302
/// Back from Kalay, the old man searching for the Psynergy Stone has told
/// Isaac it is gone.
/// ★ Named from the game: "Where'd that Psynergy Stone go? It's gone!"
/// (MsgValeThePsynergyStoneIsGone)
#define FLAG_VALE_PSYNERGY_STONE_GONE_TOLD 0x306

/// Each object event template table numbers its own rows from
/// LOCALID_FIRST_PLACED, Garet keeping his own party slot, and the same people
/// stand in several tables under other numbers. The three ivy object events
/// come first in every table.
enum ValePlazaLocalId {
  /// Ivy, drawn half as tall again: the sprite of the ivy that holds the stump
  /// in goma_cave_entrance.c ("It looks like it's trapped in the ivy.").
  /// ★ Named from what the player sees: a clump of green ivy against the cliff
  /// (its render)
  LOCALID_FIRST_IVY = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a clump of green ivy against the cliff
  /// (its render)
  LOCALID_SECOND_IVY,
  /// ★ Named from what the player sees: a clump of green ivy against the cliff
  /// (its render)
  LOCALID_THIRD_IVY
};

/// By day (gValePlaza_ObjectEventTemplates, before and after the party leaves).
/// The notes quote the day the story begins.
enum ValePlazaDayLocalId {
  /// An old man with a cane: "The Valeans are a chosen people." On the stormy
  /// night, after the party has left and back from Kalay, a question for Isaac
  /// instead (EventScript_AskAboutJourney); after the party has left, "You saw
  /// the
  /// Wise One?"
  /// ★ Named from the game: "The Valeans are a chosen people."
  /// (MsgValeChosenPeople)
  LOCALID_CURIOUS_OLD_MAN = LOCALID_FIRST_PLACED + 3,
  /// In the old woman's sprite, who on the stormy night asks "You expect an old
  /// woman like me to use Psynergy?" By day: "Long ago, my grandfather was a
  /// powerful Adept... I'm so proud of him."
  /// ★ Named from the game: "You expect an old woman like me to use
  /// Psynergy?" (MsgValeOldWomanCannotUsePsynergy)
  LOCALID_OLD_WOMAN,
  /// A young man with brown hair: "Well, if it isn't Isaac. It's just another
  /// quiet day in Vale."
  /// ★ Named from the game: "It's just another quiet day in Vale."
  /// (MsgValeQuietDay)
  LOCALID_YOUNG_MAN_QUIET_DAY,
  /// A woman with fair hair: "Kraden had visitors when I dropped by his place
  /// this morning."
  /// ★ Named from the game: "Kraden had visitors when I dropped by his place
  /// this morning." (MsgValeKradenHadVisitors)
  LOCALID_WOMAN_ON_KRADENS_VISITORS,
  /// Two children, a small boy with fair hair and a little girl with orange
  /// pigtails; back from Kalay the boy thinks "I wonder if we can play
  /// with Isaac and his pals again, like we used to." By day: "Did the
  /// travelers that stopped by today go to meet the mayor?"
  /// ★ Named from what the player sees: a small boy with fair hair in a blue
  /// shirt (its render)
  LOCALID_FIRST_CHILD,
  /// The little girl: "With Psynergy, the more you practice, the more you
  /// learn."
  /// ★ Named from what the player sees: a little girl with orange pigtails in
  /// a green dress (its render)
  LOCALID_SECOND_CHILD,
  /// A strongman who shakes the ground with his Psynergy
  /// (EventScript_DemonstratePsynergy). "Hey,
  /// Isaac, have I shown you my new ability?"
  /// ★ Named from the game: "have I shown you my new ability?"
  /// (MsgValeShownNewAbility)
  LOCALID_STRONGMAN_SHOWING_PSYNERGY,
  /// A second strongman: "If your Psynergy runs low, just walk around. You'll
  /// recover it slowly."
  /// ★ Named from the game: "If your Psynergy runs low, just walk around."
  /// (MsgValeWalkToRecoverPsynergy)
  LOCALID_STRONGMAN_WALK_TO_RECOVER,
  /// An invisible object event (as in gondowan_cave.c), out of sight until the
  /// party has left; then it stands on the large Psynergy Stone that fell on
  /// the hut, for the hut's owner to turn to.
  /// ★ Named from the game: "That large Psynergy Stone fell right on top of the
  /// hut." (MsgValeTheStoneFellOnThe)
  LOCALID_FALLEN_STONE_SPOT = LOCALID_FIRST_PLACED + 13,
  /// A burly grey-haired man: "Isaac! Have you remembered to meditate on Mt.
  /// Aleph daily?"
  /// ★ Named from the game: "Have you remembered to meditate on Mt. Aleph
  /// daily?" (MsgValeMeditateOnMtAlephDaily)
  LOCALID_MAN_ASKING_ABOUT_MEDITATION = LOCALID_FIRST_PLACED + 15,
  /// A woman in a green dress: "You had a very difficult time three years ago,
  /// didn't you?"
  /// ★ Named from the game: "You had a very difficult time three years ago"
  /// (MsgValeADifficultTimeThreeYears)
  LOCALID_WOMAN_RECALLING_STORM,
  /// The hut's owner, an old man with a cane, who back from Kalay finds "a big
  /// hole in my roof". "Since food is stored in this hut, it isn't scary at
  /// all..." Once the prologue is over, ValePlaza_Initialize hides him by day
  /// as if he were the storm's LOCALID_STORM_JENNA (a BUG), so only a debug
  /// jump
  /// past the prologue shows him; the renders show him by the hut with no
  /// story flags set and gone once the party has left.
  /// ★ Named from the game: "Since food is stored in this hut"
  /// (MsgValeFoodStoredForWinter)
  LOCALID_HUT_OWNER = LOCALID_FIRST_PLACED + 18
};

/// On the stormy night (gValePlaza_StormObjectEventTemplates).
enum ValePlazaStormLocalId {
  /// The same old man as LOCALID_CURIOUS_OLD_MAN: "Huh? Are you...asking me
  /// if I can use Psynergy, Isaac?"
  /// ★ Named from the game: "Did you see anything interesting on your trip"
  /// (MsgValeAnythingInterestingOnYourTrip)
  LOCALID_STORM_CURIOUS_OLD_MAN = LOCALID_FIRST_PLACED + 3,
  /// The old woman: "You expect an old woman like me to use Psynergy?"
  /// ★ Named from the game: "You expect an old woman like me to use
  /// Psynergy?" (MsgValeOldWomanCannotUsePsynergy)
  LOCALID_STORM_OLD_WOMAN,
  /// A young man with brown hair: "I'm exhausted. I used up all my Psynergy
  /// deflecting falling rocks..."
  /// ★ Named from the game: "I used up all my Psynergy deflecting falling
  /// rocks..." (MsgValeExhaustedFromFallingRocks)
  LOCALID_STORM_ROCK_DEFLECTING_YOUNG_MAN,
  /// A woman with fair hair: "Oh! Little Felix has fallen into the river? That
  /// must be why Jenna looked so upset."
  /// ★ Named from the game: "has fallen into the river?"
  /// (MsgValeFelixFellInRiver)
  LOCALID_STORM_WOMAN_HEARING_OF_FELIX,
  /// A small boy with fair hair who lifts LOCALID_STORM_SMALL_ROCK
  /// (EventScript_BoyLiftsSmallRock). "I have some Psynergy left."
  /// ★ Named from what the player sees: a small boy with fair hair holds a
  /// small rock up in the air with his Psynergy, "See?" (its render)
  LOCALID_STORM_ROCK_LIFTING_BOY,
  /// A little girl with orange pigtails: "Jenna went off that way!"
  /// ★ Named from the game: "went off that way!" (MsgValeWentOffWay)
  LOCALID_STORM_GIRL_WHO_SAW_JENNA,
  /// The two strongmen Garet's grandfather sends to aid the elders. "We can't
  /// let the Mt. Aleph Boulder destroy our village."
  /// ★ Named from the game: "destroy our village" (MsgValeGoAidElders)
  LOCALID_STORM_FIRST_AIDE,
  /// ★ Named from the game: "help the elders" (MsgValeGoAidElders)
  LOCALID_STORM_SECOND_AIDE, ///< "Let's go help the elders."
  /// Garet's grandfather, the mayor, an old man with white hair and a cane
  /// ("Grandpa!", Garet calls him; his sprite is vale_interiors.c's). "Go
  /// aid the elders!"
  /// ★ Named from the game: "aid the elders!" (MsgValeGoAidElders)
  LOCALID_STORM_GARETS_GRANDFATHER,
  /// The storm's rock sprite at three tenths of its size.
  /// ★ Named from what the player sees: a small rock floating beside the
  /// boy who lifts it (its render)
  LOCALID_STORM_SMALL_ROCK,
  /// The small rock's shadow: a blank object event on
  /// LOCALID_STORM_SMALL_ROCK's spot at three fifths of its size, whose sprite
  /// draws only its shadow, while
  /// the setup turns off the rock's own. It blocks nothing the rock does not
  /// already block, and nothing moves it, so the shadow stays on the ground as
  /// the boy lifts the rock (render probe vale-gate: the sprites with it and
  /// without it).
  /// ★ Named from what the player sees: the shadow under the floating rock,
  /// render probe vale-gate (its render)
  LOCALID_STORM_SMALL_ROCK_SHADOW,
  /// Rested at the Psynergy Stone, the strongman vale_stormy_night.c calls
  /// LOCALID_EXHAUSTED_STRONGMAN. "Great! I'm all charged up!"
  /// ★ Named from the game: "all charged up" (MsgValeGoAidElders)
  LOCALID_STORM_CHARGED_UP_STRONGMAN,
  /// The two travelers who survived Sol Sanctum, Saturos and Menardi by their
  /// sprites (as in vale_kradens_cottage.c). "Only the two of us survived..."
  /// ★ Named from the game: "Only the two of us survived"
  /// (MsgValeOnlyTwoSurvived)
  LOCALID_FIRST_TRAVELER,
  /// ★ Named from the game: "Only the two of us survived"
  /// (MsgValeOnlyTwoSurvived)
  LOCALID_SECOND_TRAVELER,
  /// Garet as a child, who follows Isaac through the night. "Isaac! Wait!"
  /// ★ Named from the game: "Garet (child)" (MsgDebugGlyphName)
  LOCALID_STORM_GARET,
  /// ★ Named from the game: "Jenna" (MsgCharacterName)
  LOCALID_STORM_JENNA ///< "Garet, Isaac... You came to help me?"
};

/// At the farewell (gValePlaza_DepartureObjectEventTemplates).
enum ValePlazaFarewellLocalId {
  /// The small boy of LOCALID_FIRST_CHILD, at the top of the path. "What,
  /// didn't you hear? Isaac and Garet are leaving Vale on a journey."
  /// ★ Named from the game: "are leaving Vale on a journey"
  /// (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_FIRST_CHILD = LOCALID_FIRST_PLACED + 3,
  /// The little girl of LOCALID_SECOND_CHILD: "Why's everyone hanging around
  /// the town gate?"
  /// ★ Named from the game: "Why's everyone hanging around the town gate?"
  /// (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_SECOND_CHILD,
  /// The Great Healer, by his sprite (as in vale_eruption.c and sanctum.c). "We
  /// are counting on you, Isaac. Oh, and you, too, Garet."
  /// ★ Named from the game: "What is it, Great Healer?"
  /// (MsgSanctumWeHadIdeaTrueSanctum), said to the same sprite in the Sanctum
  LOCALID_FAREWELL_GREAT_HEALER,
  /// The man with long brown hair and a beard in a long robe of vale.c's
  /// LOCALID_MAN_GUARDING_MT_ALEPH: "Hmm... Dora is very late..."
  /// ★ Named from the game: "Dora is very late" (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_MAN_AWAITING_DORA,
  /// Garet's grandfather, the old man with white hair and a cane. "They can't
  /// keep standing around here like this, or they'll never leave."
  /// ★ Named from the game: "keep standing around here"
  /// (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_GARETS_GRANDFATHER,
  /// Garet's sister, the young woman with an orange ponytail, who gives the
  /// Herb. "I know not to drink stagnant water, Sis!", Garet answers her.
  /// ★ Named from the game: "stagnant water" (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_GARETS_SISTER,
  /// Garet's brother, the small boy with orange hair of vale.c's
  /// LOCALID_GARETS_BROTHER, who brings the Catch Beads: "Aunty Dora asked me
  /// to
  /// give this to you, Isaac." Of Garet: "He's a pig! He'll eat anything that's
  /// not tied down!"
  /// ★ Named from the game: "Aunty Dora asked me" (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_GARETS_BROTHER,
  /// A woman with orange hair under a green headscarf: "As Garet's mother, I
  /// know how Dora must be feeling."
  /// ★ Named from the game: "I know how Dora must be feeling"
  /// (MsgValeWhysEveryoneHanging)
  LOCALID_FAREWELL_GARETS_MOTHER,
  /// Garet's father, a man with brown hair in a teal shirt, by his sprite
  /// (vale_interiors.c's).
  /// ★ Named from the game: "My own son" (MsgValeImProudOfYouGaretMy), what the
  /// same sprite says in vale_interiors.c
  LOCALID_FAREWELL_GARETS_FATHER,
  /// The rest of the crowd at the gate, ten who only nod and jump: an old man
  /// with a cane, an old woman, a young man with brown hair, a woman with fair
  /// hair, three strongmen, a burly grey-haired man, a woman in a green dress
  /// and a stout man.
  /// ★ Named from what the player sees: men, women and a young man of Vale in
  /// their sprites (its render)
  LOCALID_FAREWELL_FIRST_CROWD_VILLAGER,
  /// The sprite of the dog of vault.c ("Woof! Woof!"), which runs past the
  /// children and later after LOCALID_FAREWELL_CAT.
  /// ★ Named from the game: "Woof! Woof!" (MsgValeRrruffRrrruff)
  LOCALID_FAREWELL_DOG = LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 10,
  /// A small cat set down where the dog stops, which runs off with the dog
  /// after it. No other map places its sprite.
  /// ★ Named from what the player sees: a small white and grey cat (its
  /// render)
  LOCALID_FAREWELL_CAT
};

/// Back from Kalay (gValePlaza_ObjectEventTemplatesAfterKalay).
enum ValePlazaAfterKalayLocalId {
  /// A young man with brown hair: "Hey, if it isn't Isaac! You came back home,
  /// huh?"
  /// ★ Named from the game: "You came back home, huh?" (MsgValeYouCameBackHome)
  LOCALID_AFTER_KALAY_WELCOMING_YOUNG_MAN = LOCALID_FIRST_PLACED + 3,
  /// A woman with fair hair: "Oh, Isaac! You came back sooner than I thought!"
  /// ★ Named from the game: "You came back sooner than I thought!"
  /// (MsgValeCameBackSooner)
  LOCALID_AFTER_KALAY_WOMAN_BACK_SOONER,
  /// A burly grey-haired man: "It looks like you made some friends in your
  /// travels, Isaac."
  /// ★ Named from the game: "made some friends in your travels"
  /// (MsgValeMadeFriendsOnJourney)
  LOCALID_AFTER_KALAY_MAN_ON_NEW_FRIENDS,
  /// A woman in a green dress: "The river in town grew so hot after Mt. Aleph
  /// erupted."
  /// ★ Named from the game: "The river in town grew so hot after Mt. Aleph
  /// erupted." (MsgValeRiverHeatedAndCooled)
  LOCALID_AFTER_KALAY_WOMAN_ON_HOT_RIVER,
  /// The same old man as LOCALID_CURIOUS_OLD_MAN: "Did you see anything
  /// interesting on your trip, Isaac?"
  /// ★ Named from the game: "Did you see anything interesting on your trip"
  /// (MsgValeAnythingInterestingOnYourTrip)
  LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN,
  /// ★ Named from the game: "How have you been!" (MsgValeReturnHowHaveYouBeen),
  /// the woman of LOCALID_STORM_OLD_WOMAN
  LOCALID_AFTER_KALAY_OLD_WOMAN, ///< "Oh, my! Isaac! How have you been!"
  /// The small boy of LOCALID_FIRST_CHILD: "Welcome back, Isaac! It's good to
  /// have you home!"
  /// ★ Named from the game: "It's good to have you home!" (MsgValeWelcomeHome)
  LOCALID_AFTER_KALAY_FIRST_CHILD,
  /// The little girl of LOCALID_SECOND_CHILD: "I see you made many friends on
  /// your journey."
  /// ★ Named from the game: "I see you made many friends on your journey."
  /// (MsgValeWantToMakeFriends)
  LOCALID_AFTER_KALAY_SECOND_CHILD,
  /// A strongman: "The eruption created a terrible mess on the other side of
  /// Mt. Aleph."
  /// ★ Named from the game: "The eruption created a terrible mess"
  /// (MsgValeEruptionMadeMess)
  LOCALID_AFTER_KALAY_STRONGMAN_ON_ERUPTION,
  /// A second strongman: "The eruption caused a crack to open on Mt. Aleph's
  /// northwest side."
  /// ★ Named from the game: "The eruption caused a crack to open"
  /// (MsgValeNorthwestCrack)
  LOCALID_AFTER_KALAY_STRONGMAN_ON_CRACK,
  /// The hut's owner, the old man with a cane of LOCALID_HUT_OWNER, searching
  /// the
  /// ground: "Hey! Where'd that Psynergy Stone go?"
  /// ★ Named from the game: "Where'd that Psynergy Stone go"
  /// (MsgValeThePsynergyStoneIsGone)
  LOCALID_AFTER_KALAY_HUT_OWNER
};

// Forward declarations.
static void EventScript_MeetSurvivingTravelers(void);
static void ValePlaza_FightTravelers(void);
static void EventScript_TravelersLeaveAfterBattle(void);
static void EventScript_DepartForQuest(void);
static void EventScript_SayThenWait(s32 speaker, s32 frames);
static void ValePlaza_FaceDirectionAndWait(s32 localId, s32 facing, s32 frames);
static void ValePlaza_FlashPsynergyPalette(struct ObjectEvent *objectEvent);
static void ValePlaza_SpawnEffectPair(struct ObjectEvent *parent);
static void ValePlaza_UpdateRockLiftingBoyPsynergy(void);
static void ValePlaza_UpdateShowingStrongmanPsynergy(void);
static void ValePlaza_UpdateSmallRockPalette(void);
static void ValePlaza_SetProgressFlagsAndExit(void);

#ifdef UBFIX
#define VALE_PLAZA_TURN_AND_WAIT ValePlaza_FaceDirectionAndWait
#else
static inline s32 ValePlaza_TurnAndWaitValue(s32 localId, s32 facing,
                                             s32 frames) {
  /* FAKEMATCH(wrapper): a forwarding inline that calls
   * ValePlaza_FaceDirectionAndWait as returning a value; as a direct call in
   * EventScript_SentToAidElders, 15 of 455 instructions differ in tbs-en. */
  return ((s32 (*)())ValePlaza_FaceDirectionAndWait)(localId, facing, frames);
}
#define VALE_PLAZA_TURN_AND_WAIT ValePlaza_TurnAndWaitValue
#endif

#include "common/object_facing.h"

const struct MapWarpDest *ValePlaza_GetWarpDests(void) {
  return gValePlaza_WarpDests;
}

const struct MapWarpRegion *ValePlaza_GetRegions(void) { return NULL; }

/// Returns the stormy night's warps while FLAG_VALE_STORM is set, else the
/// day's.
const u32 *ValePlaza_GetWarps(void) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValePlaza_StormWarps;
  }
  return gValePlaza_Warps;
}

/// Selects the object event templates: the stormy night's, the farewell's
/// (VALE_PLAZA_FROM_SANCTUM), after Kalay, else the day's.
const struct ObjectEventTemplate *ValePlaza_GetObjectEventTemplates(void) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValePlaza_StormObjectEventTemplates;
  }
  if (gSaveBlock.warpDest == VALE_PLAZA_FROM_SANCTUM) {
    return gValePlaza_DepartureObjectEventTemplates;
  }
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gValePlaza_ObjectEventTemplatesAfterKalay;
  }
  return gValePlaza_ObjectEventTemplates;
}

/// In Vale, searching the Psynergy Stone restores the party's PP.
///
/// Reached by searching metatile behavior 100 in every table but the
/// farewell's. Clears FLAG_NOTHING_HAPPENS.
static void EventScript_UsePsynergyStone(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgValeCheckedThePsynergyStone, 1);
  Sound_PlaySong(SE_RECOVERY);
  Party_AddPp(999, 0);
  FieldScript_WaitFrames(10);
  FieldMessage_Show(MsgValePartyPpRestored, 1);
  TextPrinter_CloseFinishedWindows();
  GameFlag_Clear(FLAG_NOTHING_HAPPENS);
  FieldScript_End();
}

/// Selects the events: after Kalay, after the party has left, the farewell's
/// (VALE_PLAZA_FROM_SANCTUM), the stormy night's, else the day's.
const struct MapEvent *ValePlaza_GetEvents(void) {
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gValePlaza_EventsAfterKalay;
  }
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    return gValePlaza_EventsAfterLeaving;
  }
  if (gSaveBlock.warpDest == VALE_PLAZA_FROM_SANCTUM) {
    return gValePlaza_DepartureEvents;
  }
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValePlaza_StormEvents;
  }
  return gValePlaza_Events;
}

/// A burly grey-haired man asks whether Isaac meditates each day. He means on
/// Mt. Aleph, and he stands in Vale on the day the story begins.
///
/// Reached by talking to LOCALID_MAN_ASKING_ABOUT_MEDITATION on the day the
/// story begins.
static void EventScript_AskAboutMeditation(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeMeditateOnMtAlephDaily);
  ObjectEvent_LinkPair(LOCALID_MAN_ASKING_ABOUT_MEDITATION,
                       LOCALID_PARTY_LEADER, 2);
  FieldScript_AskYesNoAndShowResponse(LOCALID_MAN_ASKING_ABOUT_MEDITATION, 0);
  FieldScript_End();
}

/// A woman in a green dress recalls the difficult time three years ago. She
/// asks Isaac about it, in Vale on the day the story begins.
///
/// Reached by talking to LOCALID_WOMAN_RECALLING_STORM on the day the story
/// begins.
static void EventScript_RecallThreeYearsAgo(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeADifficultTimeThreeYears);
  ObjectEvent_LinkPair(LOCALID_WOMAN_RECALLING_STORM, LOCALID_PARTY_LEADER, 2);
  FieldScript_AskYesNoAndShowResponse(LOCALID_WOMAN_RECALLING_STORM, 0);
  FieldScript_End();
}

/// A small boy asks whether the travelers went to meet the mayor. He has fair
/// hair, and asks it in Vale on the day the story begins, about the travelers
/// who stopped by.
///
/// Reached by talking to LOCALID_FIRST_CHILD on the day the story begins.
static void EventScript_AskAboutTheTravelers(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeDidTheTravelersMeetThe);
  ObjectEvent_LinkPair(LOCALID_FIRST_CHILD, LOCALID_PARTY_LEADER, 2);
  FieldScript_AskYesNoAndShowResponse(LOCALID_FIRST_CHILD, 0);
  FieldScript_End();
}

/// In Vale before the party sets out, a strongman shows Isaac his Psynergy and
/// shakes the ground; later he asks whether they worry about crossing the land
/// so few.
///
/// Reached by talking to LOCALID_STRONGMAN_SHOWING_PSYNERGY on the day the
/// story begins and after the party has left. The display adds and removes the
/// callback ValePlaza_UpdateShowingStrongmanPsynergy and shakes the map for
/// three seconds.
static void EventScript_DemonstratePsynergy(void) {
  u32 i;
  s32 message;
  s32 shakes;
  s32 *origin;

  FieldScript_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    message = MsgValeArentWorriedCrossing;
    FieldScript_SetMessageId(message);
    // "...with just the two of you?", three or four: the flag numbered for a
    // character is set once that character has joined.
    if (GameFlag_Get(FLAG_IN_PARTY(IVAN)) != 0) {
      gFieldWork->message++;
    }
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      gFieldWork->message++;
    }
    FieldMessage_ShowAtObject(LOCALID_STRONGMAN_SHOWING_PSYNERGY, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        0) {
      FieldScript_SetMessageId(message + 3);
    } else {
      FieldScript_SetMessageId(message + 4);
    }
    FieldScript_ShowMessageAndWait(LOCALID_STRONGMAN_SHOWING_PSYNERGY, 0);
  } else {
    origin = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
    FieldScript_SetMessageId(MsgValeShownNewAbility);
    ObjectEvent_LinkPair(LOCALID_STRONGMAN_SHOWING_PSYNERGY,
                         LOCALID_PARTY_LEADER, 0);
    FieldScript_AskYesNoAndShowResponse(LOCALID_STRONGMAN_SHOWING_PSYNERGY, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVariantCallback(LOCALID_STRONGMAN_SHOWING_PSYNERGY, 2);
    FieldScript_WaitFrames(15);
    // Flash the caster before starting the palette cycle and camera shake.
    Map_StartCutscenePsynergy();
    shakes = 0;
    for (i = 0; i < 40; i++) {
      ValePlaza_FlashPsynergyPalette(
          ObjectEvent_GetById(LOCALID_STRONGMAN_SHOWING_PSYNERGY));
      WaitFrames(1);
    }
    FieldScheduler_AddCallback(ValePlaza_UpdateShowingStrongmanPsynergy,
                               TASK_PRIORITY_MAP);
    Sound_PlaySong(SE_TREMOR);
    for (i = 0; i != 180; i++) {
      if (Math_RemainderUnsigned(i, 10) == 0) {
        if ((1 & shakes) != 0) {
          *origin -= PIXELS(1);
        } else {
          *origin += PIXELS(1);
        }
        shakes++;
      }
      FieldScript_WaitFrames(1);
    }
    // End the display and restore the strongman's ordinary appearance.
    Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
    Task_Remove(ValePlaza_UpdateShowingStrongmanPsynergy);
    WaitFrames(1);
    Map_EndCutscenePsynergy();
    ObjectEvent_ConfigureSpriteColor(LOCALID_STRONGMAN_SHOWING_PSYNERGY, 0);
    FieldScript_WaitFrames(40);
    FieldScript_SetMessageId(MsgValeBeholdPowerPsynergy);
    FieldScript_ShowMessageAndWait(LOCALID_STRONGMAN_SHOWING_PSYNERGY, 0);
  }
  FieldScript_End();
}

/// An old man with a cane asks Isaac a question that changes with the story.
/// In Vale he answers Isaac about Psynergy on the stormy night, asks about the
/// Wise One after the party has left, and about the journey after Kalay.
///
/// Reached by talking to LOCALID_STORM_CURIOUS_OLD_MAN on the stormy night,
/// LOCALID_CURIOUS_OLD_MAN after the party has left and
/// LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN after Kalay; the story state picks the
/// question.
static void EventScript_AskAboutJourney(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    FieldScript_SetMessageId(MsgValeAnythingInterestingOnYourTrip);
    FieldMessage_ShowAtObject(LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        1) {
      FieldScript_ShowMessageAndWait(LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
    } else {
      struct FieldWork *work = gFieldWork;

      work->message++;
      FieldScript_AskYesNoAndShowResponse(LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN,
                                          0);
    }
  } else if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    FieldScript_SetMessageId(MsgValeYouSawTheWiseOne);
    FieldScript_AskYesNoAndShowResponse(LOCALID_CURIOUS_OLD_MAN, 0);
  } else {
    FieldScript_SetMessageId(MsgValeCanIUsePsynergy);
    FieldScript_AskYesNoAndShowResponse(LOCALID_STORM_CURIOUS_OLD_MAN, 0);
  }
  FieldScript_End();
}

/// An old man shows Isaac the Psynergy Stone that fell on his hut. In Vale
/// after the party has left, the old man whose hut it is points out the large
/// stone, and the camera looks at it.
///
/// Reached by talking to LOCALID_HUT_OWNER after the party has left Vale; in a
/// play-through ValePlaza_Initialize has hidden him by then (a BUG), so only
/// the debug menus, setting FLAG_VALE_PARTY_LEFT without the prologue's flags,
/// reach it.
static void EventScript_TalkAboutFallenStone(void) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_HUT_OWNER, ANIM_STAND);
  ObjectMotion_SetAngleToward(LOCALID_HUT_OWNER, LOCALID_PARTY_LEADER, 20);
  ObjectMotion_SetAngleToward(LOCALID_HUT_OWNER, LOCALID_FALLEN_STONE_SPOT, 40);
  FieldScript_SetMessageId(MsgValeTheStoneFellOnThe);
  EventScript_SayThenWait(LOCALID_HUT_OWNER, 20);
  FieldCamera_SetSpeedParameters(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveWithinMapBounds(PIXELS(337), -1, PIXELS(272), 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_HUT_OWNER, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_HUT_OWNER, LOCALID_PARTY_LEADER, 10);
  EventScript_SayThenWait(LOCALID_HUT_OWNER, 40);
  ObjectEvent_SetBuiltinAction(LOCALID_HUT_OWNER, 2);
  FieldScript_End();
}

/// In Vale on the stormy night, a small boy with some Psynergy left lifts
/// a small rock (LOCALID_STORM_SMALL_ROCK) into the air.
///
/// Reached by talking to LOCALID_STORM_ROCK_LIFTING_BOY on the stormy night.
/// Adds and removes the callbacks ValePlaza_UpdateRockLiftingBoyPsynergy and
/// ValePlaza_UpdateSmallRockPalette.
static void EventScript_BoyLiftsSmallRock(void) {
  u32 i;
  s32 savedMotionFlags;
  struct ObjectEvent *rock;

  FieldScript_Start();
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 82, 760);
  ObjectEvent_LinkPair(LOCALID_STORM_ROCK_LIFTING_BOY, LOCALID_PARTY_LEADER,
                       30);
  FieldScript_SetMessageId(MsgValeIHaveSomePsynergyLeft);
  EventScript_SayThenWait(LOCALID_STORM_ROCK_LIFTING_BOY, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_STORM_ROCK_LIFTING_BOY,
                                 FACING_NORTHWEST, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_STORM_ROCK_LIFTING_BOY,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(20);
  // Charge the caster's display, then start both palette updates.
  Map_StartCutscenePsynergy();
  for (i = 0; i < 40; i++) {
    ValePlaza_FlashPsynergyPalette(
        ObjectEvent_GetById(LOCALID_STORM_ROCK_LIFTING_BOY));
    WaitFrames(1);
  }
  FieldScheduler_AddOrUpdateCallback(ValePlaza_UpdateRockLiftingBoyPsynergy,
                                     TASK_PRIORITY_MAP);
  FieldScheduler_AddCallback(ValePlaza_UpdateSmallRockPalette,
                             TASK_PRIORITY_MAP);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 10);
  // Suspend ordinary movement while lifting the target a little each frame.
  rock = ObjectEvent_GetById(LOCALID_STORM_SMALL_ROCK);
  savedMotionFlags = rock->motionFlags;
  rock->motionFlags = 0;
  for (i = 0; i < 40; i++) {
    rock->y.fixed += PIXELS(3) / 32;
    WaitFrames(1);
  }
  rock->motionFlags = savedMotionFlags;
  Task_Remove(ValePlaza_UpdateRockLiftingBoyPsynergy);
  Task_Remove(ValePlaza_UpdateSmallRockPalette);
  WaitFrames(1);
  Script_PlaySong(SE_RATTLE);
  ObjectEvent_SetSpriteColorById(LOCALID_STORM_ROCK_LIFTING_BOY, 0);
  ObjectEvent_SetSpriteColorById(LOCALID_STORM_SMALL_ROCK, 0);
  FieldScript_WaitFrames(40);
  Map_EndCutscenePsynergy();
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_STORM_ROCK_LIFTING_BOY,
                       30);
  Script_ShowMessage(LOCALID_STORM_ROCK_LIFTING_BOY, 0);
  FieldScript_End();
}

/// A little girl tells Isaac which way Jenna went. In Vale on the stormy night,
/// once he has been sent back to the river, she tells him to help Felix
/// instead.
///
/// Reached by talking to LOCALID_STORM_GIRL_WHO_SAW_JENNA on the stormy
/// night.
static void EventScript_TalkAboutHelping(void) {
  FieldScript_Start();
  ObjectEvent_LinkPair(LOCALID_STORM_GIRL_WHO_SAW_JENNA, LOCALID_PARTY_LEADER,
                       10);
  if (GameFlag_Get(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
    FieldScript_SetMessageId(MsgValeSureHelp);
    FieldScript_ShowMessageAndWait(LOCALID_STORM_GIRL_WHO_SAW_JENNA, 0);
  } else {
    FieldScript_SetMessageId(MsgValeWentOffWay);
    FieldScript_ShowMessageAndWait(LOCALID_STORM_GIRL_WHO_SAW_JENNA, 0);
  }
  FieldScript_End();
}

/// In Vale back from Kalay, a young man welcomes Isaac home; later he only says
/// this is Vale.
///
/// Reached by talking to LOCALID_AFTER_KALAY_WELCOMING_YOUNG_MAN after Kalay.
/// Sets FLAG_VALE_WELCOMED_HOME.
static void EventScript_WelcomeIsaacHome(void) {
  FieldScript_Start();
  if (Script_GetFlag(FLAG_VALE_WELCOMED_HOME) != 0) {
    FieldScript_SetMessageId(MsgValeThisIsVale);
  } else {
    FieldScript_SetMessageId(MsgValeYouCameBackHome);
    GameFlag_Set(FLAG_VALE_WELCOMED_HOME);
  }
  Script_ShowMessage(LOCALID_AFTER_KALAY_WELCOMING_YOUNG_MAN, 0);
  FieldScript_End();
}

/// The old man searching the ground says the Psynergy Stone is gone. In Vale
/// back from Kalay, he stops and tells Isaac the stone that fell on his roof
/// has gone.
///
/// Reached after Kalay by talking to LOCALID_AFTER_KALAY_HUT_OWNER, or by Mind
/// Read on it until FLAG_VALE_PSYNERGY_STONE_GONE_TOLD is set; sets it and
/// restarts the search.
static void EventScript_PsynergyStoneIsGone(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_AFTER_KALAY_HUT_OWNER);
  FieldScript_Start();
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  Import_ObjectEventSetAnimation(LOCALID_AFTER_KALAY_HUT_OWNER, ANIM_STAND);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_AFTER_KALAY_HUT_OWNER);
  FieldEffect_SpawnLinked(LOCALID_AFTER_KALAY_HUT_OWNER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  objectEvent->facing = FACING_NORTHWEST + FACING_STEP;
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_AFTER_KALAY_HUT_OWNER, 2);
  FieldScript_SetMessageId(MsgValeThePsynergyStoneIsGone);
  FieldScript_ShowMessageThenWait(LOCALID_AFTER_KALAY_HUT_OWNER, 0, 40);
  ObjectMotion_SetAngleToward(LOCALID_AFTER_KALAY_HUT_OWNER,
                              LOCALID_PARTY_LEADER, 20);
  ObjectMotion_SetVariantCallback(LOCALID_AFTER_KALAY_HUT_OWNER, 2);
  FieldScript_ShowMessageAndWait(LOCALID_AFTER_KALAY_HUT_OWNER, 0);
  GameFlag_Set(FLAG_VALE_PSYNERGY_STONE_GONE_TOLD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_AFTER_KALAY_HUT_OWNER);
  WaitFrames(1);
  ObjectEvent_SetActionScript(LOCALID_AFTER_KALAY_HUT_OWNER,
                              gValePlaza_HutOwnerSearchActions);
  FieldScript_End();
}

#include "common/vale.h"

/// Opens a door's map grid cells and sends the leader through its threshold.
static inline void EventScript_OpenDoorAndWalkThrough(s32 cellX, s32 cellZ,
                                                      s32 x, s32 z) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gValePlaza_DoorOpens, cellX, cellZ);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, x, z);
}

// Vale's doors here: walking north into metatile behavior n opens door n and
// takes warp n (Vale_RequestBackdropWarp), into the houses
// (MapId_ValeInteriors) or the inn and the forge (MapId_WoodenRoomsA).

/// In Vale, opens the northwest door, into a house (MapId_ValeInteriors,
/// VALE_INTERIORS_FROM_VALE_PLAZA_EAST).
static void EventScript_OpenNorthwestDoor(void) {
  EventScript_OpenDoorAndWalkThrough(44, 7, 248, 279);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_VALE_INTERIORS_EAST);
}

/// In Vale back from Kalay, opens the northeast doorway in two stages and walks
/// into the second forge room (MapId_WoodenRoomsA,
/// WOODEN_ROOMS_A_FROM_VALE_PLAZA_NORTHEAST).
///
/// Reached by walking north into metatile behavior 2 after Kalay; the other
/// tables have no such door.
static void EventScript_OpenNortheastDoor(void) {
  Script_PlaySong(SE_THUMP);
  MapGrid_CopyCellsTo(0, 63, 51, 8, 2, 2);
  WaitFrames(10);
  MapGrid_CopyCellsTo(2, 63, 51, 8, 2, 2);
  WaitFrames(10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 352, 306);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 352, 296);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_WOODEN_ROOMS_A_NORTH);
}

/// In Vale, enters through the western door of the middle row of houses
/// (MapId_ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_NORTH).
static void EventScript_OpenCentralWestDoor(void) {
  EventScript_OpenDoorAndWalkThrough(43, 15, 230, 407);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_VALE_INTERIORS_NORTH);
}

/// In Vale, enters through the eastern door of the middle row of houses
/// (MapId_ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_SOUTH).
static void EventScript_OpenCentralEastDoor(void) {
  EventScript_OpenDoorAndWalkThrough(52, 18, 374, 419);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_VALE_INTERIORS_SOUTH);
}

/// In Vale, opens the southwest door, into Vale's shops
/// (MapId_ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_WEST).
static void EventScript_OpenSouthwestDoor(void) {
  EventScript_OpenDoorAndWalkThrough(41, 32, 200, 546);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_VALE_INTERIORS_WEST);
}

/// In Vale, opens the door at the west edge, into the forge
/// (MapId_WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_NORTHWEST).
static void EventScript_OpenWestEdgeDoor(void) {
  EventScript_OpenDoorAndWalkThrough(35, 36, 102, 611);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_WOODEN_ROOMS_A_NORTHWEST);
}

/// In Vale, opens the southeast door, into the inn
/// (MapId_WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_WEST).
static void EventScript_OpenSoutheastDoor(void) {
  EventScript_OpenDoorAndWalkThrough(51, 39, 358, 670);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTH);
}

/// In Vale on the stormy night, leaves by the west passage, back to the
/// storm's map: stepping on metatile behavior 9 takes VALE_PLAZA_TO_VALE, to
/// the storm's VALE_STORMY_NIGHT_FROM_VALE_PLAZA.
static void EventScript_LeaveThroughWestPassage(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Vale_RequestBackdropWarp(VALE_PLAZA_TO_VALE);
}

/// In Vale, leaves by the town gate for the world map, once the party has
/// begun its journey.
///
/// Reached by stepping on metatile behavior 8; takes VALE_PLAZA_TO_WORLD_MAP
/// once FLAG_VALE_PARTY_LEFT is set, else does nothing. The farewell
/// (EventScript_DepartForQuest) shows the villagers gathered at the gate and
/// leaves by the same warp.
static void EventScript_LeaveThroughTownGate(void) {
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Sound_PlaySong(SE_MAP_EXIT);
    Vale_RequestBackdropWarp(VALE_PLAZA_TO_WORLD_MAP);
  }
}

/// In Vale, Whirlwind blows LOCALID_FIRST_IVY away.
///
/// Reached by Whirlwind on it by day, until it is gone; sets
/// FLAG_VALE_FIRST_IVY_BLOWN_AWAY.
static void EventScript_BlowAwayFirstIvy(void) {
  GameFlag_Set((FLAG_VALE_FIRST_IVY_BLOWN_AWAY));
}

/// In Vale, Whirlwind blows LOCALID_SECOND_IVY away.
///
/// Reached by Whirlwind on it by day, until it is gone; sets
/// FLAG_VALE_SECOND_IVY_BLOWN_AWAY.
static void EventScript_BlowAwaySecondIvy(void) {
  GameFlag_Set((FLAG_VALE_SECOND_IVY_BLOWN_AWAY));
}

/// In Vale, Whirlwind blows LOCALID_THIRD_IVY away.
///
/// Reached by Whirlwind on it by day, until it is gone; sets
/// FLAG_VALE_THIRD_IVY_BLOWN_AWAY.
static void EventScript_BlowAwayThirdIvy(void) {
  GameFlag_Set((FLAG_VALE_THIRD_IVY_BLOWN_AWAY));
}

/// Arriving in this part of Vale: hides the ivy Whirlwind has blown away, runs
/// the three debug warp destinations (the strangers' departure and two
/// presets), and sets up the storm, the farewell or the village after Kalay.
///
/// The map's initialize entry. VALE_PLAZA_DEBUG_CHILDHOOD sets FLAG_CHILDHOOD
/// and takes VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTHEAST; VALE_PLAZA_FROM_SANCTUM
/// runs EventScript_DepartForQuest; VALE_PLAZA_AFTER_TRAVELERS_BATTLE (and
/// VALE_PLAZA_DEBUG_BEFORE_TRAVELERS_LEAVE)
/// EventScript_TravelersLeaveAfterBattle; on the stormy night, once Garet has
/// gone after Isaac (FLAG_VALE_STORM_GARET_LEFT),
/// EventScript_MeetSurvivingTravelers. Once the boys have been sent back to the
/// river, it removes the two strongmen who left to aid the elders, Jenna and
/// the man who went with them.
s32 ValePlaza_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 flag;
  s32 verticalScale;

  if (GameFlag_Get((FLAG_VALE_FIRST_IVY_BLOWN_AWAY)) != 0) {
    Import_ObjectEventSetPosition(LOCALID_FIRST_IVY, 0, 0);
  }
  if (GameFlag_Get((FLAG_VALE_SECOND_IVY_BLOWN_AWAY)) != 0) {
    Import_ObjectEventSetPosition(LOCALID_SECOND_IVY, 0, 0);
  }
  if (GameFlag_Get((FLAG_VALE_THIRD_IVY_BLOWN_AWAY)) != 0) {
    Import_ObjectEventSetPosition(LOCALID_THIRD_IVY, 0, 0);
  }
  switch (gSaveBlock.warpDest) {
  case VALE_PLAZA_DEBUG_CHILDHOOD:
    GameFlag_Set(FLAG_CHILDHOOD);
    Import_ScriptRequestWarp(VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTHEAST);
    return 0;
  case VALE_PLAZA_DEBUG_STORY_FLAGS:
    ValePlaza_SetProgressFlagsAndExit();
    return 0;
  case VALE_PLAZA_DEBUG_BEFORE_TRAVELERS_LEAVE:
    EventScript_TravelersLeaveAfterBattle();
    return 0;
  }
  verticalScale = 3 * Q16_ONE / 2;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_IVY);
  objectEvent->scaleY = verticalScale;
  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_IVY);
  objectEvent->scaleY = verticalScale;
  objectEvent = ObjectEvent_GetById(LOCALID_THIRD_IVY);
  objectEvent->scaleY = verticalScale;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    MapGrid_CopyCellsTo(97, 2, 80, 5, 2, 2);
    MapGrid_CopyCellsTo(42, 53, 42, 54, 3, 1);
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
  } else {
    if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
      Weather_StartRain();
      Weather_StartLightning();
      MapGrid_CopyCellAttributes(21, 38, 1, 1, 18, 41);
      flag = GameFlag_Get(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
      if (flag != 0) {
        Import_ObjectEventSetPosition(LOCALID_STORM_FIRST_AIDE, 0, 0);
        Import_ObjectEventSetPosition(LOCALID_STORM_SECOND_AIDE, 0, 0);
        Object_SetTargetAndCallback(LOCALID_STORM_GARETS_GRANDFATHER,
                                    OBJ_EVENT_LINK_KEEP_MOTION |
                                        LOCALID_PARTY_LEADER,
                                    gValePlaza_FaceTargetActions);
      }
    } else {
      if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
        ObjectEvent_SetPosition(LOCALID_SECOND_CHILD, PIXELS(180), PIXELS(568));
        MapGrid_CopyCellsTo(92, 2, 80, 5, 2, 2);
        MapGrid_CopyCellsTo(42, 53, 42, 54, 3, 1);
        MapGrid_CenterOnFollowedPoint();
        WaitFrames(1);
      }
    }
    if (gSaveBlock.warpDest == VALE_PLAZA_FROM_SANCTUM) {
      EventScript_DepartForQuest();
    } else {
      if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
        objectEvent = ObjectEvent_GetById(LOCALID_STORM_SMALL_ROCK);
        objectEvent->scaleX = 3 * Q16_ONE / 10;
        objectEvent->scaleY = 3 * Q16_ONE / 10;
        objectEvent = ObjectEvent_GetById(LOCALID_STORM_SMALL_ROCK);
        ObjectEvent_SetSpriteFlags(objectEvent, 0);
        objectEvent = ObjectEvent_GetById(LOCALID_STORM_SMALL_ROCK_SHADOW);
        objectEvent->scaleX = 3 * Q16_ONE / 5;
        objectEvent->scaleY = 3 * Q16_ONE / 5;
        Import_ObjectEventSetAnimation(LOCALID_STORM_ROCK_DEFLECTING_YOUNG_MAN,
                                       5);
      } else {
        if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
          ObjectEvent_SetPosition(LOCALID_FALLEN_STONE_SPOT, PIXELS(331),
                                  PIXELS(249));
          objectEvent = ObjectEvent_GetById(LOCALID_FALLEN_STONE_SPOT);
          ObjectEvent_SetSpriteFlags(objectEvent, 0);
        }
      }
      // BUG: the test is meant for the stormy night, but the flag stays set,
      // and by day these numbers are LOCALID_HUT_OWNER and a strongman already
      // out of sight: the hut's owner is hidden from the prologue's end until
      // Kalay, and his talk and EventScript_TalkAboutFallenStone never play.
#ifdef BUGFIX
      if (GameFlag_Get(FLAG_VALE_STORM) != 0 &&
          GameFlag_Get(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
#else
      if (GameFlag_Get(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
#endif
        Import_ObjectEventSetPosition(LOCALID_STORM_JENNA, 0, 0);
        Import_ObjectEventSetPosition(LOCALID_STORM_CHARGED_UP_STRONGMAN, 0, 0);
      }
      if (gSaveBlock.warpDest == VALE_PLAZA_AFTER_TRAVELERS_BATTLE) {
        EventScript_TravelersLeaveAfterBattle();
      } else {
        if (GameFlag_Get(FLAG_VALE_STORM) != 0 &&
            (flag = GameFlag_Get(FLAG_VALE_STORM_GARET_LEFT)) != 0) {
          EventScript_MeetSurvivingTravelers();
        } else {
          if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
            FieldScript_StartScreenTransition();
            FieldScript_WaitTransitionFrames();
            Weather_ResumeLightning();
          }
        }
      }
    }
  }
  return 0;
}

/// Walks a separate map object event back to the leader, then hides it.
static inline void ValePlaza_RejoinParty(s32 localId) {
  struct ObjectEvent *leader;

  Import_ObjectEventSetAnimation(localId, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectMotion_ResetAndSetPosition(localId, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(localId);
  Import_ObjectEventSetPosition(localId, 0, 0);
}

/// The mayor sends help to the elders and the boys back to the river. In Vale's
/// plaza on the stormy night, Garet's grandfather, the mayor, sends two
/// strongmen to aid the elders holding back the Boulder, then sends Isaac and
/// Garet back to the river to show Jenna and a man whose Psynergy is restored
/// the way. "Go aid the elders!"
///
/// Reached by stepping on metatile behavior 20 on the stormy night, until
/// FLAG_VALE_STORM_SENT_TO_AID_ELDERS is set; sets it. The flag is named for
/// the mayor's first order, but what it records is the boys being sent back:
/// the return to the river (vale_stormy_night.c) waits for it.
static void EventScript_SentToAidElders(void) {
  struct ObjectEvent *leader;
  s32 skipReply = 0;
  s32 speechFlags;

  if (GameFlag_Get(FLAG_VALE_STORM) != 0 &&
      GameFlag_Get(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
    // The camera goes to the villagers by the stone. Garet's grandfather: go
    // aid the elders, the Boulder is too heavy for them.
    FieldScript_Start();
    FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
    FieldCamera_MoveWithinMapBounds(PIXELS(197), -1, PIXELS(768), 1);
    FieldCamera_WaitForArrival();
    FieldScript_SetMessageId(MsgValeGoAidElders);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_STORM_GARETS_GRANDFATHER, 2);
    Script_ShowMessageAndWait(
        SPEAKER_WINDOW_ABOVE | LOCALID_STORM_GARETS_GRANDFATHER, 0, 10);
    // Isaac walks over and Garet steps out of him.
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetSpeed(LOCALID_STORM_GARET, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 179, 789);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != NULL) {
      Import_ObjectEventSetPosition(LOCALID_STORM_GARET, leader->x.fixed,
                                    leader->z.fixed);
    }
    ObjectEvent_WalkToAndWait(LOCALID_STORM_GARET, 179, 804);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_STORM_GARET, 40);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    ObjectMotion_ArmCallback(LOCALID_STORM_GARET, FACING_EAST, 0);
    // The two aides will not let the Boulder destroy the village and go to
    // help; Garet's grandfather counts on them.
    Import_ObjectEventSetAnimation(LOCALID_STORM_FIRST_AIDE, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_SECOND_AIDE, ANIM_NOD);
    ObjectEvent_LinkPair(LOCALID_STORM_FIRST_AIDE, LOCALID_STORM_SECOND_AIDE,
                         0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVariantCallback(LOCALID_STORM_FIRST_AIDE, 1);
    FieldScript_ShowMessageThenWait(
        SPEAKER_WINDOW_ABOVE | LOCALID_STORM_FIRST_AIDE, 0, 10);
    Import_ObjectEventSetAnimation(LOCALID_STORM_SECOND_AIDE, ANIM_NOD);
    FieldScript_ShowMessageThenWait(LOCALID_STORM_SECOND_AIDE, 0, 10);
    ObjectMotion_ArmCallback(LOCALID_STORM_FIRST_AIDE, FACING_EAST, 0);
    ObjectEvent_FaceDirection(LOCALID_STORM_SECOND_AIDE,
                              FACING_NORTHEAST + FACING_STEP, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_GARETS_GRANDFATHER,
                                     ANIM_NOD);
    Script_ShowMessageAndWait(
        SPEAKER_WINDOW_ABOVE | LOCALID_STORM_GARETS_GRANDFATHER, 0, 10);
#if TBS_PATCH_4
    ObjectMotion_SetActionVariant(LOCALID_STORM_SECOND_AIDE, 1);
#endif
    ObjectEvent_SetSpeed(LOCALID_STORM_FIRST_AIDE, PIXELS(8) / 5,
                         PIXELS(4) / 5);
    ObjectEvent_SetSpeed(LOCALID_STORM_SECOND_AIDE, PIXELS(8) / 5,
                         PIXELS(4) / 5);
    // The two run off; Isaac and Garet step up to the group.
    ObjectEvent_SetActionScript(LOCALID_STORM_FIRST_AIDE,
                                gValePlaza_AideRunActions);
    FieldScript_WaitFrames(20);
#if TBS_PATCH_4
    ObjectEvent_GetById(LOCALID_STORM_SECOND_AIDE)->priorityFlags |= 1;
#endif
    ObjectEvent_SetActionScript(LOCALID_STORM_SECOND_AIDE,
                                gValePlaza_AideRunActions);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_STORM_GARET, FACING_NORTH, 60);
    ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                                gValePlaza_LeaderStepActions);
    ObjectEvent_RunActionScriptAndWait(LOCALID_STORM_GARET,
                                       gValePlaza_StormGaretStepActions);
    FieldScript_WaitFrames(20);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    ObjectMotion_ArmCallback(LOCALID_STORM_GARET, FACING_EAST, 10);
    // Garet calls out to Grandpa and Jenna; Jenna asks whether they came to
    // help her.
    FieldScript_ShowMessageAndWait(LOCALID_STORM_GARET, 0);
    ObjectEvent_FaceDirection(LOCALID_STORM_GARETS_GRANDFATHER, FACING_WEST, 0);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_JENNA, FACING_SOUTHWEST, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_STORM_JENNA, 2);
    EventScript_SayThenWait(LOCALID_STORM_JENNA, 10);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_GARET, ANIM_NOD);
    FieldScript_WaitFrames(20);
    // Garet's grandfather asks whether Jenna told them about Felix. Yes: good
    // boys, Jenna could use a hand; no: help her regardless.
    ObjectMotion_SetVarCbAndRefresh(LOCALID_STORM_GARETS_GRANDFATHER, 2);
    FieldMessage_ShowAtObject(
        SPEAKER_WINDOW_ABOVE | LOCALID_STORM_GARETS_GRANDFATHER, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        1) {
      skipReply = 1;
      Import_ObjectEventSetAnimation(LOCALID_STORM_GARETS_GRANDFATHER,
                                     ANIM_SHAKE_HEAD);
    } else {
      Import_ObjectEventSetAnimation(LOCALID_STORM_GARETS_GRANDFATHER,
                                     ANIM_NOD);
      gFieldWork->message++;
    }
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_STORM_GARETS_GRANDFATHER,
                       0);
    if (skipReply != 0) {
      gFieldWork->message++;
    }
    // The rested strongman is all charged up; everyone turns to him.
    speechFlags = SPEAKER_WINDOW_ABOVE;
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_CHARGED_UP_STRONGMAN,
                                   FACING_SOUTH, 30);
    FieldScript_ShowMessageAndWait(LOCALID_STORM_CHARGED_UP_STRONGMAN, 0);
    ObjectEvent_ShowEmote(LOCALID_STORM_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_STORM_JENNA,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_STORM_GARET,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    ObjectEvent_FaceDirection(LOCALID_STORM_GARETS_GRANDFATHER,
                              FACING_NORTHWEST, 0);
    ObjectEvent_FaceDirection(LOCALID_STORM_JENNA, FACING_NORTHWEST, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_GARET, FACING_NORTHEAST, 10);
    FieldCamera_SetSpeedParameters(PIXELS(6) / 5, PIXELS(3) / 20);
    FieldCamera_MoveTo(PIXELS(215), -1, PIXELS(758), 1);
    FieldCamera_WaitForArrival();
    FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
    FieldCamera_MoveTo(PIXELS(205), -1, PIXELS(778), 1);
    // He hops down to them; asked about his Psynergy, he has enough to save
    // Felix, thanks to the Psynergy Stone.
    ObjectEvent_SetActionScript(LOCALID_STORM_CHARGED_UP_STRONGMAN,
                                gValePlaza_ChargedUpStrongmanActions);
    ObjectEvent_RefreshSelectorById(LOCALID_STORM_CHARGED_UP_STRONGMAN);
    VALE_PLAZA_TURN_AND_WAIT(LOCALID_STORM_CHARGED_UP_STRONGMAN,
                             FACING_SOUTHEAST, 60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_STORM_GARETS_GRANDFATHER, 2);
    EventScript_SayThenWait(LOCALID_STORM_GARETS_GRANDFATHER, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_CHARGED_UP_STRONGMAN,
                                     ANIM_NOD);
    EventScript_SayThenWait(LOCALID_STORM_CHARGED_UP_STRONGMAN, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_GARETS_GRANDFATHER,
                                     ANIM_NOD);
    FieldScript_WaitFrames(10);
    // Garet's grandfather sends Jenna back with the rested strongman, and Isaac
    // and Garet to show the way past the fallen rocks; Jenna thanks Isaac.
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_GARETS_GRANDFATHER,
                                   FACING_SOUTH, 30);
    EventScript_SayThenWait(speechFlags + LOCALID_STORM_GARETS_GRANDFATHER, 10);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_JENNA, FACING_NORTHEAST, 30);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_JENNA, ANIM_NOD);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_GARETS_GRANDFATHER,
                                   FACING_WEST, 30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_STORM_GARETS_GRANDFATHER, 2);
    EventScript_SayThenWait(speechFlags + LOCALID_STORM_GARETS_GRANDFATHER, 10);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_STORM_GARET, 40);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_GARET, FACING_EAST, 20);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_JENNA, FACING_WEST, 30);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_JENNA, ANIM_NOD);
    EventScript_SayThenWait(LOCALID_STORM_JENNA, 30);
    ValePlaza_FaceDirectionAndWait(LOCALID_STORM_JENNA, FACING_NORTH, 30);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_JENNA, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_STORM_CHARGED_UP_STRONGMAN,
                                     ANIM_NOD);
    // Garet, Jenna and the rested strongman walk into Isaac; Garet's
    // grandfather keeps turning to watch him.
    ValePlaza_RejoinParty(LOCALID_STORM_GARET);
    ValePlaza_RejoinParty(LOCALID_STORM_JENNA);
    ValePlaza_RejoinParty(LOCALID_STORM_CHARGED_UP_STRONGMAN);
    Object_SetTargetAndCallback(LOCALID_STORM_GARETS_GRANDFATHER,
                                OBJ_EVENT_LINK_KEEP_MOTION |
                                    LOCALID_PARTY_LEADER,
                                gValePlaza_FaceTargetActions);
    GameFlag_Set(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
    FieldScript_End();
  }
}

/// In Vale on the stormy night, after the river, Isaac overhears two travelers
/// who survived Sol Sanctum's fury, the game's first sight of Saturos and
/// Menardi. "Only the two of us survived..."
///
/// Called by ValePlaza_Initialize on the stormy night once
/// FLAG_VALE_STORM_GARET_LEFT is set; goes on into ValePlaza_FightTravelers.
static void EventScript_MeetSurvivingTravelers(void) {
  struct ObjectEvent *camera;

  camera = FieldCamera_GetScriptObject();
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  // The screen opens on the storm at the west path, where Isaac comes in from
  // the river; the music fades out as he walks up and hears the first
  // traveler.
  FieldCamera_MoveWithinMapBounds(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_PrepareObjectAndApplyValue(1, 0);
  FieldScript_StartScreenTransition();
  Sound_PlaySong(SONG_FADE_OUT_BGM);
  Weather_ResumeLightning();
  Import_ObjectEventSetPosition(LOCALID_FIRST_TRAVELER, PIXELS(105),
                                PIXELS(267));
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 93, 343);
  FieldScript_SetMessageId(MsgValeOnlyTwoSurvived);
  FieldScript_ShowMessageAndWait(LOCALID_FIRST_TRAVELER, 0);
  // The travelers' theme starts; the camera rises to them and the second
  // traveler walks in.
  Sound_PlaySong(MUS_SATUROS_AND_MENARDI);
  camera->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveWithinMapBounds(PIXELS(109), PIXELS(176), PIXELS(281), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectEvent_SetPosition(LOCALID_SECOND_TRAVELER, PIXELS(135), PIXELS(177));
  ObjectEvent_SetSpeed(LOCALID_SECOND_TRAVELER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(LOCALID_SECOND_TRAVELER, 126, 258);
  FieldScript_WaitFrames(40);
  ObjectMotion_ArmCallback(LOCALID_FIRST_TRAVELER, FACING_NORTH + FACING_STEP,
                           0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SECOND_TRAVELER);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_TRAVELER, ANIM_STAND);
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER,
                                 FACING_SOUTHWEST + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TRAVELER, ANIM_NOD);
  // They talk of Sol Sanctum's fury and its trap switch, of the power of
  // Alchemy, and of their next attempt on the sanctum.
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_SECOND_TRAVELER, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TRAVELER, 2);
  EventScript_SayThenWait(SPEAKER_WINDOW_BELOW | LOCALID_FIRST_TRAVELER, 30);
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER,
                                 FACING_NORTHWEST + FACING_STEP, 20);
  EventScript_SayThenWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_SECOND_TRAVELER, 10);
  ValePlaza_FaceDirectionAndWait(LOCALID_FIRST_TRAVELER,
                                 FACING_NORTHWEST + FACING_STEP, 40);
  FieldScript_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_FIRST_TRAVELER,
                                 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | LOCALID_SECOND_TRAVELER, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_FIRST_TRAVELER,
                                 FACING_NORTHEAST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TRAVELER, 2);
  FieldScript_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_FIRST_TRAVELER,
                                 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER, FACING_SOUTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_TRAVELER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  EventScript_SayThenWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_SECOND_TRAVELER, 20);
  ValePlaza_FightTravelers();
  FieldScript_End();
}

/// In Vale on the stormy night, the travelers catch Isaac eavesdropping and
/// fight; win or lose, the party returns by VALE_PLAZA_AFTER_TRAVELERS_BATTLE.
/// "You must forget everything you heard."
///
/// Called by EventScript_MeetSurvivingTravelers. Starts the fifth slot of the
/// encounter zone of the scripted battles of Vale, Vault and the world map,
/// with both outcomes leading back to VALE_PLAZA_AFTER_TRAVELERS_BATTLE, and
/// sets FLAG_KEEP_MUSIC.
static void ValePlaza_FightTravelers(void) {
  struct ObjectEvent *objectEvent;
  s32 southSouthwestFacing;
  s32 westFacing;
  s32 northNortheastFacing;

  // Garet, still out of sight, calls Isaac to wait; the travelers turn and
  // look down at Isaac.
  ObjectEvent_ConfigureSpriteColor(LOCALID_STORM_GARET, 15);
  objectEvent = ObjectEvent_GetById(LOCALID_STORM_GARET);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_SetPosition(LOCALID_STORM_GARET, 0, PIXELS(331));
  WaitFrames(1);
  Script_ShowMessage(SPEAKER_PORTRAIT_LEFT | LOCALID_STORM_GARET, 0);
  ObjectEvent_ShowEmote(LOCALID_FIRST_TRAVELER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_SECOND_TRAVELER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Import_ObjectEventSetPosition(LOCALID_STORM_GARET, 0, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRAVELER, FACING_SOUTH + FACING_STEP,
                            0);
  southSouthwestFacing = FACING_SOUTH + FACING_STEP;
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER, southSouthwestFacing,
                                 40);
  FieldCamera_SetSpeedParameters(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveWithinMapBounds(PIXELS(89), PIXELS(176), PIXELS(313), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRAVELER, FACING_NORTHEAST, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER,
                                 FACING_SOUTHWEST + FACING_STEP, 40);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(100), PIXELS(144), PIXELS(333), 1);
  ObjectEvent_SetSpeed(LOCALID_FIRST_TRAVELER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_SECOND_TRAVELER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_FIRST_TRAVELER, 105, 329);
  FieldScript_WaitFrames(10);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_SECOND_TRAVELER, 124, 329);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_FIRST_TRAVELER);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TRAVELER, ANIM_STAND);
  ObjectMotion_ArmCallback(LOCALID_FIRST_TRAVELER, southSouthwestFacing, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SECOND_TRAVELER);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_TRAVELER, ANIM_STAND);
  ObjectMotion_ArmCallback(LOCALID_SECOND_TRAVELER, southSouthwestFacing, 0);
  // Garet runs in from the west. The first traveler asks whether Isaac was
  // eavesdropping; whatever he answers, Garet thinks they look scary.
  ObjectEvent_ConfigureSpriteColor(LOCALID_STORM_GARET, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_STORM_GARET);
  ObjectEvent_SetSpriteFlags(objectEvent, 1);
  ObjectEvent_SetPosition(LOCALID_STORM_GARET, 0, PIXELS(331));
  ObjectEvent_SetSpeed(LOCALID_STORM_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectMotion_SetPositionAndReset(LOCALID_STORM_GARET, 37, 339);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TRAVELER, ANIM_NOD);
  FieldMessage_ShowAtObject(LOCALID_FIRST_TRAVELER, 0);
  ObjectEvent_ShowEmote(LOCALID_STORM_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  northNortheastFacing = FACING_NORTH + FACING_STEP;
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, northNortheastFacing,
                                 10);
  FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_ArmCallback(LOCALID_FIRST_TRAVELER, FACING_EAST, 0);
  westFacing = FACING_WEST;
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER, westFacing, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_STORM_GARET, 2);
  EventScript_SayThenWait(SPEAKER_PORTRAIT_LEFT | LOCALID_STORM_GARET, 10);
  Object_SetTargetAndCallback(LOCALID_PARTY_LEADER,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_STORM_GARET,
                              gValePlaza_FaceTargetActions);
  ObjectEvent_WalkToAndWait(LOCALID_STORM_GARET, 93, 361);
  ValePlaza_FaceDirectionAndWait(LOCALID_STORM_GARET, northNortheastFacing, 40);
  EventScript_SayThenWait(LOCALID_STORM_GARET, 20);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_ArmCallback(LOCALID_FIRST_TRAVELER, FACING_EAST, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER, westFacing, 15);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TRAVELER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_TRAVELER, ANIM_NOD);
  ObjectMotion_ArmCallback(LOCALID_FIRST_TRAVELER, southSouthwestFacing, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_SECOND_TRAVELER, southSouthwestFacing,
                                 30);
  // The travelers: forget everything you heard; we'll help you forget.
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | LOCALID_SECOND_TRAVELER, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, northNortheastFacing,
                                 30);
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, FACING_SOUTH, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TRAVELER, 2);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TRAVELER, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_FIRST_TRAVELER, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldEffect_ConfigureLinkedObject(
      LOCALID_STORM_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  ObjectEvent_SetActionScript(LOCALID_SECOND_TRAVELER,
                              gValePlaza_SecondTravelerLeapActions);
  FieldScript_WaitFrames(6);
  ObjectEvent_SetActionScript(LOCALID_FIRST_TRAVELER,
                              gValePlaza_FirstTravelerLeapActions);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gValePlaza_LeaderActions);
  FieldScript_WaitFrames(6);
  ObjectEvent_RunActionScriptAndWait(LOCALID_STORM_GARET,
                                     gValePlaza_StormGaretLeapActions);
  // The confrontation continues in battle; both outcomes return here.
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  {
    const void *message = &MapId_ValePlaza;

    FieldScript_SetNextDestination((s32)message,
                                   VALE_PLAZA_AFTER_TRAVELERS_BATTLE);
    FieldScript_SetDefeatDestination((s32)message,
                                     VALE_PLAZA_AFTER_TRAVELERS_BATTLE);
  }
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 4);
  GameFlag_Set(FLAG_KEEP_MUSIC);
}

/// In Vale after the battle, the travelers leave Isaac and Garet lying in the
/// storm, and the storm, and the prologue, end.
///
/// Called by ValePlaza_Initialize from VALE_PLAZA_AFTER_TRAVELERS_BATTLE, after
/// the battle, or VALE_PLAZA_DEBUG_BEFORE_TRAVELERS_LEAVE. Clears
/// FLAG_VALE_STORM and takes VALE_PLAZA_TO_TITLE, to the title (MapId_Title,
/// TITLE_FROM_VALE_PLAZA), which then opens on Vale three years later
/// (menu/title.c).
static void EventScript_TravelersLeaveAfterBattle(void) {
  struct ObjectEvent *objectEvent;
  s32 gravityQ8;

  // The screen opens on Isaac and Garet lying beaten in the storm, the
  // travelers standing over them.
  Sound_PlaySong(SONG_FADE_OUT_BGM);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  FieldCamera_MoveWithinMapBounds(PIXELS(64), PIXELS(144), PIXELS(350), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(48), PIXELS(346));
  ObjectEvent_SetPosition(LOCALID_STORM_GARET, PIXELS(78), PIXELS(358));
  ObjectEvent_SetPosition(LOCALID_FIRST_TRAVELER, PIXELS(103), PIXELS(342));
  ObjectEvent_SetPosition(LOCALID_SECOND_TRAVELER, PIXELS(112), PIXELS(360));
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRAVELER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_TRAVELER, FACING_WEST, 0);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 16);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->scaleX = -Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  Import_ObjectEventSetAnimation(LOCALID_STORM_GARET, 7);
  objectEvent = ObjectEvent_GetById(LOCALID_STORM_GARET);
  objectEvent->sprite->rotation = 0x1555;
  objectEvent = ObjectEvent_GetById(LOCALID_STORM_GARET);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  {
    struct FieldWork *work = gFieldWork;

    work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  }
  FieldScript_StartScreenTransition();
  Weather_ResumeLightning();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(80);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRAVELER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_TRAVELER, FACING_NORTH, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TRAVELER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_TRAVELER, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_FIRST_TRAVELER, FACING_WEST, 10);
  ObjectEvent_FaceDirection(LOCALID_SECOND_TRAVELER, FACING_WEST, 10);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetActionVariant(LOCALID_STORM_GARET, 3);
  ObjectEvent_SetSpeed(LOCALID_FIRST_TRAVELER, PIXELS(12) / 5, PIXELS(6) / 5);
  gravityQ8 = 128;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_TRAVELER);
  objectEvent->bounce = 0x28f;
  objectEvent->gravity = gravityQ8 << 8;
  ObjectEvent_SetActionScript(LOCALID_FIRST_TRAVELER,
                              gValePlaza_FirstTravelerBounceActions);
  FieldScript_WaitFrames(24);
  ObjectMotion_SetSpeedParameters(LOCALID_SECOND_TRAVELER, PIXELS(12) / 5,
                                  PIXELS(6) / 5);
  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_TRAVELER);
  objectEvent->bounce = 0x28f;
  objectEvent->gravity = gravityQ8 << 8;
  ObjectEvent_RunActionScriptAndWait(LOCALID_SECOND_TRAVELER,
                                     gValePlaza_SecondTravelerBounceActions);
  FieldScript_WaitFrames(40);
  // Three last flashes of lightning, then the storm ends and the screen
  // fades slowly out.
  Weather_StopLightning();
  Weather_LastLightning();
  WaitFrames(20);
  Weather_LastLightning();
  WaitFrames(60);
  Weather_LastLightning();
  WaitFrames(20);
  Weather_StopLightning();
  FieldScript_WaitFrames(40);
  gFieldWork->transitionFrames = 120;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Clear(FLAG_VALE_STORM);
  Import_ScriptRequestWarp(VALE_PLAZA_TO_TITLE);
  FieldScript_End();
}

/// In Vale, at the town gate, the villagers give their supplies and farewells
/// as Isaac and Garet set out on the Wise One's quest.
///
/// Called by ValePlaza_Initialize from VALE_PLAZA_FROM_SANCTUM, the way out of
/// the Vale Sanctum once the quest is accepted. Restores the party's HP and PP,
/// gives a Herb (from Garet's sister) and the Catch Beads (from Dora), and
/// takes VALE_PLAZA_TO_WORLD_MAP.
static void EventScript_DepartForQuest(void) {
  struct ObjectEvent *objectEvent;
  struct FieldWork *work;
  s32 priorityFlags;
  const s32 *faceActions;
  const s32 *chaseActions;
  const s32 *departureActions;

  // The village gathers at the gate to see Isaac and Garet off; the camera
  // starts on the two children up the path.
  Party_RestoreHpPp(1);
  FieldScript_Start();
  MapGrid_CopyCellsTo(42, 53, 42, 54, 3, 1);
  FieldCamera_MoveTo(PIXELS(180), PIXELS(16), PIXELS(618), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 2);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 3);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 4);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 5);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 6);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 9);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetActionVariant(LOCALID_GARET, 1);
  ObjectMotion_SetActionVariant(LOCALID_FAREWELL_GARETS_BROTHER, 1);
  ObjectMotion_SetActionVariant(LOCALID_FAREWELL_GARETS_SISTER, 1);
  ObjectMotion_SetActionVariant(LOCALID_FAREWELL_GARETS_GRANDFATHER, 1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(208), PIXELS(814));
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(80);
  // The second child asks why everyone is at the gate; the first says Isaac and
  // Garet are leaving on a journey.
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_SECOND_CHILD,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHWEST + FACING_STEP, 20);
  Script_SetMessage(MSGNO(MsgValeWhysEveryoneHanging));
  EventScript_SayThenWait(LOCALID_FAREWELL_SECOND_CHILD, 10);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_FIRST_CHILD,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_EAST + FACING_STEP, 10);
  EventScript_SayThenWait(LOCALID_FAREWELL_FIRST_CHILD, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_SECOND_CHILD, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_FIRST_CHILD, 2);
  EventScript_SayThenWait(LOCALID_FAREWELL_FIRST_CHILD, 10);
  // They walk to the top of the path: the second child wishes to go too, the
  // first has heard it will be dangerous, and everyone is down there to say
  // good-bye.
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_SECOND_CHILD,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_SECOND_CHILD, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_SECOND_CHILD, 184, 618);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 60);
  EventScript_SayThenWait(LOCALID_FAREWELL_SECOND_CHILD, 20);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_FIRST_CHILD, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_FIRST_CHILD, 168, 618);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_NORTHEAST + FACING_STEP, 10);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CHILD, ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_FAREWELL_FIRST_CHILD, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHWEST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_SECOND_CHILD, 1);
  FieldScript_ShowMessageAndWait(LOCALID_FAREWELL_SECOND_CHILD, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_DOG, PIXELS(12) / 5, PIXELS(6) / 5);
  ObjectEvent_SetPosition(LOCALID_FAREWELL_DOG, PIXELS(110), PIXELS(744));
  WaitFrames(2);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_DOG, ANIM_NOD);
  ObjectEvent_SetActionScript(LOCALID_FAREWELL_DOG,
                              gValePlaza_FarewellDogActions);
  FieldScript_WaitFrames(40);
  faceActions = gValePlaza_FaceTargetActions;
  Object_SetTargetAndCallback(LOCALID_FAREWELL_FIRST_CHILD,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_FAREWELL_DOG,
                              faceActions);
  Object_SetTargetAndCallback(LOCALID_FAREWELL_SECOND_CHILD,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_FAREWELL_DOG,
                              faceActions);
  ObjectEvent_RefreshSelectorById(LOCALID_FAREWELL_DOG);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FAREWELL_FIRST_CHILD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FAREWELL_SECOND_CHILD);
  FieldScript_WaitFrames(60);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_FIRST_CHILD,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_SECOND_CHILD,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  ObjectMotion_ArmCallback(LOCALID_FAREWELL_FIRST_CHILD,
                           FACING_EAST + FACING_STEP, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHWEST + FACING_STEP, 80);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTH + FACING_STEP, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_EAST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_FIRST_CHILD, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTH + FACING_STEP, 60);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTH + FACING_STEP, 60);
  // They step down to look over the crowd: Isaac's mother is not there to
  // see him off.
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_SECOND_CHILD,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectMotion_ArmCallback(LOCALID_FAREWELL_FIRST_CHILD,
                           FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_SECOND_CHILD, 184, 630);
  FieldScript_WaitFrames(20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTH + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_SECOND_CHILD,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_FIRST_CHILD,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_FIRST_CHILD, 168, 630);
  FieldScript_WaitFrames(20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTH + FACING_STEP, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_FIRST_CHILD,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_EAST + FACING_STEP, 10);
  EventScript_SayThenWait(LOCALID_FAREWELL_FIRST_CHILD, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_SECOND_CHILD, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_FAREWELL_SECOND_CHILD, 10);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_FIRST_CHILD,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTH + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTH + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_SOUTH + FACING_STEP, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_FIRST_CHILD, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_FAREWELL_FIRST_CHILD, 10);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_FAREWELL_DOG);

    if (objectEvent != NULL) {
      Import_ObjectEventSetPosition(LOCALID_FAREWELL_CAT, objectEvent->x.fixed,
                                    objectEvent->z.fixed);
    }
  }
  priorityFlags = (u8)~OBJ_EVENT_PRIORITY_AUTOMATIC;
  WaitFrames(2);
  ObjectEvent_GetById(LOCALID_FAREWELL_DOG)->priorityFlags &= priorityFlags;
  ObjectEvent_GetById(LOCALID_FAREWELL_CAT)->priorityFlags &= priorityFlags;
  ObjectMotion_SetActionVariant(LOCALID_FAREWELL_DOG, 2);
  ObjectMotion_SetActionVariant(LOCALID_FAREWELL_CAT, 2);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_CAT, PIXELS(18) / 5, PIXELS(9) / 5);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_CAT, ANIM_WALK);
  chaseActions = gValePlaza_FarewellChaseActions;
  ObjectEvent_SetActionScript(LOCALID_FAREWELL_CAT, chaseActions);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_DOG, ANIM_NOD);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_DOG, PIXELS(24) / 5, PIXELS(12) / 5);
  ObjectEvent_RunActionScriptAndWait(LOCALID_FAREWELL_DOG, chaseActions);
  FieldScript_WaitFrames(60);
  // Should they say good-bye too? Sure: both run down, and the camera
  // follows them to the gate.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_SECOND_CHILD, 2);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHWEST + FACING_STEP, 10);
  EventScript_SayThenWait(LOCALID_FAREWELL_SECOND_CHILD, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_FIRST_CHILD, 1);
  FieldScript_WaitFrames(20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_FIRST_CHILD,
                                 FACING_EAST + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_FIRST_CHILD, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_FAREWELL_FIRST_CHILD, 20);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_SECOND_CHILD, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_FIRST_CHILD, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_FIRST_CHILD, PIXELS(12) / 5,
                       PIXELS(6) / 5);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_SECOND_CHILD, PIXELS(12) / 5,
                       PIXELS(6) / 5);
  ObjectEvent_SetActionScript(LOCALID_FAREWELL_FIRST_CHILD,
                              gValePlaza_FarewellFirstChildActions);
  FieldScript_WaitFrames(10);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(215), PIXELS(16), PIXELS(801), 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetActionScript(LOCALID_FAREWELL_SECOND_CHILD,
                              gValePlaza_FarewellSecondChildActions);
  ObjectEvent_RefreshSelectorById(LOCALID_FAREWELL_SECOND_CHILD);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_SECOND_CHILD,
                                 FACING_SOUTHEAST + FACING_STEP, 120);
  // The Great Healer counts on Isaac, and on Garet too; the two nod.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_GREAT_HEALER, 2);
  FieldScript_WaitFrames(20);
  EventScript_SayThenWait(LOCALID_FAREWELL_GREAT_HEALER, 20);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_GARET, FACING_WEST + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, FACING_NORTH, 10);
  ValePlaza_FaceDirectionAndWait(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP,
                                 10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(40);
  // Garet's sister warns them off water that smells funny and hands over a
  // Herb.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_GARETS_SISTER, 2);
  FieldScript_WaitFrames(20);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_SISTER, 10);
  ObjectEvent_SetSpeed(LOCALID_FAREWELL_GARETS_SISTER, PIXELS(1),
                       PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_GARETS_SISTER, 216, 800);
  ObjectEvent_FaceDirection(LOCALID_FAREWELL_GARETS_SISTER, FACING_SOUTH, 0);
  PartyInventory_GiveItem(ITEM_HERB, 0);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_GARETS_SISTER, 264, 800);
  ObjectEvent_FaceDirection(LOCALID_FAREWELL_GARETS_SISTER, FACING_SOUTHWEST,
                            0);
  // Garet protests he knows that, Sis; his brother says he will eat anything,
  // and Garet jumps.
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ValePlaza_FaceDirectionAndWait(LOCALID_GARET, FACING_NORTHEAST + FACING_STEP,
                                 10);
  EventScript_SayThenWait(LOCALID_GARET, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_GARETS_BROTHER,
                                   ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_BROTHER, 10);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  ObjectMotion_Launch(LOCALID_GARET, 4, 60);
  // Dora is very late, the bearded man in a long robe says. Everyone wonders;
  // Garet's brother remembers.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_MAN_AWAITING_DORA, 2);
  FieldScript_WaitFrames(20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_MAN_AWAITING_DORA,
                                 FACING_NORTH + FACING_STEP, 10);
  EventScript_SayThenWait(LOCALID_FAREWELL_MAN_AWAITING_DORA, 60);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_GARETS_SISTER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_GARETS_BROTHER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_GARETS_MOTHER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_GARETS_FATHER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(LOCALID_FAREWELL_GARETS_BROTHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_BROTHER, 60);
  // Garet's brother brings the Catch Beads Aunty Dora sent for Isaac; Isaac and
  // Garet turn to follow.
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_PARTY_LEADER,
                                       LOCALID_FAREWELL_GARETS_BROTHER);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_GARET,
                                       LOCALID_FAREWELL_GARETS_BROTHER);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_GARETS_BROTHER, 216, 800);
  ObjectEvent_FaceDirection(LOCALID_FAREWELL_GARETS_BROTHER, FACING_SOUTH, 0);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_BROTHER, 60);
  PartyInventory_GiveItem(ITEM_CATCH_BEADS, 0);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  ObjectEvent_WalkToAndWait(LOCALID_FAREWELL_GARETS_BROTHER, 272, 816);
  ObjectEvent_FaceDirection(LOCALID_FAREWELL_GARETS_BROTHER, FACING_WEST, 0);
  // Garet thinks they are all set; his sister says think again, it must be hard
  // for Dora.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ValePlaza_FaceDirectionAndWait(LOCALID_GARET, FACING_WEST + FACING_STEP, 10);
  EventScript_SayThenWait(LOCALID_GARET, 10);
  ObjectMotion_ArmCallback(LOCALID_FAREWELL_MAN_AWAITING_DORA,
                           FACING_SOUTHEAST + FACING_STEP, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, FACING_EAST, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FAREWELL_GARETS_SISTER, 1);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_SISTER, 10);
  ValePlaza_FaceDirectionAndWait(LOCALID_GARET, FACING_NORTHEAST + FACING_STEP,
                                 10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_GARETS_SISTER,
                                   ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_SISTER, 10);
  // Garet's mother knows how Dora feels: she does not want Isaac to go, but
  // knows he must. The crowd nods.
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_GARETS_MOTHER, ANIM_NOD);
  FieldScript_ShowMessageAndWait(LOCALID_FAREWELL_GARETS_MOTHER, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_MOTHER,
                                 ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageAndWait(LOCALID_FAREWELL_GARETS_MOTHER, 0);
  ObjectMotion_SetVariantCallback(LOCALID_FAREWELL_GARETS_MOTHER, 3);
  FieldScript_ShowMessageAndWait(LOCALID_FAREWELL_GARETS_MOTHER, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_GARETS_SISTER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_FATHER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_BROTHER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 4,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_MOTHER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 7,
                                 ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 8,
                                 ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 5,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER,
                                 ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 1,
                                   ANIM_NOD);
  // Garet's grandfather hops: they will never leave standing around like this,
  // so one final farewell.
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_GRANDFATHER, 2, 10);
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_GRANDFATHER, 4, 40);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_GRANDFATHER, 10);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_GARETS_GRANDFATHER,
                                 FACING_NORTH + FACING_STEP, 10);
  EventScript_SayThenWait(LOCALID_FAREWELL_GARETS_GRANDFATHER, 10);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_GARETS_GRANDFATHER,
                                 FACING_WEST + FACING_STEP, 20);
  ValePlaza_FaceDirectionAndWait(LOCALID_FAREWELL_GARETS_GRANDFATHER,
                                 FACING_SOUTH + FACING_STEP, 10);
  // The crowd nods in three waves, then all jump together: "Farewell!"
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CHILD, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_MAN_AWAITING_DORA, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_BROTHER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 3,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 6,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 9,
                                 ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_SECOND_CHILD, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_GRANDFATHER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_MOTHER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 1,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 4,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 7,
                                 ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GREAT_HEALER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_SISTER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_GARETS_FATHER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 2,
                                 ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 5,
                                 ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 8,
                                   ANIM_NOD);
  FieldScript_WaitFrames(80);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CHILD, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_MAN_AWAITING_DORA, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_BROTHER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 3, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 6, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 9, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_SECOND_CHILD, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_GRANDFATHER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_MOTHER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 1, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 4, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 7, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_GREAT_HEALER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_SISTER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_GARETS_FATHER, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 2, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 5, 4, 0);
  ObjectMotion_Launch(LOCALID_FAREWELL_FIRST_CROWD_VILLAGER + 8, 4, 0);
  FieldMessage_Show(MSGNO(MsgValeFarewell), 1);
  priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  FieldScript_WaitFrames(80);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |= priorityFlags;
  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_GARET);
    u8 value = (priorityFlags | objectEvent->priorityFlags);

    objectEvent->priorityFlags = value;
  }
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ValePlaza_FaceDirectionAndWait(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);
  ValePlaza_FaceDirectionAndWait(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  departureActions = gValePlaza_PartyDepartureActions;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, departureActions);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveWithinMapBounds(PIXELS(216), PIXELS(16), PIXELS(905), 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetActionScript(LOCALID_GARET, departureActions);
  FieldScript_WaitFrames(60);
  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 60;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  Import_ScriptRequestWarp(VALE_PLAZA_TO_WORLD_MAP);
  FieldScript_End();
}

static void EventScript_SayThenWait(s32 speaker, s32 frames) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(frames);
}

static void ValePlaza_FaceDirectionAndWait(s32 localId, s32 facing,
                                           s32 frames) {
  ObjectMotion_ArmCallback(localId, facing, 0);
  FieldScript_WaitFrames(frames);
}

/// A villager calling up Psynergy: drawn SPRITE_COLOR_WHITE for two frames in
/// four, and a pair of sparks every 16 frames.
static void ValePlaza_FlashPsynergyPalette(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 2) != 0) {
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_WHITE);
  } else {
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    ValePlaza_SpawnEffectPair(objectEvent);
  }
}

/// A villager using Psynergy: colour modes SPRITE_COLOR_NORMAL to
/// SPRITE_COLOR_BRG, the sprite and its five swaps of colour channels, in turn,
/// and a pair of sparks every 16 frames.
static void
ValePlaza_CyclePsynergyPaletteAndSpawnEffects(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    ObjectEvent_SetColorMode(objectEvent, palette);
  }
  if ((gFrameCount & 15) == 0) {
    ValePlaza_SpawnEffectPair(objectEvent);
  }
}

/// Cycles the lifted rock through colour modes SPRITE_COLOR_NORMAL to
/// SPRITE_COLOR_BRG, a step every second frame.
static void ValePlaza_CyclePsynergyPalette(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 1) != 0) {
    s32 palette = Math_RemainderUnsigned(gFrameCount >> 1, 6);
    ObjectEvent_SetColorMode(objectEvent, palette);
  }
}

#include "common/effect_vertical.h"

/// Spawns a pair of Psynergy sparks, one rising and one falling
/// (common/effect_vertical.h).
static void ValePlaza_SpawnEffectPair(struct ObjectEvent *parent) {
  struct ObjectEvent *pair[2];
  struct ObjectEvent *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 i;

  Sound_PlaySong(SE_PSYNERGY_SPARKLE);
  for (i = 0; i < 2; ++i) {
    child = ObjectEvent_Create(26, parent->x.fixed, parent->y.fixed,
                               parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        Sprite_StartAnim(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct ObjectEvent *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = 2;
  }
  {
    struct ObjectEvent *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = 2;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

/// Updates the rock-lifting boy's Psynergy display.
///
/// A callback EventScript_BoyLiftsSmallRock adds while the boy lifts.
static void ValePlaza_UpdateRockLiftingBoyPsynergy(void) {
  ValePlaza_CyclePsynergyPaletteAndSpawnEffects(
      ObjectEvent_GetById(LOCALID_STORM_ROCK_LIFTING_BOY));
}

/// Updates the demonstrating strongman's Psynergy display.
///
/// A callback EventScript_DemonstratePsynergy adds while the ground shakes.
static void ValePlaza_UpdateShowingStrongmanPsynergy(void) {
  ValePlaza_CyclePsynergyPaletteAndSpawnEffects(
      ObjectEvent_GetById(LOCALID_STRONGMAN_SHOWING_PSYNERGY));
}

/// Updates the small rock's palette while it is lifted.
///
/// A callback EventScript_BoyLiftsSmallRock adds while LOCALID_STORM_SMALL_ROCK
/// rises.
static void ValePlaza_UpdateSmallRockPalette(void) {
  ValePlaza_CyclePsynergyPalette(ObjectEvent_GetById(LOCALID_STORM_SMALL_ROCK));
}

/// Arriving by VALE_PLAZA_DEBUG_STORY_FLAGS: sets a preset of story flags,
/// which differs by edition, and leaves by VALE_PLAZA_TO_BILIBIN.
///
/// Called by ValePlaza_Initialize. Sets FLAG_NO_ENCOUNTERS and
/// FLAG_DEBUG_BATTLE_KEYS; from the third patch the whole story up to Kalay,
/// before it FLAG_TRET_DEFEATED and FLAG_KOLIMA_CURSE_LIFTED.
static void ValePlaza_SetProgressFlagsAndExit(void) {
  GameFlag_Set(FLAG_NO_ENCOUNTERS);
  GameFlag_Set(FLAG_DEBUG_BATTLE_KEYS);
#if TBS_PATCH_3
  GameFlag_Set(FLAG_VALE_STORM_FIRST_ROCK_FELL);
  GameFlag_Set(FLAG_VALE_STORM_SECOND_ROCK_FELL);
  GameFlag_Set(FLAG_VALE_STORM_THIRD_ROCK_FELL);
  GameFlag_Set(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
  GameFlag_Set(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
  GameFlag_Set(FLAG_VALE_STORM_HEARD_GARET);
  GameFlag_Set(FLAG_VALE_STORM_GARET_JOINED);
  GameFlag_Set(FLAG_VALE_BOULDER_FELL);
  GameFlag_Set(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
  GameFlag_Set(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
  GameFlag_Set(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
  GameFlag_Set(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
  GameFlag_Set(FLAG_VALE_STORM_GARET_LEFT);
  GameFlag_Set(FLAG_VALE_ASKED_IF_GOING_TO_KRADEN);
  GameFlag_Set(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
  GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);
  GameFlag_Set(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
  GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
  GameFlag_Set(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND);
  GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND);
  GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET);
  GameFlag_Set(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_FOUND);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
  GameFlag_Set(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
  GameFlag_Set(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  GameFlag_Set(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
  GameFlag_Set(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
  GameFlag_Set(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
  GameFlag_Set(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
  GameFlag_Set(FLAG_VALE_PARTY_LEFT);
  GameFlag_Set(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  GameFlag_Set(FLAG_GARET_CAME_HOME);
  GameFlag_Set(FLAG_KALAY_REACHED);
#else
  GameFlag_Set(FLAG_TRET_DEFEATED);
  GameFlag_Set(FLAG_KOLIMA_CURSE_LIFTED);
#endif
  Import_ScriptRequestWarp(VALE_PLAZA_TO_BILIBIN);
}

// clang-format off
/// Unused: nothing refers to it. A run of palette numbers, 0 to 6 three times
/// each and then 0 to 5 twice each, as a palette cycle of the Psynergy
/// displays would step through them.
u8 gValePlaza_UnusedPaletteSteps[] = {
    0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
    5, 5, 6, 6, 6, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5,
};

s32 gValePlaza_ChargedUpStrongmanActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(214), 0, PIXELS(721)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(40),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_MOTION, 3),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(214), 0, PIXELS(763)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gValePlaza_SecondTravelerLeapActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(12)),
    SC_MOVE(PIXELS(114), PIXELS(144), PIXELS(360)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gValePlaza_FirstTravelerLeapActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(12)),
    SC_MOVE(PIXELS(114), PIXELS(144), PIXELS(343)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

s32 gValePlaza_LeaderActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(82), PIXELS(144), PIXELS(343)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gValePlaza_StormGaretLeapActions[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(82), PIXELS(144), PIXELS(360)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gValePlaza_FirstTravelerBounceActions[] = {
    SC_ANIMATION(5),
    SC_WAIT(10),
    SC_ANIMATION(6),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_MOVE(PIXELS(65), 0, PIXELS(328)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ANIMATION(5),
    SC_WAIT(10),
    SC_ANIMATION(6),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_MOVE(PIXELS(31), 0, PIXELS(338)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ANIMATION(5),
    SC_WAIT(10),
    SC_ANIMATION(6),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(0, 0, PIXELS(338)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gValePlaza_SecondTravelerBounceActions[] = {
    SC_ANIMATION(5),
    SC_WAIT(10),
    SC_ANIMATION(6),
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_MOVE(PIXELS(63), 0, PIXELS(374)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ANIMATION(5),
    SC_WAIT(10),
    SC_ANIMATION(6),
    SC_SET(SW_VELOCITY_Y, PIXELS(7)),
    SC_MOVE(PIXELS(17), 0, PIXELS(338)),
    SC_SYNC,
    SC_WAIT(5),
    SC_ANIMATION(5),
    SC_WAIT(10),
    SC_ANIMATION(6),
    SC_SE(SE_HIGH_JUMP),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(0, 0, PIXELS(338)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gValePlaza_FaceTargetActions[] = {
    SC_CALL(Map_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValePlaza_FarewellDogActions[] = {
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

s32 gValePlaza_FarewellChaseActions[] = {
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

s32 gValePlaza_FarewellFirstChildActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(137), 0, PIXELS(639)),
    SC_SYNC,
    SC_MOVE(PIXELS(122), 0, PIXELS(692)),
    SC_SYNC,
    SC_MOVE(PIXELS(141), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(6) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(3) / 5),
    SC_MOVE(PIXELS(148), 0, PIXELS(770)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gValePlaza_FarewellSecondChildActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(137), 0, PIXELS(639)),
    SC_SYNC,
    SC_MOVE(PIXELS(122), 0, PIXELS(692)),
    SC_SYNC,
    SC_MOVE(PIXELS(141), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(6) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(3) / 5),
    SC_MOVE(PIXELS(164), 0, PIXELS(758)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gValePlaza_PartyDepartureActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(854)),
    SC_SYNC,
    SC_MOVE(PIXELS(215), 0, PIXELS(921)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gValePlaza_HutOwnerSearchActions[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 20),
    SC_SET(SW_COLLISION, 3),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gValePlaza_AideRunActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(148), 0, PIXELS(758)),
    SC_SYNC,
    SC_MOVE(PIXELS(128), 0, PIXELS(709)),
    SC_SYNC,
    SC_MOVE(PIXELS(129), 0, PIXELS(647)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gValePlaza_LeaderStepActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(774)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gValePlaza_StormGaretStepActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(790)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

struct MapWarpDest gValePlaza_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_VALE_INTERIORS_NORTH, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_WOODEN_ROOMS_A_NORTH, CONDITION_ALWAYS, 352, 0, 308, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_VALE_INTERIORS_WEST, CONDITION_ALWAYS, 230, 0, 423, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_VALE_INTERIORS_EAST, CONDITION_ALWAYS, 374, 0, 435, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_VALE_INTERIORS_SOUTH, CONDITION_ALWAYS, 200, 0, 562, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_WOODEN_ROOMS_A_WEST, CONDITION_ALWAYS, 102, 0, 627, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_WOODEN_ROOMS_A_SOUTH, CONDITION_ALWAYS, 358, 0, 678, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_VALE, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_WORLD_MAP, CONDITION_ALWAYS, 217, 0, 843, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 186, 0, 474, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_FROM_SANCTUM, CONDITION_ALWAYS, 180, 0, 618, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_GOMA_CAVE_STREAM_UNUSED, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_PLAZA_AFTER_TRAVELERS_BATTLE, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gValePlaza_Warps[] = {
    MAP_WARPS(ValePlaza),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_EAST, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_EAST),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_NORTH, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_NORTHEAST),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_NORTH, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_NORTH),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_SOUTH, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_SOUTH),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_WEST, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_WEST),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_NORTHWEST, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_NORTHWEST),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTH, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_WEST),
    MAP_WARP(VALE_PLAZA_TO_VALE, Vale, VALE_FROM_VALE_PLAZA),
    MAP_WARP(VALE_PLAZA_TO_TITLE, Title, TITLE_FROM_VALE_PLAZA),
    MAP_WARP(VALE_PLAZA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_VALE_PLAZA),
    MAP_WARP(VALE_PLAZA_TO_GOMA_CAVE_STREAM, GomaCaveStream, GOMA_CAVE_STREAM_FROM_VALE_PLAZA),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTHEAST, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_SOUTHEAST),
#if !TBS_PATCH_3
    MAP_WARP(VALE_PLAZA_TO_BILIBIN, Bilibin, BILIBIN_FROM_WORLD_MAP),
#else
    MAP_WARP(VALE_PLAZA_TO_BILIBIN, SolSanctumStatue, SOL_SANCTUM_STATUE_FROM_VALE_PLAZA),
#endif
    MAP_WARPS_END,
};

u32 gValePlaza_StormWarps[] = {
    MAP_WARPS(ValePlaza),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_EAST, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_EAST),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_NORTH, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_NORTHEAST),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_NORTH, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_NORTH),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_SOUTH, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_SOUTH),
    MAP_WARP(VALE_PLAZA_TO_VALE_INTERIORS_WEST, ValeInteriors, VALE_INTERIORS_FROM_VALE_PLAZA_WEST),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_NORTHWEST, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_NORTHWEST),
    MAP_WARP(VALE_PLAZA_TO_WOODEN_ROOMS_A_SOUTH, WoodenRoomsA, WOODEN_ROOMS_A_FROM_VALE_PLAZA_WEST),
    MAP_WARP(VALE_PLAZA_TO_VALE, ValeStormyNight, VALE_STORMY_NIGHT_FROM_VALE_PLAZA),
    MAP_WARP(VALE_PLAZA_TO_TITLE, Title, TITLE_FROM_VALE_PLAZA),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gValePlaza_ObjectEventTemplates[] = {
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, 3,
        PIXELS(392), 0, PIXELS(712), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(616), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(760), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(168), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(600), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(824), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(568), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHWEST, 3, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(240), 0, PIXELS(760), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(680), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(370), 0, PIXELS(319), FACING_WEST, TALK_FACING_PARTY, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { JENNA_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValePlaza_StormObjectEventTemplates[] = {
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, 3,
        PIXELS(249), 0, PIXELS(628), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(214), 0, PIXELS(635), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(294), 0, PIXELS(670), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, 3,
        PIXELS(136), 0, PIXELS(678), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(87), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(173), 0, PIXELS(568), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(213), 0, PIXELS(771), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
#if TBS_PATCH_4
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(198), 0, PIXELS(779), FACING_NORTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
#else
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(204), 0, PIXELS(783), FACING_NORTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
#endif
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(250), 0, PIXELS(773), FACING_SOUTHWEST, 3, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(729), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(729), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(215), 0, PIXELS(709), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { GARET_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { JENNA_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(234), 0, PIXELS(786), FACING_NORTHEAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValePlaza_DepartureObjectEventTemplates[] = {
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(225), 0, PIXELS(814), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(607), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(607), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(199), 0, PIXELS(781), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(180), 0, PIXELS(790), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(778), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(262), 0, PIXELS(799), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(270), 0, PIXELS(814), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(787), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(234), 0, PIXELS(781), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(268), 0, PIXELS(742), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(286), 0, PIXELS(761), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(271), 0, PIXELS(778), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(762), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(227), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(209), 0, PIXELS(760), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(288), 0, PIXELS(795), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(161), 0, PIXELS(790), FACING_EAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(171), 0, PIXELS(815), FACING_EAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(191), 0, PIXELS(758), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValePlaza_ObjectEventTemplatesAfterKalay[] = {
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(186), 0, PIXELS(460), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(560), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(217), 0, PIXELS(804), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(107), 0, PIXELS(765), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(267), 0, PIXELS(744), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(681), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, 3,
        PIXELS(391), 0, PIXELS(716), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(617), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(166), 0, PIXELS(603), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(176), 0, PIXELS(568), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(315), 0, PIXELS(823), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(287), 0, PIXELS(571), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(370), 0, PIXELS(319), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gValePlaza_Events[] = {
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeQuietDay) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeKradenHadVisitors) },
    { MAP_EVENT_TALK, LOCALID_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutMeditation) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RecallThreeYearsAgo) },
    { MAP_EVENT_TALK, LOCALID_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeChosenPeople) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeProudOfAdeptGrandfather) },
    { MAP_EVENT_TALK, LOCALID_FIRST_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutTheTravelers) },
    { MAP_EVENT_TALK, LOCALID_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValePracticePsynergy) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DemonstratePsynergy) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeWalkToRecoverPsynergy) },
    { MAP_EVENT_TALK, LOCALID_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeFoodStoredForWinter) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenNorthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralWestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralEastDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSouthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenWestEdgeDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSoutheastDoor) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughTownGate) },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, VALE_PLAZA_TO_VALE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, VALE_PLAZA_TO_GOMA_CAVE_STREAM },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY, (FLAG_VALE_FIRST_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwayFirstIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_SECOND_IVY, (FLAG_VALE_SECOND_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwaySecondIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_THIRD_IVY, (FLAG_VALE_THIRD_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwayThirdIvy) },
    { BG_EVENT(BG_EVENT_SILENT), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UsePsynergyStone) },
    { MAP_EVENTS_END },
};

struct MapEvent gValePlaza_StormEvents[] = {
    { MAP_EVENT_TALK, LOCALID_STORM_CURIOUS_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutJourney) },
    { MAP_EVENT_TALK, LOCALID_STORM_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeOldWomanCannotUsePsynergy) },
    { MAP_EVENT_TALK, LOCALID_STORM_ROCK_DEFLECTING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgValeExhaustedFromFallingRocks) },
    { MAP_EVENT_TALK, LOCALID_STORM_WOMAN_HEARING_OF_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixFellInRiver) },
    { MAP_EVENT_TALK, LOCALID_STORM_ROCK_LIFTING_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BoyLiftsSmallRock) },
    { MAP_EVENT_TALK, LOCALID_STORM_GIRL_WHO_SAW_JENNA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutHelping) },
    { MAP_EVENT_TALK, LOCALID_STORM_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeRaceAgainstTime) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenNorthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralWestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralEastDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSouthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenWestEdgeDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSoutheastDoor) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughWestPassage) },
    { MAP_EVENT_COORD, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SentToAidElders) },
    { BG_EVENT(BG_EVENT_SILENT), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UsePsynergyStone) },
    { MAP_EVENTS_END },
};

struct MapEvent gValePlaza_DepartureEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gValePlaza_EventsAfterLeaving[] = {
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeTremorsSettled) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeRoadToVault) },
    { MAP_EVENT_TALK, LOCALID_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, MSGNO(MsgValeRoadToGomaRange) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, MSGNO(MsgValePsynergyStonePowder) },
    { MAP_EVENT_TALK, LOCALID_CURIOUS_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutJourney) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeLunpaFoundedNorthernTown) },
    { MAP_EVENT_TALK, LOCALID_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeDoraHasBeenCrying) },
    { MAP_EVENT_TALK, LOCALID_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeStonesScatteredByEruption) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DemonstratePsynergy) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeReplenishPsynergyBeforeDungeon) },
    { MAP_EVENT_TALK, LOCALID_HUT_OWNER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutFallenStone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeWorriedAboutAnotherEruption) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeVaultUndergroundPath) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, MSGNO(MsgValeGomaWinterCold) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, MSGNO(MsgValeStonesPowerAleph) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeProofOfWiseOne) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeLunpaThievesChanged) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeDoraMustBeLonely) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeLuckyToFindPsynergyStones) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAlive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeRecoverPsynergyByWalking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeStonesReplenishPsynergy) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenNorthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralWestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralEastDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSouthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenWestEdgeDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSoutheastDoor) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughTownGate) },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, VALE_PLAZA_TO_VALE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, VALE_PLAZA_TO_GOMA_CAVE_STREAM },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY, (FLAG_VALE_FIRST_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwayFirstIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_SECOND_IVY, (FLAG_VALE_SECOND_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwaySecondIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_THIRD_IVY, (FLAG_VALE_THIRD_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwayThirdIvy) },
    { BG_EVENT(BG_EVENT_SILENT), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UsePsynergyStone) },
    { MAP_EVENTS_END },
};

struct MapEvent gValePlaza_EventsAfterKalay[] = {
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WelcomeIsaacHome) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_WOMAN_BACK_SOONER, CONDITION_ALWAYS, MSGNO(MsgValeCameBackSooner) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_MAN_ON_NEW_FRIENDS, CONDITION_ALWAYS, MSGNO(MsgValeMadeFriendsOnJourney) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_WOMAN_ON_HOT_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeRiverHeatedAndCooled) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutJourney) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeReturnHowHaveYouBeen) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHome) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWantToMakeFriends) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_STRONGMAN_ON_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeEruptionMadeMess) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_STRONGMAN_ON_CRACK, CONDITION_ALWAYS, MSGNO(MsgValeNorthwestCrack) },
    { MAP_EVENT_TALK, LOCALID_AFTER_KALAY_HUT_OWNER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PsynergyStoneIsGone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgValeSurprisedByEarlyReturn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_WOMAN_BACK_SOONER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacHasGrown) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_MAN_ON_NEW_FRIENDS, CONDITION_ALWAYS, MSGNO(MsgValeNewFriendsAreAdepts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_WOMAN_ON_HOT_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeEruptionHeatsRiver) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeStrangeEventsElsewhere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeWhenDidYouReturn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValePlayTogetherAgain) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWantToJourney) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_STRONGMAN_ON_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeLavaBurntOtherSide) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_STRONGMAN_ON_CRACK, CONDITION_ALWAYS, MSGNO(MsgValeCrackWidening) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_AFTER_KALAY_HUT_OWNER, FLAG_VALE_PSYNERGY_STONE_GONE_TOLD, EVENT_SCRIPT(EventScript_PsynergyStoneIsGone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_AFTER_KALAY_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeEruptionTooMuchForPeople) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenNorthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenNortheastDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralWestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenCentralEastDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSouthwestDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenWestEdgeDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSoutheastDoor) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughTownGate) },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, VALE_PLAZA_TO_VALE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, VALE_PLAZA_TO_GOMA_CAVE_STREAM },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY, (FLAG_VALE_FIRST_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwayFirstIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_SECOND_IVY, (FLAG_VALE_SECOND_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwaySecondIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_THIRD_IVY, (FLAG_VALE_THIRD_IVY_BLOWN_AWAY), EVENT_SCRIPT(EventScript_BlowAwayThirdIvy) },
    { BG_EVENT(BG_EVENT_SILENT), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UsePsynergyStone) },
    { MAP_EVENTS_END },
};

u16 gValePlaza_DoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 57, 2, 2, 1), MAPGRID_ANIMATION_STEP(0, 59, 2, 2, 1), MAPGRID_ANIMATION_STEP(2, 59, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};
// clang-format on
