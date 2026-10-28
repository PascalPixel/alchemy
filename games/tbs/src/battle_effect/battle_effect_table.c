#include "module/battle_effect/battle_effect_table.h"
#include "module/battle_effect/battle_effect.h"
#include "types.h"
#include "vector3.h"
#include "render_placement.h"
#include "fixed_math.h"

// Cell layouts, timing rows and handler entries for the battle effects.
// Keep the definitions in link order. Picture cell offsets are byte offsets
// from the sheet start, accumulated as width times height. Each table is
// read only by the part its prefix names, except the gBattleEffect_ tables,
// which many parts share.

// The tables are plain arrays, not const, as the effect code reads them:
// it reloads their addresses where const would let the compiler keep
// them. Their .data is linked into the effect bank in ROM.

struct BattleEffectParams;

// Picture layouts that several effects read from the same sheet. The ROM
// holds a copy of each table for every effect, so each copy below is
// defined from one of these.

/// The six cells of an erupting flame puff: where each begins in the sheet.
#define FLAME_PUFF_OFFSETS {0, 552, 1480, 2590, 3913, 5185}
#define FLAME_PUFF_CELL_WIDTHS {24, 29, 30, 27, 24, 16}  ///< widths
#define FLAME_PUFF_CELL_HEIGHTS {23, 32, 37, 49, 53, 52} ///< heights
/// How far below its anchor each cell's top goes.
#define FLAME_PUFF_CELL_BIAS_Y {41, 32, 26, 11, 4, 2}
/// The sides of the seven square pictures of a blast, a puff or a flash.
#define BLAST_CELL_SIZES {16, 32, 48, 48, 48, 48, 48}
/// Where each blast picture begins.
#define BLAST_OFFSETS {0, 256, 1280, 3584, 5888, 8192, 10496}
#define EARTH_WALL_CELL_WIDTHS {17, 19, 20}  ///< the earth wall's three widths
#define EARTH_WALL_CELL_HEIGHTS {49, 55, 64} ///< and heights
/// Where each earth wall picture begins.
#define EARTH_WALL_OFFSETS {2702, 3535, 4580}
#define SLASH_CELL_WIDTHS {38, 54, 71, 74, 77, 65} ///< the six slash widths
#define SLASH_CELL_HEIGHTS {7, 9, 11, 13, 13, 11}  ///< and heights
/// Each slash picture's reach from the target.
#define SLASH_CELL_REACH {58, 42, 25, 13, 5, 0}
/// The flash picture for each three frames at the end of a flash's life.
#define FLASH_FADE_OFFSETS {5, 5, 4, 3, 2, 1}
/// The flips a glint may be drawn in: none, and mirrored each way and both.
#define GLINT_FLIPS {0, COMPOSITE_FLIP_X, COMPOSITE_FLIP_Y, COMPOSITE_FLIP}

// Shared cell layouts, read by many parts.

/// Mote cells: particle sheets hold ten, cell n (from 0) n + 1 pixels wide
/// and twice that tall. EFFECT_DRAW_MOTE and many parts read it.
/// It stays the first table: BattleEffect_BuildBlitter
/// (battle_effect/blitter.c) checks that a walk of the blitter template ended
/// at its address.
u16 gBattleEffect_MoteCells[] = {0, 2, 10, 28, 60, 110, 182, 280, 408, 570};
/// Flare cells: the first ten are squares 2x2 to 20x20, cell n (from 0) n + 1
/// pixels from its centre to each edge (EFFECT_DRAW_FLARE, and Neptune and
/// Bind directly). The second ten begin at twice those offsets.
u16 gBattleEffect_FlareCells[] = {0,   4,   20,   56,   120,  220, 364,
                                  560, 816, 1140, 0,    8,    40,  112,
                                  240, 440, 728,  1120, 1632, 2280};
/// Nine square puff cells, read by the falling, frost, healing, Djinni heal
/// and Boreas parts.
u16 gBattleEffect_PuffCells[] = {0, 25, 74, 123, 172, 221, 246, 255, 256};
/// Each puff cell's side.
u8 gBattleEffect_PuffSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};
// The six cells of an erupting flame puff: width, height, how far below its
// anchor its top goes (so that all six stand on about the same line) and
// where each begins in the sheet. Each is drawn centred across x; the Mars
// Djinni shows each for 8 frames, centred on the flame's x. Read by the
// Djinn attacks, Flare, the casting impacts, Atalanta and the dragon cloud.
u8 gBattleEffect_PuffArcCellWidths[] = FLAME_PUFF_CELL_WIDTHS;
u8 gBattleEffect_PuffArcCellHeights[] = FLAME_PUFF_CELL_HEIGHTS;
u8 gBattleEffect_PuffArcCellBiasY[] = FLAME_PUFF_CELL_BIAS_Y;
u16 gBattleEffect_PuffArcCells[] = FLAME_PUFF_OFFSETS;
// Six glint cells, each GlintCellWidths by GlintCellHeights pixels: read by
// the target strikes and motes, Bolt, the vortices, the casting impacts and
// Thor.
u16 gBattleEffect_GlintCells[] = {0, 589, 1302, 2015, 2414, 2750};
u8 gBattleEffect_GlintCellWidths[] = {19, 23, 23, 19, 16, 17};
u8 gBattleEffect_GlintCellHeights[] = {31, 31, 31, 21, 21, 24};

// falling.c: Falling shard and bolt wave counts.

/// Ice, by variant: shards and frames (ICE_SHARD_COUNT, ICE_FRAMES).
u8 gFalling_IceCounts[][2] = {
    // shards, frames
    [EFFECT_VARIANT_BASIC] = {16, 46},     // Ice
    [EFFECT_VARIANT_STRONGER] = {20, 52},  // Ice Horn
    [EFFECT_VARIANT_STRONGEST] = {32, 80}, // Ice Missile
};
/// Douse, by variant: the FALLING_BOLT_ columns.
u8 gFalling_Counts[][4] = {
    // bolts, sparks per bolt, spacing, frames
    [EFFECT_VARIANT_BASIC] = {6, 8, 32, 80},     // Douse
    [EFFECT_VARIANT_STRONGER] = {8, 16, 24, 80}, // Drench
    [EFFECT_VARIANT_STRONGEST] = {32, 4, 8, 80}, // Deluge
};

// streaks.c: Falling streak timing and motion rows.

/// Each streak style's effect, hit, motion and shake, by the STREAKS_
/// columns (streaks.c).
s8 gStreaks_Styles[][4] = {
    // STREAKS_ANIMATE, STREAKS_HIT, STREAKS_MOTION, STREAKS_SHAKE
    {0, 1, 0, 0},  // STREAKS_STYLE_EMBER
    {0, 1, 4, 0},  // STREAKS_STYLE_LIME
    {0, 1, -1, 1}, // STREAKS_STYLE_VIOLET
    {1, 1, -1, 1}, // STREAKS_STYLE_RED
    {1, 1, 4, 0},  // STREAKS_STYLE_CYAN
};
/// Pixels the target trembles by, frame by frame.
s8 gStreaks_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

// target_strike.c: Target strike timing, motion and picture cells.

