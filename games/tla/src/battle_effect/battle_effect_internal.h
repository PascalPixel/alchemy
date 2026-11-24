#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle_effect object shares: its includes, the
/// canvas's and the shared sheets' sizes, the modes of the bodies several
/// effects play, and the routines and tables the parts reach across each other.
/// battle_effect.c includes it before the parts, and each part includes it too.

#include "types.h"
#include "sprite_ids.h"
#include "resource_ids.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/bg_scroll.h"
#include "system/dma.h"
#include "battle/battle_effect.h"
#include "battle_effect/battle_effect.h"
#include "battle_effect/battle_effect_table.h"
#include "battle/battle_effect_state.h"
#include "field/particle.h"
#include "battle/battle_camera.h"
#include "system/task.h"
#include "battle/motobj.h"
#include "system/iwram_call.h"
#include "fixed_math.h"
#include "graphics/graphics.h"
#include "system/matrix.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "battle/battle_state.h"
#include "system/irq_lock.h"
#include "battle/render_model.h"
#include "battle/battle.h"
#include "graphics/vram_tab.h"
#include "window/item_icon.h"
#include "system/math_table.h"
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
#include "system/irq.h"
#include "system/scene.h"
#include "game/class_djinn.h"
#include "version.h"
#include "field/field_sprite.h"
#include "system/vector3.h"
#include "system/ram_code.h"

struct EffectSprite;
void *EffectSprite_Acquire(s32 width, s32 height, u32 shape, s32 mode);
void *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes,
                                   s32 flags);

/// A point of a streak: x, y and z.
struct StreakPoint {
  s32 x;
  s32 y;
  s32 z;
};

/// One of Cruel Ruin's 256 streaks: its head and tail and how it is turned.
struct Streak {
  s32 head;
  s32 tail;
  s32 unusedA;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 unusedB;
};

/// The element streaks' state, which BattleEffect_StartElementStreaks allocates
/// in the HEAP_SLOT_BATTLE_EFFECT heap slot in place of the battle effect
/// state.
struct ElementStreaksState {
  s32 frame;        ///< the streaks' frame count (BattleEffect_DrawStreaks)
  s32 halvePending; ///< the streaks are drawn: halve the canvas into VRAM
  /// Frames of the background's fade left (BattleEffect_DimBackgroundStep).
  s32 fadeFrames;
  s32 fadeStep; ///< the fade's steps so far, BACKGROUND_LEVEL_STEP each
};

/// An EffectSprite as Cruel Ruin reads its OAM attribute 2: its tile, priority
/// and palette, as fields or as one halfword.
struct RuinObject {
  u8 unusedBeforeAttr[8]; ///< the sprite's next and OAM attributes 0 and 1
  union {
    struct {
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } bits;
    u16 all;
    u8 bytes[2];
  } attr;
};

/// The map cell buffer from its second 16 KB on, which some effects borrow as
/// scratch: no map is loaded during a battle.
extern u8 gEffectScratch[];

/// The map cell buffer holds WIN0's line table and then the droplets Douse's
/// drops throw up.
struct DropletBuffer {
  u16 lines[160];
  struct Particle droplets[512];
};

/// Eclipse's scale for the dragon's sprites, and Cruel Ruin's for its
/// summon, one to one.
extern const struct Scale gEclipse_DragonScale;
extern const struct Scale gCruelRuin_UnitScale;

/// One handler for each battle effect, by effect less one; 0 where none.
extern void (*const gBattleEffect_Handlers[])(struct BattleEffectParams *args);

/// Zagan's scale, one to one.
extern const struct Scale gZagan_UnitScale;

extern const struct Scale gProcne_UnitScale; ///< the comets' unit scale

/// The battle screen's display mode while an effect plays: background mode 1
/// with BG0 to BG2, sprites and both windows on.
#define EFFECT_DISPCNT                                                         \
  (DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |     \
   DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

/// The whole pixels of a stored 16.16 value, read as its upper halfword.
// UB: the s32 is read through an s16.
#ifdef UBFIX
#define FIXED_INTEGER_PART(v) FIXEDtoINT((s32)(v))
#else
#define FIXED_INTEGER_PART(v) (((s16 *)&(v))[1])
#endif

/// The canvas added to the battle under it: both blend weights whole.
#define EFFECT_BLEND_ADD BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX)
/// The canvas at `level` sixteenths over the whole battle under it.
#define EFFECT_BLEND_CANVAS(level) BLEND_ALPHA(level, BLEND_WEIGHT_MAX)
/// The canvas's usual weight over the battle, in sixteenths: every canvas
/// opens with it.
#define CANVAS_BLEND_LEVEL 14
/// The element pillar's and element burst's canvas weight over the battle.
#define ELEMENT_BLEND_LEVEL 12
/// The motes the element pillar and element burst set up in PARTICLES; the
/// burst leaves them undrawn.
#define ELEMENT_SPIRAL_MOTES 128

/// The map cell buffer as BattleEffect_BuildWindowEdgeTable reads it: window
/// 0's right edge, how far each of its lines is set in from that edge, and the
/// right edge it builds for every screen line.
#define WINDOW_EDGE_RIGHT ((u16 *)gMapGridCellBuffer)
#define WINDOW_EDGE_INSETS (Ram_MapCellBuffer + 2)
#define WINDOW_EDGE_INSET_LINES 128
#define WINDOW_EDGE_LINES (WINDOW_EDGE_INSETS + WINDOW_EDGE_INSET_LINES)

/// The particles an effect keeps in the map cell buffer, from its start.
#define PARTICLES ((struct Particle *)Ram_MapCellBuffer)
/// The particles an effect keeps in the map cell buffer from its second 16 KB
/// on, past PARTICLES or the pictures and meshes it keeps in the first: a
/// second, larger table, always reached at its fixed address, where the
/// effects that throw more sparks, motes or trails than the state's 64
/// particles keep them.
#define EXTRA_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 16384))

/// The VRAM address of a sprite's tiles, from its VRAM cache entry.
#define OBJ_TILES(sprite)                                                      \
  (OBJ_VRAM0 + gVramCacheEntries[((struct Sprite *)(sprite))->slot].offset)

/// Index of DMAxCNT_H, the control half, in a channel's 16-bit registers.
#define DMA_CONTROL_HALF 5

/// The particles the map cell buffer holds when an effect lays its particles
/// over it from the start.
#define PARTICLE_POOL_COUNT 1024
/// Nereid's drops burst up, in BattleEffect_RunNereid and in the unused copy of
/// it: every free one of the first NEREID_BURST_DROPS as the columns gather,
/// then NEREID_DROPS_A_FRAME a frame while they rise.
#define NEREID_BURST_DROPS 256
#define NEREID_DROPS_A_FRAME 16
/// The sprites that draw Megaera, in her summon and in her formation.
#define MEGAERA_OBJECTS 11

/// The canvas: 128x128 pixels, one byte each, in its heap slot and in VRAM at
/// BG_CHAR_BLOCK(1) (CANVAS_BYTES).
#define CANVAS_SIZE 128
/// The canvas's rows and columns of 8x8 tiles.
#define CANVAS_TILES (CANVAS_SIZE / 8)
/// The byte of canvas pixel (x, y): the canvas is laid out in 8x8 tiles, row
/// by row, each tile 64 bytes row by row.
#define CANVAS_PIXEL_OFFSET(x, y)                                              \
  ((((y) / 8 * CANVAS_TILES + (x) / 8) * 8 + ((y) & 7)) * 8 + ((x) & 7))
/// The bytes of one 8x8 tile of the canvas; a row of pixels steps from one tile
/// to the next by CANVAS_TILE_BYTES - 8.
#define CANVAS_TILE_BYTES (8 * 8)
/// The bytes of a 32x32 sprite's tiles in 256 colours, and in 16.
#define OBJ_32X32_8BPP_BYTES (32 * 32)
#define OBJ_32X32_4BPP_BYTES (32 * 32 / 2)
/// The bytes of a 64x64 sprite's tiles in 256 colours, and in 16.
#define OBJ_64X64_8BPP_BYTES (64 * 64)
#define OBJ_64X64_4BPP_BYTES (64 * 64 / 2)
/// The bytes of a 16x16 sprite's tiles in 16 colours.
#define OBJ_16X16_4BPP_BYTES (16 * 16 / 2)
/// The byte of the first pixel of canvas row y, y not negative.
#define CANVAS_ROW_OFFSET(y) ((((y) & 7) << 3) + (((y) >> 3) << 10))
/// The brightest canvas level: the additive blitters and
/// CANVAS_COPY_AND_BRIGHTEN stop at it.
#define CANVAS_LEVEL_BRIGHTEST 63

/// The battle view, under the top window and over the menus: screen lines 16
/// to 135.
#define BATTLE_VIEW_TOP 16
#define BATTLE_VIEW_HEIGHT 120
#define BATTLE_VIEW_BOTTOM (BATTLE_VIEW_TOP + BATTLE_VIEW_HEIGHT)
/// The backdrop gradients Azul and Coatlicue write over the battle view, a
/// colour a line from BATTLE_VIEW_TOP - 1: each channel a level darker every
/// so many lines down, blue every 15, green every 16 and red every 17.
#define GRADIENT_BLUE_LINES 15
#define GRADIENT_GREEN_LINES 16
#define GRADIENT_RED_LINES 17

/// Writes one row of BG2's map for the canvas, two tiles at a time.
#define EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset)                       \
  for ((column) = 0; (column) != 8; (column)++, (offset) += 2) {               \
    s32 tileIndex;                                                             \
    s16 pair;                                                                  \
                                                                               \
    tileIndex = (row) * 16 + (column) * 2;                                     \
    pair = ((tileIndex + 1) << 8) | tileIndex;                                 \
    *(volatile s16 *)((u8 *)BG_CANVAS_MAP + (offset)) = pair;                  \
  }

/// Three summons open alike (Eclipse, Daedalus and Flora): the battle flashes
/// away over SUMMON_OPENING_FRAMES, and at SUMMON_OPENING_LOAD_FRAME
/// their sky and their sprites load.
#define SUMMON_OPENING_FRAMES 52
#define SUMMON_OPENING_LOAD_FRAME 27

/// Windows 0 and 1 both cover the battle view: the full width, lines 16 to
/// 135.
#define EFFECT_SET_BATTLE_WINDOWS()                                            \
  (REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH),                                    \
   REG_WIN0V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM),                 \
   REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH),                                    \
   REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM))
/// The canvas's first BATTLE_VIEW_HEIGHT lines, which the battle view shows:
/// what most of this game's effects clear and fill, leaving the last 8.
#define CANVAS_VIEW_BYTES (CANVAS_SIZE * BATTLE_VIEW_HEIGHT)

/// How far right the narrow canvas is set to show over the party: against the
/// screen's right edge.
#define NARROW_CANVAS_PARTY_X (DISPLAY_WIDTH - CANVAS_SIZE)

/// Where the summons centre the projection while their figures fly, lower
/// than the battle's BATTLE_PROJECTION_CENTER_Y.
#define SUMMON_PROJECTION_CENTER_Y 240

/// The frames the battle background darkens behind an effect's canvas
/// (state->fadeFrames, BattleEffect_DarkenBackgroundStep).
#define BACKGROUND_DARKEN_FRAMES 24
/// The level BattleBackground_LoadFar darkens the battle's own background to
/// as it comes back after an effect: as dark as the canvas's fade left it.
#define BACKGROUND_DARKENED_LEVEL BACKGROUND_DARKEN_FRAMES

/// A sprite sheet resource opens with its 64 colours, ahead of its compressed
/// pictures.
#define SHEET_PALETTE_COLORS 64
#define SHEET_PALETTE_BYTES (SHEET_PALETTE_COLORS * 2)

/// Loads a sprite sheet's colours into the background palette.
#define LOAD_BG_PALETTE(resource)                                              \
  {                                                                            \
    void *palette = Resource_GetTableEntry(resource);                          \
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;             \
                                                                               \
    copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);                       \
  }

/// The damping that keeps a particle's whole speed.
#define PARTICLE_DAMPING_NONE DAMPING_SCALE

/// The bytes of a sprite sheet picture, one byte a pixel, from its name's
/// _WIDTH and _HEIGHT.
#define PICTURE_BYTES(name) (name##_WIDTH * name##_HEIGHT)

