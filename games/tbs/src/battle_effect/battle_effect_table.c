/* Picture layouts, timing rows and handler entries for the battle effects. Each
   table is read only by the part its prefix names, except the BattleEffect_
   tables, which many parts share. Picture offsets are byte offsets from the
   sprite sheet start, accumulated as width times height. */

#include "battle_effect/battle_effect_table.h"
#include "battle_effect/battle_effect.h"
#include "field/sprite.h"

/* Keep the definitions in link order. */

struct BATTLE_EFFECT_PARM;

/* Picture layouts that several effects read from the same sprite sheet; each
   effect keeps its own copy, defined from one of these. */

/* The six pictures of an erupting flame puff: width, height, how far below its
   anchor its top goes, so that all six stand on one line, and where each
   begins. */
#define FLAME_PUFF_PICTURES		   {0, 552, 1480, 2590, 3913, 5185}
#define FLAME_PUFF_PICTURE_WIDTHS  {24, 29, 30, 27, 24, 16}
#define FLAME_PUFF_PICTURE_HEIGHTS {23, 32, 37, 49, 53, 52}
/* How far below its anchor each picture's top goes. */
#define FLAME_PUFF_PICTURE_BIAS_Y  {41, 32, 26, 11, 4, 2}
/* The sides of the seven square pictures of a blast, a puff or a flash. */
#define BLAST_PICTURE_SIZES		   {16, 32, 48, 48, 48, 48, 48}
/* Where each blast picture begins. */
#define BLAST_PICTURES			   {0, 256, 1280, 3584, 5888, 8192, 10496}
#define EARTH_WALL_PICTURE_WIDTHS \
	{17, 19, 20} /* the earth wall's three widths */
#define EARTH_WALL_PICTURE_HEIGHTS {49, 55, 64}
/* Where each earth wall picture begins. */
#define EARTH_WALL_PICTURES		   {2702, 3535, 4580}
/* the six slash widths */
#define SLASH_PICTURE_WIDTHS	   {38, 54, 71, 74, 77, 65}
#define SLASH_PICTURE_HEIGHTS	   {7, 9, 11, 13, 13, 11}
/* Each slash picture's reach from the target. */
#define SLASH_PICTURE_REACH		   {58, 42, 25, 13, 5, 0}
/* The flash picture for each three ticks at the end of a flash's life. */
#define FLASH_FADE_PICTURES		   {5, 5, 4, 3, 2, 1}
/* The flips a glint may be drawn in: none, and mirrored each way and both. */
#define GLINT_FLIPS				   {0, COMPOSITE_FLIP_X, COMPOSITE_FLIP_Y, COMPOSITE_FLIP}
/* Where Condemn and Mortal Danger draw the reaper's seven pictures (enum
   REAPER_SHEET_PICTURE, then RED_CRESCENT_PICTURE) as it sways: an x for each
   casting side (BATTLE_EFFECT_SIDE), and a y. */
#define REAPER_PICTURE_X		   {{10, 30, 0, 0, 0, 6, 52}, {60, 0, 0, 0, 0, 0, 0}}
#define REAPER_PICTURE_Y		   {0, 20, 2, 2, 34, 64, 68}

/* Shared picture layouts, read by many parts. */

/* Mote pictures: particle sprite sheets hold ten, picture n (from 0) n + 1
   pixels wide and twice that tall. It stays the first table, where the blitter
   template's walk ends. */
Uint16 BattleEffectMotePictures[] = {0, 2, 10, 28, 60, 110, 182, 280, 408, 570};
/* Flare pictures: the first ten are squares 2x2 to 20x20, picture n (from 0) n
   + 1 pixels from its centre to each edge (EFFECT_DRAW_FLARE, and Neptune and
   Bind directly). The second ten begin at twice those offsets. */
Uint16 BattleEffectFlarePictures[] = {0, 4, 20, 56, 120, 220, 364, 560, 816,
	1140, 0, 8, 40, 112, 240, 440, 728, 1120, 1632, 2280};
/* Nine square sparkle pictures (sparkle_dots_00 to _08), read by the falling,
   frost, healing, Djinni heal and Boreas parts. */
Uint16 BattleEffectSparklePictures[] = {
	0, 25, 74, 123, 172, 221, 246, 255, 256};
/* Each sparkle picture's side. */
Uint8 BattleEffectSparkleSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};
/* The six pictures of an erupting flame puff: width, height, how far below its
   anchor its top goes (so that all six stand on about the same line) and where
   each begins in the sprite sheet. Each is drawn centred across x; the Mars
   Djinni shows each for 8 ticks, centred on the flame's x. Read by the Djinn
   attacks, Flare, the casting impacts, Atalanta and the dragon cloud. */
Uint8  BattleEffectPuffArcPictureWidths[] = FLAME_PUFF_PICTURE_WIDTHS;
Uint8  BattleEffectPuffArcPictureHeights[] = FLAME_PUFF_PICTURE_HEIGHTS;
Uint8  BattleEffectPuffArcPictureBiasY[] = FLAME_PUFF_PICTURE_BIAS_Y;
Uint16 BattleEffectPuffArcPictures[] = FLAME_PUFF_PICTURES;
/* Six glint pictures, each GlintPictureWidths by GlintPictureHeights pixels. */
Uint16 BattleEffectGlintPictures[] = {0, 589, 1302, 2015, 2414, 2750};
Uint8  BattleEffectGlintPictureWidths[] = {19, 23, 23, 19, 16, 17};
Uint8  BattleEffectGlintPictureHeights[] = {31, 31, 31, 21, 21, 24};

/* battle_effect_falling.c: Falling shard and bolt wave counts. */

/* Ice, by variant: shards and ticks (ICE_SHARD_COUNT, ICE_TICKS). */
Uint8 IceCounts[][2] = {
	/* shards, ticks */
	[EFFECT_VARIANT_BASIC] = {16, 46},	   /* Ice */
	[EFFECT_VARIANT_STRONGER] = {20, 52},  /* Ice Horn */
	[EFFECT_VARIANT_STRONGEST] = {32, 80}, /* Ice Missile */
};
/* Douse, by variant: the DOUSE_ columns. */
Uint8 DouseCounts[][4] = {
	/* bolts, sparks per bolt, spacing, ticks */
	[EFFECT_VARIANT_BASIC] = {6, 8, 32, 80},	 /* Douse */
	[EFFECT_VARIANT_STRONGER] = {8, 16, 24, 80}, /* Drench */
	[EFFECT_VARIANT_STRONGEST] = {32, 4, 8, 80}, /* Deluge */
};

/* battle_effect_streaks.c: Streak volley styles and the struck target's
   tremble. */

/* Each streak style's effect, hit, motion and shake, by the STREAK_ columns
   (battle_effect_streaks.c). */
Sint8 StreakStyles[][4] = {
	/* STREAK_ANIMATE, STREAK_HIT, STREAK_MOTION, STREAK_SHAKE */
	{0, 1, 0, 0},  /* STREAK_STYLE_EMBER */
	{0, 1, 4, 0},  /* STREAK_STYLE_LIME */
	{0, 1, -1, 1}, /* STREAK_STYLE_VIOLET */
	{1, 1, -1, 1}, /* STREAK_STYLE_RED */
	{1, 1, 4, 0},  /* STREAK_STYLE_CYAN */
};
/* Pixels the target trembles by, tick by tick. */
Sint8 StreakTremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

/* battle_effect_target_strike.c: Target strike timing, motion and pictures. */

/* Each of the twelve target strike styles, by the TARGET_STRIKE_ columns
   (battle_effect_target_strike.c). */
/* clang-format off */
Uint8 TargetStrikeStyles[][7] = {
	/* TARGET_STRIKE_: KIND, HITS, FLAGS, COLOUR, PICTURE_FRAMES, COMPOSITE, */
	/* TICKS */
	[TARGET_STRIKE_STYLE_BLUE_SPARK_BITES] =
		{STRIKE_KIND_BITE, 2, STRIKE_SPARKS, STRIKE_COLOR_BLUE, 6, COMPOSITE_ADDITIVE, 70},
	[TARGET_STRIKE_STYLE_LIME_SMOKE_BITE] =
		{STRIKE_KIND_BITE, 1, STRIKE_SMOKE, STRIKE_COLOR_LIME, 6, COMPOSITE_LIGHTEN, 54},
	[TARGET_STRIKE_STYLE_PINK_SMOKE_BITES] =
		{STRIKE_KIND_BITE, 3, STRIKE_SMOKE, STRIKE_COLOR_PINK, 6, COMPOSITE_LIGHTEN, 96},
	[TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES] =
		{STRIKE_KIND_BITE, 3, STRIKE_ABSORB, STRIKE_COLOR_EMBER, 4, COMPOSITE_LIGHTEN, 96},
	[TARGET_STRIKE_STYLE_MARS_DRAIN_BITES] =
		{STRIKE_KIND_BITE, 3, STRIKE_ABSORB, STRIKE_COLOR_MARS, 4, COMPOSITE_LIGHTEN, 96},
	[TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES] =
		{STRIKE_KIND_BITE, 3, STRIKE_GLINTS, STRIKE_COLOR_LIGHTNING, 4, COMPOSITE_ADDITIVE, 96},
	[TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH] =
		{STRIKE_KIND_SLASH, 1, STRIKE_GLINTS, STRIKE_COLOR_LIGHTNING, 4, COMPOSITE_ADDITIVE, 38},
	[TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES] =
		{STRIKE_KIND_SLASH, 2, STRIKE_SPARKS, STRIKE_COLOR_LIGHTNING, 2, COMPOSITE_ADDITIVE, 48},
	[TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH] =
		{STRIKE_KIND_SLASH, 1, STRIKE_SPARKS | STRIKE_HEAVY, STRIKE_COLOR_MARS, 2,
		 COMPOSITE_ADDITIVE, 50},
	[TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH] =
		{STRIKE_KIND_SLASH, 1, STRIKE_SMOKE, STRIKE_COLOR_LIME, 2, COMPOSITE_ADDITIVE, 46},
	[TARGET_STRIKE_STYLE_MARS_SMOKE_BITE] =
		{STRIKE_KIND_BITE, 1, STRIKE_SMOKE, STRIKE_COLOR_MARS, 6, COMPOSITE_LIGHTEN, 54},
	[TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES] =
		{STRIKE_KIND_BITE, 3, STRIKE_ABSORB, STRIKE_COLOR_MARS, 4, COMPOSITE_ADDITIVE, 96},
};
/* clang-format on */
/* The six slash pictures: size, sprite sheet offset and reach from the
   target. */
