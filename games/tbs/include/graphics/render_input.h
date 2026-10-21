#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

/// @file
/// The sprite renderer's inputs and outputs: render inputs, OAM attributes and
/// render output lists.

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

/// A 16-byte record of a rectangle. In TBS it only reserves stack space in
/// tbs-ja's glyph renderer; no TBS code reads its fields.
struct RenderInput {
  u8 unk00[8]; ///< not named by TBS code
  u16 width;   ///< not named by TBS code
  u16 height;  ///< not named by TBS code
  u16 x;       ///< not named by TBS code
  u16 y;       ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct RenderInput) == 0x10);
STATIC_ASSERT(offsetof(struct RenderInput, width) == 8);
STATIC_ASSERT(offsetof(struct RenderInput, x) == 12);

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

/// Attributes zero through two copied to OAM, plus the halfword after them.
struct RenderOam {
  u16 y : 8;          ///< screen pixels
  u16 affineMode : 2; ///< 0 plain, 1 affine, 3 affine at double size
  u16 objectMode : 2; ///< an enum ObjMode
  u16 mosaic : 1;
  u16 palette256 : 1; ///< 256 colours instead of a 16-colour palette
  u16 shape : 2;      ///< 0 square, 1 wide, 2 tall
  u16 x : 9;          ///< screen pixels
  /// The affine matrix; for a plain sprite, 8 mirrors it left to right and 16
  /// top to bottom.
  u16 matrix : 5;
  u16 size : 2;   ///< with shape, the sprite's size
  u16 index : 10; ///< first sprite tile in VRAM
  /// 0 to 3: drawn over every background of the same or a larger priority
  /// number.
  u16 priority : 2;
  u16 palette : 4; ///< 16-colour palette
  u16 high;        ///< not named by TBS code
};

/// A render output: a sprite in a window's output list (a character, an icon
/// or a portrait), with its VRAM cache slot and OAM attributes.
struct RenderOutput {
  struct RenderOutput *next; ///< the next output in its list
  /// 0 for a glyph; otherwise it owns VRAM cache slot index, freed with it,
  /// and 2 (a portrait) also its palette slot.
  s8 kind;
  s8 active;    ///< its UI_SPRITE_ motion; 0 while free
  u16 x;        ///< screen pixels
  u16 y;        ///< screen pixels
  u16 unk0a;    ///< not named by TBS code
  u16 animStep; ///< step of its motion's animation
  s8 index;     ///< VRAM cache slot, or a glyph's tile after glyphTiles
  u8 sentinel;  ///< depth slot it is drawn at (254 for glyphs)
  /// The depth queue's link: with oam it is the sprite entry queued each
  /// frame, read as struct CursorSprite.
  u8 unk10[4];
  struct RenderOam oam;
};

/// A list of render outputs: its first output and the link to append after.
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
