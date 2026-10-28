#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "glyph.h"

s32 Icon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(0x44, sizeof(GlyphTransfer));
  ItemIcon_Compose(arg0, arg1);
  VramCache_Load(arg2, 0x80, work->tiles);
  HeapSlot_Release(0x44);
  return 1;
}
