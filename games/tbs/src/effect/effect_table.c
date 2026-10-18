#include "unit/effect/effect_table.h"
#include "unit/effect/effect.h"
#include "types.h"
#include "vector3.h"
#include "render_placement.h"

// Cell layouts, timing rows and handler entries for the battle effects.
// Keep the definitions in link order. Picture cell offsets are byte offsets
// from the sheet start, accumulated as width times height.

// The tables are plain arrays, not const, as the effect code reads them:
// it reloads their addresses where const would let the compiler keep
// them. Their .data is linked into the effect bank in ROM.

struct BattleEffectArgument;

// Shared cell layouts.
//
// Mote cells: particle sheets hold ten, cell n (from 0) n + 1 pixels wide and
// twice that tall (EFFECT_DRAW_MOTE).
u16 gBattleFx_MoteCellOffsets[] = {0, 2, 10, 28, 60, 110, 182, 280, 408, 570};
// Flare cells: the first ten are squares 2x2 to 20x20, cell n (from 0) n + 1
// pixels from its centre to each edge (EFFECT_DRAW_FLARE).
u16 gBattleFx_FlareCells[] = {0, 4, 20, 56,  120, 220, 364, 560,  816,  1140,
                              0, 8, 40, 112, 240, 440, 728, 1120, 1632, 2280};
// Nine square puff cells; gBattleFx_PuffSizes gives each one's side.
u16 gBattleFx_PuffCells[] = {0, 25, 74, 123, 172, 221, 246, 255, 256};
u8 gBattleFx_PuffSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};
// The six cells of an erupting flame puff: size, a downward offset from
// the flame's point, and where each begins in the sheet. The Mars Djinni
// shows each for 8 frames, centred on the flame's x.
// Six cells drawn centred across x with their tops PuffArcCellBiasY below
// the anchor, so that all six stand on about the same line.
u8 gBattleFx_PuffArcCellWidths[] = {24, 29, 30, 27, 24, 16};
u8 gBattleFx_PuffArcCellHeights[] = {23, 32, 37, 49, 53, 52};
u8 gBattleFx_PuffArcCellBiasY[] = {41, 32, 26, 11, 4, 2};
u16 gBattleFx_PuffArcCellSourceOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
// Six glint cells, each GlintCellWidths by GlintCellHeights pixels.
u16 gBattleFx_GlintCellOffsets[] = {0, 589, 1302, 2015, 2414, 2750};
u8 gBattleFx_GlintCellWidths[] = {19, 23, 23, 19, 16, 17};
u8 gBattleFx_GlintCellHeights[] = {31, 31, 31, 21, 21, 24};

// Falling shard and bolt wave counts.
u8 gIce_Counts[][2] = {
    // shards, frames
    {16, 46}, // variant 0
    {20, 52}, // variant 1
    {32, 80}, // variant 2
};
u8 gFallingBolts_Counts[][4] = {
    // bolts, sparks per bolt, spacing, frames
    {6, 8, 32, 80},  // variant 0
    {8, 16, 24, 80}, // variant 1
    {32, 4, 8, 80},  // variant 2
};

// Falling streak timing and motion records.
s8 gStreakVolley_Records[] = {
    // STREAK_ANIMATE, STREAK_HIT, STREAK_MOTION, STREAK_SHAKE
    0, 1, 0,  0, // STREAK_STYLE_EMBER
    0, 1, 4,  0, // STREAK_STYLE_LIME
    0, 1, -1, 1, // STREAK_STYLE_VIOLET
    1, 1, -1, 1, // STREAK_STYLE_RED
    1, 1, 4,  0, // STREAK_STYLE_CYAN
};
// Pixels the target trembles by, frame by frame.
s8 gStreakVolley_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

// Target strike timing, motion and picture cells.
// clang-format off
u8 gTargetStrike_Records[] = {
    // STRIKE_KIND, _HITS, _FLAGS, _COLOUR, _IMAGE_FRAMES, _COMPOSITE, _FRAMES
    STRIKE_KIND_COLUMN, 2, STRIKE_SPARKS, 0, 6, COMPOSITE_ADDITIVE, 70, // 0
    STRIKE_KIND_COLUMN, 1, STRIKE_SMOKE,  1, 6, COMPOSITE_MAX,      54, // 1
    STRIKE_KIND_COLUMN, 3, STRIKE_SMOKE,  2, 6, COMPOSITE_MAX,      96, // 2
    STRIKE_KIND_COLUMN, 3, STRIKE_ABSORB, 3, 4, COMPOSITE_MAX,      96, // 3
    STRIKE_KIND_COLUMN, 3, STRIKE_ABSORB, 4, 4, COMPOSITE_MAX,      96, // 4
    STRIKE_KIND_COLUMN, 3, STRIKE_GLINTS, 5, 4, COMPOSITE_ADDITIVE, 96, // 5
    STRIKE_KIND_SLASH,  1, STRIKE_GLINTS, 5, 4, COMPOSITE_ADDITIVE, 38, // 6
    STRIKE_KIND_SLASH,  2, STRIKE_SPARKS, 5, 2, COMPOSITE_ADDITIVE, 48, // 7
    STRIKE_KIND_SLASH,  1, STRIKE_SPARKS | STRIKE_HEAVY,
                                          4, 2, COMPOSITE_ADDITIVE, 50, // 8
    STRIKE_KIND_SLASH,  1, STRIKE_SMOKE,  1, 2, COMPOSITE_ADDITIVE, 46, // 9
    STRIKE_KIND_COLUMN, 1, STRIKE_SMOKE,  4, 6, COMPOSITE_MAX,      54, // 10
    STRIKE_KIND_COLUMN, 3, STRIKE_ABSORB, 4, 4, COMPOSITE_ADDITIVE, 96, // 11
};
// clang-format on
// The six slash pictures: size, sheet offset and reach from the target.
u8 gTargetStrike_Widths[] = {38, 54, 71, 74, 77, 65};
u8 gTargetStrike_Heights[] = {7, 9, 11, 13, 13, 11};
u16 gTargetStrike_Offsets[] = {3200, 3466, 3952, 4733, 5695, 6696};
u8 gTargetStrike_Reach[] = {58, 42, 25, 13, 5, 0};
// The column stage shown by each picture of a rise.
u8 gTargetStrike_ColumnStages[] = {0, 1, 2, 3, 2};
// The flips a glint may be drawn in.
u8 gTargetStrike_GlintDrawFlags[] = {0, 4, 8, 12};

