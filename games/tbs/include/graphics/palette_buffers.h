#ifndef ALCHEMY_PALETTE_BUFFERS_H
#define ALCHEMY_PALETTE_BUFFERS_H

/* @file */
/* Palette fades: the colours a fade moves from and to, and the tints (grey, */
/* sepia, dusk and the rest) a scene or the title intro builds a target from. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "system.h"

/* The colour channels a TITLE_PALETTE_FADE moves: B, G and R for each of the */
/* palette's colours. */
#define TITLE_PALETTE_FADE_CHANNELS (PLTT_COLORS * 3)

/* The background colours a PALETTE_FADE moves, palettes 0 to 13; as many */
/* sprite colours follow them. */
#define PALETTE_FADE_BG_COLORS (14 * PALETTE_COLORS)
/* The colour channels a PALETTE_FADE moves: B, G and R for each of its */
/* background and sprite colours. */
#define PALETTE_FADE_CHANNELS (2 * PALETTE_FADE_BG_COLORS * 3)

/* The title intro's palette fade, over every BG and sprite colour: the source
 */
/* palette, the shown and target colour channels, their steps, and two pages */
/* of packed colours. */
typedef struct TITLE_PALETTE_FADE {
	/* The source palette, RGB555: the BG colours, then the sprite colours. */
	u16 pal[PLTT_COLORS];
	/* The shown colours, as B, G and R channels shifted up 10 bits, three a */
	/* colour. */
	u16 cur[TITLE_PALETTE_FADE_CHANNELS];
	u16 tgt[TITLE_PALETTE_FADE_CHANNELS];	/* the colours current moves */
											/* toward, as current */
	u16 delta[TITLE_PALETTE_FADE_CHANNELS]; /* added to each current channel */
											/* each frame */
	/* Two pages of RGB555 colours: one is built while the other is queued. */
	u16 packed[2][PLTT_COLORS];
	u8 page;	 /* the packed page queued this frame */
	s8 duration; /* frames; 0 when the colours are not moving */
	u8 step;	 /* frames */
	u8 padding;
} TITLE_PALETTE_FADE;

STATIC_ASSERT(sizeof(TITLE_PALETTE_FADE) == 12292);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, cur) == 1024);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, tgt) == 4096);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, delta) == 7168);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, packed) == 10240);
STATIC_ASSERT(offsetof(TITLE_PALETTE_FADE, page) == 12288);

/* The palette fade (HEAP_SLOT_PALETTE_FADE) that fades the BG and sprite */
/* palettes: the source palette, the shown and target colour channels, their */
/* per-frame steps, and two pages of packed RGB555 colours queued each frame. */
typedef struct PALETTE_FADE {
	/* The source palette, RGB555: the BG colours, then as many sprite colours;
	 */
	/* palette[0] is the backdrop. */
	u16 pal[2 * PALETTE_FADE_BG_COLORS];
	/* The shown colours, as B, G and R channels shifted up 10 bits, three a */
	/* colour. */
	u16 cur[PALETTE_FADE_CHANNELS];
	u16 tgt[PALETTE_FADE_CHANNELS];	  /* the colours current moves toward, as
									   */
									  /* current */
	u16 delta[PALETTE_FADE_CHANNELS]; /* added to each current channel each */
									  /* frame */
	/* Two pages of RGB555 colours: one is built while the other is queued. */
	u16 packed[2][2 * PALETTE_FADE_BG_COLORS];
	u8 page;			/* the packed page queued this frame */
	s8 duration;		/* frames; 0 when the colours are not moving */
	s8 step;			/* frames */
	u8 unusedAfterStep; /* never read; two scenes set it to 1 with step */
} PALETTE_FADE;

STATIC_ASSERT(sizeof(PALETTE_FADE) == 10756);
STATIC_ASSERT(offsetof(PALETTE_FADE, cur) == 896);
STATIC_ASSERT(offsetof(PALETTE_FADE, tgt) == 3584);
STATIC_ASSERT(offsetof(PALETTE_FADE, delta) == 6272);
STATIC_ASSERT(offsetof(PALETTE_FADE, packed) == 8960);
STATIC_ASSERT(offsetof(PALETTE_FADE, page) == 10752);