Uint8  TargetStrikeWidths[] = SLASH_PICTURE_WIDTHS;
Uint8  TargetStrikeHeights[] = SLASH_PICTURE_HEIGHTS;
Uint16 TargetStrikePictures[] = {3200, 3466, 3952, 4733, 5695, 6696};
Uint8  TargetStrikeReach[] = SLASH_PICTURE_REACH;
/* The fang stage drawn at each picture time of a bite, four to a hit: the first
   hit closes through stages 0 to 3, and each later hit reads entries 4, 1, 2
   and 3 (pictures above 4 wrap back by 4), opening to 2 and 1 and closing again
   to 3. The hit lands on stage 3, the fangs met. */
Uint8 TargetStrikeBiteStages[] = {0, 1, 2, 3, 2};
/* The flips a glint may be drawn in. */
Uint8 TargetStrikeGlintDrawFlags[] = GLINT_FLIPS;

/* battle_effect_death_plunge.c: Blade rain and shattering tile pieces. */

/* The sword pictures, by how steeply the sword falls. */
Uint8  BladeRainSwordWidths[] = {20, 21, 22, 25};
Uint8  BladeRainSwordHeights[] = {32, 31, 27, 24};
Uint16 BladeRainSwordPictures[] = {0, 640, 1291, 1885};
/* The 33 pieces of the ShatteringOpeningSheet picture: where each starts on the
   canvas, its size, and where its pixels begin in the decoded picture. The
   offsets carry one more entry, where the last piece ends. */
Uint8 ShatteringOpeningPieceWidths[] = {21, 20, 24, 30, 35, 69, 19, 23, 9, 22,
	33, 10, 18, 9, 7, 9, 17, 55, 30, 25, 40, 29, 10, 28, 14, 5, 32, 23, 19, 32,
	8, 43, 15};
Uint8 ShatteringOpeningPieceHeights[] = {23, 47, 25, 40, 20, 53, 28, 29, 23, 14,
	35, 17, 18, 14, 15, 12, 13, 37, 38, 41, 60, 41, 15, 45, 59, 41, 26, 50, 21,
	35, 12, 57, 11};
Uint16 ShatteringOpeningPiecePictures[] = {0, 483, 1423, 2023, 3223, 3923, 7580,
	8112, 8779, 8986, 9294, 10449, 10619, 10943, 11069, 11174, 11282, 11503,
	13538, 14678, 15703, 18103, 19292, 19442, 20702, 21528, 21733, 22565, 23715,
	24114, 25234, 25330, 27781, 27946};
Uint8  ShatteringOpeningPieceStartX[] = {0, 21, 28, 46, 70, 51, 0, 13, 37, 77,
	 87, 19, 25, 35, 44, 45, 48, 65, 0, 0, 0, 5, 33, 15, 36, 46, 47, 50, 53, 65,
	 72, 77, 105};
Uint8 ShatteringOpeningPieceStartY[] = {0, 0, 0, 0, 0, 0, 19, 17, 23, 28, 8, 36,
	43, 43, 40, 47, 48, 37, 42, 67, 60, 79, 62, 75, 61, 62, 60, 70, 74, 85, 108,
	63, 74};

/* battle_effect_death_plunge.c and battle_effect_spider_web.c: Canvas corner
   sparks and two-resource pictures. */

/* The element burst's corner sparks: offsets and flips. */
Sint8 ElementBurstSparkX[] = {0, 32, 0, 32};
Sint8 ElementBurstSparkY[] = {0, 0, 32, 32};
Uint8 ElementBurstSparkFlips[] = GLINT_FLIPS;
/* The six pictures: the first four from the swirl sprite sheet, the last two
   from the tail sprite sheet. SwirlSwipePictureX has six entries for each
   side. */
Uint8  SwirlSwipePictureWidths[] = {40, 112, 112, 112, 88, 64};
Uint8  SwirlSwipePictureHeights[] = {56, 80, 64, 64, 80, 88};
Uint16 SwirlSwipePictures[] = {0, 2240, 11200, 18368, 0, 7040};
Uint8  SwirlSwipePictureX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};
Sint8  SwirlSwipePictureBiasY[] = {0, 4, 24, -6, -20, -28, 0, 0};

/* battle_effect_gathering_lights.c */

/* Gathering lights, by mode (GATHERING_LIGHTS_MODE): the lights per target and
   the ticks before the targets' turns. */
Uint8 GatheringLightsCounts[][2] = {
	/* lights per target, ticks */
	[GATHERING_LIGHTS_DEMON_NIGHT] = {16, 88},
	[GATHERING_LIGHTS_RUMBLE] = {8, 72},
	[GATHERING_LIGHTS_HAUNTING] = {12, 80},
};

/* battle_effect_mirrored_wall.c: Mirrored earth wall pictures. */

/* The earth wall's three images: the mirrored wall reads them at the start of
   its sprite sheet, the rising wall after its rebuilt rows. Both share the
   sizes. */
Uint16 MirroredWallPictures[] = EARTH_WALL_PICTURES;
Uint8  MirroredWallPictureWidths[] = EARTH_WALL_PICTURE_WIDTHS;
Uint8  MirroredWallPictureHeights[] = EARTH_WALL_PICTURE_HEIGHTS;

/* battle_effect_djinn_beams.c: Djinni beam motes and heads. */

/* The picture of each step of a mote's eight-step cycle. */
Uint8 DjinnBeamsMotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};
/* The three pictures of the glow at a target's feet. */
Uint16 DjinnBeamsHeadPictures[] = EARTH_WALL_PICTURES;
Uint8  DjinnBeamsHeadWidths[] = EARTH_WALL_PICTURE_WIDTHS;
Uint8  DjinnBeamsHeadHeights[] = EARTH_WALL_PICTURE_HEIGHTS;

/* battle_effect_lobbed_shells.c: Shell explosion rows and shards. */

/* Each lobbed shell style (LOBBED_SHELLS_MODE, battle_effect_lobbed_shells.c):
   the LOBBED_SHELLS_STYLE_ columns. */
Sint8 LobbedShellsStyles[][2] = {
	/* shatter, unused */
	{0, 0}, /* LOBBED_SHELLS_BONE_CHILLER */
	{0, 0}, /* LOBBED_SHELLS_STICKY_GOO */
	{0, 0}, /* LOBBED_SHELLS_STICKY_POISON */
	{1, 0}, /* LOBBED_SHELLS_ROTTEN_BLOOD */
	{1, 0}, /* LOBBED_SHELLS_MAD_SPATTER */
	{0, 0}, /* LOBBED_SHELLS_DECOMPOSE */
	{0, 0}, /* LOBBED_SHELLS_POISON_INK */
};
/* Lobbed shells: nine tumbling images in two sets, the second (pictures 9 to
   17) smaller. */
Uint8 LobbedShellsShardWidths[] = {
	9, 10, 9, 9, 9, 11, 11, 11, 12, 6, 7, 7, 7, 6, 6, 8, 8, 8};
Uint8 LobbedShellsShardHeights[] = {
	12, 12, 12, 12, 12, 12, 12, 12, 12, 8, 7, 6, 6, 8, 6, 4, 3, 3};
Uint16 LobbedShellsShardPictures[] = {0, 108, 228, 336, 444, 552, 684, 816, 948,
	1092, 1140, 1189, 1231, 1273, 1321, 1357, 1389, 1413};

/* battle_effect_condemn.c: Where the reaper's pictures are drawn. */

/* Where each of the reaper's seven pictures is drawn, in pixels, by the casting
   side's column (X) and row (Y). */
Uint8 CondemnPictureX[][7] = REAPER_PICTURE_X;
Uint8 CondemnPictureY[][7] = {
	REAPER_PICTURE_Y,
	/* Unused: Condemn reads only the first row. */
	{0, 9, 4, 4, 4, 4, 4},
};

/* battle_effect_bind.c: Spinning and striking triangle vertices. */

/* The spinning triangle's first vertex, 24 up from its centre. */
VECTOR3 BindVertex[] = {{.x = 0, .y = 24, .z = 0}};
/* The striking triangle's first vertex, the same. */
VECTOR3 BindTriangleStrikeVertex[] = {{.x = 0, .y = 24, .z = 0}};

/* battle_effect_stars.c and battle_effect_procne.c: The star outlines; Procne's
   sprite grid, strike ticks and comet pictures. */

/* The two star points, 48 and 28 pixels up (16.16). */
VECTOR3 StarPoints[] = {
	{.x = 0, .y = toFIXED(48), .z = 0}, {.x = 0, .y = toFIXED(28), .z = 0}};
Uint8  StarRadii[] = {32, 16};
Uint8  ProcneGridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};
Uint8  ProcneGridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};
Uint16 ProcneTicks[] = {334, 347, 359, 372};
Uint8  ProcnePictureWidths[] = {14, 24, 48};
Uint8  ProcnePictureHeights[] = {14, 19, 24};
Uint8  ProcnePictureBiasY[] = {8, 12, 17};
Uint16 ProcnePictures[] = {13824, 14020, 14476};

/* battle_effect_tiamat.c and battle_effect_nereid.c: Tiamat's embers and
   flames; Nereid's columns of water and falling drops. */

