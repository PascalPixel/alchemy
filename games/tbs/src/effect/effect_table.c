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

// Shared cell layouts
u16 gBattleFx_MoteCellOffsets[] = {0, 2, 10, 28, 60, 110, 182, 280, 408, 570};
u16 gBattleFx_FlareCells[] = {0, 4, 20, 56,  120, 220, 364, 560,  816,  1140,
                              0, 8, 40, 112, 240, 440, 728, 1120, 1632, 2280};
u16 gBattleFx_PuffCells[] = {0, 25, 74, 123, 172, 221, 246, 255, 256};
u8 gBattleFx_PuffSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};
u8 gBattleFx_PuffArcCellWidths[] = {24, 29, 30, 27, 24, 16};
u8 gBattleFx_PuffArcCellHeights[] = {23, 32, 37, 49, 53, 52};
u8 gBattleFx_PuffArcCellBiasY[] = {41, 32, 26, 11, 4, 2};
u16 gBattleFx_PuffArcCellSourceOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
u16 gBattleFx_GlintCellOffsets[] = {0, 589, 1302, 2015, 2414, 2750};
u8 gBattleFx_GlintCellWidths[] = {19, 23, 23, 19, 16, 17};
u8 gBattleFx_GlintCellHeights[] = {31, 31, 31, 21, 21, 24};

// Falling shard and bolt wave counts.
u8 gIce_Counts[] = {16, 46, 20, 52, 32, 80};
u8 gFallingBolts_Counts[] = {6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80};

// Falling streak timing and motion records.
s8 gFiveMode_Records[] = {0,  1, 0, 0, 0,  1, 4, 0, 0, 1,
                                -1, 1, 1, 1, -1, 1, 1, 1, 4, 0};
s8 gFiveMode_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

// Target strike timing, motion and picture cells.
u8 gTwelveMode_Records[] = {
    1, 2, 8, 0, 6, 3, 70, 1, 1, 1, 1, 6, 2, 54, 1, 3, 1,  2, 6, 2, 96,
    1, 3, 2, 3, 4, 2, 96, 1, 3, 2, 4, 4, 2, 96, 1, 3, 4,  5, 4, 3, 96,
    0, 1, 4, 5, 4, 3, 38, 0, 2, 8, 5, 2, 3, 48, 0, 1, 24, 4, 2, 3, 50,
    0, 1, 1, 1, 2, 3, 46, 1, 1, 1, 4, 6, 2, 54, 1, 3, 2,  4, 4, 3, 96};
u8 gTwelveMode_StrikeWidths[] = {38, 54, 71, 74, 77, 65};
u8 gTwelveMode_StrikeHeights[] = {7, 9, 11, 13, 13, 11};
u16 gTwelveMode_StrikeOffsets[] = {3200, 3466, 3952, 4733, 5695, 6696};
u8 gTwelveMode_StrikeReach[] = {58, 42, 25, 13, 5, 0};
u8 gTwelveMode_ColumnStages[] = {0, 1, 2, 3, 2};
u8 gTwelveMode_GlintDrawFlags[] = {0, 4, 8, 12};

// Blade rain and shattering tile pieces.
u8 gBladeRain_CellWidths[] = {20, 21, 22, 25};
u8 gBladeRain_CellHeights[] = {32, 31, 27, 24};
u16 gBladeRain_CellSourceOffsets[] = {0, 640, 1291, 1885};
u8 gBattlePieceWidth[] = {21, 20, 24, 30, 35, 69, 19, 23, 9,  22, 33,
                                10, 18, 9,  7,  9,  17, 55, 30, 25, 40, 29,
                                10, 28, 14, 5,  32, 23, 19, 32, 8,  43, 15};
u8 gBattlePieceHeight[] = {23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35,
                                 17, 18, 14, 15, 12, 13, 37, 38, 41, 60, 41,
                                 15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11};
