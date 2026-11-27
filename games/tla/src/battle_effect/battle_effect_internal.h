#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/* What every part of the battle_effect object shares: its includes, the
   canvas's and the shared sheets' sizes, the modes of the bodies several
   effects play, and the functions and tables the parts reach across each other.
   battle_effect.c includes it before the parts, and each part includes it
   too. */

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
#include "field/field_script.h"
#include "battle/battle_action.h"
#include "field/sprite.h"
#include "graphics/render_placement.h"
#include "battle/battle_unit.h"
#include "game/class_djinn.h"
#include "edition.h"
#include "field/field_sprite.h"

struct EFFECT_SPR;
void *btEffectSprAcquire(
	Sint32 width, Sint32 height, Uint32 shape, Sint32 mode);
void *btEffectSprCreateWithTiles(
	Sint32 width, Sint32 height, Sint32 attributes, Sint32 flg);

/* A point of a streak: x, y and z. */
typedef struct STREAK_POINT {
	Sint32 x;
	Sint32 y;
	Sint32 z;
} STREAK_POINT;

/* One of Cruel Ruin's 256 streaks: its head and tail and how it is turned. */
typedef struct PLUNGE_STREAK {
	Sint32 head;
	Sint32 tail;
	Sint32 unusedA;
	Sint32 pitch;
	Sint32 yaw;
	Sint32 roll;
	Sint32 unusedB;
} PLUNGE_STREAK;

/* The element streaks' state, which btEffectStartElementStreaks allocates in
   the HEAP_SLOT_BATTLE_EFFECT heap slot in place of the battle effect state. */
typedef struct ELEMENT_STREAKS_STATE {
	Sint32 frm;			 /* the streaks' tick count (btEffectDrawStreaks) */
	Sint32 halvePending; /* the streaks are drawn: halve the canvas into VRAM */
	/* Ticks of the background's fade left (btEffectDimBackgroundStep). */
	Sint32 fadeFrm;
	Sint32 fadeStep; /* the fade's steps so far, BACKGROUND_LEVEL_STEP each */
} ELEMENT_STREAKS_STATE;

/* An EFFECT_SPR as Cruel Ruin reads its OAM attribute 2: its tile, priority and
   palette, as fields or as one halfword. */
typedef struct RUIN_OBJECT {
	/* the sprite's next and OAM attributes 0 and 1 */
	Uint8 unusedBeforeAttr[8];

	union {
		struct {
			Uint16 tile : 10;
			Uint16 pri : 2;
			Uint16 pal : 4;
		} bits;

		Uint16 all;
		Uint8  bytes[2];
	} attr;
} RUIN_OBJECT;

/* The map cell buffer holds WIN0's line table and then the droplets Douse's
   drops throw up. */
typedef struct DROPLET_BUFFER {
	Uint16	 lines[160];
	PARTICLE droplets[512];
} DROPLET_BUFFER;

/* Eclipse's scale for the dragon's sprites, and Cruel Ruin's for its summon,
   one to one. */
extern const SCALE EclipseDragonScale;
extern const SCALE CruelRuinUnitScale;

/* One handler for each battle effect, by effect less one; 0 where none. */
extern void (*const BattleEffectHandlers[])(BATTLE_EFFECT_PARM *args);

/* Zagan's scale, one to one. */
extern const SCALE ZaganUnitScale;

extern const SCALE ProcneUnitScale; /* the comets' unit scale */

/* The battle screen's display mode while an effect plays: background mode 1
   with BG0 to BG2, sprites and both windows on. */
#define EFFECT_DISPCNT \
	(DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON | \
		DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

/* The whole pixels of a stored 16.16 value, read as its upper halfword. */
/* UB: the Sint32 is read through an Sint16. */
#if FIX_TYPE_PUNNING
#define FIXED_INTEGER_PART(v) FIXEDtoINT((Sint32)(v))
#else
#define FIXED_INTEGER_PART(v) (((Sint16 *)&(v))[1])
#endif

/* The canvas added to the battle under it: both blend weights whole. */
#define EFFECT_BLEND_ADD		   BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX)
/* The canvas at `level` sixteenths over the whole battle under it. */
#define EFFECT_BLEND_CANVAS(level) BLEND_ALPHA(level, BLEND_WEIGHT_MAX)
/* The canvas's usual weight over the battle, in sixteenths: every canvas opens
   with it. */
#define CANVAS_BLEND_LEVEL		   14
/* The element pillar's and element burst's canvas weight over the battle. */
#define ELEMENT_BLEND_LEVEL		   12
/* The motes the element pillar and element burst set up in PARTICLES; the burst
   leaves them undrawn. */
#define ELEMENT_SPIRAL_MOTES	   128

/* The map cell buffer as btEffectBuildWindowEdgeTable reads it: window 0's
   right edge, how far each of its lines is set in from that edge, and the right
   edge it builds for every screen line. */
#define WINDOW_EDGE_RIGHT		((Uint16 *)MapGridMetatileBuffer)
#define WINDOW_EDGE_INSETS		(Ram_MapMetatileBuffer + 2)
#define WINDOW_EDGE_INSET_LINES 128
#define WINDOW_EDGE_LINES		(WINDOW_EDGE_INSETS + WINDOW_EDGE_INSET_LINES)

/* The particles an effect keeps in the map cell buffer, from its start. */
#define PARTICLES		((PARTICLE *)Ram_MapMetatileBuffer)
/* The particles an effect keeps in the map cell buffer from its second 16 KB
   on, past PARTICLES or the pictures and meshes it keeps in the first: a
   second, larger table, always reached at its fixed address, where the effects
   that throw more sparks, motes or trails than the state's 64 particles keep
   them. */
#define EXTRA_PARTICLES ((PARTICLE *)(Ram_MapMetatileBuffer + 16384))

/* The VRAM address of a sprite's tiles, from its VRAM cache entry. */
#define OBJ_TILES(sprite) \
	(OBJ_VRAM0 + VramCacheEntries[((SPR *)(sprite))->slot].ofs)

/* Index of DMAxCNT_H, the control half, in a channel's 16-bit registers. */
#define DMA_CONTROL_HALF 5

/* The particles the map cell buffer holds when an effect lays its particles
   over it from the start. */
#define PARTICLE_POOL_COUNT 1024
/* Nereid's drops burst up, in btEffectRunNereid and in the unused copy of it:
   every free one of the first NEREID_BURST_DROPS as the columns gather, then
   NEREID_DROPS_A_TICK a tick while they rise. */
#define NEREID_BURST_DROPS	256
#define NEREID_DROPS_A_TICK 16
/* The sprites that draw Megaera, in her summon and in her formation. */
#define MEGAERA_OBJECTS		11

/* The canvas: 128x128 pixels, one byte each, in its heap slot and in VRAM at
   BG_CHAR_BLOCK(1) (CANVAS_BYTES). */
#define CANVAS_SIZE	 128
/* The canvas's rows and columns of 8x8 tiles. */
#define CANVAS_TILES (CANVAS_SIZE / 8)
/* The byte of canvas pixel (x, y): the canvas is laid out in 8x8 tiles, row by
   row, each tile 64 bytes row by row. */
#define CANVAS_PIXEL_OFFSET(x, y) \
	((((y) / 8 * CANVAS_TILES + (x) / 8) * 8 + ((y) & 7)) * 8 + ((x) & 7))
/* The bytes of one 8x8 tile of the canvas; a row of pixels steps from one tile
   to the next by CANVAS_TILE_BYTES - 8. */
#define CANVAS_TILE_BYTES	   (8 * 8)
/* The bytes of a 32x32 sprite's tiles in 256 colours, and in 16. */
#define OBJ_32X32_8BPP_BYTES   (32 * 32)
#define OBJ_32X32_4BPP_BYTES   (32 * 32 / 2)
/* The bytes of a 64x64 sprite's tiles in 256 colours, and in 16. */
#define OBJ_64X64_8BPP_BYTES   (64 * 64)
#define OBJ_64X64_4BPP_BYTES   (64 * 64 / 2)
/* The bytes of a 16x16 sprite's tiles in 16 colours. */
#define OBJ_16X16_4BPP_BYTES   (16 * 16 / 2)
/* The byte of the first pixel of canvas row y, y not negative. */
#define CANVAS_ROW_OFFSET(y)   ((((y) & 7) << 3) + (((y) >> 3) << 10))
/* The brightest canvas level: the additive blitters and
   CANVAS_COPY_AND_BRIGHTEN stop at it. */
#define CANVAS_LEVEL_BRIGHTEST 63

/* The battle view, under the top window and over the menus: screen lines 16 to
   135. */
#define BATTLE_VIEW_TOP		 16
#define BATTLE_VIEW_HEIGHT	 120
#define BATTLE_VIEW_BOTTOM	 (BATTLE_VIEW_TOP + BATTLE_VIEW_HEIGHT)
/* The backdrop gradients Azul and Coatlicue write over the battle view, a
   colour a line from BATTLE_VIEW_TOP - 1: each channel a level darker every so
   many lines down, blue every 15, green every 16 and red every 17. */
#define GRADIENT_BLUE_LINES	 15
#define GRADIENT_GREEN_LINES 16
#define GRADIENT_RED_LINES	 17

/* Writes one row of BG2's map for the canvas, two tiles at a time. */
#define EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset) \
	for((column) = 0; (column) != 8; (column)++, (offset) += 2) { \
		Sint32 tileIndex; \
		Sint16 pair; \
\
		tileIndex = (row) * 16 + (column) * 2; \
		pair = ((tileIndex + 1) << 8) | tileIndex; \
		*(volatile Sint16 *)((Uint8 *)BG_CANVAS_MAP + (offset)) = pair; \
	}

/* Three summons open alike (Eclipse, Daedalus and Flora): the battle flashes
   away over SUMMON_OPENING_TICKS, and at SUMMON_OPENING_LOAD_TICK their sky
   and their sprites load. */
#define SUMMON_OPENING_TICKS	 52
#define SUMMON_OPENING_LOAD_TICK 27

/* Windows 0 and 1 both cover the battle view: the full width, lines 16 to
   135. */
#define EFFECT_SET_BATTLE_WINDOWS() \
	(REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH), \
		REG_WIN0V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM), \
		REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH), \
		REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM))
/* The canvas's first BATTLE_VIEW_HEIGHT lines, which the battle view shows:
   what most of this game's effects clear and fill, leaving the last 8. */
#define CANVAS_VIEW_BYTES (CANVAS_SIZE * BATTLE_VIEW_HEIGHT)

/* How far right the narrow canvas is set to show over the party: against the
   screen's right edge. */
#define NARROW_CANVAS_PARTY_X (DISPLAY_WIDTH - CANVAS_SIZE)

/* Where the summons centre the projection while their figures fly, lower than
   the battle's BATTLE_PROJECTION_CENTER_Y. */
#define SUMMON_PROJECTION_CENTER_Y 240

/* The ticks the battle background darkens behind an effect's canvas
   (state->fadeFrm, btEffectDarkenBackgroundStep). */
#define BACKGROUND_DARKEN_TICKS	  24
/* The level btBackgroundLoadStub darkens the battle's own background to as it
   comes back after an effect: as dark as the canvas's fade left it. */
#define BACKGROUND_DARKENED_LEVEL BACKGROUND_DARKEN_TICKS

/* A sprite sheet resource opens with its 64 colours, ahead of its compressed
   pictures. */
#define SHEET_PALETTE_COLORS 64
#define SHEET_PALETTE_BYTES	 (SHEET_PALETTE_COLORS * 2)

/* Loads a sprite sheet's colours into the background palette. */
#define LOAD_BG_PALETTE(resource) \
	{ \
		void *palette = syResourceGetTableEntry(resource); \
\
		syWordsCopy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES); \
	}

/* The damping that keeps a particle's whole speed. */
#define PARTICLE_DAMPING_NONE DAMPING_SCALE

/* The bytes of a sprite sheet picture, one byte a pixel, from its name's _WIDTH
   and _HEIGHT. */