/// Draws picture `name` of `sheet` at its own size, its top left at (x, y).
#define EFFECT_DRAW_PICTURE(draw, canvas, sheet, name, x, y)                   \
  (draw)(canvas, (sheet) + (name), x, y, name##_WIDTH, name##_HEIGHT)

/// A bite's half pictures (BlueDropletSheet's closing fangs, each drawn beside
/// its mirror): the canvas row they start at, and their size.
#define TARGET_STRIKE_BITE_Y 56
#define TARGET_STRIKE_BITE_WIDTH 18
#define TARGET_STRIKE_BITE_HEIGHT 48

/// The battle background's image as BG1 shows it from BG character block 2,
/// one byte a pixel.
#define BATTLE_BACKGROUND_WIDTH 256
#define BATTLE_BACKGROUND_HEIGHT 120
#define BATTLE_BACKGROUND_BYTES                                                \
  (BATTLE_BACKGROUND_WIDTH * BATTLE_BACKGROUND_HEIGHT)
/// A background's scroll (struct BgScroll) is a halfword: a negative scroll is
/// held BG_SCROLL_WRAP above itself, and read back by taking it away.
#define BG_SCROLL_WRAP (1 << 16)
/// Two entries of a background map in one word: `left`, then the one to its
/// right.
#define MAP_ENTRY_PAIR(left, right) ((u32)(right) << 16 | (left))
/// The tile BG1 shows around a picture of its own, the last a map entry can
/// name.
#define BG1_CLEAR_TILE 1023
/// BG character block 2's first tile, counted from BG1's character base.
#define BG1_BLOCK2_FIRST_TILE (2 * BG_CHAR_SIZE / TILE_SIZE_8BPP)
/// The bytes of one 16-colour palette.
#define PALETTE_BANK_BYTES 32
/// A word of two palette entries, both `color`, to fill a palette a word at a
/// time.
#define COLOR_PAIR(color) ((color) << 16 | (color))

/// A model's shade (RenderModel.shade) is added to each of its pixels' canvas
/// levels: at this or below, nothing of the model is left to draw, and the
/// effects skip it.
#define MODEL_SHADE_INVISIBLE (-64)

/// RenderModel.kind: the rasterizer kernel Render_DrawModelList copies into
/// IWRAM to draw a model, and how it puts each pixel on the canvas. A solid
/// model fills its faces with their colours; a textured one maps its texture;
/// a shaded one adds the model's shade to each texel and skips those it leaves
/// at 0 or below. The narrow kernels draw into a target half the canvas's
/// width.
enum ModelKind {
  MODEL_END,            ///< ends the list
  MODEL_SOLID,          ///< Render_SolidKernel: the colour replaces the canvas
  MODEL_SOLID_MAX,      ///< the colour where it is brighter
  MODEL_SOLID_ADDITIVE, ///< the colour added, up to the brightest level
  MODEL_TEXTURED_MAX,   ///< the texel where it is brighter
  MODEL_TEXTURED_ADDITIVE, ///< the texel added, up to the brightest level
  MODEL_SHADED_MAX,        ///< the shaded texel where it is brighter
  MODEL_SHADED_ADDITIVE,   ///< the shaded texel added, up to the brightest
  MODEL_TEXTURED,          ///< the texel replaces the canvas
  MODEL_SHADED,            ///< the shaded texel replaces the canvas
  MODEL_TEXTURED_NARROW,   ///< MODEL_TEXTURED into a narrow target
  MODEL_SOLID_NARROW       ///< MODEL_SOLID into a narrow target
};

/// RenderModel.flags that the rasterizer kernels read: a one-sided model draws
/// only the faces wound one way round on the screen, clockwise with
/// MODEL_CLOCKWISE and anticlockwise without it, and skips the others.
#define MODEL_ONE_SIDED 2
#define MODEL_CLOCKWISE 1

/// Starts a frame's 3D projection from the battle camera.
#define EFFECT_VIEW_FROM_CAMERA(camera)                                        \
  (Matrix_Initialize(), Matrix_SetLookAt((camera)->eye, (camera)->focus))

// The sprite sheets' pictures, as The Broken Seal names them: this game draws
// the same sprite sheets at the same sizes.

/// The bytes of the six glint pictures that open the tornado sprite sheet.
#define GLINT_SET_BYTES                                                        \
  (19 * 31 + 23 * 31 + 23 * 31 + 19 * 21 + 16 * 21 + 17 * 24)

/// Draws flare picture `size`, a square, centred on (x, y).
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size)                     \
  (draw)(canvas, (sheet) + gBattleEffect_FlarePictures[(size) - 1],            \
         (x) - (size), (y) - (size), (size) * 2, (size) * 2)

/// The reaper in CounterRevealSheet, which Condemn and Mortal Danger decode:
/// the reaper with its scythe raised, then swung, then the red crescent the
/// swing leaves.
#define REAPER_SHEET_RAISED_WIDTH 57
#define REAPER_SHEET_RAISED_HEIGHT 98
#define REAPER_SHEET_SWUNG_WIDTH 99
#define REAPER_SHEET_SWUNG_HEIGHT 69
#define REAPER_SHEET_CRESCENT_WIDTH 128
#define REAPER_SHEET_CRESCENT_HEIGHT 91
/// Where each picture starts in ReaperSheet, in bytes.
enum ReaperSheetPicture {
  REAPER_SHEET_RAISED = 0,
  REAPER_SHEET_SWUNG = REAPER_SHEET_RAISED + PICTURE_BYTES(REAPER_SHEET_RAISED),
  REAPER_SHEET_CRESCENT = REAPER_SHEET_SWUNG + PICTURE_BYTES(REAPER_SHEET_SWUNG)
};

/// RedCrescentSheetA: the swing's crescent fading in four pictures.
#define RED_CRESCENT_FADE_A_WIDTH 128
#define RED_CRESCENT_FADE_A_HEIGHT 91
#define RED_CRESCENT_FADE_B_WIDTH 128
#define RED_CRESCENT_FADE_B_HEIGHT 59
#define RED_CRESCENT_FADE_C_WIDTH 122
#define RED_CRESCENT_FADE_C_HEIGHT 29
#define RED_CRESCENT_FADE_D_WIDTH 76
#define RED_CRESCENT_FADE_D_HEIGHT 25
/// Where each picture starts in RedCrescentSheetA, in bytes.
enum RedCrescentPicture {
  RED_CRESCENT_FADE_A = 0,
  RED_CRESCENT_FADE_B = RED_CRESCENT_FADE_A +
      PICTURE_BYTES(RED_CRESCENT_FADE_A),
  RED_CRESCENT_FADE_C = RED_CRESCENT_FADE_B +
      PICTURE_BYTES(RED_CRESCENT_FADE_B),
  RED_CRESCENT_FADE_D = RED_CRESCENT_FADE_C + PICTURE_BYTES(RED_CRESCENT_FADE_C)
};

/// The slash pictures as wide and tall as the canvas's view: StarBurstSheet's,
/// CrescentSheet's and the reveal masks.
#define SLASH_PICTURE_SIZE 120

/// Closing bolts and lightning columns use the same 24x120 pictures.
#define LIGHTNING_BOLT_WIDTH 24
#define LIGHTNING_BOLT_HEIGHT 120
#define LIGHTNING_BOLT_PICTURE_BYTES PICTURE_BYTES(LIGHTNING_BOLT)

/// BlueFlameSheet: the column of flame, then the flame that splashes from it,
/// twice its width.
#define BLUE_FLAME_COLUMN_WIDTH 17
#define BLUE_FLAME_COLUMN_HEIGHT 104
#define BLUE_FLAME_SPLASH_WIDTH 34
#define BLUE_FLAME_SPLASH_HEIGHT 65
/// WaveSheet's six pictures, BlueArcSheetA and B's six, and YellowOrbSheet's.
#define WAVE_WIDTH 48
#define WAVE_HEIGHT 16
#define BLUE_ARC_WIDTH 48
#define BLUE_ARC_HEIGHT 72
#define YELLOW_ORB_WIDTH 40
#define YELLOW_ORB_HEIGHT 48
/// The yellow orb as BattleEffect_PackYellowOrbs packs it, every second byte.
#define YELLOW_ORB_PACKED_WIDTH (YELLOW_ORB_WIDTH / 2)
#define YELLOW_ORB_PACKED_HEIGHT YELLOW_ORB_HEIGHT
#define YELLOW_ORB_PICTURES 6

/// FireStreakSheet's six pictures and FireBurstSheet's.
#define FIRE_STREAK_WIDTH 48
#define FIRE_STREAK_HEIGHT 72
#define FIRE_BURST_WIDTH 40
#define FIRE_BURST_HEIGHT 80
/// The small Djinn sprite sheets' pictures are SMALL_DJINNI_WIDTH wide;
/// Mercury's and Venus's are shorter than SMALL_DJINNI_HEIGHT.
#define MERCURY_SMALL_DJINNI_HEIGHT 34
#define VENUS_SMALL_DJINNI_HEIGHT 38
/// FireBlobSheet's blobs and CrystalSheet's crystal.
#define FIRE_BLOB_WIDTH 48
#define FIRE_BLOB_HEIGHT 32
#define CRYSTAL_WIDTH 32
#define CRYSTAL_HEIGHT 64
/// CrystalSheet's shard pictures, after its crystal, and the shards of
/// gPrism_ShardStarts a crystal breaks into.
#define CRYSTAL_SHARD_PICTURES PICTURE_BYTES(CRYSTAL)
#define SHARDS_PER_CRYSTAL 16
/// BoulderSheet's boulder, its first picture, and its shard pictures after it.
#define BOULDER_WIDTH 34
#define BOULDER_HEIGHT 62
#define BOULDER_SHARD_PICTURES (BOULDER_WIDTH * BOULDER_HEIGHT)
/// A shard turns through its three pictures, one each SHARD_TURN of its
/// variant.
#define SHARD_TURN 96
/// PaletteRampImage's shard pictures (drawn through gStatChange_ShardPictures),
/// where the stat reduction and the Sword Card decode them in the sprite sheet,
/// followed by DIMMED_COPIES - 1 copies of them, each darker than the one
/// before.
#define STAT_REDUCTION_SHARD_PICTURES 9600
#define STAT_REDUCTION_SHARD_BYTES 696 ///< bytes in each copy

/// LightningPillarSheet's pillar, in two pictures.
#define LIGHTNING_PILLAR_WIDTH 48
#define LIGHTNING_PILLAR_HEIGHT 112
/// FirePillarSheetA: the pillar of fire, then two flames.
#define FIRE_PILLAR_WIDTH 34
#define FIRE_PILLAR_HEIGHT 104
#define FIRE_PILLAR_FLAME_WIDTH 20
#define FIRE_PILLAR_FLAME_HEIGHT 37
/// SpiralSheet's pictures.
#define SPIRAL_WIDTH 32
#define SPIRAL_HEIGHT 32
/// A card's picture, as the Sleep, Baffle, Death and Sword Cards and the
/// Bramble, Frost, Flame and Thunder Cards draw it.
#define CARD_WIDTH 32
#define CARD_HEIGHT 64
/// SheepCardSheet's sheep, four pictures of it tumbling after its card.
#define SHEEP_WIDTH 32
#define SHEEP_HEIGHT 32

/// FlameballSheet and FireballSheet's pictures.
#define FLAMEBALL_WIDTH 40
#define FLAMEBALL_HEIGHT 64

/// Each half of RuneSheet's rune, the second drawn mirrored.
#define RUNE_HALF_WIDTH 24
#define RUNE_HALF_HEIGHT 48
/// The 64x64 texture Flash Force and Void Beam build: the rune's two halves
/// side by side, the second mirrored, eight pixels in from the top left.
#define RUNE_TEXTURE_SIZE 64
#define RUNE_TEXTURE_RUNE (8 * RUNE_TEXTURE_SIZE + 8)

/// A Djinni's picture: each frame of the full Djinn sprite sheets
/// (VenusDjinnSheet and its kind), and the one picture of the small sprite
/// sheets.
#define DJINNI_WIDTH 40
#define DJINNI_HEIGHT 40
#define SMALL_DJINNI_WIDTH 20
#define SMALL_DJINNI_HEIGHT 40