u16 gBattlePieceOffset[] = {
    0,     483,   1423,  2023,  3223,  3923,  7580,  8112,  8779,
    8986,  9294,  10449, 10619, 10943, 11069, 11174, 11282, 11503,
    13538, 14678, 15703, 18103, 19292, 19442, 20702, 21528, 21733,
    22565, 23715, 24114, 25234, 25330, 27781, 27946};
u8 gBattlePieceStartX[] = {0,  21, 28, 46, 70, 51, 0,  13, 37, 77, 87,
                                 19, 25, 35, 44, 45, 48, 65, 0,  0,  0,  5,
                                 33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};
u8 gBattlePieceStartY[] = {0,  0,  0,  0,  0,  0,  19, 17, 23,  28, 8,
                                 36, 43, 43, 40, 47, 48, 37, 42, 67,  60, 79,
                                 62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

// Canvas corner sparks and two-resource picture cells.
s8 gCornerSparks_X[] = {0, 32, 0, 32};
s8 gCornerSparks_Y[] = {0, 0, 32, 32};
u8 gCornerSparks_DrawFlags[] = {0, 4, 8, 12};
u8 gTwoResource_CellWidths[] = {40, 112, 112, 112, 88, 64};
u8 gTwoResource_CellHeights[] = {56, 80, 64, 64, 80, 88};
u16 gTwoResource_CellSourceOffsets[] = {0, 2240, 11200, 18368, 0, 7040};
u8 gTwoResource_CellX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};
s8 gTwoResource_CellBiasY[] = {0, 4, 24, -6, -20, -28, 0, 0};

// Leaping strike counts.
u8 gMemberBurst_Counts[] = {16, 88, 8, 72, 12, 80};

// Mirrored earth wall picture cells.
u16 gEarthWall_CellSourceOffsets[] = {2702, 3535, 4580};
u8 gEarthWall_CellWidths[] = {17, 19, 20};
u8 gEarthWall_CellHeights[] = {49, 55, 64};

// Djinni beam motes and heads.
u8 gMemberBeam_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};
u16 gMemberBeam_HeadOffsets[] = {2702, 3535, 4580};
u8 gMemberBeam_HeadWidths[] = {17, 19, 20};
u8 gMemberBeam_HeadHeights[] = {49, 55, 64};

// Shell explosion records and shards.
s8 gSevenMode_Records[] = {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
u8 gSevenMode_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12,
                                     6, 7,  7, 7, 6, 6,  8,  8,  8};
u8 gSevenMode_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12,
                                      8,  7,  6,  6,  8,  6,  4,  3,  3};
u16 gSevenMode_ShardOffsets[] = {0,    108,  228,  336,  444,  552,
                                       684,  816,  948,  1092, 1140, 1189,
                                       1231, 1273, 1321, 1357, 1389, 1413};

// Shattered panel coordinates.
u8 gCondemn_PanelX[] = {10, 30, 0, 0, 0, 6, 52,
                                    60, 0,  0, 0, 0, 0, 0};
u8 gCondemn_PanelY[] = {0, 20, 2, 2, 34, 64, 68,
                                    0, 9,  4, 4, 4,  4,  4};

// Spinning and striking triangle vertices.
struct Vector3 gBind_Vertex[] = {{.x = 0, .y = 24, .z = 0}};
struct Vector3 gTriangleStrike_Vertex[] = {{.x = 0, .y = 24, .z = 0}};

// Ring bolt, spinning star and whirlwind comet paths.
struct Vector3 gAstralBlast_Points[] = {
    {.x = 0, .y = 48 << 16, .z = 0}, {.x = 0, .y = 28 << 16, .z = 0}};
u8 gShiningStar_Radii[] = {32, 16};
u8 gWhirlwindComets_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};
u8 gWhirlwindComets_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};
u16 gWhirlwindComets_Frames[] = {334, 347, 359, 372};
u8 gWhirlwindComets_CellWidths[] = {14, 24, 48};
u8 gWhirlwindComets_CellHeights[] = {14, 19, 24};
u8 gWhirlwindComets_CellBiasY[] = {8, 12, 17};
u16 gWhirlwindComets_CellOffsets[] = {13824, 14020, 14476};

