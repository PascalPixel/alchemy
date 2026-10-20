#ifndef ALCHEMY_PALBUF_H
#define ALCHEMY_PALBUF_H

/// @file
/// Palette fades, the battle effect palette buffers and the colour channel
/// curves.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A colour buffer for all 512 palette colours: the source palette, the shown
/// and target colour channels, their steps, and two pages of packed colours.
struct PaletteFade {
  /// The source palette, RGB555: 256 BG colours, then 256 OBJ colours.
  u16 palette[512];
  /// The shown colours, as B, G and R channels shifted up 10 bits, three a
  /// colour.
  u16 current[0x600];
  u16 target[0x600]; ///< the colours current moves toward, as current
  u16 delta[0x600];  ///< added to each current channel each frame
  /// Two pages of RGB555 colours: one is built while the other is queued.
  u16 packed[2][512];
  u8 page;        ///< the packed page queued this frame
  s8 duration;    ///< frames; 0 when the colours are not moving
  u8 step;        ///< frames
  u8 padding3003; ///< never named
};

STATIC_ASSERT(sizeof(struct PaletteFade) == 0x3004);
STATIC_ASSERT(offsetof(struct PaletteFade, current) == 0x400);
STATIC_ASSERT(offsetof(struct PaletteFade, target) == 0x1000);
STATIC_ASSERT(offsetof(struct PaletteFade, delta) == 0x1c00);
STATIC_ASSERT(offsetof(struct PaletteFade, packed) == 0x2800);
STATIC_ASSERT(offsetof(struct PaletteFade, page) == 0x3000);

/// The colour buffer (HEAP_SLOT_PALETTE_BUFFER) that fades the BG and OBJ
/// palettes: the source palette, the shown and target colour channels, their
/// per-frame steps, and two pages of packed RGB555 colours queued each frame.
struct BattleEffectBuffers {
  /// The source palette, RGB555: 224 BG colours, then 224 OBJ colours;
  /// palette[0] is the backdrop.
  u16 palette[0x1c0];
  /// The shown colours, as B, G and R channels shifted up 10 bits, three a
  /// colour.
  u16 current[0x540];
  u16 target[0x540]; ///< the colours current moves toward, as current
  u16 delta[0x540];  ///< added to each current channel each frame
  /// Two pages of RGB555 colours: one is built while the other is queued.
  u16 packed[2][0x1c0];
  u8 page;        ///< the packed page queued this frame
  s8 duration;    ///< frames; 0 when the colours are not moving
  s8 step;        ///< frames
  u8 padding2a03; ///< never named
};

STATIC_ASSERT(sizeof(struct BattleEffectBuffers) == 0x2a04);
STATIC_ASSERT(offsetof(struct BattleEffectBuffers, current) == 0x380);
STATIC_ASSERT(offsetof(struct BattleEffectBuffers, target) == 0xe00);
STATIC_ASSERT(offsetof(struct BattleEffectBuffers, delta) == 0x1880);
STATIC_ASSERT(offsetof(struct BattleEffectBuffers, packed) == 0x2300);
STATIC_ASSERT(offsetof(struct BattleEffectBuffers, page) == 0x2a00);

extern u16 gPalette_ChannelCurve[32];
extern u16 gPalette_ChannelCurveLight[32];
extern u16 gPalette_ChannelCurveContrast[32];

/// A colour buffer's build mode: below 0x8000 one RGB555 colour for all.
enum PaletteTransform {
  /// The backdrop colour (palette RAM's first colour) for all.
  PALETTE_TRANSFORM_FROM_SOURCE = 0x8000,
  PALETTE_TRANSFORM_PLAIN = 0x10000, ///< each colour unchanged
  PALETTE_TRANSFORM_GREY = 0x10001,  ///< grey of brightness (2R + 4G + B) / 7
  PALETTE_TRANSFORM_SEPIA = 0x10002,
  PALETTE_TRANSFORM_WARM = 0x10003,
  PALETTE_TRANSFORM_MUTED = 0x10004,
  PALETTE_TRANSFORM_PALE = 0x10005,
  PALETTE_TRANSFORM_RED = 0x10006,
  PALETTE_TRANSFORM_WARM2 = 0x10007, ///< half R + 6, two thirds G + 4, B - 6
  /// From here the TINT, BLEND and SPLIT bits are tested, TINT and BLEND
  /// carrying an RGB555 colour in the low 15 bits; any other value is the
  /// address of a ready buffer to copy.
  PALETTE_TRANSFORM_FLAGS = 0x100000,
  /// The brightness (2R + 4G + B) times the colour carried.
  PALETTE_TRANSFORM_TINT = 0x200000,
  /// The brightness R + G + B shared out in the proportions of the colour
  /// carried.
  PALETTE_TRANSFORM_BLEND = 0x400000,
  PALETTE_TRANSFORM_SPLIT = 0x800000 ///< each colour unchanged
};

#endif
