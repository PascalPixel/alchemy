#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/* @file */
/* What every part of the battle_effect object shares: its includes, the */
/* canvas's and the shared sheets' sizes, the modes of the bodies several */
/* effects play, and the functions and tables the parts reach across each */
/* other. */
/* battle_effect.c includes it before the parts, and each part includes it too.
 */

#include "types.h"
#include "sprite_ids.h"
#include "resource_ids.h"
#include "system.h"
#include "battle/battle_effect.h"
#include "battle_effect/battle_effect.h"
#include "battle_effect/battle_effect_table.h"
#include "battle/battle_effect_state.h"
#include "field/particle.h"
#include "battle/battle_camera.h"
#include "battle/motobj.h"
#include "fixed_math.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "battle/battle_state.h"
#include "battle/render_model.h"
#include "battle/battle.h"
#include "graphics/vram_tab.h"
#include "window/item_icon.h"
#include "sound/sound.h"
#include "battle/battle_effect_params.h"
#include "field/actor.h"
#include "macros.h"

#include "debug/debug.h"
#include "window/window.h"
#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "field/field_event.h"
#include "battle/battle_action.h"
#include "field/sprite.h"
#include "graphics/render_placement.h"
#include "battle/battle_unit.h"
#include "game/class_djinn.h"
#include "version.h"
#include "field/field_sprite.h"

struct EFFECT_SPR;
void *btEffectSprAcquire(s32 width, s32 height, u32 shape, s32 mode);
void *btEffectSprCreateWithTiles(s32 width, s32 height, s32 attributes,
								 s32 flags);

/* A point of a streak: x, y and z. */
typedef struct STREAK_POINT {
	s32 x;
	s32 y;
	s32 z;
} STREAK_POINT;

/* One of Cruel Ruin's 256 streaks: its head and tail and how it is turned. */
typedef struct PLUNGE_STREAK {
	s32 head;
	s32 tail;
	s32 unusedA;
	s32 pitch;
	s32 yaw;
	s32 roll;
	s32 unusedB;
} PLUNGE_STREAK;

/* The element streaks' state, which btEffectStartElementStreaks allocates */
/* in the HEAP_SLOT_BATTLE_EFFECT heap slot in place of the battle effect */
/* state. */
typedef struct ELEMENT_STREAKS_STATE {
	s32 frm;		  /* the streaks' frame count (btEffectDrawStreaks) */
	s32 halvePending; /* the streaks are drawn: halve the canvas into VRAM */
	/* Frames of the background's fade left (btEffectDimBackgroundStep). */
	s32 fadeFrm;
	s32 fadeStep; /* the fade's steps so far, BACKGROUND_LEVEL_STEP each */
} ELEMENT_STREAKS_STATE;

/* An EFFECT_SPR as Cruel Ruin reads its OAM attribute 2: its tile, priority
 */
/* and palette, as fields or as one halfword. */
typedef struct RUIN_OBJECT {
	u8 unusedBeforeAttr[8]; /* the sprite's next and OAM attributes 0 and 1 */
	union {
		struct {
			u16 tile : 10;
			u16 pri : 2;
			u16 pal : 4;
		} bits;
		u16 all;
		u8 bytes[2];
	} attr;
} RUIN_OBJECT;

/* The map cell buffer holds WIN0's line table and then the droplets Douse's */
/* drops throw up. */
typedef struct DROPLET_BUFFER {
	u16 lines[160];
	PARTICLE droplets[512];
} DROPLET_BUFFER;

/* Eclipse's scale for the dragon's sprites, and Cruel Ruin's for its */
/* summon, one to one. */
extern const SCALE Eclipse_DragonScale;
extern const SCALE CruelRuin_UnitScale;

/* One handler for each battle effect, by effect less one; 0 where none. */
extern void (*const BattleEffect_Handlers[])(BATTLE_EFFECT_PARM *args);

/* Zagan's scale, one to one. */
extern const SCALE Zagan_UnitScale;

extern const SCALE Procne_UnitScale; /* the comets' unit scale */

/* The battle screen's display mode while an effect plays: background mode 1 */
/* with BG0 to BG2, sprites and both windows on. */
#define EFFECT_DISPCNT                                                         \
	(DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |   \
	 DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

/* The whole pixels of a stored 16.16 value, read as its upper halfword. */
/* UB: the s32 is read through an s16. */
#ifdef UBFIX
#define FIXED_INTEGER_PART(v) FIXEDtoINT((s32)(v))
#else
#define FIXED_INTEGER_PART(v) (((s16 *)&(v))[1])
#endif

/* The canvas added to the battle under it: both blend weights whole. */
#define EFFECT_BLEND_ADD BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX)
/* The canvas at `level` sixteenths over the whole battle under it. */
#define EFFECT_BLEND_CANVAS(level) BLEND_ALPHA(level, BLEND_WEIGHT_MAX)
/* The canvas's usual weight over the battle, in sixteenths: every canvas */
/* opens with it. */
#define CANVAS_BLEND_LEVEL 14
/* The element pillar's and element burst's canvas weight over the battle. */
#define ELEMENT_BLEND_LEVEL 12
/* The motes the element pillar and element burst set up in PARTICLES; the */
/* burst leaves them undrawn. */
#define ELEMENT_SPIRAL_MOTES 128

/* The map cell buffer as btEffectBuildWindowEdgeTable reads it: window */
/* 0's right edge, how far each of its lines is set in from that edge, and the
 */
/* right edge it builds for every screen line. */
#define WINDOW_EDGE_RIGHT		((u16 *)MapGridCellBuffer)
#define WINDOW_EDGE_INSETS		(Ram_MapCellBuffer + 2)
#define WINDOW_EDGE_INSET_LINES 128
#define WINDOW_EDGE_LINES		(WINDOW_EDGE_INSETS + WINDOW_EDGE_INSET_LINES)

/* The particles an effect keeps in the map cell buffer, from its start. */
#define PARTICLES ((PARTICLE *)Ram_MapCellBuffer)
/* The particles an effect keeps in the map cell buffer from its second 16 KB */
/* on, past PARTICLES or the pictures and meshes it keeps in the first: a */
/* second, larger table, always reached at its fixed address, where the */
/* effects that throw more sparks, motes or trails than the state's 64 */
/* particles keep them. */
#define EXTRA_PARTICLES ((PARTICLE *)(Ram_MapCellBuffer + 16384))

/* The VRAM address of a sprite's tiles, from its VRAM cache entry. */
#define OBJ_TILES(sprite)                                                      \
	(OBJ_VRAM0 + VramCacheEntries[((SPR *)(sprite))->slot].ofs)

/* Index of DMAxCNT_H, the control half, in a channel's 16-bit registers. */
#define DMA_CONTROL_HALF 5

/* The particles the map cell buffer holds when an effect lays its particles */
/* over it from the start. */
#define PARTICLE_POOL_COUNT 1024
/* Nereid's drops burst up, in btEffectRunNereid and in the unused copy of */
/* it: every free one of the first NEREID_BURST_DROPS as the columns gather, */
/* then NEREID_DROPS_A_FRAME a frame while they rise. */
#define NEREID_BURST_DROPS	 256
#define NEREID_DROPS_A_FRAME 16
/* The sprites that draw Megaera, in her summon and in her formation. */
#define MEGAERA_OBJECTS 11

/* The canvas: 128x128 pixels, one byte each, in its heap slot and in VRAM at */
/* BG_CHAR_BLOCK(1) (CANVAS_BYTES). */
#define CANVAS_SIZE 128
/* The canvas's rows and columns of 8x8 tiles. */
#define CANVAS_TILES (CANVAS_SIZE / 8)
/* The byte of canvas pixel (x, y): the canvas is laid out in 8x8 tiles, row */
/* by row, each tile 64 bytes row by row. */
#define CANVAS_PIXEL_OFFSET(x, y)                                              \
	((((y) / 8 * CANVAS_TILES + (x) / 8) * 8 + ((y) & 7)) * 8 + ((x) & 7))
/* The bytes of one 8x8 tile of the canvas; a row of pixels steps from one tile
 */
/* to the next by CANVAS_TILE_BYTES - 8. */
#define CANVAS_TILE_BYTES (8 * 8)
/* The bytes of a 32x32 sprite's tiles in 256 colours, and in 16. */
#define OBJ_32X32_8BPP_BYTES (32 * 32)
#define OBJ_32X32_4BPP_BYTES (32 * 32 / 2)
/* The bytes of a 64x64 sprite's tiles in 256 colours, and in 16. */
#define OBJ_64X64_8BPP_BYTES (64 * 64)
#define OBJ_64X64_4BPP_BYTES (64 * 64 / 2)
/* The bytes of a 16x16 sprite's tiles in 16 colours. */
#define OBJ_16X16_4BPP_BYTES (16 * 16 / 2)
/* The byte of the first pixel of canvas row y, y not negative. */
#define CANVAS_ROW_OFFSET(y) ((((y) & 7) << 3) + (((y) >> 3) << 10))
/* The brightest canvas level: the additive blitters and */
/* CANVAS_COPY_AND_BRIGHTEN stop at it. */
#define CANVAS_LEVEL_BRIGHTEST 63

/* The battle view, under the top window and over the menus: screen lines 16 */
/* to 135. */
#define BATTLE_VIEW_TOP	   16
#define BATTLE_VIEW_HEIGHT 120
#define BATTLE_VIEW_BOTTOM (BATTLE_VIEW_TOP + BATTLE_VIEW_HEIGHT)
/* The backdrop gradients Azul and Coatlicue write over the battle view, a */
/* colour a line from BATTLE_VIEW_TOP - 1: each channel a level darker every */
/* so many lines down, blue every 15, green every 16 and red every 17. */
#define GRADIENT_BLUE_LINES	 15
#define GRADIENT_GREEN_LINES 16
#define GRADIENT_RED_LINES	 17

/* Writes one row of BG2's map for the canvas, two tiles at a time. */
#define EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset)                       \
	for((column) = 0; (column) != 8; (column)++, (offset) += 2) {              \
		s32 tileIndex;                                                         \
		s16 pair;                                                              \
                                                                               \
		tileIndex = (row) * 16 + (column) * 2;                                 \
		pair = ((tileIndex + 1) << 8) | tileIndex;                             \
		*(volatile s16 *)((u8 *)BG_CANVAS_MAP + (offset)) = pair;              \
	}

/* Three summons open alike (Eclipse, Daedalus and Flora): the battle flashes */
/* away over SUMMON_OPENING_FRAMES, and at SUMMON_OPENING_LOAD_FRAME */
/* their sky and their sprites load. */
#define SUMMON_OPENING_FRAMES	  52
#define SUMMON_OPENING_LOAD_FRAME 27

/* Windows 0 and 1 both cover the battle view: the full width, lines 16 to */
/* 135. */
#define EFFECT_SET_BATTLE_WINDOWS()                                            \
	(REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH),                                  \
	 REG_WIN0V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM),               \
	 REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH),                                  \
	 REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM))
/* The canvas's first BATTLE_VIEW_HEIGHT lines, which the battle view shows: */
/* what most of this game's effects clear and fill, leaving the last 8. */
#define CANVAS_VIEW_BYTES (CANVAS_SIZE * BATTLE_VIEW_HEIGHT)

/* How far right the narrow canvas is set to show over the party: against the */
/* screen's right edge. */
#define NARROW_CANVAS_PARTY_X (DISPLAY_WIDTH - CANVAS_SIZE)

/* Where the summons centre the projection while their figures fly, lower */
/* than the battle's BATTLE_PROJECTION_CENTER_Y. */
#define SUMMON_PROJECTION_CENTER_Y 240

/* The frames the battle background darkens behind an effect's canvas */
/* (state->fadeFrm, btEffectDarkenBackgroundStep). */
#define BACKGROUND_DARKEN_FRAMES 24
/* The level btBackgroundLoadStub darkens the battle's own background to */
/* as it comes back after an effect: as dark as the canvas's fade left it. */
#define BACKGROUND_DARKENED_LEVEL BACKGROUND_DARKEN_FRAMES

/* A sprite sheet resource opens with its 64 colours, ahead of its compressed */
/* pictures. */
#define SHEET_PALETTE_COLORS 64
#define SHEET_PALETTE_BYTES	 (SHEET_PALETTE_COLORS * 2)

/* Loads a sprite sheet's colours into the background palette. */
#define LOAD_BG_PALETTE(resource)                                              \
	{                                                                          \
		void *palette = syResourceGetTableEntry(resource);                     \
                                                                               \
		syWordsCopy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);            \
	}

/* The damping that keeps a particle's whole speed. */
#define PARTICLE_DAMPING_NONE DAMPING_SCALE

/* The bytes of a sprite sheet picture, one byte a pixel, from its name's */
/* _WIDTH and _HEIGHT. */
#define PICTURE_BYTES(name) (name##_WIDTH * name##_HEIGHT)

