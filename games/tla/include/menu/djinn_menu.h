#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/// @file
/// The Djinn menu's work, held in the menu runtime heap slot
/// (Ram_HeapSlots->menuRuntime) while the menu is open.

#include "static_assert.h"
#include "types.h"

/// The Djinn list's entries and the backdrop's tiles, allocated when the menu
/// opens and released when it closes.
struct DjinnMenuBackdrop {
  u16 ids[8][10];
  s8 counts[8];
  u8 tiles[0x2000];
  u8 palette[0x80];
  s32 tutorialTimer;
  s32 tutorialStep;
};

struct DjinnMenuIcon {
  u8 unk00[5];
  u8 active;
};

struct DjinnPreviewWindow {
  u8 unk00[8];
  u16 width;
  u8 unk0a[4];
  s16 y;
};

struct DjinnMenuWork {
  u8 unk000[3];
  u8 dirty; ///< set to redraw the menu
  u8 unk004[2];
  u8 holdTilemapUpload;
  u8 unk007;
  s32 owner;
  u8 unk00c[4];
  s32 statusWindow;
  struct DjinnMenuIcon *cursor;
  u8 unk018[4];
  s8 selectedColumn[2];
  u8 unk01e[0xa];
  s32 window;
  s32 unopenedWindow;
  u8 unk030[4];
  s32 listWindow;
  struct DjinnPreviewWindow *secondWindow;
  u8 unk03c[0xb4];
  s32 helpWindow;
  u8 unk0f4[4];
  s32 headerWindows[4];
  u8 unk108[0x10];
  s16 memberX[8];
  s16 memberY[8];
  u8 unk138[0x30];
  u16 selection[2];
  u16 pairEntries[2];
  struct MenuIcon *selector;
  u8 unk174[4];
  struct MenuIcon *arrows[2];
  struct DjinnMenuBackdrop *backdrop;
  u8 unk184[0x80];
  u16 owners[8];
  u8 unk214;
  u8 memberCount;
  u8 shownMember[2];
  u8 unk218[4];
  u16 pending;
  u8 unk21e[0x12];
  u16 rowX[4];
  u16 rowY[4];
  u8 unk240[0x18];
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairMember[2];
  u8 unk25e[2];
};

STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tiles) == 0xa8);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialTimer) == 0x2128);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, window) == 0x28);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, helpWindow) == 0xf0);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, memberY) == 0x128);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, backdrop) == 0x180);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, owners) == 0x204);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, shownMember) == 0x216);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, pairMember) == 0x25c);

#endif