// Blade rain and shattering tile pieces.
// The sword pictures, by how steeply the sword falls.
u8 gBladeRain_CellWidths[] = {20, 21, 22, 25};
u8 gBladeRain_CellHeights[] = {32, 31, 27, 24};
u16 gBladeRain_CellSourceOffsets[] = {0, 640, 1291, 1885};
// The 33 pieces of the TileAnimationMask picture: where each starts on the
// canvas, its size, and where its pixels begin in the decoded picture. The
// offsets carry one more entry, where the last piece ends.
u8 gBattlePieceWidth[] = {21, 20, 24, 30, 35, 69, 19, 23, 9,  22, 33,
                          10, 18, 9,  7,  9,  17, 55, 30, 25, 40, 29,
                          10, 28, 14, 5,  32, 23, 19, 32, 8,  43, 15};
u8 gBattlePieceHeight[] = {23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35,
                           17, 18, 14, 15, 12, 13, 37, 38, 41, 60, 41,
                           15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11};
u16 gBattlePieceOffset[] = {0,     483,   1423,  2023,  3223,  3923,  7580,
                            8112,  8779,  8986,  9294,  10449, 10619, 10943,
                            11069, 11174, 11282, 11503, 13538, 14678, 15703,
                            18103, 19292, 19442, 20702, 21528, 21733, 22565,
                            23715, 24114, 25234, 25330, 27781, 27946};
u8 gBattlePieceStartX[] = {0,  21, 28, 46, 70, 51, 0,  13, 37, 77, 87,
                           19, 25, 35, 44, 45, 48, 65, 0,  0,  0,  5,
                           33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};
u8 gBattlePieceStartY[] = {0,  0,  0,  0,  0,  0,  19, 17, 23,  28, 8,
                           36, 43, 43, 40, 47, 48, 37, 42, 67,  60, 79,
                           62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

// Canvas corner sparks and two-resource picture cells.
// The gathering burst's corner sparks: offsets and flips.
s8 gCornerSparks_X[] = {0, 32, 0, 32};
s8 gCornerSparks_Y[] = {0, 0, 32, 32};
u8 gCornerSparks_DrawFlags[] = {0, 4, 8, 12};
// The six pictures: the first four from the swirl sheet, the last two from the
// tail sheet. gSwirlSwipe_CellX has six entries for each side.
u8 gSwirlSwipe_CellWidths[] = {40, 112, 112, 112, 88, 64};
u8 gSwirlSwipe_CellHeights[] = {56, 80, 64, 64, 80, 88};
u16 gSwirlSwipe_CellSourceOffsets[] = {0, 2240, 11200, 18368, 0, 7040};
u8 gSwirlSwipe_CellX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};
s8 gSwirlSwipe_CellBiasY[] = {0, 4, 24, -6, -20, -28, 0, 0};

// Gathering lights (Demon Night, Rumble, Haunting), by mode: the lights per
// target and the frames before the targets' turns.
u8 gMemberBurst_Counts[][2] = {
    // lights per target, frames
    {16, 88}, // mode 0
    {8, 72},  // mode 1
    {12, 80}, // mode 2
};

// Mirrored earth wall picture cells.
// The earth wall's three images: the mirrored wall reads them at the start of
// its sheet, the rising wall after its rebuilt rows. Both share the sizes.
u16 gEarthWall_CellSourceOffsets[] = {2702, 3535, 4580};
u8 gEarthWall_CellWidths[] = {17, 19, 20};
u8 gEarthWall_CellHeights[] = {49, 55, 64};

// Djinni beam motes and heads.
// The picture of each step of a mote's eight-step cycle.
u8 gMemberBeam_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};
// The three cells of the glow at a target's feet.
u16 gMemberBeam_HeadOffsets[] = {2702, 3535, 4580};
u8 gMemberBeam_HeadWidths[] = {17, 19, 20};
u8 gMemberBeam_HeadHeights[] = {49, 55, 64};

// Shell explosion records and shards.
s8 gLobbedShells_Records[] = {
    // nonzero: the shells burst into shards, not smoke; unused
    0, 0, // mode 0
    0, 0, // mode 1
    0, 0, // mode 2
    1, 0, // mode 3
    1, 0, // mode 4
    0, 0, // mode 5
    0, 0, // mode 6
};
// Lobbed shells: nine tumbling images in two sets, the second (cells 9 to 17)
// smaller.
u8 gLobbedShells_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12,
                                  6, 7,  7, 7, 6, 6,  8,  8,  8};
u8 gLobbedShells_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12,
                                   8,  7,  6,  6,  8,  6,  4,  3,  3};