#define PICTURE_BYTES(name) (name##_WIDTH * name##_HEIGHT)

/* Draws picture `name` of `sheet` at its own size, its top left at (x, y). */
#define EFFECT_DRAW_PICTURE(draw, canvas, sheet, name, x, y) \
	(draw)(canvas, (sheet) + (name), x, y, name##_WIDTH, name##_HEIGHT)

/* A bite's half pictures (BlueDropletSheet's closing fangs, each drawn beside
   its mirror): the canvas row they start at, and their size. */
#define TARGET_STRIKE_BITE_Y	  56
#define TARGET_STRIKE_BITE_WIDTH  18
#define TARGET_STRIKE_BITE_HEIGHT 48

/* The battle background's image as BG1 shows it from BG character block 2, one
   byte a pixel. */
#define BATTLE_BACKGROUND_WIDTH	 256
#define BATTLE_BACKGROUND_HEIGHT 120
#define BATTLE_BACKGROUND_BYTES \
	(BATTLE_BACKGROUND_WIDTH * BATTLE_BACKGROUND_HEIGHT)
/* A background's scroll (BG_SCROLL) is a halfword: a negative scroll is held
   BG_SCROLL_WRAP above itself, and read back by taking it away. */
#define BG_SCROLL_WRAP				(1 << 16)
/* Two entries of a background map in one word: `left`, then the one to its
   right. */
#define MAP_ENTRY_PAIR(left, right) ((Uint32)(right) << 16 | (left))
/* The tile BG1 shows around a picture of its own, the last a map entry can
   name. */
#define BG1_CLEAR_TILE				1023
/* BG character block 2's first tile, counted from BG1's character base. */
#define BG1_BLOCK2_FIRST_TILE		(2 * BG_CHAR_SIZE / TILE_SIZE_8BPP)
/* The bytes of one 16-colour palette. */
#define PALETTE_BANK_BYTES			32
/* A word of two palette entries, both `color`, to fill a palette a word at a
   time. */
#define COLOR_PAIR(color)			((color) << 16 | (color))

/* A model's shade (RENDER_MODEL.shade) is added to each of its pixels' canvas
   levels: at this or below, nothing of the model is left to draw, and the
   effects skip it. */
#define MODEL_SHADE_INVISIBLE (-64)

/* RENDER_MODEL.kind: the rasterizer kernel btRenderDrawModelList copies into
   IWRAM to draw a model, and how it puts each pixel on the canvas. A solid
   model fills its faces with their colours; a textured one maps its texture; a
   shaded one adds the model's shade to each texel and skips those it leaves at
   0 or below. The narrow kernels draw into a target half the canvas's width. */
typedef enum MODEL_KIND {
	MODEL_END,		 /* ends the list */
	MODEL_SOLID,	 /* btRenderSolidKernel: the colour replaces the canvas */
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
   only the faces wound one way round on the screen, clockwise with
   MODEL_CLOCKWISE and anticlockwise without it, and skips the others. */
#define MODEL_ONE_SIDED 2
#define MODEL_CLOCKWISE 1

/* Starts a tick's 3D projection from the battle camera. */
#define EFFECT_VIEW_FROM_CAMERA(camera) \
	(syInitMatrix(), syLookAtLoad((camera)->eye, (camera)->focus))

/* The sprite sheets' pictures, as The Broken Seal names them: this game draws
   the same sprite sheets at the same sizes. */

/* The bytes of the six glint pictures that open the tornado sprite sheet. */
#define GLINT_SET_BYTES \
	(19 * 31 + 23 * 31 + 23 * 31 + 19 * 21 + 16 * 21 + 17 * 24)

/* Draws flare picture `size`, a square, centred on (x, y). */
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size) \
	(draw)(canvas, (sheet) + BattleEffectFlarePictures[(size) - 1], \
		(x) - (size), (y) - (size), (size) * 2, (size) * 2)

/* The reaper in CounterRevealSheet, which Condemn and Mortal Danger decode: the
   reaper with its scythe raised, then swung, then the red crescent the swing
   leaves. */
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
   CrescentSheet's and the reveal masks. */
#define SLASH_PICTURE_SIZE 120

/* Closing bolts and lightning columns use the same 24x120 pictures. */
#define LIGHTNING_BOLT_WIDTH		 24
#define LIGHTNING_BOLT_HEIGHT		 120
#define LIGHTNING_BOLT_PICTURE_BYTES PICTURE_BYTES(LIGHTNING_BOLT)

/* BlueFlameSheet: the column of flame, then the flame that splashes from it,
   twice its width. */
#define BLUE_FLAME_COLUMN_WIDTH	 17
#define BLUE_FLAME_COLUMN_HEIGHT 104
#define BLUE_FLAME_SPLASH_WIDTH	 34
#define BLUE_FLAME_SPLASH_HEIGHT 65
/* WaveSheet's six pictures, BlueArcSheetA and B's six, and YellowOrbSheet's. */
#define WAVE_WIDTH				 48
#define WAVE_HEIGHT				 16
#define BLUE_ARC_WIDTH			 48
#define BLUE_ARC_HEIGHT			 72
#define YELLOW_ORB_WIDTH		 40
#define YELLOW_ORB_HEIGHT		 48
/* The yellow orb as btEffectPackYellowOrbs packs it, every second byte. */
#define YELLOW_ORB_PACKED_WIDTH	 (YELLOW_ORB_WIDTH / 2)
#define YELLOW_ORB_PACKED_HEIGHT YELLOW_ORB_HEIGHT
#define YELLOW_ORB_PICTURES		 6

/* FireStreakSheet's six pictures and FireBurstSheet's. */
#define FIRE_STREAK_WIDTH			  48
#define FIRE_STREAK_HEIGHT			  72
#define FIRE_BURST_WIDTH			  40
#define FIRE_BURST_HEIGHT			  80
/* The small Djinn sprite sheets' pictures are SMALL_DJINNI_WIDTH wide;
   Mercury's and Venus's are shorter than SMALL_DJINNI_HEIGHT. */
#define MERCURY_SMALL_DJINNI_HEIGHT	  34
#define VENUS_SMALL_DJINNI_HEIGHT	  38
/* FireBlobSheet's blobs and CrystalSheet's crystal. */
#define FIRE_BLOB_WIDTH				  48
#define FIRE_BLOB_HEIGHT			  32
#define CRYSTAL_WIDTH				  32
#define CRYSTAL_HEIGHT				  64
/* CrystalSheet's shard pictures, after its crystal, and the shards of
   PrismShardStarts a crystal breaks into. */
#define CRYSTAL_SHARD_PICTURES		  PICTURE_BYTES(CRYSTAL)
#define SHARDS_PER_CRYSTAL			  16
/* BoulderSheet's boulder, its first picture, and its shard pictures after
   it. */
#define BOULDER_WIDTH				  34
#define BOULDER_HEIGHT				  62
#define BOULDER_SHARD_PICTURES		  (BOULDER_WIDTH * BOULDER_HEIGHT)
/* A shard turns through its three pictures, one each SHARD_TURN of its
   variant. */
#define SHARD_TURN					  96
/* PaletteRampImage's shard pictures (drawn through StatChangeShardPictures),
   where the stat reduction and the Sword Card decode them in the sprite sheet,
   followed by DIMMED_COPIES - 1 copies of them, each darker than the one
   before. */
#define STAT_REDUCTION_SHARD_PICTURES 9600
#define STAT_REDUCTION_SHARD_BYTES	  696 /* bytes in each copy */

/* LightningPillarSheet's pillar, in two pictures. */
#define LIGHTNING_PILLAR_WIDTH	 48
#define LIGHTNING_PILLAR_HEIGHT	 112
/* FirePillarSheetA: the pillar of fire, then two flames. */
#define FIRE_PILLAR_WIDTH		 34
#define FIRE_PILLAR_HEIGHT		 104
#define FIRE_PILLAR_FLAME_WIDTH	 20
#define FIRE_PILLAR_FLAME_HEIGHT 37
/* SpiralSheet's pictures. */
#define SPIRAL_WIDTH			 32
#define SPIRAL_HEIGHT			 32
/* A card's picture, as the Sleep, Baffle, Death and Sword Cards and the
   Bramble, Frost, Flame and Thunder Cards draw it. */
#define CARD_WIDTH				 32
#define CARD_HEIGHT				 64
/* SheepCardSheet's sheep, four pictures of it tumbling after its card. */
#define SHEEP_WIDTH				 32
#define SHEEP_HEIGHT			 32

/* FlameballSheet and FireballSheet's pictures. */
#define FLAMEBALL_WIDTH	 40
#define FLAMEBALL_HEIGHT 64

/* Each half of RuneSheet's rune, the second drawn mirrored. */
#define RUNE_HALF_WIDTH	  24
#define RUNE_HALF_HEIGHT  48
/* The 64x64 texture Flash Force and Void Beam build: the rune's two halves side
   by side, the second mirrored, eight pixels in from the top left. */
#define RUNE_TEXTURE_SIZE 64
#define RUNE_TEXTURE_RUNE (8 * RUNE_TEXTURE_SIZE + 8)

/* A Djinni's picture: each frame of the full Djinn sprite sheets
   (VenusDjinnSheet and its kind), and the one picture of the small sprite
   sheets. */
#define DJINNI_WIDTH		40
#define DJINNI_HEIGHT		40
#define SMALL_DJINNI_WIDTH	20
#define SMALL_DJINNI_HEIGHT 40

/* The pictures of the sprite sheets several effects share, each sprite sheet's
   pictures all one size: SmokeSheet's puffs, FlashBurstSheet's bursts,
   SkullSheet's skulls and the tornado strips after TornadoSheet's glints. */
#define SMOKE_PUFF_WIDTH		  24
#define SMOKE_PUFF_HEIGHT		  48
/* A puff of SmokeSheet's lives through its six pictures, four ticks each. */
#define SMOKE_PUFF_TICKS		  (6 * 4)
#define FLASH_BURST_WIDTH		  32
#define FLASH_BURST_HEIGHT		  64
/* FlashBurstSheet's six pictures: a white flash turning to a fireball that
   burns out to embers. */
#define FLASH_BURST_PICTURE_COUNT 6
/* BlueBeamSheet: a blue bubble, a small splash of blue light the bubble's size,
   then a large splash. */
#define BLUE_BUBBLE_WIDTH		  24
#define BLUE_BUBBLE_HEIGHT		  24
#define BLUE_LARGE_SPLASH_WIDTH	  42
#define BLUE_LARGE_SPLASH_HEIGHT  42
/* BlueCloudPuffSheet: four 32x32 puffs of cloud. */
#define CLOUD_PUFF_WIDTH		  32
#define CLOUD_PUFF_HEIGHT		  32
#define SKULL_WIDTH				  24
#define SKULL_HEIGHT			  24
#define TORNADO_STRIP_WIDTH		  40
#define TORNADO_STRIP_HEIGHT	  32
/* The pictures of DaggerSheet, ShieldSheet, GlowOrbSheet, VioletStreakSheet and
   WispSheet. */
#define DAGGER_WIDTH			  24
#define DAGGER_HEIGHT			  40
#define SHIELD_WIDTH			  40
#define SHIELD_HEIGHT			  40
#define GLOW_ORB_WIDTH			  40
#define GLOW_ORB_HEIGHT			  40
#define VIOLET_STREAK_WIDTH		  32
#define VIOLET_STREAK_HEIGHT	  64
#define WISP_WIDTH				  64
#define WISP_HEIGHT				  32
/* VortexSheet's six pictures and HeartSheet's heart. */
#define VORTEX_WIDTH			  24
#define VORTEX_HEIGHT			  48
#define HEART_WIDTH				  22
#define HEART_HEIGHT			  42
/* WaterSpraySheet: the wave, then the splash's pictures. */
#define WATER_WAVE_WIDTH		  32
#define WATER_WAVE_HEIGHT		  32
#define WATER_SPLASH_WIDTH		  48
#define WATER_SPLASH_HEIGHT		  48

/* The canvas life cycle, shared by every canvas effect. */

/* The bytes an effect's state is lent: more than struct */
/* BATTLE_EFFECT_STATE holds. */
#define BATTLE_EFFECT_STATE_SIZE 31612

/* The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100). */
#define MOTE_SET_BYTES		  770
/* A picture's dimmed copies: DIMMED_COPIES of it, copy i with every level
   capped at DIMMED_COPY_CAP(i), 7 levels lower than the copy before, copy 0 not
   at all. Thor's ring darkens through copies of the mote pictures, a stat
   reduction's shards through copies of the shard. */
#define DIMMED_COPIES		  8
#define DIMMED_COPY_CAP(copy) (CANVAS_LEVEL_BRIGHTEST + 1 - (copy) * 7)
/* Caps a colour level at cap, and at 0 from below. */
#define EFFECT_CAP_LEVEL(level, cap) \
	{ \
		if((level) > (cap)) \
			(level) = (cap); \
		if((level) < 0) \
			(level) = 0; \
	}
/* The bytes of BlueGlowSheet's ten flare pictures, 2 to 20 pixels square:
   4 * (1 + 4 + ... + 100). */
#define FLARE_SET_BYTES	   1540
/* The battle sheet (HEAP_SLOT_BATTLE_SHEET) the casting impacts, the plain
   attacks and Cruel Ruin decode their glows into: room for BlueGlowSheet's
   flares, the largest of them, with ten bytes to spare. */
#define BATTLE_SHEET_BYTES (FLARE_SET_BYTES + 10)
#define MOTE_PICTURES	   10

/* The columns of TargetStrikeStyles, one row a style. */
typedef enum TARGET_STRIKE_COLUMN {
	TARGET_STRIKE_KIND,			 /* STRIKE_KIND_SLASH or STRIKE_KIND_BITE */
	TARGET_STRIKE_HITS,			 /* slashes or bites */
	TARGET_STRIKE_FLAGS,		 /* the extras (STRIKE_SMOKE and on) */
	TARGET_STRIKE_COLOR,		 /* the background's palette (STRIKE_COLOR_) */
	TARGET_STRIKE_PICTURE_TICKS, /* ticks each picture shows */
	TARGET_STRIKE_COMPOSITE,	 /* the bites' blitter mode */
	TARGET_STRIKE_TICKS			 /* the effect's length */
} TARGET_STRIKE_COLUMN;

/* The two kinds of target strike (TargetStrikeStyles). */
#define STRIKE_KIND_SLASH 0 /* sideways slashes */
/* Rows of fangs above and below the target that close on it. */
#define STRIKE_KIND_BITE  1

/* A target strike style's extras. each hit raises smoke puffs from the target's
   feet */
#define STRIKE_SMOKE  1
/* motes fly out of the target and back into the user */
#define STRIKE_ABSORB 2
#define STRIKE_GLINTS 4 /* glints sparkle around the target */
#define STRIKE_SPARKS 8 /* each hit scatters sparks */
/* one heavy hit: a big shake and the target recoils */
#define STRIKE_HEAVY  16
#define STRIKE_FLASH  32 /* as heavy, with flash pictures for the smoke */
#define STRIKE_STAR	  64 /* a star swells on the target */

/* The palettes a target strike's background takes (TARGET_STRIKE_COLOR). */
#define STRIKE_COLOR_BLUE	   0 /* BlueBeamSheet's */
#define STRIKE_COLOR_LIME	   1 /* LimePalette */
#define STRIKE_COLOR_PINK	   2 /* PinkPalette */
#define STRIKE_COLOR_EMBER	   3 /* EmberStreakSheet's */
#define STRIKE_COLOR_MARS	   4 /* MarsDjinnSheet's */
#define STRIKE_COLOR_LIGHTNING 5 /* LightningBoltSheet's */
#define STRIKE_COLOR_ORANGE	   6 /* OrangePaletteB */

/* The target strike styles, rows of TargetStrikeStyles: The Broken Seal's
   twelve, then this game's own. */
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
	/* the same row as MARS_ADDITIVE_DRAIN_BITES */
	TARGET_STRIKE_STYLE_MANEATER,
	TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH,
	TARGET_STRIKE_STYLE_MARS_HEAVY_BITE,
	TARGET_STRIKE_STYLE_ORANGE_FLASH_BITE,
	TARGET_STRIKE_STYLE_MARS_STAR_SLASH,
	TARGET_STRIKE_STYLE_BLUE_SPARK_BITE,
} TARGET_STRIKE_STYLE;

