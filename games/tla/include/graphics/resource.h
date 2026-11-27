#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

/* Resources and the sprites and VRAM cache entries made from them. */

#include "types.h"
#include "edition.h"
#include "field/sprite.h"
#include "system.h"

/* A battle sprite id's low twelve bits, the sprite. The bits above hold the
   enemy sprite slot its pictures are loaded into (btEnemySprLoadIntoSlots). */
#define SPRITE_ID_MASK	   0xfff
/* What a two-part enemy's second part adds to its first (the Serpent, the
   Kraken, the Fusion Dragon, Poseidon and the Doom Dragon): the next sprite,
   its pictures two slots on, past the two slots a large enemy's first part
   takes. */
#define SPRITE_SECOND_PART SPRITE_PICTURE_TAG(2, 1)

/* The orbs of RESOURCE_COLOURED_ORBS, one 256-colour tile each: sixteen in five
   colours, large and small, three colours twice. */
#define ORB_PICTURES 16

struct RESOURCE_OBJECT;
struct SPR;

/* Creates a sprite for animation `resource`, taking one of the 64 free sprites
   and a VRAM cache entry for its tiles. */
struct RESOURCE_OBJECT *fdSprCreateStub(Sint32 resource);
/* Releases a sprite and, unless it keeps it, its VRAM cache entry. */
void fdSprDestroyStub(SPR *sprite);

#endif