u16 gLobbedShells_ShardOffsets[] = {0,    108,  228,  336,  444,  552,
                                    684,  816,  948,  1092, 1140, 1189,
                                    1231, 1273, 1321, 1357, 1389, 1413};

// Shattered panel coordinates.
// Where each stage of the panel is drawn: seven x positions for each side of
// the field, and seven y positions (above the swing) for both sides.
u8 gCondemn_PanelX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};
u8 gCondemn_PanelY[] = {0, 20, 2, 2, 34, 64, 68, 0, 9, 4, 4, 4, 4, 4};

// Spinning and striking triangle vertices.
struct Vector3 gBind_Vertex[] = {{.x = 0, .y = 24, .z = 0}};
struct Vector3 gTriangleStrike_Vertex[] = {{.x = 0, .y = 24, .z = 0}};

// Ring bolt, spinning star and whirlwind comet paths.
struct Vector3 gAstralBlast_Points[] = {{.x = 0, .y = 48 << 16, .z = 0},
                                        {.x = 0, .y = 28 << 16, .z = 0}};
u8 gShiningStar_Radii[] = {32, 16};
u8 gWhirlwindComets_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};
u8 gWhirlwindComets_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};
u16 gWhirlwindComets_Frames[] = {334, 347, 359, 372};
u8 gWhirlwindComets_CellWidths[] = {14, 24, 48};
u8 gWhirlwindComets_CellHeights[] = {14, 19, 24};
u8 gWhirlwindComets_CellBiasY[] = {8, 12, 17};
u16 gWhirlwindComets_CellOffsets[] = {13824, 14020, 14476};

// Palette particle and ember column layouts.
// Ember speeds and sizes, by ember index & 3.
u8 gPaletteParticles_DropSpeeds[] = {0, 8, 12, 16};
// A spark's 1x2 picture.
u8 gPaletteParticles_SparkPixels[] = {32, 32};
u8 gPaletteParticles_DropSizes[] = {1, 1, 2, 3};
// The six streak pictures: size, drop below the streak and sheet offset.
u8 gPaletteParticles_StreakWidths[] = {24, 29, 30, 27, 24, 16};
u8 gPaletteParticles_StreakHeights[] = {23, 32, 37, 49, 53, 52};
u8 gPaletteParticles_StreakDrops[] = {41, 32, 26, 11, 4, 2};
u16 gPaletteParticles_StreakOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
u8 gNereid_Columns[][2] = {
    // the foot of each column of fire: x, y
    {16, 104},
    {32, 104},
    {48, 104},
    {64, 104},
};
// Ember gravity while falling, by ember index & 3.
s32 gNereid_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// Vortex mote counts and tornado shapes.
u8 gFroth_Counts[][2] = {
    // motes, frames
    {6, 140},  // variant 0
    {12, 160}, // variant 1
    {30, 200}, // variant 2
};
u8 gWhirlwind_Shapes[][3] = {
    // tornadoes, sway, screen shake
    {1, 16, 2}, // variant 0
    {2, 24, 2}, // variant 1
    {4, 32, 4}, // variant 2
};

// Crystal counts and picture cells.
// Where each of the 16 shards starts in the crystal: x, then y.
u8 gPrism_ShardStarts[][2] = {
    {32, 60}, {33, 82}, {49, 94}, {41, 89}, {49, 76}, {32, 99},
    {54, 69}, {34, 82}, {45, 71}, {31, 91}, {32, 69}, {26, 80},
    {40, 68}, {41, 60}, {46, 60}, {36, 63}, {0, 0},
};
u8 gPrism_Counts[][2] = {
    // crystals, frames
    {1, 110},  // variant 0
    {4, 134},  // variant 1
    {12, 166}, // variant 2
};
// The twelve shard pictures.
u8 gPrism_ShardWidths[] = {5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22};
u8 gPrism_ShardHeights[] = {14, 10, 7, 13, 14, 14, 32, 30, 24, 38, 20, 20};
s32 gPrism_ShardOffsets[] = {0,   70,   170,  247,  429,  597,
                             751, 1359, 1899, 2451, 2907, 3347};

// Lightning pillar sparks, columns and counts.
u16 gPlasma_Sparks[][2] = {
    // sparks per pillar, shake frames
    {384, 2}, // variant 0
    {256, 3}, // variant 1
    {192, 4}, // variant 2
};
// Each pillar's canvas x, seven for each side.
u8 gPlasma_Columns[] = {32, 64, 16,  48, 72, 96, 64,
                        88, 56, 104, 72, 48, 24, 56};
// How many pillars strike, by variant.
u8 gPlasma_Counts[] = {1, 3, 7};

// Spark group shapes and flash cells.
u16 gSparkGroups_Shapes[][5] = {
    // sparks per burst, bursts, each burst's offset across
    {128, 1, 16, 0, 0},   // variant 0
    {255, 1, 16, 0, 0},   // variant 1
    {192, 3, 0, 24, 48},  // variant 2
    {340, 1, 16, 24, 48}, // variant 3
    {32, 1, 16, 0, 0},    // variant 4
};
// The flash picture for each three frames at the end of a flash's life.
u8 gSparkGroups_FlashCells[] = {5, 5, 4, 3, 2, 1};