/* Ember speeds, by ember index & 3: added to a random 0 to 127, in 128ths of a
   pixel a tick across and 32nds up. */
Uint8 TiamatEmberSpeeds[] = {0, 8, 12, 16};
/* A spark's 1x2 picture, drawn additively, so a spark adds 32 of the ramp's 63
   levels to what it covers. */
Uint8 TiamatSparkPixels[] = {32, 32};
/* Ember sizes, by ember index & 3: mote pictures 1x2, 1x2, 2x4 and 3x6. */
Uint8 TiamatEmberSizes[] = {1, 1, 2, 3};
/* The six flame pictures: size, drop below the flame and sprite sheet
   offset. */
Uint8  TiamatFlameWidths[] = FLAME_PUFF_PICTURE_WIDTHS;
Uint8  TiamatFlameHeights[] = FLAME_PUFF_PICTURE_HEIGHTS;
Uint8  TiamatFlameDrops[] = FLAME_PUFF_PICTURE_BIAS_Y;
Uint16 TiamatFlamePictures[] = FLAME_PUFF_PICTURES;
/* Nereid's four columns of water (NEREID_COLUMN_X, NEREID_COLUMN_Y), drawn from
   the fire pillar picture in the water spray's colours. */
Uint8 NereidColumns[][2] = {
	/* the foot of each column: x, y */
	{16, 104},
	{32, 104},
	{48, 104},
	{64, 104},
};
/* A drop's gravity while falling, by drop index & 3, in 16.16 pixels a tick
   each tick: 1/2, 1/4, 1/4 and 1/8. */
Sint32 NereidGravity[] = {
	toFIXED(1) / 2, toFIXED(1) / 4, toFIXED(1) / 4, toFIXED(1) / 8};

/* battle_effect_vortices.c: Vortex mote counts and tornado shapes. */

/* Froth, by variant: motes and ticks. */
Uint8 VorticesCounts[][2] = {
	/* bubbles (at most 30, battle_effect_vortices.c), ticks */
	[EFFECT_VARIANT_BASIC] = {6, 140},		/* Froth */
	[EFFECT_VARIANT_STRONGER] = {12, 160},	/* Froth Sphere */
	[EFFECT_VARIANT_STRONGEST] = {30, 200}, /* Froth Spiral */
};
/* Whirlwind, by variant: the WHIRLWIND_ columns. The sway is in pixels each
   way; the shake is btCameraShake's horizontal range, the vertical twice
   that. */
Uint8 VorticesShapes[][3] = {
	/* tornadoes, sway, screen shake */
	[EFFECT_VARIANT_BASIC] = {1, 16, 2},	 /* Whirlwind, Gale */
	[EFFECT_VARIANT_STRONGER] = {2, 24, 2},	 /* Tornado, Typhoon */
	[EFFECT_VARIANT_STRONGEST] = {4, 32, 4}, /* Tempest, Hurricane */
};

/* battle_effect_prism.c: Crystal counts and pictures. */

/* Where each of the 16 shards starts in the crystal: x, then y. */
Uint8 PrismShardStarts[][2] = {
	{32, 60},
	{33, 82},
	{49, 94},
	{41, 89},
	{49, 76},
	{32, 99},
	{54, 69},
	{34, 82},
	{45, 71},
	{31, 91},
	{32, 69},
	{26, 80},
	{40, 68},
	{41, 60},
	{46, 60},
	{36, 63},
	{0, 0},
};
/* Prism, by variant: crystals and ticks. */
Uint8 PrismCounts[][2] = {
	/* crystals, ticks */
	[EFFECT_VARIANT_BASIC] = {1, 110},		/* Prism */
	[EFFECT_VARIANT_STRONGER] = {4, 134},	/* Hail Prism */
	[EFFECT_VARIANT_STRONGEST] = {12, 166}, /* Freeze Prism */
};
/* The twelve shard pictures. */
Uint8  PrismShardWidths[] = {5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22};
Uint8  PrismShardHeights[] = {14, 10, 7, 13, 14, 14, 32, 30, 24, 38, 20, 20};
Sint32 PrismShardPictures[] = {
	0, 70, 170, 247, 429, 597, 751, 1359, 1899, 2451, 2907, 3347};

/* battle_effect_plasma.c: Lightning pillar sparks, columns and counts. */

/* Plasma, by variant: sparks per pillar and shake ticks. */
Uint16 PlasmaSparks[][2] = {
	/* sparks per pillar, shake ticks */
	[EFFECT_VARIANT_BASIC] = {384, 2},	   /* Plasma, Thunderclap */
	[EFFECT_VARIANT_STRONGER] = {256, 3},  /* Shine Plasma, Thunderbolt */
	[EFFECT_VARIANT_STRONGEST] = {192, 4}, /* Spark Plasma, Thunderstorm */
};
/* Each pillar's canvas x, seven for each side. */
Uint8 PlasmaColumns[][7] = {
	{32, 64, 16, 48, 72, 96, 64},
	{88, 56, 104, 72, 48, 24, 56},
};
/* How many pillars strike, by variant. */
Uint8 PlasmaCounts[] = {1, 3, 7};

/* battle_effect_plasma.c: Spark group shapes and flash pictures. */

/* The spark groups, by variant: the SPARK_GROUPS_ columns. */
Uint16 SparkGroupsShapes[][5] = {
	/* sparks per burst, bursts, each burst's offset across */
	[EFFECT_VARIANT_BASIC] = {128, 1, 16, 0, 0},
	[EFFECT_VARIANT_STRONGER] = {255, 1, 16, 0, 0},
	[EFFECT_VARIANT_STRONGEST] = {192, 3, 0, 24, 48},
	[SPARK_GROUPS_SEVERE_BLOW] = {340, 1, 16, 24, 48},
	[SPARK_GROUPS_THRASH] = {32, 1, 16, 0, 0},
};
/* The flash picture for each three ticks at the end of a flash's life. */
Uint8 PlasmaFlashPictures[] = FLASH_FADE_PICTURES;

/* battle_effect_volcano.c: Fire pillar counts, depths and start ticks. */

