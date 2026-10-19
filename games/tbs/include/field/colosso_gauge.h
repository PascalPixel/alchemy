#ifndef ALCHEMY_COLOSSO_GAUGE_H
#define ALCHEMY_COLOSSO_GAUGE_H

/// @file
/// The Colosso race gauge's sprites and work.

#include "types.h"
#include "static_assert.h"

/// One sprite of the race gauge's list: a link word, its position and shape
/// attributes, and its tile.
struct ColossoGaugeSprite {
  s32 link;
  s32 attr;
  s32 tile;
};

/// The Colosso race gauge's work at the start of work slot 59: its sprites,
/// how far it has slid down, the racers and the finish.
struct ColossoGaugeWork {
  struct ColossoGaugeSprite sprites[18];
  s16 vramBlock;
  s16 level;
  s16 hold;
  s16 playerActor;
  s16 rivalActor;
  u16 finishItemActorA;
  u16 finishItemActorB;
  s16 segments;
  s32 finishX; ///< 16.16 fixed-point pixels
  s32 finishZ; ///< 16.16 fixed-point pixels
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
