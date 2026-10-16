#include "unit/effect/effect_table.h"
#include "unit/effect/effect.h"
#include "types.h"

//--------------------------------------------------------------------------
// Battle effect tables
// Ordered like the effect code that uses them. Cell offsets are bytes from
// the sheet start, accumulated as width times height.
// Battle effect tables, in the order of the effect code that reads them.
//--------------------------------------------------------------------------

// Consumers declare these without const to preserve matching code.

struct BattleEffectArgument;

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

typedef void (*BattleFxHandler)(struct BattleEffectArgument *effect);

typedef struct Scale {
  s32 x;
  s32 y;
} Scale;

// Shared cell layouts
const u16 gParticleStreams_CellOffsets[] = {0,   2,   10,  28,  60,
                                            110, 182, 280, 408, 570};
const u16 gBattleFx6_FlareCells[] = {0,   4,   20,   56,   120,  220, 364,
                                     560, 816, 1140, 0,    8,    40,  112,
                                     240, 440, 728,  1120, 1632, 2280};
const u16 gBattleFx_PuffCells[] = {0, 25, 74, 123, 172, 221, 246, 255, 256};
const u8 gBattleFx_PuffSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};
const u8 gPuffArc_CellWidths[] = {24, 29, 30, 27, 24, 16};
const u8 gPuffArc_CellHeights[] = {23, 32, 37, 49, 53, 52};
const u8 gPuffArc_CellBiasY[] = {41, 32, 26, 11, 4, 2};
const u16 gPuffArc_CellSourceOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
const u16 gBattleFx_GlintCellOffsets[] = {0, 589, 1302, 2015, 2414, 2750};
const u8 gBattleFx_GlintCellWidths[] = {19, 23, 23, 19, 16, 17};
const u8 gBattleFx_GlintCellHeights[] = {31, 31, 31, 21, 21, 24};

// falling.c
const u8 gFallingShards_Counts[] = {16, 46, 20, 52, 32, 80};
const u8 gFallingBolts_Counts[] = {6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80};

// run_five_mode_mode0.c
const s8 gFiveMode_Records[] = {0,  1, 0, 0, 0,  1, 4, 0, 0, 1,
                                -1, 1, 1, 1, -1, 1, 1, 1, 4, 0};
const s8 gFiveMode_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

// twelve_mode.c
const u8 gTwelveMode_Records[] = {
    1, 2, 8, 0, 6, 3, 70, 1, 1, 1, 1, 6, 2, 54, 1, 3, 1,  2, 6, 2, 96,
    1, 3, 2, 3, 4, 2, 96, 1, 3, 2, 4, 4, 2, 96, 1, 3, 4,  5, 4, 3, 96,
    0, 1, 4, 5, 4, 3, 38, 0, 2, 8, 5, 2, 3, 48, 0, 1, 24, 4, 2, 3, 50,
    0, 1, 1, 1, 2, 3, 46, 1, 1, 1, 4, 6, 2, 54, 1, 3, 2,  4, 4, 3, 96};
const u8 gTwelveMode_StrikeWidths[] = {38, 54, 71, 74, 77, 65};
const u8 gTwelveMode_StrikeHeights[] = {7, 9, 11, 13, 13, 11};
const u16 gTwelveMode_StrikeOffsets[] = {3200, 3466, 3952, 4733, 5695, 6696};
const u8 gTwelveMode_StrikeReach[] = {58, 42, 25, 13, 5, 0};
const u8 gTwelveMode_ColumnStages[] = {0, 1, 2, 3, 2};
const u8 gTwelveMode_GlintDrawFlags[] = {0, 4, 8, 12};

// flipping_burst.c
const u8 gBladeRain_CellWidths[] = {20, 21, 22, 25};
const u8 gBladeRain_CellHeights[] = {32, 31, 27, 24};
const u16 gBladeRain_CellSourceOffsets[] = {0, 640, 1291, 1885};
const u8 gBattlePieceWidth[] = {21, 20, 24, 30, 35, 69, 19, 23, 9,  22, 33,
                                10, 18, 9,  7,  9,  17, 55, 30, 25, 40, 29,
                                10, 28, 14, 5,  32, 23, 19, 32, 8,  43, 15};
const u8 gBattlePieceHeight[] = {23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35,
                                 17, 18, 14, 15, 12, 13, 37, 38, 41, 60, 41,
                                 15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11};
const u16 gBattlePieceOffset[] = {
    0,     483,   1423,  2023,  3223,  3923,  7580,  8112,  8779,
    8986,  9294,  10449, 10619, 10943, 11069, 11174, 11282, 11503,
    13538, 14678, 15703, 18103, 19292, 19442, 20702, 21528, 21733,
    22565, 23715, 24114, 25234, 25330, 27781, 27946};