// Fire pillar counts, depths and start frames.
// How many pillars stand, by variant.
u8 gVolcano_Counts[] = {1, 2, 4};
// Each pillar's depth, by variant.
s8 gVolcano_Depths[][4] = {{0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};
// The frame each pillar starts.
u8 gVolcano_StartFrames[] = {8, 18, 24, 30, 34};

// Target mote glint orientations.
// Glint orientations: none, and mirrored each way and both.
u8 gTargetMotes_GlintFlips[] = {0, 4, 8, 12};

// Indexed by effect kind minus one; kind zero is handled by the dispatcher.
//
// Each handler is named for the first ability, by ability id, whose entry in
// gBattle_ActionFlags (battle/actflags.tsv) selects its kind, with that
// ability's English name (MsgAbilityName + id): kind 1 is Quake, Earthquake
// and Quake Sphere, so BattleFx_RunQuake. A handler several kinds share takes
// the name of its lowest kind's first ability, and a name another handler has
// already taken passes to the next ability (kind 15 is BattleFx_RunNova, as
// its first ability is a second Blast). Each handler's comment lists every
// ability that reaches it. Kinds that no ability uses keep descriptive names.
BattleFxHandler gBattleFx_EffectHandlers[] = {
    BattleFx_RunQuake,           // 1
    BattleFx_RunSpire,           // 2
    BattleFx_RunGaia,            // 3
    BattleFx_RunGrowth,          // 4
    BattleFx_RunThorn,           // 5
    BattleFx_RunFrost,           // 6
    BattleFx_RunIce,             // 7
    BattleFx_RunPrism,           // 8
    BattleFx_RunDouse,           // 9
    BattleFx_RunFroth,           // 10
    BattleFx_RunFlare,           // 11
    BattleFx_RunFire,            // 12
    BattleFx_RunVolcano,         // 13
    BattleFx_RunBlast,           // 14
    BattleFx_RunNova,            // 15
    BattleFx_RunBolt,            // 16
    BattleFx_RunRay,             // 17
    BattleFx_RunPlasma,          // 18
    BattleFx_RunSlash,           // 19
    BattleFx_RunWhirlwind,       // 20
    BattleFx_RunCure,            // 21
    BattleFx_RunPly,             // 22
    BattleFx_RunWish,            // 23
    BattleFx_RunCurePoison,      // 24
    BattleFx_RunRevive,          // 25
    BattleFx_RunImpact,          // 26
    BattleFx_RunDull,            // 27
    BattleFx_RunGuard,           // 28
    BattleFx_RunImpair,          // 29
    BattleFx_RunWard,            // 30
    BattleFx_RunWeaken,          // 31
    BattleFx_RunTaint,           // 32
    BattleFx_RunDelude,          // 33
    BattleFx_RunOrbitingSpirals, // 34
    BattleFx_RunHeartrender,     // 35
    BattleFx_RunParalyze,        // 36
    BattleFx_RunSleep,           // 37
    BattleFx_RunBind,            // 38
    BattleFx_RunHaunt,           // 39
    BattleFx_RunCondemn,         // 40
    BattleFx_RunCurse,           // 41
    BattleFx_RunDrain,           // 42
    BattleFx_RunBreak,           // 43
    BattleFx_RunWish,            // 44
    BattleFx_RunWish,            // 45
    BattleFx_RunImpact,          // 46
    BattleFx_RunImpair,          // 47
    NULL,                        // 48
    BattleFx_RunDragonCloud,     // 49
    BattleFx_RunDemonNight,      // 50
    BattleFx_RunHelmSplitter,    // 51
    BattleFx_RunQuickStrike,     // 52
    BattleFx_RunDeathPlunge,     // 53
    BattleFx_RunShuriken,        // 54
    BattleFx_RunAnnihilation,    // 55
    BattleFx_RunPunji,           // 56
    BattleFx_RunCuttingEdge,     // 57
    BattleFx_RunRagnarok,        // 58
    BattleFx_RunHeatWave,        // 59
    BattleFx_RunPlanetDiver,     // 60
    BattleFx_RunTitanBlade,      // 61
    BattleFx_RunShiningStar,     // 62
    BattleFx_RunBlizzard,        // 63
    BattleFx_RunDemonFire,       // 64
    BattleFx_RunVorpalSlash,     // 65
    BattleFx_RunDrown,           // 66
    BattleFx_RunTriangleStrike,  // 67
    BattleFx_RunAstralBlast,     // 68
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 69-76
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 77-84
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 85-92
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 93-100
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 101-108
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 109-116
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 117-124
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 125-132
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 133-140
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 141-148
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 149-156
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 157-164
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 165-172
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 173-180
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 181-188
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 189-196
    NULL,
    NULL,
    NULL,                   // 197-199
    BattleFx_RunGranite,    // 200
    BattleFx_RunRevive,     // 201
    BattleFx_RunVine,       // 202
    BattleFx_RunGround,     // 203
    BattleFx_RunPly,        // 204
    BattleFx_RunSleet,      // 205
    BattleFx_RunWish,       // 206
    BattleFx_RunHail,       // 207
    BattleFx_RunTonic,      // 208
    BattleFx_RunDew,        // 209
    BattleFx_RunImpact,     // 210
    BattleFx_RunGuard,      // 211
    BattleFx_RunPsyCrystal, // 212
    BattleFx_RunFlash,      // 213
    BattleFx_RunTorch,      // 214
    BattleFx_RunWard,       // 215
    BattleFx_RunZephyr,     // 216
    BattleFx_RunKite,       // 217
    BattleFx_RunBind,       // 218
    BattleFx_RunGust,       // 219
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 220-227
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 228-235
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 236-243
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,                // 244-249
    BattleFx_RunVenus,   // 250
    BattleFx_RunMercury, // 251
    BattleFx_RunMars,    // 252
    BattleFx_RunJupiter, // 253
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 254-261
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 262-269
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 270-277
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 278-285
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 286-293
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,                      // 294-299
    BattleFx_RunSonicWave,     // 300
    BattleFx_RunShriek,        // 301
    BattleFx_RunBansheeHowl,   // 302
    BattleFx_RunCrazyVoice,    // 303
    BattleFx_RunWarCry,        // 304
    BattleFx_RunWickedHowl,    // 305
    BattleFx_RunWingBeat,      // 306
    BattleFx_RunWingFlutter,   // 307
    BattleFx_RunRumble,        // 308
    BattleFx_RunBoneChiller,   // 309
    BattleFx_RunGust,          // 310
    BattleFx_RunBoneCharge,    // 311
    BattleFx_RunMysticFlame,   // 312
    BattleFx_RunNumbingSting,  // 313
    BattleFx_RunBruteForce,    // 314
    BattleFx_RunStickyGoo,     // 315
    BattleFx_RunCannibalFang,  // 316
    BattleFx_RunBearClaw,      // 317
    BattleFx_RunPoisonousBite, // 318
    BattleFx_RunFlyingAttack,  // 319
    BattleFx_RunUndeadSword,   // 320
    BattleFx_RunRansack,       // 321
    BattleFx_RunStickyPoison,  // 322
    BattleFx_RunPoisonousBite, // 323
    BattleFx_RunElectricBite,  // 324
    BattleFx_RunPoisonTail,    // 325
    BattleFx_RunOnslaught,     // 326
    BattleFx_RunVampiricFang,  // 327
    BattleFx_RunBacteriaRush,  // 328
    BattleFx_RunFlyingAttack,  // 329
    BattleFx_RunRottenBlood,   // 330
    NULL,                      // 331
    BattleFx_RunDoubleFang,    // 332
    BattleFx_RunMortalBlow,    // 333
    BattleFx_RunFreebiteRush,  // 334
    BattleFx_RunTwinBeaks,     // 335
    BattleFx_RunRabidFang,     // 336
    BattleFx_RunAcidBite,      // 337
    BattleFx_RunDynamite,      // 338
    BattleFx_RunHeadbutt,      // 339
    BattleFx_RunPoisonInk,     // 340
    NULL,                      // 341
    BattleFx_RunBearClaw,      // 342
    BattleFx_RunRansack,       // 343
    BattleFx_RunSoothingStar,  // 344
    BattleFx_RunSpiderWeb,     // 345
    BattleFx_RunHeartrender,   // 346
    BattleFx_RunMadSpatter,    // 347
    BattleFx_RunSpasm,         // 348
    BattleFx_RunSleepStar,     // 349
    BattleFx_RunDecompose,     // 350
    BattleFx_RunHaunting,      // 351
    BattleFx_RunCure,          // 352
    BattleFx_RunBerserk,       // 353
    BattleFx_RunCurse,         // 354
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 355-362
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 363-370
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,                              // 371-378
    NULL,                              // 379
    BattleFx_RunSmokeBomb,             // 380
    BattleFx_RunWingStroke,            // 381
    BattleFx_RunPoisonBeat,            // 382
    BattleFx_RunSpinningBeat,          // 383
    BattleFx_RunOuterSpace,            // 384
    BattleFx_RunDragonDriver,          // 385
    BattleFx_RunDrainFang,             // 386
    BattleFx_RunSparkApproachVariant3, // 387
    BattleFx_RunThrash,                // 388
    BattleFx_RunRecovery,              // 389
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 390-397
    NULL,
    NULL,                      // 398-399
    BattleFx_RunFireBlessing,  // 400
    BattleFx_RunWaterBlessing, // 401
    BattleFx_RunIceBlessing,   // 402
    BattleFx_RunDarkBlessing,  // 403
    BattleFx_RunAcidBlessing,  // 404
    BattleFx_RunStormBlessing, // 405
    BattleFx_RunEvilBlessing,  // 406
    BattleFx_RunDeadlyGas      // 407
};

// Rock and ice scene geometry.
// Poses of objects 3 and 4 while they wave, three pairs.
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
u16 gCybele_ChipOffsets[] = {3920, 3932, 3947, 3955, 3970};
u8 gCybele_ChipWidths[] = {2, 5, 2, 5, 3};
u8 gCybele_ChipHeights[] = {6, 3, 4, 3, 3};
// The five frames of a spark, square.
u16 gCybele_SparkOffsets[] = {3979, 4043, 4268, 4389, 4470};
u8 gCybele_SparkSizes[] = {8, 15, 11, 9, 5};
// The ember cells of the blast sheet, square. Only the last, largest one is
// drawn.
u16 gCybele_EmberOffsets[] = {5760, 6016, 7040, 9344, 11648, 13952, 16256};
u16 gCybele_EmberSizes[] = {16, 32, 48, 48, 48, 48, 48};
// Rock and ice scene: where the rocks stand (and later the boulders, 56 pixels
// further left), by the bottom-left corner.
u8 gBoreas_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
                          {86, 126}, {85, 98},  {64, 98},  {98, 96},
                          {77, 87},  {84, 88},  {68, 67},  {81, 49},
                          {94, 70},  {70, 51},  {93, 36},  {81, 50}};