// Palette particle and ember column layouts.
u8 gPaletteParticles_DropSpeeds[] = {0, 8, 12, 16};
u8 gPaletteParticles_SparkPixels[] = {32, 32};
u8 gPaletteParticles_DropSizes[] = {1, 1, 2, 3};
u8 gPaletteParticles_StreakWidths[] = {24, 29, 30, 27, 24, 16};
u8 gPaletteParticles_StreakHeights[] = {23, 32, 37, 49, 53, 52};
u8 gPaletteParticles_StreakDrops[] = {41, 32, 26, 11, 4, 2};
u16 gPaletteParticles_StreakOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
u8 gNereid_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104};
s32 gNereid_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// Vortex mote counts and tornado shapes.
u8 gFroth_Counts[] = {6, 140, 12, 160, 30, 200};
u8 gWhirlwind_Shapes[] = {1, 16, 2, 2, 24, 2, 4, 32, 4};

// Crystal counts and picture cells.
u8 gPrism_ShardStarts[] = {
    32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99, 54, 69, 34, 82, 45,
    71, 31, 91, 32, 69, 26, 80, 40, 68, 41, 60, 46, 60, 36, 63, 0,  0};
u8 gPrism_Counts[] = {1, 110, 4, 134, 12, 166};
u8 gPrism_ShardWidths[] = {5,  10, 11, 14, 12, 11,
                                   19, 18, 23, 12, 22, 22};
u8 gPrism_ShardHeights[] = {14, 10, 7,  13, 14, 14,
                                    32, 30, 24, 38, 20, 20};
s32 gPrism_ShardOffsets[] = {0,   70,   170,  247,  429,  597,
                                     751, 1359, 1899, 2451, 2907, 3347};

// Lightning pillar sparks, columns and counts.
u16 gPlasma_Sparks[] = {384, 2, 256, 3, 192, 4};
u8 gPlasma_Columns[] = {32, 64, 16,  48, 72, 96, 64,
                                       88, 56, 104, 72, 48, 24, 56};
u8 gPlasma_Counts[] = {1, 3, 7};

// Spark group shapes and flash cells.
u16 gSparkGroups_Shapes[] = {128, 1,   16, 0, 0,  255, 1,   16, 0,
                                   0,   192, 3,  0, 24, 48,  340, 1,  16,
                                   24,  48,  32, 1, 16, 0,   0};
u8 gSparkGroups_FlashCells[] = {5, 5, 4, 3, 2, 1};

// Fire pillar counts, depths and start frames.
u8 gVolcano_Counts[] = {1, 2, 4};
s8 gVolcano_Depths[][4] = {
    {0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};
u8 gVolcano_StartFrames[] = {8, 18, 24, 30, 34};

// Target mote glint orientations.
u8 gRenderMode_GlintFlips[] = {0, 4, 8, 12};

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
u8 gCybele_ObjectPoses[] = {8, 10, 9, 11, 3, 4};
u16 gCybele_ThornOffsets[] = {3920, 3932, 3947, 3955, 3970};
u8 gCybele_ThornWidths[] = {2, 5, 2, 5, 3};
u8 gCybele_ThornHeights[] = {6, 3, 4, 3, 3};
u16 gCybele_LeafOffsets[] = {3456, 3712, 3856};
u8 gCybele_LeafSizes[] = {16, 12, 8};
u16 gCybele_ChipOffsets[] = {3920, 3932, 3947, 3955, 3970};
u8 gCybele_ChipWidths[] = {2, 5, 2, 5, 3};
u8 gCybele_ChipHeights[] = {6, 3, 4, 3, 3};
u16 gCybele_SparkOffsets[] = {3979, 4043, 4268, 4389, 4470};
u8 gCybele_SparkSizes[] = {8, 15, 11, 9, 5};
u16 gCybele_EmberOffsets[] = {5760,  6016,  7040, 9344,
                                        11648, 13952, 16256};
u16 gCybele_EmberSizes[] = {16, 32, 48, 48, 48, 48, 48};
u8 gBoreas_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130},
                                    {86, 126}, {85, 98},  {64, 98},  {98, 96},
                                    {77, 87},  {84, 88},  {68, 67},  {81, 49},
                                    {94, 70},  {70, 51},  {93, 36},  {81, 50}};
