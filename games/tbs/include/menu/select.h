#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

#include "types.h"
#include "static_assert.h"
#include "window.h"
#include "affine.h"

struct SelectionSprite {
  struct SelectionSprite *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 param : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

struct SelectionNode {
  struct SelectionNode *prev;
  struct SelectionNode *next;
  u16 base;
  u16 kind;
  u16 slot;
  u16 tile;
  s16 x;
  s16 y;
  s16 dx;
  s16 dy;
  s16 targetX;
  s16 targetY;
  s16 homeX;
  s16 homeY;
  u16 message;
  s16 scale;
  s16 scaleStep;
  s16 scaleEnd;
  struct SelectionSprite sprite;
};

struct SelectionScreen {
  struct SelectionNode records[16];
  struct AffineTransform transform;
  struct SelectionNode *head;
  struct SelectionNode *path;
  struct UiWindow *window;
  u16 kinds[16];
  u16 bases[16];
  u16 count;
  u16 baseX;
  u16 baseY;
  u16 unk39a;
  u16 top;
  u16 cursorIndex;
  u16 verticalScroll;
  u16 status;
  u16 pathTop[5];
  u16 pathCursor[5];
  u16 locked;
  u8 unk3ba[42];
};

STATIC_ASSERT(sizeof(struct SelectionSprite) == 0x0c);
STATIC_ASSERT(sizeof(struct SelectionNode) == 0x34);
STATIC_ASSERT(offsetof(struct SelectionNode, message) == 0x20);
STATIC_ASSERT(offsetof(struct SelectionNode, sprite) == 0x28);
STATIC_ASSERT(offsetof(struct SelectionScreen, head) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionScreen, kinds) == 0x354);
STATIC_ASSERT(offsetof(struct SelectionScreen, cursorIndex) == 0x39e);
STATIC_ASSERT(offsetof(struct SelectionScreen, locked) == 0x3b8);
STATIC_ASSERT(sizeof(struct SelectionScreen) == 0x3e4);

#endif
