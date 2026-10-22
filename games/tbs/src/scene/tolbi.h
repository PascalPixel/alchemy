#include "edition.h"
#ifndef TOLBI_H
#define TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "window.h"
#include "field_runtime.h"
#include "heap_state.h"

#define NULL ((void *)0)

#include "common/colosso_story_flags.h"
#include "flags.h"

// Scene flags: FLAG_SCENE_TEMP_00 is set once Growth has raised the sprout,
// FLAG_SCENE_TEMP_01 once Frost has frozen the puddle; both reset when the
// party leaves the town.

/// Actors, numbered in the order of the placement table from
/// ACTOR_FIRST_PLACED. The townspeople the text gives no name are named by
/// what their sprites show and what they say or, under Mind Read, think
/// during the festival.
enum TolbiActor {
  TOLBI_SPROUT = 8,
  TOLBI_PUDDLE = 9,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "This festival only happens once a year, so I'm
  /// really excited." (MsgTolbiTolbisAlwaysReallyBustlingDuringColosso)
  TOLBI_EXCITED_YOUNG_MAN = 10,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "If we don't go right now, we won't get good
  /// seats!" (MsgTolbiColossoIsHeldInTheColosseum)
  TOLBI_HURRYING_WOMAN = 11,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR); after
  /// Colosso he asks whether the party was in the finals
  /// (Tolbi_AskWhetherPartyWasInFinals).
  /// ★ Named from the game: "I'm planning to go watch the finals."
  /// (MsgTolbiTryoutsStartedAtTheColosseumToday)
  TOLBI_FINALS_FAN = 12,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "What if something's happened to our leader?"
  /// (MsgTolbiColossoShouldHaveStartedTodayWhat)
  TOLBI_WORRIED_WOMAN = 13,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who says he is 65 and
  /// later talks of the Lucky Guess. He copies the leader's sprite priority
  /// every frame (Tolbi_CopyLeaderPriority).
  TOLBI_OLD_MAN = 14,
  /// An old woman (SPRITE_OLD_WOMAN) who asks whether someone can live past
  /// a hundred (Tolbi_AskAboutLivingPastHundred).
  /// ★ Named from the game: "What if Babi is actually a totally different
  /// person now" (MsgTolbiWhatIfBabiIsActuallyA)
  TOLBI_SUSPICIOUS_OLD_WOMAN = 15,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who runs after
  /// his sister.
  /// ★ Named from the game: "I'm the oldest, so I shouldn't have to share"
  /// (MsgTolbiOldestShouldntShare)
  TOLBI_OLDER_BROTHER = 16,
  /// A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS).
  /// ★ Named from the game: "My big brother is being nice to me again!"
  /// (MsgTolbiHaHaHa)
  TOLBI_YOUNGER_SISTER = 17,
  /// A strongman (SPRITE_STRONGMAN).
  /// ★ Named from the game: "Get your Colosso souvenirs!"
  /// (MsgTolbiGetYourColossoSouvenirsTheyMake)
  TOLBI_SOUVENIR_SELLER = 18,
  /// A stout man (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "Get your snacks and drinks"
  /// (MsgTolbiGetYourSnacksAndDrinksBefore)
  TOLBI_SNACK_VENDOR = 19,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND).
  /// ★ Named from the game: "I've decided not to fight at Colosso after all."
  /// (MsgTolbiIveDecidedNotToFightAt)
  TOLBI_WOULD_BE_FIGHTER = 20,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND), whom Mind Read
  /// finds thinking of his home.
  /// ★ Named from the game: "Tolbi is not like Xian."
  /// (MsgTolbiTolbiIsNotLikeXianIt)
  TOLBI_XIAN_VISITOR = 21,
  /// A woman with a sword (SPRITE_PERSON_WITH_SWORD), beside a second whose
  /// row has no name.
  /// ★ Named from the game: "Rats! We overslept"
  /// (MsgTolbiRatsWeOversleptAndNowThe)
  TOLBI_OVERSLEEPER = 22,
  /// A family at the festival: the father, a burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR), the mother, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS), and their small boy
  /// (SPRITE_BOY_WITH_FAIR_HAIR), who cries for sweets.
  /// ★ Named from the game: "the wife doesn't go in for"
  /// (MsgTolbiChildrenLoveSouvenirs)
  TOLBI_FATHER = 24,
  TOLBI_MOTHER = 25,
  /// ★ Named from the game: "Mom, you said you were all out of money"
  /// (MsgTolbiMomYouSaidYouWereAll)
  TOLBI_CRYING_BOY = 26,
  /// A strongman (SPRITE_STRONGMAN) who sleeps under the archway during the
  /// festival; on finals day he sells seats.
  /// ★ Named from the game: "us scalpers" (MsgTolbiTodayIsABigDayFor)
  TOLBI_SCALPER = 27,
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) at the gate of Tolbi
  /// Spring, who lets the party go first.
  /// ★ Named from the game: "Do you want to test your luck at Tolbi Spring?"
  /// (MsgTolbiWantTestLuck)
  TOLBI_SPRING_VISITOR = 28,
  TOLBI_SOLDIER_A = 29, ///< Babi's soldiers, searching the town for him
  TOLBI_SOLDIER_B = 30,
  TOLBI_EMBER = 31,
  /// The "Missing Person" poster, placed with Babi's sprite (SPRITE_BABI).
  /// ★ Named from the game: "Missing Person" (MsgTolbiSeenAnyoneWho)
  TOLBI_MISSING_PERSON_POSTER = 32,
};

/// The touch cells whose events are more than a door. The doors' own cells
/// carry the numbers of their exits (Tolbi_EnterDoor,
/// Tolbi_RequestTouchedExit).
enum TolbiCell {
  /// Where Babi's soldiers stop the party during the festival.
  TOLBI_CELL_SOLDIERS = 20,
  /// The gate to Tolbi Spring, open once the visitor at the gate has stepped
  /// aside.
  TOLBI_CELL_SPRING_GATE = 239
};

/// The two signs beside the gate to Tolbi Spring.
enum TolbiSign {
  /// ★ Named from the game: "Face away from Tolbi Spring and toss a coin into
  /// it." (MsgTolbiFaceAwayTolbi)
  TOLBI_SPRING_SIGN = 30,
  /// ★ Named from the game: "If you toss a Lucky Medal"
  /// (MsgTolbiTossLuckyMedal)
  TOLBI_LUCKY_MEDAL_SIGN = 31
};

/// Map objects the party can search.
enum TolbiMapObject {
  TOLBI_BARREL = 100,
  TOLBI_JAR = 101,
  TOLBI_HIDDEN_CHEST = 102,
};

/// A word of the event work, at offset 0x1f4, that is set to the poster
/// before the yes/no choice under the "Missing Person" window. What reads it
/// is not yet traced.
#define EVENT_WORK_1F4 (*(s32 *)((u8 *)gEventWork + 0x1f4))

/// The running children keep these flags in FieldActor.timer, their scene
/// scratch halfword. Bits: 1 talking | 0 chases the younger sister.
enum {
  /// Set on the older brother, who runs after TOLBI_YOUNGER_SISTER.
  KID_CHASES_YOUNGER = 1,
  KID_TALKING = 2, ///< set while the party talks to this child
};

#endif
