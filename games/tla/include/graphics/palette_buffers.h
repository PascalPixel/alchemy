#ifndef ALCHEMY_PALETTE_BUFFERS_H
#define ALCHEMY_PALETTE_BUFFERS_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "system/io_reg.h"

/// @file
/// Palette fades, the battle effect palette buffers and the colour channel
/// curves.

/// The colour channels a TitlePaletteFade moves: B, G and R for each of its 512
/// colours.
#define TITLE_PALETTE_FADE_CHANNELS (512 * 3)

/// The colour channels a PaletteFade moves: B, G and R for each of its 448
/// colours.
#define PALETTE_FADE_CHANNELS (448 * 3)

/// The entry that ends a u16 list of palette slots or of colours.
#define PALETTE_LIST_END 0xffff

/// A colour buffer for all 512 palette colours: the source palette, the shown
/// and target colour channels, their steps, and two pages of packed colours.
struct TitlePaletteFade {
  /// The source palette, RGB555: 256 BG colours, then 256 OBJ colours.
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
  u8 page;     ///< the packed page queued this frame
  s8 duration; ///< frames; 0 when the colours are not moving
  u8 step;     ///< frames
  u8 padding;  ///< never named
};

STATIC_ASSERT(sizeof(struct TitlePaletteFade) == 12292);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, current) == 1024);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, target) == 4096);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, delta) == 7168);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, packed) == 10240);
STATIC_ASSERT(offsetof(struct TitlePaletteFade, page) == 12288);

/// The BG colours a palette fade keeps, palettes 0 to 13, before its OBJ
/// colours.
#define PALETTE_FADE_BG_COLORS (14 * PALETTE_COLORS)

/// The palette fade (HEAP_SLOT_PALETTE_FADE) that fades the BG and OBJ
/// palettes: the source palette, the shown and target colour channels, their
/// per-frame steps, and two pages of packed RGB555 colours queued each frame.
struct PaletteFade {
  /// The source palette, RGB555: 224 BG colours, then 224 OBJ colours;
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
  u8 page;     ///< the packed page queued this frame
  s8 duration; ///< frames; 0 when the colours are not moving
  s8 step;     ///< frames
  u8 padding;  ///< never named
};

STATIC_ASSERT(sizeof(struct PaletteFade) == 10756);
STATIC_ASSERT(offsetof(struct PaletteFade, current) == 896);
STATIC_ASSERT(offsetof(struct PaletteFade, target) == 3584);
STATIC_ASSERT(offsetof(struct PaletteFade, delta) == 6272);
STATIC_ASSERT(offsetof(struct PaletteFade, packed) == 8960);
STATIC_ASSERT(offsetof(struct PaletteFade, page) == 10752);

void TitlePaletteFade_BuildColors(u32 mode, u16 *source, u16 *destination,
                                  s32 half);

void TitlePaletteFade_Initialize(void);
void TitlePaletteFade_Update(void);
void TitlePaletteFade_SetTarget(s32 mode, s32 selectedPalettes);
void TitlePaletteFade_SetBackdrop(s32 value);
void TitlePaletteFade_Start(s32 frames);
void TitlePaletteFade_CalculateSteps(s16 *source, s16 *target, s16 *step,
                                     s32 frames);
void TitlePaletteFade_Release(void);

/// A palette tint, a palette fade's build mode: below 0x8000 one RGB555 colour
/// for all.
enum PaletteTint {
  /// The backdrop colour (palette RAM's first colour) for all.
  PALETTE_TINT_FROM_SOURCE = 0x8000,
  PALETTE_TINT_NONE = 0x10000, ///< each colour unchanged
  PALETTE_TINT_GRAYSCALE,      ///< grey of brightness (2R + 4G + B) / 7
  /// Brownish grey: the brightness (R + G + B) / 10, red at 4 times it plus
  /// 5, green and blue 3 times plus 5, each held to 8 to 28.
  PALETTE_TINT_SEPIA,
  /// Half R + 10, two thirds G + 8, B - 7; black stays black.
  PALETTE_TINT_WARM,
  /// Each channel held to a narrow range, then G and B + 2: the field's fade
  /// holds R 8 to 29, G 14 to 25, B 14 to 27 and leaves the first colour; the
  /// title's (TitlePaletteFade) R 10 to 28, G 16 to 24, B 16 to 26.
  PALETTE_TINT_MUTED,
  PALETTE_TINT_PALE, ///< half of each channel, plus their mean
  /// R plus an eighth of G and of B; G and B two thirds.
  PALETTE_TINT_RED,
  PALETTE_TINT_WARM_DARK, ///< half R + 6, two thirds G + 4, B - 6
  /// A fifth off each channel of a bright colour.
  PALETTE_TINT_DIM_BRIGHT,
  /// A third of R + G + B, B three steps darker.
  PALETTE_TINT_DIM_GRAY,
  /// From here the CUSTOM, BLEND and SPLIT bits are tested, CUSTOM and BLEND
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

/// The brightness 2R + 4G + B that PALETTE_TINT_GRAYSCALE and
/// PALETTE_TINT_CUSTOM build, with each channel where a fade keeps it, in
/// blue's place: red moved up and doubled, green moved up and doubled twice.
#define BRIGHTNESS_RED_SHIFT (RGB_BLUE_SHIFT + 1)
#define BRIGHTNESS_GREEN_SHIFT (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT + 2)

/// Which palettes TitlePaletteFade_BuildColors builds.
enum PaletteSelection {
  PALETTES_ALL = 0,
  PALETTES_BG = 1,
  PALETTES_OBJECTS = 2
};

#endif
