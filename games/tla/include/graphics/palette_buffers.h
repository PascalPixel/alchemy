#ifndef ALCHEMY_PALETTE_BUFFERS_H
#define ALCHEMY_PALETTE_BUFFERS_H

/* The palette fades, the title's and the field's, and the tints they fade
   to. */

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"

/* The colour channels a TITLE_PALETTE_FADE moves: B, G and R for each of its
   512 colours. */
#define TITLE_PALETTE_FADE_CHANNELS (512 * 3)

/* The BG colours a PALETTE_FADE moves, palettes 0 to 13, and the OBJ colours
   that follow them, as many. */
#define PALETTE_FADE_BG_COLORS	(14 * PALETTE_COLORS)
#define PALETTE_FADE_OBJ_COLORS (14 * PALETTE_COLORS)
/* The colour channels a PALETTE_FADE moves: B, G and R for each of its BG and
   OBJ colours. */
#define PALETTE_FADE_CHANNELS \
	((PALETTE_FADE_BG_COLORS + PALETTE_FADE_OBJ_COLORS) * 3)

/* The entry that ends a Uint16 list of palette slots or of colours. */
#define PALETTE_LIST_END 65535

/* A colour buffer for all 512 palette colours: the source palette, the shown
   and target colour channels, their steps, and two pages of packed colours. */
typedef struct TITLE_PALETTE_FADE {
	/* The source palette, RGB555: 256 BG colours, then 256 OBJ colours. */
	Uint16 pal[512];
	/* The shown colours, as B, G and R channels shifted up 10 bits, three a
	   colour. */
	Uint16 cur[TITLE_PALETTE_FADE_CHANNELS];
	/* the colours current moves toward, as current */
	Uint16 tgt[TITLE_PALETTE_FADE_CHANNELS];
	/* added to each current channel each frame */
	Uint16 delta[TITLE_PALETTE_FADE_CHANNELS];
	/* Two pages of RGB555 colours: one is built while the other is queued. */
	Uint16 packed[2][512];
	Uint8  page;	 /* the packed page queued this frame */
	Sint8  duration; /* frames; 0 when the colours are not moving */
	Uint8  step;	 /* frames */
	Uint8  padding;
} TITLE_PALETTE_FADE;

STATIC_ASSERT(sizeof(TITLE_PALETTE_FADE) == 12292);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, cur) == 1024);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, tgt) == 4096);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, delta) == 7168);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, packed) == 10240);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, page) == 12288);

/* The palette fade (HEAP_SLOT_PALETTE_FADE) that fades the BG and OBJ palettes:
   the source palette, the shown and target colour channels, their per-frame
   steps, and two pages of packed RGB555 colours queued each frame. */
typedef struct PALETTE_FADE {
	/* The source palette, RGB555: 224 BG colours, then 224 OBJ colours;
	   palette[0] is the backdrop. */
	Uint16 pal[448];
	/* The shown colours, as B, G and R channels shifted up 10 bits, three a
	   colour. */
	Uint16 cur[PALETTE_FADE_CHANNELS];
	/* the colours current moves toward, as current */
	Uint16 tgt[PALETTE_FADE_CHANNELS];
	/* added to each current channel each frame */
	Uint16 delta[PALETTE_FADE_CHANNELS];
	/* Two pages of RGB555 colours: one is built while the other is queued. */
	Uint16 packed[2][448];
	Uint8  page;	 /* the packed page queued this frame */
	Sint8  duration; /* frames; 0 when the colours are not moving */
	Sint8  step;	 /* frames */
	Uint8  padding;
} PALETTE_FADE;

STATIC_ASSERT(sizeof(PALETTE_FADE) == 10756);
STATIC_ASSERT(offsetof(PALETTE_FADE, cur) == 896);
STATIC_ASSERT(offsetof(PALETTE_FADE, tgt) == 3584);
STATIC_ASSERT(offsetof(PALETTE_FADE, delta) == 6272);
STATIC_ASSERT(offsetof(PALETTE_FADE, packed) == 8960);
STATIC_ASSERT(offsetof(PALETTE_FADE, page) == 10752);

void mnTitlePaletteFadeBuildCols(
	Uint32 tint, Uint16 *source, Uint16 *destination, Sint32 selectedPalettes);