const u8 gBattlePieceStartX[] = {0,  21, 28, 46, 70, 51, 0,  13, 37, 77, 87,
                                 19, 25, 35, 44, 45, 48, 65, 0,  0,  0,  5,
                                 33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};
const u8 gBattlePieceStartY[] = {0,  0,  0,  0,  0,  0,  19, 17, 23,  28, 8,
                                 36, 43, 43, 40, 47, 48, 37, 42, 67,  60, 79,
                                 62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

// canvfx.c
const s8 gCornerSparks_X[] = {0, 32, 0, 32};
const s8 gCornerSparks_Y[] = {0, 0, 32, 32};
const u8 gCornerSparks_DrawFlags[] = {0, 4, 8, 12};
const u8 gTwoResource_CellWidths[] = {40, 112, 112, 112, 88, 64};
const u8 gTwoResource_CellHeights[] = {56, 80, 64, 64, 80, 88};
const u16 gTwoResource_CellSourceOffsets[] = {0, 2240, 11200, 18368, 0, 7040};
const u8 gTwoResource_CellX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};
const s8 gTwoResource_CellBiasY[] = {0, 4, 24, -6, -20, -28, 0, 0};

// memberfx.c
const u8 gMemberBurst_Counts[] = {16, 88, 8, 72, 12, 80};

// earthwal.c
const u16 gEarthWall_CellSourceOffsets[] = {2702, 3535, 4580};
const u8 gEarthWall_CellWidths[] = {17, 19, 20};
const u8 gEarthWall_CellHeights[] = {49, 55, 64};

// member_beam.c
const u8 gMemberBeam_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};
const u16 gMemberBeam_HeadOffsets[] = {2702, 3535, 4580};
const u8 gMemberBeam_HeadWidths[] = {17, 19, 20};
const u8 gMemberBeam_HeadHeights[] = {49, 55, 64};

// seven_mode_dispatch.c
const s8 gSevenMode_Records[] = {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
const u8 gSevenMode_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12,
                                     6, 7,  7, 7, 6, 6,  8,  8,  8};
const u8 gSevenMode_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12,
                                      8,  7,  6,  6,  8,  6,  4,  3,  3};
const u16 gSevenMode_ShardOffsets[] = {0,    108,  228,  336,  444,  552,
                                       684,  816,  948,  1092, 1140, 1189,
                                       1231, 1273, 1321, 1357, 1389, 1413};

// reveal.c
const u8 gCounterReveal_PanelX[] = {10, 30, 0, 0, 0, 6, 52,
                                    60, 0,  0, 0, 0, 0, 0};
const u8 gCounterReveal_PanelY[] = {0, 20, 2, 2, 34, 64, 68,
                                    0, 9,  4, 4, 4,  4,  4};

// triangle.c
const struct Vector3 gSpinningTriangle_Vertex[] = {{0, 24, 0}};
const struct Vector3 gTriangleStrike_Vertex[] = {{0, 24, 0}};

// stars.c
const struct Vector3 gRingBolts_Points[] = {{0, 48 << 16, 0}, {0, 28 << 16, 0}};
const u8 gSpinningStars_Radii[] = {32, 16};
const u8 gWhirlwindComets_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};
const u8 gWhirlwindComets_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};
const u16 gWhirlwindComets_Frames[] = {334, 347, 359, 372};
const u8 gWhirlwindComets_CellWidths[] = {14, 24, 48};
const u8 gWhirlwindComets_CellHeights[] = {14, 19, 24};
const u8 gWhirlwindComets_CellBiasY[] = {8, 12, 17};
const u16 gWhirlwindComets_CellOffsets[] = {13824, 14020, 14476};

// embers.c
const u8 gPaletteParticles_DropSpeeds[] = {0, 8, 12, 16};
const u8 gPaletteParticles_SparkPixels[] = {32, 32};
const u8 gPaletteParticles_DropSizes[] = {1, 1, 2, 3};
const u8 gPaletteParticles_StreakWidths[] = {24, 29, 30, 27, 24, 16};
const u8 gPaletteParticles_StreakHeights[] = {23, 32, 37, 49, 53, 52};
const u8 gPaletteParticles_StreakDrops[] = {41, 32, 26, 11, 4, 2};
const u16 gPaletteParticles_StreakOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
const u8 gEmberColumns_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104};
const s32 gEmberColumns_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// vortex.c
const u8 gVortexMotes_Counts[] = {6, 140, 12, 160, 30, 200};
const u8 gTornado_Shapes[] = {1, 16, 2, 2, 24, 2, 4, 32, 4};

