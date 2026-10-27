#include "edition.h"
#ifndef TOLBI_H
#define TOLBI_H

#include "types.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "window.h"
#include "scene.h"
#include "heap.h"

#define NULL ((void *)0)

#include "shared/colosso_story_flags.h"
#include "flags.h"

/// Growth has raised the sprout into a vine the leader can climb.
/// ★ Named from what the player sees: a small green sprout at the foot of a
/// cliff (its render)
#define FLAG_TOLBI_SPROUT_GROWN FLAG_SCENE_TEMP_00
/// Frost has frozen the puddle into an ice pillar.
/// ★ Named from what the player sees: a small puddle of water just north of the
/// sprout (its render)
#define FLAG_TOLBI_PUDDLE_FROZEN FLAG_SCENE_TEMP_01

// Scene flags: FLAG_TOLBI_SPROUT_GROWN is set once Growth has raised the
// sprout, FLAG_TOLBI_PUDDLE_FROZEN once Frost has frozen the puddle; both reset
// when the party leaves the town.

/// Actors, numbered in the order of the actor templates from
/// ACTOR_FIRST_PLACED. The townspeople the text gives no name are named by
/// what their sprites show and what they say or, under Mind Read, think
/// during the festival.
enum TolbiActorId {
  /// The sprout Growth grows into a vine (Tolbi_GrowSprout).
  /// ★ Named from what the player sees: a small green sprout (SPRITE_SPROUT)
  /// at the foot of a cliff (its render)
  ACTOR_TOLBI_SPROUT = 8,
  /// The puddle Frost freezes into an ice pillar (Tolbi_FreezePuddle).
  /// ★ Named from what the player sees: a small puddle of water
  /// (SPRITE_PUDDLE) just north of the sprout (its render)
  ACTOR_TOLBI_PUDDLE = 9,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "This festival only happens once a year, so I'm
  /// really excited." (MsgTolbiTolbisAlwaysReallyBustlingDuringColosso)
  ACTOR_TOLBI_EXCITED_YOUNG_MAN = 10,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "If we don't go right now, we won't get good
  /// seats!" (MsgTolbiColossoIsHeldInTheColosseum)
  ACTOR_TOLBI_HURRYING_WOMAN = 11,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR); after
  /// Colosso he asks whether the party was in the finals
  /// (Tolbi_AskWhetherPartyWasInFinals).
  /// ★ Named from the game: "I'm planning to go watch the finals."
  /// (MsgTolbiTryoutsStartedAtTheColosseumToday)
  ACTOR_TOLBI_FINALS_FAN = 12,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "What if something's happened to our leader?"
  /// (MsgTolbiColossoShouldHaveStartedTodayWhat)
  ACTOR_TOLBI_WORRIED_WOMAN = 13,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who says he is 65 and
  /// later talks of the Lucky Guess. He copies the leader's sprite priority
  /// every frame (Tolbi_CopyLeaderPriority).
  /// ★ Named from the game: "I'm 65 years old now"
  /// (MsgTolbiBabiWasAlreadyPrettyOldBack)
  ACTOR_TOLBI_OLD_MAN = 14,
  /// An old woman (SPRITE_OLD_WOMAN) who asks whether someone can live past
  /// a hundred (Tolbi_AskAboutLivingPastHundred).
  /// ★ Named from the game: "What if Babi is actually a totally different
  /// person now" (MsgTolbiWhatIfBabiIsActuallyA)
  ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN = 15,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who runs after
  /// his sister.
  /// ★ Named from the game: "I'm the oldest, so I shouldn't have to share"
  /// (MsgTolbiOldestShouldntShare)
  ACTOR_TOLBI_OLDER_BROTHER = 16,
  /// A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS).
  /// ★ Named from the game: "My big brother is being nice to me again!"
  /// (MsgTolbiHaHaHa)
  ACTOR_TOLBI_YOUNGER_SISTER = 17,
  /// A strongman (SPRITE_STRONGMAN).
  /// ★ Named from the game: "Get your Colosso souvenirs!"
  /// (MsgTolbiGetYourColossoSouvenirsTheyMake)
  ACTOR_TOLBI_SOUVENIR_SELLER = 18,
  /// A stout man (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "Get your snacks and drinks"
  /// (MsgTolbiGetYourSnacksAndDrinksBefore)
  ACTOR_TOLBI_SNACK_VENDOR = 19,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND).
  /// ★ Named from the game: "I've decided not to fight at Colosso after all."
  /// (MsgTolbiIveDecidedNotToFightAt)
  ACTOR_TOLBI_WOULD_BE_FIGHTER = 20,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND), whom Mind Read
  /// finds thinking of his home.
  /// ★ Named from the game: "Tolbi is not like Xian."
  /// (MsgTolbiTolbiIsNotLikeXianIt)
  ACTOR_TOLBI_XIAN_VISITOR = 21,
  /// A woman with a sword (SPRITE_PERSON_WITH_SWORD), beside a second whose
  /// row has no name.
  /// ★ Named from the game: "Rats! We overslept"
  /// (MsgTolbiRatsWeOversleptAndNowThe)
  ACTOR_TOLBI_OVERSLEEPER = 22,
  /// A family at the festival: the father, a burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR), the mother, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS), and their small boy
  /// (SPRITE_BOY_WITH_FAIR_HAIR), who cries for sweets.
  /// ★ Named from the game: "the wife doesn't go in for"
  /// (MsgTolbiChildrenLoveSouvenirs)
  ACTOR_TOLBI_FATHER = 24,
  /// ★ Named from the game: "Be patient, little guy."
  /// (MsgTolbiPatientLittleGuy)
  ACTOR_TOLBI_MOTHER = 25,
  /// ★ Named from the game: "Mom, you said you were all out of money"
  /// (MsgTolbiMomYouSaidYouWereAll)
  ACTOR_TOLBI_CRYING_BOY = 26,
  /// A strongman (SPRITE_STRONGMAN) who sleeps under the archway during the
  /// festival; on finals day he sells seats.
  /// ★ Named from the game: "us scalpers" (MsgTolbiTodayIsABigDayFor)
  ACTOR_TOLBI_SCALPER = 27,
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) at the gate of Tolbi
  /// Spring, who lets the party go first.
  /// ★ Named from the game: "Do you want to test your luck at Tolbi Spring?"
  /// (MsgTolbiWantTestLuck)
  ACTOR_TOLBI_SPRING_VISITOR = 28,
  /// Babi's soldiers (SPRITE_SOLDIER), searching the town for him and
  /// showing his picture (Tolbi_MeetSearchingSoldiers).
  /// ★ Named from the game: "Did you run into any of Babi's soldiers in
  /// Town?" (MsgTolbiRunBabisSoldiers)
  ACTOR_TOLBI_SOLDIER_A = 29,
  /// The second of the two soldiers.
  /// ★ Named from the game: "Did you run into any of Babi's soldiers in
  /// Town?" (MsgTolbiRunBabisSoldiers)
  ACTOR_TOLBI_SOLDIER_B = 30,
  /// Ember, a Mars Djinni, until it joins the party (Tolbi_JoinEmber).
  /// ★ Named from the game: "Ember" (MsgDjinnName)
  ACTOR_TOLBI_EMBER = 31,
  /// The "Missing Person" poster, placed with Babi's sprite (SPRITE_BABI).
  /// ★ Named from the game: "Missing Person" (MsgTolbiSeenAnyoneWho)
  ACTOR_TOLBI_MISSING_PERSON_POSTER = 32,
};

