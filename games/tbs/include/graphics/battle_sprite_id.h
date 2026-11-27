#ifndef ALCHEMY_BATTLE_SPRITE_ID_H
#define ALCHEMY_BATTLE_SPRITE_ID_H

/* @file */
/* The parts of a battle sprite id. */

#include "edition.h"
#include "types.h"
#include "field/sprite.h"
#include "stubs.h"

/* A battle sprite id's low twelve bits, the sprite. The bits above hold the */
/* enemy sprite slot its pictures are loaded into (btEnemySprLoadIntoSlots). */
#define SPRITE_ID_MASK 0xfff
/* What the Kraken's and the Fusion Dragon's second part adds to their first: */
/* the next sprite, its pictures two slots on, past the two slots a large */
/* enemy's first part takes. */
#define SPRITE_SECOND_PART SPRITE_PICTURE_TAG(2, 1)

#endif