// crystal.c
const u8 gCrystal_ShardStarts[] = {
    32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99, 54, 69, 34, 82, 45,
    71, 31, 91, 32, 69, 26, 80, 40, 68, 41, 60, 46, 60, 36, 63, 0,  0};
const u8 gCrystal_Counts[] = {1, 110, 4, 134, 12, 166};
const u8 gCrystal_ShardWidths[] = {5,  10, 11, 14, 12, 11,
                                   19, 18, 23, 12, 22, 22};
const u8 gCrystal_ShardHeights[] = {14, 10, 7,  13, 14, 14,
                                    32, 30, 24, 38, 20, 20};
const s32 gCrystal_ShardOffsets[] = {0,   70,   170,  247,  429,  597,
                                     751, 1359, 1899, 2451, 2907, 3347};

// lightning.c
const u16 gLightningPillar_Sparks[] = {384, 2, 256, 3, 192, 4};
const u8 gLightningPillar_Columns[] = {32, 64, 16,  48, 72, 96, 64,
                                       88, 56, 104, 72, 48, 24, 56};
const u8 gLightningPillar_Counts[] = {1, 3, 7};

// sparks.c
const u16 gSparkGroups_Shapes[] = {128, 1,   16, 0, 0,  255, 1,   16, 0,
                                   0,   192, 3,  0, 24, 48,  340, 1,  16,
                                   24,  48,  32, 1, 16, 0,   0};
const u8 gSparkGroups_FlashCells[] = {5, 5, 4, 3, 2, 1};