/* Draws picture `name` of `sheet` at its own size, its top left at (x, y). */
#define EFFECT_DRAW_PICTURE(draw, canvas, sheet, name, x, y)                   \
	(draw)(canvas, (sheet) + (name), x, y, name##_WIDTH, name##_HEIGHT)

/* A bite's half pictures (BlueDropletSheet's closing fangs, each drawn beside
 */
/* its mirror): the canvas row they start at, and their size. */
#define TARGET_STRIKE_BITE_Y	  56
#define TARGET_STRIKE_BITE_WIDTH  18
#define TARGET_STRIKE_BITE_HEIGHT 48

/* The battle background's image as BG1 shows it from BG character block 2, */
/* one byte a pixel. */
#define BATTLE_BACKGROUND_WIDTH	 256
#define BATTLE_BACKGROUND_HEIGHT 120
#define BATTLE_BACKGROUND_BYTES                                                \
	(BATTLE_BACKGROUND_WIDTH * BATTLE_BACKGROUND_HEIGHT)
/* A background's scroll (BG_SCROLL) is a halfword: a negative scroll is
 */
/* held BG_SCROLL_WRAP above itself, and read back by taking it away. */
#define BG_SCROLL_WRAP (1 << 16)
/* Two entries of a background map in one word: `left`, then the one to its */
/* right. */
#define MAP_ENTRY_PAIR(left, right) ((u32)(right) << 16 | (left))
/* The tile BG1 shows around a picture of its own, the last a map entry can */
/* name. */
#define BG1_CLEAR_TILE 1023
/* BG character block 2's first tile, counted from BG1's character base. */
#define BG1_BLOCK2_FIRST_TILE (2 * BG_CHAR_SIZE / TILE_SIZE_8BPP)
/* The bytes of one 16-colour palette. */
#define PALETTE_BANK_BYTES 32
/* A word of two palette entries, both `color`, to fill a palette a word at a */
/* time. */
#define COLOR_PAIR(color) ((color) << 16 | (color))

/* A model's shade (RENDER_MODEL.shade) is added to each of its pixels' canvas
 */
/* levels: at this or below, nothing of the model is left to draw, and the */
/* effects skip it. */
#define MODEL_SHADE_INVISIBLE (-64)

/* RENDER_MODEL.kind: the rasterizer kernel btRenderDrawModelList copies into */
/* IWRAM to draw a model, and how it puts each pixel on the canvas. A solid */
/* model fills its faces with their colours; a textured one maps its texture; */
/* a shaded one adds the model's shade to each texel and skips those it leaves
 */
/* at 0 or below. The narrow kernels draw into a target half the canvas's */
/* width. */
typedef enum MODEL_KIND {
	MODEL_END,		 /* ends the list */
	MODEL_SOLID,	 /* Render_SolidKernel: the colour replaces the canvas */
	MODEL_SOLID_MAX, /* the colour where it is brighter */
	MODEL_SOLID_ADDITIVE,	 /* the colour added, up to the brightest level */
	MODEL_TEXTURED_MAX,		 /* the texel where it is brighter */
	MODEL_TEXTURED_ADDITIVE, /* the texel added, up to the brightest level */
	MODEL_SHADED_MAX,		 /* the shaded texel where it is brighter */
	MODEL_SHADED_ADDITIVE,	 /* the shaded texel added, up to the brightest */
	MODEL_TEXTURED,			 /* the texel replaces the canvas */
	MODEL_SHADED,			 /* the shaded texel replaces the canvas */
	MODEL_TEXTURED_NARROW,	 /* MODEL_TEXTURED into a narrow target */
	MODEL_SOLID_NARROW		 /* MODEL_SOLID into a narrow target */
} MODEL_KIND;

/* RENDER_MODEL.flg that the rasterizer kernels read: a one-sided model draws
 */
/* only the faces wound one way round on the screen, clockwise with */
/* MODEL_CLOCKWISE and anticlockwise without it, and skips the others. */
#define MODEL_ONE_SIDED 2
#define MODEL_CLOCKWISE 1

/* Starts a frame's 3D projection from the battle camera. */
#define EFFECT_VIEW_FROM_CAMERA(camera)                                        \
	(syInitMatrix(), syMatrixSetLookAt((camera)->eye, (camera)->focus))

/* The sprite sheets' pictures, as The Broken Seal names them: this game draws
 */
/* the same sprite sheets at the same sizes. */

/* The bytes of the six glint pictures that open the tornado sprite sheet. */
#define GLINT_SET_BYTES                                                        \
	(19 * 31 + 23 * 31 + 23 * 31 + 19 * 21 + 16 * 21 + 17 * 24)

/* Draws flare picture `size`, a square, centred on (x, y). */
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size)                     \
	(draw)(canvas, (sheet) + BattleEffect_FlarePictures[(size) - 1],           \
		   (x) - (size), (y) - (size), (size) * 2, (size) * 2)

/* The reaper in CounterRevealSheet, which Condemn and Mortal Danger decode: */
/* the reaper with its scythe raised, then swung, then the red crescent the */
/* swing leaves. */
#define REAPER_SHEET_RAISED_WIDTH	 57
#define REAPER_SHEET_RAISED_HEIGHT	 98
#define REAPER_SHEET_SWUNG_WIDTH	 99
#define REAPER_SHEET_SWUNG_HEIGHT	 69
#define REAPER_SHEET_CRESCENT_WIDTH	 128
#define REAPER_SHEET_CRESCENT_HEIGHT 91
/* Where each picture starts in ReaperSheet, in bytes. */
typedef enum REAPER_SHEET_PICTURE {
	REAPER_SHEET_RAISED = 0,
	REAPER_SHEET_SWUNG =
		REAPER_SHEET_RAISED + PICTURE_BYTES(REAPER_SHEET_RAISED),
	REAPER_SHEET_CRESCENT =
		REAPER_SHEET_SWUNG + PICTURE_BYTES(REAPER_SHEET_SWUNG)
} REAPER_SHEET_PICTURE;

/* RedCrescentSheetA: the swing's crescent fading in four pictures. */
#define RED_CRESCENT_FADE_A_WIDTH  128
#define RED_CRESCENT_FADE_A_HEIGHT 91
#define RED_CRESCENT_FADE_B_WIDTH  128
#define RED_CRESCENT_FADE_B_HEIGHT 59
#define RED_CRESCENT_FADE_C_WIDTH  122
#define RED_CRESCENT_FADE_C_HEIGHT 29
#define RED_CRESCENT_FADE_D_WIDTH  76
#define RED_CRESCENT_FADE_D_HEIGHT 25
/* Where each picture starts in RedCrescentSheetA, in bytes. */
typedef enum RED_CRESCENT_PICTURE {
	RED_CRESCENT_FADE_A = 0,
	RED_CRESCENT_FADE_B =
		RED_CRESCENT_FADE_A + PICTURE_BYTES(RED_CRESCENT_FADE_A),
	RED_CRESCENT_FADE_C =
		RED_CRESCENT_FADE_B + PICTURE_BYTES(RED_CRESCENT_FADE_B),
	RED_CRESCENT_FADE_D =
		RED_CRESCENT_FADE_C + PICTURE_BYTES(RED_CRESCENT_FADE_C)
} RED_CRESCENT_PICTURE;

/* The slash pictures as wide and tall as the canvas's view: StarBurstSheet's,
 */
/* CrescentSheet's and the reveal masks. */
#define SLASH_PICTURE_SIZE 120

/* Closing bolts and lightning columns use the same 24x120 pictures. */
#define LIGHTNING_BOLT_WIDTH		 24
#define LIGHTNING_BOLT_HEIGHT		 120
#define LIGHTNING_BOLT_PICTURE_BYTES PICTURE_BYTES(LIGHTNING_BOLT)

/* BlueFlameSheet: the column of flame, then the flame that splashes from it, */
/* twice its width. */
#define BLUE_FLAME_COLUMN_WIDTH	 17
#define BLUE_FLAME_COLUMN_HEIGHT 104
#define BLUE_FLAME_SPLASH_WIDTH	 34
#define BLUE_FLAME_SPLASH_HEIGHT 65
/* WaveSheet's six pictures, BlueArcSheetA and B's six, and YellowOrbSheet's. */
#define WAVE_WIDTH		  48
#define WAVE_HEIGHT		  16
#define BLUE_ARC_WIDTH	  48
#define BLUE_ARC_HEIGHT	  72
#define YELLOW_ORB_WIDTH  40
#define YELLOW_ORB_HEIGHT 48
/* The yellow orb as btEffectPackYellowOrbs packs it, every second byte. */
#define YELLOW_ORB_PACKED_WIDTH	 (YELLOW_ORB_WIDTH / 2)
#define YELLOW_ORB_PACKED_HEIGHT YELLOW_ORB_HEIGHT
#define YELLOW_ORB_PICTURES		 6

/* FireStreakSheet's six pictures and FireBurstSheet's. */
#define FIRE_STREAK_WIDTH  48
#define FIRE_STREAK_HEIGHT 72
#define FIRE_BURST_WIDTH   40
#define FIRE_BURST_HEIGHT  80
/* The small Djinn sprite sheets' pictures are SMALL_DJINNI_WIDTH wide; */
/* Mercury's and Venus's are shorter than SMALL_DJINNI_HEIGHT. */
#define MERCURY_SMALL_DJINNI_HEIGHT 34
#define VENUS_SMALL_DJINNI_HEIGHT	38
/* FireBlobSheet's blobs and CrystalSheet's crystal. */
#define FIRE_BLOB_WIDTH	 48
#define FIRE_BLOB_HEIGHT 32
#define CRYSTAL_WIDTH	 32
#define CRYSTAL_HEIGHT	 64
/* CrystalSheet's shard pictures, after its crystal, and the shards of */
/* Prism_ShardStarts a crystal breaks into. */
#define CRYSTAL_SHARD_PICTURES PICTURE_BYTES(CRYSTAL)
#define SHARDS_PER_CRYSTAL	   16
/* BoulderSheet's boulder, its first picture, and its shard pictures after it.
 */
#define BOULDER_WIDTH		   34
#define BOULDER_HEIGHT		   62
#define BOULDER_SHARD_PICTURES (BOULDER_WIDTH * BOULDER_HEIGHT)
/* A shard turns through its three pictures, one each SHARD_TURN of its */
/* variant. */
#define SHARD_TURN 96
/* PaletteRampImage's shard pictures (drawn through StatChange_ShardPictures),
 */
/* where the stat reduction and the Sword Card decode them in the sprite sheet,
 */
/* followed by DIMMED_COPIES - 1 copies of them, each darker than the one */
/* before. */
#define STAT_REDUCTION_SHARD_PICTURES 9600
#define STAT_REDUCTION_SHARD_BYTES	  696 /* bytes in each copy */

/* LightningPillarSheet's pillar, in two pictures. */
#define LIGHTNING_PILLAR_WIDTH	48
#define LIGHTNING_PILLAR_HEIGHT 112
/* FirePillarSheetA: the pillar of fire, then two flames. */
#define FIRE_PILLAR_WIDTH		 34
#define FIRE_PILLAR_HEIGHT		 104
#define FIRE_PILLAR_FLAME_WIDTH	 20
#define FIRE_PILLAR_FLAME_HEIGHT 37
/* SpiralSheet's pictures. */
#define SPIRAL_WIDTH  32
#define SPIRAL_HEIGHT 32
/* A card's picture, as the Sleep, Baffle, Death and Sword Cards and the */
/* Bramble, Frost, Flame and Thunder Cards draw it. */
#define CARD_WIDTH	32
#define CARD_HEIGHT 64
/* SheepCardSheet's sheep, four pictures of it tumbling after its card. */
#define SHEEP_WIDTH	 32
#define SHEEP_HEIGHT 32

/* FlameballSheet and FireballSheet's pictures. */
#define FLAMEBALL_WIDTH	 40
#define FLAMEBALL_HEIGHT 64

/* Each half of RuneSheet's rune, the second drawn mirrored. */
#define RUNE_HALF_WIDTH	 24
#define RUNE_HALF_HEIGHT 48
/* The 64x64 texture Flash Force and Void Beam build: the rune's two halves */
/* side by side, the second mirrored, eight pixels in from the top left. */
#define RUNE_TEXTURE_SIZE 64
#define RUNE_TEXTURE_RUNE (8 * RUNE_TEXTURE_SIZE + 8)

/* A Djinni's picture: each frame of the full Djinn sprite sheets */
/* (VenusDjinnSheet and its kind), and the one picture of the small sprite */
/* sheets. */
#define DJINNI_WIDTH		40
#define DJINNI_HEIGHT		40
#define SMALL_DJINNI_WIDTH	20
#define SMALL_DJINNI_HEIGHT 40

/* The pictures of the sprite sheets several effects share, each sprite sheet's
 */