/// Each of the twelve target strike styles, by the TARGET_STRIKE_ columns
/// (target_strike.c).
// clang-format off
u8 gTargetStrike_Styles[][7] = {
    // TARGET_STRIKE_: KIND, HITS, FLAGS, COLOUR, IMAGE_FRAMES, COMPOSITE,
    // FRAMES
    [TARGET_STRIKE_STYLE_BLUE_SPARK_COLUMNS] =
        {STRIKE_KIND_COLUMN, 2, STRIKE_SPARKS, STRIKE_COLOR_BLUE, 6, COMPOSITE_ADDITIVE, 70},
    [TARGET_STRIKE_STYLE_LIME_SMOKE_COLUMN] =
        {STRIKE_KIND_COLUMN, 1, STRIKE_SMOKE, STRIKE_COLOR_LIME, 6, COMPOSITE_LIGHTEN, 54},
    [TARGET_STRIKE_STYLE_PINK_SMOKE_COLUMNS] =
        {STRIKE_KIND_COLUMN, 3, STRIKE_SMOKE, STRIKE_COLOR_PINK, 6, COMPOSITE_LIGHTEN, 96},
    [TARGET_STRIKE_STYLE_EMBER_DRAIN_COLUMNS] =
        {STRIKE_KIND_COLUMN, 3, STRIKE_ABSORB, STRIKE_COLOR_EMBER, 4, COMPOSITE_LIGHTEN, 96},
    [TARGET_STRIKE_STYLE_MARS_DRAIN_COLUMNS] =
        {STRIKE_KIND_COLUMN, 3, STRIKE_ABSORB, STRIKE_COLOR_MARS, 4, COMPOSITE_LIGHTEN, 96},
    [TARGET_STRIKE_STYLE_LIGHTNING_GLINT_COLUMNS] =
        {STRIKE_KIND_COLUMN, 3, STRIKE_GLINTS, STRIKE_COLOR_LIGHTNING, 4, COMPOSITE_ADDITIVE, 96},
    [TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH] =
        {STRIKE_KIND_SLASH, 1, STRIKE_GLINTS, STRIKE_COLOR_LIGHTNING, 4, COMPOSITE_ADDITIVE, 38},
    [TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES] =
        {STRIKE_KIND_SLASH, 2, STRIKE_SPARKS, STRIKE_COLOR_LIGHTNING, 2, COMPOSITE_ADDITIVE, 48},
    [TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH] =
        {STRIKE_KIND_SLASH, 1, STRIKE_SPARKS | STRIKE_HEAVY, STRIKE_COLOR_MARS, 2,
         COMPOSITE_ADDITIVE, 50},
    [TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH] =
        {STRIKE_KIND_SLASH, 1, STRIKE_SMOKE, STRIKE_COLOR_LIME, 2, COMPOSITE_ADDITIVE, 46},
    [TARGET_STRIKE_STYLE_MARS_SMOKE_COLUMN] =
        {STRIKE_KIND_COLUMN, 1, STRIKE_SMOKE, STRIKE_COLOR_MARS, 6, COMPOSITE_LIGHTEN, 54},
    [TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_COLUMNS] =
        {STRIKE_KIND_COLUMN, 3, STRIKE_ABSORB, STRIKE_COLOR_MARS, 4, COMPOSITE_ADDITIVE, 96},
};
// clang-format on
// The six slash pictures: size, sheet offset and reach from the target.
u8 gTargetStrike_Widths[] = SLASH_CELL_WIDTHS;
u8 gTargetStrike_Heights[] = SLASH_CELL_HEIGHTS;
u16 gTargetStrike_Offsets[] = {3200, 3466, 3952, 4733, 5695, 6696};
u8 gTargetStrike_Reach[] = SLASH_CELL_REACH;
/// The column stage shown by each picture of a rise, four pictures to a hit:
/// the first hit grows through stages 0 to 3, and each later hit reads
/// entries 4, 1, 2 and 3 (pictures above 4 wrap back by 4), sinking to 2 and
/// 1 and growing back to 3. The hit lands on stage 3.
u8 gTargetStrike_ColumnStages[] = {0, 1, 2, 3, 2};
/// The flips a glint may be drawn in.
u8 gTargetStrike_GlintDrawFlags[] = GLINT_FLIPS;

// death_plunge.c: Blade rain and shattering tile pieces.

// The sword pictures, by how steeply the sword falls.
u8 gDeathPlunge_CellWidths[] = {20, 21, 22, 25};
u8 gDeathPlunge_CellHeights[] = {32, 31, 27, 24};
u16 gDeathPlunge_Cells[] = {0, 640, 1291, 1885};
/// The 33 pieces of the TileAnimationMask picture: where each starts on the
/// canvas, its size, and where its pixels begin in the decoded picture. The
/// offsets carry one more entry, where the last piece ends.
u8 gDeathPlunge_PieceWidths[] = {21, 20, 24, 30, 35, 69, 19, 23, 9,  22, 33,
                                 10, 18, 9,  7,  9,  17, 55, 30, 25, 40, 29,
                                 10, 28, 14, 5,  32, 23, 19, 32, 8,  43, 15};
u8 gDeathPlunge_PieceHeights[] = {23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35,
                                  17, 18, 14, 15, 12, 13, 37, 38, 41, 60, 41,
                                  15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11};
u16 gDeathPlunge_PieceCells[] = {
    0,     483,   1423,  2023,  3223,  3923,  7580,  8112,  8779,
    8986,  9294,  10449, 10619, 10943, 11069, 11174, 11282, 11503,
    13538, 14678, 15703, 18103, 19292, 19442, 20702, 21528, 21733,
    22565, 23715, 24114, 25234, 25330, 27781, 27946};
u8 gDeathPlunge_PieceStartX[] = {0,  21, 28, 46, 70, 51, 0,  13, 37, 77, 87,
                                 19, 25, 35, 44, 45, 48, 65, 0,  0,  0,  5,
                                 33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};
u8 gDeathPlunge_PieceStartY[] = {0,  0,  0,  0,  0,  0,  19, 17, 23,  28, 8,
                                 36, 43, 43, 40, 47, 48, 37, 42, 67,  60, 79,
                                 62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

// death_plunge.c and spider_web.c: Canvas corner sparks and two-resource
// picture cells.

// The element burst's corner sparks: offsets and flips.
s8 gDeathPlunge_X[] = {0, 32, 0, 32};
s8 gDeathPlunge_Y[] = {0, 0, 32, 32};
u8 gDeathPlunge_DrawFlags[] = GLINT_FLIPS;
// The six pictures: the first four from the swirl sheet, the last two from the
// tail sheet. gSpiderWeb_CellX has six entries for each side.
u8 gSpiderWeb_CellWidths[] = {40, 112, 112, 112, 88, 64};
u8 gSpiderWeb_CellHeights[] = {56, 80, 64, 64, 80, 88};
u16 gSpiderWeb_Cells[] = {0, 2240, 11200, 18368, 0, 7040};
u8 gSpiderWeb_CellX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};
s8 gSpiderWeb_CellBiasY[] = {0, 4, 24, -6, -20, -28, 0, 0};

// gathering_lights.c

/// Gathering lights, by mode (enum GatheringLightsMode): the lights per
/// target and the frames before the targets' turns.
u8 gGatheringLights_Counts[][2] = {
    // lights per target, frames
    [GATHERING_LIGHTS_DEMON_NIGHT] = {16, 88},
    [GATHERING_LIGHTS_RUMBLE] = {8, 72},
    [GATHERING_LIGHTS_HAUNTING] = {12, 80},
};

// mirrored_wall.c: Mirrored earth wall picture cells.

// The earth wall's three images: the mirrored wall reads them at the start of
// its sheet, the rising wall after its rebuilt rows. Both share the sizes.
u16 gMirroredWall_Cells[] = EARTH_WALL_OFFSETS;
u8 gMirroredWall_CellWidths[] = EARTH_WALL_CELL_WIDTHS;
u8 gMirroredWall_CellHeights[] = EARTH_WALL_CELL_HEIGHTS;

// djinn_beams.c: Djinni beam motes and heads.

/// The picture of each step of a mote's eight-step cycle.
u8 gDjinnBeams_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};
// The three cells of the glow at a target's feet.
u16 gDjinnBeams_HeadOffsets[] = EARTH_WALL_OFFSETS;
u8 gDjinnBeams_HeadWidths[] = EARTH_WALL_CELL_WIDTHS;
u8 gDjinnBeams_HeadHeights[] = EARTH_WALL_CELL_HEIGHTS;

// lobbed_shells.c: Shell explosion rows and shards.

/// Two bytes for each lobbed shell style (enum LobbedShellsMode,
/// lobbed_shells.c).
s8 gLobbedShells_Styles[] = {
    // nonzero: the shells burst into shards, not smoke; unused
    0, 0, // LOBBED_SHELLS_BONE_CHILLER
    0, 0, // LOBBED_SHELLS_STICKY_GOO
    0, 0, // LOBBED_SHELLS_STICKY_POISON
    1, 0, // LOBBED_SHELLS_ROTTEN_BLOOD
    1, 0, // LOBBED_SHELLS_MAD_SPATTER
    0, 0, // LOBBED_SHELLS_DECOMPOSE
    0, 0, // LOBBED_SHELLS_POISON_INK
};
/// Lobbed shells: nine tumbling images in two sets, the second (cells 9 to 17)
/// smaller.
u8 gLobbedShells_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12,
                                  6, 7,  7, 7, 6, 6,  8,  8,  8};
u8 gLobbedShells_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12,
                                   8,  7,  6,  6,  8,  6,  4,  3,  3};
u16 gLobbedShells_ShardOffsets[] = {0,    108,  228,  336,  444,  552,
                                    684,  816,  948,  1092, 1140, 1189,
                                    1231, 1273, 1321, 1357, 1389, 1413};

// condemn.c: Shattered panel coordinates.

/// Where each stage of the panel is drawn: seven x positions for each side of
/// the field, and seven y positions (above the swing) for both sides.
u8 gCondemn_PanelX[][7] = {
    {10, 30, 0, 0, 0, 6, 52},
    {60, 0, 0, 0, 0, 0, 0},
};
u8 gCondemn_PanelY[][7] = {
    {0, 20, 2, 2, 34, 64, 68},
    {0, 9, 4, 4, 4, 4, 4},
};

// bind.c: Spinning and striking triangle vertices.

/// The spinning triangle's first vertex, 24 up from its centre.
struct Vector3 gBind_Vertex[] = {{.x = 0, .y = 24, .z = 0}};
/// The striking triangle's first vertex, the same.
struct Vector3 gBind_TriangleStrikeVertex[] = {{.x = 0, .y = 24, .z = 0}};