/// The pictures of the sprite sheets several effects share, each sprite sheet's
/// pictures all one size: SmokeSheet's puffs, FlashBurstSheet's bursts,
/// SkullSheet's skulls and the tornado strips after TornadoSheet's glints.
#define SMOKE_PUFF_WIDTH 24
#define SMOKE_PUFF_HEIGHT 48
/// A puff of SmokeSheet's lives through its six pictures, four frames each.
#define SMOKE_PUFF_FRAMES (6 * 4)
#define FLASH_BURST_WIDTH 32
#define FLASH_BURST_HEIGHT 64
/// FlashBurstSheet's six pictures: a white flash turning to a fireball that
/// burns out to embers.
#define FLASH_BURST_PICTURE_COUNT 6
/// BlueBeamSheet: a blue bubble, a small splash of blue light the bubble's
/// size, then a large splash.
#define BLUE_BUBBLE_WIDTH 24
#define BLUE_BUBBLE_HEIGHT 24
#define BLUE_LARGE_SPLASH_WIDTH 42
#define BLUE_LARGE_SPLASH_HEIGHT 42
/// BlueCloudPuffSheet: four 32x32 puffs of cloud.
#define CLOUD_PUFF_WIDTH 32
#define CLOUD_PUFF_HEIGHT 32
#define SKULL_WIDTH 24
#define SKULL_HEIGHT 24
#define TORNADO_STRIP_WIDTH 40
#define TORNADO_STRIP_HEIGHT 32
/// The pictures of DaggerSheet, ShieldSheet, GlowOrbSheet, VioletStreakSheet
/// and WispSheet.
#define DAGGER_WIDTH 24
#define DAGGER_HEIGHT 40
#define SHIELD_WIDTH 40
#define SHIELD_HEIGHT 40
#define GLOW_ORB_WIDTH 40
#define GLOW_ORB_HEIGHT 40
#define VIOLET_STREAK_WIDTH 32
#define VIOLET_STREAK_HEIGHT 64
#define WISP_WIDTH 64
#define WISP_HEIGHT 32
/// VortexSheet's six pictures and HeartSheet's heart.
#define VORTEX_WIDTH 24
#define VORTEX_HEIGHT 48
#define HEART_WIDTH 22
#define HEART_HEIGHT 42
/// WaterSpraySheet: the wave, then the splash's pictures.
#define WATER_WAVE_WIDTH 32
#define WATER_WAVE_HEIGHT 32
#define WATER_SPLASH_WIDTH 48
#define WATER_SPLASH_HEIGHT 48

// The canvas life cycle, shared by every canvas effect.

/// The bytes an effect's state is lent: more than struct
/// BattleEffectState holds.
#define BATTLE_EFFECT_STATE_SIZE 31612

/// The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770
/// A picture's dimmed copies: DIMMED_COPIES of it, copy i with every level
/// capped at DIMMED_COPY_CAP(i), 7 levels lower than the copy before, copy 0
/// not at all. Thor's ring darkens through copies of the mote pictures, a stat
/// reduction's shards through copies of the shard.
#define DIMMED_COPIES 8
#define DIMMED_COPY_CAP(copy) (CANVAS_LEVEL_BRIGHTEST + 1 - (copy) * 7)
/// Caps a colour level at cap, and at 0 from below.
#define EFFECT_CAP_LEVEL(level, cap)                                           \
  {                                                                            \
    if ((level) > (cap))                                                       \
      (level) = (cap);                                                         \
    if ((level) < 0)                                                           \
      (level) = 0;                                                             \
  }
/// The bytes of BlueGlowSheet's ten flare pictures, 2 to 20 pixels square:
/// 4 * (1 + 4 + ... + 100).
#define FLARE_SET_BYTES 1540
/// The battle sheet (HEAP_SLOT_BATTLE_SHEET) the casting impacts, the plain
/// attacks and Cruel Ruin decode their glows into: room for BlueGlowSheet's
/// flares, the largest of them, with ten bytes to spare.
#define BATTLE_SHEET_BYTES (FLARE_SET_BYTES + 10)
#define MOTE_PICTURES 10

/// The columns of gTargetStrike_Styles, one row a style.
enum TargetStrikeColumn {
  TARGET_STRIKE_KIND,           ///< STRIKE_KIND_SLASH or STRIKE_KIND_BITE
  TARGET_STRIKE_HITS,           ///< slashes or bites
  TARGET_STRIKE_FLAGS,          ///< the extras (STRIKE_SMOKE and on)
  TARGET_STRIKE_COLOR,          ///< the background's palette (STRIKE_COLOR_)
  TARGET_STRIKE_PICTURE_FRAMES, ///< frames each picture shows
  TARGET_STRIKE_COMPOSITE,      ///< the bites' blitter mode
  TARGET_STRIKE_FRAMES          ///< the effect's length
};

// The two kinds of target strike (gTargetStrike_Styles).
#define STRIKE_KIND_SLASH 0 ///< sideways slashes
/// Rows of fangs above and below the target that close on it.
#define STRIKE_KIND_BITE 1

// A target strike style's extras.
#define STRIKE_SMOKE 1  ///< each hit raises smoke puffs from the target's feet
#define STRIKE_ABSORB 2 ///< motes fly out of the target and back into the user
#define STRIKE_GLINTS 4 ///< glints sparkle around the target
#define STRIKE_SPARKS 8 ///< each hit scatters sparks
#define STRIKE_HEAVY 16 ///< one heavy hit: a big shake and the target recoils
#define STRIKE_FLASH 32 ///< as heavy, with flash pictures for the smoke
#define STRIKE_STAR 64  ///< a star swells on the target

// The palettes a target strike's background takes (TARGET_STRIKE_COLOR).
#define STRIKE_COLOR_BLUE 0      ///< BlueBeamSheet's
#define STRIKE_COLOR_LIME 1      ///< LimePalette
#define STRIKE_COLOR_PINK 2      ///< PinkPalette
#define STRIKE_COLOR_EMBER 3     ///< EmberStreakSheet's
#define STRIKE_COLOR_MARS 4      ///< MarsDjinnSheet's
#define STRIKE_COLOR_LIGHTNING 5 ///< LightningBoltSheet's
#define STRIKE_COLOR_ORANGE 6    ///< OrangePaletteB

/// The target strike styles, rows of gTargetStrike_Styles: The Broken Seal's
/// twelve, then this game's own.
enum TargetStrikeStyle {
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
  TARGET_STRIKE_STYLE_MANEATER, ///< the same row as MARS_ADDITIVE_DRAIN_BITES
  TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH,
  TARGET_STRIKE_STYLE_MARS_HEAVY_BITE,
  TARGET_STRIKE_STYLE_ORANGE_FLASH_BITE,
  TARGET_STRIKE_STYLE_MARS_STAR_SLASH,
  TARGET_STRIKE_STYLE_BLUE_SPARK_BITE,
};

/// Window 0 shows the battle view: BG0 to BG2, the sprites and blending.
#define EFFECT_WIN0_LAYERS                                                     \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR)

/// Outside the battle view: BG0 and blending only.
#define EFFECT_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

/// Opens the canvas layer at its own width: BG2 shows it one pixel a pixel.
#define EFFECT_START_NARROW_CANVAS(bgControl)                                  \
  (BattleEffect_StartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/// Strikes target `i`: it flashes white in its damage pose and is knocked back
/// by `motion`.
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
  (BattleEffect_TintBattler((effect)->targets[i], SPRITE_COLOR_WHITE,          \
                            POSE_DAMAGE, i, frames),                           \
   BattleMotion_ApplyKnockbackFar((effect)->targets[i], motion))

/// A blitter that clips its rectangles to the canvas.
#define EFFECT_CLIP_BOUNDS (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// The second blitter, the drawing routine in the paired heap slot.
#define ALT_BLITTER ((DrawRectangle)Ram_HeapSlots->ramCodePaired)

/// Builds a blitter for the canvas in heap slot `slot`, for one clip, mirror
/// and blend mode (BLITTER_ADDITIVE to BLITTER_COPY): where both are constants
/// they go packed into one word to BattleEffect_LoadBlitter, the shorter call,
/// and otherwise to BattleEffect_BuildBlitter as they are. The name is ours:
/// every glint draw and mirrored pair that picks its flags or its mode at run
/// time is written so.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  (__builtin_constant_p((flags) | (mode) << 4)                                 \
       ? BattleEffect_LoadBlitter(slot, (flags) | (mode) << 4)                 \
       : BattleEffect_BuildBlitter(slot, 7, 7, flags, mode))

/// Sets what the canvas transfer leaves in the canvas after each copy.
#define EFFECT_SET_TRANSFER(state, mode, value)                                \
  ((state)->transferMode = (mode), (state)->transferValue = (value))

/// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(state, mode, value)                              \
  (EFFECT_SET_TRANSFER(state, mode, value),                                    \
   Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK))

/// Presents the canvas and waits for the next frame.
#define EFFECT_PRESENT_CANVAS(state) ((state)->transferPending = 1, wait(1))

/// Waits until a VBlank has passed since the one that last copied the canvas.
// DEAD: each test reads the count twice and drops the first read; the ROM
// keeps both loads.
#define WAIT_TRANSFER(state)                                                   \
  while ((state)->vblanksSinceTransfer, (state)->vblanksSinceTransfer <= 1)

/// The usual end of an effect's frame: counts down the targets' flash timers,
/// then presents the canvas.
#define EFFECT_END_FRAME(state)                                                \
  (BattleEffect_CountDownBattlerTints(), EFFECT_PRESENT_CANVAS(state))

/// Frees the two blitters, the second first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED),                                \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE))

/// The usual end of a canvas effect: the transfer, the blitters, then the
/// canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Task_Remove(BattleEffect_TransferCanvas), EFFECT_RELEASE_BLITTERS(),        \
   BattleEffect_EndCanvasLayer())

/// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Task_Remove(BattleEffect_TransferCanvas),                                   \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE), BattleEffect_EndCanvasLayer())

/// The largest of the ten mote pictures.
#define MOTE_SIZE_MAX 10
/// Draws mote picture `size` (1 to MOTE_SIZE_MAX: size pixels wide, twice that
/// tall), centred across x and standing on y.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleEffect_MotePictures[(size) - 1],             \
         (x) - (size) / 2, (y) - (size), size, (size) * 2)

/// Marks particles[0] to particles[count - 1] free: a particle whose variant
/// is -1 is not drawn or moved until an effect gives it a life.
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
  for ((i) = 0; (i) != (count); (i)++)                                         \
  (particles)[i].variant = -1

/// Shows the canvas stretched to twice its width, `top` sixteenths of its
/// colour added over what lies under it.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = EFFECT_BLEND_CANVAS(top),                                    \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// A line's 16.16 sideways sway as its BG2X value in state->bg2xLines, which
/// the wave HBlank DMA writes line by line. BG2X counts 256ths of a canvas
/// pixel, so a sway of toFIXED(n) moves the line n / 4 canvas pixels: n / 2
/// screen pixels where the canvas is shown at twice its width.
#define CANVAS_WAVE_BG2X(sway) ((sway) >> 10)

/// Which way BattleEffect_WipeCanvas sweeps.
enum CanvasWipeDirection {
  CANVAS_WIPE_DOWN,  ///< top to bottom
  CANVAS_WIPE_ACROSS ///< left to right
};
/// What BattleEffect_WipeCanvas leaves behind it.
enum CanvasWipeFill {
  CANVAS_WIPE_FILL, ///< colour 1, which its callers make black first
  CANVAS_WIPE_CLEAR ///< colour 0: the canvas cleared
};

/// Lets the battle show the damage, playing hitSound as the targets react: 0
/// keeps the one queued, and HIT_SOUND_NONE plays none where the effect has
/// played its own. A second use changes nothing.
#define EFFECT_SHOW_DAMAGE(hitSound) BattlePlayback_StartFar(hitSound)
#define HIT_SOUND_NONE (-1)

/// Passed to BattleEffect_TintBattler as its palette, pose or timer slot: leave
/// that one as it is.
#define TINT_KEEP (-1)

/// The tumbling quads' modes (BattleEffect_RunTumblingQuads), each named for
/// the effect that plays it.
enum TumblingQuadsMode {
  TUMBLING_QUADS_BALM,   ///< over a prepared canvas, with the Mercury Djinn
  TUMBLING_QUADS_TINDER, ///< over a prepared canvas, with the Mars Djinn
  TUMBLING_QUADS_PHOENIX ///< rising from the first target
};

/// The gasp family's modes (BattleEffect_RunGaspFamily), each named for the
/// effect that plays it.
enum GaspMode {
  GASP_MODE_GASP,
  /// Unused: no wrapper passes it; a Mercury Djinni and a burst of light.
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
};

/// The twisting column's modes (BattleEffect_RunTwistingColumn), each named for
/// the effect that plays it.
enum TwistingColumnMode {
  TWISTING_COLUMN_WHORL,
  TWISTING_COLUMN_GALE,
  TWISTING_COLUMN_LETHE_ALBION,
  TWISTING_COLUMN_POISON_FLOW,
  TWISTING_COLUMN_RAGING_FLOOD
};