/* The title intro's own copies of the colour channel curves its tints read: */
/* each maps a 5-bit channel to a fade channel shifted up 10 bits, plain, */
/* lighter, or with more contrast. */
extern u16 TitlePaletteFade_ChannelCurve[32];
extern u16 TitlePaletteFade_ChannelCurveLight[32];
extern u16 TitlePaletteFade_ChannelCurveContrast[32];

/* A palette tint, a palette fade's build mode: below PALETTE_TINT_FROM_SOURCE,
 */
/* one RGB555 colour for all. */
typedef enum PALETTE_TINT {
	/* The backdrop colour (palette RAM's first colour) for all. */
	PALETTE_TINT_FROM_SOURCE = 0x8000,
	/* Each colour unchanged; the tints after it are numbered on from it. */
	PALETTE_TINT_NONE = 0x10000,
	PALETTE_TINT_GRAYSCALE, /* grey of brightness (2R + 4G + B) / 7 */
	/* Brownish grey, as when Ivan reports to Layana in Hammet Palace: the shade
	 */
	/* (R + G + B) / SEPIA_SHADE_DIVISOR, red at 4 */
	/* times it plus 5, green and blue 3 times plus 5, each held to */
	/* SEPIA_LEVEL_MIN to SEPIA_LEVEL_MAX. */
	PALETTE_TINT_SEPIA,
	/* The storm's gloom: Vale's grass and paths go a dull, dark olive while the
	 */
	/* sprites keep their colours, between the lightning's flashes and at the */
	/* ending's darkened sea. Half R + 10, two thirds G + 8, B - 7; black stays
	 */
	/* black. */
	PALETTE_TINT_DIM,
	/* The backgrounds World Map fades to as Venus Lighthouse shakes after its
	 */
	/* beacon is lit: each channel held to a narrow range (MUTED_RED_MIN to */
	/* MUTED_RED_MAX and so on), then G and B + 2. */
	PALETTE_TINT_MUTED,
	/* A lightning flash: the storm's backgrounds wash out almost to white. Half
	 */
	/* of each channel, plus their mean. */
	PALETTE_TINT_PALE,
	/* The red Vale's eruption lightning flashes to from PALETTE_TINT_DIM: R */
	/* plus an eighth of G and of B; G and B two thirds. */
	PALETTE_TINT_RED,
	/* PALETTE_TINT_DIM darker: half R + 6, two thirds G + 4, B - 6. The */
	/* title's intro builds PALETTE_TINT_DIM with this recipe. */
	PALETTE_TINT_DIMMER,
	/* From here the TINT, BLEND and SPLIT bits are tested, TINT and BLEND */
	/* carrying an RGB555 colour in the low 15 bits; any other value is the */
	/* address of a ready buffer to copy. */
	PALETTE_TINT_FLAGS = 0x100000,
	/* The brightness (2R + 4G + B) times the colour carried, over */
	/* CUSTOM_TINT_DIVISOR. */
	PALETTE_TINT_CUSTOM = 0x200000,
	/* The brightness R + G + B shared out in the proportions of the colour */
	/* carried. */
	PALETTE_TINT_BLEND = 0x400000,
	PALETTE_TINT_SPLIT = 0x800000 /* each colour unchanged */
} PALETTE_TINT;

/* The brightness 2R + 4G + B that PALETTE_TINT_GRAYSCALE and */
/* PALETTE_TINT_CUSTOM build, with each channel where a fade keeps it, in */
/* blue's place: red moved up and doubled, green moved up and doubled twice. */
#define BRIGHTNESS_RED_SHIFT   (BLUE_SHIFT + 1)
#define BRIGHTNESS_GREEN_SHIFT (BLUE_SHIFT - GREEN_SHIFT + 2)

/* PALETTE_TINT_CUSTOM gives each channel the brightness 2R + 4G + B times the
 */
/* tint's level over this, so white reaches full level under a tint level of */
/* 14. */
#define CUSTOM_TINT_DIVISOR 96

/* PALETTE_TINT_SEPIA's shade is a colour's R + G + B over this, 0 to 9. */
#define SEPIA_SHADE_DIVISOR 10
/* PALETTE_TINT_SEPIA holds each channel to these levels. */
#define SEPIA_LEVEL_MIN 8
#define SEPIA_LEVEL_MAX 28

/* PALETTE_TINT_MUTED holds each channel to its own range. */
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
	PALETTES_SPRITES = 2
} PALETTE_SELECTION;

#endif