/// The metatile behaviors Tolbi's coord and examine triggers answer besides the
/// doors, whose own cells carry the numbers of their exits (Tolbi_EnterDoor,
/// Tolbi_RequestTouchedExit).
enum TolbiMetatileBehavior {
  /// Where Babi's soldiers stop the party during the festival.
  MB_TOLBI_SOLDIERS = 20,
  /// The first of the two signs beside the gate to Tolbi Spring.
  /// ★ Named from the game: "Face away from Tolbi Spring and toss a coin into
  /// it." (MsgTolbiFaceAwayTolbi)
  MB_TOLBI_SPRING_SIGN = 30,
  /// ★ Named from the game: "If you toss a Lucky Medal"
  /// (MsgTolbiTossLuckyMedal)
  MB_TOLBI_LUCKY_MEDAL_SIGN = 31,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_TOLBI_BARREL = 100,
  MB_TOLBI_JAR = 101,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_TOLBI_HIDDEN_CHEST = 102,
  /// The gate to Tolbi Spring, open once the visitor at the gate has stepped
  /// aside.
  MB_TOLBI_GATE_TO_SPRING = 239
};

/// The running children keep these flags in Actor.timer, their scene
/// scratch halfword. Bits: 1 talking | 0 chases the younger sister.
enum {
  /// Set on the older brother, who runs after ACTOR_TOLBI_YOUNGER_SISTER.
  KID_CHASES_YOUNGER = 1,
  KID_TALKING = 2, ///< set while the party talks to this child
};

#endif
