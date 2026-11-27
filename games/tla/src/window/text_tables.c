/* @file */
/* The text's tables: each glyph's width, the text colours' pens, and the */
/* party status's divider columns. */

#include "types.h"
#include "version.h"

/* clang-format off */
/* Each glyph's width in pixels from character 0x20: 224 in the Latin fonts, */
/* 96 in the Japanese one. */
const u8 Font_GlyphWidths[] = {
#if LANGUAGE != LANGUAGE_JAPANESE
    5, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 5, 4, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 6, 5, 6,
    8, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 6, 6, 8, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 5, 6, 5, 6, 6,
    6, 6, 6, 6, 6, 6, 5, 6, 6, 4, 4, 5, 4, 8, 6, 6,
    6, 6, 6, 6, 5, 6, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1,
    8, 2, 8, 2, 8, 6, 8, 6, 8, 4, 8, 4, 8, 7, 7, 7,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 4, 1, 1, 1, 1, 1, 1, 6, 1, 6, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 8, 8, 6,
    6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
#else
    7, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 6, 4, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 5, 6, 5, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 6, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 5, 6, 5, 7, 7,
    6, 7, 7, 7, 7, 7, 5, 7, 7, 4, 4, 6, 4, 8, 7, 7,
    7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1,
#endif
};
/* clang-format on */

/* clang-format off */
/* The {background, foreground} pens of the eight text colours. */
const u8 Font_ColorPens[16] = {
    0, 3,
    13, 13,
    14, 15,
    15, 15,
    15, 1,
    1, 13,
    13, 2,
    2, 1,
};
/* clang-format on */

/* The party status's divider columns, in tiles: the Djinn column's line */
/* first, then the members', ending at -1; mnWindowDrawColumnBorders reads from
 */
/* index 0 or 1 to that end. A second list follows, which it never reaches. */
const s8 Window_PartyColumnOffsets[] = {
	0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1,
};

#if TLA_PATCH_4 || !TLA_PATCH_3
/* Unused: the HP and PP labels' tile numbers (WINDOW_TILE_HP_LABEL and */
/* WINDOW_TILE_PP_LABEL, each with its second tile and a 0), which nothing */
/* reads; the French edition drops them. */
const u8 Window_UnusedPartyColumnBytes[] = {
#if TLA_PATCH_4
	29, 30, 0, 31, 5, 0,
#else
	29, 30, 0, 31, 30, 0,
#endif
};
#endif