/* pictures all one size: SmokeSheet's puffs, FlashBurstSheet's bursts, */
/* SkullSheet's skulls and the tornado strips after TornadoSheet's glints. */
#define SMOKE_PUFF_WIDTH  24
#define SMOKE_PUFF_HEIGHT 48
/* A puff of SmokeSheet's lives through its six pictures, four frames each. */
#define SMOKE_PUFF_FRAMES  (6 * 4)
#define FLASH_BURST_WIDTH  32
#define FLASH_BURST_HEIGHT 64
/* FlashBurstSheet's six pictures: a white flash turning to a fireball that */
/* burns out to embers. */
#define FLASH_BURST_PICTURE_COUNT 6
/* BlueBeamSheet: a blue bubble, a small splash of blue light the bubble's */
/* size, then a large splash. */
#define BLUE_BUBBLE_WIDTH		 24
#define BLUE_BUBBLE_HEIGHT		 24
#define BLUE_LARGE_SPLASH_WIDTH	 42
#define BLUE_LARGE_SPLASH_HEIGHT 42
/* BlueCloudPuffSheet: four 32x32 puffs of cloud. */
#define CLOUD_PUFF_WIDTH	 32
#define CLOUD_PUFF_HEIGHT	 32
#define SKULL_WIDTH			 24
#define SKULL_HEIGHT		 24
#define TORNADO_STRIP_WIDTH	 40
#define TORNADO_STRIP_HEIGHT 32
/* The pictures of DaggerSheet, ShieldSheet, GlowOrbSheet, VioletStreakSheet */
/* and WispSheet. */
#define DAGGER_WIDTH		 24
#define DAGGER_HEIGHT		 40
#define SHIELD_WIDTH		 40
#define SHIELD_HEIGHT		 40
#define GLOW_ORB_WIDTH		 40
#define GLOW_ORB_HEIGHT		 40
#define VIOLET_STREAK_WIDTH	 32
#define VIOLET_STREAK_HEIGHT 64
#define WISP_WIDTH			 64
#define WISP_HEIGHT			 32
/* VortexSheet's six pictures and HeartSheet's heart. */
#define VORTEX_WIDTH  24
#define VORTEX_HEIGHT 48
#define HEART_WIDTH	  22
#define HEART_HEIGHT  42
/* WaterSpraySheet: the wave, then the splash's pictures. */
#define WATER_WAVE_WIDTH	32
#define WATER_WAVE_HEIGHT	32
#define WATER_SPLASH_WIDTH	48
#define WATER_SPLASH_HEIGHT 48

/* The canvas life cycle, shared by every canvas effect. */

/* The bytes an effect's state is lent: more than struct */
/* BATTLE_EFFECT_STATE holds. */
#define BATTLE_EFFECT_STATE_SIZE 31612

/* The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100). */
#define MOTE_SET_BYTES 770
/* A picture's dimmed copies: DIMMED_COPIES of it, copy i with every level */
/* capped at DIMMED_COPY_CAP(i), 7 levels lower than the copy before, copy 0 */
/* not at all. Thor's ring darkens through copies of the mote pictures, a stat
 */
/* reduction's shards through copies of the shard. */
#define DIMMED_COPIES		  8
#define DIMMED_COPY_CAP(copy) (CANVAS_LEVEL_BRIGHTEST + 1 - (copy) * 7)
/* Caps a colour level at cap, and at 0 from below. */
#define EFFECT_CAP_LEVEL(level, cap)                                           \
	{                                                                          \
		if((level) > (cap))                                                    \
			(level) = (cap);                                                   \
		if((level) < 0)                                                        \
			(level) = 0;                                                       \
	}
/* The bytes of BlueGlowSheet's ten flare pictures, 2 to 20 pixels square: */
/* 4 * (1 + 4 + ... + 100). */
#define FLARE_SET_BYTES 1540
/* The battle sheet (HEAP_SLOT_BATTLE_SHEET) the casting impacts, the plain */
/* attacks and Cruel Ruin decode their glows into: room for BlueGlowSheet's */
/* flares, the largest of them, with ten bytes to spare. */
#define BATTLE_SHEET_BYTES (FLARE_SET_BYTES + 10)
#define MOTE_PICTURES	   10

/* The columns of TargetStrike_Styles, one row a style. */
typedef enum TARGET_STRIKE_COLUMN {
	TARGET_STRIKE_KIND,			  /* STRIKE_KIND_SLASH or STRIKE_KIND_BITE */
	TARGET_STRIKE_HITS,			  /* slashes or bites */
	TARGET_STRIKE_FLAGS,		  /* the extras (STRIKE_SMOKE and on) */
	TARGET_STRIKE_COLOR,		  /* the background's palette (STRIKE_COLOR_) */
	TARGET_STRIKE_PICTURE_FRAMES, /* frames each picture shows */
	TARGET_STRIKE_COMPOSITE,	  /* the bites' blitter mode */
	TARGET_STRIKE_FRAMES		  /* the effect's length */
} TARGET_STRIKE_COLUMN;

/* The two kinds of target strike (TargetStrike_Styles). */
#define STRIKE_KIND_SLASH 0 /* sideways slashes */
/* Rows of fangs above and below the target that close on it. */
#define STRIKE_KIND_BITE 1

/* A target strike style's extras. */
#define STRIKE_SMOKE                                                           \
	1 /* each hit raises smoke puffs from the target's feet                    \
	   */
#define STRIKE_ABSORB                                                          \
	2					/* motes fly out of the target and back into the user  \
						 */
#define STRIKE_GLINTS 4 /* glints sparkle around the target */
#define STRIKE_SPARKS 8 /* each hit scatters sparks */
#define STRIKE_HEAVY                                                           \
	16					/* one heavy hit: a big shake and the target recoils   \
						 */
#define STRIKE_FLASH 32 /* as heavy, with flash pictures for the smoke */
#define STRIKE_STAR	 64 /* a star swells on the target */

/* The palettes a target strike's background takes (TARGET_STRIKE_COLOR). */
#define STRIKE_COLOR_BLUE	   0 /* BlueBeamSheet's */
#define STRIKE_COLOR_LIME	   1 /* LimePalette */
#define STRIKE_COLOR_PINK	   2 /* PinkPalette */
#define STRIKE_COLOR_EMBER	   3 /* EmberStreakSheet's */
#define STRIKE_COLOR_MARS	   4 /* MarsDjinnSheet's */
#define STRIKE_COLOR_LIGHTNING 5 /* LightningBoltSheet's */
#define STRIKE_COLOR_ORANGE	   6 /* OrangePaletteB */

/* The target strike styles, rows of TargetStrike_Styles: The Broken Seal's */
/* twelve, then this game's own. */
typedef enum TARGET_STRIKE_STYLE {
	TARGET_STRIKE_STYLE_BLUE_SPARK_BITES,
	TARGET_STRIKE_STYLE_LIME_SMOKE_BITE,
	TARGET_STRIKE_STYLE_PINK_SMOKE_BITES,
	TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES,
	TARGET_STRIKE_STYLE_MARS_DRAIN_BITES,
	TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES,
	TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH,
	TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES,
	TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH,
	TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH,
	TARGET_STRIKE_STYLE_MARS_SMOKE_BITE,
	TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES,
	TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES,
	TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES,
	TARGET_STRIKE_STYLE_ORANGE_FLASH_GLINT_SLASH,
	TARGET_STRIKE_STYLE_MANEATER, /* the same row as MARS_ADDITIVE_DRAIN_BITES
								   */
	TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH,
	TARGET_STRIKE_STYLE_MARS_HEAVY_BITE,
	TARGET_STRIKE_STYLE_ORANGE_FLASH_BITE,
	TARGET_STRIKE_STYLE_MARS_STAR_SLASH,
	TARGET_STRIKE_STYLE_BLUE_SPARK_BITE,
} TARGET_STRIKE_STYLE;

/* Window 0 shows the battle view: BG0 to BG2, the sprites and blending. */
#define EFFECT_WIN0_LAYERS                                                     \
	(WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |       \
	 WININ_WIN0_CLR)

/* Outside the battle view: BG0 and blending only. */
#define EFFECT_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

/* Opens the canvas layer at its own width: BG2 shows it one pixel a pixel. */
#define EFFECT_START_NARROW_CANVAS(bgControl)                                  \
	(btEffectStartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/* Strikes target `i`: it flashes white in its damage pose and is knocked back
 */
/* by `motion`. */
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
	(btEffectTintBattler((effect)->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE,   \
						 i, frames),                                           \
	 btMotionApplyKnockbackStub((effect)->tgts[i], motion))

/* A blitter that clips its rectangles to the canvas. */
#define EFFECT_CLIP_BOUNDS (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/* The second blitter, the drawing function in the paired heap slot. */
#define ALT_BLITTER (Ram_HeapSlots->ramCodePaired)

/* Builds a blitter for the canvas in heap slot `slot`, for one clip, mirror */
/* and blend mode (BLITTER_ADDITIVE to BLITTER_COPY): where both are constants
 */
/* they go packed into one word to btEffectLoadBlitter, the shorter call, */
/* and otherwise to btEffectBuildBlitter as they are. The name is ours: */
/* every glint draw and mirrored pair that picks its flags or its mode at run */
/* time is written so. */
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
	(__builtin_constant_p((flags) | (mode) << 4)                               \
		 ? btEffectLoadBlitter(slot, (flags) | (mode) << 4)                    \
		 : btEffectBuildBlitter(slot, 7, 7, flags, mode))

/* Sets what the canvas transfer leaves in the canvas after each copy. */
#define EFFECT_SET_TRANSFER(state, mode, value)                                \
	((state)->transferMode = (mode), (state)->transferValue = (value))

/* Sets the transfer, then starts copying the canvas to VRAM once a frame. */
#define EFFECT_START_TRANSFER(state, mode, value)                              \
	(EFFECT_SET_TRANSFER(state, mode, value),                                  \
	 sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK))

/* Presents the canvas and waits for the next frame. */
#define EFFECT_PRESENT_CANVAS(state) ((state)->transferPending = 1, sySynch(1))

/* Waits until a VBlank has passed since the one that last copied the canvas. */
/* DEAD: each test reads the count twice and drops the first read; the ROM */
/* keeps both loads. */
#define WAIT_TRANSFER(state)                                                   \
	while((state)->vblanksSinceTransfer, (state)->vblanksSinceTransfer <= 1)

/* The usual end of an effect's frame: counts down the targets' flash timers, */
/* then presents the canvas. */
#define EFFECT_END_FRAME(state)                                                \
	(btEffectCountDownBattlerTints(), EFFECT_PRESENT_CANVAS(state))

/* Frees the two blitters, the second first. */
#define EFFECT_RELEASE_BLITTERS()                                              \
	(syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED),                             \
	 syHeapSlotRelease(HEAP_SLOT_RAM_CODE))

/* The usual end of a canvas effect: the transfer, the blitters, then the */
/* canvas layer. */
#define EFFECT_FINISH_CANVAS()                                                 \
	(syCloseEvent(btEffectTransferCanvas), EFFECT_RELEASE_BLITTERS(),          \
	 btEffectEndCanvasLayer())

/* The same for an effect that drew through one blitter. */
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
	(syCloseEvent(btEffectTransferCanvas),                                     \
	 syHeapSlotRelease(HEAP_SLOT_RAM_CODE), btEffectEndCanvasLayer())

/* The largest of the ten mote pictures. */
#define MOTE_SIZE_MAX 10
/* Draws mote picture `size` (1 to MOTE_SIZE_MAX: size pixels wide, twice that
 */
/* tall), centred across x and standing on y. */
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
	(draw)(canvas, (sheet) + BattleEffect_MotePictures[(size) - 1],            \
		   (x) - (size) / 2, (y) - (size), size, (size) * 2)

/* Marks particles[0] to particles[count - 1] free: a particle whose variant */
/* is -1 is not drawn or moved until an effect gives it a life. */
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
	for((i) = 0; (i) != (count); (i)++)                                        \
	(particles)[i].variant = -1

