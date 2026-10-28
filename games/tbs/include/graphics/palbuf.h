#ifndef ALCHEMY_PALBUF_H
#define ALCHEMY_PALBUF_H

/// @file
/// Palette fades, the battle effect palette buffers and the colour channel
/// curves.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// The colour channels a TitlePaletteFade moves: B, G and R for each of its 512
/// colours.
#define TITLE_PALETTE_FADE_CHANNELS (512 * 3)

/// The colour channels a PaletteFade moves: B, G and R for each of its 448
/// colours.
#define PALETTE_FADE_CHANNELS (448 * 3)

/// A palette fade for all 512 palette colours: the source palette, the shown
/// and target colour channels, their steps, and two pages of packed colours.
/// The title intro's form of HEAP_SLOT_PALETTE_FADE, allocated by
/// TitlePaletteFade_Initialize.
struct TitlePaletteFade {
  /// The source palette, RGB555: 256 BG colours, then 256 sprite colours.
  u16 palette[512];
  /// The shown colours, as B, G and R channels shifted up 10 bits, three a
  /// colour.
  u16 current[TITLE_PALETTE_FADE_CHANNELS];
  u16 target[TITLE_PALETTE_FADE_CHANNELS]; ///< the colours current moves
                                           ///< toward, as current
  u16 delta[TITLE_PALETTE_FADE_CHANNELS];  ///< added to each current channel
                                           ///< each frame
  /// Two pages of RGB555 colours: one is built while the other is queued.
  u16 packed[2][512];
  u8 page;        ///< the packed page queued this frame
  s8 duration;    ///< frames; 0 when the colours are not moving
  u8 step;        ///< frames
  u8 padding3003; ///< never named
};

STATIC_ASSERT(sizeof(struct TitlePaletteFade) == 0x3004);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, current) == 0x400);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, target) == 0x1000);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, delta) == 0x1c00);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, packed) == 0x2800);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, page) == 0x3000);

/// The palette fade (HEAP_SLOT_PALETTE_FADE) that fades the BG and sprite
/// palettes: the source palette, the shown and target colour channels, their
/// per-frame steps, and two pages of packed RGB555 colours queued each frame.
/// FieldSequence_Run allocates it on scene load (PaletteFade_Initialize),
/// and gPaletteFadeWork reads it.
struct PaletteFade {
  /// The source palette, RGB555: 224 BG colours, then 224 sprite colours;
  /// palette[0] is the backdrop.
  u16 palette[448];
  /// The shown colours, as B, G and R channels shifted up 10 bits, three a
  /// colour.
  u16 current[PALETTE_FADE_CHANNELS];
  u16 target[PALETTE_FADE_CHANNELS]; ///< the colours current moves toward, as
                                     ///< current
  u16 delta[PALETTE_FADE_CHANNELS];  ///< added to each current channel each
                                     ///< frame
  /// Two pages of RGB555 colours: one is built while the other is queued.
  u16 packed[2][448];
  u8 page;        ///< the packed page queued this frame
  s8 duration;    ///< frames; 0 when the colours are not moving
  s8 step;        ///< frames
  u8 padding2a03; ///< never named
};

STATIC_ASSERT(sizeof(struct PaletteFade) == 0x2a04);
STATIC_ASSERT(offsetof(struct PaletteFade, current) == 0x380);
STATIC_ASSERT(offsetof(struct PaletteFade, target) == 0xe00);
STATIC_ASSERT(offsetof(struct PaletteFade, delta) == 0x1880);
STATIC_ASSERT(offsetof(struct PaletteFade, packed) == 0x2300);
STATIC_ASSERT(offsetof(struct PaletteFade, page) == 0x2a00);

extern u16 gTitleIntro_ChannelCurve[32];
extern u16 gTitleIntro_ChannelCurveLight[32];
extern u16 gTitleIntro_ChannelCurveContrast[32];

/// A palette tint, a palette fade's build mode: below 0x8000 one RGB555 colour
/// for all.
enum PaletteTint {
  /// The backdrop colour (palette RAM's first colour) for all.
  PALETTE_TINT_FROM_SOURCE = 0x8000,
  PALETTE_TINT_NONE = 0x10000,      ///< each colour unchanged
  PALETTE_TINT_GRAYSCALE = 0x10001, ///< grey of brightness (2R + 4G + B) / 7
  /// Brownish grey: the brightness (R + G + B) / 10, red at 4 times it plus
  /// 5, green and blue 3 times plus 5, each held to 8 to 28.
  PALETTE_TINT_SEPIA = 0x10002,
  /// Half R + 10, two thirds G + 8, B - 7; black stays black.
  PALETTE_TINT_WARM = 0x10003,
  /// Each channel held to a narrow range (R 10 to 28, G 16 to 24, B 16 to 26),
  /// then G and B + 2.
  PALETTE_TINT_MUTED = 0x10004,
  PALETTE_TINT_PALE = 0x10005, ///< half of each channel, plus their mean
  /// R plus an eighth of G and of B; G and B two thirds.
  PALETTE_TINT_RED = 0x10006,
  PALETTE_TINT_WARM_DARK = 0x10007, ///< half R + 6, two thirds G + 4, B - 6
  /// From here the TINT, BLEND and SPLIT bits are tested, TINT and BLEND
  /// carrying an RGB555 colour in the low 15 bits; any other value is the
  /// address of a ready buffer to copy.
  PALETTE_TINT_FLAGS = 0x100000,
  /// The brightness (2R + 4G + B) times the colour carried.
  PALETTE_TINT_CUSTOM = 0x200000,
  /// The brightness R + G + B shared out in the proportions of the colour
  /// carried.
  PALETTE_TINT_BLEND = 0x400000,
  PALETTE_TINT_SPLIT = 0x800000 ///< each colour unchanged
};

/// Which palettes TitlePaletteFade_BuildColors builds.
enum PaletteSelection {
  PALETTES_ALL = 0,
  PALETTES_BG = 1,
  PALETTES_OBJECTS = 2
};

#endif
