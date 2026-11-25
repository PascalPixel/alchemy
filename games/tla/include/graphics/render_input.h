#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

/// @file
/// Sprites as OAM takes them, and the window sprites: the characters, icons and
/// portraits a window carries.

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "oam.h"

struct Window;

s32 WindowSprite_CreateAbilityIcon(struct Window *window, s32 x, s32 y,
                                   s32 resource);

/// Attributes zero through two as the hardware lays them out in OAM, plus the
/// halfword after them.
struct OamData {
  u16 y : 8;          ///< screen pixels
  u16 affineMode : 2; ///< 0 plain, 1 affine, 3 affine at double size
  u16 objectMode : 2; ///< an enum ObjMode
  u16 mosaic : 1;
  u16 fullColor : 1; ///< 256 colours instead of a 16-colour palette
  u16 shape : 2;     ///< 0 square, 1 wide, 2 tall
  u16 x : 9;         ///< screen pixels
  /// The affine matrix; for a plain sprite, 8 mirrors it left to right and 16
  /// top to bottom.
  u16 matrix : 5;
  u16 size : 2;   ///< with shape, the sprite's size
  u16 index : 10; ///< first sprite tile in VRAM
  /// 0 to 3: drawn over every background of the same or a larger priority
  /// number.
  u16 priority : 2;
  u16 palette : 4; ///< 16-colour palette
  u16 high;
};

/// A sprite as the depth queue takes it: Oam_AddSpriteAtDepth writes the link
/// to the next sprite queued at the same depth, and the OAM attributes follow.
struct QueuedSprite {
  struct QueuedSprite *next; ///< the next sprite queued at its depth
  struct OamData oam;
};

/// A window sprite: a sprite in a window's sprite list (a character, an icon
/// or a portrait), with its VRAM cache slot and OAM attributes.
struct WindowSprite {
  struct WindowSprite *next; ///< the next sprite in its list
  s8 kind;                   ///< an enum WindowSpriteKind
  s8 motion;                 ///< its WINDOW_SPRITE_MOTION_ motion; 0 while free
  u16 x;                     ///< screen pixels
  u16 y;                     ///< screen pixels
  u16 unused;
  u16 animationStep; ///< step of its motion's animation
  s8 index;          ///< VRAM cache slot, or a glyph's tile after glyphTiles
  /// The depth slot it is drawn at (Oam_AddSpriteAtDepth): a higher slot is
  /// drawn in front.
  u8 depth;
  struct QueuedSprite sprite; ///< queued at its depth each frame
};

/// The queued sprite of window sprite `sprite` (WindowSprite.sprite).
static inline struct QueuedSprite *
WindowSprite_GetSprite(struct WindowSprite *sprite) {
  return &sprite->sprite;
}

/// A list of window sprites: its first sprite and the link to append after.
struct WindowSpriteList {
  struct WindowSprite *head;
  struct WindowSprite **tailLink;
};

STATIC_ASSERT(sizeof(struct WindowSpriteList) == 8);
STATIC_ASSERT(offsetof(struct WindowSpriteList, tailLink) == 4);

struct WindowSprite *WindowSprite_Create(s32 slot, s32 oamFlags,
                                         struct Window *window, s32 offsetX,
                                         s32 offsetY);

/// Takes a free window sprite showing VRAM cache slot `slot`, places it
/// (offsetX, offsetY) pixels from the window's inner corner and appends it to
/// the window's list.
struct WindowSprite *WindowSprite_CreateFar(s32 slot, s32 oamFlags,
                                            struct Window *window, s32 offsetX,
                                            s32 offsetY);

STATIC_ASSERT(offsetof(struct WindowSprite, animationStep) == 12);

STATIC_ASSERT(offsetof(struct WindowSprite, depth) == 15);
STATIC_ASSERT(offsetof(struct WindowSprite, sprite) == 16);
STATIC_ASSERT(offsetof(struct WindowSprite, sprite.oam) == 20);
STATIC_ASSERT(sizeof(struct WindowSprite) == 28);

#endif
