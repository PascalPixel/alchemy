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

/// Actors, numbered in the order of the placement table from 8.
enum TolbiActor {
  TOLBI_SPROUT = 8,
  TOLBI_PUDDLE = 9,
  /// A man who, during the festival, says he is 65 and that Babi was already
  /// old when he was born; later he talks of the Lucky Guess. Copies the
  /// leader's sprite priority every frame.
  TOLBI_PLAYER_COPY = 14,
  TOLBI_OLDER_BROTHER = 16,
  TOLBI_YOUNGER_SIBLING = 17,
  TOLBI_SNACK_VENDOR = 19,
  /// A family at the festival: the father, the mother and their child, who
  /// cries for sweets. The father: "the wife doesn't go in for that sort of
  /// thing"; the child: "Mom, you said you were all out of money".
  TOLBI_FATHER = 24,
  TOLBI_MOTHER = 25,
  TOLBI_CRYING_CHILD = 26,
  /// Sleeps under the archway during the festival; on finals day he sells
  /// seats. "Today is a big day for us scalpers"
  TOLBI_SCALPER = 27,
  TOLBI_GAMBLER = 28,
  TOLBI_SOLDIER_A = 29,
  TOLBI_SOLDIER_B = 30,
  TOLBI_EMBER = 31,
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
/// scratch halfword. Bits: 1 talking | 0 chases the younger sibling.
enum {
  KID_CHASES_YOUNGER = 1, ///< set on the older brother, who runs after actor 17
  KID_TALKING = 2,        ///< set while the party talks to this child
};

#endif
