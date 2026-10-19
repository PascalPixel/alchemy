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
  struct UiWindow *window;
  u8 padding010[0x380];
  u16 resourceEntries[6];
  u8 padding39c[8];
  u16 resourceId;
  u8 padding3a6[3];
  u8 active;
  u8 padding3aa[2];
  u8 specialActive;
};

STATIC_ASSERT(offsetof(struct InnRuntimeState, window) == 0x0c);
STATIC_ASSERT(offsetof(struct InnRuntimeState, resourceEntries) == 0x390);
STATIC_ASSERT(offsetof(struct InnRuntimeState, resourceId) == 0x3a4);
STATIC_ASSERT(offsetof(struct InnRuntimeState, active) == 0x3a9);
STATIC_ASSERT(offsetof(struct InnRuntimeState, specialActive) == 0x3ac);

#endif