// The backdrop's jolt on frames 176 to 179.
s8 gBoreas_ShakeOffsets[] = {-4, -3, -2, 0};
// The first half's three rock pictures, drawn standing on their points.
u16 gBoreas_RockCells[] = {0, 1152, 2064};
u8 gBoreas_RockWidths[] = {24, 24, 20};
u8 gBoreas_RockHeights[] = {48, 38, 26};
// The third object's animations from frame 209, one every 4 frames.
u8 gBoreas_Animations[] = {2, 9, 10, 11};
// Rock and ice scene: the chips thrown up as the rocks crumble: four large
// ones, then from frame 192 seven small ones.
u8 gBoreas_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};
u8 gBoreas_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};
u16 gBoreas_DebrisCells[] = {2584, 2704, 2880, 2992, 5062, 5094,
                             5142, 5157, 5185, 5227, 5299};
u8 gBoreas_SprayWidths[] = {16, 16, 16};
u8 gBoreas_SprayHeights[] = {16, 16, 16};
// Rock and ice scene: the three 16x16 spray images.
u16 gBoreas_SprayCells[] = {4294, 4550, 4806};
// The second half's boulders (56 pixels left of the rock points) and the
// chunks they crumble into: the same three pictures as the rocks.
u16 gBoreas_BoulderCells[] = {0, 1152, 2064};
u8 gBoreas_BoulderWidths[] = {24, 24, 20};
u8 gBoreas_BoulderHeights[] = {48, 38, 26};
u8 gBoreas_FallWidths[] = {24, 24, 20};
u8 gBoreas_FallHeights[] = {48, 38, 26};
u16 gBoreas_FallCells[] = {0, 1152, 2064};
// Ice shards: shard i falls with gravity i & 3.
s32 gBreak_Gravities[] = {0x4000, 0x2000, 0x4000, 0x8000};

