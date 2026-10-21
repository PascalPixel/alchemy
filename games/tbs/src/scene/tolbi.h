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
  /// A young man with brown hair (SPRITE_BOY_WITH_BROWN_HAIR): "This festival
  /// only happens once a year, so I'm really excited."
  TOLBI_EXCITED_YOUNG_MAN = 10,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR): "If we don't go
  /// right now, we won't get good seats!"
  TOLBI_HURRYING_WOMAN = 11,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR): "I'm planning
  /// to go watch the finals."; after Colosso, asks whether the
  /// party was in them (Tolbi_AskWhetherPartyWasInFinals).
  TOLBI_FINALS_FAN = 12,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS): "I saw Babi's
  /// bodyguards wandering around. What if something's happened
  /// to our leader?"
  TOLBI_WORRIED_WOMAN = 13,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE): "I'm 65 years old now,
  /// so just imagine how old Babi must be!"; later he talks of the Lucky Guess.
  /// Copies the leader's sprite priority every frame
  /// (Tolbi_CopyLeaderPriority).
  TOLBI_OLD_MAN = 14,
  /// An old woman (SPRITE_OLD_WOMAN): "Can someone live to be more than 100
  /// years old?"
  /// (Tolbi_AskAboutLivingPastHundred); Mind Read finds the thought "What if
  /// Babi is actually a totally different person now".
  TOLBI_SUSPICIOUS_OLD_WOMAN = 15,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who runs after
  /// his sister: "I'm the oldest, so I shouldn't have to share my
  /// allowance."
  TOLBI_OLDER_BROTHER = 16,
  /// A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS): "My big brother
  /// is being nice to me again!"
  TOLBI_YOUNGER_SISTER = 17,
  /// A strongman (SPRITE_STRONGMAN): "Get your Colosso souvenirs! They make
  /// great gifts!"
  TOLBI_SOUVENIR_SELLER = 18,
  /// A stout man (SPRITE_STOUT_MAN): "Get your snacks and drinks before you get
  /// in the colosseum!"
  TOLBI_SNACK_VENDOR = 19,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND): "I've decided
  /// not to fight at Colosso after all."
  TOLBI_WOULD_BE_FIGHTER = 20,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND): "Wow, Tolbi's
  /// festival is humongous!"; Mind Read finds the thought
  /// "Tolbi is not like Xian. It is very lively here!"
  TOLBI_XIAN_VISITOR = 21,
  /// A woman with a sword (SPRITE_WOMAN_WITH_SWORD), beside a second whose
  /// row has no name: "Rats! We overslept, and now the colosseum is full!"
  TOLBI_OVERSLEEPER = 22,
  /// A family at the festival: the father, a burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR), the mother, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS), and their small boy
  /// (SPRITE_BOY_WITH_FAIR_HAIR), who cries for sweets. The father: "the wife
  /// doesn't go in for that sort of thing"; the boy: "Mom, you said you were
  /// all out of money".
  TOLBI_FATHER = 24,
  TOLBI_MOTHER = 25,
  TOLBI_CRYING_BOY = 26,
  /// A strongman (SPRITE_STRONGMAN) who sleeps under the archway during the
  /// festival; on finals day he sells
  /// seats. "Today is a big day for us scalpers"
  TOLBI_SCALPER = 27,
  TOLBI_GAMBLER = 28, ///< a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK)
  TOLBI_SOLDIER_A = 29,
  TOLBI_SOLDIER_B = 30,
  TOLBI_EMBER = 31,
  /// The "Missing Person" poster, placed with Babi's sprite (SPRITE_BABI).
  TOLBI_MISSING_PERSON_POSTER = 32,
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
