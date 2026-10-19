#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

struct RenderInput {
  u8 unk00[8];
  u16 width;
  u16 height;
  u16 x;
  u16 y;
};

STATIC_ASSERT(sizeof(struct RenderInput) == 0x10);
STATIC_ASSERT(offsetof(struct RenderInput, width) == 8);
STATIC_ASSERT(offsetof(struct RenderInput, x) == 12);

union RenderTableValue {
  u32 value;
  struct {
    unsigned short index : 10;
    unsigned short priority : 2;
    unsigned short palette : 4;
    u16 high;
  } bits;
  struct {
    u16 low;
    u16 high;
  } half;
};

/// Attributes zero through two copied to OAM, plus one unused word.
struct RenderOam {
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 palette256 : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 index : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 high;
};

struct RenderOutput {
  struct RenderOutput *next;
  s8 kind;
  s8 active;
  u16 x;
  u16 y;
  u16 unk0a;
  u16 animStep;
  s8 index;
  u8 sentinel;
  u8 unk10[4];
  struct RenderOam oam;
};

struct RenderOutputList {
  struct RenderOutput *head;
  struct RenderOutput **tailLink;
};

STATIC_ASSERT(sizeof(struct RenderOutputList) == 8);
STATIC_ASSERT(offsetof(struct RenderOutputList, tailLink) == 4);

STATIC_ASSERT(offsetof(struct RenderOutput, animStep) == 0x0c);

STATIC_ASSERT(offsetof(struct RenderOutput, sentinel) == 0x0f);
STATIC_ASSERT(offsetof(struct RenderOutput, oam) == 0x14);
STATIC_ASSERT(sizeof(struct RenderOutput) == 0x1c);

#endif