// stars.c and procne.c: Ring bolt, spinning star and whirlwind comet paths.

/// The two star points, 48 and 28 pixels up (16.16).
struct Vector3 gStar_Points[] = {{.x = 0, .y = 48 << 16, .z = 0},
                                 {.x = 0, .y = 28 << 16, .z = 0}};
u8 gStar_Radii[] = {32, 16};
u8 gProcne_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};
u8 gProcne_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};
u16 gProcne_Frames[] = {334, 347, 359, 372};
u8 gProcne_CellWidths[] = {14, 24, 48};
u8 gProcne_CellHeights[] = {14, 19, 24};
u8 gProcne_CellBiasY[] = {8, 12, 17};
u16 gProcne_Cells[] = {13824, 14020, 14476};

// tiamat.c and nereid.c: Palette particle and ember column layouts.

/// Ember speeds, by ember index & 3: added to a random 0 to 127, in 128ths
/// of a pixel a frame across and 32nds up.
u8 gTiamat_DropSpeeds[] = {0, 8, 12, 16};
/// A spark's 1x2 picture, drawn additively, so a spark adds 32 of the ramp's
/// 63 levels to what it covers.
u8 gTiamat_SparkPixels[] = {32, 32};
/// Ember sizes, by ember index & 3: mote cells 1x2, 1x2, 2x4 and 3x6.
u8 gTiamat_DropSizes[] = {1, 1, 2, 3};
// The six streak pictures: size, drop below the streak and sheet offset.
u8 gTiamat_StreakWidths[] = FLAME_PUFF_CELL_WIDTHS;
u8 gTiamat_StreakHeights[] = FLAME_PUFF_CELL_HEIGHTS;
u8 gTiamat_StreakDrops[] = FLAME_PUFF_CELL_BIAS_Y;
u16 gTiamat_StreakOffsets[] = FLAME_PUFF_OFFSETS;
/// Nereid's four columns of fire (NEREID_COLUMN_X, NEREID_COLUMN_Y).
u8 gNereid_Columns[][2] = {
    // the foot of each column of fire: x, y
    {16, 104},
    {32, 104},
    {48, 104},
    {64, 104},
};
/// Ember gravity while falling, by ember index & 3, in 16.16 pixels a frame
/// each frame: 1/2, 1/4, 1/4 and 1/8.
s32 gNereid_Gravity[] = {Q16_ONE / 2, Q16_ONE / 4, Q16_ONE / 4, Q16_ONE / 8};

// vortices.c: Vortex mote counts and tornado shapes.

/// Froth, by variant: motes and frames.
u8 gVortices_Counts[][2] = {
    // motes (at most FROTH_MOTES, vortices.c), frames
    [EFFECT_VARIANT_BASIC] = {6, 140},      // Froth
    [EFFECT_VARIANT_STRONGER] = {12, 160},  // Froth Sphere
    [EFFECT_VARIANT_STRONGEST] = {30, 200}, // Froth Spiral
};
/// Whirlwind, by variant: the WHIRLWIND_ columns. The sway is in pixels each
/// way; the shake is BattleCamera_Shake's horizontal range, the vertical
/// twice that.
u8 gVortices_Shapes[][3] = {
    // tornadoes, sway, screen shake
    [EFFECT_VARIANT_BASIC] = {1, 16, 2},     // Whirlwind, Gale
    [EFFECT_VARIANT_STRONGER] = {2, 24, 2},  // Tornado, Typhoon
    [EFFECT_VARIANT_STRONGEST] = {4, 32, 4}, // Tempest, Hurricane
};

// prism.c: Crystal counts and picture cells.

/// Where each of the 16 shards starts in the crystal: x, then y.
u8 gPrism_ShardStarts[][2] = {
    {32, 60}, {33, 82}, {49, 94}, {41, 89}, {49, 76}, {32, 99},
    {54, 69}, {34, 82}, {45, 71}, {31, 91}, {32, 69}, {26, 80},
    {40, 68}, {41, 60}, {46, 60}, {36, 63}, {0, 0},
};
/// Prism, by variant: crystals and frames.
u8 gPrism_Counts[][2] = {
    // crystals, frames
    [EFFECT_VARIANT_BASIC] = {1, 110},      // Prism
    [EFFECT_VARIANT_STRONGER] = {4, 134},   // Hail Prism
    [EFFECT_VARIANT_STRONGEST] = {12, 166}, // Freeze Prism
};
// The twelve shard pictures.
u8 gPrism_ShardWidths[] = {5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22};
u8 gPrism_ShardHeights[] = {14, 10, 7, 13, 14, 14, 32, 30, 24, 38, 20, 20};
s32 gPrism_ShardOffsets[] = {0,   70,   170,  247,  429,  597,
                             751, 1359, 1899, 2451, 2907, 3347};

// plasma.c: Lightning pillar sparks, columns and counts.

/// Plasma, by variant: sparks per pillar and shake frames.
u16 gPlasma_Sparks[][2] = {
    // sparks per pillar, shake frames
    [EFFECT_VARIANT_BASIC] = {384, 2},     // Plasma, Thunderclap
    [EFFECT_VARIANT_STRONGER] = {256, 3},  // Shine Plasma, Thunderbolt
    [EFFECT_VARIANT_STRONGEST] = {192, 4}, // Spark Plasma, Thunderstorm
};
/// Each pillar's canvas x, seven for each side.
u8 gPlasma_Columns[][7] = {
    {32, 64, 16, 48, 72, 96, 64},
    {88, 56, 104, 72, 48, 24, 56},
};
/// How many pillars strike, by variant.
u8 gPlasma_Counts[] = {1, 3, 7};

// plasma.c: Spark group shapes and flash cells.

/// The spark groups, by variant: the SPARK_GROUPS_ columns.
u16 gPlasma_Shapes[][5] = {
    // sparks per burst, bursts, each burst's offset across
    [EFFECT_VARIANT_BASIC] = {128, 1, 16, 0, 0},
    [EFFECT_VARIANT_STRONGER] = {255, 1, 16, 0, 0},
    [EFFECT_VARIANT_STRONGEST] = {192, 3, 0, 24, 48},
    [SPARK_GROUPS_SEVERE_BLOW] = {340, 1, 16, 24, 48},
    [SPARK_GROUPS_THRASH] = {32, 1, 16, 0, 0},
};
/// The flash picture for each three frames at the end of a flash's life.
u8 gPlasma_FlashCells[] = FLASH_FADE_OFFSETS;

// volcano.c: Fire pillar counts, depths and start frames.