/* Window 0 shows the battle view: BG0 to BG2, the sprites and blending. */
#define EFFECT_WIN0_LAYERS \
	(WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | \
		WININ_WIN0_CLR)

/* Outside the battle view: BG0 and blending only. */
#define EFFECT_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

/* Opens the canvas layer at its own width: BG2 shows it one pixel a pixel. */
#define EFFECT_START_NARROW_CANVAS(bgControl) \
	(btEffectStartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/* Strikes target `i`: it flashes white in its damage pose and is knocked back
   by `motion`. */
#define EFFECT_STRIKE_TARGET(effect, i, ticks, motion) \
	(btEffectTintBattler( \
		 (effect)->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, ticks), \
		btMotionApplyKnockbackStub((effect)->tgts[i], motion))

/* A blitter that clips its rectangles to the canvas. */
#define EFFECT_CLIP_BOUNDS (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/* The second blitter, the drawing function in the paired heap slot. */
#define ALT_BLITTER (Ram_HeapSlots->ramCodePaired)

/* Builds a blitter for the canvas in heap slot `slot`, for one clip, mirror and
   blend mode (BLITTER_ADDITIVE to BLITTER_COPY): where both are constants they
   go packed into one word to btEffectLoadBlitter, the shorter call, and
   otherwise to btEffectBuildBlitter as they are. The name is ours:
   every glint draw and mirrored pair that picks its flags or its mode at run
   time is written so. */
#define EFFECT_LOAD_BLITTER(slot, flags, mode) \
	(__builtin_constant_p((flags) | (mode) << 4) \
			? btEffectLoadBlitter(slot, (flags) | (mode) << 4) \
			: btEffectBuildBlitter(slot, 7, 7, flags, mode))

/* Sets what the canvas transfer leaves in the canvas after each copy. */
#define EFFECT_SET_TRANSFER(state, mode, value) \
	((state)->transferMode = (mode), (state)->transferValue = (value))

/* Sets the transfer, then starts copying the canvas to VRAM once a tick. */
#define EFFECT_START_TRANSFER(state, mode, value) \
	(EFFECT_SET_TRANSFER(state, mode, value), \
		sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK))

/* Presents the canvas and waits for the next tick. */
#define EFFECT_PRESENT_CANVAS(state) ((state)->transferPending = 1, sySynch(1))

/* Waits until a VBlank has passed since the one that last copied the canvas. */
/* DEAD: each test reads the count twice and drops the first read; the ROM keeps
   both loads. */
#define WAIT_TRANSFER(state) \
	while((state)->framesSinceTransfer, (state)->framesSinceTransfer <= 1)

/* The usual end of an effect's tick: counts down the targets' flash timers,
   then presents the canvas. */
#define EFFECT_END_TICK(state) \
	(btEffectCountDownBattlerTints(), EFFECT_PRESENT_CANVAS(state))

/* Frees the two blitters, the second first. */
#define EFFECT_RELEASE_BLITTERS() \
	(syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED), \
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE))

/* The usual end of a canvas effect: the transfer, the blitters, then the canvas
   layer. */
#define EFFECT_FINISH_CANVAS() \
	(syCloseEvent(btEffectTransferCanvas), EFFECT_RELEASE_BLITTERS(), \
		btEffectEndCanvasLayer())

/* The same for an effect that drew through one blitter. */
#define EFFECT_FINISH_CANVAS_ONE_BLITTER() \
	(syCloseEvent(btEffectTransferCanvas), \
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE), btEffectEndCanvasLayer())

/* The largest of the ten mote pictures. */
#define MOTE_SIZE_MAX 10
/* Draws mote picture `size` (1 to MOTE_SIZE_MAX: size pixels wide, twice that
   tall), centred across x and standing on y. */
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size) \
	(draw)(canvas, (sheet) + BattleEffectMotePictures[(size) - 1], \
		(x) - (size) / 2, (y) - (size), size, (size) * 2)

/* Marks particles[0] to particles[count - 1] free: a particle whose variant is
   -1 is not drawn or moved until an effect gives it a life. */
#define EFFECT_FREE_PARTICLES(i, particles, count) \
	for((i) = 0; (i) != (count); (i)++) \
	(particles)[i].variant = -1

/* Shows the canvas stretched to twice its width, `top` sixteenths of its colour
   added over what lies under it. */
#define EFFECT_SHOW_WIDE_CANVAS(top) \
	(REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5), \
		REG_BLDALPHA = EFFECT_BLEND_CANVAS(top), \
		REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/* A line's 16.16 sideways sway as its BG2X value in state->bg2xLines, which the
   wave HBlank DMA writes line by line. BG2X counts 256ths of a canvas pixel, so
   a sway of toFIXED(n) moves the line n / 4 canvas pixels: n / 2 screen pixels
   where the canvas is shown at twice its width. */
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

/* Lets the battle show the damage, playing hitSound as the targets react: 0
   keeps the one queued, and HIT_SOUND_NONE plays none where the effect has
   played its own. A second use changes nothing. */
#define EFFECT_SHOW_DAMAGE(hitSound) btPlaybackStartStub(hitSound)
#define HIT_SOUND_NONE				 (-1)

/* Passed to btEffectTintBattler as its palette, pose or timer slot: leave that
   one as it is. */
#define TINT_KEEP (-1)

/* The tumbling quads' modes (btEffectRunTumblingQuads), each named for the
   effect that plays it. */
typedef enum TUMBLING_QUADS_MODE {
	TUMBLING_QUADS_BALM,   /* over a prepared canvas, with the Mercury Djinn */
	TUMBLING_QUADS_TINDER, /* over a prepared canvas, with the Mars Djinn */
	TUMBLING_QUADS_PHOENIX /* rising from the first target */
} TUMBLING_QUADS_MODE;

/* The gasp family's modes (btEffectRunGaspFamily), each named for the effect
   that plays it. */
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

/* The twisting column's modes (btEffectRunTwistingColumn), each named for the
   effect that plays it. */
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

/* The rushing strike's sparks' palettes (btEffectRunRushingStrike), each named
   for the effect that first plays it; Bone Charge plays Stun Muscle's. */
typedef enum RUSHING_STRIKE_MODE {
	/* Unused: no wrapper passes it; the fire pillar's colours. */
	RUSHING_STRIKE_UNUSED,
	RUSHING_STRIKE_BRUTE_FORCE,
	RUSHING_STRIKE_ONSLAUGHT,
	RUSHING_STRIKE_HEADBUTT,
	RUSHING_STRIKE_STUN_MUSCLE,
	RUSHING_STRIKE_LIVING_ARMOR
} RUSHING_STRIKE_MODE;

/* The mirrored wall's modes (btEffectRunMirroredWall), each named for the
   effect that plays it. */
typedef enum MIRRORED_WALL_MODE {
	MIRRORED_WALL_SPASM, /* actual size, a gentle shake for 32 ticks */
	/* A ninth wider than Spasm's, a hard shake for 8 ticks. */
	MIRRORED_WALL_BERSERK,
	/* A ninth wider than Spasm's, no shake, in the Fireball colours. */
	MIRRORED_WALL_RECOVERY,
	MIRRORED_WALL_SACK
} MIRRORED_WALL_MODE;

/* Where btEffectRunSparkGroups' bursts go off. */
typedef enum SPARK_GROUPS_MODE {
	SPARK_GROUPS_NOVA,	   /* Nova's */
	SPARK_GROUPS_DYNAMITE, /* Dynamite's, orange */
	/* below the user, in the LightningBolt colours, the sparks rising */
	SPARK_GROUPS_RISING
} SPARK_GROUPS_MODE;