/* How many pillars stand, by variant. */
Uint8 VolcanoCounts[] = {1, 2, 4};
/* Each pillar's depth in pixels, by variant. */
Sint8 VolcanoDepths[][4] = {{0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};
/* The tick each pillar starts. At most four pillars stand, so the fifth entry
   is never read. */
Uint8 VolcanoStartTicks[] = {8, 18, 24, 30, 34};

/* battle_effect_target_motes.c: Target mote glint orientations. */

/* Glint orientations: none, and mirrored each way and both. */
Uint8 TargetMotesGlintFlips[] = GLINT_FLIPS;

/* The effect handlers, indexed by effect kind minus one, which
   btEffectRunFromTable calls; BATTLE_EFFECT_NONE is no effect, for which it
   only clears args->variant. */
BATTLE_EFFECT_HANDLER BattleEffectHandlers[BATTLE_EFFECT_COUNT - 1] = {
	/* battle_effect_quake.c: Quake */
	[BATTLE_EFFECT_QUAKE - 1] = btEffectRunQuake,
	/* battle_effect_spire.c: Spire */
	[BATTLE_EFFECT_SPIRE - 1] = btEffectRunSpire,
	[BATTLE_EFFECT_GAIA - 1] = btEffectRunGaia, /* battle_effect_gaia.c: Gaia */
	/* battle_effect_growth.c: Growth */
	[BATTLE_EFFECT_GROWTH - 1] = btEffectRunGrowth,
	/* battle_effect_growth.c: Thorn */
	[BATTLE_EFFECT_THORN - 1] = btEffectRunThorn,
	/* battle_effect_frost.c: Frost */
	[BATTLE_EFFECT_FROST - 1] = btEffectRunFrost,
	[BATTLE_EFFECT_ICE - 1] = btEffectRunIce, /* battle_effect_falling.c: Ice */
	/* battle_effect_prism.c: Prism */
	[BATTLE_EFFECT_PRISM - 1] = btEffectRunPrism,
	/* battle_effect_falling.c: Douse */
	[BATTLE_EFFECT_DOUSE - 1] = btEffectRunDouse,
	/* battle_effect_vortices.c: Froth */
	[BATTLE_EFFECT_FROTH - 1] = btEffectRunFroth,
	/* battle_effect_flare.c: Flare */
	[BATTLE_EFFECT_FLARE - 1] = btEffectRunFlare,
	[BATTLE_EFFECT_FIRE - 1] = btEffectRunFire, /* battle_effect_fire.c: Fire */
	/* battle_effect_volcano.c: Volcano */
	[BATTLE_EFFECT_VOLCANO - 1] = btEffectRunVolcano,
	/* battle_effect_eruption.c: Blast */
	[BATTLE_EFFECT_BLAST - 1] = btEffectRunBlast,
	/* battle_effect_plasma.c: Nova */
	[BATTLE_EFFECT_NOVA - 1] = btEffectRunNova,
	[BATTLE_EFFECT_BOLT - 1] = btEffectRunBolt, /* battle_effect_bolt.c: Bolt */
	[BATTLE_EFFECT_RAY - 1] = btEffectRunRay,	/* battle_effect_ray.c: Ray */
	/* battle_effect_plasma.c: Plasma */
	[BATTLE_EFFECT_PLASMA - 1] = btEffectRunPlasma,
	/* battle_effect_projectiles.c: Slash */
	[BATTLE_EFFECT_SLASH - 1] = btEffectRunSlash,
	/* battle_effect_vortices.c: Whirlwind */
	[BATTLE_EFFECT_WHIRLWIND - 1] = btEffectRunWhirlwind,
	/* battle_effect_healing.c: Cure */
	[BATTLE_EFFECT_CURE - 1] = btEffectRunCure,
	[BATTLE_EFFECT_PLY - 1] = btEffectRunPly, /* battle_effect_healing.c: Ply */
	/* battle_effect_healing.c: Wish */
	[BATTLE_EFFECT_WISH - 1] = btEffectRunWish,
	/* battle_effect_healing.c: Cure Poison */
	[BATTLE_EFFECT_CURE_POISON - 1] = btEffectRunCurePoison,
	/* battle_effect_djinn_beams.c: Revive */
	[BATTLE_EFFECT_REVIVE - 1] = btEffectRunRevive,
	/* battle_effect_stat_change.c: Impact */
	[BATTLE_EFFECT_IMPACT - 1] = btEffectRunImpact,
	/* battle_effect_stat_change.c: Dull */
	[BATTLE_EFFECT_DULL - 1] = btEffectRunDull,
	/* battle_effect_stat_change.c: Guard */
	[BATTLE_EFFECT_GUARD - 1] = btEffectRunGuard,
	/* battle_effect_stat_change.c: Impair */
	[BATTLE_EFFECT_IMPAIR - 1] = btEffectRunImpair,
	/* battle_effect_stat_change.c: Ward */
	[BATTLE_EFFECT_WARD - 1] = btEffectRunWard,
	/* battle_effect_stat_change.c: Weaken */
	[BATTLE_EFFECT_WEAKEN - 1] = btEffectRunWeaken,
	/* battle_effect_target_motes.c: Taint */
	[BATTLE_EFFECT_TAINT - 1] = btEffectRunTaint,
	/* battle_effect_target_motes.c: Delude */
	[BATTLE_EFFECT_DELUDE - 1] = btEffectRunDelude,
	/* battle_effect_planet_diver.c */
	[BATTLE_EFFECT_UNUSED_ORBITING_SPIRALS - 1] =
		btEffectRunUnusedOrbitingSpirals,
	/* battle_effect_target_motes.c: Heartrender */
	[BATTLE_EFFECT_UNUSED_HEARTRENDER - 1] = btEffectRunHeartrender,
	/* battle_effect_target_motes.c: Paralyze */
	[BATTLE_EFFECT_PARALYZE - 1] = btEffectRunParalyze,
	/* battle_effect_target_motes.c: Sleep */
	[BATTLE_EFFECT_SLEEP - 1] = btEffectRunSleep,
	[BATTLE_EFFECT_BIND - 1] = btEffectRunBind, /* battle_effect_bind.c: Bind */
	/* battle_effect_planet_diver.c: Haunt */
	[BATTLE_EFFECT_HAUNT - 1] = btEffectRunHaunt,
	/* battle_effect_condemn.c: Condemn */
	[BATTLE_EFFECT_CONDEMN - 1] = btEffectRunCondemn,
	/* battle_effect_target_motes.c: Curse */
	[BATTLE_EFFECT_CURSE - 1] = btEffectRunCurse,
	/* battle_effect_drain.c: Drain */
	[BATTLE_EFFECT_DRAIN - 1] = btEffectRunDrain,
	/* battle_effect_drain.c: Break */
	[BATTLE_EFFECT_BREAK - 1] = btEffectRunBreak,
	/* battle_effect_healing.c: Wish */
	[BATTLE_EFFECT_REGENERATE - 1] = btEffectRunWish,
	/* battle_effect_healing.c: Wish */
	[BATTLE_EFFECT_REFLECT - 1] = btEffectRunWish,
	/* battle_effect_stat_change.c: Impact */
	[BATTLE_EFFECT_UNUSED_IMPACT - 1] = btEffectRunImpact,
	/* battle_effect_stat_change.c: Impair */
	[BATTLE_EFFECT_UNUSED_IMPAIR - 1] = btEffectRunImpair,
	/* battle_effect_dragon_cloud.c: Dragon Cloud */
	[BATTLE_EFFECT_DRAGON_CLOUD - 1] = btEffectRunDragonCloud,
	/* battle_effect_gathering_lights.c: Demon Night */
	[BATTLE_EFFECT_DEMON_NIGHT - 1] = btEffectRunDemonNight,
	/* battle_effect_helm_splitter.c: Helm Splitter */
	[BATTLE_EFFECT_HELM_SPLITTER - 1] = btEffectRunHelmSplitter,
	/* battle_effect_thrash.c: Quick Strike */
	[BATTLE_EFFECT_QUICK_STRIKE - 1] = btEffectRunQuickStrike,
	/* battle_effect_death_plunge.c: Death Plunge */
	[BATTLE_EFFECT_DEATH_PLUNGE - 1] = btEffectRunDeathPlunge,
	/* battle_effect_projectiles.c: Shuriken */
	[BATTLE_EFFECT_SHURIKEN - 1] = btEffectRunShuriken,
	/* battle_effect_annihilation.c: Annihilation */
	[BATTLE_EFFECT_ANNIHILATION - 1] = btEffectRunAnnihilation,
	/* battle_effect_growth.c: Punji */
	[BATTLE_EFFECT_PUNJI - 1] = btEffectRunPunji,
	/* battle_effect_projectiles.c: Cutting Edge */
	[BATTLE_EFFECT_CUTTING_EDGE - 1] = btEffectRunCuttingEdge,
	/* battle_effect_ragnarok.c: Ragnarok */
	[BATTLE_EFFECT_RAGNAROK - 1] = btEffectRunRagnarok,
	/* battle_effect_projectiles.c: Heat Wave */
	[BATTLE_EFFECT_HEAT_WAVE - 1] = btEffectRunHeatWave,
	/* battle_effect_planet_diver.c: Planet Diver */
	[BATTLE_EFFECT_PLANET_DIVER - 1] = btEffectRunPlanetDiver,
	/* battle_effect_titan_blade.c: Titan Blade */
	[BATTLE_EFFECT_TITAN_BLADE - 1] = btEffectRunTitanBlade,
	/* battle_effect_stars.c: Shining Star */
	[BATTLE_EFFECT_SHINING_STAR - 1] = btEffectRunShiningStar,
	/* battle_effect_projectiles.c: Blizzard */
	[BATTLE_EFFECT_BLIZZARD - 1] = btEffectRunBlizzard,
	/* battle_effect_target_motes.c: Demon Fire */
	[BATTLE_EFFECT_DEMON_FIRE - 1] = btEffectRunDemonFire,
	/* battle_effect_thrash.c: Vorpal Slash */
	[BATTLE_EFFECT_VORPAL_SLASH - 1] = btEffectRunVorpalSlash,
	/* battle_effect_target_motes.c: Drown */
	[BATTLE_EFFECT_DROWN - 1] = btEffectRunDrown,
	/* battle_effect_bind.c */
	[BATTLE_EFFECT_UNUSED_TRIANGLE_STRIKE - 1] =
		btEffectRunUnusedTriangleStrike,
	/* battle_effect_stars.c: Astral Blast */
	[BATTLE_EFFECT_ASTRAL_BLAST - 1] = btEffectRunAstralBlast,
	/* battle_effect_stat_change.c: Granite */
	[BATTLE_EFFECT_GRANITE - 1] = btEffectRunGranite,
	/* battle_effect_djinn_beams.c: Revive */
	[BATTLE_EFFECT_QUARTZ - 1] = btEffectRunRevive,
	/* battle_effect_growth.c: Vine */
	[BATTLE_EFFECT_VINE - 1] = btEffectRunVine,
	/* battle_effect_hail.c: Ground */
	[BATTLE_EFFECT_GROUND - 1] = btEffectRunGround,
	/* battle_effect_healing.c: Ply */
	[BATTLE_EFFECT_FIZZ - 1] = btEffectRunPly,
	/* battle_effect_projectiles.c: Sleet */
	[BATTLE_EFFECT_SLEET - 1] = btEffectRunSleet,
	/* battle_effect_healing.c: Wish */
	[BATTLE_EFFECT_SPRITZ - 1] = btEffectRunWish,
	[BATTLE_EFFECT_HAIL - 1] = btEffectRunHail, /* battle_effect_hail.c: Hail */
	/* battle_effect_healing.c: Tonic */
	[BATTLE_EFFECT_TONIC - 1] = btEffectRunTonic,
	/* battle_effect_djinn_beams.c: Dew */
	[BATTLE_EFFECT_DEW - 1] = btEffectRunDew,
	/* battle_effect_stat_change.c: Impact */
	[BATTLE_EFFECT_FORGE - 1] = btEffectRunImpact,
	/* battle_effect_stat_change.c: Guard */
	[BATTLE_EFFECT_CORONA - 1] = btEffectRunGuard,
	/* battle_effect_healing.c: Psy Crystal */
	[BATTLE_EFFECT_PSY_CRYSTAL - 1] = btEffectRunPsyCrystal,
	/* battle_effect_stat_change.c: Flash */
	[BATTLE_EFFECT_FLASH - 1] = btEffectRunFlash,
	/* battle_effect_torch.c: Torch */
	[BATTLE_EFFECT_TORCH - 1] = btEffectRunTorch,
	/* battle_effect_stat_change.c: Ward */
	[BATTLE_EFFECT_BREEZE - 1] = btEffectRunWard,
	/* battle_effect_stat_change.c: Zephyr */
	[BATTLE_EFFECT_ZEPHYR - 1] = btEffectRunZephyr,
	/* battle_effect_torch.c: Kite */
	[BATTLE_EFFECT_KITE - 1] = btEffectRunKite,
	[BATTLE_EFFECT_LUFF - 1] = btEffectRunBind, /* battle_effect_bind.c: Bind */
	/* battle_effect_flurry.c: Gust */
	[BATTLE_EFFECT_GUST - 1] = btEffectRunGust,
	/* battle_effect_djinn_attacks.c: Venus */
	[BATTLE_EFFECT_VENUS - 1] = btEffectRunVenus,
	/* battle_effect_djinn_attacks.c: Mercury */
	[BATTLE_EFFECT_MERCURY - 1] = btEffectRunMercury,
	/* battle_effect_djinn_attacks.c: Mars */
	[BATTLE_EFFECT_MARS - 1] = btEffectRunMars,
	/* battle_effect_djinn_attacks.c: Jupiter */
	[BATTLE_EFFECT_JUPITER - 1] = btEffectRunJupiter,
	/* battle_effect_streaks.c: Sonic Wave */
	[BATTLE_EFFECT_SONIC_WAVE - 1] = btEffectRunSonicWave,
	/* battle_effect_streaks.c: Shriek */
	[BATTLE_EFFECT_SHRIEK - 1] = btEffectRunShriek,
	/* battle_effect_streaks.c: Banshee Howl */
	[BATTLE_EFFECT_BANSHEE_HOWL - 1] = btEffectRunBansheeHowl,
	/* battle_effect_projectiles.c: Crazy Voice */
	[BATTLE_EFFECT_CRAZY_VOICE - 1] = btEffectRunCrazyVoice,
	/* battle_effect_streaks.c: War Cry */
	[BATTLE_EFFECT_WAR_CRY - 1] = btEffectRunWarCry,
	/* battle_effect_streaks.c: Wicked Howl */
	[BATTLE_EFFECT_WICKED_HOWL - 1] = btEffectRunWickedHowl,
	/* battle_effect_projectiles.c: Wing Beat */
	[BATTLE_EFFECT_WING_BEAT - 1] = btEffectRunWingBeat,
	/* battle_effect_projectiles.c: Wing Flutter */
	[BATTLE_EFFECT_WING_FLUTTER - 1] = btEffectRunWingFlutter,
	/* battle_effect_gathering_lights.c: Rumble */
	[BATTLE_EFFECT_RUMBLE - 1] = btEffectRunRumble,
	/* battle_effect_lobbed_shells.c: Bone Chiller */
	[BATTLE_EFFECT_BONE_CHILLER - 1] = btEffectRunBoneChiller,
	/* battle_effect_flurry.c: Gust */
	[BATTLE_EFFECT_RAPID_SMASH - 1] = btEffectRunGust,
	/* battle_effect_rushing_strike.c: Bone Charge */
	[BATTLE_EFFECT_BONE_CHARGE - 1] = btEffectRunBoneCharge,
	/* battle_effect_target_motes.c: Mystic Flame */
	[BATTLE_EFFECT_MYSTIC_FLAME - 1] = btEffectRunMysticFlame,
	/* battle_effect_target_strike.c: Numbing Sting */
	[BATTLE_EFFECT_NUMBING_STING - 1] = btEffectRunNumbingSting,
	/* battle_effect_rushing_strike.c: Brute Force */
	[BATTLE_EFFECT_BRUTE_FORCE - 1] = btEffectRunBruteForce,
	/* battle_effect_lobbed_shells.c: Sticky Goo */
	[BATTLE_EFFECT_STICKY_GOO - 1] = btEffectRunStickyGoo,
	/* battle_effect_target_strike.c: Cannibal Fang */
	[BATTLE_EFFECT_CANNIBAL_FANG - 1] = btEffectRunCannibalFang,
	/* battle_effect_flurry.c: Bear Claw */
	[BATTLE_EFFECT_BEAR_CLAW - 1] = btEffectRunBearClaw,
	/* battle_effect_target_strike.c: Poisonous Bite */
	[BATTLE_EFFECT_POISONOUS_BITE - 1] = btEffectRunPoisonousBite,
	/* battle_effect_flurry.c: Flying Attack */
	[BATTLE_EFFECT_FLYING_ATTACK - 1] = btEffectRunFlyingAttack,
	/* battle_effect_undead_sword.c: Undead Sword */
	[BATTLE_EFFECT_UNDEAD_SWORD - 1] = btEffectRunUndeadSword,
	/* battle_effect_flurry.c: Ransack */
	[BATTLE_EFFECT_RANSACK - 1] = btEffectRunRansack,
	/* battle_effect_lobbed_shells.c: Sticky Poison */
	[BATTLE_EFFECT_STICKY_POISON - 1] = btEffectRunStickyPoison,
	/* battle_effect_target_strike.c: Poisonous Bite */
	[BATTLE_EFFECT_POISON_FANG - 1] = btEffectRunPoisonousBite,
	/* battle_effect_target_strike.c: Electric Bite */
	[BATTLE_EFFECT_ELECTRIC_BITE - 1] = btEffectRunElectricBite,
	/* battle_effect_target_strike.c: Poison Tail */
	[BATTLE_EFFECT_POISON_TAIL - 1] = btEffectRunPoisonTail,
	/* battle_effect_rushing_strike.c: Onslaught */
	[BATTLE_EFFECT_ONSLAUGHT - 1] = btEffectRunOnslaught,
	/* battle_effect_target_strike.c: Vampiric Fang */
	[BATTLE_EFFECT_VAMPIRIC_FANG - 1] = btEffectRunVampiricFang,
	/* battle_effect_flurry.c: Bacteria Rush */
	[BATTLE_EFFECT_BACTERIA_RUSH - 1] = btEffectRunBacteriaRush,
	/* battle_effect_flurry.c: Flying Attack */
	[BATTLE_EFFECT_SWIFT_STRIKE - 1] = btEffectRunFlyingAttack,
	/* battle_effect_lobbed_shells.c: Rotten Blood */
	[BATTLE_EFFECT_ROTTEN_BLOOD - 1] = btEffectRunRottenBlood,
	/* battle_effect_target_strike.c: Double Fang */
	[BATTLE_EFFECT_DOUBLE_FANG - 1] = btEffectRunDoubleFang,
	/* battle_effect_target_strike.c: Mortal Blow */
	[BATTLE_EFFECT_MORTAL_BLOW - 1] = btEffectRunMortalBlow,
	/* battle_effect_flurry.c: Freebite Rush */
	[BATTLE_EFFECT_FREEBITE_RUSH - 1] = btEffectRunFreebiteRush,
	/* battle_effect_target_strike.c: Twin Beaks */
	[BATTLE_EFFECT_TWIN_BEAKS - 1] = btEffectRunTwinBeaks,
	/* battle_effect_target_strike.c: Rabid Fang */
	[BATTLE_EFFECT_RABID_FANG - 1] = btEffectRunRabidFang,
	/* battle_effect_target_strike.c: Acid Bite */
	[BATTLE_EFFECT_ACID_BITE - 1] = btEffectRunAcidBite,
	/* battle_effect_plasma.c: Dynamite */
	[BATTLE_EFFECT_DYNAMITE - 1] = btEffectRunDynamite,
	/* battle_effect_rushing_strike.c: Headbutt */
	[BATTLE_EFFECT_HEADBUTT - 1] = btEffectRunHeadbutt,
	/* battle_effect_lobbed_shells.c: Poison Ink */
	[BATTLE_EFFECT_POISON_INK - 1] = btEffectRunPoisonInk,
	/* battle_effect_flurry.c: Bear Claw */
	[BATTLE_EFFECT_COUNTERSTRIKE - 1] = btEffectRunBearClaw,
	/* battle_effect_flurry.c: Ransack */
	[BATTLE_EFFECT_MAD_DASH - 1] = btEffectRunRansack,
	/* battle_effect_streaks.c: Soothing Star */
	[BATTLE_EFFECT_SOOTHING_STAR - 1] = btEffectRunSoothingStar,
	/* battle_effect_spider_web.c: Spider Web */
	[BATTLE_EFFECT_SPIDER_WEB - 1] = btEffectRunSpiderWeb,
	/* battle_effect_target_motes.c: Heartrender */
	[BATTLE_EFFECT_HEARTRENDER - 1] = btEffectRunHeartrender,
	/* battle_effect_lobbed_shells.c: Mad Spatter */
	[BATTLE_EFFECT_MAD_SPATTER - 1] = btEffectRunMadSpatter,
	/* battle_effect_mirrored_wall.c: Spasm */
	[BATTLE_EFFECT_SPASM - 1] = btEffectRunSpasm,
	/* battle_effect_streaks.c: Sleep Star */
	[BATTLE_EFFECT_SLEEP_STAR - 1] = btEffectRunSleepStar,
	/* battle_effect_lobbed_shells.c: Decompose */
	[BATTLE_EFFECT_DECOMPOSE - 1] = btEffectRunDecompose,
	/* battle_effect_gathering_lights.c: Haunting */
	[BATTLE_EFFECT_HAUNTING - 1] = btEffectRunHaunting,
	/* battle_effect_healing.c: Cure */
	[BATTLE_EFFECT_WORMS - 1] = btEffectRunCure,
	/* battle_effect_mirrored_wall.c: Berserk */
	[BATTLE_EFFECT_BERSERK - 1] = btEffectRunBerserk,
	/* battle_effect_target_motes.c: Curse */
	[BATTLE_EFFECT_LUCID_PROPHECY - 1] = btEffectRunCurse,
	/* battle_effect_eruption.c: Smoke Bomb */
	[BATTLE_EFFECT_SMOKE_BOMB - 1] = btEffectRunSmokeBomb,
	/* battle_effect_projectiles.c: Wing Stroke */
	[BATTLE_EFFECT_WING_STROKE - 1] = btEffectRunWingStroke,
	/* battle_effect_spider_web.c: Poison Beat */
	[BATTLE_EFFECT_POISON_BEAT - 1] = btEffectRunPoisonBeat,
	/* battle_effect_spider_web.c: Spinning Beat */
	[BATTLE_EFFECT_SPINNING_BEAT - 1] = btEffectRunSpinningBeat,
	/* battle_effect_starfield_fall.c: Outer Space */
	[BATTLE_EFFECT_OUTER_SPACE - 1] = btEffectRunOuterSpace,
	/* battle_effect_tiamat.c: Dragon Driver */
	[BATTLE_EFFECT_DRAGON_DRIVER - 1] = btEffectRunDragonDriver,
	/* battle_effect_target_strike.c: Drain Fang */
	[BATTLE_EFFECT_DRAIN_FANG - 1] = btEffectRunDrainFang,
	/* battle_effect_thrash.c */
	[BATTLE_EFFECT_UNUSED_SEVERE_BLOW_LEAP - 1] =
		btEffectRunUnusedSevereBlowLeap,
	/* battle_effect_thrash.c: Thrash */
	[BATTLE_EFFECT_THRASH - 1] = btEffectRunThrash,
	/* battle_effect_mirrored_wall.c: Recovery */
	[BATTLE_EFFECT_RECOVERY - 1] = btEffectRunRecovery,
	/* battle_effect_burst_shower.c: Fire Blessing */
	[BATTLE_EFFECT_FIRE_BLESSING - 1] = btEffectRunFireBlessing,
	/* battle_effect_burst_shower.c: Water Blessing */
	[BATTLE_EFFECT_WATER_BLESSING - 1] = btEffectRunWaterBlessing,
	/* battle_effect_projectiles.c: Ice Blessing */
	[BATTLE_EFFECT_ICE_BLESSING - 1] = btEffectRunIceBlessing,
	/* battle_effect_burst_shower.c: Dark Blessing */
	[BATTLE_EFFECT_DARK_BLESSING - 1] = btEffectRunDarkBlessing,
	/* battle_effect_burst_shower.c: Acid Blessing */
	[BATTLE_EFFECT_ACID_BLESSING - 1] = btEffectRunAcidBlessing,
	/* battle_effect_burst_shower.c: Storm Blessing */
	[BATTLE_EFFECT_STORM_BLESSING - 1] = btEffectRunStormBlessing,
	/* battle_effect_burst_shower.c: Evil Blessing */
	[BATTLE_EFFECT_EVIL_BLESSING - 1] = btEffectRunEvilBlessing,
	/* battle_effect_burst_shower.c: Deadly Gas */
	[BATTLE_EFFECT_DEADLY_GAS - 1] = btEffectRunDeadlyGas,
};

/* battle_effect_boreas.c, battle_effect_cybele.c and battle_effect_drain.c:
   Rock and ice effect geometry. */

/* The poses of the two halves of Cybele's face, three pairs, by
   CYBELE_POSE_FACE_LEFT and _RIGHT. */
Uint8 CybeleObjectPoses[][2] = {
	{8, 10},
	{9, 11},
	{3, 4},
};
/* The five thorn pictures, which the chips use too. */
Uint16 CybeleThornPictures[] = {3920, 3932, 3947, 3955, 3970};
Uint8  CybeleThornWidths[] = {2, 5, 2, 5, 3};
Uint8  CybeleThornHeights[] = {6, 3, 4, 3, 3};
/* The three flower pictures, square. */
Uint16 CybeleFlowerPictures[] = {3456, 3712, 3856};
Uint8  CybeleFlowerSizes[] = {16, 12, 8};
/* The five chip pictures, the thorn pictures again. */
Uint16 CybeleChipPictures[] = {3920, 3932, 3947, 3955, 3970};
Uint8  CybeleChipWidths[] = {2, 5, 2, 5, 3};
Uint8  CybeleChipHeights[] = {6, 3, 4, 3, 3};
/* The five pictures of a spark, square. */
Uint16 CybeleSparkPictures[] = {3979, 4043, 4268, 4389, 4470};
Uint8  CybeleSparkSizes[] = {8, 15, 11, 9, 5};
/* The seven growing pictures of the blast sprite sheet, square. Cybele draws
   only the last, largest one, as a dust cloud. */
Uint16 CybeleBlastPictures[] = {5760, 6016, 7040, 9344, 11648, 13952, 16256};
Uint16 CybeleBlastSizes[] = {16, 32, 48, 48, 48, 48, 48};
/* Boreas: where the crystals stand (and in the second half the rising crystals,
   56 pixels further left), by the bottom-left corner. */
Uint8 BoreasPoints[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
	{86, 126}, {85, 98}, {64, 98}, {98, 96}, {77, 87}, {84, 88}, {68, 67},
	{81, 49}, {94, 70}, {70, 51}, {93, 36}, {81, 50}};
/* The battle background's jolt on ticks 176 to 179. */
Sint8 BoreasShakeOffsets[] = {-4, -3, -2, 0};
/* The first half's three crystal pictures, drawn standing on their points. */
Uint16 BoreasCrystalPictures[] = {0, 1152, 2064};
Uint8  BoreasCrystalWidths[] = {24, 24, 20};
Uint8  BoreasCrystalHeights[] = {48, 38, 26};
/* The third sprite's animations from tick 209, one every 4 ticks. */
Uint8 BoreasAnimations[] = {2, 9, 10, 11};
/* Boreas: the shards thrown up as the crystals crumble: four large ones, then
   from tick 192 seven small ones. */
Uint8  BoreasDebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};
Uint8  BoreasDebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};
Uint16 BoreasDebrisPictures[] = {
	2584, 2704, 2880, 2992, 5062, 5094, 5142, 5157, 5185, 5227, 5299};
