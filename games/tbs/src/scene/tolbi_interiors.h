#ifndef TOLBI_INTERIORS_H
#define TOLBI_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "common/colosso_story_flags.h"
#include "flags.h"

/// Actors, numbered in the order of the placement table from 8.
enum TolbiInteriorsActor {
  TOLBI_INTERIORS_HOST = 9, ///< offers a stay. "Do you want to stay with us?"
  TOLBI_INTERIORS_WEAPON_SHOPKEEPER = 19,
  TOLBI_INTERIORS_ARMOR_SHOPKEEPER = 20,
  TOLBI_INTERIORS_ITEM_SHOPKEEPER = 21,
  TOLBI_INTERIORS_MEDAL_WOMAN = 25,
  TOLBI_INTERIORS_CHEF = 26,
  TOLBI_INTERIORS_WAITER = 27,
  TOLBI_INTERIORS_PRIEST = 30,
  TOLBI_INTERIORS_ANGRY_SAILOR = 31,
  TOLBI_INTERIORS_SAILOR_B = 32,
  TOLBI_INTERIORS_SAILOR_C = 33,
  TOLBI_INTERIORS_WOULD_BE_WARRIOR = 35,
};

/// Map objects the party can search.
enum TolbiInteriorsMapObject {
  TOLBI_INTERIORS_INN_SIGN = 40, ///< the inn's sign. "Inn Full / No Vacancies"
  TOLBI_INTERIORS_VIAL_BARREL = 100,
  TOLBI_INTERIORS_MEDAL_BARREL = 101,
  TOLBI_INTERIORS_HIDDEN_MEDAL_BARREL = 102, ///< where the woman hid her find
  TOLBI_INTERIORS_PIZZA_OVEN = 200,
  TOLBI_INTERIORS_PASTA_OVEN = 201,
  TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN = 202,
};

/// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing) ((u16)(((facing) + 0x2000) & ~0x3fff))

#endif