/// How many pillars stand, by variant.
u8 gVolcano_Counts[] = {1, 2, 4};
/// Each pillar's depth in pixels, by variant.
s8 gVolcano_Depths[][4] = {{0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};
/// The frame each pillar starts. At most four pillars stand, so the fifth
/// entry is never read.
u8 gVolcano_StartFrames[] = {8, 18, 24, 30, 34};

// target_motes.c: Target mote glint orientations.

/// Glint orientations: none, and mirrored each way and both.
u8 gTargetMotes_GlintFlips[] = GLINT_FLIPS;

/// The effect handlers, indexed by effect kind minus one, which
/// BattleEffect_RunFromTable calls; BATTLE_EFFECT_NONE is no effect, for
/// which it only clears args->variant.
///
/// Each handler is named for the first ability, by ability id, whose entry in
/// gAbilityBattleEffects (battle/ability_staging.c) selects its kind, with that
/// ability's English name (MsgAbilityName + id): BATTLE_EFFECT_QUAKE is Quake,
/// Earthquake and Quake Sphere, so BattleEffect_RunQuake. A handler several
/// kinds share takes the name of its lowest kind's first ability, and a name
/// another handler has already taken passes to the next ability
/// (BATTLE_EFFECT_NOVA is BattleEffect_RunNova, as its first ability is a
/// second Blast). Kinds that no ability uses keep descriptive names. Each row
/// gives the kind's name, the part of battle_effect.c that defines its handler
/// and its first ability; the handler's own comment there lists every ability
/// that reaches it.
BattleEffectHandler gBattleEffect_Handlers[BATTLE_EFFECT_COUNT - 1] = {
    [BATTLE_EFFECT_QUAKE - 1] = BattleEffect_RunQuake,     // quake.c: Quake
    [BATTLE_EFFECT_SPIRE - 1] = BattleEffect_RunSpire,     // spire.c: Spire
    [BATTLE_EFFECT_GAIA - 1] = BattleEffect_RunGaia,       // gaia.c: Gaia
    [BATTLE_EFFECT_GROWTH - 1] = BattleEffect_RunGrowth,   // growth.c: Growth
    [BATTLE_EFFECT_THORN - 1] = BattleEffect_RunThorn,     // growth.c: Thorn
    [BATTLE_EFFECT_FROST - 1] = BattleEffect_RunFrost,     // frost.c: Frost
    [BATTLE_EFFECT_ICE - 1] = BattleEffect_RunIce,         // falling.c: Ice
    [BATTLE_EFFECT_PRISM - 1] = BattleEffect_RunPrism,     // prism.c: Prism
    [BATTLE_EFFECT_DOUSE - 1] = BattleEffect_RunDouse,     // falling.c: Douse
    [BATTLE_EFFECT_FROTH - 1] = BattleEffect_RunFroth,     // vortices.c: Froth
    [BATTLE_EFFECT_FLARE - 1] = BattleEffect_RunFlare,     // flare.c: Flare
    [BATTLE_EFFECT_FIRE - 1] = BattleEffect_RunFire,       // fire.c: Fire
    [BATTLE_EFFECT_VOLCANO - 1] = BattleEffect_RunVolcano, // volcano.c: Volcano
    [BATTLE_EFFECT_BLAST - 1] = BattleEffect_RunBlast,     // eruption.c: Blast
    [BATTLE_EFFECT_NOVA - 1] = BattleEffect_RunNova,       // plasma.c: Blast
    [BATTLE_EFFECT_BOLT - 1] = BattleEffect_RunBolt,       // bolt.c: Bolt
    [BATTLE_EFFECT_RAY - 1] = BattleEffect_RunRay,         // ray.c: Ray
    [BATTLE_EFFECT_PLASMA - 1] = BattleEffect_RunPlasma,   // plasma.c: Plasma
    [BATTLE_EFFECT_SLASH - 1] = BattleEffect_RunSlash, // projectiles.c: Slash
    [BATTLE_EFFECT_WHIRLWIND - 1] =
        BattleEffect_RunWhirlwind,                   // vortices.c: Whirlwind
    [BATTLE_EFFECT_CURE - 1] = BattleEffect_RunCure, // healing.c: Cure
    [BATTLE_EFFECT_PLY - 1] = BattleEffect_RunPly,   // healing.c: Ply
    [BATTLE_EFFECT_WISH - 1] = BattleEffect_RunWish, // healing.c: Wish
    [BATTLE_EFFECT_CURE_POISON - 1] =
        BattleEffect_RunCurePoison, // healing.c: Cure Poison
    [BATTLE_EFFECT_REVIVE - 1] =
        BattleEffect_RunRevive, // djinn_beams.c: Revive
    [BATTLE_EFFECT_IMPACT - 1] =
        BattleEffect_RunImpact,                        // stat_change.c: Impact
    [BATTLE_EFFECT_DULL - 1] = BattleEffect_RunDull,   // stat_change.c: Dull
    [BATTLE_EFFECT_GUARD - 1] = BattleEffect_RunGuard, // stat_change.c: Guard
    [BATTLE_EFFECT_IMPAIR - 1] =
        BattleEffect_RunImpair,                      // stat_change.c: Impair
    [BATTLE_EFFECT_WARD - 1] = BattleEffect_RunWard, // stat_change.c: Ward
    [BATTLE_EFFECT_WEAKEN - 1] =
        BattleEffect_RunWeaken,                        // stat_change.c: Weaken
    [BATTLE_EFFECT_TAINT - 1] = BattleEffect_RunTaint, // target_motes.c: Taint
    [BATTLE_EFFECT_DELUDE - 1] =
        BattleEffect_RunDelude, // target_motes.c: Delude
    [BATTLE_EFFECT_UNUSED_ORBITING_SPIRALS - 1] =
        BattleEffect_RunUnusedOrbitingSpirals, // planet_diver.c
    [BATTLE_EFFECT_UNUSED_HEARTRENDER - 1] =
        BattleEffect_RunHeartrender, // target_motes.c: Heartrender
    [BATTLE_EFFECT_PARALYZE - 1] =
        BattleEffect_RunParalyze, // target_motes.c: Paralyze
    [BATTLE_EFFECT_SLEEP - 1] = BattleEffect_RunSleep, // target_motes.c: Sleep
    [BATTLE_EFFECT_BIND - 1] = BattleEffect_RunBind,   // bind.c: Bind
    [BATTLE_EFFECT_HAUNT - 1] = BattleEffect_RunHaunt, // planet_diver.c: Haunt
    [BATTLE_EFFECT_CONDEMN - 1] = BattleEffect_RunCondemn, // condemn.c: Condemn
    [BATTLE_EFFECT_CURSE - 1] = BattleEffect_RunCurse, // target_motes.c: Curse
    [BATTLE_EFFECT_DRAIN - 1] = BattleEffect_RunDrain, // drain.c: Drain
    [BATTLE_EFFECT_BREAK - 1] = BattleEffect_RunBreak, // drain.c: Break
    [BATTLE_EFFECT_REGENERATE - 1] = BattleEffect_RunWish, // healing.c: Wish
    [BATTLE_EFFECT_REFLECT - 1] = BattleEffect_RunWish,    // healing.c: Wish
    [BATTLE_EFFECT_UNUSED_IMPACT - 1] =
        BattleEffect_RunImpact, // stat_change.c: Impact
    [BATTLE_EFFECT_UNUSED_IMPAIR - 1] =
        BattleEffect_RunImpair, // stat_change.c: Impair
    [BATTLE_EFFECT_DRAGON_CLOUD - 1] =
        BattleEffect_RunDragonCloud, // dragon_cloud.c: Dragon Cloud
    [BATTLE_EFFECT_DEMON_NIGHT - 1] =
        BattleEffect_RunDemonNight, // gathering_lights.c: Demon Night
    [BATTLE_EFFECT_HELM_SPLITTER - 1] =
        BattleEffect_RunHelmSplitter, // helm_splitter.c: Helm Splitter
    [BATTLE_EFFECT_QUICK_STRIKE - 1] =
        BattleEffect_RunQuickStrike, // thrash.c: Quick Strike
    [BATTLE_EFFECT_DEATH_PLUNGE - 1] =
        BattleEffect_RunDeathPlunge, // death_plunge.c: Death Plunge
    [BATTLE_EFFECT_SHURIKEN - 1] =
        BattleEffect_RunShuriken, // projectiles.c: Shuriken
    [BATTLE_EFFECT_ANNIHILATION - 1] =
        BattleEffect_RunAnnihilation, // annihilation.c: Annihilation
    [BATTLE_EFFECT_PUNJI - 1] = BattleEffect_RunPunji, // growth.c: Punji
    [BATTLE_EFFECT_CUTTING_EDGE - 1] =
        BattleEffect_RunCuttingEdge, // projectiles.c: Cutting Edge
    [BATTLE_EFFECT_RAGNAROK - 1] =
        BattleEffect_RunRagnarok, // ragnarok.c: Ragnarok
    [BATTLE_EFFECT_HEAT_WAVE - 1] =
        BattleEffect_RunHeatWave, // projectiles.c: Heat Wave
    [BATTLE_EFFECT_PLANET_DIVER - 1] =
        BattleEffect_RunPlanetDiver, // planet_diver.c: Planet Diver
    [BATTLE_EFFECT_TITAN_BLADE - 1] =
        BattleEffect_RunTitanBlade, // titan_blade.c: Titan Blade
    [BATTLE_EFFECT_SHINING_STAR - 1] =
        BattleEffect_RunShiningStar, // stars.c: Shining Star
    [BATTLE_EFFECT_BLIZZARD - 1] =
        BattleEffect_RunBlizzard, // projectiles.c: Blizzard
    [BATTLE_EFFECT_DEMON_FIRE - 1] =
        BattleEffect_RunDemonFire, // target_motes.c: Demon Fire
    [BATTLE_EFFECT_VORPAL_SLASH - 1] =
        BattleEffect_RunVorpalSlash,                   // thrash.c: Vorpal Slash
    [BATTLE_EFFECT_DROWN - 1] = BattleEffect_RunDrown, // target_motes.c: Drown
    [BATTLE_EFFECT_UNUSED_TRIANGLE_STRIKE - 1] =
        BattleEffect_RunUnusedTriangleStrike, // bind.c
    [BATTLE_EFFECT_ASTRAL_BLAST - 1] =
        BattleEffect_RunAstralBlast, // stars.c: Astral Blast
    [BATTLE_EFFECT_GRANITE - 1] =
        BattleEffect_RunGranite, // stat_change.c: Granite
    [BATTLE_EFFECT_QUARTZ - 1] =
        BattleEffect_RunRevive,                      // djinn_beams.c: Revive
    [BATTLE_EFFECT_VINE - 1] = BattleEffect_RunVine, // growth.c: Vine
    [BATTLE_EFFECT_GROUND - 1] = BattleEffect_RunGround, // hail.c: Ground
    [BATTLE_EFFECT_FIZZ - 1] = BattleEffect_RunPly,      // healing.c: Ply
    [BATTLE_EFFECT_SLEET - 1] = BattleEffect_RunSleet,   // projectiles.c: Sleet
    [BATTLE_EFFECT_SPRITZ - 1] = BattleEffect_RunWish,   // healing.c: Wish
    [BATTLE_EFFECT_HAIL - 1] = BattleEffect_RunHail,     // hail.c: Hail
    [BATTLE_EFFECT_TONIC - 1] = BattleEffect_RunTonic,   // healing.c: Tonic
    [BATTLE_EFFECT_DEW - 1] = BattleEffect_RunDew,       // djinn_beams.c: Dew
    [BATTLE_EFFECT_FORGE - 1] = BattleEffect_RunImpact, // stat_change.c: Impact
    [BATTLE_EFFECT_CORONA - 1] = BattleEffect_RunGuard, // stat_change.c: Guard
    [BATTLE_EFFECT_PSY_CRYSTAL - 1] =
        BattleEffect_RunPsyCrystal,                    // healing.c: Psy Crystal
    [BATTLE_EFFECT_FLASH - 1] = BattleEffect_RunFlash, // stat_change.c: Flash
    [BATTLE_EFFECT_TORCH - 1] = BattleEffect_RunTorch, // torch.c: Torch
    [BATTLE_EFFECT_BREEZE - 1] = BattleEffect_RunWard, // stat_change.c: Ward
    [BATTLE_EFFECT_ZEPHYR - 1] =
        BattleEffect_RunZephyr,                        // stat_change.c: Zephyr
    [BATTLE_EFFECT_KITE - 1] = BattleEffect_RunKite,   // torch.c: Kite
    [BATTLE_EFFECT_LUFF - 1] = BattleEffect_RunBind,   // bind.c: Bind
    [BATTLE_EFFECT_GUST - 1] = BattleEffect_RunGust,   // flurry.c: Gust
    [BATTLE_EFFECT_VENUS - 1] = BattleEffect_RunVenus, // djinn_attacks.c: Venus
    [BATTLE_EFFECT_MERCURY - 1] =
        BattleEffect_RunMercury,                     // djinn_attacks.c: Mercury
    [BATTLE_EFFECT_MARS - 1] = BattleEffect_RunMars, // djinn_attacks.c: Mars
    [BATTLE_EFFECT_JUPITER - 1] =
        BattleEffect_RunJupiter, // djinn_attacks.c: Jupiter
    [BATTLE_EFFECT_SONIC_WAVE - 1] =
        BattleEffect_RunSonicWave, // streaks.c: Sonic Wave
    [BATTLE_EFFECT_SHRIEK - 1] = BattleEffect_RunShriek, // streaks.c: Shriek
    [BATTLE_EFFECT_BANSHEE_HOWL - 1] =
        BattleEffect_RunBansheeHowl, // streaks.c: Banshee Howl
    [BATTLE_EFFECT_CRAZY_VOICE - 1] =
        BattleEffect_RunCrazyVoice, // projectiles.c: Crazy Voice
    [BATTLE_EFFECT_WAR_CRY - 1] = BattleEffect_RunWarCry, // streaks.c: War Cry
    [BATTLE_EFFECT_WICKED_HOWL - 1] =
        BattleEffect_RunWickedHowl, // streaks.c: Wicked Howl
    [BATTLE_EFFECT_WING_BEAT - 1] =
        BattleEffect_RunWingBeat, // projectiles.c: Wing Beat
    [BATTLE_EFFECT_WING_FLUTTER - 1] =
        BattleEffect_RunWingFlutter, // projectiles.c: Wing Flutter
    [BATTLE_EFFECT_RUMBLE - 1] =
        BattleEffect_RunRumble, // gathering_lights.c: Rumble
    [BATTLE_EFFECT_BONE_CHILLER - 1] =
        BattleEffect_RunBoneChiller, // lobbed_shells.c: Bone Chiller
    [BATTLE_EFFECT_RAPID_SMASH - 1] = BattleEffect_RunGust, // flurry.c: Gust
    [BATTLE_EFFECT_BONE_CHARGE - 1] =
        BattleEffect_RunBoneCharge, // rushing_strike.c: Bone Charge
    [BATTLE_EFFECT_MYSTIC_FLAME - 1] =
        BattleEffect_RunMysticFlame, // target_motes.c: Mystic Flame
    [BATTLE_EFFECT_NUMBING_STING - 1] =
        BattleEffect_RunNumbingSting, // target_strike.c: Numbing Sting
    [BATTLE_EFFECT_BRUTE_FORCE - 1] =
        BattleEffect_RunBruteForce, // rushing_strike.c: Brute Force
    [BATTLE_EFFECT_STICKY_GOO - 1] =
        BattleEffect_RunStickyGoo, // lobbed_shells.c: Sticky Goo
    [BATTLE_EFFECT_CANNIBAL_FANG - 1] =
        BattleEffect_RunCannibalFang, // target_strike.c: Cannibal Fang
    [BATTLE_EFFECT_BEAR_CLAW - 1] =
        BattleEffect_RunBearClaw, // flurry.c: Bear Claw
    [BATTLE_EFFECT_POISONOUS_BITE - 1] =
        BattleEffect_RunPoisonousBite, // target_strike.c: Poisonous Bite
    [BATTLE_EFFECT_FLYING_ATTACK - 1] =
        BattleEffect_RunFlyingAttack, // flurry.c: Flying Attack
    [BATTLE_EFFECT_UNDEAD_SWORD - 1] =
        BattleEffect_RunUndeadSword, // undead_sword.c: Undead Sword
    [BATTLE_EFFECT_RANSACK - 1] = BattleEffect_RunRansack, // flurry.c: Ransack
    [BATTLE_EFFECT_STICKY_POISON - 1] =
        BattleEffect_RunStickyPoison, // lobbed_shells.c: Sticky Poison
    [BATTLE_EFFECT_POISON_FANG - 1] =
        BattleEffect_RunPoisonousBite, // target_strike.c: Poisonous Bite
    [BATTLE_EFFECT_ELECTRIC_BITE - 1] =
        BattleEffect_RunElectricBite, // target_strike.c: Electric Bite
    [BATTLE_EFFECT_POISON_TAIL - 1] =
        BattleEffect_RunPoisonTail, // target_strike.c: Poison Tail
    [BATTLE_EFFECT_ONSLAUGHT - 1] =
        BattleEffect_RunOnslaught, // rushing_strike.c: Onslaught
    [BATTLE_EFFECT_VAMPIRIC_FANG - 1] =
        BattleEffect_RunVampiricFang, // target_strike.c: Vampiric Fang
    [BATTLE_EFFECT_BACTERIA_RUSH - 1] =
        BattleEffect_RunBacteriaRush, // flurry.c: Bacteria Rush
    [BATTLE_EFFECT_SWIFT_STRIKE - 1] =
        BattleEffect_RunFlyingAttack, // flurry.c: Flying Attack
    [BATTLE_EFFECT_ROTTEN_BLOOD - 1] =
        BattleEffect_RunRottenBlood, // lobbed_shells.c: Rotten Blood
    [BATTLE_EFFECT_DOUBLE_FANG - 1] =
        BattleEffect_RunDoubleFang, // target_strike.c: Double Fang
    [BATTLE_EFFECT_MORTAL_BLOW - 1] =
        BattleEffect_RunMortalBlow, // target_strike.c: Mortal Blow
    [BATTLE_EFFECT_FREEBITE_RUSH - 1] =
        BattleEffect_RunFreebiteRush, // flurry.c: Freebite Rush
    [BATTLE_EFFECT_TWIN_BEAKS - 1] =
        BattleEffect_RunTwinBeaks, // target_strike.c: Twin Beaks
    [BATTLE_EFFECT_RABID_FANG - 1] =
        BattleEffect_RunRabidFang, // target_strike.c: Rabid Fang
    [BATTLE_EFFECT_ACID_BITE - 1] =
        BattleEffect_RunAcidBite, // target_strike.c: Acid Bite
    [BATTLE_EFFECT_DYNAMITE - 1] =
        BattleEffect_RunDynamite, // plasma.c: Dynamite
    [BATTLE_EFFECT_HEADBUTT - 1] =
        BattleEffect_RunHeadbutt, // rushing_strike.c: Headbutt
    [BATTLE_EFFECT_POISON_INK - 1] =
        BattleEffect_RunPoisonInk, // lobbed_shells.c: Poison Ink
    [BATTLE_EFFECT_COUNTERSTRIKE - 1] =
        BattleEffect_RunBearClaw, // flurry.c: Bear Claw
    [BATTLE_EFFECT_MAD_DASH - 1] = BattleEffect_RunRansack, // flurry.c: Ransack
    [BATTLE_EFFECT_SOOTHING_STAR - 1] =
        BattleEffect_RunSoothingStar, // streaks.c: Soothing Star
    [BATTLE_EFFECT_SPIDER_WEB - 1] =
        BattleEffect_RunSpiderWeb, // spider_web.c: Spider Web
    [BATTLE_EFFECT_HEARTRENDER - 1] =
        BattleEffect_RunHeartrender, // target_motes.c: Heartrender
    [BATTLE_EFFECT_MAD_SPATTER - 1] =
        BattleEffect_RunMadSpatter, // lobbed_shells.c: Mad Spatter
    [BATTLE_EFFECT_SPASM - 1] = BattleEffect_RunSpasm, // mirrored_wall.c: Spasm
    [BATTLE_EFFECT_SLEEP_STAR - 1] =
        BattleEffect_RunSleepStar, // streaks.c: Sleep Star
    [BATTLE_EFFECT_DECOMPOSE - 1] =
        BattleEffect_RunDecompose, // lobbed_shells.c: Decompose
    [BATTLE_EFFECT_HAUNTING - 1] =
        BattleEffect_RunHaunting, // gathering_lights.c: Haunting
    [BATTLE_EFFECT_WORMS - 1] = BattleEffect_RunCure, // healing.c: Cure
    [BATTLE_EFFECT_BERSERK - 1] =
        BattleEffect_RunBerserk, // mirrored_wall.c: Berserk
    [BATTLE_EFFECT_LUCID_PROPHECY - 1] =
        BattleEffect_RunCurse, // target_motes.c: Curse
    [BATTLE_EFFECT_SMOKE_BOMB - 1] =
        BattleEffect_RunSmokeBomb, // eruption.c: Smoke Bomb
    [BATTLE_EFFECT_WING_STROKE - 1] =
        BattleEffect_RunWingStroke, // projectiles.c: Wing Stroke
    [BATTLE_EFFECT_POISON_BEAT - 1] =
        BattleEffect_RunPoisonBeat, // spider_web.c: Poison Beat
    [BATTLE_EFFECT_SPINNING_BEAT - 1] =
        BattleEffect_RunSpinningBeat, // spider_web.c: Spinning Beat
    [BATTLE_EFFECT_OUTER_SPACE - 1] =
        BattleEffect_RunOuterSpace, // particle_streams.c: Outer Space
    [BATTLE_EFFECT_DRAGON_DRIVER - 1] =
        BattleEffect_RunDragonDriver, // tiamat.c: Dragon Driver
    [BATTLE_EFFECT_DRAIN_FANG - 1] =
        BattleEffect_RunDrainFang, // target_strike.c: Drain Fang
    [BATTLE_EFFECT_UNUSED_SEVERE_BLOW_LEAP - 1] =
        BattleEffect_RunUnusedSevereBlowLeap,            // thrash.c
    [BATTLE_EFFECT_THRASH - 1] = BattleEffect_RunThrash, // thrash.c: Thrash
    [BATTLE_EFFECT_RECOVERY - 1] =
        BattleEffect_RunRecovery, // mirrored_wall.c: Recovery
    [BATTLE_EFFECT_FIRE_BLESSING - 1] =
        BattleEffect_RunFireBlessing, // burst_shower.c: Fire Blessing
    [BATTLE_EFFECT_WATER_BLESSING - 1] =
        BattleEffect_RunWaterBlessing, // burst_shower.c: Water Blessing
    [BATTLE_EFFECT_ICE_BLESSING - 1] =
        BattleEffect_RunIceBlessing, // projectiles.c: Ice Blessing
    [BATTLE_EFFECT_DARK_BLESSING - 1] =
        BattleEffect_RunDarkBlessing, // burst_shower.c: Dark Blessing
    [BATTLE_EFFECT_ACID_BLESSING - 1] =
        BattleEffect_RunAcidBlessing, // burst_shower.c: Dark Blessing
    [BATTLE_EFFECT_STORM_BLESSING - 1] =
        BattleEffect_RunStormBlessing, // burst_shower.c: Storm Blessing
    [BATTLE_EFFECT_EVIL_BLESSING - 1] =
        BattleEffect_RunEvilBlessing, // burst_shower.c: Evil Blessing
    [BATTLE_EFFECT_DEADLY_GAS - 1] =
        BattleEffect_RunDeadlyGas, // burst_shower.c: Deadly Gas
};

// boreas.c, cybele.c and drain.c: Rock and ice effect geometry.

/// The poses of the two halves of Cybele's face, three pairs, by
/// CYBELE_POSE_FACE_LEFT and _RIGHT.
u8 gCybele_ObjectPoses[][2] = {
    {8, 10},
    {9, 11},
    {3, 4},
};
// The five thorn cells, which the chips use too.
u16 gCybele_ThornOffsets[] = {3920, 3932, 3947, 3955, 3970};
u8 gCybele_ThornWidths[] = {2, 5, 2, 5, 3};
u8 gCybele_ThornHeights[] = {6, 3, 4, 3, 3};
// The three leaf cells, square.
u16 gCybele_LeafOffsets[] = {3456, 3712, 3856};
u8 gCybele_LeafSizes[] = {16, 12, 8};
// The five chip cells, the thorn cells again.
u16 gCybele_ChipOffsets[] = {3920, 3932, 3947, 3955, 3970};
u8 gCybele_ChipWidths[] = {2, 5, 2, 5, 3};
u8 gCybele_ChipHeights[] = {6, 3, 4, 3, 3};
// The five pictures of a spark, square.
u16 gCybele_SparkOffsets[] = {3979, 4043, 4268, 4389, 4470};
u8 gCybele_SparkSizes[] = {8, 15, 11, 9, 5};
// The ember cells of the blast sheet, square. Only the last, largest one is
// drawn.
u16 gCybele_EmberOffsets[] = {5760, 6016, 7040, 9344, 11648, 13952, 16256};
u16 gCybele_EmberSizes[] = {16, 32, 48, 48, 48, 48, 48};
/// Rock and ice effect: where the rocks stand (and later the boulders, 56
/// pixels further left), by the bottom-left corner.
u8 gBoreas_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
                          {86, 126}, {85, 98},  {64, 98},  {98, 96},
                          {77, 87},  {84, 88},  {68, 67},  {81, 49},
                          {94, 70},  {70, 51},  {93, 36},  {81, 50}};