/* The declarations the parts share. */

struct SPR_ANIMATION;
struct BATTLE_EFFECT_PARM;
struct SCREEN_POINT;
struct PARTICLE;

/* A mesh triangle: its three vertex numbers and the texture coordinates of each
   corner. A strip quad is two of them; a triangle whose three vertices are 0
   ends a list. */
typedef struct MESH_TRIANGLE {
	Uint8 vertex[3];
	Uint8 unusedA; /* nothing reads or writes it */
	Uint8 uv[3][2];
	Uint8 unusedB[2]; /* nothing reads or writes them */
} MESH_TRIANGLE;

struct SPR_SCALE;
struct STRIP_QUAD;

/* The parts' functions that another part calls before their definition, by the
   part that first calls them. */

/* display.c */
void btEffectAdvanceScrollOnInterval(void);
void btEffectConfigureEffectDisp(void);
void btEffectAdvanceBg2OnInterval(void);

/* canvas.c */
void btEffectStartCanvasLayer(Sint32 bgControl);
/* Closes the canvas layer. */
void btEffectEndCanvasLayer(void);
/* Counts down the actors' flash timers. */
void btEffectCountDownBattlerTints(void);
void btEffectStartTiledCanvas(Sint32 bgControl);
void btEffectStartTiledCanvasFilled(Sint32 bgControl);
void btEffectProcessPendingBlock2Transfer(void);
void btDispApplyBg2Affine(void);
/* Sets up the canvas tile map on BG1. */
void btEffectSetupCanvasTileMap(void);
/* Marks the transition done and shows the battle view again. */
void btEffectHideBg1AndRestoreBackdrop(void);
void btEffectTransferFullScreenImage(void);
void btEffectDrawClippedCanvasLine(
	Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 col);
void btEffectBuildCanvasTilemap(void);
/* The 64x64 stamp Haures draws in 256 colours: the VRAM address of its sprite's
   tiles, two words before the state's end, past what struct BATTLE_EFFECT_STATE
   names, and the picture drawn for them, the 4 KB before transferMode. */
#define STAMP_TILES(state) \
	(*(Uint32 *)((Uint8 *)(state) + BATTLE_EFFECT_STATE_SIZE - \
		2 * sizeof(Uint32)))
#define STAMP_CANVAS(state) \
	((Uint8 *)(state) + offsetof(BATTLE_EFFECT_STATE, transferMode) - \
		OBJ_64X64_8BPP_BYTES)
void btEffectProcessPendingStampTransfer(void);

/* djinn_beams.c Fetches the max and additive blitters, flipped or not. */
void btEffectBuildCanvasBlitters(Sint32 flip, DRAW_RECTANGLE *out);

/* prism.c */
void btEffectUpdatePhasePalette(
	Sint32 frm, Sint32 redPhase, Sint32 greenPhase, Sint32 bluePhase);

/* djinn_heal.c state->cameraPhase, which btCameraApplyPhasedDelta and the
   effects that turn the camera through it (Fire, Froth) pass between them. */
typedef enum CAMERA_PHASE {
	CAMERA_PHASE_IDLE, /* nothing marked since the last turn */
	/* the effect has finished a tick: turn the whole delta */
	CAMERA_PHASE_TICK_DONE,
	CAMERA_PHASE_HALF_TURNED /* half a delta turned on an unmarked tick */
} CAMERA_PHASE;

void btCameraApplyPhasedDelta(void);

/* dispatch.c */
void btEffectShowLivingTargets(BATTLE_EFFECT_PARM *args);
void btEffectCallNoOperation(Sint32 cnt);
void btEffectBuildWindowEdgeTable(void);

/* battler_tint.c Shows a battler in a palette and a pose, for `ticks` ticks
   counted in timer slot `tmrSlot`; TINT_KEEP leaves any of them as it is. */
void btEffectTintBattler(
	Sint32 unitId, Sint32 palette, Sint32 pose, Sint32 tmrSlot, Sint32 ticks);
void btEffectSetupBlendedDisp(void);

/* stat_change.c */
void btEffectRunImpair(BATTLE_EFFECT_PARM *args);
void btEffectRunDull(BATTLE_EFFECT_PARM *args);
void btEffectRunWeaken(BATTLE_EFFECT_PARM *args);

/* eruption.c */
void btEffectCreateSprs(Sint32 cnt, Sint32 sprId, Uint32 pri);

/* projectiles.c */
void btParticleAdvanceWithGravity3D(
	PARTICLE *step, Sint32 damping, Sint32 gravity);
void btParticleAdvanceWithGravity2D(
	PARTICLE *step, Sint32 damping, Sint32 gravity);
void btEffectRunDjinniEntrance(
	BATTLE_EFFECT_PARM *effect, Uint32 packed, Sint32 *screen, Sint32 *world);
void btEffectSetApproachMotion(
	Sint32 id, Sint32 tgtId, Sint32 ticks, Sint32 rate);

/* djinn_attacks.c */
void btResourceLoadAndDecompress(
	Sint32 id, void *dst, Sint32 skipPalette, Sint32 copyPalette);

/* hail.c Shakes the camera while state->shakeFrm lasts. */
void btCameraShake(Sint32 rangeX, Uint32 rangeY);
void btCameraApplyVerticalShake(Sint32 range);

/* gaia.c */
SPR_ANIMATION *btAnimationObjectAllocateEmpty(void);

/* spider_web.c */
void btEffectStartActorMotion(Sint32 id, Sint32 force, Sint32 acceleration,
	Sint32 speedLimit, Sint32 velY, Sint32 percent);
void btRenderBuildQuadStrip(
	struct STRIP_QUAD *quad, Sint32 width, Sint32 height, Sint32 cnt);