// pillars.c
const u8 gFirePillars_Counts[] = {1, 2, 4};
const s8 gFirePillars_Depths[][4] = {
    {0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};
const u8 gFirePillars_StartFrames[] = {8, 18, 24, 30, 34};

// render_mode.c
const u8 gRenderMode_GlintFlips[] = {0, 4, 8, 12};

// dispatch.c
// Handler for each effect kind minus 1
const BattleFxHandler gBattleFx_ModeHandlers[] = {
    BattleFx_RunRockWall,               // 1
    BattleFx_RunShatteringRocks,        // 2
    BattlePres_RunRisingWall,           // 3
    BattleFx_RunDualTableMode0,         // 4
    BattleFx_RaisePillars,              // 5
    BattleFx_RunRisingMotes,            // 6
    BattleEffect_RunFallingParticles,   // 7
    BattleFx_RunFallingCrystals,        // 8
    BattleEffect_RunFallingBolts,       // 9
    BattleFx_RunVortexMotes,            // 10
    BattleFx_RunPuffArc,                // 11
    BattleFx_RunHomingEmbers,           // 12
    BattleFx_RunFirePillars,            // 13
    BattleFx_RunParticleFieldMode0,     // 14
    BattleFx_RunSparkBursts,            // 15
    BattleFx_RunLightningBolts,         // 16
    BattleFx_RunClosingBolts,           // 17
    BattleFx_RunLightningPillars,       // 18
    BattleFx_RunMode0,                  // 19
    BattleFx_RunTornado,                // 20
    BattleFx_RunSeriesAMode2,           // 21
    BattleFx_RunSeriesAMode0,           // 22
    BattleFx_RunSeriesAMode1,           // 23
    BattleFx_RunSeriesAMode3Or4,        // 24
    BattleFx_RunVenusDjinnBeams,        // 25
    BattleFx_RunSeriesBMode0,           // 26
    BattleFx_RunPaletteRampMode0,       // 27
    BattleFx_RunSeriesBMode1,           // 28
    BattleFx_RunPaletteRampMode1,       // 29
    BattleFx_RunSeriesBMode2,           // 30
    BattleFx_RunPaletteRampMode2,       // 31
    BattleFx_RenderMode0Or1,            // 32
    BattleFx_RenderMode2,               // 33
    BattleFx_RunOrbitingSpirals,        // 34
    BattleFx_RenderMode4,               // 35
    BattleFx_RenderMode3,               // 36
    BattleFx_RunBouncingSheep,          // 37
    BattleFx_RunSpinningTriangle,       // 38
    BattleFx_RunClosingSkulls,          // 39
    BattleFx_RunShatteringPanel,        // 40
    BattleFx_RunTargetMarkers,          // 41
    BattleFx_RunGatheringMotes,         // 42
    BattleFx_RunIceShardBursts,         // 43
    BattleFx_RunSeriesAMode1,           // 44
    BattleFx_RunSeriesAMode1,           // 45
    BattleFx_RunSeriesBMode0,           // 46
    BattleFx_RunPaletteRampMode1,       // 47
    NULL,                               // 48
    BattleEffect_RunDualParticleStream, // 49
    BattleFx_RunPinkGatheringLights,    // 50
    BattleFx_RunFireSwirlCharge,        // 51
    BattleFx_RunRevealColumnMode1,      // 52
    BattleFx_RunFlippingBurst,          // 53
    BattleFx_RunMode1,                  // 54
    BattleFx_RunFireBurstShards,        // 55
    BattleFx_RunDualTableMode1,         // 56
    BattleEffect_RunMode5WithAction,    // 57
    BattleFx_RunFlameBlade,             // 58
    BattleFx_RunMode6WithAction,        // 59
    BattleFx_RunLeapingStrike,          // 60
    BattleFx_RunFallingSword,           // 61
    BattleFx_RunSpinningStars,          // 62
    BattleFx_RunMode9WithAction,        // 63
    BattleFx_RenderMode7,               // 64
    BattleFx_RunRevealColumnMode2,      // 65
    BattleFx_RenderMode6,               // 66
    BattleFx_RunTriangleStrike,         // 67
    BattleFx_RunRingBolts,              // 68
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
    NULL,                               // 197-199
    BattleFx_RunSeriesBMode3,           // 200
    BattleFx_RunVenusDjinnBeams,        // 201
    BattleEffect_RunRisingColumns,      // 202
    BattleFx_RunVenusDjinnRockToss,     // 203
    BattleFx_RunSeriesAMode0,           // 204
    BattleFx_RunMode7,                  // 205
    BattleFx_RunSeriesAMode1,           // 206
    BattleFx_RunParticleReveal,         // 207
    BattleFx_RunSeriesAMode5,           // 208
    BattleFx_RunMercuryDjinnBeams,      // 209
    BattleFx_RunSeriesBMode0,           // 210
    BattleFx_RunSeriesBMode1,           // 211
    BattleFx_RunSeriesAMode6,           // 212
    BattleFx_RunSeriesBMode5,           // 213
    BattleFx_RunRipplingDisc,           // 214
    BattleFx_RunSeriesBMode2,           // 215
    BattleFx_RunSeriesBMode4,           // 216
    ObjectGroup_StoreObjectAndRunStep7, // 217
    BattleFx_RunSpinningTriangle,       // 218
    EffectStep_RunAnimationMode6Or7Or8, // 219
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
    NULL,                           // 244-249
    BattleFx_RunVenusDjinnColumns,  // 250
    BattleFx_RunMercuryDjinnFlames, // 251
    BattleFx_RunMarsDjinnPuffs,     // 252
    BattleFx_RunJupiterDjinnFall,   // 253
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
    NULL,                                  // 294-299
    BattleFx_RunEmberStreaks,              // 300
    BattleFx_RunLimeStreaks,               // 301
    BattleFx_RunVioletStreaks,             // 302
    BattleFx_RunMode2,                     // 303
    BattleFx_RunCyanStreaks,               // 304
    BattleFx_RunRedStreaks,                // 305
    BattleFx_RunMode4,                     // 306
    BattleFx_RunMode3,                     // 307
    BattleFx_RunMarsGatheringLights,       // 308
    BattleFx_RunBlueShells,                // 309
    EffectStep_RunAnimationMode6Or7Or8,    // 310
    BattleFx_RunParticleFieldVariant0,     // 311
    BattleFx_RenderMode5,                  // 312
    BattleFx_RunLightningGlintSlash,       // 313
    BattleFx_RunParticleFieldVariant1,     // 314
    BattleFx_RunWebShells,                 // 315
    BattleFx_RunEmberDrainColumns,         // 316
    EffectStep_RunAnimationMode0,          // 317
    BattleFx_RunLimeSmokeColumn,           // 318
    EffectStep_RunAnimationMode3Or4Or5,    // 319
    BattleFx_RunRisingBurst,               // 320
    EffectStep_RunAnimationMode0Alternate, // 321
    BattleFx_RunLimeShells,                // 322
    BattleFx_RunLimeSmokeColumn,           // 323
    BattleFx_RunLightningGlintColumns,     // 324
    BattleFx_RunLimeSmokeSlash,            // 325
    BattleFx_RunParticleFieldVariant2,     // 326
    BattleFx_RunMarsDrainColumns,          // 327
    EffectStep_RunAnimationMode1,          // 328
    EffectStep_RunAnimationMode3Or4Or5,    // 329
    BattleFx_RunGreenShardShells,          // 330
    NULL,                                  // 331
    BattleFx_RunBlueSparkColumns,          // 332
    BattleFx_RunHeavyMarsSlash,            // 333
    EffectStep_RunAnimationMode2,          // 334
    BattleFx_RunLightningSparkSlashes,     // 335
    BattleFx_RunMarsSmokeColumn,           // 336
    BattleFx_RunPinkSmokeColumns,          // 337
    BattleFx_RunOrangeSparkBursts,         // 338
    BattleFx_RunParticleFieldVariant3,     // 339
    BattleFx_RunMarsShells,                // 340
    NULL,                                  // 341
    EffectStep_RunAnimationMode0,          // 342
    EffectStep_RunAnimationMode0Alternate, // 343
    BattleFx_RunHealingMoteFountain,       // 344
    BattleFx_RunSpiderWeb,                 // 345
    BattleFx_RenderMode4,                  // 346
    BattleFx_RunGoldShardShells,           // 347
    BattleFx_RunShakingWall,               // 348
    BattleFx_RunDriftingMoteFountain,      // 349
    BattleFx_RunGreenShells,               // 350
    BattleFx_RunMercuryGatheringLights,    // 351
    BattleFx_RunSeriesAMode2,              // 352
    BattleFx_RunWideWall,                  // 353
    BattleFx_RunTargetMarkers,             // 354
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
    BattleFx_RunParticleFieldMode1,    // 380
    BattleFx_RunMode11,                // 381
    BattleFx_RunLimeSwipe,             // 382
    BattleFx_RunLaunchingSwipe,        // 383
    BattleFx_InitializeMode1,          // 384
    BattleFx_RunSideEmbers,            // 385
    BattleFx_RunLargeDrainColumns,     // 386
    BattleFx_RunSparkApproachVariant3, // 387
    BattleFx_RunSparkApproachVariant4, // 388
    BattleFx_RunWallOfFire,            // 389
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, // 390-397
    NULL,
    NULL,                     // 398-399
    BattleFx_RunSeriesIMode0, // 400
    BattleFx_RunSeriesIMode2, // 401
    BattleFx_RunMode10,       // 402
    BattleFx_RunSeriesIMode6, // 403
    BattleFx_RunSeriesIMode3, // 404
    BattleFx_RunSeriesIMode5, // 405
    BattleFx_RunSeriesIMode7, // 406
    BattleFx_RunSeriesIMode4  // 407
};

// initialize_mode10.c
const u8 gThornVines_ObjectPoses[] = {8, 10, 9, 11, 3, 4};
const u16 gThornVines_ThornOffsets[] = {3920, 3932, 3947, 3955, 3970};
const u8 gThornVines_ThornWidths[] = {2, 5, 2, 5, 3};
const u8 gThornVines_ThornHeights[] = {6, 3, 4, 3, 3};
const u16 gThornVines_LeafOffsets[] = {3456, 3712, 3856};
const u8 gThornVines_LeafSizes[] = {16, 12, 8};
const u16 gThornVines_ChipOffsets[] = {3920, 3932, 3947, 3955, 3970};
const u8 gThornVines_ChipWidths[] = {2, 5, 2, 5, 3};
const u8 gThornVines_ChipHeights[] = {6, 3, 4, 3, 3};
const u16 gThornVines_SparkOffsets[] = {3979, 4043, 4268, 4389, 4470};
const u8 gThornVines_SparkSizes[] = {8, 15, 11, 9, 5};
const u16 gThornVines_EmberOffsets[] = {5760,  6016,  7040, 9344,
                                        11648, 13952, 16256};
const u16 gThornVines_EmberSizes[] = {16, 32, 48, 48, 48, 48, 48};
const u8 gBattleFx10_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
                                    {86, 126}, {85, 98},  {64, 98},  {98, 96},
                                    {77, 87},  {84, 88},  {68, 67},  {81, 49},
                                    {94, 70},  {70, 51},  {93, 36},  {81, 50}};