s8 gBoreas_ShakeOffsets[] = {-4, -3, -2, 0};
u16 gBoreas_RockCells[] = {0, 1152, 2064};
u8 gBoreas_RockWidths[] = {24, 24, 20};
u8 gBoreas_RockHeights[] = {48, 38, 26};
u8 gBoreas_Animations[] = {2, 9, 10, 11};
u8 gBoreas_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};
u8 gBoreas_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};
u16 gBoreas_DebrisCells[] = {2584, 2704, 2880, 2992, 5062, 5094,
                                       5142, 5157, 5185, 5227, 5299};
u8 gBoreas_SprayWidths[] = {16, 16, 16};
u8 gBoreas_SprayHeights[] = {16, 16, 16};
u16 gBoreas_SprayCells[] = {4294, 4550, 4806};
u16 gBoreas_BoulderCells[] = {0, 1152, 2064};
u8 gBoreas_BoulderWidths[] = {24, 24, 20};
u8 gBoreas_BoulderHeights[] = {48, 38, 26};
u8 gBoreas_FallWidths[] = {24, 24, 20};
u8 gBoreas_FallHeights[] = {48, 38, 26};
u16 gBoreas_FallCells[] = {0, 1152, 2064};
s32 gBreak_Gravities[] = {0x4000, 0x2000, 0x4000, 0x8000};

// Stat-reduction shard geometry.
u16 gPaletteRamp_ShardCells[] = {0,   120, 180, 258, 298, 352,
                                       408, 471, 567, 607, 631, 661};
u8 gPaletteRamp_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};
u8 gPaletteRamp_ShardHeights[] = {12, 12, 13, 8, 9, 8,
                                        7,  12, 10, 4, 5, 5};

// Rock wall timing.
u8 gQuake_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

// Homing ember counts and flare geometry.
u8 gFire_Counts[] = {16, 32, 64};
u8 gFire_FlareWidths[] = {24, 29, 30, 27, 24, 16};
u8 gFire_FlareHeights[] = {23, 32, 37, 49, 53, 52};
u8 gFire_FlareBiasY[] = {41, 32, 26, 11, 4, 2};
u16 gFire_FlareCells[] = {0, 552, 1480, 2590, 3913, 5185};

// Rising mote geometry and timing.
u8 gRisingMotes_ColumnSpots[] = {
    53,  108, 73, 110, 29,  110, 73,  108, 33, 104, 96, 106, 8,
    109, 62,  98, 33,  100, 87,  100, 21,  95, 53,  90, 33,  90,
    72,  91,  82, 91,  77,  86,  52,  84,  18, 92,  56, 81};
u8 gRisingMotes_Timings[] = {32, 8, 85, 48, 12, 97, 64, 19, 109};
u8 gRisingMotes_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
u8 gRisingMotes_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
u16 gRisingMotes_MoteCells[] = {1792, 1900, 2060, 2158,
                                      2308, 2372, 2436, 2500};
u16 gRisingMotes_ColumnCells[] = {0, 768, 1280};
u8 gRisingMotes_ColumnHeights[] = {24, 16, 16};
u8 gRisingMotes_EmberWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
u8 gRisingMotes_EmberHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
u16 gRisingMotes_EmberCells[] = {1792, 1900, 2060, 2158,
                                       2308, 2372, 2436, 2500};

