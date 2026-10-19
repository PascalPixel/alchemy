#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

#include "types.h"
#include "static_assert.h"
#include "edition.h"

/// A window as the render functions take it: its fields sit where a
/// UiWindow keeps its width, height and place.
struct RenderInput {
  u8 unk00[8]; ///< not named by ⚓️ code; a UiWindow's output list here
  u16 width;   ///< tiles
  u16 height;  ///< tiles
  u16 x;       ///< tiles
  u16 y;       ///< tiles
};

STATIC_ASSERT(sizeof(struct RenderInput) == 0x10);
STATIC_ASSERT(offsetof(struct RenderInput, width) == 8);
STATIC_ASSERT(offsetof(struct RenderInput, x) == 12);

void UiWindow_CopyTilemapRegion(const struct RenderInput *window,
                                const void *source);
void UiWindow_SetTileAttributeRect(const struct RenderInput *window, s32 x,
                                   s32 y, s32 width, s32 height, u32 field);
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *window, s32 x, s32 y,
                                    s32 resource);

/// A sprite's OAM attribute 2 (tile index, priority and palette) and the
/// halfword after it, as one word or as fields.
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

/// A render output: a sprite in a window's output list (a character, an icon
/// or a portrait), with its VRAM cache slot and OAM attributes.
struct RenderOutput {
  struct RenderOutput *next; ///< the next output in its list
  /// 0 for a glyph; otherwise it owns VRAM cache slot index, freed with it,
  /// and 2 (a portrait) also its palette slot.
  s8 kind;
  s8 active;    ///< its sprite motion; 0 while free
  u16 x;        ///< screen pixels
  u16 y;        ///< screen pixels
  u16 unk0a;    ///< not named by ⚓️ code
  u16 animStep; ///< step of its motion's animation
  s8 index;     ///< VRAM cache slot, or a glyph's tile
  u8 sentinel;  ///< depth slot it is drawn at
  u8 unk10[4];  ///< with oam, the sprite entry, read through casts
  struct RenderOam oam;
};

/// A list of render outputs: its first output and the link to append after.
struct RenderOutputList {
  struct RenderOutput *head;
  struct RenderOutput **tailLink;
};

STATIC_ASSERT(sizeof(struct RenderOutputList) == 8);
STATIC_ASSERT(offsetof(struct RenderOutputList, tailLink) == 4);

struct RenderOutput *RenderOutput_Create(s32 no, s32 flags,
                                         struct RenderInput *input, s32 offsetX,
                                         s32 offsetY);

struct RenderOutput *RenderOutput_CreateFar(s32 resource, s32 flags,
                                            struct RenderInput *window, s32 x,
                                            s32 y);

STATIC_ASSERT(offsetof(struct RenderOutput, animStep) == 0x0c);

STATIC_ASSERT(offsetof(struct RenderOutput, sentinel) == 0x0f);
STATIC_ASSERT(offsetof(struct RenderOutput, oam) == 0x14);
STATIC_ASSERT(sizeof(struct RenderOutput) == 0x1c);

#endif