/* Boreas: the three 16x16 spray pictures. */
Uint8  BoreasSprayWidths[] = {16, 16, 16};
Uint8  BoreasSprayHeights[] = {16, 16, 16};
Uint16 BoreasSprayPictures[] = {4294, 4550, 4806};
/* The second half's rising crystals (56 pixels left of the first half's points)
   and the crystals they burst into: the same three pictures. */
Uint16 BoreasRisingCrystalPictures[] = {0, 1152, 2064};
Uint8  BoreasRisingCrystalWidths[] = {24, 24, 20};
Uint8  BoreasRisingCrystalHeights[] = {48, 38, 26};
Uint8  BoreasFallWidths[] = {24, 24, 20};
Uint8  BoreasFallHeights[] = {48, 38, 26};
Uint16 BoreasFallPictures[] = {0, 1152, 2064};
/* Ice shards: shard i falls with gravity i & 3, in 16.16 pixels a tick each
   tick: 1/4, 1/8, 1/4 and 1/2. */
Sint32 DrainGravities[] = {
	toFIXED(1) / 4, toFIXED(1) / 8, toFIXED(1) / 4, toFIXED(1) / 2};

/* battle_effect_stat_change.c: Stat-reduction shard geometry. */

/* The twelve shard pictures, three ticks for each of four shapes. */
Uint16 StatChangeShardPictures[] = {
	0, 120, 180, 258, 298, 352, 408, 471, 567, 607, 631, 661};