// Closing bolt sparks by variant.
u8 gLightningBolts_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

// Eruption counts and puff layouts.
u8 gBlastAndSmokeBomb_Counts[][3] = {
    {16, 8, 72}, {32, 16, 80}, {128, 32, 88}};
u16 gBlastAndSmokeBomb_PuffCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
u16 gBlastAndSmokeBomb_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// Unreferenced cell layouts, from 6x6 through 20x20
u16 gUnused_SheetCells6[] = {0, 36, 100, 200, 344, 540};
u16 gUnused_SheetCells[] = {0,   36,   100,  200,  344, 540,
                                  796, 1120, 1520, 1900, 2300};
u8 gUnused_SheetWidths[] = {6, 8, 10, 12, 14, 16, 18, 20, 19, 20};
u8 gUnused_SheetHeights[] = {6, 8, 10, 12, 14, 16, 18, 20, 20, 20};

// Staged particle scale.
struct SpriteScale gStagedParticles_UnitScale = {{{0x10000, 0x10000}}};

// Growing vine and pillar geometry.
u8 gDualTable_VineWidths[] = {21, 28, 19};
u8 gDualTable_HeightLimits[] = {119, 119, 102};
u16 gDualTable_VineCells[] = {0, 2499, 5831};
u8 gDualTable_PillarWidths[] = {10, 8, 9};
u16 gDualTable_PillarCells[] = {0, 1080, 1888};
u8 gDualTable_ColumnCounts[] = {4, 8, 12};
u8 gDualTable_ColumnX[] = {64, 72,  56, 80, 48, 88, 40, 96,
                                 32, 104, 24, 72, 56, 80, 48, 88};
s8 gDualTable_PillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};
s8 gDualTable_PuffWidths[] = {5, 7, 11, 16, 23, 27, 16};
u8 gDualTable_PuffHeights[] = {10, 14, 21, 32, 45, 50, 32};
u16 gDualTable_PuffCells[] = {0, 50, 148, 379, 891, 1926, 3276};
s8 gVine_ColumnOffsets[] = {0,   14, -32, 8,   32, -48, 4,   12,
                                           -24, 24, -12, -40, -4, -16, -32, 12};
u8 gThorn_Kinds[] = {0, 3, 2, 1, 2, 3, 2, 1};
s8 gThorn_X[] = {24, 80, 24, 48, 88, 40, 8, 96};
u8 gThorn_Counts[] = {3, 5, 8};
s8 gThorn_PuffWidths[] = {5, 7, 11, 17, 32, 32, 19};
u8 gThorn_PuffHeights[] = {10, 14, 21, 34, 59, 60, 38};
u16 gThorn_PuffCells[] = {5888, 5938, 6036, 6267,
                                         6845, 8733, 10653};

// Lightning bolt counts and glint palettes.
u8 gLightningBolts_Counts[] = {16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3};
u8 gLightningBolts_GlintPalettes[] = {0, 4, 8, 12};
u8 gLightningBolts_GlintModes[] = {2, 2, 3};

// Projectile volley modes and picture cells.
u8 gVolley_CometHeights[] = {24, 36, 48};
u8 gVolley_Modes[][5] = {
    {6, 10, 3, 32, 4},  {4, 10, 5, 32, 8},  {10, 32, 3, 32, 4},
    {5, 10, 5, 32, 4},  {10, 10, 1, 32, 4}, {1, 5, 0, 32, 80},
    {7, 8, 2, 32, 40},  {30, 24, 1, 32, 4}, {6, 2, 3, 32, 4},
    {60, 10, 1, 32, 4}, {60, 10, 1, 32, 4}, {10, 10, 1, 32, 4}};
u8 gVolley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};
u8 gVolley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};
u8 gVolley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};
u8 gVolley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12};
u16 gVolley_FlashOffsets[] = {8192, 8312, 8582, 9014, 9674, 10646, 11738};
u16 gVolley_PelletOffsets[] = {2308, 2372, 2436, 2500};