/// The ball lightning's modes (BattleEffect_RunBallLightning).
enum BallLightningMode {
  BALL_LIGHTNING_THUNDER_MINE,
  BALL_LIGHTNING_EARTH_FORCE
};

/// The Djinni charge's modes (BattleEffect_RunDjinniCharge).
enum DjinniChargeMode { DJINNI_CHARGE_CORE, DJINNI_CHARGE_CANNON };

/// The heavy blow's modes (BattleEffect_RunHeavyBlow).
enum HeavyBlowMode { HEAVY_BLOW_HEAVY_DIVIDE, HEAVY_BLOW_STRONG_HIT };

/// The Djinni sign's modes (BattleEffect_RunDjinniSign).
enum DjinniSignMode { DJINNI_SIGN_RIME, DJINNI_SIGN_HAZE };

/// The cards (BattleEffect_RunCards).
enum CardKind {
  CARD_SLEEP,  ///< Sleep Card: sheep jump from the card.
  CARD_BAFFLE, ///< Baffle Card: butterflies, and the screen waves.
  CARD_DEATH,  ///< Death Card: a skeleton reaps the target.
  CARD_SWORD,  ///< Sword Card: a dagger strikes, and shards fly.
};

/// The element cards (BattleEffect_RunElementCards).
enum ElementCardKind {
  CARD_BRAMBLE, ///< Bramble Card: vines grow from the card.
  CARD_FROST,   ///< Frost Card: the card throws crystal shards.
  CARD_FLAME,   ///< Flame Card: the card throws smoke and flashes.
  CARD_THUNDER, ///< Thunder Card: bolts strike from the card.
};

/// The breaths (BattleEffect_RunBreaths).
enum BreathKind {
  BREATH_BEAM,          ///< Beam: a beam of fading rings.
  BREATH_ROLLING_FLAME, ///< Rolling Flame: a ball rolls to the target.
  BREATH_PIKE,          ///< Ghost Soldier and Pike Assault.
  BREATH_BLAST,         ///< Blast Breath: a blast and a jet.
};

/// The rushing strike's sparks' palettes (BattleEffect_RunRushingStrike), each
/// named for the effect that first plays it; Bone Charge plays Stun Muscle's.
enum RushingStrikeMode {
  /// Unused: no wrapper passes it; the fire pillar's colours.
  RUSHING_STRIKE_UNUSED,
  RUSHING_STRIKE_BRUTE_FORCE,
  RUSHING_STRIKE_ONSLAUGHT,
  RUSHING_STRIKE_HEADBUTT,
  RUSHING_STRIKE_STUN_MUSCLE,
  RUSHING_STRIKE_LIVING_ARMOR
};

/// The mirrored wall's modes (BattleEffect_RunMirroredWall), each named for the
/// effect that plays it.
enum MirroredWallMode {
  MIRRORED_WALL_SPASM, ///< actual size, a gentle shake for 32 frames
  /// A ninth wider than Spasm's, a hard shake for 8 frames.
  MIRRORED_WALL_BERSERK,
  /// A ninth wider than Spasm's, no shake, in the Fireball colours.
  MIRRORED_WALL_RECOVERY,
  MIRRORED_WALL_SACK
};

/// Where BattleEffect_RunSparkGroups' bursts go off.
enum SparkGroupsMode {
  SPARK_GROUPS_NOVA,     ///< Nova's
  SPARK_GROUPS_DYNAMITE, ///< Dynamite's, orange
  SPARK_GROUPS_RISING    ///< below the user, in the LightningBolt colours, the
                         ///< sparks rising
};

// The declarations the parts share.

struct SpriteAnimation;
struct BattleEffectParams;
struct ScreenPoint;
struct Particle;

/// A mesh triangle: its three vertex numbers and the texture coordinates of
/// each corner. A strip quad is two of them; a triangle whose three vertices
/// are 0 ends a list.
struct MeshTriangle {
  u8 vertex[3];
  u8 unusedA; ///< nothing reads or writes it
  u8 uv[3][2];
  u8 unusedB[2]; ///< nothing reads or writes them
};

struct PoolEntry;
struct SpriteScale;
struct StripQuad;

// The parts' routines that another part calls before their definition, by
// the part that first calls them.

// display.c
void BattleEffect_AdvanceScrollOnInterval(void);
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_AdvanceBg2OnInterval(void);

// canvas.c
void BattleEffect_StartCanvasLayer(s32 bgControl);
/// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
/// Counts down the actors' flash timers.
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void BattleEffect_ProcessPendingBlock2Transfer(void);
void Display_ApplyBg2Affine(void);
/// Sets up the canvas tile map on BG1.
void BattleEffect_SetupCanvasTileMap(void);
/// Marks the transition done and shows the battle view again.
void BattleEffect_HideBg1AndRestoreBackdrop(void);
void BattleEffect_TransferFullScreenImage(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
void BattleEffect_BuildCanvasTilemap(void);
/// The 64x64 stamp Haures draws in 256 colours: the VRAM address of its
/// sprite's tiles, two words before the state's end, past what struct
/// BattleEffectState names, and the picture drawn for them, the 4 KB before
/// transferMode.
#define STAMP_TILES(state)                                                     \
  (*(u32 *)((u8 *)(state) + BATTLE_EFFECT_STATE_SIZE - 2 * sizeof(u32)))
#define STAMP_CANVAS(state)                                                    \
  ((u8 *)(state) + offsetof(struct BattleEffectState, transferMode) -          \
   OBJ_64X64_8BPP_BYTES)
void BattleEffect_ProcessPendingStampTransfer(void);

// djinn_beams.c
/// Fetches the max and additive blitters, flipped or not.
void BattleEffect_BuildCanvasBlitters(s32 flip, void **out);

// prism.c
void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase);

// djinn_heal.c
/// state->cameraPhase, which BattleCamera_ApplyPhasedDelta and the effects
/// that turn the camera through it (Fire, Froth) pass between them.
enum CameraPhase {
  CAMERA_PHASE_IDLE,       ///< nothing marked since the last turn
  CAMERA_PHASE_FRAME_DONE, ///< the effect has finished a frame: turn the
                           ///< whole delta
  CAMERA_PHASE_HALF_TURNED ///< half a delta turned on an unmarked frame
};
void BattleCamera_ApplyPhasedDelta(void);

// dispatch.c
void BattleEffect_ShowLivingTargets(struct BattleEffectParams *args);
void BattleEffect_CallDoNothing(s32 cnt);
void BattleEffect_BuildWindowEdgeTable(void);

// battler_tint.c
/// Shows a battler in a palette and a pose, for `frames` frames counted in
/// timer slot `timerSlot`; TINT_KEEP leaves any of them as it is.
void BattleEffect_TintBattler(s32 unitId, s32 palette, s32 pose, s32 timerSlot,
                              s32 frames);
void BattleEffect_SetupBlendedDisplay(void);

// stat_change.c
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunWeaken(struct BattleEffectParams *args);

// eruption.c
void BattleEffect_CreateSprites(s32 count, s32 spriteId, u32 priority);

// projectiles.c
void Particle_AdvanceWithGravity3D(struct Particle *step, s32 damping,
                                   s32 gravity);
void Particle_AdvanceWithGravity2D(struct Particle *step, s32 damping,
                                   s32 gravity);
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *effect,
                                    u32 packed, s32 *screen, s32 *world);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

// djinn_attacks.c
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);

// hail.c
/// Shakes the camera while state->shakeFrames lasts.
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void Camera_ApplyVerticalShake(s32 range);

// gaia.c
struct SpriteAnimation *AnimationObject_AllocateEmpty(void);

// spider_web.c
/// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
/// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *effect, s32 mode);
void Palette_MirrorBgColors(void);
void Graphics_LoadGreyRamp(void);

// azul.c
void EffectPosition_ProjectBattler(s32 id, struct ScreenPoint *position);
/// The middle between the first and the last target.
void BattleEffect_GetTargetsMidpoint(struct ScreenPoint *position);
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void EffectPosition_ProjectSprite(s32 id, struct ScreenPoint *position);

// positions.c
void BattleEffect_RunCastingImpact(struct BattleEffectParams *action, s32 kind);
void BattleEffect_RunCriticalHit(struct BattleEffectParams *effect);
void BattleEffect_RunAttackSlash(struct BattleEffectParams *effect);

// casting_impact.c
/// Steps the background colours one shade toward a resource's palette.
void BattleEffect_StepPaletteToResource(s32 resource);

// canvas_wipe.c
void BattleEffect_WipeCanvas(s32 direction, s32 fill);

// particle_streams.c
void BattleEffect_PlaceFormationSprites(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, FIXED y);
void BattleEffect_ArmPaletteHBlankDma(void);
void Render_ApplyProjectedPlacementFar(struct Sprite *sprite, s32 *position,
                                       s32 *scale, s32 mode);

// ramses.c
/// An actor's recoil motion, through a far call.
void BattleMotion_ApplyKnockbackFar(s32 actor, s32 motion);

// ragnarok.c
void BattleEffect_RunFallingSword(struct BattleEffectParams *effect, s32 mode);

// thrash.c
void BattleEffect_RunFlashingStrike(struct BattleEffectParams *effect,
                                    s32 variant);

// undead_sword.c
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunWaterSheet(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunWindFunnel(struct BattleEffectParams *effect, s32 mode);

// heavy_divide.c
void BattleEffect_RunHeavyBlow(struct BattleEffectParams *effect, s32 mode);

// thunder_mine.c
void BattleEffect_RunBallLightning(struct BattleEffectParams *effect, s32 mode);

// whorl.c
void BattleEffect_RunTwistingColumn(struct BattleEffectParams *effect,
                                    u32 mode);
void BattleEffect_RunDjinniCharge(struct BattleEffectParams *effect, s32 mode);

// rime.c
void BattleEffect_RunDjinniSign(struct BattleEffectParams *effect, s32 mode);

// gasp.c
void BattleEffect_RunGaspFamily(struct BattleEffectParams *effect, s32 mode);

// raging_heat.c
void BattleEffect_FillWhiteBand(void);

// star_mine.c
void BattleEffect_BuildRepeatingTilemap(void);

// rushing_strike.c
void BattleEffect_RunRushingStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunMirroredWall(struct BattleEffectParams *effect, s32 mode);

// vengeance.c
void BattleEffect_RunGeyser(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunStarSwarm(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunBubbleColumn(struct BattleEffectParams *effect);

// legend.c
void BattleEffect_RunElementCards(struct BattleEffectParams *effect, s32 mode);

// cards.c
void BattleEffect_RunCards(struct BattleEffectParams *effect, s32 mode);

// breaths.c
void Canvas_BlitMirroredQuarters(void *source, s32 x, s32 y, s32 size);
void BattleEffect_RunBreaths(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunCrescentSlash(struct BattleEffectParams *effect, s32 mode);

// trident.c
void BattleEffect_RunLeapingStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_ArmMeteorBlowPaletteDma(void);

// liquifier.c
void BattleEffect_RunBallAttack(struct BattleEffectParams *effect);

// odyssey.c
/// The five-argument blitter builder's far call, which only a call in
/// odyssey.c that is never taken names.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);
void Iris_WipeScreen(u8 side, u8 speed, void (*midpoint)(void));
void Iris_RestoreBattle(void);
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats,
                                          s32 scale);

// moloch.c
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);

// target_motes.c
/// Glowing orbs spiral in on each target.
///
/// Abilities: Mystic Flame.
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args);

// Routines of other banks the parts call.
s32 Sprite_StartAnimationFar(void *object, s32 value);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
void BattleBackground_SetModeFar(s32 mode, s32 resource, s32 speed);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);
/// Turns the battle camera, over `frames` frames, to look at the point between
/// two units.
void BattleCamera_SetViewAtPairMidpointFar(s32 actor, s32 target, s32 frames);
void BattleCamera_SetViewFar(s32 x, s32 depth, s32 y, s32 zoom);
void BattleObject_PutFar(void);
/// Spawns the battle objects of a list of units through its far call.
void BattleObject_SpawnObjectsForListFar(s16 *unitIds, s32 refreshPoses,
                                         s32 releaseOthers);
