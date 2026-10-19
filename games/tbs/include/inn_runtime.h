#ifndef ALCHEMY_INN_RUNTIME_H
#define ALCHEMY_INN_RUNTIME_H

/// @file
/// InnRuntimeState, an inn's work.

#include "global_cells.h"
#include "static_assert.h"
#include "types.h"

/// An inn's work, in the menu work: the coins window and the innkeeper's
/// portrait.
struct InnRuntimeState {
  u8 padding000[0x0c];
  struct UiWindow *window; ///< the coins window: ShopRuntime's moneyWindow
  u8 padding010[0x380];
  /// ShopRuntime's six cursor icon slots; not named by TBS code.
  u16 resourceEntries[6];
  u8 padding39c[8];
  /// The innkeeper's animation, which picks the portrait: ShopRuntime's
  /// keeperResource.
  u16 resourceId;
  u8 padding3a6[3];
  u8 active; ///< ShopRuntime's shopType; the inn sets 1 when checking in
  u8 padding3aa[2];
  /// ShopRuntime's warriorShop; the inn sets 1 for inn 5.
  u8 specialActive;
};

STATIC_ASSERT(offsetof(struct InnRuntimeState, window) == 0x0c);
STATIC_ASSERT(offsetof(struct InnRuntimeState, resourceEntries) == 0x390);
STATIC_ASSERT(offsetof(struct InnRuntimeState, resourceId) == 0x3a4);
STATIC_ASSERT(offsetof(struct InnRuntimeState, active) == 0x3a9);
STATIC_ASSERT(offsetof(struct InnRuntimeState, specialActive) == 0x3ac);

#endif