Uint8 StatChangeShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};
Uint8 StatChangeShardHeights[] = {12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5};

/* battle_effect_quake.c */

/* Rock wall timing, by variant: ticks in the effect, ticks each pillar
   stands, and how far the screen shakes. */
Uint8 QuakeTimings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

/* battle_effect_fire.c: Homing ember counts and flare geometry. */

/* Embers by variant, at most FIRE_EMBERS (battle_effect_fire.c). */
Uint8 FireCounts[] = {16, 32, 64};
/* The six impact flare pictures: sizes, y offsets and where each begins. */
Uint8  FireFlareWidths[] = FLAME_PUFF_PICTURE_WIDTHS;
Uint8  FireFlareHeights[] = FLAME_PUFF_PICTURE_HEIGHTS;
Uint8  FireFlareBiasY[] = FLAME_PUFF_PICTURE_BIAS_Y;
Uint16 FireFlarePictures[] = FLAME_PUFF_PICTURES;

/* battle_effect_frost.c: Frost's motes, ice mounds and chips. */

/* Where each ice mound stands: x, then the y of its foot. */
Uint8 FrostMoundSpots[][2] = {
	{53, 108},
	{73, 110},
	{29, 110},
	{73, 108},
	{33, 104},
	{96, 106},
	{8, 109},
	{62, 98},
	{33, 100},
	{87, 100},
	{21, 95},
	{53, 90},
	{33, 90},
	{72, 91},
	{82, 91},
	{77, 86},
	{52, 84},
	{18, 92},
	{56, 81},
};
/* Motes, ice mounds and the burst tick, for each variant. */
Uint8 FrostTimings[][3] = {
	{32, 8, 85},
	{48, 12, 97},
	{64, 19, 109},
};
/* The eight mote pictures. */
Uint8  FrostMoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
Uint8  FrostMoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
Uint16 FrostMotePictures[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};
/* The three ice mound pictures, 32 wide. */
Uint16 FrostMoundPictures[] = {0, 768, 1280};
Uint8  FrostMoundHeights[] = {24, 16, 16};
/* The eight chip pictures (the same as the motes). */
Uint8  FrostChipWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
Uint8  FrostChipHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
Uint16 FrostChipPictures[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

/* battle_effect_ray.c: Closing bolt sparks by variant. */

/* Sparks by variant; [1] is how many each bolt throws a tick. */
Uint8 RaySparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

/* battle_effect_eruption.c: Eruption counts and puff layouts. */

/* Motes, puffs of smoke and ticks, for each variant. */
Uint8 EruptionCounts[][3] = {{16, 8, 72}, {32, 16, 80}, {128, 32, 88}};
/* The seven puff pictures, square. */
Uint16 EruptionPuffPictures[] = BLAST_PICTURES;
Uint16 EruptionPuffSizes[] = BLAST_PICTURE_SIZES;

/* Unreferenced picture layouts, from 6x6 through 20x20.
   UnusedSheetPicturesShort stops at the 16x16 picture's offset, the first six
   of UnusedSheetPictures. */
Uint16 UnusedSheetPicturesShort[] = {0, 36, 100, 200, 344, 540};
Uint16 UnusedSheetPictures[] = {
	0, 36, 100, 200, 344, 540, 796, 1120, 1520, 1900, 2300};
Uint8 UnusedSheetWidths[] = {6, 8, 10, 12, 14, 16, 18, 20, 19, 20};
Uint8 UnusedSheetHeights[] = {6, 8, 10, 12, 14, 16, 18, 20, 20, 20};

/* battle_effect_atalanta.c */

/* The scale Atalanta's formation sprites keep throughout, one to one. */
SPR_SCALE AtalantaUnitScale = {{{toFIXED(1), toFIXED(1)}}};

/* battle_effect_growth.c: Growing vine and pillar geometry. */

/* The three column shapes: vine and pillar widths, pictures and height caps. */
Uint8  GrowthVineWidths[] = {21, 28, 19};
Uint8  GrowthHeightLimits[] = {119, 119, 102};
Uint16 GrowthVinePictures[] = {0, 2499, 5831};
Uint8  GrowthPillarWidths[] = {10, 8, 9};
Uint16 GrowthPillarPictures[] = {0, 1080, 1888};
/* Columns by variant, and where each column's root stands. */
Uint8 GrowthColumnCounts[] = {4, 8, 12};
Uint8 GrowthColumnX[] = {
	64, 72, 56, 80, 48, 88, 40, 96, 32, 104, 24, 72, 56, 80, 48, 88};
Sint8 GrowthPillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};
/* The seven pictures of a puff of dust. */
Sint8  GrowthPuffWidths[] = {5, 7, 11, 16, 23, 27, 16};
Uint8  GrowthPuffHeights[] = {10, 14, 21, 32, 45, 50, 32};
Uint16 GrowthPuffPictures[] = {0, 50, 148, 379, 891, 1926, 3276};
/* Each vine's root, across from the middle of the targets. */
Sint8 VineColumnOffsets[] = {
	0, 14, -32, 8, 32, -48, 4, 12, -24, 24, -12, -40, -4, -16, -32, 12};