/// The battle background's jolt on frames 176 to 179.
s8 gBoreas_ShakeOffsets[] = {-4, -3, -2, 0};
// The first half's three rock pictures, drawn standing on their points.
u16 gBoreas_RockCells[] = {0, 1152, 2064};
u8 gBoreas_RockWidths[] = {24, 24, 20};
u8 gBoreas_RockHeights[] = {48, 38, 26};
/// The third sprite's animations from frame 209, one every 4 frames.
u8 gBoreas_Animations[] = {2, 9, 10, 11};
// Rock and ice effect: the chips thrown up as the rocks crumble: four large
// ones, then from frame 192 seven small ones.
u8 gBoreas_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};
u8 gBoreas_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};
u16 gBoreas_DebrisCells[] = {2584, 2704, 2880, 2992, 5062, 5094,
                             5142, 5157, 5185, 5227, 5299};
// Rock and ice effect: the three 16x16 spray images.
u8 gBoreas_SprayWidths[] = {16, 16, 16};
u8 gBoreas_SprayHeights[] = {16, 16, 16};
u16 gBoreas_SprayCells[] = {4294, 4550, 4806};
// The second half's boulders (56 pixels left of the rock points) and the
// chunks they crumble into: the same three pictures as the rocks.
u16 gBoreas_BoulderCells[] = {0, 1152, 2064};
u8 gBoreas_BoulderWidths[] = {24, 24, 20};
u8 gBoreas_BoulderHeights[] = {48, 38, 26};
u8 gBoreas_FallWidths[] = {24, 24, 20};
u8 gBoreas_FallHeights[] = {48, 38, 26};
u16 gBoreas_FallCells[] = {0, 1152, 2064};
/// Ice shards: shard i falls with gravity i & 3, in 16.16 pixels a frame
/// each frame: 1/4, 1/8, 1/4 and 1/2.
s32 gDrain_Gravities[] = {Q16_ONE / 4, Q16_ONE / 8, Q16_ONE / 4, Q16_ONE / 2};