/// BattlePlayback_Start through its far call.
void BattlePlayback_StartFar(s32 phase);
/// Whether an enemy's sprite takes the heavy impact, through its far call.
s32 BattleSprite_HasHeavyImpactFar(s32 enemyId);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void *Actor_GetSpriteFar(void *object, s32 index);
u32 Heap_GetRemainingIwram(void);
void Palette_LoadBasicSpriteColors(void);
u32 Random16(void);
void BattleObject_DestroySpritesFar(s32 actor);
s32 Matrix_ProjectPoint(const void *point, s32 *screen);
void Matrix_TranslateVector(const FIXED *pos);
void Matrix_Translate(FIXED x, FIXED y, FIXED z);
void Matrix_ScaleVector(FIXED *scale);
void Matrix_Scale(FIXED x, FIXED y, FIXED z);
void Matrix_ScaleUniform(FIXED scale);
void Sprite_RemoveAnimationFar(void *record, s32 entry);
void Matrix_RotateX(s32 angle);
void Matrix_RotateZ(s32 angle);
void Matrix_RotateY(s32 angle);
void Matrix_Pop(void);
void Matrix_Push(void);
void Matrix_ResetUnit(void);
void Matrix_SetLookAt(FIXED *eye, FIXED *target);
FIXED Trig_Cos(s32 angle);
FIXED Trig_Sin(s32 angle);
void ActorShadow_LoadTilesFar(void);
void ActorShadow_FreeTilesFar(void);
void wait(s32 frames);

// The tables of effect_table.c, in its order.