// Stat-reduction shard geometry.
// The twelve shard cells, three frames for each of four shapes.
u16 gStatReduction_ShardCells[] = {0,   120, 180, 258, 298, 352,
                                   408, 471, 567, 607, 631, 661};
u8 gStatReduction_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};
u8 gStatReduction_ShardHeights[] = {12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5};

// Rock wall timing, by variant: frames in the effect, frames each pillar
// stands, and how far the screen shakes.
u8 gQuake_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

// Homing ember counts and flare geometry.
// Embers by variant.
u8 gFire_Counts[] = {16, 32, 64};
// The impact flare pictures' sizes and y offsets.
u8 gFire_FlareWidths[] = {24, 29, 30, 27, 24, 16};
u8 gFire_FlareHeights[] = {23, 32, 37, 49, 53, 52};
u8 gFire_FlareBiasY[] = {41, 32, 26, 11, 4, 2};
// The six pictures of an impact flare.
u16 gFire_FlareCells[] = {0, 552, 1480, 2590, 3913, 5185};

// Rising mote geometry and timing.
// Where each ice column stands: x, then the y of its foot.
u8 gRisingMotes_ColumnSpots[][2] = {
    {53, 108}, {73, 110}, {29, 110}, {73, 108}, {33, 104}, {96, 106}, {8, 109},
    {62, 98},  {33, 100}, {87, 100}, {21, 95},  {53, 90},  {33, 90},  {72, 91},
    {82, 91},  {77, 86},  {52, 84},  {18, 92},  {56, 81},
};
// Motes, ice columns and the burst frame, for each variant.
u8 gRisingMotes_Timings[] = {32, 8, 85, 48, 12, 97, 64, 19, 109};
// The eight mote pictures.
u8 gRisingMotes_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
u8 gRisingMotes_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
u16 gRisingMotes_MoteCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};
// The three ice column pictures, 32 wide.
u16 gRisingMotes_ColumnCells[] = {0, 768, 1280};
u8 gRisingMotes_ColumnHeights[] = {24, 16, 16};
// The eight chip pictures (the same as the motes).
u8 gRisingMotes_ChipWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
u8 gRisingMotes_ChipHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
u16 gRisingMotes_ChipCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

// Closing bolt sparks by variant.
// Sparks by variant; [1] is how many each bolt throws a frame.
u8 gRay_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

// Eruption counts and puff layouts.
// Motes, puffs of smoke and frames, for each variant.
u8 gEruptionEffect_Counts[][3] = {{16, 8, 72}, {32, 16, 80}, {128, 32, 88}};
// The seven puff pictures, square.
u16 gEruptionEffect_PuffCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
u16 gEruptionEffect_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// Unreferenced cell layouts, from 6x6 through 20x20
u16 gUnused_SheetCells6[] = {0, 36, 100, 200, 344, 540};
u16 gUnused_SheetCells[] = {0,   36,   100,  200,  344, 540,
                            796, 1120, 1520, 1900, 2300};
u8 gUnused_SheetWidths[] = {6, 8, 10, 12, 14, 16, 18, 20, 19, 20};
u8 gUnused_SheetHeights[] = {6, 8, 10, 12, 14, 16, 18, 20, 20, 20};

// Staged particle scale: Atalanta's formation objects keep this scale, one
// to one, throughout.
struct SpriteScale gStagedParticles_UnitScale = {{{0x10000, 0x10000}}};

// Growing vine and pillar geometry.
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
// Each vine's root, across from the middle of the targets.
s8 gVine_ColumnOffsets[] = {0,   14, -32, 8,   32, -48, 4,   12,
                            -24, 24, -12, -40, -4, -16, -32, 12};
