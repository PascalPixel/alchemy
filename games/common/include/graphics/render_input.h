#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

#include "types.h"
#include "layout_guard.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct RenderInput {
  u8 unk00[8];
  u16 width;
  u16 height;
  u16 x;
  u16 y;
};

LAYOUT_SIZE_GUARD(RenderInput_size, struct RenderInput, 0x10);
LAYOUT_OFFSET_GUARD(RenderInput_width_offset, struct RenderInput, width, 8);
LAYOUT_OFFSET_GUARD(RenderInput_x_offset, struct RenderInput, x, 12);

#if GAME_TLA
void UiWindow_CopyTilemapRegion(const struct RenderInput *window,
                                const void *source);
void UiWindow_SetTileAttributeRect(const struct RenderInput *window, s32 x,
                                   s32 y, s32 width, s32 height, u32 field);
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *window, s32 x, s32 y,
                                    s32 resource);
#endif

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

// OAM に写す属性 0-2 と未使用の 1 ワード
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

LAYOUT_SIZE_GUARD(RenderOutputList_Size, struct RenderOutputList, 8);
LAYOUT_OFFSET_GUARD(RenderOutputList_Tail, struct RenderOutputList, tailLink,
                    4);

#if GAME_TLA
struct RenderOutput *RenderOutput_Create(s32 no, s32 flags,
                                         struct RenderInput *input, s32 offsetX,
                                         s32 offsetY);
#endif

#if GAME_TLA
struct RenderOutput *RenderOutput_CreateFar(s32 resource, s32 flags,
                                            struct RenderInput *window, s32 x,
                                            s32 y);
#endif

LAYOUT_OFFSET_GUARD(RenderOutput_AnimStep, struct RenderOutput, animStep, 0x0c);

LAYOUT_OFFSET_GUARD(RenderOutput_sentinel_offset, struct RenderOutput, sentinel,
                    0x0f);
LAYOUT_OFFSET_GUARD(RenderOutput_oam_offset, struct RenderOutput, oam, 0x14);
LAYOUT_SIZE_GUARD(RenderOutput_size, struct RenderOutput, 0x1c);

#endif
