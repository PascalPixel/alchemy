#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

/// @file
/// What games/tla/src/window/screens/ declares for the files that use it,
/// written once.

#include "types.h"

struct PsynergyMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
  u8 textPalette;
};

#endif