const s8 gBattleFx10_ShakeOffsets[] = {-4, -3, -2, 0};
const u16 gBattleFx10_RockCells[] = {0, 1152, 2064};
const u8 gBattleFx10_RockWidths[] = {24, 24, 20};
const u8 gBattleFx10_RockHeights[] = {48, 38, 26};
const u8 gBattleFx10_Animations[] = {2, 9, 10, 11};
const u8 gBattleFx10_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};
const u8 gBattleFx10_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};
const u16 gBattleFx10_DebrisCells[] = {2584, 2704, 2880, 2992, 5062, 5094,
                                       5142, 5157, 5185, 5227, 5299};
const u8 gBattleFx10_SprayWidths[] = {16, 16, 16};
const u8 gBattleFx10_SprayHeights[] = {16, 16, 16};
const u16 gBattleFx10_SprayCells[] = {4294, 4550, 4806};
const u16 gBattleFx10_BoulderCells[] = {0, 1152, 2064};
const u8 gBattleFx10_BoulderWidths[] = {24, 24, 20};
const u8 gBattleFx10_BoulderHeights[] = {48, 38, 26};
const u8 gBattleFx10_FallWidths[] = {24, 24, 20};
const u8 gBattleFx10_FallHeights[] = {48, 38, 26};
const u16 gBattleFx10_FallCells[] = {0, 1152, 2064};
const s32 gIceShardBursts_Gravities[] = {0x4000, 0x2000, 0x4000, 0x8000};