/* Each thorn's kind (THORN_ flags) and root. */
Uint8 ThornKinds[] = {0, THORN_SMALL | THORN_MIRRORED, THORN_SMALL,
	THORN_MIRRORED, THORN_SMALL, THORN_SMALL | THORN_MIRRORED, THORN_SMALL,
	THORN_MIRRORED};
Sint8 Thorn_X[] = {24, 80, 24, 48, 88, 40, 8, 96};
/* Thorns by variant. */
Uint8 ThornCounts[] = {3, 5, 8};
/* The seven pictures of a puff of dust. */
Sint8  ThornPuffWidths[] = {5, 7, 11, 17, 32, 32, 19};
Uint8  ThornPuffHeights[] = {10, 14, 21, 34, 59, 60, 38};
Uint16 ThornPuffPictures[] = {5888, 5938, 6036, 6267, 6845, 8733, 10653};

/* battle_effect_bolt.c: Lightning bolt counts and glint palettes. */

/* Bolt, by variant: sparks each bolt throws, glints a tick around each target,
   how far the glints spread, and bolts drawn over each other. */
Uint8 BoltCounts[][4] = {
	[EFFECT_VARIANT_BASIC] = {16, 0, 15, 1},	  /* Bolt */
	[EFFECT_VARIANT_STRONGER] = {32, 3, 31, 1},	  /* Flash Bolt */
	[EFFECT_VARIANT_STRONGEST] = {128, 5, 48, 3}, /* Blue Bolt */
};
/* The flips a glint may be drawn in, and its blend by variant. */
Uint8 BoltGlintFlips[] = GLINT_FLIPS;
/* The glints' blend mode by variant: the brighter pixel wins for Bolt and Flash
   Bolt, and Blue Bolt adds the glint to the canvas. */
Uint8 BoltGlintModes[] = {
	COMPOSITE_LIGHTEN, COMPOSITE_LIGHTEN, COMPOSITE_ADDITIVE};

/* battle_effect_projectiles.c: Projectile volley modes and pictures. */

/* Comet picture heights, by comet (PROJECTILE_VOLLEY_COMET). */
Uint8 ProjectileVolleyCometHeights[] = {24, 36, 48};
/* Each projectile volley mode, by the PROJECTILE_VOLLEY_ columns
   (battle_effect_projectiles.c). */
Uint8 ProjectileVolleyModes[][5] = {
	/* PROJECTILE_VOLLEY_: SHOTS, STEPS, STAGGER, DELAY, SPARKS */
	[PROJECTILE_VOLLEY_SLASH] = {6, 10, 3, 32, 4},
	[PROJECTILE_VOLLEY_SHURIKEN] = {4, 10, 5, 32, 8},
	[PROJECTILE_VOLLEY_CRAZY_VOICE] = {10, 32, 3, 32, 4},
	[PROJECTILE_VOLLEY_WING_FLUTTER] = {5, 10, 5, 32, 4},
	[PROJECTILE_VOLLEY_WING_BEAT] = {10, 10, 1, 32, 4},
	[PROJECTILE_VOLLEY_CUTTING_EDGE] = {1, 5, 0, 32, 80},
	[PROJECTILE_VOLLEY_HEAT_WAVE] = {7, 8, 2, 32, 40},
	[PROJECTILE_VOLLEY_SLEET] = {30, 24, 1, 32, 4},
	[PROJECTILE_VOLLEY_IMPACT_COMETS] = {6, 2, 3, 32, 4},
	[PROJECTILE_VOLLEY_BLIZZARD] = {60, 10, 1, 32, 4},
	[PROJECTILE_VOLLEY_ICE_BLESSING] = {60, 10, 1, 32, 4},
	[PROJECTILE_VOLLEY_WING_STROKE] = {10, 10, 1, 32, 4},
};
/* Hit flash pictures, by step (flash tick / 2). */
Uint8  ProjectileVolleyFlashWidths[] = {12, 18, 24, 30, 36, 39, 40};
Uint8  ProjectileVolleyFlashHeights[] = {10, 15, 18, 22, 27, 28, 30};
Uint8  ProjectileVolleyFlashSteps[] = {2, 3, 4, 4, 5, 6, 6};
Uint8  ProjectileVolleyFlashRises[] = {0, 3, 5, 7, 10, 12, 12};
Uint16 ProjectileVolleyFlashPictures[] = {
	8192, 8312, 8582, 9014, 9674, 10646, 11738};
/* Ice chip pictures, by shot index & 3. */
Uint16 ProjectileVolleyPelletPictures[] = {2308, 2372, 2436, 2500};

/* battle_effect_djinn_attacks.c: Mercury Djinni flame launch ticks. */

/* The ticks on which the Djinni fires each of its five flames. */
Uint8 DjinnAttacksLaunchTicks[] = {4, 7, 10, 21, 34};

/* battle_effect_hail.c: Hail's ice chips; Ground's portal and crackles. */

/* The four ice chip pictures. */
Uint8  HailPictureWidths[] = {8, 8, 8, 8};
Uint8  HailPictureHeights[] = {8, 8, 8, 8};
Uint16 HailPictures[] = {2308, 2372, 2436, 2500};
/* The blitter flags a crackle may take, chosen at random: clipped at the canvas
   edges, and drawn as is, mirrored across, mirrored down or both. */
Uint8 GroundCrackleFlips[] = {EFFECT_CLIP_BOUNDS,
	EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
	EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP};
/* The portal (picture 0), its white core (1 and 2) and its crackles (3 to 8) in
   the portal sprite sheet: size, where each begins in the sprite sheet, and its
   offset from the portal's corner. */
Uint8  GroundPortalWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};
Uint8  GroundPortalHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};
Uint16 GroundPortalPictures[] = {
	0, 1152, 1243, 1341, 1533, 1677, 1898, 2098, 2252};
Uint8 GroundPortalX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};
Uint8 GroundPortalY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

/* battle_effect_spire.c and battle_effect_thor.c: Storm effect colours and
   shattering rocks. */

/* The colours a target flashes when a bolt strikes it, chosen at random. */
Uint8 ThorTargetPalettes[] = {SPRITE_COLOR_WHITE, SPRITE_COLOR_BLACK,
	SPRITE_COLOR_RED_TINT, SPRITE_COLOR_GBR};
/* Flip flags for the crackles of lightning around Thor, chosen at random. */
Uint8 ThorCrackleFlips[] = GLINT_FLIPS;
/* Where each of a rock's 21 shards starts, x then y, before the rock's offset
   across. */
Uint8 SpireShardOffsets[32][2] = {
	{65, 104},
	{68, 95},
	{64, 93},
	{61, 93},
	{64, 87},
	{72, 85},
	{56, 88},
	{51, 80},
	{61, 75},
	{72, 81},
	{69, 71},
	{66, 70},
	{71, 64},
	{49, 71},
	{59, 69},
	{53, 68},
	{78, 73},
	{55, 67},
	{75, 62},
	{61, 65},
	{53, 61},
};
/* Each rock's offset across, the tick it appears (it lands 18 ticks later),
   and the rocks by variant. */
Sint8 SpireRockX[] = {0, -12, 12, -8, 16};
Uint8 SpireDropTicks[] = {8, 16, 20, 24, 32};
Uint8 SpireRockCounts[] = {1, 3, 5};
/* Shattering rocks: five shard shapes in three tumbling phases each, decoded
   after the boulder picture. */
Uint8 SpireShardWidths[] = {
	5, 11, 10, 7, 7, 12, 13, 9, 19, 12, 17, 22, 15, 16, 14};
Uint8 SpireShardHeights[] = {
	11, 6, 10, 11, 10, 7, 17, 21, 12, 21, 17, 13, 16, 16, 15};
Uint16 SpireShardPictures[] = {0, 55, 121, 221, 298, 368, 452, 673, 862, 1090,
	1342, 1631, 1917, 2157, 2413, 2623};

/* battle_effect_gaia.c and battle_effect_flurry.c: flurry rows and rising wall
   geometry. */

/* Each flurry mode (FLURRY_MODE), by the FLURRY_ columns
   (battle_effect_flurry.c). */
