#ifndef ALCHEMY_COLOSSO_GAUGE_H
#define ALCHEMY_COLOSSO_GAUGE_H

/// @file
/// The Colosso race gauge's sprites and work.

#include "types.h"
#include "static_assert.h"

/// One sprite of the race gauge's list: a link word, its position and shape
/// attributes, and its tile.
struct ColossoGaugeSprite {
  s32 link; ///< written 0; Runtime_PushSlotEntry links the queue through it
  s32 attr; ///< OAM attributes 0 and 1: y | x << 16 | shape and size bits
  s32 tile; ///< OAM attribute 2: the tile, with palette 14 and priority 1
};

/// The Colosso race gauge's work at the start of work slot 59: its sprites,
/// how far it has slid down, the racers and the finish.
struct ColossoGaugeWork {
  struct ColossoGaugeSprite sprites[18]; ///< the bar, its ends and the racers
  s16 vramBlock;   ///< the VRAM cache entry holding the gauge's tiles
  s16 level;       ///< 0 hidden to 2 fully down; the bar's y is level * 6 - 8
  s16 hold;        ///< nonzero keeps the gauge down (level 2)
  s16 playerActor; ///< the actor id drawn as the player's marker
  s16 rivalActor;  ///< the actor id drawn as the rival's marker
  u16 finishItemActorA; ///< the first actor standing at the finish
  u16 finishItemActorB; ///< the second actor standing at the finish
  s16 segments;         ///< 16-pixel bar pieces either side of the centre
  s32 finishX;          ///< 16.16 fixed-point pixels
  s32 finishZ;          ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct ColossoGaugeSprite) == 12);
STATIC_ASSERT(sizeof(struct ColossoGaugeWork) == 240);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, vramBlock) == 216);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, level) == 218);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, hold) == 220);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, playerActor) == 222);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, rivalActor) == 224);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, finishItemActorA) == 226);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, finishItemActorB) == 228);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, segments) == 230);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, finishX) == 232);
STATIC_ASSERT(offsetof(struct ColossoGaugeWork, finishZ) == 236);

#endif
