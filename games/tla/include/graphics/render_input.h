#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "oam.h"

struct Window;

s32 WindowSprite_CreateAbilityIcon(struct Window *window, s32 x, s32 y,
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
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 index : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 high;
};

/// A sprite as the depth queue takes it: Oam_AddSpriteAtDepth writes the link
/// to the next sprite queued at the same depth, and the OAM attributes follow.
struct QueuedSprite {
  struct QueuedSprite *next; ///< the next sprite queued at its depth
  struct RenderOam oam;
};

/// A window sprite: a sprite in a window's output list (a character, an icon
/// or a portrait), with its VRAM cache slot and OAM attributes.
struct WindowSprite {
  struct WindowSprite *next; ///< the next output in its list
  /// 0 for a glyph; otherwise it owns VRAM cache slot index, freed with it,
  /// and 2 (a portrait) also its palette slot.
  s8 kind;
  s8 motion; ///< its WINDOW_SPRITE_MOTION_; 0 while free

  u16 x;                      ///< screen pixels
  u16 y;                      ///< screen pixels
  u16 unused;                 ///< nothing reads it
  u16 animationStep;          ///< step of its motion's animation
  s8 index;                   ///< VRAM cache slot, or a glyph's tile
  u8 depth;                   ///< depth slot it is drawn at
  struct QueuedSprite sprite; ///< queued at its depth each frame
};

/// A list of window sprites: its first output and the link to append after.
struct WindowSpriteList {
  struct WindowSprite *head;
  struct WindowSprite **tailLink;
};

STATIC_ASSERT(sizeof(struct WindowSpriteList) == 8);
STATIC_ASSERT(offsetof(struct WindowSpriteList, tailLink) == 4);

struct WindowSprite *WindowSprite_Create(s32 no, s32 flags,
                                         struct Window *input, s32 offsetX,
                                         s32 offsetY);

/// Takes a free window output, writes its OAM attributes at the window's
/// position offset by (offsetX, offsetY) pixels and appends it to the window's
/// list.
struct WindowSprite *WindowSprite_CreateFar(s32 resource, s32 flags,
                                            struct Window *window, s32 x,
                                            s32 y);

STATIC_ASSERT(offsetof(struct WindowSprite, animationStep) == 12);

STATIC_ASSERT(offsetof(struct WindowSprite, depth) == 15);
STATIC_ASSERT(offsetof(struct WindowSprite, sprite) == 16);
STATIC_ASSERT(offsetof(struct WindowSprite, sprite.oam) == 20);
STATIC_ASSERT(sizeof(struct WindowSprite) == 28);

/// The sprite attributes, as the hardware lays them out in OAM.
struct OamData {
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 index : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 high;
};

#endif