/* Shows the canvas stretched to twice its width, `top` sixteenths of its */
/* colour added over what lies under it. */
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
	(REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                     \
	 REG_BLDALPHA = EFFECT_BLEND_CANVAS(top),                                  \
	 REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/* A line's 16.16 sideways sway as its BG2X value in state->bg2xLines, which */
/* the wave HBlank DMA writes line by line. BG2X counts 256ths of a canvas */
/* pixel, so a sway of toFIXED(n) moves the line n / 4 canvas pixels: n / 2 */
/* screen pixels where the canvas is shown at twice its width. */
#define CANVAS_WAVE_BG2X(sway) ((sway) >> 10)

/* Which way btEffectWipeCanvas sweeps. */
typedef enum CANVAS_WIPE_DIRECTION {
	CANVAS_WIPE_DOWN,  /* top to bottom */
	CANVAS_WIPE_ACROSS /* left to right */
} CANVAS_WIPE_DIRECTION;
/* What btEffectWipeCanvas leaves behind it. */
typedef enum CANVAS_FILL {
	CANVAS_WIPE_FILL, /* colour 1, which its callers make black first */
	CANVAS_WIPE_CLEAR /* colour 0: the canvas cleared */
} CANVAS_FILL;

/* Lets the battle show the damage, playing hitSound as the targets react: 0 */
/* keeps the one queued, and HIT_SOUND_NONE plays none where the effect has */
/* played its own. A second use changes nothing. */
#define EFFECT_SHOW_DAMAGE(hitSound) btPlaybackStartStub(hitSound)
#define HIT_SOUND_NONE				 (-1)

/* Passed to btEffectTintBattler as its palette, pose or timer slot: leave */
/* that one as it is. */
#define TINT_KEEP (-1)

/* The tumbling quads' modes (btEffectRunTumblingQuads), each named for */
/* the effect that plays it. */
typedef enum TUMBLING_QUADS_MODE {
	TUMBLING_QUADS_BALM,   /* over a prepared canvas, with the Mercury Djinn */
	TUMBLING_QUADS_TINDER, /* over a prepared canvas, with the Mars Djinn */
	TUMBLING_QUADS_PHOENIX /* rising from the first target */
} TUMBLING_QUADS_MODE;

/* The gasp family's modes (btEffectRunGaspFamily), each named for the */
/* effect that plays it. */
typedef enum GASP_MODE {
	GASP_MODE_GASP,
	/* Unused: no wrapper passes it; a Mercury Djinni and a burst of light. */
	GASP_MODE_UNUSED,
	GASP_MODE_REFLUX,
	GASP_MODE_DJINN_COUNTER,
	GASP_MODE_MOLD,
	GASP_MODE_SHINE,
	GASP_MODE_LULL,
	GASP_MODE_COUNTER_RUSH,
	GASP_MODE_DEMON_EYE,
	GASP_MODE_EMU,
	GASP_MODE_CALL_DULLAHAN,
	GASP_MODE_BEAT_DANCE,
	GASP_MODE_ELEMENT_SWAP,
	GASP_MODE_CAGE,
	GASP_MODE_CLARION_CRY,
	GASP_MODE_PSY_BOOST
} GASP_MODE;

/* The twisting column's modes (btEffectRunTwistingColumn), each named for */
/* the effect that plays it. */
typedef enum TWISTING_COLUMN_MODE {
	TWISTING_COLUMN_WHORL,
	TWISTING_COLUMN_GALE,
	TWISTING_COLUMN_LETHE_ALBION,
	TWISTING_COLUMN_POISON_FLOW,
	TWISTING_COLUMN_RAGING_FLOOD
} TWISTING_COLUMN_MODE;

/* The ball lightning's modes (btEffectRunBallLightning). */
typedef enum BALL_LIGHTNING_MODE {
	BALL_LIGHTNING_THUNDER_MINE,
	BALL_LIGHTNING_EARTH_FORCE
} BALL_LIGHTNING_MODE;

/* The Djinni charge's modes (btEffectRunDjinniCharge). */
typedef enum DJINNI_CHARGE_MODE {
	DJINNI_CHARGE_CORE,
	DJINNI_CHARGE_CANNON
} DJINNI_CHARGE_MODE;

/* The heavy blow's modes (btEffectRunHeavyBlow). */
typedef enum HEAVY_BLOW_MODE {
	HEAVY_BLOW_HEAVY_DIVIDE,
	HEAVY_BLOW_STRONG_HIT
} HEAVY_BLOW_MODE;

/* The Djinni sign's modes (btEffectRunDjinniSign). */
typedef enum DJINNI_SIGN_MODE {
	DJINNI_SIGN_RIME,
	DJINNI_SIGN_HAZE
} DJINNI_SIGN_MODE;

/* The cards (btEffectRunCards). */
typedef enum CARD_KIND {
	CARD_SLEEP,	 /* Sleep Card: sheep jump from the card. */
	CARD_BAFFLE, /* Baffle Card: butterflies, and the screen waves. */
	CARD_DEATH,	 /* Death Card: a skeleton reaps the target. */
	CARD_SWORD,	 /* Sword Card: a dagger strikes, and shards fly. */
} CARD_KIND;

/* The element cards (btEffectRunElementCards). */
typedef enum ELEMENT_CARD_KIND {
	CARD_BRAMBLE, /* Bramble Card: vines grow from the card. */
	CARD_FROST,	  /* Frost Card: the card throws crystal shards. */
	CARD_FLAME,	  /* Flame Card: the card throws smoke and flashes. */
	CARD_THUNDER, /* Thunder Card: bolts strike from the card. */
} ELEMENT_CARD_KIND;

/* The breaths (btEffectRunBreaths). */
typedef enum BREATH_KIND {
	BREATH_BEAM,		  /* Beam: a beam of fading rings. */
	BREATH_ROLLING_FLAME, /* Rolling Flame: a ball rolls to the target. */
	BREATH_PIKE,		  /* Ghost Soldier and Pike Assault. */
	BREATH_BLAST,		  /* Blast Breath: a blast and a jet. */
} BREATH_KIND;

/* The rushing strike's sparks' palettes (btEffectRunRushingStrike), each */
/* named for the effect that first plays it; Bone Charge plays Stun Muscle's. */
typedef enum RUSHING_STRIKE_MODE {
	/* Unused: no wrapper passes it; the fire pillar's colours. */
	RUSHING_STRIKE_UNUSED,
	RUSHING_STRIKE_BRUTE_FORCE,
	RUSHING_STRIKE_ONSLAUGHT,
	RUSHING_STRIKE_HEADBUTT,
	RUSHING_STRIKE_STUN_MUSCLE,
	RUSHING_STRIKE_LIVING_ARMOR
} RUSHING_STRIKE_MODE;

/* The mirrored wall's modes (btEffectRunMirroredWall), each named for the */
/* effect that plays it. */
typedef enum MIRRORED_WALL_MODE {
	MIRRORED_WALL_SPASM, /* actual size, a gentle shake for 32 frames */
	/* A ninth wider than Spasm's, a hard shake for 8 frames. */
	MIRRORED_WALL_BERSERK,
	/* A ninth wider than Spasm's, no shake, in the Fireball colours. */
	MIRRORED_WALL_RECOVERY,
	MIRRORED_WALL_SACK
} MIRRORED_WALL_MODE;

/* Where btEffectRunSparkGroups' bursts go off. */
typedef enum SPARK_GROUPS_MODE {
	SPARK_GROUPS_NOVA,	   /* Nova's */
	SPARK_GROUPS_DYNAMITE, /* Dynamite's, orange */
	SPARK_GROUPS_RISING /* below the user, in the LightningBolt colours, the */
						/* sparks rising */
} SPARK_GROUPS_MODE;

/* The declarations the parts share. */

struct SPR_ANIMATION;
struct BATTLE_EFFECT_PARM;
struct SCR_POINT;
struct PARTICLE;

/* A mesh triangle: its three vertex numbers and the texture coordinates of */
/* each corner. A strip quad is two of them; a triangle whose three vertices */
/* are 0 ends a list. */
typedef struct MESH_TRIANGLE {
	u8 vertex[3];
	u8 unusedA; /* nothing reads or writes it */
	u8 uv[3][2];
	u8 unusedB[2]; /* nothing reads or writes them */
} MESH_TRIANGLE;

struct SPR_SCALE;
struct STRIP_QUAD;

/* The parts' functions that another part calls before their definition, by */
/* the part that first calls them. */

/* display.c */
void btEffectAdvanceScrollOnInterval(void);
void btEffectConfigureEffectDisp(void);
void btEffectAdvanceBg2OnInterval(void);

/* canvas.c */
void btEffectStartCanvasLayer(s32 bgControl);
/* Closes the canvas layer. */
void btEffectEndCanvasLayer(void);
/* Counts down the actors' flash timers. */
void btEffectCountDownBattlerTints(void);
void btEffectStartTiledCanvas(s32 bgControl);
void btEffectStartTiledCanvasFilled(s32 bgControl);
void btEffectProcessPendingBlock2Transfer(void);
void btDispApplyBg2Affine(void);
/* Sets up the canvas tile map on BG1. */
void btEffectSetupCanvasTileMap(void);
/* Marks the transition done and shows the battle view again. */
void btEffectHideBg1AndRestoreBackdrop(void);
void btEffectTransferFullScrImage(void);
void btEffectDrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void btEffectBuildCanvasTilemap(void);
/* The 64x64 stamp Haures draws in 256 colours: the VRAM address of its */
/* sprite's tiles, two words before the state's end, past what struct */
/* BATTLE_EFFECT_STATE names, and the picture drawn for them, the 4 KB before */
/* transferMode. */
#define STAMP_TILES(state)                                                     \
	(*(u32 *)((u8 *)(state) + BATTLE_EFFECT_STATE_SIZE - 2 * sizeof(u32)))
#define STAMP_CANVAS(state)                                                    \
	((u8 *)(state) + offsetof(BATTLE_EFFECT_STATE, transferMode) -             \
	 OBJ_64X64_8BPP_BYTES)
void btEffectProcessPendingStampTransfer(void);

/* djinn_beams.c */
/* Fetches the max and additive blitters, flipped or not. */
void btEffectBuildCanvasBlitters(s32 flip, DRAW_RECTANGLE *out);

/* prism.c */
void btEffectUpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
								s32 bluePhase);

/* djinn_heal.c */
/* state->cameraPhase, which btCameraApplyPhasedDelta and the effects */
/* that turn the camera through it (Fire, Froth) pass between them. */
typedef enum CAMERA_PHASE {
	CAMERA_PHASE_IDLE,		 /* nothing marked since the last turn */
	CAMERA_PHASE_FRAME_DONE, /* the effect has finished a frame: turn the */
							 /* whole delta */
	CAMERA_PHASE_HALF_TURNED /* half a delta turned on an unmarked frame */
} CAMERA_PHASE;
void btCameraApplyPhasedDelta(void);

/* dispatch.c */
void btEffectShowLivingTargets(BATTLE_EFFECT_PARM *args);
void btEffectCallDoNothing(s32 cnt);
void btEffectBuildWindowEdgeTable(void);

/* battler_tint.c */
/* Shows a battler in a palette and a pose, for `frames` frames counted in */
/* timer slot `timerSlot`; TINT_KEEP leaves any of them as it is. */
void btEffectTintBattler(s32 unitId, s32 palette, s32 pose, s32 timerSlot,
						 s32 frames);
void btEffectSetupBlendedDisp(void);

/* stat_change.c */
void btEffectRunImpair(BATTLE_EFFECT_PARM *args);
void btEffectRunDull(BATTLE_EFFECT_PARM *args);
void btEffectRunWeaken(BATTLE_EFFECT_PARM *args);

/* eruption.c */
void btEffectCreateSprs(s32 count, s32 spriteId, u32 priority);

/* projectiles.c */
void btParticleAdvanceWithGravity3D(PARTICLE *step, s32 damping, s32 gravity);
void btParticleAdvanceWithGravity2D(PARTICLE *step, s32 damping, s32 gravity);
void btEffectRunDjinniEntrance(BATTLE_EFFECT_PARM *effect, u32 packed,
							   s32 *screen, s32 *world);
void btEffectSetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

/* djinn_attacks.c */
void btResourceLoadAndDecompress(s32 id, void *dst, s32 skipPalette,
								 s32 copyPalette);

/* hail.c */
/* Shakes the camera while state->shakeFrm lasts. */
void btCameraShake(s32 rangeX, u32 rangeY);
void btCameraApplyVerticalShake(s32 range);

/* gaia.c */
SPR_ANIMATION *btAnimationObjectAllocateEmpty(void);

/* spider_web.c */
void btEffectStartActorMotion(s32 id, s32 force, s32 acceleration,
							  s32 speedLimit, s32 velocityY, s32 percent);
void btRenderBuildQuadStrip(struct STRIP_QUAD *quad, s32 width, s32 height,
							s32 count);
void btEffectRunSwirlSwipe(BATTLE_EFFECT_PARM *effect, s32 mode);
void btPaletteMirrorBgCols(void);
void btGraphicsLoadGreyRamp(void);

/* azul.c */
void btEffectPosProjectBattler(s32 id, SCR_POINT *position);
/* The middle between the first and the last target. */
void btEffectGetTargetsMidpoint(SCR_POINT *position);
void btMeshBuildGrid(MESH_TRIANGLE *triangles, s32 columns, s32 rows,
					 s32 cellWidth, s32 cellHeight);
void syColBufferScaleStub(void *source, void *destination, s32 scale,
						  s32 count);
void btEffectPosProjectSpr(s32 id, SCR_POINT *position);

/* positions.c */
void btEffectRunCastingImpact(BATTLE_EFFECT_PARM *action, s32 kind);
void btEffectRunCriticalHit(BATTLE_EFFECT_PARM *effect);
void btEffectRunAttackSlash(BATTLE_EFFECT_PARM *effect);

/* casting_impact.c */
/* Steps the background colours one shade toward a resource's palette. */
void btEffectStepPaletteToResource(s32 resource);

/* canvas_wipe.c */
void btEffectWipeCanvas(s32 direction, s32 fill);

/* particle_streams.c */
void btEffectPlaceFormationSprs(s32 formation, s32 x, s32 z);
void btEffectRunImpactBurst(s32 formation, s32 x, FIXED y);
void btEffectArmPaletteHBlankDma(void);
void btRenderApplyProjectedPlacementStub(SPR *sprite, s32 *position, s32 *scale,
										 s32 mode);

/* ramses.c */
/* An actor's recoil motion, through a stub call. */
void btMotionApplyKnockbackStub(s32 actor, s32 motion);

/* ragnarok.c */
void btEffectRunFallingSword(BATTLE_EFFECT_PARM *effect, s32 mode);

/* thrash.c */
void btEffectRunFlashingStrike(BATTLE_EFFECT_PARM *effect, s32 variant);

