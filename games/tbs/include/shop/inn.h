#ifndef ALCHEMY_INN_H
#define ALCHEMY_INN_H

/// @file
/// InnWork, an inn's work.

#include "heap_slots.h"
#include "static_assert.h"
#include "types.h"

/// An inn's work, in the menu work: the coins window and the innkeeper's
/// portrait.
struct InnWork {
  u8 padding000[12];
  struct Window *window; ///< the coins window: ShopWork's moneyWindow
  u8 padding010[896];
  /// ShopWork's six cursor icon slots; not named by TBS code.
  u16 resourceEntries[6];
  u8 padding39c[8];
  /// The innkeeper's animation, which picks the portrait: ShopWork's
  /// keeperGraphics.
  u16 resourceId;
  u8 padding3a6[3];
  u8 active; ///< ShopWork's shopType; the inn sets 1 when checking in
  u8 padding3aa[2];
  /// ShopWork's warriorShop; the inn sets 1 for inn 5.
  u8 specialActive;
};

STATIC_ASSERT(offsetof(struct InnWork, window) == 0x0c);
STATIC_ASSERT(offsetof(struct InnWork, resourceEntries) == 0x390);
STATIC_ASSERT(offsetof(struct InnWork, resourceId) == 0x3a4);
STATIC_ASSERT(offsetof(struct InnWork, active) == 0x3a9);
STATIC_ASSERT(offsetof(struct InnWork, specialActive) == 0x3ac);

#endif
