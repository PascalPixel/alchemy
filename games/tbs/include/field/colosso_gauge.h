#ifndef ALCHEMY_COLOSSO_GAUGE_H
#define ALCHEMY_COLOSSO_GAUGE_H

/// @file
/// The Colosso race's heap slot, and the race gauge's sprites and work.

#include "types.h"
#include "static_assert.h"

/// The heap slot of the Colosso finals' race work (struct ColossoRaceWork),
/// which gColossoWork reads by its own name.
#define HEAP_SLOT_COLOSSO_RACE 59

/// One sprite of the race gauge's list: a link word, its position and shape
/// attributes, and its tile.
struct ColossoGaugeSprite {
  s32 link; ///< written 0; Runtime_PushSlotEntry links the queue through it
  s32 attr; ///< OAM attributes 0 and 1: y | x << 16 | shape and size bits
  s32 tile; ///< OAM attribute 2: the tile, with palette 14 and priority 1
};

/// The Colosso race gauge's work at the start of the race's heap slot
/// (HEAP_SLOT_COLOSSO_RACE): its sprites, how far it has slid down, the racers
/// and the finish.
struct ColossoGaugeWork {
  /// The bar, its ends and the racers' marks: 2 * segments + 6 sprites, all
  /// 18 for Colosseum Final 3's six segments.
  struct ColossoGaugeSprite sprites[18];
  /// The VRAM cache entry holding the gauge's tiles: released while the gauge
  /// is hidden, and loaded again when it slides back down.
  s16 vramBlock;
  /// 0 hidden to 2 fully down, stepping up while FLAG_MENU_OPEN is clear and
  /// down while it is set; the bar's y is level * 6 - 8.
  s16 level;
  /// Nonzero keeps the gauge down (level 2); only the unused
  /// Colosso_SetGaugeHold sets it.
  s16 hold;
  s16 playerActor; ///< the actor id drawn as the player's marker
  /// The actor id drawn as the rival's marker, and the actor that runs the
  /// recorded path.
  s16 rivalActor;
  /// The first finisher's prize: the actor raised over that finisher, who
  /// stops at finishZ minus 16 pixels.
  u16 finishItemActorA;
  /// The second finisher's prize; that finisher stops at finishZ plus 16
  /// pixels.
  u16 finishItemActorB;
  /// 16-pixel bar pieces either side of the centre: 6 in Colosseum Final 3,
  /// 4 in Final 1 and 5 in Final 2.
  s16 segments;
  /// 16.16 fixed-point pixels: the rival starts opposite Isaac across it and
  /// runs its path mirrored across it.
  s32 finishX;
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