/* undead_sword.c */
void btPictureMirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void btEffectRunWaterSheet(BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectRunWindFunnel(BATTLE_EFFECT_PARM *effect, s32 mode);

/* heavy_divide.c */
void btEffectRunHeavyBlow(BATTLE_EFFECT_PARM *effect, s32 mode);

/* thunder_mine.c */
void btEffectRunBallLightning(BATTLE_EFFECT_PARM *effect, s32 mode);

/* whorl.c */
void btEffectRunTwistingColumn(BATTLE_EFFECT_PARM *effect, u32 mode);
void btEffectRunDjinniCharge(BATTLE_EFFECT_PARM *effect, s32 mode);

/* rime.c */
void btEffectRunDjinniSign(BATTLE_EFFECT_PARM *effect, s32 mode);

/* gasp.c */
void btEffectRunGaspFamily(BATTLE_EFFECT_PARM *effect, s32 mode);

/* raging_heat.c */
void btEffectFillWhiteBand(void);

/* star_mine.c */
void btEffectBuildRepeatingTilemap(void);

/* rushing_strike.c */
void btEffectRunRushingStrike(BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectRunMirroredWall(BATTLE_EFFECT_PARM *effect, s32 mode);

/* vengeance.c */
void btEffectRunGeyser(BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectRunStarSwarm(BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectRunBubbleColumn(BATTLE_EFFECT_PARM *effect);

/* legend.c */
void btEffectRunElementCards(BATTLE_EFFECT_PARM *effect, s32 mode);

/* cards.c */
void btEffectRunCards(BATTLE_EFFECT_PARM *effect, s32 mode);

/* breaths.c */
void btCanvasBlitMirroredQuarters(void *source, s32 x, s32 y, s32 size);
void btEffectRunBreaths(BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectRunCrescentSlash(BATTLE_EFFECT_PARM *effect, s32 mode);

/* trident.c */
void btEffectRunLeapingStrike(BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectArmMeteorBlowPaletteDma(void);

/* liquifier.c */
void btEffectRunBallAttack(BATTLE_EFFECT_PARM *effect);

/* odyssey.c */
/* The five-argument blitter builder's stub call, which only a call in */
/* odyssey.c that is never taken names. */
s32 btEffectBuildBlitterStub(s32 slot, s32 widthShift, s32 heightShift,
							 s32 flags, u32 mode);
void btIrisWipeScr(u8 side, u8 speed, void (*midpoint)(void));
void btIrisRestoreBattle(void);
void btEffectBuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats,
									 s32 scale);

/* moloch.c */
RENDER_MODEL *btRenderAllocateModelList(s32 count);
void btRenderTransformVertices(const void *corners, void *vertices, s32 count);

/* target_motes.c */
/* Glowing orbs spiral in on each target. */
/* */
/* Abilities: Mystic Flame. */
void btEffectRunMysticFlame(BATTLE_EFFECT_PARM *args);

/* Functions of other modules the parts call. */
s32 fdSprStartAnimationStub(void *object, s32 value);
void btActorSpawnObjectsForListStub(s16 *list, s32 mode);
void btBackgroundLoadStub(s32 mode, s32 resource, s32 level);
void btBackgroundSetModeStub(s32 mode, s32 resource, s32 speed);
void btBackgroundSetPaletteLevelStub(s32 background, s32 level);
/* Turns the battle camera, over `frames` frames, to look at the point between
 */
/* two units. */
void btCameraSetViewAtPairMidpointStub(s32 actor, s32 target, s32 frames);
void btCameraSetViewStub(s32 x, s32 depth, s32 y, s32 zoom);
void btObjectPutStub(void);
/* Spawns the battle objects of a list of units through its stub call. */
void btObjectSpawnObjectsForListStub(s16 *unitIds, s32 refreshPoses,
									 s32 releaseOthers);
/* btPlaybackStart through its stub call. */
void btPlaybackStartStub(s32 phase);
/* Whether an enemy's sprite takes the heavy impact, through its stub call. */
s32 btSprHasHeavyImpactStub(s32 enemyId);
s32 btUnitGetEffectHeightStub(s32 unit);
void *fdActorGetSprStub(void *object, s32 index);
void btObjectDestroySprsStub(s32 actor);
void fdSprRemoveAnimationStub(void *record, s32 entry);
void fdActorShadowLoadTilesStub(void);
void fdActorShadowFreeTilesStub(void);

/* The tables of effect_table.c, in its order. */

/* Where each size of mote and of flare starts in its sprite sheet, in bytes. */
extern u16 BattleEffect_MotePictures[];
extern u16 BattleEffect_FlarePictures[];
/* Where each of Cruel Ruin's circles starts in its sprite sheet. */
extern const u16 CruelRuin_CirclePictures[];
/* The pictures several effects share: the sparkles, the ember puff that rises
 */
/* in six pictures, and the six glints, with their sizes and offsets. */
extern u16 BattleEffect_SparklePictures[];
extern u8 BattleEffect_SparkleSizes[];
extern u8 BattleEffect_EmberStreakPictureWidths[];
extern u8 BattleEffect_EmberStreakPictureHeights[];
extern u8 BattleEffect_EmberStreakPictureBiasY[];
extern u16 BattleEffect_EmberStreakPictures[];
extern u16 BattleEffect_GlintPictures[];
extern u8 BattleEffect_GlintPictureWidths[];
extern u8 BattleEffect_GlintPictureHeights[];
/* Spire's shard pictures. */
extern u8 Spire_ShardWidths[];
extern u8 Spire_ShardHeights[];
extern u16 Spire_ShardPictures[];
/* The stat change's shard pictures. */
extern u16 StatChange_ShardPictures[];
extern u8 StatChange_ShardWidths[];
extern u8 StatChange_ShardHeights[];
/* Where the Death Card's Reaper draws its seven panels, seven x for each side.
 */
extern u8 Reaper_PictureX[];
extern u8 Reaper_PictureY[];
/* Ice's shards and frames, and Douse's drops, by variant. */
extern u8 Ice_Counts[];
extern u8 Douse_Counts[];
/* The streaks' styles, and the struck target's tremble. */
extern s8 Streak_Styles[];
extern s8 Streak_Tremble[];
/* The target strikes: their styles (TARGET_STRIKE_COLUMN), the slash */
/* pictures and how far each reaches, the bite's stages and the glints' flips.
 */
extern u8 TargetStrike_Styles[][7];
extern u8 TargetStrike_Widths[];
extern u8 TargetStrike_Heights[];
extern u16 TargetStrike_Pictures[];
extern u8 TargetStrike_Reach[];
extern u8 TargetStrike_BiteStages[];
extern u8 TargetStrike_GlintDrawFlags[];
/* The 33 pieces the screen shatters into as Death Plunge opens: their pictures
 */
/* and where they start. */
extern u8 ShatteringOpening_PieceWidths[];
extern u8 ShatteringOpening_PieceHeights[];
extern u16 ShatteringOpening_PiecePictures[];
extern u8 ShatteringOpening_PieceStartX[];
extern u8 ShatteringOpening_PieceStartY[];
/* The element burst's four corner sparks: their places and flips. */
extern s8 ElementBurst_SparkX[];
extern s8 ElementBurst_SparkY[];
extern u8 ElementBurst_GlintFlips[];
/* Cruel Ruin: the streak's corners, the discs' turns, the pillars, the blasts,
 */
/* and each streak's start, size, path and roll. */
extern const MODEL_VERTEX CruelRuin_StreakCorners[];
extern const u16 CruelRuin_DiscPitches[];
extern const u16 CruelRuin_DiscYaws[];
extern const u8 CruelRuin_PillarSways[];
extern const u8 CruelRuin_PillarWidths[];
extern const u8 CruelRuin_BlastFrames[];
extern const u8 CruelRuin_BlastSizes[];
extern const u8 CruelRuin_BlastX[];
extern const u8 CruelRuin_BlastY[];
extern const u8 CruelRuin_StreakStarts[];
extern const u8 CruelRuin_StreakSizes[];
extern const u8 CruelRuin_StreakX[];
extern const u8 CruelRuin_StreakStepX[];
extern const u8 CruelRuin_StreakStepY[];
extern const u8 CruelRuin_StreakEndY[];
extern const s32 CruelRuin_StreakRolls[];
/* Eclipse: the glints' flips, the dragon's sprites that take new tiles as it */
/* turns and their places, the canvas's fills, and the rings. */
extern const u8 Eclipse_GlintFlips[];
extern const u8 Eclipse_TileObjects[];
extern const u8 Eclipse_FillFrames[];
extern const u32 Eclipse_FillLevels[];
extern const u8 Eclipse_ObjectX[];
extern const u8 Eclipse_ObjectZ[];
extern const u8 Eclipse_RingGrowth[];
extern const s8 Eclipse_RingHeights[];
/* The form change's four waves: when each starts, how fast it grows and how */
/* far it is stretched. */
extern u8 FormChange_WaveStarts[];
extern u8 FormChange_WaveGrowth[];
extern u8 FormChange_WaveStretch[];
/* Prism's crystal and the shards it breaks into: their places and pictures. */
extern u8 Prism_ShardStarts[];
extern u8 Prism_ShardWidths[];
extern u8 Prism_ShardHeights[];
extern s32 Prism_ShardPictures[];
/* The gathering lights' motes for each target, and the frames before the */
/* targets' share, by mode. */
extern u8 GatheringLights_Counts[];
/* The Djinn beams: the motes' pictures as they sway, and the beam head's three
 */
/* pictures. */
extern u8 DjinnBeams_MotePictures[];
extern u16 DjinnBeams_HeadPictures[];
extern u8 DjinnBeams_HeadWidths[];
extern u8 DjinnBeams_HeadHeights[];
/* The lobbed shells: each style's record (LOBBED_SHELLS_SHATTER and the rest),
 */
/* and the shards' pictures. */
extern s8 LobbedShells_Styles[];
extern u8 LobbedShells_ShardWidths[];
extern u8 LobbedShells_ShardHeights[];
extern u16 LobbedShells_ShardPictures[];
/* Where Condemn draws its seven pictures of the reaper and his crescent, by */
/* side. */
extern u8 Condemn_PictureX[];
extern u8 Condemn_PictureY[];
/* Bind's triangles: the first corner of each. */
extern s32 Bind_Vertex[];
extern s32 Bind_TriangleStrikeVertex[];
/* The star's ring: its points and radii, alternating. */
extern s32 Star_Points[];
extern u8 Star_Radii[];
/* Procne: the grid her nine sprites sit on, the four strikes' frames and a */
/* strike's three pictures. */
extern u8 Procne_GridX[];
extern u8 Procne_GridY[];
extern u16 Procne_Frames[];
extern u8 Procne_PictureWidths[];
extern u8 Procne_PictureHeights[];
extern u8 Procne_PictureBiasY[];
extern u16 Procne_Pictures[];
/* Tiamat: the embers' speeds and sizes, a spark's two pixels, and the flame */
/* streak's six pictures. */
extern u8 Tiamat_EmberSpeeds[];
extern u8 Tiamat_SparkPixels[];
extern u8 Tiamat_EmberSizes[];
extern u8 Tiamat_EmberStreakWidths[];
extern u8 Tiamat_EmberStreakHeights[];
extern u8 Tiamat_EmberStreakBiasY[];
extern u16 Tiamat_EmberStreakPictures[];
/* Nereid: the three water columns' feet and the falling sparks' gravity. */
extern u8 Nereid_Columns[];
extern s32 Nereid_Gravity[];
/* The vortices: Froth's counts and the whirlwind's shapes, by variant (the */
/* FROTH_ and WHIRLWIND_ columns). */
extern u8 Vortices_Counts[][2];
extern u8 Vortices_Shapes[][3];
/* Prism's crystals, frames and damage frame, by variant. */
extern u8 Prism_Counts[];
/* Plasma: the sparks and shake of each lightning pillar, the columns, and the
 */
/* counts, by variant. */
extern u16 Plasma_Sparks[];
extern u8 Plasma_Columns[];
extern u8 Plasma_Counts[];
/* The spark groups' shapes, by variant: sparks in a group, groups and each */
/* group's x. */
extern u16 SparkGroups_Shapes[][5];
/* Plasma's flash burst picture for each three frames of a flash. */
extern u8 Plasma_FlashPictures[];
/* Volcano's pillars of fire: how many, their depths and when each erupts. */
extern u8 Volcano_Counts[];
extern s8 Volcano_Depths[][4];
extern u8 Volcano_StartFrames[];
/* The target motes' glint flips. */
extern u8 TargetMotes_GlintFlips[];
/* Zagan: where his sprites sit as he charges and as he gallops, and his dust */
/* rings. */
extern const u16 Zagan_ChargePoints[][2];
extern const u16 Zagan_GallopPoints[][2];
extern const u8 Zagan_RingStarts[];
extern const u8 Zagan_RingGrowth[];
extern const s8 Zagan_RingHeights[];
/* Cybele: her sprites' poses, and the pictures of her thorns, flowers, chips,
 */
/* sparks and blasts. */
extern u8 Cybele_ObjectPoses[];
extern u16 Cybele_ThornPictures[];
extern u8 Cybele_ThornWidths[];
extern u8 Cybele_ThornHeights[];
extern u16 Cybele_FlowerPictures[];
extern u8 Cybele_FlowerSizes[];
extern u16 Cybele_ChipPictures[];
extern u8 Cybele_ChipWidths[];
extern u8 Cybele_ChipHeights[];
extern u16 Cybele_SparkPictures[];
extern u8 Cybele_SparkSizes[];
extern u16 Cybele_BlastPictures[];
extern u16 Cybele_BlastSizes[];
/* Boreas: the crystals' points, the shake, his sprites' animations, and the */
/* pictures of the crystals, debris, spray, rising crystals and falls. */
extern u8 Boreas_Points[][2];
extern s8 Boreas_ShakeOffsets[];
extern u16 Boreas_CrystalPictures[];
extern u8 Boreas_CrystalWidths[];
extern u8 Boreas_CrystalHeights[];
extern u8 Boreas_Animations[];
extern u8 Boreas_DebrisWidths[];
extern u8 Boreas_DebrisHeights[];
extern u16 Boreas_DebrisPictures[];
extern u8 Boreas_SprayWidths[];
extern u8 Boreas_SprayHeights[];
extern u16 Boreas_SprayPictures[];
extern u16 Boreas_RisingCrystalPictures[];
extern u8 Boreas_RisingCrystalWidths[];
extern u8 Boreas_RisingCrystalHeights[];
extern u8 Boreas_FallWidths[];
extern u8 Boreas_FallHeights[];
extern u16 Boreas_FallPictures[];
/* Drain's motes: their gravities, and their size by mode. */
extern s32 Drain_Gravities[];
extern u8 DrainMotes_Sizes[];
/* Quake's frames, rise and shake, by variant. */
extern u8 Quake_Timings[][3];
/* Fire's counts by variant, and its flare pictures. */
extern u8 Fire_Counts[];
extern u8 Fire_EmberStreakWidths[];
extern u8 Fire_EmberStreakHeights[];
extern u8 Fire_EmberStreakBiasY[];
extern u16 Fire_EmberStreakPictures[];
/* Frost: where the mounds rise, its timings by variant, and the pictures of */
/* its motes, mounds and ice chips. */
extern u8 Frost_MoundSpots[];
extern u8 Frost_Timings[];
extern u8 Frost_MoteWidths[];
extern u8 Frost_MoteHeights[];
extern u16 Frost_MoteCells[];
extern u16 Frost_MoundPictures[];
extern u8 Frost_MoundHeights[];
extern u8 Frost_ChipWidths[];
extern u8 Frost_ChipHeights[];
extern u16 Frost_ChipCells[];
/* The sparks each of Ray's bolts throws a frame, by variant. */
extern u8 Ray_Sparks[][2];
/* Eruption's counts by variant, and its puffs' pictures. */
extern u8 Eruption_Counts[][3];
extern u16 Eruption_PuffPictures[];
extern u16 Eruption_PuffSizes[];
/* The burst shower's glint flips. */
extern u8 BurstShower_GlintFlips[];
/* Atalanta's sprites' scale, one to one. */
extern s32 Atalanta_UnitScale[];
/* Growth: the vines' and pillars' pictures, the columns by strength and where
 */
/* they root, how tall they grow, and the dust puffs. */
extern u8 Growth_VineWidths[];
extern u8 Growth_HeightLimits[];
extern u16 Growth_VinePictures[];
extern u8 Growth_PillarWidths[];
extern u16 Growth_PillarPictures[];
extern u8 Growth_ColumnCounts[];
extern u8 Growth_ColumnX[];
extern s8 Growth_PillarHeights[];
extern s8 Growth_PuffWidths[];
extern u8 Growth_PuffHeights[];
extern u16 Growth_PuffPictures[];
/* Each vine's x about the middle of the targets. */
extern const s8 Vine_ColumnOffsets[];
/* Each thorn's kind (THORN_MIRRORED, THORN_SMALL). */
extern u8 Thorn_Kinds[];
/* A thorn's kind in Thorn_Kinds. */
#define THORN_MIRRORED 1 /* drawn mirrored across */
#define THORN_SMALL	   2 /* 32 pixels wide, not 48 */
/* The thorns: where each roots, how many by strength, and the dust puffs as */
/* each bursts out. */
extern s8 Thorn_X[];
extern u8 Thorn_Counts[];
extern s8 Thorn_PuffWidths[];
extern u8 Thorn_PuffHeights[];
extern u16 Thorn_PuffPictures[];
/* Bolt: its sparks, glints, spread and bolts by strength, and how its glints */
/* are flipped and blended. */
extern u8 Bolt_Counts[];
extern u8 Bolt_GlintFlips[];
extern u8 Bolt_GlintModes[];
/* The projectile volley: the comets' heights, each mode's record (enum */
/* VOLLEY_FIELD), the hit flash's pictures and spread, the pellets' pictures and
 */
/* the glints' flips. */
extern u8 ProjectileVolley_CometHeights[];
extern u8 ProjectileVolley_Modes[][PROJECTILE_VOLLEY_COLUMNS];
extern u8 ProjectileVolley_FlashWidths[];
extern u8 ProjectileVolley_FlashHeights[];
extern u8 ProjectileVolley_FlashSteps[];
extern u8 ProjectileVolley_FlashRises[];
extern u16 ProjectileVolley_FlashPictures[];
extern u16 ProjectileVolley_PelletPictures[];
extern u8 ProjectileVolley_GlintFlips[];
/* The frames the Djinni throws its five flames on. */
extern const u8 DjinnAttacks_LaunchFrames[];
/* Hail's four ice chip pictures. */
extern u8 Hail_PictureWidths[];
extern u8 Hail_PictureHeights[];
extern u16 Hail_Pictures[];
/* Ground: the crackles' flips, and the portals' pictures and places. */
extern const u8 Ground_CrackleFlips[];
extern const u8 Ground_PortalWidths[];
extern const u8 Ground_PortalHeights[];
extern const u16 Ground_PortalPictures[];
extern const u8 Ground_PortalX[];
extern const u8 Ground_PortalY[];
/* Thor: the palettes a struck target flashes in, and the crackles' flips. */
extern const u8 Thor_TargetPalettes[];
extern const u8 Thor_CrackleFlips[];
/* Spire: where each shard sits in the boulder, the boulders' x and drop */
/* frames, and how many by variant. */
extern u8 Spire_ShardOffsets[];
extern s8 Spire_RockX[];
extern u8 Spire_DropFrames[];
extern u8 Spire_RockCounts[];
/* The flurries: their styles (FLURRY_COLUMN) and the glints' flips. */
extern u8 Flurry_Styles[][8];
extern const u8 Flurry_GlintFlips[];
/* Gaia: the growing wall's rows, the flame's pictures, the wall layer's shift
 */
/* and the late rocks. */
extern u16 Gaia_RowPictures[];
extern u8 Gaia_RowWidths[];
extern u16 Gaia_FlamePictures[];
extern u8 Gaia_FlameWidths[];
extern u8 Gaia_FlameHeights[];
extern s8 Gaia_Bg2Shifts[][3];
extern u8 Gaia_LateRockColumns[];
/* Ulysses: where the mage's sprites sit, the summon's pictures, and where its
 */
/* six particles start. */
extern const u8 Ulysses_ObjectX[];
extern const u8 Ulysses_ObjectZ[];
extern const u8 Ulysses_PictureWidths[];
extern const u8 Ulysses_PictureHeights[];
extern const u16 Ulysses_Pictures[];
extern const s8 Ulysses_ParticleStarts[][5];
/* The swirl swipe's six pictures: their sizes, pictures, x and rise. */
extern const u8 SwirlSwipe_PictureWidths[];
extern const u8 SwirlSwipe_PictureHeights[];
extern const u16 SwirlSwipe_Pictures[];
extern const u8 SwirlSwipe_PictureX[];
extern const s8 SwirlSwipe_PictureBiasY[];
/* Azul: the points of the battlefield's shattered parts, and the serpent's */
/* sprites, early and late. */
extern const u8 Azul_PartPoints[][2];
extern const SCALE Azul_UnitScale;
extern const u8 Azul_ObjectX[];
extern const u8 Azul_ObjectZ[];
extern const SCALE Azul_LateUnitScale;
extern const s8 Azul_LateObjectX[];
extern const s8 Azul_LateObjectZ[];
/* The plain attacks' beam: its six pictures and how far each reaches. */
extern u8 Beam_PictureWidths[];
extern u8 Beam_PictureHeights[];
extern u16 Beam_Pictures[];
extern u8 Beam_PictureReaches[];
/* The casting impacts: glint flips, the bite's pictures, Raiden's Wrath's and
 */
/* Apocalypse's pictures and places, the ice chips, and Stun Jip's bands and */
/* Armor Crush's waves. */
extern const u8 CastingImpact_SquallGlintFlips[];
extern const u8 CastingImpact_SapGlintFlips[];
extern const u8 CastingImpact_BitePictures[];
extern const u16 CastingImpact_RaidensWrathPictures[];
extern const u16 CastingImpact_RaidensWrathWidths[];
extern const u16 CastingImpact_RaidensWrathHeights[];
extern u16 CastingImpact_RaidensWrathX[];
extern u16 CastingImpact_RaidensWrathY[];
extern const u16 CastingImpact_ApocalypsePictures[];
extern const u16 CastingImpact_ApocalypseWidths[];
extern const u16 CastingImpact_ApocalypseHeights[];
extern const u16 CastingImpact_ApocalypseX[];
extern u16 CastingImpact_ApocalypseY[];
extern const u16 CastingImpact_ApocalypseBurstPictures[];
extern const u8 CastingImpact_ApocalypseBurstWidths[];
extern const u8 CastingImpact_ApocalypseBurstHeights[];
extern u8 CastingImpact_ApocalypseBurstPoints[][2];
extern const u16 CastingImpact_IceChipPictures[];
extern const u8 CastingImpact_StunBoltGlintFlips[];
extern const u8 CastingImpact_StunJipBandStarts[];
extern const u8 CastingImpact_StunJipBandGrowth[];
extern const s8 CastingImpact_StunJipBandHeights[];
extern const u8 CastingImpact_ArmorCrushWaveStarts[];
extern const u8 CastingImpact_ArmorCrushWaveGrowth[];
extern const u8 CastingImpact_ArmorCrushWaveStretch[];
/* The unused copy of Nereid: its columns and gravity. */
extern u8 NereidCopy_Columns[];
extern const s32 NereidCopy_Gravity[];
/* The summons' sprite formations, each sprite's x and depth: Nereid's, */
/* Cybele's, Tiamat's two and Megaera's. */
extern const u8 SummonFormation_NereidX[];
extern const u8 SummonFormation_NereidZ[];
extern const u8 SummonFormation_CybeleX[];
extern const u8 SummonFormation_CybeleZ[];
extern const u8 SummonFormation_TiamatAlternateX[];
extern const u8 SummonFormation_TiamatAlternateZ[];
extern const u8 SummonFormation_TiamatX[];
extern const u8 SummonFormation_TiamatZ[];
extern const s8 SummonFormation_MegaeraX[];
extern const u8 SummonFormation_MegaeraZ[];
/* The impact burst's pictures. */
extern u8 ImpactBurst_PictureWidths[];
extern u8 ImpactBurst_PictureHeights[];
extern u16 ImpactBurst_Pictures[];
/* The starfield fall: the orbiting sparks, the fireballs and the flash */
/* pictures. */
extern s16 StarfieldFall_OrbitPoints[][3];
extern s16 StarfieldFall_FireballPoints[][2];
extern u16 StarfieldFall_FlashPictures[];
extern u16 StarfieldFall_FlashSizes[];
/* Ramses: where each of the statue's sprites sits, and the puffs' pictures. */
extern u8 Ramses_StatueColumns[];
extern u8 Ramses_StatueRows[];
extern u16 Ramses_PuffPictures[];
extern u16 Ramses_PuffSizes[];
/* Ragnarok: the strikes' columns and the flash's pictures. */
extern u8 Ragnarok_StrikeColumns[];
extern u8 Ragnarok_FlashPictures[];
/* Titan Blade: the flash's pictures and the dust's gravities. */
extern u8 TitanBlade_FlashPictures[];
extern s32 TitanBlade_DustGravity[];
/* Judgment: the sprites its shield is copied into, and the shards' pictures. */
extern const u8 Judgment_ShieldSprites[];
extern const u16 Judgment_ShardPictures[];
extern const u8 Judgment_ShardWidths[];
extern const u8 Judgment_ShardHeights[];
/* Kirin: the row's x offsets and heights. */
extern u8 Kirin_Columns[];
extern u8 Kirin_Rows[];
/* Neptune's spray: its four gravities. */
extern s32 Neptune_Gravity[];
/* Undead Sword's sparks: their pictures and sizes. */
extern const u16 UndeadSword_PuffPictures[];
extern const u16 UndeadSword_PuffSizes[];
/* The wind funnels, by mode. */
extern const u8 WindFunnel_Counts[];
/* Diamond Berg: its ripples, and its flashes' roll and pitch. */
extern const u8 DiamondBerg_RingStarts[4];
extern const u8 DiamondBerg_RingGrowth[4];
extern const s8 DiamondBerg_RingHeights[4];
extern const s16 DiamondBerg_ShardRolls[8];
extern const s16 DiamondBerg_ShardPitches[8];
/* Geode's rings: when each starts, how fast it grows, and its height. */
extern const u8 Geode_WaveStarts[];
extern const u8 Geode_WaveGrowth[];
extern const s8 Geode_WaveHeights[];
/* Soul Shatter's sparks: their pictures and sizes. */
extern u16 SoulShatter_SparkPictures[];
extern u16 SoulShatter_SparkSizes[];
/* Serac: the spire's rings, and the ripples of mist. */
extern const s8 Serac_RowHeights[];
extern const s8 Serac_RowRadii[];
extern const u8 Serac_RingStarts[];
extern const u8 Serac_RingGrowth[];
extern const s8 Serac_RingHeights[];
/* The heavy blow: its strike frame and chips, by mode, and a chip's two */
/* pixels. */
extern u8 HeavyBlow_StrikeFrames[];
extern u8 HeavyBlow_ChipCounts[];
extern const u8 HeavyBlow_ChipPixels[];
/* Cool: its columns of ice and its sparks. */
extern s16 Cool_ColumnX[];
extern u8 Cool_ColumnStarts[];
extern u8 Cool_ColumnHeights[];
extern u8 Cool_ColumnLeans[];
extern u16 Cool_SparkPictures[];
extern u16 Cool_SparkSizes[];
/* Wyrd Curse's drops: their bounce speeds. */
extern const s8 WyrdCurse_DropBounces[];
/* The ball lightning's glint flips. */
extern const u8 BoltTrail_GlintFlips[];
/* Megaera's glints: their flips and how they are blended, by variant. */
extern const u8 Megaera_GlintFlips[];
extern const u8 Megaera_GlintModes[];
/* Light Surge's four rings: their full sizes, shades and sways. */
extern const s32 LightSurge_RingSizes[4];
extern const s8 LightSurge_RingShades[4];
extern s8 LightSurge_RingSways[4];
/* The Djinni charge: the Djinni's entrance by mode, and the debris's */
/* gravities. */
extern const u8 DjinniCharge_EntranceFlags[];
extern const s32 DjinniCharge_DebrisGravity[4];
/* The Gasp family: the streak's steps and mesh, and the counts by variant. */
extern const s8 GaspFamily_StreakSteps[];
extern const u16 GaspFamily_StreakMesh[];
extern const u8 GaspFamily_Counts[];
/* The frames Charon's canvas flashes. */
extern u8 Charon_FlashFrames[];
/* Models several effects share: a band's and a cylinder's corners. */
extern const MODEL_VERTEX BattleEffect_OctagonBandCorners[];
extern const MODEL_VERTEX BattleEffect_CylinderCorners[];
/* The casting impacts' band mesh. */
extern const u16 CastingImpact_BandMesh[];
/* The geyser's mesh. */
extern const u16 Geyser_Mesh[];
/* The cylinder's mesh, the ripples and rings several effects spread. */
extern const u16 BattleEffect_CylinderMesh[];
/* Geode's ring's corners. */
extern const MODEL_VERTEX Geode_RingCorners[];
/* Models several effects share: a ring, a triangle, beams and squares, their */
/* corners and meshes. */
extern const MODEL_VERTEX BattleEffect_RingCorners[];
extern const u16 BattleEffect_RingMesh[];
extern const u16 BattleEffect_TriangleMesh[];
extern const MODEL_VERTEX BattleEffect_TriangleCorners[];
extern const MODEL_VERTEX BattleEffect_TaperedBeamCorners[];
extern const MODEL_VERTEX BattleEffect_BeamCorners[];
extern const MODEL_VERTEX BattleEffect_LongBeamCorners[];
extern const MODEL_VERTEX BattleEffect_SquareCorners[];
extern const MODEL_VERTEX BattleEffect_StandingSquareCorners[];
/* A flame of Legend's fan: its corners. */
extern MODEL_VERTEX Legend_FanFlameCorners[];
/* Models several effects share: a square on the ground, and the meshes of */
/* small squares and a column. */
extern const MODEL_VERTEX BattleEffect_GroundSquareCorners[];
extern const u16 BattleEffect_SmallSquareMesh[];
extern const u16 BattleEffect_SmallSquareMeshReversed[];
extern const u16 BattleEffect_ColumnMesh[];
/* The heavy blow's sword mesh. */
extern const u16 HeavyBlow_SwordMesh[];
/* A tall quad's mesh. */
extern const u16 BattleEffect_TallMesh[];
/* Haures's band mesh. */
extern const u16 Haures_BandMesh[];
/* Catastrophe's burst mesh. */
extern const u16 Catastrophe_ForkMesh[];
/* The square meshes several effects share. */
extern const u16 BattleEffect_SquareMesh[];
extern const u16 BattleEffect_LargeSquareMesh[];
/* Haures's burst mesh. */
extern const u16 Haures_BurstMesh[];
/* Heartbreak: the burst of sparks at the figure's hand, and the heart in its */
/* fist burning away. */
extern u8 Heartbreak_SparkBurstWidths[];
extern u8 Heartbreak_SparkBurstHeights[];
extern u16 Heartbreak_SparkBurstPictures[];
extern u8 Heartbreak_HeartBurnHeights[];
extern u8 Heartbreak_HeartBurnY[];
extern u16 Heartbreak_HeartBurnPictures[];
/* Raging Heat's sparks, spread, rings and embers, by level. */
extern const u8 RagingHeat_Styles[][4];
/* Juggle's styles, one row a mode. */
extern u8 Juggle_Styles[][7];
/* Flora: the petals' pictures, and when and where each bloom opens and how it
 */
/* is turned. */
extern const u8 Flora_PetalSizes[];
extern const u16 Flora_PetalPictures[];
extern const u16 Flora_BloomFrames[];
extern const s16 Flora_BloomRolls[];
extern const s16 Flora_BloomPitches[];
extern const s8 Flora_BloomX[];
extern const s8 Flora_BloomY[];
/* Sabre Dance's dagger: its mesh and corners. */
extern const u16 SabreDance_DaggerMesh[];
extern const MODEL_VERTEX SabreDance_DaggerCorners[];
/* Fume's levels, one row a level (FUME_LEVEL_COLUMN). */
extern const u16 Fume_Levels[][FUME_COLUMNS];
/* Ocean Fist's early drops: their gravities. */
extern const s32 OceanFist_DropGravity[4];
/* The rushing strike's streak scale, by mode. */
extern const s32 RushingStrike_Scales[];
/* The mirrored wall: its scale by mode and its bursts' shades. */
extern const s32 MirroredWall_Scales[];
extern const s32 MirroredWall_Shades[];
/* Daedalus: the shake, the birds' wing beats and drifts, the clatters, the */
/* rings, the leg, the splashes and the dives. */
extern s8 Daedalus_ShakeOffsets[];
extern u8 Daedalus_WingBeatPictures[];
extern u8 Daedalus_BirdStarts[][2];
extern s32 Daedalus_BirdSpeeds[][2];
extern u8 Daedalus_BirdFrames[];
extern u16 Daedalus_ClatterFrames[];
extern u16 Daedalus_RingStarts[];
extern s8 Daedalus_RingX[];
extern u8 Daedalus_RightLegPoints[][2];
extern u8 Daedalus_SplashPoints[][6][2];
extern u16 Daedalus_DiveStarts[];
/* The seven frames Daedalus's strikes land on. */
extern const u8 DaedalusStrike_HitFrames[];
/* Which of Vengeance's seven arrows strike, by level. */
extern const u8 Vengeance_ArrowStrikes[][7];
/* Life Shear's scythe: its two blades' meshes and its corners. */
extern const u16 LifeShear_PaleBladeMesh[];
extern const u16 LifeShear_OrangeBladeMesh[];
extern const MODEL_VERTEX LifeShear_BladeCorners[];
/* The geyser's drops: their gravity and smallest picture, by mode (enum */
/* GEYSER_MODE). */
extern const s32 Geyser_DropGravity[];
extern const s8 Geyser_DropMinSizes[];
/* The unused bubble column's glowing oval, fifteen pictures. */
extern u8 BubbleColumn_PictureWidths[];
extern u8 BubbleColumn_PictureHeights[];
extern u16 BubbleColumn_Pictures[];
/* Coatlicue: the bubbles' pictures, the bursts of bubbles, the ring's, */
/* flashes' and glows' frames, and the coils. */
extern u8 Coatlicue_PictureWidths[];
extern u8 Coatlicue_PictureHeights[];
extern u16 Coatlicue_Pictures[];
extern u16 Coatlicue_BurstFrames[];
extern u8 Coatlicue_BurstX[];
extern u8 Coatlicue_BurstY[];
extern u8 Coatlicue_BurstSpreadX[];
extern u8 Coatlicue_BurstSpacingY[];
extern s8 Coatlicue_BurstVelocityX[];
extern s8 Coatlicue_BurstVelocityY[];
extern s8 Coatlicue_BurstZ[];
extern s8 Coatlicue_BurstVelocityZ[];
extern u8 Coatlicue_PictureBias[];
extern u16 Coatlicue_RingFrames[];
extern u16 Coatlicue_FlashFrames[];
extern u16 Coatlicue_GlowFrames[];
extern u8 Coatlicue_CoilStarts[];
extern u16 Coatlicue_CoilScales[];
extern u16 Coatlicue_CoilRolls[];
extern u16 Coatlicue_CoilGrowth[];
extern u16 Coatlicue_CoilMaxScales[];
/* Catastrophe: the flashes, the demon's, armour's and mirrored demon's */
/* sprites, the bursts of lightning, the strikes and the glints. */
extern const u16 Catastrophe_FlashFrames[];
extern const u16 Catastrophe_FlashLengths[];
extern const u8 Catastrophe_DemonX[];
extern const u8 Catastrophe_DemonZ[];
extern const u8 Catastrophe_ArmourX[];
extern const u16 Catastrophe_ArmourZ[];
extern const u8 Catastrophe_MirroredDemonX[];
extern const u8 Catastrophe_MirroredDemonZ[];
extern const u16 Catastrophe_ForkStarts[];
extern const u8 Catastrophe_ForkX[];
extern const u8 Catastrophe_ForkY[];
extern const u8 Catastrophe_ForkPictures[];
extern const u16 Catastrophe_CanvasFlashFrames[];
extern const u8 Catastrophe_StrikeX[];
extern const u8 Catastrophe_StrikeY[];
extern const u16 Catastrophe_StrikeFrames[];
extern const u8 Catastrophe_StrikeSpeedsY[];
extern const s16 Catastrophe_StrikeRolls[];
extern const s16 Catastrophe_StrikeTurns[];
extern const s16 Catastrophe_StrikePictures[];
extern const u8 Catastrophe_GlintFlips[];
extern const s16 Catastrophe_BoltRolls[];
/* Haures: the particles, his scale, the wings' shapes and morphs, the parts */
/* and the bands. */
extern const u8 Haures_ParticleX[];
extern s32 Haures_UnitScale[];
extern const u8 Haures_MoundShapes[];
extern u8 Haures_MoundMorphSteps[];
extern const u8 Haures_PartPoints[];
extern const u8 Haures_BandStarts[];
/* Helm Splitter's pictures, and where each is drawn. */
extern u8 HelmSplitter_PictureWidths[];
extern u8 HelmSplitter_PictureHeights[];
extern u16 HelmSplitter_Pictures[];
extern s8 HelmSplitter_PictureBiasX[];
extern s8 HelmSplitter_PictureBiasY[];
/* How many of Dragon Cloud's embers are sparks, by variant. */
extern u8 DragonCloud_SparkCounts[];
/* The element cards: each card's length and how its sparks move. */
extern u8 ElementCards_Frames[];
extern SPARK_MOTION ElementCards_SparkMotions[];
/* Triple Chomp's three bites' frames. */
extern u8 TripleChomp_BiteFrames[];
/* The breaths: their lengths, bursts, glows, jaws and the frames they hit. */
extern u8 Breath_Frames[];
extern u8 Breath_BurstFrames[];
extern u8 Breath_MaxSizes[];
extern u8 Breath_RingStarts[];
extern u8 Breath_RingGrowth[];
extern s8 Breath_RingX[];
extern u8 Breath_FlashFrames[];
/* Trident: its rings, each trident's spin, the prong's corners and the glints'
 */
/* flips. */
extern const u8 Trident_RingStarts[];
extern const u8 Trident_RingGrowth[];
extern const s8 Trident_RingHeights[];
extern const s32 Trident_Spins[];
extern const MODEL_VERTEX Trident_ProngCorners[];
extern const u8 Trident_GlintFlips[];
/* The leaping strike's waves and rings. */
extern const u8 LeapingStrike_WaveStarts[];
extern const u8 LeapingStrike_WaveGrowth[];
extern const s8 LeapingStrike_WaveHeights[];
extern const u8 LeapingStrike_RingStarts[];
extern const u8 LeapingStrike_RingGrowth[];
extern const s8 LeapingStrike_RingHeights[];
/* Meteor Blow: its pictures, waves and rings. */
extern const u8 MeteorBlow_PictureWidths[];
extern const u8 MeteorBlow_PictureHeights[];
extern const u16 MeteorBlow_Pictures[];
extern const u8 MeteorBlow_WaveStarts[];
extern const u8 MeteorBlow_WaveGrowth[];
extern const u8 MeteorBlow_WaveStretch[];
extern const u8 MeteorBlow_RingStarts[];
extern const u8 MeteorBlow_RingGrowth[];
extern const s8 MeteorBlow_RingHeights[];
/* Liquifier's splash: its corners, start frames and heights. */
extern const MODEL_VERTEX Liquifier_SplashCorners[];
extern const u8 Liquifier_SplashStarts[];
extern const u8 Liquifier_SplashHeights[];
/* The canvas burst's flame picture for each three frames of its life. */
extern const u8 CanvasBurst_FlamePictures[];
/* Megiddo: its flashes' turns and starts, and its rays' turns and places. */
extern const s16 Megiddo_FlashPitches[];
extern const s16 Megiddo_FlashYaws[];
extern const u16 Megiddo_FlashStarts[];
extern const s16 Megiddo_RayPitches[];
extern const s16 Megiddo_RayYaws[];
extern const s16 Megiddo_RayRolls[];
extern const s8 Megiddo_RayX[];
extern const s8 Megiddo_RayY[];
/* Odyssey's flashes: their pitch and yaw. */
extern const s16 Odyssey_FlashPitches[];
extern const s16 Odyssey_FlashYaws[];
/* Moloch's rings: when each starts, how fast it grows, and its height. */
extern const u8 Moloch_RingStarts[4];
extern const u8 Moloch_RingGrowth[];
extern const s8 Moloch_RingHeights[];
/* Iris: the splashes, the pillar and particle models, the sound frames, the */
/* streaks, the drops, the strips and the cone. */
extern s32 Iris_SplashRadii[];
extern const u8 Iris_SplashOffsetsY[];
extern const u16 Iris_PillarMesh[];
extern const MODEL_VERTEX Iris_ParticleCorners[];
extern const u16 Iris_ParticleMesh[];
extern const u8 Iris_PsynergySoundFrames[];
extern const u16 Iris_DoorSoundFrames[];
extern const u16 Iris_PowerUpSoundFrames[];
extern const s8 Iris_StreakX[];
extern const u8 Iris_StreakY[];
extern const u16 Iris_StreakAngles[];
extern const s16 Iris_StreakSpeeds[];
extern const u8 Iris_LineShades[];
extern s32 Iris_MoteSizes[];
extern const u16 Iris_SplashStarts[];
extern const u16 Iris_SplashEnds[];
extern const u8 Iris_SplashX[];
extern const u8 Iris_SplashY[];
extern const u8 Iris_SplashSmall[];
extern const u8 Iris_DropSpeeds[];
extern const u8 Iris_DropFrames[];
extern const u8 Iris_DropPictureSteps[];
extern const MODEL_VERTEX Iris_LeftStripCorners[];
extern const MODEL_VERTEX Iris_RightStripCorners[];
extern const u16 Iris_StripMesh[];
extern const MODEL_VERTEX Iris_ConeCorners[];
extern const u16 Iris_ConeMesh[];

/* The buffers and tables the parts share, by the first part that reads them. */

/* falling.c */
/* The map cell buffer at its own address, which the effects borrow: no map is
 */
/* loaded during a battle. */
extern u8 MapGridCellBuffer[];

/* thor.c */
extern const s16 Thor_Points[6][2]; /* the three lines' end points */

/* flurry.c */
void btEffectRunFlurry(BATTLE_EFFECT_PARM *args, s32 mode);
/* btEffectRunFlurry's modes, by the abilities that play them: the rows */
/* of Flurry_Styles. Gust and Flying Attack take one, two or three strikes as */
/* a double or triple blow raises their variant. */
typedef enum FLURRY_MODE {
	FLURRY_BEAR_CLAW,			 /* five strikes; also Ransack */
	FLURRY_BACTERIA_RUSH,		 /* seven strikes, in lime */
	FLURRY_FREEBITE_RUSH,		 /* nine strikes, in blue */
	FLURRY_FLYING_ATTACK,		 /* one strike, in embers */
	FLURRY_FLYING_ATTACK_DOUBLE, /* two */
	FLURRY_FLYING_ATTACK_TRIPLE, /* three */
	FLURRY_GUST,				 /* one strike, each filling the canvas */
	FLURRY_GUST_DOUBLE,			 /* two */
	FLURRY_GUST_TRIPLE,			 /* three */
	FLURRY_UR_FLASH,	 /* three strikes, in the Mars Djinni's colours */
	FLURRY_CRUSHER_GRIP, /* one strike, with glints */
	FLURRY_VANISH_CLAW,	 /* one strike, with a ripple and smoke */
	FLURRY_CLAW_ATTACK	 /* one strike after a longer lunge, in embers */
} FLURRY_MODE;
/* A flurry's arcs, its FLURRY_ARC_SHEET. */
typedef enum FLURRY_ARC {
	FLURRY_ARCS_AND_WAVES, /* blue arc A, with a wave for the third and */
						   /* fourth of every four strikes */
	FLURRY_ARCS			   /* blue arc B for every strike */
} FLURRY_ARC;
/* Whose palette a flurry takes, its FLURRY_PALETTE. */
typedef enum FLURRY_PAL {
	FLURRY_PALETTE_MARS_DJINNI, /* the Mars Djinni sprite sheet's */
	FLURRY_PALETTE_LIME,
	FLURRY_PALETTE_BLUE_ARC,
	FLURRY_PALETTE_EMBER /* the ember streak sprite sheet's */
} FLURRY_PAL;
/* What else a flurry plays around its target, its FLURRY_EXTRA. */
typedef enum FLURRY_EXTRA_KIND {
	FLURRY_EXTRA_NONE,
	FLURRY_EXTRA_GLINTS, /* three glints a frame until frame 31 */
	FLURRY_EXTRA_RIPPLE	 /* BG2 swings on a sine wave and smoke rises */
} FLURRY_EXTRA_KIND;

/* The scale each of Gaia's rocks starts from before it grows. */
extern const SCALE Gaia_UnitScale;

/* spider_web.c */
/* A scale of one in 16.16 on both axes. */
extern const SPR_SCALE Ulysses_ParticleScale;

/* neptune.c */
extern const SCALE Neptune_UnitScale; /* the serpent's scale */

extern const s8 VoidBeam_Corners[][2]; /* the hexagon's corners (z, y) */

/* flora.c */
/* Flora's scale, one to one. */
extern const SCALE Flora_UnitScale;

/* daedalus.c */
/* The scales of the forest canopy Daedalus flies over, of his machine's */
/* pieces and of his missiles. */
extern const SCALE Daedalus_CanopyScale;
extern const SCALE Daedalus_MachineScale;
extern const SCALE Daedalus_MissileScale;

/* Coatlicue's and Catastrophe's figures' scales. */
extern const SCALE Coatlicue_UnitScale;
extern const SCALE Catastrophe_UnitScale;

/* haures.c */
/* Haures's scales as he charges, as he circles and as he opens. */
extern const SCALE Haures_ChargeScale;
extern const SCALE Haures_OrbitScale;
extern const SCALE Haures_OpeningScale;

/* moloch.c */
/* Moloch's motes' sizes, by a mote's number's low three bits. */
extern const u8 Moloch_MoteSizes[];
/* Iris's scale while she rises. */
extern const SCALE Iris_UnitScale;
/* The template btEffectBuildBlitter walks word by word to write a blitter */
/* (patch_template.s); none of it runs where it is. */
extern const u32 BlitterTemplate[];

/* The pixel fragments btEffectBuildBlitter copies into its blitters, from */
/* blitter_fragments.s: Forward reads the picture left to right and Reverse */
/* right to left. None of them runs where it is. */
/* Sets 0x40 in every byte of sl, which caps an additive word's pixels at the */
/* brightest canvas level. */
extern const u32 BattleEffect_Mask[];
/* The ARM immediate operand for 1 << n, n from 0 to 15 (blitter_operands.c). */
extern const u16 Blitter_ImmediatePowers[];
/* Copy, one pixel, right to left. */
extern const u32 BattleEffect_CopyReverse[];
/* Copy, one pixel, left to right. */
extern const u32 BattleEffect_CopyForward[];
/* Copy, a full cell's eight pixels, right to left. */
extern const u32 BattleEffect_Copy8Reverse[];
/* Copy, a full cell's eight pixels, left to right. */
extern const u32 BattleEffect_Copy8Forward[];
/* Transparent, one pixel, right to left. */
extern const u32 BattleEffect_TransparentReverse[];
/* Transparent, one pixel, left to right. */
extern const u32 BattleEffect_TransparentForward[];
/* Transparent, a full cell's eight pixels, right to left. */
extern const u32 BattleEffect_Transparent8Reverse[];
/* Transparent, a full cell's eight pixels, left to right. */
extern const u32 BattleEffect_Transparent8Forward[];
/* Max, one pixel, right to left. */
extern const u32 BattleEffect_MaxReverse[];
/* Max, one pixel, left to right. */
extern const u32 BattleEffect_MaxForward[];
/* Max, four pixels (one canvas word), right to left; a full cell's row takes */
/* it twice. */
extern const u32 BattleEffect_Max4Reverse[];
/* Max, four pixels (one canvas word), left to right; a full cell's row takes */
/* it twice. */
extern const u32 BattleEffect_Max4Forward[];
/* Additive, one pixel, right to left. */
extern const u32 BattleEffect_AdditiveReverse[];
/* Additive, one pixel, left to right. */
extern const u32 BattleEffect_AdditiveForward[];
/* Additive, four pixels added a word at a time, right to left; a full cell's */
/* row takes it twice. */
extern const u32 BattleEffect_ColorReverse[];
/* Additive, four pixels added a word at a time, left to right; a full cell's */
/* row takes it twice. */
extern const u32 BattleEffect_ColorForward[];

/* The model renderer's ARM functions, which run from IWRAM, each with its size
 */
/* in bytes and, for three of them, in DMA words. */
/* An ARM build of btRenderFillMesh, which btRenderRunFillMeshKernel runs. */
extern const u8 Render_FillMeshKernel[];
extern u8 Render_FillMeshKernelCodeSize[]; /* Render_FillMeshKernel's bytes */
/* The vertex transform btRenderTransformVertices runs. */
extern const u8 Render_TransformKernel[];
extern u8 Render_TransformKernelCodeSize[]; /* Render_TransformKernel's bytes */

/* The rasterizer kernels btRenderDrawModelList copies into IWRAM, one for each
 */
/* MODEL_KIND, with their sizes in bytes and, for three of them, in DMA words.
 */
extern const u8 Render_SolidKernel[];			 /* MODEL_SOLID */
extern const u8 Render_SolidMaxKernel[];		 /* MODEL_SOLID_MAX */
extern const u8 Render_SolidAdditiveKernel[];	 /* MODEL_SOLID_ADDITIVE */
extern const u8 Render_TexturedKernel[];		 /* MODEL_TEXTURED */
extern const u8 Render_TexturedMaxKernel[];		 /* MODEL_TEXTURED_MAX */
extern const u8 Render_TexturedAdditiveKernel[]; /* MODEL_TEXTURED_ADDITIVE */
extern const u8 Render_ShadedMaxKernel[];		 /* MODEL_SHADED_MAX */
extern const u8 Render_ShadedAdditiveKernel[];	 /* MODEL_SHADED_ADDITIVE */
extern const u8 Render_ShadedKernel[];			 /* MODEL_SHADED */
extern const u8 Render_TexturedNarrowKernel[];	 /* MODEL_TEXTURED_NARROW */
extern const u8 Render_SolidNarrowKernel[];		 /* MODEL_SOLID_NARROW */
extern u8 Render_SolidKernelCodeSize[];	   /* Render_SolidKernel's bytes */
extern u8 Render_SolidMaxKernelCodeSize[]; /* Render_SolidMaxKernel's bytes */
extern u8
	Render_SolidAdditiveKernelCodeSize[];  /* Render_SolidAdditiveKernel's */
										   /* bytes */
extern u8 Render_TexturedKernelCodeSize[]; /* Render_TexturedKernel's bytes */
extern u8
	Render_TexturedMaxKernelCodeSize[]; /* Render_TexturedMaxKernel's bytes */
extern u8
	Render_TexturedAdditiveKernelCodeSize[]; /* Render_TexturedAdditiveKernel's
											  */
											 /* bytes */
extern u8 Render_ShadedMaxKernelCodeSize[]; /* Render_ShadedMaxKernel's bytes */
extern u8
	Render_ShadedAdditiveKernelCodeSize[]; /* Render_ShadedAdditiveKernel's */
										   /* bytes */
extern u8 Render_ShadedKernelCodeSize[];   /* Render_ShadedKernel's bytes */
extern u8
	Render_TexturedNarrowKernelCodeSize[]; /* Render_TexturedNarrowKernel's */
										   /* bytes */
extern u8
	Render_SolidNarrowKernelCodeSize[]; /* Render_SolidNarrowKernel's bytes */
extern u8 Render_TexturedKernelCodeWords[]; /* Render_TexturedKernel's words */
extern u8 Render_ShadedKernelCodeWords[];	/* Render_ShadedKernel's words */
extern u8
	Render_TexturedNarrowKernelCodeWords[]; /* Render_TexturedNarrowKernel's */
											/* words */

/* The IWRAM bytes left to allocate, read as unsigned here: the comparison */
/* in btRenderCopyTextureToIwram is unsigned in the ROM, though the function */
/* returns a signed number. */
u32 syHeapGetRemainingIwram(void);

#endif /* ALCHEMY_BATTLE_EFFECT_INTERNAL_H */