/// Where each size of mote and of flare starts in its sprite sheet, in bytes.
extern u16 gBattleEffect_MotePictures[];
extern u16 gBattleEffect_FlarePictures[];
/// Where each of Cruel Ruin's circles starts in its sprite sheet.
extern const u16 gCruelRuin_CirclePictures[];
/// The pictures several effects share: the sparkles, the ember puff that rises
/// in six pictures, and the six glints, with their sizes and offsets.
extern u16 gBattleEffect_SparklePictures[];
extern u8 gBattleEffect_SparkleSizes[];
extern u8 gBattleEffect_EmberStreakPictureWidths[];
extern u8 gBattleEffect_EmberStreakPictureHeights[];
extern u8 gBattleEffect_EmberStreakPictureBiasY[];
extern u16 gBattleEffect_EmberStreakPictures[];
extern u16 gBattleEffect_GlintPictures[];
extern u8 gBattleEffect_GlintPictureWidths[];
extern u8 gBattleEffect_GlintPictureHeights[];
/// Spire's shard pictures.
extern u8 gSpire_ShardWidths[];
extern u8 gSpire_ShardHeights[];
extern u16 gSpire_ShardPictures[];
/// The stat change's shard pictures.
extern u16 gStatChange_ShardPictures[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];
/// Where the Death Card's Reaper draws its seven panels, seven x for each side.
extern u8 gReaper_PictureX[];
extern u8 gReaper_PictureY[];
/// Ice's shards and frames, and Douse's drops, by variant.
extern u8 gIce_Counts[];
extern u8 gDouse_Counts[];
/// The streaks' styles, and the struck target's tremble.
extern s8 gStreak_Styles[];
extern s8 gStreak_Tremble[];
/// The target strikes: their styles (enum TargetStrikeColumn), the slash
/// pictures and how far each reaches, the bite's stages and the glints' flips.
extern u8 gTargetStrike_Styles[][7];
extern u8 gTargetStrike_Widths[];
extern u8 gTargetStrike_Heights[];
extern u16 gTargetStrike_Pictures[];
extern u8 gTargetStrike_Reach[];
extern u8 gTargetStrike_BiteStages[];
extern u8 gTargetStrike_GlintDrawFlags[];
/// The 33 pieces the screen shatters into as Death Plunge opens: their pictures
/// and where they start.
extern u8 gShatteringOpening_PieceWidths[];
extern u8 gShatteringOpening_PieceHeights[];
extern u16 gShatteringOpening_PiecePictures[];
extern u8 gShatteringOpening_PieceStartX[];
extern u8 gShatteringOpening_PieceStartY[];
/// The element burst's four corner sparks: their places and flips.
extern s8 gElementBurst_SparkX[];
extern s8 gElementBurst_SparkY[];
extern u8 gElementBurst_GlintFlips[];
/// Cruel Ruin: the streak's corners, the discs' turns, the pillars, the blasts,
/// and each streak's start, size, path and roll.
extern const struct ModelVertex gCruelRuin_StreakCorners[];
extern const u16 gCruelRuin_DiscPitches[];
extern const u16 gCruelRuin_DiscYaws[];
extern const u8 gCruelRuin_PillarSways[];
extern const u8 gCruelRuin_PillarWidths[];
extern const u8 gCruelRuin_BlastFrames[];
extern const u8 gCruelRuin_BlastSizes[];
extern const u8 gCruelRuin_BlastX[];
extern const u8 gCruelRuin_BlastY[];
extern const u8 gCruelRuin_StreakStarts[];
extern const u8 gCruelRuin_StreakSizes[];
extern const u8 gCruelRuin_StreakX[];
extern const u8 gCruelRuin_StreakStepX[];
extern const u8 gCruelRuin_StreakStepY[];
extern const u8 gCruelRuin_StreakEndY[];
extern const s32 gCruelRuin_StreakRolls[];
/// Eclipse: the glints' flips, the dragon's sprites that take new tiles as it
/// turns and their places, the canvas's fills, and the rings.
extern const u8 gEclipse_GlintFlips[];
extern const u8 gEclipse_TileObjects[];
extern const u8 gEclipse_FillFrames[];
extern const u32 gEclipse_FillLevels[];
extern const u8 gEclipse_ObjectX[];
extern const u8 gEclipse_ObjectZ[];
extern const u8 gEclipse_RingGrowth[];
extern const s8 gEclipse_RingHeights[];
/// The form change's four waves: when each starts, how fast it grows and how
/// far it is stretched.
extern u8 gFormChange_WaveStarts[];
extern u8 gFormChange_WaveGrowth[];
extern u8 gFormChange_WaveStretch[];
/// Prism's crystal and the shards it breaks into: their places and pictures.
extern u8 gPrism_ShardStarts[];
extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern s32 gPrism_ShardPictures[];
/// The gathering lights' motes for each target, and the frames before the
/// targets' share, by mode.
extern u8 gGatheringLights_Counts[];
/// The Djinn beams: the motes' pictures as they sway, and the beam head's three
/// pictures.
extern u8 gDjinnBeams_MotePictures[];
extern u16 gDjinnBeams_HeadPictures[];
extern u8 gDjinnBeams_HeadWidths[];
extern u8 gDjinnBeams_HeadHeights[];
/// The lobbed shells: each style's record (LOBBED_SHELLS_SHATTER and the rest),
/// and the shards' pictures.
extern s8 gLobbedShells_Styles[];
extern u8 gLobbedShells_ShardWidths[];
extern u8 gLobbedShells_ShardHeights[];
extern u16 gLobbedShells_ShardPictures[];
/// Where Condemn draws its seven pictures of the reaper and his crescent, by
/// side.
extern u8 gCondemn_PictureX[];
extern u8 gCondemn_PictureY[];
/// Bind's triangles: the first corner of each.
extern s32 gBind_Vertex[];
extern s32 gBind_TriangleStrikeVertex[];
/// The star's ring: its points and radii, alternating.
extern s32 gStar_Points[];
extern u8 gStar_Radii[];
/// Procne: the grid her nine sprites sit on, the four strikes' frames and a
/// strike's three pictures.
extern u8 gProcne_GridX[];
extern u8 gProcne_GridY[];
extern u16 gProcne_Frames[];
extern u8 gProcne_PictureWidths[];
extern u8 gProcne_PictureHeights[];
extern u8 gProcne_PictureBiasY[];
extern u16 gProcne_Pictures[];
/// Tiamat: the embers' speeds and sizes, a spark's two pixels, and the flame
/// streak's six pictures.
extern u8 gTiamat_EmberSpeeds[];
extern u8 gTiamat_SparkPixels[];
extern u8 gTiamat_EmberSizes[];
extern u8 gTiamat_EmberStreakWidths[];
extern u8 gTiamat_EmberStreakHeights[];
extern u8 gTiamat_EmberStreakBiasY[];
extern u16 gTiamat_EmberStreakPictures[];
/// Nereid: the three water columns' feet and the falling sparks' gravity.
extern u8 gNereid_Columns[];
extern s32 gNereid_Gravity[];
/// The vortices: Froth's counts and the whirlwind's shapes, by variant (the
/// FROTH_ and WHIRLWIND_ columns).
extern u8 gVortices_Counts[][2];
extern u8 gVortices_Shapes[][3];
/// Prism's crystals, frames and damage frame, by variant.
extern u8 gPrism_Counts[];
/// Plasma: the sparks and shake of each lightning pillar, the columns, and the
/// counts, by variant.
extern u16 gPlasma_Sparks[];
extern u8 gPlasma_Columns[];
extern u8 gPlasma_Counts[];
/// The spark groups' shapes, by variant: sparks in a group, groups and each
/// group's x.
extern u16 gSparkGroups_Shapes[][5];
/// Plasma's flash burst picture for each three frames of a flash.
extern u8 gPlasma_FlashPictures[];
/// Volcano's pillars of fire: how many, their depths and when each erupts.
extern u8 gVolcano_Counts[];
extern s8 gVolcano_Depths[][4];
extern u8 gVolcano_StartFrames[];
/// The target motes' glint flips.
extern u8 gTargetMotes_GlintFlips[];
/// Zagan: where his sprites sit as he charges and as he gallops, and his dust
/// rings.
extern const u16 gZagan_ChargePoints[][2];
extern const u16 gZagan_GallopPoints[][2];
extern const u8 gZagan_RingStarts[];
extern const u8 gZagan_RingGrowth[];
extern const s8 gZagan_RingHeights[];
/// Cybele: her sprites' poses, and the pictures of her thorns, flowers, chips,
/// sparks and blasts.
extern u8 gCybele_ObjectPoses[];
extern u16 gCybele_ThornPictures[];
extern u8 gCybele_ThornWidths[];
extern u8 gCybele_ThornHeights[];
extern u16 gCybele_FlowerPictures[];
extern u8 gCybele_FlowerSizes[];
extern u16 gCybele_ChipPictures[];
extern u8 gCybele_ChipWidths[];
extern u8 gCybele_ChipHeights[];
extern u16 gCybele_SparkPictures[];
extern u8 gCybele_SparkSizes[];
extern u16 gCybele_BlastPictures[];
extern u16 gCybele_BlastSizes[];
/// Boreas: the crystals' points, the shake, his sprites' animations, and the
/// pictures of the crystals, debris, spray, rising crystals and falls.
extern u8 gBoreas_Points[][2];
extern s8 gBoreas_ShakeOffsets[];
extern u16 gBoreas_CrystalPictures[];
extern u8 gBoreas_CrystalWidths[];
extern u8 gBoreas_CrystalHeights[];
extern u8 gBoreas_Animations[];
extern u8 gBoreas_DebrisWidths[];
extern u8 gBoreas_DebrisHeights[];
extern u16 gBoreas_DebrisPictures[];
extern u8 gBoreas_SprayWidths[];
extern u8 gBoreas_SprayHeights[];
extern u16 gBoreas_SprayPictures[];
extern u16 gBoreas_RisingCrystalPictures[];
extern u8 gBoreas_RisingCrystalWidths[];
extern u8 gBoreas_RisingCrystalHeights[];
extern u8 gBoreas_FallWidths[];
extern u8 gBoreas_FallHeights[];
extern u16 gBoreas_FallPictures[];
/// Drain's motes: their gravities, and their size by mode.
extern s32 gDrain_Gravities[];
extern u8 gDrainMotes_Sizes[];
/// Quake's frames, rise and shake, by variant.
extern u8 gQuake_Timings[][3];
/// Fire's counts by variant, and its flare pictures.
extern u8 gFire_Counts[];
extern u8 gFire_EmberStreakWidths[];
extern u8 gFire_EmberStreakHeights[];
extern u8 gFire_EmberStreakBiasY[];
extern u16 gFire_EmberStreakPictures[];
/// Frost: where the mounds rise, its timings by variant, and the pictures of
/// its motes, mounds and ice chips.
extern u8 gFrost_MoundSpots[];
extern u8 gFrost_Timings[];
extern u8 gFrost_MoteWidths[];
extern u8 gFrost_MoteHeights[];
extern u16 gFrost_MoteCells[];
extern u16 gFrost_MoundPictures[];
extern u8 gFrost_MoundHeights[];
extern u8 gFrost_ChipWidths[];
extern u8 gFrost_ChipHeights[];
extern u16 gFrost_ChipCells[];
/// The sparks each of Ray's bolts throws a frame, by variant.
extern u8 gRay_Sparks[][2];
/// Eruption's counts by variant, and its puffs' pictures.
extern u8 gEruption_Counts[][3];
extern u16 gEruption_PuffPictures[];
extern u16 gEruption_PuffSizes[];
/// The burst shower's glint flips.
extern u8 gBurstShower_GlintFlips[];
/// Atalanta's sprites' scale, one to one.
extern s32 gAtalanta_UnitScale[];
/// Growth: the vines' and pillars' pictures, the columns by strength and where
/// they root, how tall they grow, and the dust puffs.
extern u8 gGrowth_VineWidths[];
extern u8 gGrowth_HeightLimits[];
extern u16 gGrowth_VinePictures[];
extern u8 gGrowth_PillarWidths[];
extern u16 gGrowth_PillarPictures[];
extern u8 gGrowth_ColumnCounts[];
extern u8 gGrowth_ColumnX[];
extern s8 gGrowth_PillarHeights[];
extern s8 gGrowth_PuffWidths[];
extern u8 gGrowth_PuffHeights[];
extern u16 gGrowth_PuffPictures[];
/// Each vine's x about the middle of the targets.
extern const s8 gVine_ColumnOffsets[];
/// Each thorn's kind (THORN_MIRRORED, THORN_SMALL).
extern u8 gThorn_Kinds[];
// A thorn's kind in gThorn_Kinds.
#define THORN_MIRRORED 1 ///< drawn mirrored across
#define THORN_SMALL 2    ///< 32 pixels wide, not 48
/// The thorns: where each roots, how many by strength, and the dust puffs as
/// each bursts out.
extern s8 gThorn_X[];
extern u8 gThorn_Counts[];
extern s8 gThorn_PuffWidths[];
extern u8 gThorn_PuffHeights[];
extern u16 gThorn_PuffPictures[];
/// Bolt: its sparks, glints, spread and bolts by strength, and how its glints
/// are flipped and blended.
extern u8 gBolt_Counts[];
extern u8 gBolt_GlintFlips[];
extern u8 gBolt_GlintModes[];
/// The projectile volley: the comets' heights, each mode's record (enum
/// VolleyField), the hit flash's pictures and spread, the pellets' pictures and
/// the glints' flips.
extern u8 gProjectileVolley_CometHeights[];
extern u8 gProjectileVolley_Modes[][PROJECTILE_VOLLEY_COLUMNS];
extern u8 gProjectileVolley_FlashWidths[];
extern u8 gProjectileVolley_FlashHeights[];
extern u8 gProjectileVolley_FlashSteps[];
extern u8 gProjectileVolley_FlashRises[];
extern u16 gProjectileVolley_FlashPictures[];
extern u16 gProjectileVolley_PelletPictures[];
extern u8 gProjectileVolley_GlintFlips[];
/// The frames the Djinni throws its five flames on.
extern const u8 gDjinnAttacks_LaunchFrames[];
/// Hail's four ice chip pictures.
extern u8 gHail_PictureWidths[];
extern u8 gHail_PictureHeights[];
extern u16 gHail_Pictures[];
/// Ground: the crackles' flips, and the portals' pictures and places.
extern const u8 gGround_CrackleFlips[];
extern const u8 gGround_PortalWidths[];
extern const u8 gGround_PortalHeights[];
extern const u16 gGround_PortalPictures[];
extern const u8 gGround_PortalX[];
extern const u8 gGround_PortalY[];
/// Thor: the palettes a struck target flashes in, and the crackles' flips.
extern const u8 gThor_TargetPalettes[];
extern const u8 gThor_CrackleFlips[];
/// Spire: where each shard sits in the boulder, the boulders' x and drop
/// frames, and how many by variant.
extern u8 gSpire_ShardOffsets[];
extern s8 gSpire_RockX[];
extern u8 gSpire_DropFrames[];
extern u8 gSpire_RockCounts[];
/// The flurries: their styles (enum FlurryColumn) and the glints' flips.
extern u8 gFlurry_Styles[][8];
extern const u8 gFlurry_GlintFlips[];
/// Gaia: the growing wall's rows, the flame's pictures, the wall layer's shift
/// and the late rocks.
extern u16 gGaia_RowPictures[];
extern u8 gGaia_RowWidths[];
extern u16 gGaia_FlamePictures[];
extern u8 gGaia_FlameWidths[];
extern u8 gGaia_FlameHeights[];
extern s8 gGaia_Bg2Shifts[][3];
extern u8 gGaia_LateRockColumns[];
/// Ulysses: where the mage's sprites sit, the summon's pictures, and where its
/// six particles start.
extern const u8 gUlysses_ObjectX[];
extern const u8 gUlysses_ObjectZ[];
extern const u8 gUlysses_PictureWidths[];
extern const u8 gUlysses_PictureHeights[];
extern const u16 gUlysses_Pictures[];
extern const s8 gUlysses_ParticleStarts[][5];
/// The swirl swipe's six pictures: their sizes, pictures, x and rise.
extern const u8 gSwirlSwipe_PictureWidths[];
extern const u8 gSwirlSwipe_PictureHeights[];
extern const u16 gSwirlSwipe_Pictures[];
extern const u8 gSwirlSwipe_PictureX[];
extern const s8 gSwirlSwipe_PictureBiasY[];
/// Azul: the points of the battlefield's shattered parts, and the serpent's
/// sprites, early and late.
extern const u8 gAzul_PartPoints[][2];
extern const struct Scale gAzul_UnitScale;
extern const u8 gAzul_ObjectX[];
extern const u8 gAzul_ObjectZ[];
extern const struct Scale gAzul_LateUnitScale;
extern const s8 gAzul_LateObjectX[];
extern const s8 gAzul_LateObjectZ[];
/// The plain attacks' beam: its six pictures and how far each reaches.
extern u8 gBeam_PictureWidths[];
extern u8 gBeam_PictureHeights[];
extern u16 gBeam_Pictures[];
extern u8 gBeam_PictureReaches[];
/// The casting impacts: glint flips, the bite's pictures, Raiden's Wrath's and
/// Apocalypse's pictures and places, the ice chips, and Stun Jip's bands and
/// Armor Crush's waves.
extern const u8 gCastingImpact_SquallGlintFlips[];
extern const u8 gCastingImpact_SapGlintFlips[];
extern const u8 gCastingImpact_BitePictures[];
extern const u16 gCastingImpact_RaidensWrathPictures[];
extern const u16 gCastingImpact_RaidensWrathWidths[];
extern const u16 gCastingImpact_RaidensWrathHeights[];
extern u16 gCastingImpact_RaidensWrathX[];
extern u16 gCastingImpact_RaidensWrathY[];
extern const u16 gCastingImpact_ApocalypsePictures[];
extern const u16 gCastingImpact_ApocalypseWidths[];
extern const u16 gCastingImpact_ApocalypseHeights[];
extern const u16 gCastingImpact_ApocalypseX[];
extern u16 gCastingImpact_ApocalypseY[];
extern const u16 gCastingImpact_ApocalypseBurstPictures[];
extern const u8 gCastingImpact_ApocalypseBurstWidths[];
extern const u8 gCastingImpact_ApocalypseBurstHeights[];
extern u8 gCastingImpact_ApocalypseBurstPoints[][2];
extern const u16 gCastingImpact_IceChipPictures[];
extern const u8 gCastingImpact_StunBoltGlintFlips[];
extern const u8 gCastingImpact_StunJipBandStarts[];
extern const u8 gCastingImpact_StunJipBandGrowth[];
extern const s8 gCastingImpact_StunJipBandHeights[];
extern const u8 gCastingImpact_ArmorCrushWaveStarts[];
extern const u8 gCastingImpact_ArmorCrushWaveGrowth[];
extern const u8 gCastingImpact_ArmorCrushWaveStretch[];
/// The unused copy of Nereid: its columns and gravity.
extern u8 gNereidCopy_Columns[];
extern const s32 gNereidCopy_Gravity[];
/// The summons' sprite formations, each sprite's x and depth: Nereid's,
/// Cybele's, Tiamat's two and Megaera's.
extern const u8 gSummonFormation_NereidX[];
extern const u8 gSummonFormation_NereidZ[];
extern const u8 gSummonFormation_CybeleX[];
extern const u8 gSummonFormation_CybeleZ[];
extern const u8 gSummonFormation_TiamatAlternateX[];
extern const u8 gSummonFormation_TiamatAlternateZ[];
extern const u8 gSummonFormation_TiamatX[];
extern const u8 gSummonFormation_TiamatZ[];
extern const s8 gSummonFormation_MegaeraX[];
extern const u8 gSummonFormation_MegaeraZ[];
/// The impact burst's pictures.
extern u8 gImpactBurst_PictureWidths[];
extern u8 gImpactBurst_PictureHeights[];
extern u16 gImpactBurst_Pictures[];
/// The starfield fall: the orbiting sparks, the fireballs and the flash
/// pictures.
extern s16 gStarfieldFall_OrbitPoints[][3];
extern s16 gStarfieldFall_FireballPoints[][2];
extern u16 gStarfieldFall_FlashPictures[];
extern u16 gStarfieldFall_FlashSizes[];
/// Ramses: where each of the statue's sprites sits, and the puffs' pictures.
extern u8 gRamses_StatueColumns[];
extern u8 gRamses_StatueRows[];
extern u16 gRamses_PuffPictures[];
extern u16 gRamses_PuffSizes[];
/// Ragnarok: the strikes' columns and the flash's pictures.
extern u8 gRagnarok_StrikeColumns[];
extern u8 gRagnarok_FlashPictures[];
/// Titan Blade: the flash's pictures and the dust's gravities.
extern u8 gTitanBlade_FlashPictures[];
extern s32 gTitanBlade_DustGravity[];
/// Judgment: the sprites its shield is copied into, and the shards' pictures.
extern const u8 gJudgment_ShieldSprites[];
extern const u16 gJudgment_ShardPictures[];
extern const u8 gJudgment_ShardWidths[];
extern const u8 gJudgment_ShardHeights[];
/// Kirin: the row's x offsets and heights.
extern u8 gKirin_Columns[];
extern u8 gKirin_Rows[];
/// Neptune's spray: its four gravities.
extern s32 gNeptune_Gravity[];
/// Undead Sword's sparks: their pictures and sizes.
extern const u16 gUndeadSword_PuffPictures[];
extern const u16 gUndeadSword_PuffSizes[];
/// The wind funnels, by mode.
extern const u8 gWindFunnel_Counts[];
/// Diamond Berg: its ripples, and its flashes' roll and pitch.
extern const u8 gDiamondBerg_RingStarts[4];
extern const u8 gDiamondBerg_RingGrowth[4];
extern const s8 gDiamondBerg_RingHeights[4];
extern const s16 gDiamondBerg_ShardRolls[8];
extern const s16 gDiamondBerg_ShardPitches[8];
/// Geode's rings: when each starts, how fast it grows, and its height.
extern const u8 gGeode_WaveStarts[];
extern const u8 gGeode_WaveGrowth[];
extern const s8 gGeode_WaveHeights[];
/// Soul Shatter's sparks: their pictures and sizes.
extern u16 gSoulShatter_SparkPictures[];
extern u16 gSoulShatter_SparkSizes[];
/// Serac: the spire's rings, and the ripples of mist.
extern const s8 gSerac_RowHeights[];
extern const s8 gSerac_RowRadii[];
extern const u8 gSerac_RingStarts[];
extern const u8 gSerac_RingGrowth[];
extern const s8 gSerac_RingHeights[];
/// The heavy blow: its strike frame and chips, by mode, and a chip's two
/// pixels.
extern u8 gHeavyBlow_StrikeFrames[];
extern u8 gHeavyBlow_ChipCounts[];
extern const u8 gHeavyBlow_ChipPixels[];
/// Cool: its columns of ice and its sparks.
extern s16 gCool_ColumnX[];
extern u8 gCool_ColumnStarts[];
extern u8 gCool_ColumnHeights[];
extern u8 gCool_ColumnLeans[];
extern u16 gCool_SparkPictures[];
extern u16 gCool_SparkSizes[];
/// Wyrd Curse's drops: their bounce speeds.
extern const s8 gWyrdCurse_DropBounces[];
/// The ball lightning's glint flips.
extern const u8 gBoltTrail_GlintFlips[];
/// Megaera's glints: their flips and how they are blended, by variant.
extern const u8 gMegaera_GlintFlips[];
extern const u8 gMegaera_GlintModes[];
/// Light Surge's four rings: their full sizes, shades and sways.
extern const s32 gLightSurge_RingSizes[4];
extern const s8 gLightSurge_RingShades[4];
extern s8 gLightSurge_RingSways[4];
/// The Djinni charge: the Djinni's entrance by mode, and the debris's
/// gravities.
extern const u8 gDjinniCharge_EntranceFlags[];
extern const s32 gDjinniCharge_DebrisGravity[4];
/// The Gasp family: the streak's steps and mesh, and the counts by variant.
extern const s8 gGaspFamily_StreakSteps[];
extern const u16 gGaspFamily_StreakMesh[];
extern const u8 gGaspFamily_Counts[];
/// The frames Charon's canvas flashes.
extern u8 gCharon_FlashFrames[];
/// Models several effects share: a band's and a cylinder's corners.
extern const struct ModelVertex gBattleEffect_OctagonBandCorners[];
extern const struct ModelVertex gBattleEffect_CylinderCorners[];
/// The casting impacts' band mesh.
extern const u16 gCastingImpact_BandMesh[];
/// The geyser's mesh.
extern const u16 gGeyser_Mesh[];
/// The cylinder's mesh, the ripples and rings several effects spread.
extern const u16 gBattleEffect_CylinderMesh[];
/// Geode's ring's corners.
extern const struct ModelVertex gGeode_RingCorners[];
/// Models several effects share: a ring, a triangle, beams and squares, their
/// corners and meshes.
extern const struct ModelVertex gBattleEffect_RingCorners[];
extern const u16 gBattleEffect_RingMesh[];
extern const u16 gBattleEffect_TriangleMesh[];
extern const struct ModelVertex gBattleEffect_TriangleCorners[];
extern const struct ModelVertex gBattleEffect_TaperedBeamCorners[];
extern const struct ModelVertex gBattleEffect_BeamCorners[];
extern const struct ModelVertex gBattleEffect_LongBeamCorners[];
extern const struct ModelVertex gBattleEffect_SquareCorners[];
extern const struct ModelVertex gBattleEffect_StandingSquareCorners[];
/// A flame of Legend's fan: its corners.
extern struct ModelVertex gLegend_FanFlameCorners[];
/// Models several effects share: a square on the ground, and the meshes of
/// small squares and a column.
extern const struct ModelVertex gBattleEffect_GroundSquareCorners[];
extern const u16 gBattleEffect_SmallSquareMesh[];
extern const u16 gBattleEffect_SmallSquareMeshReversed[];
extern const u16 gBattleEffect_ColumnMesh[];
/// The heavy blow's sword mesh.
extern const u16 gHeavyBlow_SwordMesh[];
/// A tall quad's mesh.
extern const u16 gBattleEffect_TallMesh[];
/// Haures's band mesh.
extern const u16 gHaures_BandMesh[];
/// Catastrophe's burst mesh.
extern const u16 gCatastrophe_ForkMesh[];
/// The square meshes several effects share.
extern const u16 gBattleEffect_SquareMesh[];
extern const u16 gBattleEffect_LargeSquareMesh[];
/// Haures's burst mesh.
extern const u16 gHaures_BurstMesh[];
/// Heartbreak: the burst of sparks at the figure's hand, and the heart in its
/// fist burning away.
extern u8 gHeartbreak_SparkBurstWidths[];
extern u8 gHeartbreak_SparkBurstHeights[];
extern u16 gHeartbreak_SparkBurstPictures[];
extern u8 gHeartbreak_HeartBurnHeights[];
extern u8 gHeartbreak_HeartBurnY[];
extern u16 gHeartbreak_HeartBurnPictures[];
/// Raging Heat's sparks, spread, rings and embers, by level.
extern const u8 gRagingHeat_Styles[][4];
/// Juggle's styles, one row a mode.
extern u8 gJuggle_Styles[][7];
/// Flora: the petals' pictures, and when and where each bloom opens and how it
/// is turned.
extern const u8 gFlora_PetalSizes[];
extern const u16 gFlora_PetalPictures[];
extern const u16 gFlora_BloomFrames[];
extern const s16 gFlora_BloomRolls[];
extern const s16 gFlora_BloomPitches[];
extern const s8 gFlora_BloomX[];
extern const s8 gFlora_BloomY[];
/// Sabre Dance's dagger: its mesh and corners.
extern const u16 gSabreDance_DaggerMesh[];
extern const struct ModelVertex gSabreDance_DaggerCorners[];
/// Fume's levels, one row a level (enum FumeLevelColumn).
extern const u16 gFume_Levels[][FUME_COLUMNS];
/// Ocean Fist's early drops: their gravities.
extern const s32 gOceanFist_DropGravity[4];
/// The rushing strike's streak scale, by mode.
extern const s32 gRushingStrike_Scales[];
/// The mirrored wall: its scale by mode and its bursts' shades.
extern const s32 gMirroredWall_Scales[];
extern const s32 gMirroredWall_Shades[];
/// Daedalus: the shake, the birds' wing beats and drifts, the clatters, the
/// rings, the leg, the splashes and the dives.
extern s8 gDaedalus_ShakeOffsets[];
extern u8 gDaedalus_WingBeatPictures[];
extern u8 gDaedalus_BirdStarts[][2];
extern s32 gDaedalus_BirdSpeeds[][2];
extern u8 gDaedalus_BirdFrames[];
extern u16 gDaedalus_ClatterFrames[];
extern u16 gDaedalus_RingStarts[];
extern s8 gDaedalus_RingX[];
extern u8 gDaedalus_RightLegPoints[][2];
extern u8 gDaedalus_SplashPoints[][6][2];
extern u16 gDaedalus_DiveStarts[];
/// The seven frames Daedalus's strikes land on.
extern const u8 gDaedalusStrike_HitFrames[];
/// Which of Vengeance's seven arrows strike, by level.
extern const u8 gVengeance_ArrowStrikes[][7];
/// Life Shear's scythe: its two blades' meshes and its corners.
extern const u16 gLifeShear_PaleBladeMesh[];
extern const u16 gLifeShear_OrangeBladeMesh[];
extern const struct ModelVertex gLifeShear_BladeCorners[];
/// The geyser's drops: their gravity and smallest picture, by mode (enum
/// GeyserMode).
extern const s32 gGeyser_DropGravity[];
extern const s8 gGeyser_DropMinSizes[];
/// The unused bubble column's glowing oval, fifteen pictures.
extern u8 gBubbleColumn_PictureWidths[];
extern u8 gBubbleColumn_PictureHeights[];
extern u16 gBubbleColumn_Pictures[];
/// Coatlicue: the bubbles' pictures, the bursts of bubbles, the ring's,
/// flashes' and glows' frames, and the coils.
extern u8 gCoatlicue_PictureWidths[];
extern u8 gCoatlicue_PictureHeights[];
extern u16 gCoatlicue_Pictures[];
extern u16 gCoatlicue_BurstFrames[];
extern u8 gCoatlicue_BurstX[];
extern u8 gCoatlicue_BurstY[];
extern u8 gCoatlicue_BurstSpreadX[];
extern u8 gCoatlicue_BurstSpacingY[];
extern s8 gCoatlicue_BurstVelocityX[];
extern s8 gCoatlicue_BurstVelocityY[];
extern s8 gCoatlicue_BurstZ[];
extern s8 gCoatlicue_BurstVelocityZ[];
extern u8 gCoatlicue_PictureBias[];
extern u16 gCoatlicue_RingFrames[];
extern u16 gCoatlicue_FlashFrames[];
extern u16 gCoatlicue_GlowFrames[];
extern u8 gCoatlicue_CoilStarts[];
extern u16 gCoatlicue_CoilScales[];
extern u16 gCoatlicue_CoilRolls[];
extern u16 gCoatlicue_CoilGrowth[];
extern u16 gCoatlicue_CoilMaxScales[];
/// Catastrophe: the flashes, the demon's, armour's and mirrored demon's
/// sprites, the bursts of lightning, the strikes and the glints.
extern const u16 gCatastrophe_FlashFrames[];
extern const u16 gCatastrophe_FlashLengths[];
extern const u8 gCatastrophe_DemonX[];
extern const u8 gCatastrophe_DemonZ[];
extern const u8 gCatastrophe_ArmourX[];
extern const u16 gCatastrophe_ArmourZ[];
extern const u8 gCatastrophe_MirroredDemonX[];
extern const u8 gCatastrophe_MirroredDemonZ[];
extern const u16 gCatastrophe_ForkStarts[];
extern const u8 gCatastrophe_ForkX[];
extern const u8 gCatastrophe_ForkY[];
extern const u8 gCatastrophe_ForkPictures[];
extern const u16 gCatastrophe_CanvasFlashFrames[];
extern const u8 gCatastrophe_StrikeX[];
extern const u8 gCatastrophe_StrikeY[];
extern const u16 gCatastrophe_StrikeFrames[];
extern const u8 gCatastrophe_StrikeSpeedsY[];
extern const s16 gCatastrophe_StrikeRolls[];
extern const s16 gCatastrophe_StrikeTurns[];
extern const s16 gCatastrophe_StrikePictures[];
extern const u8 gCatastrophe_GlintFlips[];
extern const s16 gCatastrophe_BoltRolls[];
/// Haures: the particles, his scale, the wings' shapes and morphs, the parts
/// and the bands.
extern const u8 gHaures_ParticleX[];
extern s32 gHaures_UnitScale[];
extern const u8 gHaures_MoundShapes[];
extern u8 gHaures_MoundMorphSteps[];
extern const u8 gHaures_PartPoints[];
extern const u8 gHaures_BandStarts[];
/// Helm Splitter's pictures, and where each is drawn.
extern u8 gHelmSplitter_PictureWidths[];
extern u8 gHelmSplitter_PictureHeights[];
extern u16 gHelmSplitter_Pictures[];
extern s8 gHelmSplitter_PictureBiasX[];
extern s8 gHelmSplitter_PictureBiasY[];
/// How many of Dragon Cloud's embers are sparks, by variant.
extern u8 gDragonCloud_SparkCounts[];
/// The element cards: each card's length and how its sparks move.
extern u8 gElementCards_Frames[];
extern struct SparkMotion gElementCards_SparkMotions[];
/// Triple Chomp's three bites' frames.
extern u8 gTripleChomp_BiteFrames[];
/// The breaths: their lengths, bursts, glows, jaws and the frames they hit.
extern u8 gBreath_Frames[];
extern u8 gBreath_BurstFrames[];
extern u8 gBreath_MaxSizes[];
extern u8 gBreath_RingStarts[];
extern u8 gBreath_RingGrowth[];
extern s8 gBreath_RingX[];
extern u8 gBreath_FlashFrames[];
/// Trident: its rings, each trident's spin, the prong's corners and the glints'
/// flips.
extern const u8 gTrident_RingStarts[];
extern const u8 gTrident_RingGrowth[];
extern const s8 gTrident_RingHeights[];
extern const s32 gTrident_Spins[];
extern const struct ModelVertex gTrident_ProngCorners[];
extern const u8 gTrident_GlintFlips[];
/// The leaping strike's waves and rings.
extern const u8 gLeapingStrike_WaveStarts[];
extern const u8 gLeapingStrike_WaveGrowth[];
extern const s8 gLeapingStrike_WaveHeights[];
extern const u8 gLeapingStrike_RingStarts[];
extern const u8 gLeapingStrike_RingGrowth[];
extern const s8 gLeapingStrike_RingHeights[];
/// Meteor Blow: its pictures, waves and rings.
extern const u8 gMeteorBlow_PictureWidths[];
extern const u8 gMeteorBlow_PictureHeights[];
extern const u16 gMeteorBlow_Pictures[];
extern const u8 gMeteorBlow_WaveStarts[];
extern const u8 gMeteorBlow_WaveGrowth[];
extern const u8 gMeteorBlow_WaveStretch[];
extern const u8 gMeteorBlow_RingStarts[];
extern const u8 gMeteorBlow_RingGrowth[];
extern const s8 gMeteorBlow_RingHeights[];
/// Liquifier's splash: its corners, start frames and heights.
extern const struct ModelVertex gLiquifier_SplashCorners[];
extern const u8 gLiquifier_SplashStarts[];
extern const u8 gLiquifier_SplashHeights[];
/// The canvas burst's flame picture for each three frames of its life.
extern const u8 gCanvasBurst_FlamePictures[];
/// Megiddo: its flashes' turns and starts, and its rays' turns and places.
extern const s16 gMegiddo_FlashPitches[];
extern const s16 gMegiddo_FlashYaws[];
extern const u16 gMegiddo_FlashStarts[];
extern const s16 gMegiddo_RayPitches[];
extern const s16 gMegiddo_RayYaws[];
extern const s16 gMegiddo_RayRolls[];
extern const s8 gMegiddo_RayX[];
extern const s8 gMegiddo_RayY[];
/// Odyssey's flashes: their pitch and yaw.
extern const s16 gOdyssey_FlashPitches[];
extern const s16 gOdyssey_FlashYaws[];
/// Moloch's rings: when each starts, how fast it grows, and its height.
extern const u8 gMoloch_RingStarts[4];
extern const u8 gMoloch_RingGrowth[];
extern const s8 gMoloch_RingHeights[];
/// Iris: the splashes, the pillar and particle models, the sound frames, the
/// streaks, the drops, the strips and the cone.
extern s32 gIris_SplashRadii[];
extern const u8 gIris_SplashOffsetsY[];
extern const u16 gIris_PillarMesh[];
extern const struct ModelVertex gIris_ParticleCorners[];
extern const u16 gIris_ParticleMesh[];
extern const u8 gIris_PsynergySoundFrames[];
extern const u16 gIris_DoorSoundFrames[];
extern const u16 gIris_PowerUpSoundFrames[];
extern const s8 gIris_StreakX[];
extern const u8 gIris_StreakY[];
extern const u16 gIris_StreakAngles[];
extern const s16 gIris_StreakSpeeds[];
extern const u8 gIris_LineShades[];
extern s32 gIris_MoteSizes[];
extern const u16 gIris_SplashStarts[];
extern const u16 gIris_SplashEnds[];
extern const u8 gIris_SplashX[];
extern const u8 gIris_SplashY[];
extern const u8 gIris_SplashSmall[];
extern const u8 gIris_DropSpeeds[];
extern const u8 gIris_DropFrames[];
extern const u8 gIris_DropPictureSteps[];
extern const struct ModelVertex gIris_LeftStripCorners[];
extern const struct ModelVertex gIris_RightStripCorners[];
extern const u16 gIris_StripMesh[];
extern const struct ModelVertex gIris_ConeCorners[];
extern const u16 gIris_ConeMesh[];