// stat_change.c: Stat-reduction shard geometry.

// The twelve shard cells, three frames for each of four shapes.
u16 gStatChange_ShardCells[] = {0,   120, 180, 258, 298, 352,
                                408, 471, 567, 607, 631, 661};
u8 gStatChange_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};
u8 gStatChange_ShardHeights[] = {12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5};

// quake.c

/// Rock wall timing, by variant: frames in the effect, frames each pillar
/// stands, and how far the screen shakes.
u8 gQuake_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

// fire.c: Homing ember counts and flare geometry.

/// Embers by variant, at most FIRE_EMBERS (fire.c).
u8 gFire_Counts[] = {16, 32, 64};
// The six impact flare pictures: sizes, y offsets and cells.
u8 gFire_FlareWidths[] = FLAME_PUFF_CELL_WIDTHS;
u8 gFire_FlareHeights[] = FLAME_PUFF_CELL_HEIGHTS;
u8 gFire_FlareBiasY[] = FLAME_PUFF_CELL_BIAS_Y;
u16 gFire_FlareCells[] = FLAME_PUFF_OFFSETS;

// frost.c: Rising mote geometry and timing.

/// Where each ice column stands: x, then the y of its foot.
u8 gFrost_ColumnSpots[][2] = {
    {53, 108}, {73, 110}, {29, 110}, {73, 108}, {33, 104}, {96, 106}, {8, 109},
    {62, 98},  {33, 100}, {87, 100}, {21, 95},  {53, 90},  {33, 90},  {72, 91},
    {82, 91},  {77, 86},  {52, 84},  {18, 92},  {56, 81},
};
/// Motes, ice columns and the burst frame, for each variant.
u8 gFrost_Timings[][3] = {
    {32, 8, 85},
    {48, 12, 97},
    {64, 19, 109},
};
// The eight mote pictures.
u8 gFrost_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
u8 gFrost_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
u16 gFrost_MoteCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};
// The three ice column pictures, 32 wide.
u16 gFrost_ColumnCells[] = {0, 768, 1280};
u8 gFrost_ColumnHeights[] = {24, 16, 16};
// The eight chip pictures (the same as the motes).
u8 gFrost_ChipWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
u8 gFrost_ChipHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
u16 gFrost_ChipCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

