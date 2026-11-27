#ifndef ALCHEMY_HANGING_VINE_H
#define ALCHEMY_HANGING_VINE_H

// The hanging vines the leader swings from: their links and their swing.

#include "fixed_math.h"
#include "types.h"
#include "system.h"
#include "field/sprite.h"

// One link of a hanging vine: a point on the vine and its sprite.
typedef struct HANGING_VINE_LINK
{
	struct HANGING_VINE_LINK *next;
	FIXED                     x; // 16.16 fixed-point pixels
	FIXED                     y;
	FIXED                     z;
	Uint16                    angle;
	Sint8                     kind;            // 0 or 1: not drawn; else its picture, from 2
	Uint8                     unusedAfterKind; // no code names it
	ANIMATION_SPR_PART        sprite;
} HANGING_VINE_LINK;

// One of the eight hanging vines: the actor it hangs from and its swing.
typedef struct HANGING_VINE_SLOT
{
	Sint16 id;                    // the actor the chain hangs from
	Uint8  unusedAfterId[2];      // no code names it
	Sint32 swing;                 // the arc's width
	FIXED  spacing;               // between links, in 16.16
	Uint8  unusedAfterSpacing[4]; // no code names it
	Sint32 phase;                 // where the swing is in its arc, an angle
	Sint32 active;
	void  *root;                  // the first link fdHangingVineHang hangs; NULL if free
} HANGING_VINE_SLOT;

// The hanging vine's state (RamHeapSlots->hangingVine): the links' picture, eight vine slots and up
// to 128 links.
typedef struct HANGING_VINE_WORK
{
	Sint32            vramEntry; // the links' picture
	HANGING_VINE_SLOT slots[ROPE_SLOT_COUNT];
	HANGING_VINE_LINK links[ROPE_NODE_COUNT];
} HANGING_VINE_WORK;

#define HANGING_VINE_STATE ((HANGING_VINE_WORK *)RamHeapSlots->hangingVine)

#endif
