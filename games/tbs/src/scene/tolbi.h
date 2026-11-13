#include "edition.h"
#ifndef ALCHEMY_TOLBI_H
#define ALCHEMY_TOLBI_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "window.h"
#include "scene.h"
#include "heap.h"

#include "shared/colosso_story_flags.h"
#include "flags.h"

/// Growth has raised the sprout into a vine the leader can climb.
#define FLAG_TOLBI_SPROUT_GROWN FLAG_SCENE_TEMP_00
/// Frost has frozen the puddle into an ice pillar.
#define FLAG_TOLBI_PUDDLE_FROZEN FLAG_SCENE_TEMP_01

// Scene flags: FLAG_TOLBI_SPROUT_GROWN is set once Growth has raised the
// sprout, FLAG_TOLBI_PUDDLE_FROZEN once Frost has frozen the puddle; both reset
// when the party leaves the town.

/// Actors, numbered in the order of the actor placements from
/// ACTOR_FIRST_PLACED. The townspeople the text gives no name are named by
/// what their sprites show and what they say or, under Mind Read, think
/// during the festival.
enum TolbiActorId {
  /// The sprout Growth grows into a vine (Tolbi_GrowSprout).
  ACTOR_TOLBI_SPROUT = 8,
  /// The puddle Frost freezes into an ice pillar (Tolbi_FreezePuddle).
  ACTOR_TOLBI_PUDDLE = 9,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  ACTOR_TOLBI_EXCITED_YOUNG_MAN = 10,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_TOLBI_HURRYING_WOMAN = 11,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR); after
  /// Colosso he asks whether the party was in the finals
  /// (Tolbi_AskWhetherPartyWasInFinals).
  ACTOR_TOLBI_FINALS_FAN = 12,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_TOLBI_WORRIED_WOMAN = 13,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who says he is 65 and
  /// later talks of the Lucky Guess. He copies the leader's sprite priority
  /// every frame (Tolbi_CopyLeaderPriority).
  ACTOR_TOLBI_OLD_MAN = 14,
  /// An old woman (SPRITE_OLD_WOMAN) who asks whether someone can live past
  /// a hundred (Tolbi_AskAboutLivingPastHundred).
  ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN = 15,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who runs after
  /// his sister.
  ACTOR_TOLBI_OLDER_BROTHER = 16,
  /// A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS).
  ACTOR_TOLBI_YOUNGER_SISTER = 17,
  /// A strongman (SPRITE_STRONGMAN).
  ACTOR_TOLBI_SOUVENIR_SELLER = 18,
  /// A stout man (SPRITE_STOUT_MAN).
  ACTOR_TOLBI_SNACK_VENDOR = 19,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND).
  ACTOR_TOLBI_WOULD_BE_FIGHTER = 20,
  /// A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND), whom Mind Read
  /// finds thinking of his home.
  ACTOR_TOLBI_XIAN_VISITOR = 21,
  /// A woman with a sword (SPRITE_PERSON_WITH_SWORD), beside a second whose
  /// row has no name.
  ACTOR_TOLBI_OVERSLEEPER = 22,
  /// A family at the festival: the father, a burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR), the mother, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS), and their small boy
  /// (SPRITE_BOY_WITH_FAIR_HAIR), who cries for sweets.
  ACTOR_TOLBI_FATHER = 24,
  ACTOR_TOLBI_MOTHER = 25,
  ACTOR_TOLBI_CRYING_BOY = 26,
  /// A strongman (SPRITE_STRONGMAN) who sleeps under the archway during the
  /// festival; on finals day he sells seats.
  ACTOR_TOLBI_SCALPER = 27,
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) at the gate of Tolbi
  /// Spring, who lets the party go first.
  ACTOR_TOLBI_SPRING_VISITOR = 28,
  /// Babi's soldiers (SPRITE_SOLDIER), searching the town for him and
  /// showing his picture (Tolbi_MeetSearchingSoldiers).
  ACTOR_TOLBI_SOLDIER_A = 29,
  /// The second of the two soldiers.
  ACTOR_TOLBI_SOLDIER_B = 30,
  /// Ember, a Mars Djinni, until it joins the party (Tolbi_JoinEmber).
  ACTOR_TOLBI_EMBER = 31,
  /// The "Missing Person" poster, placed with Babi's sprite (SPRITE_BABI).
  ACTOR_TOLBI_MISSING_PERSON_POSTER = 32,
};

/// The cell behaviors Tolbi's coord and examine triggers answer besides the
/// doors, whose own cells carry the numbers of their exits (Tolbi_EnterDoor,
/// Tolbi_RequestTouchedExit).
enum TolbiCellBehavior {
  /// Where Babi's soldiers stop the party during the festival.
  CELL_BEHAVIOR_TOLBI_SOLDIERS = 20,
  /// The first of the two signs beside the gate to Tolbi Spring.
  CELL_BEHAVIOR_TOLBI_SPRING_SIGN = 30,
  CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN = 31,
  CELL_BEHAVIOR_TOLBI_BARREL = 100,
  CELL_BEHAVIOR_TOLBI_JAR = 101,
  CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST = 102,
  /// The gate to Tolbi Spring, open once the visitor at the gate has stepped
  /// aside.
  CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING = 239
};

/// The running children keep these flags in Actor.timer, their scene
/// scratch halfword. Bits: 1 talking | 0 chases the younger sister.
enum {
  /// Set on the older brother, who runs after ACTOR_TOLBI_YOUNGER_SISTER.
  KID_CHASES_YOUNGER = 1,
  KID_TALKING = 2, ///< set while the party talks to this child
};

#endif