// ray.c: Closing bolt sparks by variant.

/// Sparks by variant; [1] is how many each bolt throws a frame.
u8 gRay_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

// eruption.c: Eruption counts and puff layouts.

/// Motes, puffs of smoke and frames, for each variant.
u8 gEruption_Counts[][3] = {{16, 8, 72}, {32, 16, 80}, {128, 32, 88}};
// The seven puff pictures, square.
u16 gEruption_PuffCells[] = BLAST_OFFSETS;
u16 gEruption_PuffSizes[] = BLAST_CELL_SIZES;

// Unreferenced cell layouts, from 6x6 through 20x20. gUnused_SheetCells6 is
// the first six offsets of gUnused_SheetCells, named for its length.
u16 gUnused_SheetCells6[] = {0, 36, 100, 200, 344, 540};
u16 gUnused_SheetCells[] = {0,   36,   100,  200,  344, 540,
                            796, 1120, 1520, 1900, 2300};
u8 gUnused_SheetWidths[] = {6, 8, 10, 12, 14, 16, 18, 20, 19, 20};
u8 gUnused_SheetHeights[] = {6, 8, 10, 12, 14, 16, 18, 20, 20, 20};

// atalanta.c

/// Staged particle scale: Atalanta's formation sprites keep this
/// scale, one to one, throughout.
struct SpriteScale gAtalanta_UnitScale = {{{Q16_ONE, Q16_ONE}}};

// growth.c: Growing vine and pillar geometry.

// The three column shapes: vine and pillar widths, cells and height caps.
u8 gGrowth_VineWidths[] = {21, 28, 19};
u8 gGrowth_HeightLimits[] = {119, 119, 102};
u16 gGrowth_VineCells[] = {0, 2499, 5831};
u8 gGrowth_PillarWidths[] = {10, 8, 9};
u16 gGrowth_PillarCells[] = {0, 1080, 1888};
// Columns by variant, and where each column's root stands.
u8 gGrowth_ColumnCounts[] = {4, 8, 12};
u8 gGrowth_ColumnX[] = {64, 72,  56, 80, 48, 88, 40, 96,
                        32, 104, 24, 72, 56, 80, 48, 88};
s8 gGrowth_PillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};
// The seven pictures of a puff of dust.
s8 gGrowth_PuffWidths[] = {5, 7, 11, 16, 23, 27, 16};
u8 gGrowth_PuffHeights[] = {10, 14, 21, 32, 45, 50, 32};
u16 gGrowth_PuffCells[] = {0, 50, 148, 379, 891, 1926, 3276};
/// Each vine's root, across from the middle of the targets.
s8 gGrowth_ColumnOffsets[] = {0,   14, -32, 8,   32, -48, 4,   12,
                              -24, 24, -12, -40, -4, -16, -32, 12};
// Each thorn's kind (THORN_ flags) and root.
u8 gGrowth_Kinds[] = {
    0,           THORN_SMALL | THORN_MIRRORED, THORN_SMALL, THORN_MIRRORED,
    THORN_SMALL, THORN_SMALL | THORN_MIRRORED, THORN_SMALL, THORN_MIRRORED};
s8 gGrowth_X[] = {24, 80, 24, 48, 88, 40, 8, 96};
/// Thorns by variant.
u8 gGrowth_Counts[] = {3, 5, 8};
// The seven pictures of a puff of dust.
s8 gGrowth_ThornPuffWidths[] = {5, 7, 11, 17, 32, 32, 19};
u8 gGrowth_ThornPuffHeights[] = {10, 14, 21, 34, 59, 60, 38};
u16 gGrowth_ThornPuffCells[] = {5888, 5938, 6036, 6267, 6845, 8733, 10653};

// bolt.c: Lightning bolt counts and glint palettes.

/// Bolt, by variant: sparks each bolt throws, glints a frame around each
/// target, how far the glints spread, and bolts drawn over each other.
u8 gBolt_Counts[][4] = {
    [EFFECT_VARIANT_BASIC] = {16, 0, 15, 1},      // Bolt
    [EFFECT_VARIANT_STRONGER] = {32, 3, 31, 1},   // Flash Bolt
    [EFFECT_VARIANT_STRONGEST] = {128, 5, 48, 3}, // Blue Bolt
};
// The flips a glint may be drawn in, and its blend by variant.
u8 gBolt_GlintFlips[] = GLINT_FLIPS;
/// The glints' blend mode by variant: the brighter pixel wins for Bolt and
/// Flash Bolt, and Blue Bolt adds the glint to the canvas.
u8 gBolt_GlintModes[] = {COMPOSITE_LIGHTEN, COMPOSITE_LIGHTEN,
                         COMPOSITE_ADDITIVE};

// projectiles.c: Projectile volley modes and picture cells.

/// Comet picture heights, by comet (enum ProjectileVolleyComet).
u8 gProjectileVolley_CometHeights[] = {24, 36, 48};
/// Each projectile volley mode, by the PROJECTILE_VOLLEY_ columns
/// (projectiles.c).
u8 gProjectileVolley_Modes[][5] = {
    // PROJECTILE_VOLLEY_: SHOTS, STEPS, STAGGER, DELAY, SPARKS
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
// Hit flash pictures, by step (flash frame / 2).
u8 gProjectileVolley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};
u8 gProjectileVolley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};
u8 gProjectileVolley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};
u8 gProjectileVolley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12};
u16 gProjectileVolley_FlashOffsets[] = {8192, 8312,  8582, 9014,
                                        9674, 10646, 11738};
/// Ice chip pictures, by shot index & 3.
u16 gProjectileVolley_PelletOffsets[] = {2308, 2372, 2436, 2500};

// djinn_attacks.c: Mercury Djinni flame launch frames.

/// The frames on which the Djinni fires each of its five flames.
u8 gDjinnAttacks_LaunchFrames[] = {4, 7, 10, 21, 34};

// hail.c: Particle reveal and rock toss chips.

// The four ice chip cells.
u8 gHail_CellWidths[] = {8, 8, 8, 8};
u8 gHail_CellHeights[] = {8, 8, 8, 8};
u16 gHail_Cells[] = {2308, 2372, 2436, 2500};
/// The blitter flags a chip may take, chosen at random: clipped at the canvas
/// edges, and drawn as is, mirrored across, mirrored down or both.
u8 gHail_ChipFlips[] = {3, 7, 11, 15};
// The rock (cell 0) and its chips in the portal sheet: size, where each begins
// in the sheet, and its offset from the rock's corner.
u8 gHail_ChipWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};
u8 gHail_ChipHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};
u16 gHail_ChipCells[] = {0, 1152, 1243, 1341, 1533, 1677, 1898, 2098, 2252};
u8 gHail_ChipX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};
u8 gHail_ChipY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

// spire.c and thor.c: Storm effect colours and shattering rocks.

/// The colours a target flashes when a bolt strikes it, chosen at random, as
/// SPRITE_COLOR_ values: white, black, red raised to full, or its colour
/// channels rotated.
u8 gThor_TargetPalettes[] = {7, 14, 10, 4};
/// Flip flags for the smoke puffs, chosen at random.
u8 gThor_SmokeFlips[] = GLINT_FLIPS;
/// Where each of a rock's 21 shards starts, x then y, before the rock's
/// offset across.
u8 gSpire_ShardOffsets[32][2] = {
    {65, 104}, {68, 95}, {64, 93}, {61, 93}, {64, 87}, {72, 85}, {56, 88},
    {51, 80},  {61, 75}, {72, 81}, {69, 71}, {66, 70}, {71, 64}, {49, 71},
    {59, 69},  {53, 68}, {78, 73}, {55, 67}, {75, 62}, {61, 65}, {53, 61},
};
// Each rock's offset across, the frame it appears (it lands 18 frames
// later), and the rocks by variant.
s8 gSpire_RockX[] = {0, -12, 12, -8, 16};
u8 gSpire_DropFrames[] = {8, 16, 20, 24, 32};
u8 gSpire_RockCounts[] = {1, 3, 5};
// Shattering rocks: five shard shapes in three tumbling phases each, decoded
// after the boulder picture.
u8 gSpire_ShardWidths[] = {5,  11, 10, 7,  7,  12, 13, 9,
                           19, 12, 17, 22, 15, 16, 14};