// Mercury Djinni flame launch frames.
u8 gMercury_LaunchFrames[] = {4, 7, 10, 21, 34};

// Particle reveal and rock toss chips.
u8 gHail_CellWidths[] = {8, 8, 8, 8};
u8 gHail_CellHeights[] = {8, 8, 8, 8};
u16 gHail_CellSourceOffsets[] = {2308, 2372, 2436, 2500};
u8 gGround_ChipFlips[] = {3, 7, 11, 15};
u8 gGround_ChipWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};
u8 gGround_ChipHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};
u16 gGround_ChipCells[] = {0,    1152, 1243, 1341, 1533,
                                   1677, 1898, 2098, 2252};
u8 gGround_ChipX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};
u8 gGround_ChipY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

// Storm scene animations and shattering rocks.
u8 gMode12_Animations[] = {7, 14, 10, 4};
u8 gMode12_SmokeFlips[] = {0, 4, 8, 12};
u8 gSpire_ShardOffsets[32 * 2] = {
    65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56, 88,
    51, 80,  61, 75, 72, 81, 69, 71, 66, 70, 71, 64, 49, 71,
    59, 69,  53, 68, 78, 73, 55, 67, 75, 62, 61, 65, 53, 61};
s8 gSpire_RockX[] = {0, -12, 12, -8, 16};
u8 gSpire_DropFrames[] = {8, 16, 20, 24, 32};
u8 gSpire_RockCounts[] = {1, 3, 5};
u8 gSpire_ShardWidths[] = {5,  11, 10, 7,  7,  12, 13, 9,
                                        19, 12, 17, 22, 15, 16, 14};
u8 gSpire_ShardHeights[] = {11, 6,  10, 11, 10, 7,  17, 21,
                                         12, 21, 17, 13, 16, 16, 15};
u16 gSpire_ShardCells[] = {0,    55,   121,  221,  298,  368,
                                        452,  673,  862,  1090, 1342, 1631,
                                        1917, 2157, 2413, 2623};

// Burst scene records and rising wall geometry.
u8 gBurstScene_Records[] = {
    1, 0, 5, 16, 12, 80, 0, 1, 1, 7, 16, 12, 104, 0, 1, 2, 9, 16, 12, 128, 0,
    1, 3, 1, 16, 16, 36, 0, 1, 3, 2, 20, 16, 52,  0, 1, 3, 3, 24, 16, 68,  0,
    0, 2, 1, 32, 12, 28, 1, 0, 2, 2, 32, 12, 40,  1, 0, 2, 3, 32, 12, 52,  1};
u16 gGaia_RowSheetOffsets[] = {0,    160,  352,  640, 1024,
                                           1504, 2080, 2752, 3776};
u8 gGaia_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};
u16 gGaia_MoundSheetOffsets[] = {10672, 11505, 12550};
u8 gGaia_MoundWidths[] = {17, 19, 20};
u8 gGaia_MoundHeights[] = {49, 55, 64};
s8 gGaia_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};
u8 gGaia_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

// Beam sequence picture cells and reach.
u8 gBeamSequence_CellWidths[] = {38, 54, 71, 74, 77, 65};
u8 gBeamSequence_CellHeights[] = {7, 9, 11, 13, 13, 11};
u16 gBeamSequence_CellSheetOffsets[] = {0, 266, 752, 1533, 2495, 3496};
u8 gBeamSequence_CellReaches[] = {58, 42, 25, 13, 5, 0};

// Casting impact glints and orbit cells.
u8 gCastingImpact_GlintDrawFlags[] = {0, 4, 8, 12};
u8 gCastingImpact_ImageX[] = {10, 30, 0, 0, 0, 6, 52,
                                    60, 0,  0, 0, 0, 0, 0};
