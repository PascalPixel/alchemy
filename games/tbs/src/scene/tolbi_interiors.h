#ifndef TOLBI_INTERIORS_H
#define TOLBI_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "common/colosso_story_flags.h"

enum TolbiInteriorsFlag {
  FLAG_TOLBI_INTERIORS_SAILORS_SEEN = 0x300, // the sailors' argument played
  FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN = 0x8bc, // the woman hid the Lucky Medal
  FLAG_TOLBI_INTERIORS_MEDAL_TAKEN = 0x8bd, // the party took it from the barrel
  FLAG_TOLBI_INTERIORS_MEDAL_MISSED = 0x8be, // she has greeted the party since
  // Set at the Tolbi docks when the ticket money is returned ("I returned the
  // cost of your ticket to you").
  FLAG_TOLBI_DOCKS_TICKETS_REFUNDED = 0x8ab,
};

// Actors, numbered in the order of the placement table from 8.
enum TolbiInteriorsActor {
  TOLBI_INTERIORS_MEDAL_WOMAN = 25,
  TOLBI_INTERIORS_ANGRY_SAILOR = 31,
  TOLBI_INTERIORS_SAILOR_B = 32,
  TOLBI_INTERIORS_SAILOR_C = 33,
  TOLBI_INTERIORS_WOULD_BE_WARRIOR = 35,
};

// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing) ((u16)(((facing) + 0x2000) & ~0x3fff))

// The byte of a sprite's attributes that holds its priority, as the code
// stores it.
struct TolbiInteriorsSpriteAttr {
  u8 unknown[9];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
};

#endif