// palette_ramp.c
const u16 gPaletteRamp_ShardCells[] = {0,   120, 180, 258, 298, 352,
                                       408, 471, 567, 607, 631, 661};
const u8 gPaletteRamp_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};
const u8 gPaletteRamp_ShardHeights[] = {12, 12, 13, 8, 9, 8,
                                        7,  12, 10, 4, 5, 5};

// rockwall.c
const u8 gRockWall_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

// homing.c
const u8 gHomingEmbers_Counts[] = {16, 32, 64};
const u8 gHomingEmbers_FlareWidths[] = {24, 29, 30, 27, 24, 16};
const u8 gHomingEmbers_FlareHeights[] = {23, 32, 37, 49, 53, 52};
const u8 gHomingEmbers_FlareBiasY[] = {41, 32, 26, 11, 4, 2};
const u16 gHomingEmbers_FlareCells[] = {0, 552, 1480, 2590, 3913, 5185};

// motes.c
const u8 gRisingMotes_ColumnSpots[] = {
    53,  108, 73, 110, 29,  110, 73,  108, 33, 104, 96, 106, 8,
    109, 62,  98, 33,  100, 87,  100, 21,  95, 53,  90, 33,  90,
    72,  91,  82, 91,  77,  86,  52,  84,  18, 92,  56, 81};
const u8 gRisingMotes_Timings[] = {32, 8, 85, 48, 12, 97, 64, 19, 109};
const u8 gRisingMotes_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
const u8 gRisingMotes_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
const u16 gRisingMotes_MoteCells[] = {1792, 1900, 2060, 2158,
                                      2308, 2372, 2436, 2500};
const u16 gRisingMotes_ColumnCells[] = {0, 768, 1280};
const u8 gRisingMotes_ColumnHeights[] = {24, 16, 16};
const u8 gRisingMotes_EmberWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
const u8 gRisingMotes_EmberHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
const u16 gRisingMotes_EmberCells[] = {1792, 1900, 2060, 2158,
                                       2308, 2372, 2436, 2500};

// bolts.c
const u8 gLightningBolts_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

// particle_field.c
const u8 gParticleField_Counts[][3] = {
    {16, 8, 72}, {32, 16, 80}, {128, 32, 88}};
const u16 gParticleField_PuffCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
const u16 gParticleField_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// Unreferenced cell layouts, from 6x6 through 20x20
const u16 gUnused_SheetCells6[] = {0, 36, 100, 200, 344, 540};
const u16 gUnused_SheetCells[] = {0,   36,   100,  200,  344, 540,
                                  796, 1120, 1520, 1900, 2300};
const u8 gUnused_SheetWidths[] = {6, 8, 10, 12, 14, 16, 18, 20, 19, 20};
const u8 gUnused_SheetHeights[] = {6, 8, 10, 12, 14, 16, 18, 20, 20, 20};

// staged_particles.c
const Scale gStagedParticles_UnitScale = {0x10000, 0x10000};

// columns.c
const u8 gDualTable_VineWidths[] = {21, 28, 19};
const u8 gDualTable_HeightLimits[] = {119, 119, 102};
const u16 gDualTable_VineCells[] = {0, 2499, 5831};
const u8 gDualTable_PillarWidths[] = {10, 8, 9};
const u16 gDualTable_PillarCells[] = {0, 1080, 1888};
const u8 gDualTable_ColumnCounts[] = {4, 8, 12};
const u8 gDualTable_ColumnX[] = {64, 72,  56, 80, 48, 88, 40, 96,
                                 32, 104, 24, 72, 56, 80, 48, 88};
const s8 gDualTable_PillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};
const s8 gDualTable_PuffWidths[] = {5, 7, 11, 16, 23, 27, 16};
const u8 gDualTable_PuffHeights[] = {10, 14, 21, 32, 45, 50, 32};
const u16 gDualTable_PuffCells[] = {0, 50, 148, 379, 891, 1926, 3276};
const s8 gRisingColumns_ColumnOffsets[] = {0,   14, -32, 8,   32, -48, 4,   12,
                                           -24, 24, -12, -40, -4, -16, -32, 12};