u8 gSpire_ShardHeights[] = {11, 6,  10, 11, 10, 7,  17, 21,
                            12, 21, 17, 13, 16, 16, 15};
u16 gSpire_ShardCells[] = {0,   55,   121,  221,  298,  368,  452,  673,
                           862, 1090, 1342, 1631, 1917, 2157, 2413, 2623};

// gaia.c and flurry.c: flurry rows and rising wall geometry.

/// Each flurry mode (enum FlurryMode), by the FLURRY_
/// columns (flurry.c).
// clang-format off
u8 gFlurry_Styles[][7] = {
    // FLURRY_: ARC_SHEET, PALETTE, STRIKES, MOTES, INTERVAL, FRAMES,
    // FLASH
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
// clang-format on
// Rising wall: the nine rows of the growing wall in the rebuilt sheet.
u16 gGaia_RowCells[] = {0, 160, 352, 640, 1024, 1504, 2080, 2752, 3776};
u8 gGaia_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};
// The swelling ground's three pictures, after the rebuilt rows; they share
// the earth wall's sizes.
u16 gGaia_MoundCells[] = {10672, 11505, 12550};
u8 gGaia_MoundWidths[] = EARTH_WALL_CELL_WIDTHS;
u8 gGaia_MoundHeights[] = EARTH_WALL_CELL_HEIGHTS;
/// BG2's x shift, by side and variant, in canvas pixels written to BG2X: a
/// positive shift moves the canvas left on the screen.
s8 gGaia_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};
/// The columns rock particles[16] to particles[21] take.
u8 gGaia_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

// beams.c: Beam picture cells and reach.

// The six cells of the growing beam picture, and how far each reaches short of
// the user. The Lost Age reads the same tables.
u8 gBeam_CellWidths[] = SLASH_CELL_WIDTHS;
u8 gBeam_CellHeights[] = SLASH_CELL_HEIGHTS;
u16 gBeam_Cells[] = {0, 266, 752, 1533, 2495, 3496};
u8 gBeam_CellReaches[] = SLASH_CELL_REACH;

// casting_impact.c: Casting impact glints and orbit cells.

/// The flips a glint may take, picked at random.
u8 gCastingImpact_GlintDrawFlags[] = GLINT_FLIPS;
// Where each picture of the counter reveal is drawn: seven x positions for
// each side of the field, and seven y positions, from the target's height, for
// both.
u8 gCastingImpact_ImageX[][7] = {
    {10, 30, 0, 0, 0, 6, 52},
    {60, 0, 0, 0, 0, 0, 0},
};
u8 gCastingImpact_ImageY[] = {0, 20, 2, 2, 34, 64, 68};
/// The four ice chip cells, 8x8.
u16 gCastingImpact_IceChipCells[] = {2308, 2372, 2436, 2500, 0};

// helm_splitter.c: Fire swirl picture cells.

// The seven cells of the fire swirl, and where each sits from the user.
u8 gHelmSplitter_CellWidths[] = {26, 46, 63, 77, 60, 39, 32};
u8 gHelmSplitter_CellHeights[] = {11, 20, 37, 65, 65, 54, 39};
u16 gHelmSplitter_Cells[] = {0, 286, 1206, 3537, 8542, 12442, 14548};
s8 gHelmSplitter_CellBiasX[] = {53, 36, 16, 2, 2, 2, 2};
s8 gHelmSplitter_CellBiasY[] = {2, 3, 3, 3, 4, 14, 29};

// helm_splitter.c: Sprite formation coordinates and impact burst cells.

// The offsets of BattleEffect_PlaceFormationObjects' formations in pixels, one
// axis at a time: Nereid's twelve in rows of 1, 3, 4 and 4; Cybele's eight in
// rows of 3, 2 and 3; Tiamat's eight drawn at 1.25 times their size. Thor's,
// a 3x3 grid, needs no table.
u8 gHelmSplitter_TwelveX[] = {52, 25, 57, 89, 7, 39, 71, 103, 21, 53, 85, 117};
u8 gHelmSplitter_TwelveZ[] = {0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96};
u8 gHelmSplitter_RightEightX[] = {0, 32, 64, 16, 48, 0, 32, 64};
u8 gHelmSplitter_RightEightZ[] = {0, 0, 0, 32, 32, 64, 64, 64};
u8 gHelmSplitter_SmallEightX[] = {0, 40, 80, 0, 40, 80, 40, 80};
u8 gHelmSplitter_SmallEightZ[] = {13, 0, 0, 53, 40, 40, 80, 80};
// The eight cells of the impact flash.
u8 gHelmSplitter_ImpactBurstCellWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};
u8 gHelmSplitter_ImpactBurstCellHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};
u16 gHelmSplitter_ImpactBurstCells[] = {0,     4608,  9216,  13356,
                                        16020, 18684, 20668, 22020};

// particle_streams.c and ramses.c: Particle stream paths and circling effect
// geometry.

/// The seven sparks that orbit the rising formation.
s16 gParticleStreams_OrbitPoints[][3] = {
    {-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48}, {-16, -72, -24},
    {16, -80, 16},  {32, -112, -32}, {48, -96, 56}};
/// Where the seven drops fall, in pixels.
s16 gParticleStreams_DropPoints[][2] = {
    {32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};
// The seven growing flash cells of the blast sheet, square.
u16 gParticleStreams_FlashCells[] = BLAST_OFFSETS;
u16 gParticleStreams_FlashSizes[] = BLAST_CELL_SIZES;
// Where the formation's sprites stand, in pixels from its corner.
u8 gRamses_ObjectColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};
u8 gRamses_ObjectRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};
// The puff cells of the blast sheet, square; only the last four are used.
u16 gRamses_PuffCells[] = BLAST_OFFSETS;
u16 gRamses_PuffSizes[] = BLAST_CELL_SIZES;

// ragnarok.c: Flame blade strike columns and flash cells.

/// The canvas x of each strike column, three for each side.
u8 gRagnarok_StrikeColumns[][3] = {
    {32, 24, 0},
    {96, 104, 128},
};
/// The flash picture for each three frames at the end of a flash's life.
u8 gRagnarok_FlashCells[] = FLASH_FADE_OFFSETS;

// judgment.c and titan_blade.c: Falling sword flashes, dust and shockwave
// debris.

/// The flash picture for each three frames at the end of a flash's life.
u8 gTitanBlade_FlashCells[] = FLASH_FADE_OFFSETS;
/// Dust gravity, by particle index & 3, in gNereid_Gravity's steps.
s32 gTitanBlade_DustGravity[] = {Q16_ONE / 2, Q16_ONE / 4, Q16_ONE / 4,
                                 Q16_ONE / 8};
// Judgment's burst: seven quarter pictures, each half as wide as it is
// tall, from JUDGMENT_SHEET_BURST.
u8 gJudgment_BurstHeights[] = {2, 3, 4, 6, 10, 11, 12};
u16 gJudgment_BurstCells[] = {0, 8, 26, 58, 130, 330, 572};
// The six shard pictures thrown from the burst, from the sheet's start.
u16 gJudgment_ShardCells[] = {2396, 2985, 3698, 4411, 4810, 5146};
u8 gJudgment_ShardWidths[] = {19, 23, 23, 19, 16, 17};
u8 gJudgment_ShardHeights[] = {31, 31, 31, 21, 21, 24};

// kirin.c: Sprite row coordinates.

// Sprite row: pixel offsets of its nine sprites.
u8 gKirin_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};
u8 gKirin_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

// neptune.c: Formation sweep coordinates and gravity.

// The formation's eight sprites: pixel offsets, scaled by 1.5 as placed.
u8 gNeptune_ObjectX[] = {0, 32, 64, 0, 32, 64, 96, 96};
u8 gNeptune_ObjectY[] = {0, 0, 0, 32, 32, 32, 0, 32};
/// Spray gravity, by particle index & 3, in gNereid_Gravity's steps.
s32 gNeptune_Gravity[] = {Q16_ONE / 2, Q16_ONE / 4, Q16_ONE / 4, Q16_ONE / 8};

// undead_sword.c: Rising burst spark cells.

// The seven square spark pictures, from UNDEAD_SWORD_SPARK_SHEET.
u16 gUndeadSword_SparkCells[] = BLAST_OFFSETS;
u16 gUndeadSword_SparkSizes[] = BLAST_CELL_SIZES;
