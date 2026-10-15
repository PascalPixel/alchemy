#include "edition.h"
#ifndef TOLBI_H
#define TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "tbs_edition.h"
extern u8 gWindowWork[];

#define NULL ((void *)0)

#include "common/colosso_story_flags.h"

// Tolbi's own flags.
enum TolbiFlag {
  FLAG_TOLBI_SPROUT_GROWN = 0x200,  // the sprout by the spring has grown
  FLAG_TOLBI_PUDDLE_FROZEN = 0x201, // the puddle by the spring is frozen
  FLAG_TOLBI_CORN_GIVEN = 0x8bf,    // the snack vendor gave away her corn
  FLAG_TOLBI_SEARCHERS_MET = 0x8c0, // the soldiers asked about Babi
  FLAG_TOLBI_SPRING_OPENED = 0x8c1, // the gambler let the party go first
};

// Actors, numbered in the order of the placement table from 8.
enum TolbiActor {
  TOLBI_SPROUT = 8,
  TOLBI_PUDDLE = 9,
  TOLBI_PLAYER_COPY = 14, // copies the leader's sprite priority every frame
  TOLBI_OLDER_BROTHER = 16,
  TOLBI_YOUNGER_SIBLING = 17,
  TOLBI_SNACK_VENDOR = 19,
  TOLBI_GAMBLER = 28,
  TOLBI_SOLDIER_A = 29,
  TOLBI_SOLDIER_B = 30,
  TOLBI_EMBER = 31,
  TOLBI_MISSING_PERSON_POSTER = 32,
};

// Map objects the party can search.
enum TolbiMapObject {
  TOLBI_BARREL = 100,
  TOLBI_JAR = 101,
  TOLBI_HIDDEN_CHEST = 102,
};

// A word of the event work, at offset 0x1f4, that is set to the poster
// before the yes/no choice under the "Missing Person" window. What reads it
// is not yet traced.
#define EVENT_WORK_1F4 (*(s32 *)((u8 *)gEventWork + 0x1f4))

// The window state the running children consult before they widen their
// stopping range. RENDER_MODE_OFFSET is the edition's offset of this byte in
// the UI render work.
struct TolbiUiWork {
  u8 unknown[RENDER_MODE_OFFSET];
  u8 renderMode;
};

// The byte holding the second sprite part's priority bits, as the code
// stores it.
struct TolbiSpritePriorityByte {
  u8 unknown[0x15];
  u8 : 2;
  u8 secondPriority : 2;
  u8 : 4;
};

// A running child, as its update reads it: the field actor's facing,
// position, paused byte and the word the scene keeps the child's flags in.
struct TolbiKid {
  u8 unknown00[6];
  u16 facing;
  s32 x, y, z;
  u8 unknown14[71];
  u8 paused;
  u8 unknown5c[8];
  u16 kidFlags;
};

// TolbiKid.kidFlags
enum {
  KID_CHASES_YOUNGER = 1, // set on the older brother, who runs after actor 17
  KID_TALKING = 2,        // set while the party talks to this child
};

#endif