void btEffectRunSwirlSwipe(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btPaletteMirrorBgCols(void);
void btGraphicsLoadGreyRamp(void);

/* azul.c */
void btEffectPosProjectBattler(Sint32 id, SCREEN_POINT *pos);
/* The middle between the first and the last target. */
void btEffectGetTargetsMidpoint(SCREEN_POINT *pos);
void btMeshBuildGrid(MESH_TRIANGLE *triangles, Sint32 columns, Sint32 rows,
	Sint32 cellWidth, Sint32 cellHeight);
void btColBufferScaleStub(void *src, void *dst, Sint32 scale, Sint32 cnt);
void btEffectPosProjectSpr(Sint32 id, SCREEN_POINT *pos);

/* positions.c */
void btEffectRunCastingImpact(BATTLE_EFFECT_PARM *action, Sint32 kind);
void btEffectRunCriticalHit(BATTLE_EFFECT_PARM *effect);
void btEffectRunAttackSlash(BATTLE_EFFECT_PARM *effect);

/* casting_impact.c Steps the background colours one shade toward a resource's
   palette. */
void btEffectStepPaletteToResource(Sint32 resource);

/* canvas_wipe.c */
void btEffectWipeCanvas(Sint32 direction, Sint32 fill);

/* particle_streams.c */
void btEffectPlaceFormationSprs(Sint32 formation, Sint32 x, Sint32 z);
void btEffectRunImpactBurst(Sint32 formation, Sint32 x, FIXED y);
void btEffectArmPaletteHBlankDMA(void);
void fdRenderApplyProjectedPlacementStub(
	SPR *spr, Sint32 *pos, Sint32 *scale, Sint32 mode);

/* ramses.c An actor's recoil motion, through a stub call. */
void btMotionApplyKnockbackStub(Sint32 actor, Sint32 motion);

/* ragnarok.c */
void btEffectRunFallingSword(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* thrash.c */
void btEffectRunFlashingStrike(BATTLE_EFFECT_PARM *effect, Sint32 variant);

/* undead_sword.c */
void btPictureMirrorIntoQuadrants(
	Uint8 *src, Uint8 *dst, Sint32 width, Sint32 height);
void btEffectRunWaterSheet(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunWindFunnel(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* heavy_divide.c */
void btEffectRunHeavyBlow(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* thunder_mine.c */
void btEffectRunBallLightning(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* whorl.c */
void btEffectRunTwistingColumn(BATTLE_EFFECT_PARM *effect, Uint32 mode);
void btEffectRunDjinniCharge(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* rime.c */
void btEffectRunDjinniSign(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* gasp.c */
void btEffectRunGaspFamily(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* raging_heat.c */
void btEffectFillWhiteBand(void);

/* star_mine.c */
void btEffectBuildRepeatingTilemap(void);

/* rushing_strike.c */
void btEffectRunRushingStrike(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunMirroredWall(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* vengeance.c */
void btEffectRunGeyser(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunStarSwarm(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunBubbleColumn(BATTLE_EFFECT_PARM *effect);

/* legend.c */
void btEffectRunElementCards(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* cards.c */
void btEffectRunCards(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* breaths.c */
void btCanvasBlitMirroredQuarters(void *src, Sint32 x, Sint32 y, Sint32 size);
void btEffectRunBreaths(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunCrescentSlash(BATTLE_EFFECT_PARM *effect, Sint32 mode);

/* trident.c */
void btEffectRunLeapingStrike(BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectArmMeteorBlowPaletteDMA(void);

/* liquifier.c */
void btEffectRunBallAttack(BATTLE_EFFECT_PARM *effect);

/* odyssey.c The five-argument blitter builder's stub call, which only a call in
   odyssey.c that is never taken names. */
Sint32 btEffectBuildBlitterStub(Sint32 slot, Sint32 widthShift,
	Sint32 heightShift, Sint32 flg, Uint32 mode);
void   btIrisWipeScreen(Uint8 side, Uint8 speed, void (*midpoint)(void));
void   btIrisRestoreBattle(void);
void   btEffectBuildSteepShadeRampWave(
	  Uint8 *tbl, Uint32 cnt, Uint32 repeats, Sint32 scale);

/* moloch.c */
RENDER_MODEL *btRenderAllocateModelList(Sint32 cnt);
void btRenderTransformVertices(const void *corners, void *vertices, Sint32 cnt);

/* target_motes.c Glowing orbs spiral in on each target.

   Abilities: Mystic Flame. */
void btEffectRunMysticFlame(BATTLE_EFFECT_PARM *args);

/* Functions of other modules the parts call. */
Sint32 fdSprStartAnimationStub(void *object, Sint32 value);
void   btActorSpawnObjectsForListStub(Sint16 *list, Sint32 mode);
void   btBackgroundLoadStub(Sint32 mode, Sint32 resource, Sint32 level);
void   btBackgroundSetModeStub(Sint32 mode, Sint32 resource, Sint32 speed);
void   btBackgroundSetPaletteLevelStub(Sint32 background, Sint32 level);
/* Turns the battle camera, over `ticks` ticks, to look at the point between
   two units. */
void btCameraSetViewAtPairMidpointStub(Sint32 actor, Sint32 tgt, Sint32 ticks);
void btCameraSetViewStub(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom);
void btObjectPutStub(void);
/* Spawns the battle objects of a list of units through its stub call. */
void btObjectSpawnObjectsForListStub(
	Sint16 *unitIds, Sint32 refreshPoses, Sint32 releaseOthers);
/* btPlaybackStart through its stub call. */
void btPlaybackStartStub(Sint32 phase);
/* Whether an enemy's sprite takes the heavy impact, through its stub call. */
Sint32 btSprHasHeavyImpactStub(Sint32 enemyId);
Sint32 btUnitGetEffectHeightStub(Sint32 unit);
void  *btActorGetSprStub(void *object, Sint32 idx);
void   btObjectDestroySprsStub(Sint32 actor);
void   fdSprRemoveAnimationStub(void *record, Sint32 entry);
void   fdActorShadowLoadTilesStub(void);
void   fdActorShadowFreeTilesStub(void);

/* The tables of effect_table.c, in its order. */

/* Where each size of mote and of flare starts in its sprite sheet, in bytes. */
extern Uint16 BattleEffectMotePictures[];
extern Uint16 BattleEffectFlarePictures[];
/* Where each of Cruel Ruin's circles starts in its sprite sheet. */
extern const Uint16 CruelRuinCirclePictures[];
/* The pictures several effects share: the sparkles, the ember puff that rises
   in six pictures, and the six glints, with their sizes and offsets. */
extern Uint16 BattleEffectSparklePictures[];
extern Uint8  BattleEffectSparkleSizes[];
extern Uint8  BattleEffectEmberStreakPictureWidths[];
extern Uint8  BattleEffectEmberStreakPictureHeights[];
extern Uint8  BattleEffectEmberStreakPictureBiasY[];
extern Uint16 BattleEffectEmberStreakPictures[];
extern Uint16 BattleEffectGlintPictures[];
extern Uint8  BattleEffectGlintPictureWidths[];
extern Uint8  BattleEffectGlintPictureHeights[];
/* Spire's shard pictures. */
extern Uint8  SpireShardWidths[];
extern Uint8  SpireShardHeights[];
extern Uint16 SpireShardPictures[];
/* The stat change's shard pictures. */
extern Uint16 StatChangeShardPictures[];
extern Uint8  StatChangeShardWidths[];
extern Uint8  StatChangeShardHeights[];
/* Where the Death Card's Reaper draws its seven panels, seven x for each
   side. */
extern Uint8 ReaperPictureX[];
extern Uint8 ReaperPictureY[];
/* Ice's shards and ticks, and Douse's drops, by variant. */
extern Uint8 IceCounts[];
extern Uint8 DouseCounts[];
/* The streaks' styles, and the struck target's tremble. */
extern Sint8 StreakStyles[];
extern Sint8 StreakTremble[];
/* The target strikes: their styles (TARGET_STRIKE_COLUMN), the slash pictures
   and how far each reaches, the bite's stages and the glints' flips. */
extern Uint8  TargetStrikeStyles[][7];
extern Uint8  TargetStrikeWidths[];
extern Uint8  TargetStrikeHeights[];
extern Uint16 TargetStrikePictures[];
extern Uint8  TargetStrikeReach[];
extern Uint8  TargetStrikeBiteStages[];
extern Uint8  TargetStrikeGlintDrawFlags[];
/* The 33 pieces the screen shatters into as Death Plunge opens: their pictures
   and where they start. */
extern Uint8  ShatteringOpeningPieceWidths[];
extern Uint8  ShatteringOpeningPieceHeights[];
extern Uint16 ShatteringOpeningPiecePictures[];
extern Uint8  ShatteringOpeningPieceStartX[];
extern Uint8  ShatteringOpeningPieceStartY[];
/* The element burst's four corner sparks: their places and flips. */
extern Sint8 ElementBurstSparkX[];
extern Sint8 ElementBurstSparkY[];
extern Uint8 ElementBurstGlintFlips[];
/* Cruel Ruin: the streak's corners, the discs' turns, the pillars, the blasts,
   and each streak's start, size, path and roll. */
extern const MODEL_VERTEX CruelRuinStreakCorners[];
extern const Uint16		  CruelRuinDiscPitches[];
extern const Uint16		  CruelRuinDiscYaws[];
extern const Uint8		  CruelRuinPillarSways[];
extern const Uint8		  CruelRuinPillarWidths[];
extern const Uint8		  CruelRuinBlastTicks[];
extern const Uint8		  CruelRuinBlastSizes[];
extern const Uint8		  CruelRuinBlastX[];
extern const Uint8		  CruelRuinBlastY[];
extern const Uint8		  CruelRuinStreakStarts[];
extern const Uint8		  CruelRuinStreakSizes[];
extern const Uint8		  CruelRuinStreakX[];
extern const Uint8		  CruelRuinStreakStepX[];
extern const Uint8		  CruelRuinStreakStepY[];
extern const Uint8		  CruelRuinStreakEndY[];
extern const Sint32		  CruelRuinStreakRolls[];
/* Eclipse: the glints' flips, the dragon's sprites that take new tiles as it
   turns and their places, the canvas's fills, and the rings. */
extern const Uint8	EclipseGlintFlips[];
extern const Uint8	EclipseTileObjects[];
extern const Uint8	EclipseFillTicks[];
extern const Uint32 EclipseFillLevels[];
extern const Uint8	EclipseObjectX[];
extern const Uint8	EclipseObjectZ[];
extern const Uint8	EclipseRingGrowth[];
extern const Sint8	EclipseRingHeights[];
/* The form change's four waves: when each starts, how fast it grows and how far
   it is stretched. */
extern Uint8 FormChangeWaveStarts[];
extern Uint8 FormChangeWaveGrowth[];
extern Uint8 FormChangeWaveStretch[];
/* Prism's crystal and the shards it breaks into: their places and pictures. */
extern Uint8  PrismShardStarts[];
extern Uint8  PrismShardWidths[];
extern Uint8  PrismShardHeights[];
extern Sint32 PrismShardPictures[];
/* The gathering lights' motes for each target, and the ticks before the
   targets' share, by mode. */
extern Uint8 GatheringLightsCounts[];
/* The Djinn beams: the motes' pictures as they sway, and the beam head's three
   pictures. */
extern Uint8  DjinnBeamsMotePictures[];
extern Uint16 DjinnBeamsHeadPictures[];
extern Uint8  DjinnBeamsHeadWidths[];
extern Uint8  DjinnBeamsHeadHeights[];
/* The lobbed shells: each style's record (LOBBED_SHELLS_SHATTER and the rest),
   and the shards' pictures. */
extern Sint8  LobbedShellsStyles[];
extern Uint8  LobbedShellsShardWidths[];
extern Uint8  LobbedShellsShardHeights[];
extern Uint16 LobbedShellsShardPictures[];
/* Where Condemn draws its seven pictures of the reaper and his crescent, by
   side. */
extern Uint8 CondemnPictureX[];
extern Uint8 CondemnPictureY[];
/* Bind's triangles: the first corner of each. */
extern Sint32 BindVertex[];
extern Sint32 BindTriangleStrikeVertex[];
/* The star's ring: its points and radii, alternating. */
extern Sint32 StarPoints[];
extern Uint8  StarRadii[];
/* Procne: the grid her nine sprites sit on, the four strikes' ticks and a
   strike's three pictures. */
extern Uint8  ProcneGridX[];
extern Uint8  ProcneGridY[];
extern Uint16 ProcneTicks[];
extern Uint8  ProcnePictureWidths[];
extern Uint8  ProcnePictureHeights[];
extern Uint8  ProcnePictureBiasY[];
extern Uint16 ProcnePictures[];
/* Tiamat: the embers' speeds and sizes, a spark's two pixels, and the flame
   streak's six pictures. */
extern Uint8  TiamatEmberSpeeds[];
extern Uint8  TiamatSparkPixels[];
extern Uint8  TiamatEmberSizes[];
extern Uint8  TiamatEmberStreakWidths[];
extern Uint8  TiamatEmberStreakHeights[];
extern Uint8  TiamatEmberStreakBiasY[];
extern Uint16 TiamatEmberStreakPictures[];
/* Nereid: the three water columns' feet and the falling sparks' gravity. */
extern Uint8  NereidColumns[];
extern Sint32 NereidGravity[];
/* The vortices: Froth's counts and the whirlwind's shapes, by variant (the
   FROTH_ and WHIRLWIND_ columns). */
extern Uint8 VorticesCounts[][2];
extern Uint8 VorticesShapes[][3];
/* Prism's crystals, ticks and damage tick, by variant. */
extern Uint8 PrismCounts[];
/* Plasma: the sparks and shake of each lightning pillar, the columns, and the
   counts, by variant. */
extern Uint16 PlasmaSparks[];
extern Uint8  PlasmaColumns[];
extern Uint8  PlasmaCounts[];
/* The spark groups' shapes, by variant: sparks in a group, groups and each
   group's x. */
extern Uint16 SparkGroupsShapes[][5];
/* Plasma's flash burst picture for each three ticks of a flash. */
extern Uint8 PlasmaFlashPictures[];
/* Volcano's pillars of fire: how many, their depths and when each erupts. */
extern Uint8 VolcanoCounts[];
extern Sint8 VolcanoDepths[][4];
extern Uint8 VolcanoStartTicks[];
/* The target motes' glint flips. */
extern Uint8 TargetMotesGlintFlips[];
/* Zagan: where his sprites sit as he charges and as he gallops, and his dust
   rings. */
extern const Uint16 ZaganChargePoints[][2];
extern const Uint16 ZaganGallopPoints[][2];
extern const Uint8	ZaganRingStarts[];
extern const Uint8	ZaganRingGrowth[];
extern const Sint8	ZaganRingHeights[];
/* Cybele: her sprites' poses, and the pictures of her thorns, flowers, chips,
   sparks and blasts. */
extern Uint8  CybeleObjectPoses[];
extern Uint16 CybeleThornPictures[];
extern Uint8  CybeleThornWidths[];
extern Uint8  CybeleThornHeights[];
extern Uint16 CybeleFlowerPictures[];
extern Uint8  CybeleFlowerSizes[];
extern Uint16 CybeleChipPictures[];
extern Uint8  CybeleChipWidths[];
extern Uint8  CybeleChipHeights[];
extern Uint16 CybeleSparkPictures[];
extern Uint8  CybeleSparkSizes[];
extern Uint16 CybeleBlastPictures[];
extern Uint16 CybeleBlastSizes[];
/* Boreas: the crystals' points, the shake, his sprites' animations, and the
   pictures of the crystals, debris, spray, rising crystals and falls. */
extern Uint8  BoreasPoints[][2];
extern Sint8  BoreasShakeOffsets[];
extern Uint16 BoreasCrystalPictures[];
extern Uint8  BoreasCrystalWidths[];
extern Uint8  BoreasCrystalHeights[];
extern Uint8  BoreasAnimations[];
extern Uint8  BoreasDebrisWidths[];
extern Uint8  BoreasDebrisHeights[];
extern Uint16 BoreasDebrisPictures[];
extern Uint8  BoreasSprayWidths[];
extern Uint8  BoreasSprayHeights[];
extern Uint16 BoreasSprayPictures[];
extern Uint16 BoreasRisingCrystalPictures[];
extern Uint8  BoreasRisingCrystalWidths[];
extern Uint8  BoreasRisingCrystalHeights[];
extern Uint8  BoreasFallWidths[];
extern Uint8  BoreasFallHeights[];
extern Uint16 BoreasFallPictures[];
/* Drain's motes: their gravities, and their size by mode. */
extern Sint32 DrainGravities[];
extern Uint8  DrainMotesSizes[];
/* Quake's ticks, rise and shake, by variant. */
extern Uint8 QuakeTimings[][3];
/* Fire's counts by variant, and its flare pictures. */
extern Uint8  FireCounts[];
extern Uint8  FireEmberStreakWidths[];
extern Uint8  FireEmberStreakHeights[];
extern Uint8  FireEmberStreakBiasY[];
extern Uint16 FireEmberStreakPictures[];
/* Frost: where the mounds rise, its timings by variant, and the pictures of its
   motes, mounds and ice chips. */
extern Uint8  FrostMoundSpots[];
extern Uint8  FrostTimings[];
extern Uint8  FrostMoteWidths[];
extern Uint8  FrostMoteHeights[];
extern Uint16 FrostMoteCells[];
extern Uint16 FrostMoundPictures[];
extern Uint8  FrostMoundHeights[];
extern Uint8  FrostChipWidths[];
extern Uint8  FrostChipHeights[];
extern Uint16 FrostChipCells[];
/* The sparks each of Ray's bolts throws a tick, by variant. */
extern Uint8 RaySparks[][2];
/* Eruption's counts by variant, and its puffs' pictures. */
extern Uint8  EruptionCounts[][3];
extern Uint16 EruptionPuffPictures[];
extern Uint16 EruptionPuffSizes[];
/* The burst shower's glint flips. */
extern Uint8 BurstShowerGlintFlips[];
/* Atalanta's sprites' scale, one to one. */
extern Sint32 AtalantaUnitScale[];
/* Growth: the vines' and pillars' pictures, the columns by strength and where
   they root, how tall they grow, and the dust puffs. */
extern Uint8  GrowthVineWidths[];
extern Uint8  GrowthHeightLimits[];
extern Uint16 GrowthVinePictures[];
extern Uint8  GrowthPillarWidths[];
extern Uint16 GrowthPillarPictures[];
extern Uint8  GrowthColumnCounts[];
extern Uint8  GrowthColumnX[];
extern Sint8  GrowthPillarHeights[];
extern Sint8  GrowthPuffWidths[];
extern Uint8  GrowthPuffHeights[];
extern Uint16 GrowthPuffPictures[];
/* Each vine's x about the middle of the targets. */
extern const Sint8 VineColumnOffsets[];
/* Each thorn's kind (THORN_MIRRORED, THORN_SMALL). */
extern Uint8 ThornKinds[];
/* A thorn's kind in ThornKinds. */
#define THORN_MIRRORED 1 /* drawn mirrored across */
#define THORN_SMALL	   2 /* 32 pixels wide, not 48 */
/* The thorns: where each roots, how many by strength, and the dust puffs as
   each bursts out. */
extern Sint8  Thorn_X[];
extern Uint8  ThornCounts[];
extern Sint8  ThornPuffWidths[];
extern Uint8  ThornPuffHeights[];
extern Uint16 ThornPuffPictures[];
/* Bolt: its sparks, glints, spread and bolts by strength, and how its glints
   are flipped and blended. */
extern Uint8 BoltCounts[];
extern Uint8 BoltGlintFlips[];
extern Uint8 BoltGlintModes[];
/* The projectile volley: the comets' heights, each mode's record (enum
   VOLLEY_FIELD), the hit flash's pictures and spread, the pellets' pictures and
   the glints' flips. */
extern Uint8  ProjectileVolleyCometHeights[];
extern Uint8  ProjectileVolleyModes[][PROJECTILE_VOLLEY_COLUMNS];
extern Uint8  ProjectileVolleyFlashWidths[];
extern Uint8  ProjectileVolleyFlashHeights[];
extern Uint8  ProjectileVolleyFlashSteps[];
extern Uint8  ProjectileVolleyFlashRises[];
extern Uint16 ProjectileVolleyFlashPictures[];
extern Uint16 ProjectileVolleyPelletPictures[];
extern Uint8  ProjectileVolleyGlintFlips[];
/* The ticks the Djinni throws its five flames on. */
extern const Uint8 DjinnAttacksLaunchTicks[];
/* Hail's four ice chip pictures. */
extern Uint8  HailPictureWidths[];
extern Uint8  HailPictureHeights[];
extern Uint16 HailPictures[];
/* Ground: the crackles' flips, and the portals' pictures and places. */
extern const Uint8	GroundCrackleFlips[];
extern const Uint8	GroundPortalWidths[];
extern const Uint8	GroundPortalHeights[];
extern const Uint16 GroundPortalPictures[];
extern const Uint8	GroundPortalX[];
extern const Uint8	GroundPortalY[];
/* Thor: the palettes a struck target flashes in, and the crackles' flips. */
extern const Uint8 ThorTargetPalettes[];
extern const Uint8 ThorCrackleFlips[];
/* Spire: where each shard sits in the boulder, the boulders' x and drop ticks,
   and how many by variant. */
extern Uint8 SpireShardOffsets[];
extern Sint8 SpireRockX[];
extern Uint8 SpireDropTicks[];
extern Uint8 SpireRockCounts[];
/* The flurries: their styles (FLURRY_COLUMN) and the glints' flips. */
extern Uint8	   FlurryStyles[][8];
extern const Uint8 FlurryGlintFlips[];
/* Gaia: the growing wall's rows, the flame's pictures, the wall layer's shift
   and the late rocks. */
extern Uint16 GaiaRowPictures[];
extern Uint8  GaiaRowWidths[];
extern Uint16 GaiaFlamePictures[];
extern Uint8  GaiaFlameWidths[];
extern Uint8  GaiaFlameHeights[];
extern Sint8  GaiaBg2Shifts[][3];
extern Uint8  GaiaLateRockColumns[];
/* Ulysses: where the mage's sprites sit, the summon's pictures, and where its
   six particles start. */
extern const Uint8	UlyssesObjectX[];
extern const Uint8	UlyssesObjectZ[];
extern const Uint8	UlyssesPictureWidths[];
extern const Uint8	UlyssesPictureHeights[];
extern const Uint16 UlyssesPictures[];
extern const Sint8	UlyssesParticleStarts[][5];
/* The swirl swipe's six pictures: their sizes, pictures, x and rise. */
extern const Uint8	SwirlSwipePictureWidths[];
extern const Uint8	SwirlSwipePictureHeights[];
extern const Uint16 SwirlSwipePictures[];
extern const Uint8	SwirlSwipePictureX[];
extern const Sint8	SwirlSwipePictureBiasY[];
/* Azul: the points of the battlefield's shattered parts, and the serpent's
   sprites, early and late. */
extern const Uint8 AzulPartPoints[][2];
extern const SCALE AzulUnitScale;
extern const Uint8 AzulObjectX[];
extern const Uint8 AzulObjectZ[];
extern const SCALE AzulLateUnitScale;
extern const Sint8 AzulLateObjectX[];
extern const Sint8 AzulLateObjectZ[];
/* The plain attacks' beam: its six pictures and how far each reaches. */
extern Uint8  BeamPictureWidths[];
extern Uint8  BeamPictureHeights[];
extern Uint16 BeamPictures[];
extern Uint8  BeamPictureReaches[];
/* The casting impacts: glint flips, the bite's pictures, Raiden's Wrath's and
   Apocalypse's pictures and places, the ice chips, and Stun Jip's bands and
   Armor Crush's waves. */
extern const Uint8	CastingImpactSquallGlintFlips[];
extern const Uint8	CastingImpactSapGlintFlips[];
extern const Uint8	CastingImpactBitePictures[];
extern const Uint16 CastingImpactRaidensWrathPictures[];
extern const Uint16 CastingImpactRaidensWrathWidths[];
extern const Uint16 CastingImpactRaidensWrathHeights[];
extern Uint16		CastingImpactRaidensWrathX[];
extern Uint16		CastingImpactRaidensWrathY[];
extern const Uint16 CastingImpactApocalypsePictures[];
extern const Uint16 CastingImpactApocalypseWidths[];
extern const Uint16 CastingImpactApocalypseHeights[];
extern const Uint16 CastingImpactApocalypseX[];
extern Uint16		CastingImpactApocalypseY[];
extern const Uint16 CastingImpactApocalypseBurstPictures[];
extern const Uint8	CastingImpactApocalypseBurstWidths[];
extern const Uint8	CastingImpactApocalypseBurstHeights[];
extern Uint8		CastingImpactApocalypseBurstPoints[][2];
extern const Uint16 CastingImpactIceChipPictures[];
extern const Uint8	CastingImpactStunBoltGlintFlips[];
extern const Uint8	CastingImpactStunJipBandStarts[];
extern const Uint8	CastingImpactStunJipBandGrowth[];
extern const Sint8	CastingImpactStunJipBandHeights[];
extern const Uint8	CastingImpactArmorCrushWaveStarts[];
extern const Uint8	CastingImpactArmorCrushWaveGrowth[];
extern const Uint8	CastingImpactArmorCrushWaveStretch[];
/* The unused copy of Nereid: its columns and gravity. */
extern Uint8		NereidCopyColumns[];
extern const Sint32 NereidCopyGravity[];
/* The summons' sprite formations, each sprite's x and depth: Nereid's,
   Cybele's, Tiamat's two and Megaera's. */
extern const Uint8 SummonFormationNereidX[];
extern const Uint8 SummonFormationNereidZ[];
extern const Uint8 SummonFormationCybeleX[];
extern const Uint8 SummonFormationCybeleZ[];
extern const Uint8 SummonFormationTiamatAlternateX[];
extern const Uint8 SummonFormationTiamatAlternateZ[];
extern const Uint8 SummonFormationTiamatX[];
extern const Uint8 SummonFormationTiamatZ[];
extern const Sint8 SummonFormationMegaeraX[];
extern const Uint8 SummonFormationMegaeraZ[];
/* The impact burst's pictures. */
extern Uint8  ImpactBurstPictureWidths[];
extern Uint8  ImpactBurstPictureHeights[];
extern Uint16 ImpactBurstPictures[];
/* The starfield fall: the orbiting sparks, the fireballs and the flash
   pictures. */
extern Sint16 StarfieldFallOrbitPoints[][3];
extern Sint16 StarfieldFallFireballPoints[][2];
extern Uint16 StarfieldFallFlashPictures[];
extern Uint16 StarfieldFallFlashSizes[];
/* Ramses: where each of the statue's sprites sits, and the puffs' pictures. */
extern Uint8  RamsesStatueColumns[];
extern Uint8  RamsesStatueRows[];
extern Uint16 RamsesPuffPictures[];
extern Uint16 RamsesPuffSizes[];
/* Ragnarok: the strikes' columns and the flash's pictures. */
extern Uint8 RagnarokStrikeColumns[];
extern Uint8 RagnarokFlashPictures[];
/* Titan Blade: the flash's pictures and the dust's gravities. */
extern Uint8  TitanBladeFlashPictures[];
extern Sint32 TitanBladeDustGravity[];
/* Judgment: the sprites its shield is copied into, and the shards' pictures. */
extern const Uint8	JudgmentShieldSprites[];
extern const Uint16 JudgmentShardPictures[];
extern const Uint8	JudgmentShardWidths[];
extern const Uint8	JudgmentShardHeights[];
/* Kirin: the row's x offsets and heights. */
extern Uint8 KirinColumns[];
extern Uint8 KirinRows[];
/* Neptune's spray: its four gravities. */
extern Sint32 NeptuneGravity[];
/* Undead Sword's sparks: their pictures and sizes. */
extern const Uint16 UndeadSwordPuffPictures[];
extern const Uint16 UndeadSwordPuffSizes[];
/* The wind funnels, by mode. */
extern const Uint8 WindFunnelCounts[];
/* Diamond Berg: its ripples, and its flashes' roll and pitch. */
extern const Uint8	DiamondBergRingStarts[4];
extern const Uint8	DiamondBergRingGrowth[4];
extern const Sint8	DiamondBergRingHeights[4];
extern const Sint16 DiamondBergShardRolls[8];
extern const Sint16 DiamondBergShardPitches[8];
/* Geode's rings: when each starts, how fast it grows, and its height. */
extern const Uint8 GeodeWaveStarts[];
extern const Uint8 GeodeWaveGrowth[];
extern const Sint8 GeodeWaveHeights[];
/* Soul Shatter's sparks: their pictures and sizes. */
extern Uint16 SoulShatterSparkPictures[];
extern Uint16 SoulShatterSparkSizes[];
/* Serac: the spire's rings, and the ripples of mist. */
extern const Sint8 SeracRowHeights[];
extern const Sint8 SeracRowRadii[];
extern const Uint8 SeracRingStarts[];
extern const Uint8 SeracRingGrowth[];
extern const Sint8 SeracRingHeights[];
/* The heavy blow: its strike tick and chips, by mode, and a chip's two
   pixels. */
extern Uint8	   HeavyBlowStrikeTicks[];
extern Uint8	   HeavyBlowChipCounts[];
extern const Uint8 HeavyBlowChipPixels[];
/* Cool: its columns of ice and its sparks. */
extern Sint16 CoolColumnX[];
extern Uint8  CoolColumnStarts[];
extern Uint8  CoolColumnHeights[];
extern Uint8  CoolColumnLeans[];
extern Uint16 CoolSparkPictures[];
extern Uint16 CoolSparkSizes[];
/* Wyrd Curse's drops: their bounce speeds. */
extern const Sint8 WyrdCurseDropBounces[];
/* The ball lightning's glint flips. */
extern const Uint8 BoltTrailGlintFlips[];
/* Megaera's glints: their flips and how they are blended, by variant. */
extern const Uint8 MegaeraGlintFlips[];
extern const Uint8 MegaeraGlintModes[];
/* Light Surge's four rings: their full sizes, shades and sways. */
extern const Sint32 LightSurgeRingSizes[4];
extern const Sint8	LightSurgeRingShades[4];
extern Sint8		LightSurgeRingSways[4];
/* The Djinni charge: the Djinni's entrance by mode, and the debris's
   gravities. */
extern const Uint8	DjinniChargeEntranceFlags[];
extern const Sint32 DjinniChargeDebrisGravity[4];
/* The Gasp family: the streak's steps and mesh, and the counts by variant. */
extern const Sint8	GaspFamilyStreakSteps[];
extern const Uint16 GaspFamilyStreakMesh[];
extern const Uint8	GaspFamilyCounts[];
/* The ticks Charon's canvas flashes. */
extern Uint8 CharonFlashTicks[];
/* Models several effects share: a band's and a cylinder's corners. */
extern const MODEL_VERTEX BattleEffectOctagonBandCorners[];
extern const MODEL_VERTEX BattleEffectCylinderCorners[];
/* The casting impacts' band mesh. */
extern const Uint16 CastingImpactBandMesh[];
/* The geyser's mesh. */
extern const Uint16 GeyserMesh[];
/* The cylinder's mesh, the ripples and rings several effects spread. */
extern const Uint16 BattleEffectCylinderMesh[];
/* Geode's ring's corners. */
extern const MODEL_VERTEX GeodeRingCorners[];
/* Models several effects share: a ring, a triangle, beams and squares, their
   corners and meshes. */
extern const MODEL_VERTEX BattleEffectRingCorners[];
extern const Uint16		  BattleEffectRingMesh[];
extern const Uint16		  BattleEffectTriangleMesh[];
extern const MODEL_VERTEX BattleEffectTriangleCorners[];
extern const MODEL_VERTEX BattleEffectTaperedBeamCorners[];
extern const MODEL_VERTEX BattleEffectBeamCorners[];
extern const MODEL_VERTEX BattleEffectLongBeamCorners[];
extern const MODEL_VERTEX BattleEffectSquareCorners[];
extern const MODEL_VERTEX BattleEffectStandingSquareCorners[];
/* A flame of Legend's fan: its corners. */
extern MODEL_VERTEX LegendFanFlameCorners[];
/* Models several effects share: a square on the ground, and the meshes of small
   squares and a column. */
extern const MODEL_VERTEX BattleEffectGroundSquareCorners[];
extern const Uint16		  BattleEffectSmallSquareMesh[];
extern const Uint16		  BattleEffectSmallSquareMeshReversed[];
extern const Uint16		  BattleEffectColumnMesh[];
/* The heavy blow's sword mesh. */
extern const Uint16 HeavyBlowSwordMesh[];
/* A tall quad's mesh. */
extern const Uint16 BattleEffectTallMesh[];
/* Haures's band mesh. */
extern const Uint16 HauresBandMesh[];
/* Catastrophe's burst mesh. */
extern const Uint16 CatastropheForkMesh[];
/* The square meshes several effects share. */
extern const Uint16 BattleEffectSquareMesh[];
extern const Uint16 BattleEffectLargeSquareMesh[];
/* Haures's burst mesh. */
extern const Uint16 HauresBurstMesh[];
/* Heartbreak: the burst of sparks at the figure's hand, and the heart in its
   fist burning away. */
extern Uint8  HeartbreakSparkBurstWidths[];
extern Uint8  HeartbreakSparkBurstHeights[];
extern Uint16 HeartbreakSparkBurstPictures[];
extern Uint8  HeartbreakHeartBurnHeights[];
extern Uint8  HeartbreakHeartBurnY[];
extern Uint16 HeartbreakHeartBurnPictures[];
/* Raging Heat's sparks, spread, rings and embers, by level. */
extern const Uint8 RagingHeatStyles[][4];
/* Juggle's styles, one row a mode. */
extern Uint8 JuggleStyles[][7];
/* Flora: the petals' pictures, and when and where each bloom opens and how it
   is turned. */
extern const Uint8	FloraPetalSizes[];
extern const Uint16 FloraPetalPictures[];
extern const Uint16 FloraBloomTicks[];
extern const Sint16 FloraBloomRolls[];
extern const Sint16 FloraBloomPitches[];
extern const Sint8	FloraBloomX[];
extern const Sint8	FloraBloomY[];
/* Sabre Dance's dagger: its mesh and corners. */
extern const Uint16		  SabreDanceDaggerMesh[];
extern const MODEL_VERTEX SabreDanceDaggerCorners[];
/* Fume's levels, one row a level (FUME_LEVEL_COLUMN). */
extern const Uint16 FumeLevels[][FUME_COLUMNS];
/* Ocean Fist's early drops: their gravities. */
extern const Sint32 OceanFistDropGravity[4];
/* The rushing strike's streak scale, by mode. */
extern const Sint32 RushingStrikeScales[];
/* The mirrored wall: its scale by mode and its bursts' shades. */
extern const Sint32 MirroredWallScales[];
extern const Sint32 MirroredWallShades[];
/* Daedalus: the shake, the birds' wing beats and drifts, the clatters, the
   rings, the leg, the splashes and the dives. */
extern Sint8  DaedalusShakeOffsets[];
extern Uint8  DaedalusWingBeatPictures[];
extern Uint8  DaedalusBirdStarts[][2];
extern Sint32 DaedalusBirdSpeeds[][2];
extern Uint8  DaedalusBirdTicks[];
extern Uint16 DaedalusClatterTicks[];
extern Uint16 DaedalusRingStarts[];
extern Sint8  DaedalusRingX[];
extern Uint8  DaedalusRightLegPoints[][2];
extern Uint8  DaedalusSplashPoints[][6][2];
extern Uint16 DaedalusDiveStarts[];
/* The seven ticks Daedalus's strikes land on. */
extern const Uint8 DaedalusStrikeHitTicks[];
/* Which of Vengeance's seven arrows strike, by level. */
extern const Uint8 VengeanceArrowStrikes[][7];
/* Life Shear's scythe: its two blades' meshes and its corners. */
extern const Uint16		  LifeShearPaleBladeMesh[];
extern const Uint16		  LifeShearOrangeBladeMesh[];
extern const MODEL_VERTEX LifeShearBladeCorners[];
/* The geyser's drops: their gravity and smallest picture, by mode (enum
   GEYSER_MODE). */
extern const Sint32 GeyserDropGravity[];
extern const Sint8	GeyserDropMinSizes[];
/* The unused bubble column's glowing oval, fifteen pictures. */
extern Uint8  BubbleColumnPictureWidths[];
extern Uint8  BubbleColumnPictureHeights[];
extern Uint16 BubbleColumnPictures[];
/* Coatlicue: the bubbles' pictures, the bursts of bubbles, the ring's, flashes'
   and glows' ticks, and the coils. */
extern Uint8  CoatlicuePictureWidths[];
extern Uint8  CoatlicuePictureHeights[];
extern Uint16 CoatlicuePictures[];
extern Uint16 CoatlicueBurstTicks[];
extern Uint8  CoatlicueBurstX[];
extern Uint8  CoatlicueBurstY[];
extern Uint8  CoatlicueBurstSpreadX[];
extern Uint8  CoatlicueBurstSpacingY[];
extern Sint8  CoatlicueBurstVelocityX[];
extern Sint8  CoatlicueBurstVelocityY[];
extern Sint8  CoatlicueBurstZ[];
extern Sint8  CoatlicueBurstVelocityZ[];
extern Uint8  CoatlicuePictureBias[];
extern Uint16 CoatlicueRingTicks[];
extern Uint16 CoatlicueFlashTicks[];
extern Uint16 CoatlicueGlowTicks[];
extern Uint8  CoatlicueCoilStarts[];
extern Uint16 CoatlicueCoilScales[];
extern Uint16 CoatlicueCoilRolls[];
extern Uint16 CoatlicueCoilGrowth[];
extern Uint16 CoatlicueCoilMaxScales[];
/* Catastrophe: the flashes, the demon's, armour's and mirrored demon's sprites,
   the bursts of lightning, the strikes and the glints. */
extern const Uint16 CatastropheFlashTicks[];
extern const Uint16 CatastropheFlashLengths[];
extern const Uint8	CatastropheDemonX[];
extern const Uint8	CatastropheDemonZ[];
extern const Uint8	CatastropheArmourX[];
extern const Uint16 CatastropheArmourZ[];
extern const Uint8	CatastropheMirroredDemonX[];
extern const Uint8	CatastropheMirroredDemonZ[];
extern const Uint16 CatastropheForkStarts[];
extern const Uint8	CatastropheForkX[];
extern const Uint8	CatastropheForkY[];
extern const Uint8	CatastropheForkPictures[];
extern const Uint16 CatastropheCanvasFlashTicks[];
extern const Uint8	CatastropheStrikeX[];
extern const Uint8	CatastropheStrikeY[];
extern const Uint16 CatastropheStrikeTicks[];
extern const Uint8	CatastropheStrikeSpeedsY[];
extern const Sint16 CatastropheStrikeRolls[];
extern const Sint16 CatastropheStrikeTurns[];
extern const Sint16 CatastropheStrikePictures[];
extern const Uint8	CatastropheGlintFlips[];
extern const Sint16 CatastropheBoltRolls[];
/* Haures: the particles, his scale, the wings' shapes and morphs, the parts and
   the bands. */
extern const Uint8 HauresParticleX[];
extern Sint32	   HauresUnitScale[];
extern const Uint8 HauresMoundShapes[];
extern Uint8	   HauresMoundMorphSteps[];
extern const Uint8 HauresPartPoints[];
extern const Uint8 HauresBandStarts[];
/* Helm Splitter's pictures, and where each is drawn. */
extern Uint8  HelmSplitterPictureWidths[];
extern Uint8  HelmSplitterPictureHeights[];
extern Uint16 HelmSplitterPictures[];
extern Sint8  HelmSplitterPictureBiasX[];
extern Sint8  HelmSplitterPictureBiasY[];
/* How many of Dragon Cloud's embers are sparks, by variant. */
extern Uint8 DragonCloudSparkCounts[];
/* The element cards: each card's length and how its sparks move. */
extern Uint8		ElementCardsTicks[];
extern SPARK_MOTION ElementCardsSparkMotions[];
/* Triple Chomp's three bites' ticks. */
extern Uint8 TripleChompBiteTicks[];
/* The breaths: their lengths, bursts, glows, jaws and the ticks they hit. */
extern Uint8 BreathTicks[];
extern Uint8 BreathBurstTicks[];
extern Uint8 BreathMaxSizes[];
extern Uint8 BreathRingStarts[];
extern Uint8 BreathRingGrowth[];
extern Sint8 BreathRingX[];
extern Uint8 BreathFlashTicks[];
/* Trident: its rings, each trident's spin, the prong's corners and the glints'
   flips. */
extern const Uint8		  TridentRingStarts[];
extern const Uint8		  TridentRingGrowth[];
extern const Sint8		  TridentRingHeights[];
extern const Sint32		  TridentSpins[];
extern const MODEL_VERTEX TridentProngCorners[];
extern const Uint8		  TridentGlintFlips[];
/* The leaping strike's waves and rings. */
extern const Uint8 LeapingStrikeWaveStarts[];
extern const Uint8 LeapingStrikeWaveGrowth[];
extern const Sint8 LeapingStrikeWaveHeights[];
extern const Uint8 LeapingStrikeRingStarts[];
extern const Uint8 LeapingStrikeRingGrowth[];
extern const Sint8 LeapingStrikeRingHeights[];
/* Meteor Blow: its pictures, waves and rings. */
extern const Uint8	MeteorBlowPictureWidths[];
extern const Uint8	MeteorBlowPictureHeights[];
extern const Uint16 MeteorBlowPictures[];
extern const Uint8	MeteorBlowWaveStarts[];
extern const Uint8	MeteorBlowWaveGrowth[];
extern const Uint8	MeteorBlowWaveStretch[];
extern const Uint8	MeteorBlowRingStarts[];
extern const Uint8	MeteorBlowRingGrowth[];
extern const Sint8	MeteorBlowRingHeights[];
/* Liquifier's splash: its corners, start ticks and heights. */
extern const MODEL_VERTEX LiquifierSplashCorners[];
extern const Uint8		  LiquifierSplashStarts[];
extern const Uint8		  LiquifierSplashHeights[];
/* The canvas burst's flame picture for each three ticks of its life. */
extern const Uint8 CanvasBurstFlamePictures[];
/* Megiddo: its flashes' turns and starts, and its rays' turns and places. */
extern const Sint16 MegiddoFlashPitches[];
extern const Sint16 MegiddoFlashYaws[];
extern const Uint16 MegiddoFlashStarts[];
extern const Sint16 MegiddoRayPitches[];
extern const Sint16 MegiddoRayYaws[];
extern const Sint16 MegiddoRayRolls[];
extern const Sint8	MegiddoRayX[];
extern const Sint8	MegiddoRayY[];
/* Odyssey's flashes: their pitch and yaw. */
extern const Sint16 OdysseyFlashPitches[];
extern const Sint16 OdysseyFlashYaws[];
/* Moloch's rings: when each starts, how fast it grows, and its height. */
extern const Uint8 MolochRingStarts[4];
extern const Uint8 MolochRingGrowth[];
extern const Sint8 MolochRingHeights[];
/* Iris: the splashes, the pillar and particle models, the sound ticks, the
   streaks, the drops, the strips and the cone. */
extern Sint32			  IrisSplashRadii[];
extern const Uint8		  IrisSplashOffsetsY[];
extern const Uint16		  IrisPillarMesh[];
extern const MODEL_VERTEX IrisParticleCorners[];
extern const Uint16		  IrisParticleMesh[];
extern const Uint8		  IrisPsynergySoundTicks[];
extern const Uint16		  IrisDoorSoundTicks[];
extern const Uint16		  IrisPowerUpSoundTicks[];
extern const Sint8		  IrisStreakX[];
extern const Uint8		  IrisStreakY[];
extern const Uint16		  IrisStreakAngles[];
extern const Sint16		  IrisStreakSpeeds[];
extern const Uint8		  IrisLineShades[];
extern Sint32			  IrisMoteSizes[];
extern const Uint16		  IrisSplashStarts[];
extern const Uint16		  IrisSplashEnds[];
extern const Uint8		  IrisSplashX[];
extern const Uint8		  IrisSplashY[];
extern const Uint8		  IrisSplashSmall[];
extern const Uint8		  IrisDropSpeeds[];
extern const Uint8		  IrisDropTicks[];
extern const Uint8		  IrisDropPictureSteps[];
extern const MODEL_VERTEX IrisLeftStripCorners[];
extern const MODEL_VERTEX IrisRightStripCorners[];
extern const Uint16		  IrisStripMesh[];
extern const MODEL_VERTEX IrisConeCorners[];
extern const Uint16		  IrisConeMesh[];

/* The buffers and tables the parts share, by the first part that reads them. */

/* falling.c The map cell buffer at its own address, which the effects borrow:
   no map is loaded during a battle. */
extern Uint8 MapGridMetatileBuffer[];

/* thor.c */
extern const Sint16 ThorPoints[6][2]; /* the three lines' end points */

/* flurry.c */
void btEffectRunFlurry(BATTLE_EFFECT_PARM *args, Sint32 mode);

/* btEffectRunFlurry's modes, by the abilities that play them: the rows of
   FlurryStyles. Gust and Flying Attack take one, two or three strikes as a
   double or triple blow raises their variant. */
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
	/* blue arc A, with a wave for the third and fourth of every four strikes */
	FLURRY_ARCS_AND_WAVES,
	FLURRY_ARCS /* blue arc B for every strike */
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
	FLURRY_EXTRA_GLINTS, /* three glints a tick until tick 31 */
	FLURRY_EXTRA_RIPPLE	 /* BG2 swings on a sine wave and smoke rises */
} FLURRY_EXTRA_KIND;

/* The scale each of Gaia's rocks starts from before it grows. */
extern const SCALE GaiaUnitScale;

/* spider_web.c A scale of one in 16.16 on both axes. */
extern const SPR_SCALE UlyssesParticleScale;

/* neptune.c */
extern const SCALE NeptuneUnitScale; /* the serpent's scale */

extern const Sint8 VoidBeamCorners[][2]; /* the hexagon's corners (z, y) */

/* flora.c Flora's scale, one to one. */
extern const SCALE FloraUnitScale;

/* daedalus.c The scales of the forest canopy Daedalus flies over, of his
   machine's pieces and of his missiles. */
extern const SCALE DaedalusCanopyScale;
extern const SCALE DaedalusMachineScale;
extern const SCALE DaedalusMissileScale;

/* Coatlicue's and Catastrophe's figures' scales. */
extern const SCALE CoatlicueUnitScale;
extern const SCALE CatastropheUnitScale;

/* haures.c Haures's scales as he charges, as he circles and as he opens. */
extern const SCALE HauresChargeScale;
extern const SCALE HauresOrbitScale;
extern const SCALE HauresOpeningScale;

/* moloch.c Moloch's motes' sizes, by a mote's number's low three bits. */
extern const Uint8 MolochMoteSizes[];
/* Iris's scale while she rises. */
extern const SCALE IrisUnitScale;
/* The template btEffectBuildBlitter walks word by word to write a blitter
   (patch_template.s); none of it runs where it is. */
extern const Uint32 BlitterTemplate[];

/* The pixel fragments btEffectBuildBlitter copies into its blitters, from
   blitter_fragments.s: Forward reads the picture left to right and Reverse
   right to left. None of them runs where it is. Sets 0x40 in every byte of sl,
   which caps an additive word's pixels at the brightest canvas level. */
extern const Uint32 BattleEffect_Mask[];
/* The ARM immediate operand for 1 << n, n from 0 to 15 (blitter_operands.c). */
extern const Uint16 BlitterImmediatePowers[];
/* Copy, one pixel, right to left. */
extern const Uint32 BattleEffect_CopyReverse[];
/* Copy, one pixel, left to right. */
extern const Uint32 BattleEffect_CopyForward[];
/* Copy, a full cell's eight pixels, right to left. */
extern const Uint32 BattleEffect_Copy8Reverse[];
/* Copy, a full cell's eight pixels, left to right. */
extern const Uint32 BattleEffect_Copy8Forward[];
/* Transparent, one pixel, right to left. */
extern const Uint32 BattleEffect_TransparentReverse[];
/* Transparent, one pixel, left to right. */
extern const Uint32 BattleEffect_TransparentForward[];
/* Transparent, a full cell's eight pixels, right to left. */
extern const Uint32 BattleEffect_Transparent8Reverse[];
/* Transparent, a full cell's eight pixels, left to right. */
extern const Uint32 BattleEffect_Transparent8Forward[];
/* Max, one pixel, right to left. */
extern const Uint32 BattleEffect_MaxReverse[];
/* Max, one pixel, left to right. */
extern const Uint32 BattleEffect_MaxForward[];
/* Max, four pixels (one canvas word), right to left; a full cell's row takes it
   twice. */
extern const Uint32 BattleEffect_Max4Reverse[];
/* Max, four pixels (one canvas word), left to right; a full cell's row takes it
   twice. */
extern const Uint32 BattleEffect_Max4Forward[];
/* Additive, one pixel, right to left. */
extern const Uint32 BattleEffect_AdditiveReverse[];
/* Additive, one pixel, left to right. */
extern const Uint32 BattleEffect_AdditiveForward[];
/* Additive, four pixels added a word at a time, right to left; a full cell's
   row takes it twice. */
extern const Uint32 BattleEffect_ColorReverse[];
/* Additive, four pixels added a word at a time, left to right; a full cell's
   row takes it twice. */
extern const Uint32 BattleEffect_ColorForward[];

/* The model renderer's ARM functions, which run from IWRAM, each with its size
   in bytes and, for three of them, in DMA words. An ARM build of
   btRenderFillMesh, which btRenderRunFillMeshKernel runs. */
extern const Uint8 btRenderFillMeshKernel[];
/* btRenderFillMeshKernel's bytes */
extern Uint8 btRenderFillMeshKernelCodeSize[];
/* The vertex transform btRenderTransformVertices runs. */
extern const Uint8 btRenderTransformKernel[];
/* btRenderTransformKernel's bytes */
extern Uint8 btRenderTransformKernelCodeSize[];

/* The rasterizer kernels btRenderDrawModelList copies into IWRAM, one for each
   MODEL_KIND, with their sizes in bytes and, for three of them, in DMA
   words. */
extern const Uint8 btRenderSolidKernel[];		  /* MODEL_SOLID */
extern const Uint8 btRenderSolidMaxKernel[];	  /* MODEL_SOLID_MAX */
extern const Uint8 btRenderSolidAdditiveKernel[]; /* MODEL_SOLID_ADDITIVE */
extern const Uint8 btRenderTexturedKernel[];	  /* MODEL_TEXTURED */
extern const Uint8 btRenderTexturedMaxKernel[];	  /* MODEL_TEXTURED_MAX */
/* MODEL_TEXTURED_ADDITIVE */
extern const Uint8 btRenderTexturedAdditiveKernel[];
extern const Uint8 btRenderShadedMaxKernel[];	   /* MODEL_SHADED_MAX */
extern const Uint8 btRenderShadedAdditiveKernel[]; /* MODEL_SHADED_ADDITIVE */
extern const Uint8 btRenderShadedKernel[];		   /* MODEL_SHADED */
extern const Uint8 btRenderTexturedNarrowKernel[]; /* MODEL_TEXTURED_NARROW */
extern const Uint8 btRenderSolidNarrowKernel[];	   /* MODEL_SOLID_NARROW */
extern Uint8 btRenderSolidKernelCodeSize[]; /* btRenderSolidKernel's bytes */
/* btRenderSolidMaxKernel's bytes */
extern Uint8 btRenderSolidMaxKernelCodeSize[];
/* btRenderSolidAdditiveKernel's bytes */
extern Uint8 btRenderSolidAdditiveKernelCodeSize[];
/* btRenderTexturedKernel's bytes */
extern Uint8 btRenderTexturedKernelCodeSize[];
/* btRenderTexturedMaxKernel's bytes */
extern Uint8 btRenderTexturedMaxKernelCodeSize[];
/* btRenderTexturedAdditiveKernel's bytes */
extern Uint8 btRenderTexturedAdditiveKernelCodeSize[];
/* btRenderShadedMaxKernel's bytes */
extern Uint8 btRenderShadedMaxKernelCodeSize[];
/* btRenderShadedAdditiveKernel's bytes */
extern Uint8 btRenderShadedAdditiveKernelCodeSize[];
extern Uint8 btRenderShadedKernelCodeSize[]; /* btRenderShadedKernel's bytes */
/* btRenderTexturedNarrowKernel's bytes */
extern Uint8 btRenderTexturedNarrowKernelCodeSize[];
/* btRenderSolidNarrowKernel's bytes */
extern Uint8 btRenderSolidNarrowKernelCodeSize[];
/* btRenderTexturedKernel's words */
extern Uint8 btRenderTexturedKernelCodeWords[];
extern Uint8 btRenderShadedKernelCodeWords[]; /* btRenderShadedKernel's words */
/* btRenderTexturedNarrowKernel's words */
extern Uint8 btRenderTexturedNarrowKernelCodeWords[];

/* The IWRAM bytes left to allocate, read as unsigned here: the comparison in
   btRenderCopyTextureToIWRAM is unsigned in the ROM, though the function
   returns a signed number. */
Uint32 syHeapGetRemainingIWRAM(void);

#endif /* ALCHEMY_BATTLE_EFFECT_INTERNAL_H */