/* clang-format off */
Uint8 FlurryStyles[][7] = {
	/* FLURRY_: ARC_SHEET, PALETTE, STRIKES, MOTES, INTERVAL, TICKS, */
	/* FLASH */
	[FLURRY_BEAR_CLAW] =
		{FLURRY_ARCS, FLURRY_PALETTE_MARS_DJINNI, 5, 16, 12, 80, 0},
	[FLURRY_BACTERIA_RUSH] =
		{FLURRY_ARCS, FLURRY_PALETTE_LIME, 7, 16, 12, 104, 0},
	[FLURRY_FREEBITE_RUSH] =
		{FLURRY_ARCS, FLURRY_PALETTE_BLUE_ARC, 9, 16, 12, 128, 0},
	[FLURRY_FLYING_ATTACK] =
		{FLURRY_ARCS, FLURRY_PALETTE_EMBER, 1, 16, 16, 36, 0},
	[FLURRY_FLYING_ATTACK_DOUBLE] =
		{FLURRY_ARCS, FLURRY_PALETTE_EMBER, 2, 20, 16, 52, 0},
	[FLURRY_FLYING_ATTACK_TRIPLE] =
		{FLURRY_ARCS, FLURRY_PALETTE_EMBER, 3, 24, 16, 68, 0},
	[FLURRY_GUST] =
		{FLURRY_ARCS_AND_WAVES, FLURRY_PALETTE_BLUE_ARC, 1, 32, 12, 28, 1},
	[FLURRY_GUST_DOUBLE] =
		{FLURRY_ARCS_AND_WAVES, FLURRY_PALETTE_BLUE_ARC, 2, 32, 12, 40, 1},
	[FLURRY_GUST_TRIPLE] =
		{FLURRY_ARCS_AND_WAVES, FLURRY_PALETTE_BLUE_ARC, 3, 32, 12, 52, 1},
};
/* clang-format on */
/* Rising wall: the nine rows of the growing wall in the rebuilt sprite
   sheet. */
Uint16 GaiaRowPictures[] = {0, 160, 352, 640, 1024, 1504, 2080, 2752, 3776};
Uint8  GaiaRowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};
/* The three pictures of the flames at the wall's foot, after the rebuilt rows;
   they share the earth wall's sizes. */
Uint16 GaiaFlamePictures[] = {10672, 11505, 12550};
Uint8  GaiaFlameWidths[] = EARTH_WALL_PICTURE_WIDTHS;
Uint8  GaiaFlameHeights[] = EARTH_WALL_PICTURE_HEIGHTS;
/* BG2's x shift, by side and variant, in canvas pixels written to BG2X: a
   positive shift moves the canvas left on the screen. */
Sint8 GaiaBg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};
/* The columns rock particles[16] to particles[21] take. */
Uint8 GaiaLateRockColumns[] = {136, 40, 120, 56, 104, 72};

/* battle_effect_beams.c: Beam pictures and reach. */

/* The six pictures of the growing beam, and how far each reaches short of the
   user. The Lost Age reads the same tables. */
Uint8  BeamPictureWidths[] = SLASH_PICTURE_WIDTHS;
Uint8  BeamPictureHeights[] = SLASH_PICTURE_HEIGHTS;
Uint16 BeamPictures[] = {0, 266, 752, 1533, 2495, 3496};
Uint8  BeamPictureReaches[] = SLASH_PICTURE_REACH;

/* battle_effect_casting_impact.c: Casting impact glints and orbit pictures. */

/* The flips a glint may take, picked at random. */
Uint8 CastingImpactGlintDrawFlags[] = GLINT_FLIPS;
Uint8 ReaperPictureX[][7] = REAPER_PICTURE_X;
Uint8 ReaperPictureY[] = REAPER_PICTURE_Y;
/* The four ice chip pictures, 8x8. */
Uint16 CastingImpactIceChipPictures[] = {2308, 2372, 2436, 2500, 0};

/* battle_effect_helm_splitter.c: Fire swirl pictures. */

/* The seven pictures of the fire swirl, and where each sits from the user. */
Uint8  HelmSplitterPictureWidths[] = {26, 46, 63, 77, 60, 39, 32};
Uint8  HelmSplitterPictureHeights[] = {11, 20, 37, 65, 65, 54, 39};
Uint16 HelmSplitterPictures[] = {0, 286, 1206, 3537, 8542, 12442, 14548};
Sint8  HelmSplitterPictureBiasX[] = {53, 36, 16, 2, 2, 2, 2};
Sint8  HelmSplitterPictureBiasY[] = {2, 3, 3, 3, 4, 14, 29};

/* battle_effect_helm_splitter.c: Sprite formation coordinates and impact burst
   pictures. */

/* The offsets of btEffectPlaceFormationSprs' formations in pixels, one axis at
   a time: Nereid's twelve in rows of 1, 3, 4 and 4; Cybele's eight in rows of
   3, 2 and 3; Tiamat's eight drawn at 1.25 times their size. Thor's, a 3x3
   grid, needs no table. */
Uint8 SummonFormationNereidX[] = {
	52, 25, 57, 89, 7, 39, 71, 103, 21, 53, 85, 117};
Uint8 SummonFormationNereidZ[] = {
	0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96};
Uint8 SummonFormationCybeleX[] = {0, 32, 64, 16, 48, 0, 32, 64};
Uint8 SummonFormationCybeleZ[] = {0, 0, 0, 32, 32, 64, 64, 64};
Uint8 SummonFormationTiamatX[] = {0, 40, 80, 0, 40, 80, 40, 80};
Uint8 SummonFormationTiamatZ[] = {13, 0, 0, 53, 40, 40, 80, 80};
/* The eight pictures of the impact flash. */
Uint8  ImpactBurstPictureWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};
Uint8  ImpactBurstPictureHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};
Uint16 ImpactBurstPictures[] = {
	0, 4608, 9216, 13356, 16020, 18684, 20668, 22020};

/* battle_effect_starfield_fall.c and battle_effect_ramses.c: The fall's
   orbiting sparks, fireballs and flashes; Ramses' statue and puffs. */

/* The seven sparks that orbit the falling sprite. */
Sint16 StarfieldFallOrbitPoints[][3] = {{-64, -96, 24}, {-48, -120, 32},
	{-32, -80, -48}, {-16, -72, -24}, {16, -80, 16}, {32, -112, -32},
	{48, -96, 56}};
/* Where the seven fireballs fall, in pixels. */
Sint16 StarfieldFallFireballPoints[][2] = {
	{32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};
/* The seven growing flash pictures of the blast sprite sheet, square. */
Uint16 StarfieldFallFlashPictures[] = BLAST_PICTURES;
Uint16 StarfieldFallFlashSizes[] = BLAST_PICTURE_SIZES;
/* Where the statue's sprites stand, in pixels from its corner. */
Uint8 RamsesStatueColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};
Uint8 RamsesStatueRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};
/* The puff pictures of the blast sprite sheet, square; only the last four are
   used. */
Uint16 RamsesPuffPictures[] = BLAST_PICTURES;
Uint16 RamsesPuffSizes[] = BLAST_PICTURE_SIZES;

/* battle_effect_ragnarok.c: Flame blade strike columns and flash pictures. */

/* The canvas x of each strike column, three for each side. */
Uint8 RagnarokStrikeColumns[][3] = {
	{32, 24, 0},
	{96, 104, 128},
};
/* The flash picture for each three ticks at the end of a flash's life. */
Uint8 RagnarokFlashPictures[] = FLASH_FADE_PICTURES;

/* battle_effect_judgment.c and battle_effect_titan_blade.c: Titan Blade's
   flashes and dust; Judgment's burst and its shards. */

/* The flash picture for each three ticks at the end of a flash's life. */
Uint8 TitanBladeFlashPictures[] = FLASH_FADE_PICTURES;
/* Dust gravity, by particle index & 3, in NereidGravity's steps. */
Sint32 TitanBladeDustGravity[] = {
	toFIXED(1) / 2, toFIXED(1) / 4, toFIXED(1) / 4, toFIXED(1) / 8};
/* Judgment's burst: seven quarter pictures, each half as wide as it is tall,
   from JUDGMENT_SHEET_BURST. */
Uint8  JudgmentBurstHeights[] = {2, 3, 4, 6, 10, 11, 12};
Uint16 JudgmentShieldSprites[] = {0, 8, 26, 58, 130, 330, 572};
/* The six shard pictures thrown from the burst, from the sprite sheet's
   start. */
Uint16 JudgmentShardPictures[] = {2396, 2985, 3698, 4411, 4810, 5146};
Uint8  JudgmentShardWidths[] = {19, 23, 23, 19, 16, 17};
Uint8  JudgmentShardHeights[] = {31, 31, 31, 21, 21, 24};

/* battle_effect_kirin.c: Kirin's sprites. */

/* Where Kirin's nine sprites stand, in pixels from its corner. */
Uint8 KirinColumns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};
Uint8 KirinRows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

/* battle_effect_neptune.c: Neptune's sprites and its spray's gravity. */

/* Neptune's eight sprites: pixel offsets, scaled by 1.5 as placed. */
Uint8 NeptuneObjectX[] = {0, 32, 64, 0, 32, 64, 96, 96};
Uint8 NeptuneObjectY[] = {0, 0, 0, 32, 32, 32, 0, 32};
/* Spray gravity, by particle index & 3, in NereidGravity's steps. */
Sint32 NeptuneGravity[] = {
	toFIXED(1) / 2, toFIXED(1) / 4, toFIXED(1) / 4, toFIXED(1) / 8};

/* battle_effect_undead_sword.c: Rising burst puff pictures. */

/* The seven square puff pictures, from UNDEAD_SWORD_PUFF_SHEET. */
Uint16 UndeadSwordPuffPictures[] = BLAST_PICTURES;
Uint16 UndeadSwordPuffSizes[] = BLAST_PICTURE_SIZES;