void mnTitlePaletteFadeInit(void);
void mnTitlePaletteFadeUpdate(void);
void mnTitlePaletteFadeSetTarget(Sint32 tint, Sint32 selectedPalettes);
void mnTitlePaletteFadeSetBackdrop(Sint32 color);
void mnTitlePaletteFadeStart(Sint32 frames);
void mnTitlePaletteFadeCalcSteps(
	Sint16 *current, Sint16 *target, Sint16 *step, Sint32 frames);
void mnTitlePaletteFadeRelease(void);

/* A palette tint, a palette fade's build mode: up to RGB_WHITE, one RGB555
   colour for all. */
typedef enum PALETTE_TINT {
	/* The backdrop colour (palette RAM's first colour) for all. */
	PALETTE_TINT_FROM_SOURCE = RGB_WHITE + 1,
	PALETTE_TINT_NONE = 0x10000, /* each colour unchanged */
	PALETTE_TINT_GRAYSCALE,		 /* grey of brightness (2R + 4G + B) / 7 */
	/* Brownish grey: the brightness (R + G + B) / 10, red at 4 times it plus 5,
	   green and blue 3 times plus 5, each held to 8 to 28. */
	PALETTE_TINT_SEPIA,
	/* Half R + 10, two thirds G + 8, B - 7; black stays black. The title's
	   (TITLE_PALETTE_FADE) is PALETTE_TINT_WARM_DARK's, black included. */
	PALETTE_TINT_WARM,
	/* Each channel held to a narrow range, then G and B + 2: the field's fade
	   holds R 8 to 29, G 14 to 25, B 14 to 27 and leaves the first colour; the
	   title's (TITLE_PALETTE_FADE) R 10 to 28, G 16 to 24, B 16 to 26. */
	PALETTE_TINT_MUTED,
	PALETTE_TINT_PALE, /* half of each channel, plus their mean */
	/* R plus an eighth of G and of B; G and B two thirds. */
	PALETTE_TINT_RED,
	PALETTE_TINT_WARM_DARK, /* half R + 6, two thirds G + 4, B - 6 */
	/* A fifth off each channel of a bright colour. */
	PALETTE_TINT_DIM_BRIGHT,
	/* A third of R + G + B, B three steps darker. */
	PALETTE_TINT_DIM_GRAY,
	/* From here the CUSTOM, BLEND and SPLIT bits are tested, CUSTOM and BLEND
	   carrying an RGB555 colour in the low 15 bits; any other value is the
	   address of a ready buffer to copy. */
	PALETTE_TINT_FLAGS = 0x100000,
	/* The brightness (2R + 4G + B) times the colour carried. */
	PALETTE_TINT_CUSTOM = 0x200000,
	/* The brightness R + G + B shared out in the proportions of the colour
	   carried. */
	PALETTE_TINT_BLEND = 0x400000,
	PALETTE_TINT_SPLIT = 0x800000 /* each colour unchanged */
} PALETTE_TINT;

/* The brightness 2R + 4G + B that PALETTE_TINT_GRAYSCALE and
   PALETTE_TINT_CUSTOM build, with each channel where a fade keeps it, in blue's
   place: red moved up and doubled, green moved up and doubled twice. */
#define BRIGHTNESS_RED_SHIFT   (RGB_BLUE_SHIFT + 1)
#define BRIGHTNESS_GREEN_SHIFT (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT + 2)

/* PALETTE_TINT_CUSTOM gives each channel the brightness 2R + 4G + B times the
   tint's level over this, so white reaches full level under a tint level of
   14. */
#define CUSTOM_TINT_DIVISOR 96

/* PALETTE_TINT_SEPIA's shade is a colour's R + G + B over this, 0 to 9. */
#define SEPIA_SHADE_DIVISOR 10
/* PALETTE_TINT_SEPIA holds each channel to these levels. */
#define SEPIA_LEVEL_MIN		8
#define SEPIA_LEVEL_MAX		28

/* The title's PALETTE_TINT_MUTED holds each channel to its own range. */
#define MUTED_RED_MIN	10
#define MUTED_RED_MAX	28
#define MUTED_GREEN_MIN 16
#define MUTED_GREEN_MAX 24
#define MUTED_BLUE_MIN	16
#define MUTED_BLUE_MAX	26

/* Which palettes mnTitlePaletteFadeBuildCols builds. */
typedef enum PALETTE_SELECTION {
	PALETTES_ALL = 0,
	PALETTES_BG = 1,
	PALETTES_OBJECTS = 2
} PALETTE_SELECTION;

#endif