// Each thorn's kind (size, and mirrored when odd) and root.
u8 gThorn_Kinds[] = {0, 3, 2, 1, 2, 3, 2, 1};
s8 gThorn_X[] = {24, 80, 24, 48, 88, 40, 8, 96};
// Thorns by variant.
u8 gThorn_Counts[] = {3, 5, 8};
// The seven pictures of a puff of dust.
s8 gThorn_PuffWidths[] = {5, 7, 11, 17, 32, 32, 19};
u8 gThorn_PuffHeights[] = {10, 14, 21, 34, 59, 60, 38};
u16 gThorn_PuffCells[] = {5888, 5938, 6036, 6267, 6845, 8733, 10653};

// Lightning bolt counts and glint palettes. By variant: sparks each bolt
// throws, glints a frame around each target, how far the glints spread,
// and bolts drawn over each other.
u8 gLightningBolts_Counts[] = {16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3};
// The flips a glint may be drawn in, and its blend by variant.
u8 gLightningBolts_GlintFlips[] = {0, 4, 8, 12};
u8 gLightningBolts_GlintModes[] = {2, 2, 3};

// Projectile volley modes and picture cells.
// Comet picture heights, by comet kind.
u8 gVolley_CometHeights[] = {24, 36, 48};
u8 gVolley_Modes[][5] = {
    // VOLLEY_SHOTS, _STEPS, _STAGGER, _DELAY, _SPARKS
    {6, 10, 3, 32, 4},  // mode 0
    {4, 10, 5, 32, 8},  // mode 1
    {10, 32, 3, 32, 4}, // mode 2
    {5, 10, 5, 32, 4},  // mode 3
    {10, 10, 1, 32, 4}, // mode 4
    {1, 5, 0, 32, 80},  // mode 5
    {7, 8, 2, 32, 40},  // mode 6
    {30, 24, 1, 32, 4}, // mode 7
    {6, 2, 3, 32, 4},   // mode 8
    {60, 10, 1, 32, 4}, // mode 9
    {60, 10, 1, 32, 4}, // mode 10
    {10, 10, 1, 32, 4}, // mode 11
};
// Hit flash pictures, by step (flash frame / 2).
u8 gVolley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};
u8 gVolley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};
u8 gVolley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};
u8 gVolley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12};
u16 gVolley_FlashOffsets[] = {8192, 8312, 8582, 9014, 9674, 10646, 11738};
// Ice chip pictures, by shot index & 3.
u16 gVolley_PelletOffsets[] = {2308, 2372, 2436, 2500};

// Mercury Djinni flame launch frames.
// The frames on which the Djinni fires each of its five flames.
u8 gMercury_LaunchFrames[] = {4, 7, 10, 21, 34};

// Particle reveal and rock toss chips.
// The four ice chip cells.
u8 gHail_CellWidths[] = {8, 8, 8, 8};
u8 gHail_CellHeights[] = {8, 8, 8, 8};
u16 gHail_CellSourceOffsets[] = {2308, 2372, 2436, 2500};
// The composite flips a chip may take.
u8 gGround_ChipFlips[] = {3, 7, 11, 15};
// The rock (cell 0) and its chips in the portal sheet: size, where each begins
// in the sheet, and its offset from the rock's corner.
u8 gGround_ChipWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};
u8 gGround_ChipHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};
u16 gGround_ChipCells[] = {0, 1152, 1243, 1341, 1533, 1677, 1898, 2098, 2252};
u8 gGround_ChipX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};
u8 gGround_ChipY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

// Storm scene colours and shattering rocks.
// The colours a target flashes when a bolt strikes it, chosen at random:
// white, black, red, or with its colour channels rotated.
u8 gThor_TargetPalettes[] = {7, 14, 10, 4};
// Flip flags for the smoke puffs, chosen at random.
u8 gThor_SmokeFlips[] = {0, 4, 8, 12};
// Where each of a rock's 21 shards starts, x then y, before the rock's
// offset across.
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

// Burst scene records and rising wall geometry.
u8 gBurstScene_Records[] = {
    // BURST_ARC_SHEET, _PALETTE, _STRIKES, _MOTES, _INTERVAL, _FRAMES, _FLASH
    1, 0, 5, 16, 12, 80,  0, // variant 0
    1, 1, 7, 16, 12, 104, 0, // variant 1
    1, 2, 9, 16, 12, 128, 0, // variant 2
    1, 3, 1, 16, 16, 36,  0, // variant 3
    1, 3, 2, 20, 16, 52,  0, // variant 4
    1, 3, 3, 24, 16, 68,  0, // variant 5
    0, 2, 1, 32, 12, 28,  1, // variant 6
    0, 2, 2, 32, 12, 40,  1, // variant 7
    0, 2, 3, 32, 12, 52,  1, // variant 8
};
// Rising wall: the nine rows of the growing wall in the rebuilt sheet.
u16 gGaia_RowSheetOffsets[] = {0, 160, 352, 640, 1024, 1504, 2080, 2752, 3776};
u8 gGaia_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};
// The swelling ground's three pictures, after the rebuilt rows; they share
// the earth wall's sizes.
u16 gGaia_MoundSheetOffsets[] = {10672, 11505, 12550};
u8 gGaia_MoundWidths[] = {17, 19, 20};
u8 gGaia_MoundHeights[] = {49, 55, 64};
// BG2's x shift, by side and variant.
s8 gGaia_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};
// The columns rock particles[16] to particles[21] take.
u8 gGaia_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

// Beam sequence picture cells and reach.
// The six cells of the growing beam picture, and how far each reaches short of
// the user. The Lost Age reads the same tables.
u8 gBeamSequence_CellWidths[] = {38, 54, 71, 74, 77, 65};
u8 gBeamSequence_CellHeights[] = {7, 9, 11, 13, 13, 11};
u16 gBeamSequence_CellSheetOffsets[] = {0, 266, 752, 1533, 2495, 3496};
u8 gBeamSequence_CellReaches[] = {58, 42, 25, 13, 5, 0};