// The buffers and tables the parts share, by the first part that reads them.

// falling.c
/// The map cell buffer at its own address, which the effects borrow: no map is
/// loaded during a battle.
extern u8 gMapGridCellBuffer[];

// thor.c
extern const s16 gThor_Points[6][2]; ///< the three lines' end points

// flurry.c
void BattleEffect_RunFlurry(struct BattleEffectParams *args, s32 mode);
/// BattleEffect_RunFlurry's modes, by the abilities that play them: the rows
/// of gFlurry_Styles. Gust and Flying Attack take one, two or three strikes as
/// a double or triple blow raises their variant.
enum FlurryMode {
  FLURRY_BEAR_CLAW,            ///< five strikes; also Ransack
  FLURRY_BACTERIA_RUSH,        ///< seven strikes, in lime
  FLURRY_FREEBITE_RUSH,        ///< nine strikes, in blue
  FLURRY_FLYING_ATTACK,        ///< one strike, in embers
  FLURRY_FLYING_ATTACK_DOUBLE, ///< two
  FLURRY_FLYING_ATTACK_TRIPLE, ///< three
  FLURRY_GUST,                 ///< one strike, each filling the canvas
  FLURRY_GUST_DOUBLE,          ///< two
  FLURRY_GUST_TRIPLE,          ///< three
  FLURRY_UR_FLASH,             ///< three strikes, in the Mars Djinni's colours
  FLURRY_CRUSHER_GRIP,         ///< one strike, with glints
  FLURRY_VANISH_CLAW,          ///< one strike, with a ripple and smoke
  FLURRY_CLAW_ATTACK           ///< one strike after a longer lunge, in embers
};
/// A flurry's arcs, its FLURRY_ARC_SHEET.
enum FlurryArcs {
  FLURRY_ARCS_AND_WAVES, ///< blue arc A, with a wave for the third and
                         ///< fourth of every four strikes
  FLURRY_ARCS            ///< blue arc B for every strike
};
/// Whose palette a flurry takes, its FLURRY_PALETTE.
enum FlurryPalette {
  FLURRY_PALETTE_MARS_DJINNI, ///< the Mars Djinni sprite sheet's
  FLURRY_PALETTE_LIME,
  FLURRY_PALETTE_BLUE_ARC,
  FLURRY_PALETTE_EMBER ///< the ember streak sprite sheet's
};
/// What else a flurry plays around its target, its FLURRY_EXTRA.
enum FlurryExtra {
  FLURRY_EXTRA_NONE,
  FLURRY_EXTRA_GLINTS, ///< three glints a frame until frame 31
  FLURRY_EXTRA_RIPPLE  ///< BG2 swings on a sine wave and smoke rises
};