const u8 gBattleFxPillar_Kinds[] = {0, 3, 2, 1, 2, 3, 2, 1};
const s8 gBattleFxPillar_X[] = {24, 80, 24, 48, 88, 40, 8, 96};
const u8 gBattleFxPillar_Counts[] = {3, 5, 8};
const s8 gBattleFxPillar_PuffWidths[] = {5, 7, 11, 17, 32, 32, 19};
const u8 gBattleFxPillar_PuffHeights[] = {10, 14, 21, 34, 59, 60, 38};
const u16 gBattleFxPillar_PuffCells[] = {5888, 5938, 6036, 6267,
                                         6845, 8733, 10653};

// lightning_bolts.c
const u8 gLightningBolts_Counts[] = {16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3};
const u8 gLightningBolts_GlintPalettes[] = {0, 4, 8, 12};
const u8 gLightningBolts_GlintModes[] = {2, 2, 3};

// approach.c
const u8 gVolley_CometHeights[] = {24, 36, 48};
const u8 gVolley_Modes[][5] = {
    {6, 10, 3, 32, 4},  {4, 10, 5, 32, 8},  {10, 32, 3, 32, 4},
    {5, 10, 5, 32, 4},  {10, 10, 1, 32, 4}, {1, 5, 0, 32, 80},
    {7, 8, 2, 32, 40},  {30, 24, 1, 32, 4}, {6, 2, 3, 32, 4},
    {60, 10, 1, 32, 4}, {60, 10, 1, 32, 4}, {10, 10, 1, 32, 4}};
const u8 gVolley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};
const u8 gVolley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};
const u8 gVolley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};
const u8 gVolley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12};
const u16 gVolley_FlashOffsets[] = {8192, 8312, 8582, 9014, 9674, 10646, 11738};
const u16 gVolley_PelletOffsets[] = {2308, 2372, 2436, 2500};

// djinn_bursts.c
const u8 gMercuryDjinnFlames_LaunchFrames[] = {4, 7, 10, 21, 34};

// particle_reveal.c
const u8 gParticleReveal_CellWidths[] = {8, 8, 8, 8};
const u8 gParticleReveal_CellHeights[] = {8, 8, 8, 8};
const u16 gParticleReveal_CellSourceOffsets[] = {2308, 2372, 2436, 2500};
const u8 gRockToss_ChipFlips[] = {3, 7, 11, 15};
const u8 gRockToss_ChipWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};
const u8 gRockToss_ChipHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};
const u16 gRockToss_ChipCells[] = {0,    1152, 1243, 1341, 1533,
                                   1677, 1898, 2098, 2252};
const u8 gRockToss_ChipX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};
const u8 gRockToss_ChipY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

// shatter_rocks.c
const u8 gMode12_Animations[] = {7, 14, 10, 4};
const u8 gMode12_SmokeFlips[] = {0, 4, 8, 12};
const u8 gShatterRocks_ShardOffsets[32 * 2] = {
    65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56, 88,
    51, 80,  61, 75, 72, 81, 69, 71, 66, 70, 71, 64, 49, 71,
    59, 69,  53, 68, 78, 73, 55, 67, 75, 62, 61, 65, 53, 61};
const s8 gShatterRocks_RockX[] = {0, -12, 12, -8, 16};
const u8 gShatterRocks_DropFrames[] = {8, 16, 20, 24, 32};
const u8 gShatterRocks_RockCounts[] = {1, 3, 5};
const u8 gShatterRocks_ShardWidths[] = {5,  11, 10, 7,  7,  12, 13, 9,
                                        19, 12, 17, 22, 15, 16, 14};
const u8 gShatterRocks_ShardHeights[] = {11, 6,  10, 11, 10, 7,  17, 21,
                                         12, 21, 17, 13, 16, 16, 15};
const u16 gShatterRocks_ShardCells[] = {0,    55,   121,  221,  298,  368,
                                        452,  673,  862,  1090, 1342, 1631,
                                        1917, 2157, 2413, 2623};

// burst_scene.c
const u8 gBurstScene_Records[] = {
    1, 0, 5, 16, 12, 80, 0, 1, 1, 7, 16, 12, 104, 0, 1, 2, 9, 16, 12, 128, 0,
    1, 3, 1, 16, 16, 36, 0, 1, 3, 2, 20, 16, 52,  0, 1, 3, 3, 24, 16, 68,  0,
    0, 2, 1, 32, 12, 28, 1, 0, 2, 2, 32, 12, 40,  1, 0, 2, 3, 32, 12, 52,  1};
const u16 gRisingWall_RowSheetOffsets[] = {0,    160,  352,  640, 1024,
                                           1504, 2080, 2752, 3776};
const u8 gRisingWall_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};
const u16 gRisingWall_MoundSheetOffsets[] = {10672, 11505, 12550};
const u8 gRisingWall_MoundWidths[] = {17, 19, 20};
const u8 gRisingWall_MoundHeights[] = {49, 55, 64};
const s8 gRisingWall_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};
const u8 gRisingWall_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