// Casting impact glints and orbit cells.
// The flips a glint may take, picked at random.
u8 gCastingImpact_GlintDrawFlags[] = {0, 4, 8, 12};
// Where each picture of the counter reveal is drawn: seven x positions for each
// side of the field, and seven y positions, from the target's height, for both.
u8 gCastingImpact_ImageX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};
u8 gCastingImpact_ImageY[] = {0, 20, 2, 2, 34, 64, 68};
// The four ice chip cells, 8x8.
u16 gCastingImpact_IceChipCells[] = {2308, 2372, 2436, 2500, 0};

// Fire swirl picture cells.
// The seven cells of the fire swirl, and where each sits from the user.
u8 gHelmSplitter_CellWidths[] = {26, 46, 63, 77, 60, 39, 32};
u8 gHelmSplitter_CellHeights[] = {11, 20, 37, 65, 65, 54, 39};
u16 gHelmSplitter_CellSourceOffsets[] = {0,    286,   1206, 3537,
                                         8542, 12442, 14548};
s8 gHelmSplitter_CellBiasX[] = {53, 36, 16, 2, 2, 2, 2};
s8 gHelmSplitter_CellBiasY[] = {2, 3, 3, 3, 4, 14, 29};

// Object formation coordinates and impact burst cells.
// The offsets of BattleFx_PlaceFormationObjects' formations 1 to 3 in
// pixels, one axis at a time: twelve in rows of 1, 3, 4 and 4; eight in rows
// of 3, 2 and 3; eight drawn at 1.25 times their size. Formation 0, a 3x3
// grid, needs no table.
u8 gFormation_TwelveX[] = {52, 25, 57, 89, 7, 39, 71, 103, 21, 53, 85, 117};
u8 gFormation_TwelveZ[] = {0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96};
u8 gFormation_RightEightX[] = {0, 32, 64, 16, 48, 0, 32, 64};
u8 gFormation_RightEightZ[] = {0, 0, 0, 32, 32, 64, 64, 64};
u8 gFormation_SmallEightX[] = {0, 40, 80, 0, 40, 80, 40, 80};
u8 gFormation_SmallEightZ[] = {13, 0, 0, 53, 40, 40, 80, 80};
// The eight cells of the impact flash.
u8 gImpactBurst_CellWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};
u8 gImpactBurst_CellHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};
u16 gImpactBurst_CellSourceOffsets[] = {0,     4608,  9216,  13356,
                                        16020, 18684, 20668, 22020};

// Particle stream paths and circling scene geometry.
// The seven sparks that orbit the rising formation.
s16 gParticleStreams_OrbitPoints[][3] = {
    {-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48}, {-16, -72, -24},
    {16, -80, 16},  {32, -112, -32}, {48, -96, 56}};
// Where the seven drops fall, in pixels.
s16 gParticleStreams_DropPoints[][2] = {
    {32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};
// The seven growing flash cells of the blast sheet, square.
u16 gParticleStreams_FlashSheetOffsets[] = {0,    256,  1280, 3584,
                                            5888, 8192, 10496};
u16 gParticleStreams_FlashSizes[] = {16, 32, 48, 48, 48, 48, 48};
// Where the formation's objects stand, in pixels from its corner.
u8 gRamses_ObjectColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};
u8 gRamses_ObjectRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};
// The puff cells of the blast sheet, square; only the last four are used.
u16 gRamses_PuffSheetOffsets[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
u16 gRamses_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// Flame blade strike columns and flash cells.
// The canvas x of each strike column, three for each side.
u8 gRagnarok_StrikeColumns[] = {32, 24, 0, 96, 104, 128};
// The flash picture for each three frames at the end of a flash's life.
u8 gRagnarok_FlashCells[] = {5, 5, 4, 3, 2, 1};

// Falling sword flashes, dust and shockwave debris.
// The flash picture for each three frames at the end of a flash's life.
u8 gTitanBlade_FlashCells[] = {5, 5, 4, 3, 2, 1};
s32 gTitanBlade_DustGravity[] = {0x8000, 0x4000, 0x4000, 0x2000};
// The burst's seven quarter cells, each half as wide as it is tall.
u8 gShockwaveScene_BurstHeights[] = {2, 3, 4, 6, 10, 11, 12};
u16 gShockwaveScene_BurstCells[] = {0, 8, 26, 58, 130, 330, 572};
// The shard cells thrown from the burst.
u16 gShockwaveScene_ShardCells[] = {2396, 2985, 3698, 4411, 4810, 5146};
u8 gShockwaveScene_ShardWidths[] = {19, 23, 23, 19, 16, 17};
u8 gShockwaveScene_ShardHeights[] = {31, 31, 31, 21, 21, 24};

// Object row coordinates.
// Object row: pixel offsets of its nine objects.
u8 gKirin_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};
u8 gKirin_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

// Formation sweep coordinates and gravity.
// Formation sweep: pixel offsets of its eight objects, and spray gravity.
u8 gNeptune_ObjectX[] = {0, 32, 64, 0, 32, 64, 96, 96};
u8 gNeptune_ObjectY[] = {0, 0, 0, 32, 32, 32, 0, 32};
s32 gNeptune_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// Rising burst spark cells.
// Rising burst: sheet offsets and sizes of the seven spark images.
u16 gUndeadSword_SparkCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
u16 gUndeadSword_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};