/// The scale each of Gaia's rocks starts from before it grows.
extern const struct Scale gGaia_UnitScale;

// spider_web.c
/// A scale of one in 16.16 on both axes.
extern const struct SpriteScale gUlysses_ParticleScale;

// neptune.c
extern const struct Scale gNeptune_UnitScale; ///< the serpent's scale

// nirvana.c
extern u8 gSerac_MeshEnd[]; ///< the mesh's end, after 112 faces

extern const s8 gVoidBeam_Corners[][2]; ///< the hexagon's corners (z, y)

// flora.c
/// Flora's scale, one to one.
extern const struct Scale gFlora_UnitScale;
/// Inside gEffectScratch: FloraState from its second tile number on, read at
/// its fixed place.
extern u16 gFlora_PetalTiles[];
/// Inside gEffectScratch: FloraState.palette, the object palette as it fades.
extern u16 gFlora_SpritePalettes[];

// daedalus.c
/// The scales of the forest canopy Daedalus flies over, of his machine's
/// pieces and of his missiles.
extern const struct Scale gDaedalus_CanopyScale;
extern const struct Scale gDaedalus_MachineScale;
extern const struct Scale gDaedalus_MissileScale;

/// Coatlicue's and Catastrophe's figures' scales.
extern const struct Scale gCoatlicue_UnitScale;
extern const struct Scale gCatastrophe_UnitScale;

// haures.c
/// In the map cell buffer: the mound's faces, eight bytes a quad.
extern u8 gHaures_MoundStrip[];
/// The face after the mound's last, which ends the strip.
extern u8 gHaures_MoundStripEnd[];
/// In the map cell buffer: each mound corner's shade.
extern u8 gHaures_MoundShades[];
/// Haures's scales as he charges, as he circles and as he opens.
extern const struct Scale gHaures_ChargeScale;
extern const struct Scale gHaures_OrbitScale;
extern const struct Scale gHaures_OpeningScale;

// moloch.c
/// Moloch's motes' sizes, by a mote's number's low three bits.
extern const u8 gMoloch_MoteSizes[];
/// In the map cell buffer: the tiles Iris draws its sprites from.
extern u8 gIris_TileBuffer[];
/// Iris's scale while she rises.
extern const struct Scale gIris_UnitScale;
/// The template BattleEffect_BuildBlitter walks word by word to write a blitter
/// (patch_template.s); none of it runs where it is.
extern const u32 gBlitterTemplate[];

// The pixel fragments BattleEffect_BuildBlitter copies into its blitters, from
// blitter_fragments.s: Forward reads the picture left to right and Reverse
// right to left. None of them runs where it is.
/// Sets 0x40 in every byte of sl, which caps an additive word's pixels at the
/// brightest canvas level.
extern const u32 BattleEffect_Mask[];
/// The ARM immediate operand for 1 << n, n from 0 to 15 (blitter_operands.c).
extern const u16 gBlitter_ImmediatePowers[];
/// Copy, one pixel, right to left.
extern const u32 BattleEffect_CopyReverse[];
/// Copy, one pixel, left to right.
extern const u32 BattleEffect_CopyForward[];
/// Copy, a full cell's eight pixels, right to left.
extern const u32 BattleEffect_Copy8Reverse[];
/// Copy, a full cell's eight pixels, left to right.
extern const u32 BattleEffect_Copy8Forward[];
/// Transparent, one pixel, right to left.
extern const u32 BattleEffect_TransparentReverse[];
/// Transparent, one pixel, left to right.
extern const u32 BattleEffect_TransparentForward[];
/// Transparent, a full cell's eight pixels, right to left.
extern const u32 BattleEffect_Transparent8Reverse[];
/// Transparent, a full cell's eight pixels, left to right.
extern const u32 BattleEffect_Transparent8Forward[];
/// Max, one pixel, right to left.
extern const u32 BattleEffect_MaxReverse[];
/// Max, one pixel, left to right.
extern const u32 BattleEffect_MaxForward[];
/// Max, four pixels (one canvas word), right to left; a full cell's row takes
/// it twice.
extern const u32 BattleEffect_Max4Reverse[];
/// Max, four pixels (one canvas word), left to right; a full cell's row takes
/// it twice.
extern const u32 BattleEffect_Max4Forward[];
/// Additive, one pixel, right to left.
extern const u32 BattleEffect_AdditiveReverse[];
/// Additive, one pixel, left to right.
extern const u32 BattleEffect_AdditiveForward[];
/// Additive, four pixels added a word at a time, right to left; a full cell's
/// row takes it twice.
extern const u32 BattleEffect_ColorReverse[];
/// Additive, four pixels added a word at a time, left to right; a full cell's
/// row takes it twice.
extern const u32 BattleEffect_ColorForward[];

// The model renderer's ARM routines, which run from IWRAM, each with its size
// in bytes and, for three of them, in DMA words.
/// An ARM build of Render_FillMesh, which Render_RunFillMeshKernel runs.
extern const u8 Render_FillMeshKernel[];
extern u8 Render_FillMeshKernelCodeSize[]; ///< Render_FillMeshKernel's bytes
/// The vertex transform Render_TransformVertices runs.
extern const u8 Render_TransformKernel[];
extern u8 Render_TransformKernelCodeSize[]; ///< Render_TransformKernel's bytes

// The rasterizer kernels Render_DrawModelList copies into IWRAM, one for each
// ModelKind, with their sizes in bytes and, for three of them, in DMA words.
extern const u8 Render_SolidKernel[];            ///< MODEL_SOLID
extern const u8 Render_SolidMaxKernel[];         ///< MODEL_SOLID_MAX
extern const u8 Render_SolidAdditiveKernel[];    ///< MODEL_SOLID_ADDITIVE
extern const u8 Render_TexturedKernel[];         ///< MODEL_TEXTURED
extern const u8 Render_TexturedMaxKernel[];      ///< MODEL_TEXTURED_MAX
extern const u8 Render_TexturedAdditiveKernel[]; ///< MODEL_TEXTURED_ADDITIVE
extern const u8 Render_ShadedMaxKernel[];        ///< MODEL_SHADED_MAX
extern const u8 Render_ShadedAdditiveKernel[];   ///< MODEL_SHADED_ADDITIVE
extern const u8 Render_ShadedKernel[];           ///< MODEL_SHADED
extern const u8 Render_TexturedNarrowKernel[];   ///< MODEL_TEXTURED_NARROW
extern const u8 Render_SolidNarrowKernel[];      ///< MODEL_SOLID_NARROW
extern u8 Render_SolidKernelCodeSize[];          ///< Render_SolidKernel's bytes
extern u8 Render_SolidMaxKernelCodeSize[]; ///< Render_SolidMaxKernel's bytes
extern u8
    Render_SolidAdditiveKernelCodeSize[];  ///< Render_SolidAdditiveKernel's
                                           ///< bytes
extern u8 Render_TexturedKernelCodeSize[]; ///< Render_TexturedKernel's bytes
extern u8
    Render_TexturedMaxKernelCodeSize[]; ///< Render_TexturedMaxKernel's bytes
extern u8 Render_TexturedAdditiveKernelCodeSize
    [];                                     ///< Render_TexturedAdditiveKernel's
                                            ///< bytes
extern u8 Render_ShadedMaxKernelCodeSize[]; ///< Render_ShadedMaxKernel's bytes
extern u8
    Render_ShadedAdditiveKernelCodeSize[]; ///< Render_ShadedAdditiveKernel's
                                           ///< bytes
extern u8 Render_ShadedKernelCodeSize[];   ///< Render_ShadedKernel's bytes
extern u8
    Render_TexturedNarrowKernelCodeSize[]; ///< Render_TexturedNarrowKernel's
                                           ///< bytes
extern u8
    Render_SolidNarrowKernelCodeSize[]; ///< Render_SolidNarrowKernel's bytes
extern u8 Render_TexturedKernelCodeWords[]; ///< Render_TexturedKernel's words
extern u8 Render_ShadedKernelCodeWords[];   ///< Render_ShadedKernel's words
extern u8
    Render_TexturedNarrowKernelCodeWords[]; ///< Render_TexturedNarrowKernel's
                                            ///< words

#endif // ALCHEMY_BATTLE_EFFECT_INTERNAL_H