u8 gCastingImpact_ImageY[] = {0, 20, 2, 2, 34, 64, 68};
u16 gCastingImpact_OrbitCells[] = {2308, 2372, 2436, 2500, 0};

// Fire swirl picture cells.
u8 gHelmSplitter_CellWidths[] = {26, 46, 63, 77, 60, 39, 32};
u8 gHelmSplitter_CellHeights[] = {11, 20, 37, 65, 65, 54, 39};
u16 gHelmSplitter_CellSourceOffsets[] = {0,    286,   1206, 3537,
                                            8542, 12442, 14548};
s8 gHelmSplitter_CellBiasX[] = {53, 36, 16, 2, 2, 2, 2};
s8 gHelmSplitter_CellBiasY[] = {2, 3, 3, 3, 4, 14, 29};

// Object formation coordinates and impact burst cells.
u8 gFormation_TwelveX[] = {52, 25,  57, 89, 7,  39,
                                 71, 103, 21, 53, 85, 117};
u8 gFormation_TwelveZ[] = {0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96};
u8 gFormation_RightEightX[] = {0, 32, 64, 16, 48, 0, 32, 64};
u8 gFormation_RightEightZ[] = {0, 0, 0, 32, 32, 64, 64, 64};
u8 gFormation_SmallEightX[] = {0, 40, 80, 0, 40, 80, 40, 80};
u8 gFormation_SmallEightZ[] = {13, 0, 0, 53, 40, 40, 80, 80};
u8 gImpactBurst_CellWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};
u8 gImpactBurst_CellHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};
u16 gImpactBurst_CellSourceOffsets[] = {0,     4608,  9216,  13356,
                                              16020, 18684, 20668, 22020};

// Particle stream paths and circling scene geometry.
s16 gParticleStreams_OrbitPoints[][3] = {
    {-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48}, {-16, -72, -24},
    {16, -80, 16},  {32, -112, -32}, {48, -96, 56}};
s16 gParticleStreams_DropPoints[][2] = {
    {32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};
u16 gParticleStreams_FlashSheetOffsets[] = {0,    256,  1280, 3584,
                                                  5888, 8192, 10496};
u16 gParticleStreams_FlashSizes[] = {16, 32, 48, 48, 48, 48, 48};
u8 gRamses_ObjectColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};
u8 gRamses_ObjectRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};
u16 gRamses_PuffSheetOffsets[] = {0,    256,  1280, 3584,
                                               5888, 8192, 10496};
u16 gRamses_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// Flame blade strike columns and flash cells.
u8 gRagnarok_StrikeColumns[] = {32, 24, 0, 96, 104, 128};
u8 gRagnarok_FlashCells[] = {5, 5, 4, 3, 2, 1};

// Falling sword flashes, dust and shockwave debris.
u8 gTitanBlade_FlashCells[] = {5, 5, 4, 3, 2, 1};
s32 gTitanBlade_DustGravity[] = {0x8000, 0x4000, 0x4000, 0x2000};
u8 gShockwaveScene_BurstHeights[] = {2, 3, 4, 6, 10, 11, 12};
u16 gShockwaveScene_BurstCells[] = {0, 8, 26, 58, 130, 330, 572};
u16 gShockwaveScene_ShardCells[] = {2396, 2985, 3698, 4411, 4810, 5146};
u8 gShockwaveScene_ShardWidths[] = {19, 23, 23, 19, 16, 17};
u8 gShockwaveScene_ShardHeights[] = {31, 31, 31, 21, 21, 24};

// Object row coordinates.
u8 gKirin_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};
u8 gKirin_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

// Formation sweep coordinates and gravity.
u8 gNeptune_ObjectX[] = {0, 32, 64, 0, 32, 64, 96, 96};
u8 gNeptune_ObjectY[] = {0, 0, 0, 32, 32, 32, 0, 32};
s32 gNeptune_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// Rising burst spark cells.
u16 gUndeadSword_SparkCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
u16 gUndeadSword_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};