// fxstep.c
const u8 gBeamSequence_CellWidths[] = {38, 54, 71, 74, 77, 65};
const u8 gBeamSequence_CellHeights[] = {7, 9, 11, 13, 13, 11};
const u16 gBeamSequence_CellSheetOffsets[] = {0, 266, 752, 1533, 2495, 3496};
const u8 gBeamSequence_CellReaches[] = {58, 42, 25, 13, 5, 0};

// casting.c
const u8 gCastingImpact_GlintDrawFlags[] = {0, 4, 8, 12};
const u8 gCastingImpact_ImageX[] = {10, 30, 0, 0, 0, 6, 52,
                                    60, 0,  0, 0, 0, 0, 0};
const u8 gCastingImpact_ImageY[] = {0, 20, 2, 2, 34, 64, 68};
const u16 gCastingImpact_OrbitCells[] = {2308, 2372, 2436, 2500, 0};

// ripple.c
const u8 gFireSwirl_CellWidths[] = {26, 46, 63, 77, 60, 39, 32};
const u8 gFireSwirl_CellHeights[] = {11, 20, 37, 65, 65, 54, 39};
const u16 gFireSwirl_CellSourceOffsets[] = {0,    286,   1206, 3537,
                                            8542, 12442, 14548};
const s8 gFireSwirl_CellBiasX[] = {53, 36, 16, 2, 2, 2, 2};
const s8 gFireSwirl_CellBiasY[] = {2, 3, 3, 3, 4, 14, 29};

// place_formation_objects.c
const u8 gFormation_TwelveX[] = {52, 25,  57, 89, 7,  39,
                                 71, 103, 21, 53, 85, 117};
const u8 gFormation_TwelveZ[] = {0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96};
const u8 gFormation_RightEightX[] = {0, 32, 64, 16, 48, 0, 32, 64};
const u8 gFormation_RightEightZ[] = {0, 0, 0, 32, 32, 64, 64, 64};
const u8 gFormation_SmallEightX[] = {0, 40, 80, 0, 40, 80, 40, 80};
const u8 gFormation_SmallEightZ[] = {13, 0, 0, 53, 40, 40, 80, 80};
const u8 gImpactBurst_CellWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};
const u8 gImpactBurst_CellHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};
const u16 gImpactBurst_CellSourceOffsets[] = {0,     4608,  9216,  13356,
                                              16020, 18684, 20668, 22020};

// scenefx.c
const s16 gParticleStreams_OrbitPoints[][3] = {
    {-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48}, {-16, -72, -24},
    {16, -80, 16},  {32, -112, -32}, {48, -96, 56}};
const s16 gParticleStreams_DropPoints[][2] = {
    {32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};
const u16 gParticleStreams_FlashSheetOffsets[] = {0,    256,  1280, 3584,
                                                  5888, 8192, 10496};
const u16 gParticleStreams_FlashSizes[] = {16, 32, 48, 48, 48, 48, 48};
const u8 gCirclingScene_ObjectColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};
const u8 gCirclingScene_ObjectRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};
const u16 gCirclingScene_PuffSheetOffsets[] = {0,    256,  1280, 3584,
                                               5888, 8192, 10496};
const u16 gCirclingScene_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// flame_blade.c
const u8 gFlameBlade_StrikeColumns[] = {32, 24, 0, 96, 104, 128};
const u8 gFlameBlade_FlashCells[] = {5, 5, 4, 3, 2, 1};

// falling_sword.c
const u8 gFallingSword_FlashCells[] = {5, 5, 4, 3, 2, 1};
const s32 gFallingSword_DustGravity[] = {0x8000, 0x4000, 0x4000, 0x2000};
const u8 gShockwaveScene_BurstHeights[] = {2, 3, 4, 6, 10, 11, 12};
const u16 gShockwaveScene_BurstCells[] = {0, 8, 26, 58, 130, 330, 572};
const u16 gShockwaveScene_ShardCells[] = {2396, 2985, 3698, 4411, 4810, 5146};
const u8 gShockwaveScene_ShardWidths[] = {19, 23, 23, 19, 16, 17};
const u8 gShockwaveScene_ShardHeights[] = {31, 31, 31, 21, 21, 24};

// object_row.c
const u8 gObjectRow_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};
const u8 gObjectRow_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

// swoop.c
const u8 gBattleFx6_ObjectX[] = {0, 32, 64, 0, 32, 64, 96, 96};
const u8 gBattleFx6_ObjectY[] = {0, 0, 0, 32, 32, 32, 0, 32};
const s32 gBattleFx6_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// colburst.c
const u16 gRisingBurst_SparkCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
const u16 gRisingBurst_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};
