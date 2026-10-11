#include "types.h"

//--------------------------------------------------------------------------
// 戦闘効果のテーブル
// 使う演出のコードと同じ順に並ぶ。セルの位置はシートの先頭からのバイト数で、
// 幅×高さを順に足したもの。
//--------------------------------------------------------------------------

// 使う側は const なしで extern する（その形でコードが一致する）。

struct BattleEffectArgument;

struct Vector3
{
	s32 x;
	s32 y;
	s32 z;
};

typedef void (*BattleFxHandler)(struct BattleEffectArgument *effect);

typedef struct Scale
{
	s32 x;
	s32 y;
} Scale;

void BattleFx_RunRockWall(struct BattleEffectArgument *effect);
void BattleFx_RunShatteringRocks(struct BattleEffectArgument *effect);
void BattlePres_RunRisingWall(struct BattleEffectArgument *effect);
void BattleFx_RunDualTableMode0(struct BattleEffectArgument *effect);
void BattleFx_RaisePillars(struct BattleEffectArgument *effect);
void BattleFx_RunRisingMotes(struct BattleEffectArgument *effect);
void BattleEffect_RunFallingParticles(struct BattleEffectArgument *effect);
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect);
void BattleEffect_RunFallingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect);
void BattleFx_RunPuffArc(struct BattleEffectArgument *effect);
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect);
void BattleFx_RunFirePillars(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeBMode0(struct BattleEffectArgument *effect);
void BattleFx_RunLightningBolts(struct BattleEffectArgument *effect);
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect);
void BattleFx_RunMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTornado(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode3Or4(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeAMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode0(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRampMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode1(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode2(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRampMode2(struct BattleEffectArgument *effect);
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect);
void BattleFx_RenderMode2(struct BattleEffectArgument *effect);
void BattleFx_RunMemberOrbit(struct BattleEffectArgument *effect);
void BattleFx_RenderMode4(struct BattleEffectArgument *effect);
void BattleFx_RenderMode3(struct BattleEffectArgument *effect);
void BattleFx_RunBouncingSheep(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect);
void BattleFx_RunClosingSkulls(struct BattleEffectArgument *effect);
void BattleFx_RunCounterReveal(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMarkers(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect);
void BattleFx_RunIceShardBursts(struct BattleEffectArgument *effect);
void BattleEffect_RunDualParticleStream(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode0(struct BattleEffectArgument *effect);
void BattleFx_RunFireSwirlCharge(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumnMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect);
void BattleFx_RunMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFireBurstShards(struct BattleEffectArgument *effect);
void BattleFx_RunDualTableMode1(struct BattleEffectArgument *effect);
void BattleEffect_RunMode5WithAction(struct BattleEffectArgument *effect);
void BattleFx_RunFlameBlade(struct BattleEffectArgument *effect);
void BattleFx_RunMode6WithAction(struct BattleEffectArgument *effect);
void BattleFx_RunLeapingStrike(struct BattleEffectArgument *effect);
void BattleFx_RunFallingSword(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect);
void BattleFx_RunMode9WithAction(struct BattleEffectArgument *effect);
void BattleFx_RenderMode7(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumnMode2(struct BattleEffectArgument *effect);
void BattleFx_RenderMode6(struct BattleEffectArgument *effect);
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect);
void BattleFx_RunRingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode3(struct BattleEffectArgument *effect);
void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect);
void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect);
void BattleFx_RunMode7(struct BattleEffectArgument *effect);
void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode5(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeAMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode5(struct BattleEffectArgument *effect);
void BattleFx_RunRipplingDisc(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode4(struct BattleEffectArgument *effect);
void ObjectGroup_StoreObjectAndRunStep7(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode6Or7Or8(struct BattleEffectArgument *effect);
void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect);
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect);
void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect);
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunMode2(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunMode4(struct BattleEffectArgument *effect);
void BattleFx_RunMode3(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldVariant0(struct BattleEffectArgument *effect);
void BattleFx_RenderMode5(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldVariant1(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode3(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode1(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode3Or4Or5(struct BattleEffectArgument *effect);
void BattleFx_RunRisingBurst(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode0Alternate(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode5(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode9(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldVariant2(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode4(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode8(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode2(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode7(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode10(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeBMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldVariant3(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunParticlePoolMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticlePoolMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode5(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode2(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect);
void BattleFx_RunMode11(struct BattleEffectArgument *effect);
void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *effect);
void BattleFx_InitializeMode1(struct BattleEffectArgument *effect);
void BattleFx_RenderAnimationMode1(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode11(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect);
void BattleFx_RunMode10(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect);

// 共有のセル配置
const u16 ParticleStreams_CellOffsets[] = {0, 2, 10, 28, 60, 110, 182, 280, 408, 570};
const u16 BattleFx6_FlareCells[] = {
	0, 4, 20, 56, 120, 220, 364, 560, 816, 1140, 0, 8, 40, 112, 240, 440, 728, 1120, 1632, 2280};
const u16 BattleFx_PuffCells[] = {0, 25, 74, 123, 172, 221, 246, 255, 256};
const u8  BattleFx_PuffSizes[] = {5, 7, 7, 7, 7, 5, 3, 1, 1};
const u8  PuffArc_CellWidths[] = {24, 29, 30, 27, 24, 16};
const u8  PuffArc_CellHeights[] = {23, 32, 37, 49, 53, 52};
const u8  PuffArc_CellBiasY[] = {41, 32, 26, 11, 4, 2};
const u16 PuffArc_CellSourceOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
const u16 BattleFx_GlintCellOffsets[] = {0, 589, 1302, 2015, 2414, 2750};
const u8  BattleFx_GlintCellWidths[] = {19, 23, 23, 19, 16, 17};
const u8  BattleFx_GlintCellHeights[] = {31, 31, 31, 21, 21, 24};

// falling.c
const u8 FallingShards_Counts[] = {16, 46, 20, 52, 32, 80};
const u8 FallingBolts_Counts[] = {6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80};

// run_five_mode_mode0.c
const s8 FiveMode_Records[] = {0, 1, 0, 0, 0, 1, 4, 0, 0, 1, -1, 1, 1, 1, -1, 1, 1, 1, 4, 0};
const s8 FiveMode_Tremble[] = {-1, -1, 1, 1, 1, 1, -1, -1};

// twelve_mode.c
const u8  TwelveMode_Records[] = {1, 2, 8, 0, 6, 3, 70, 1, 1, 1, 1, 6, 2, 54, 1, 3, 1, 2, 6, 2, 96,
	1, 3, 2, 3, 4, 2, 96, 1, 3, 2, 4, 4, 2, 96, 1, 3, 4, 5, 4, 3, 96, 0, 1, 4, 5, 4, 3, 38, 0, 2, 8,
	5, 2, 3, 48, 0, 1, 24, 4, 2, 3, 50, 0, 1, 1, 1, 2, 3, 46, 1, 1, 1, 4, 6, 2, 54, 1, 3, 2, 4, 4,
	3, 96};
const u8  TwelveMode_StrikeWidths[] = {38, 54, 71, 74, 77, 65};
const u8  TwelveMode_StrikeHeights[] = {7, 9, 11, 13, 13, 11};
const u16 TwelveMode_StrikeOffsets[] = {3200, 3466, 3952, 4733, 5695, 6696};
const u8  TwelveMode_StrikeReach[] = {58, 42, 25, 13, 5, 0};
const u8  TwelveMode_ColumnStages[] = {0, 1, 2, 3, 2};
const u8  TwelveMode_GlintDrawFlags[] = {0, 4, 8, 12};

// flipping_burst.c
const u8  BladeRain_CellWidths[] = {20, 21, 22, 25};
const u8  BladeRain_CellHeights[] = {32, 31, 27, 24};
const u16 BladeRain_CellSourceOffsets[] = {0, 640, 1291, 1885};
const u8  BattlePieceWidth[] = {21, 20, 24, 30, 35, 69, 19, 23, 9, 22, 33, 10, 18, 9, 7, 9, 17, 55,
	30, 25, 40, 29, 10, 28, 14, 5, 32, 23, 19, 32, 8, 43, 15};
const u8  BattlePieceHeight[] = {23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35, 17, 18, 14, 15, 12, 13,
	37, 38, 41, 60, 41, 15, 45, 59, 41, 26, 50, 21, 35, 12, 57, 11};
const u16 BattlePieceOffset[] = {0, 483, 1423, 2023, 3223, 3923, 7580, 8112, 8779, 8986, 9294,
	10449, 10619, 10943, 11069, 11174, 11282, 11503, 13538, 14678, 15703, 18103, 19292, 19442,
	20702, 21528, 21733, 22565, 23715, 24114, 25234, 25330, 27781, 27946};
const u8  BattlePieceStartX[] = {0, 21, 28, 46, 70, 51, 0, 13, 37, 77, 87, 19, 25, 35, 44, 45, 48,
	65, 0, 0, 0, 5, 33, 15, 36, 46, 47, 50, 53, 65, 72, 77, 105};
const u8 BattlePieceStartY[] = {0, 0, 0, 0, 0, 0, 19, 17, 23, 28, 8, 36, 43, 43, 40, 47, 48, 37, 42,
	67, 60, 79, 62, 75, 61, 62, 60, 70, 74, 85, 108, 63, 74};

// canvfx.c
const s8  CornerSparks_X[] = {0, 32, 0, 32};
const s8  CornerSparks_Y[] = {0, 0, 32, 32};
const u8  CornerSparks_DrawFlags[] = {0, 4, 8, 12};
const u8  TwoResource_CellWidths[] = {40, 112, 112, 112, 88, 64};
const u8  TwoResource_CellHeights[] = {56, 80, 64, 64, 80, 88};
const u16 TwoResource_CellSourceOffsets[] = {0, 2240, 11200, 18368, 0, 7040};
const u8  TwoResource_CellX[] = {72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0};
const s8  TwoResource_CellBiasY[] = {0, 4, 24, -6, -20, -28, 0, 0};

// memberfx.c
const u8 MemberBurst_Counts[] = {16, 88, 8, 72, 12, 80};

// earthwal.c
const u16 EarthWall_CellSourceOffsets[] = {2702, 3535, 4580};
const u8  EarthWall_CellWidths[] = {17, 19, 20};
const u8  EarthWall_CellHeights[] = {49, 55, 64};

// member_beam.c
const u8  MemberBeam_MotePictures[] = {2, 1, 0, 1, 2, 3, 4, 3};
const u16 MemberBeam_HeadOffsets[] = {2702, 3535, 4580};
const u8  MemberBeam_HeadWidths[] = {17, 19, 20};
const u8  MemberBeam_HeadHeights[] = {49, 55, 64};

// seven_mode_dispatch.c
const s8  SevenMode_Records[] = {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
const u8  SevenMode_ShardWidths[] = {9, 10, 9, 9, 9, 11, 11, 11, 12, 6, 7, 7, 7, 6, 6, 8, 8, 8};
const u8 SevenMode_ShardHeights[] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 8, 7, 6, 6, 8, 6, 4, 3, 3};
const u16 SevenMode_ShardOffsets[] = {0, 108, 228, 336, 444, 552, 684, 816, 948, 1092, 1140, 1189,
	1231, 1273, 1321, 1357, 1389, 1413};

// reveal.c
const u8 CounterReveal_PanelX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};
const u8 CounterReveal_PanelY[] = {0, 20, 2, 2, 34, 64, 68, 0, 9, 4, 4, 4, 4, 4};

// triangle.c
const struct Vector3 SpinningTriangle_Vertex[] = {{0, 24, 0}};
const struct Vector3 TriangleStrike_Vertex[] = {{0, 24, 0}};

// stars.c
const struct Vector3 RingBolts_Points[] = {{0, 48 << 16, 0}, {0, 28 << 16, 0}};
const u8			 SpinningStars_Radii[] = {32, 16};
const u8			 WhirlwindComets_GridX[] = {0, 32, 64, 0, 32, 64, 0, 32, 64};
const u8			 WhirlwindComets_GridY[] = {0, 0, 0, 32, 32, 32, 64, 64, 64};
const u16			 WhirlwindComets_Frames[] = {334, 347, 359, 372};
const u8			 WhirlwindComets_CellWidths[] = {14, 24, 48};
const u8			 WhirlwindComets_CellHeights[] = {14, 19, 24};
const u8			 WhirlwindComets_CellBiasY[] = {8, 12, 17};
const u16			 WhirlwindComets_CellOffsets[] = {13824, 14020, 14476};

// embers.c
const u8  PaletteParticles_DropSpeeds[] = {0, 8, 12, 16};
const u8  PaletteParticles_SparkPixels[] = {32, 32};
const u8  PaletteParticles_DropSizes[] = {1, 1, 2, 3};
const u8  PaletteParticles_StreakWidths[] = {24, 29, 30, 27, 24, 16};
const u8  PaletteParticles_StreakHeights[] = {23, 32, 37, 49, 53, 52};
const u8  PaletteParticles_StreakDrops[] = {41, 32, 26, 11, 4, 2};
const u16 PaletteParticles_StreakOffsets[] = {0, 552, 1480, 2590, 3913, 5185};
const u8  EmberColumns_Columns[] = {16, 104, 32, 104, 48, 104, 64, 104};
const s32 EmberColumns_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// vortex.c
const u8 VortexMotes_Counts[] = {6, 140, 12, 160, 30, 200};
const u8 Tornado_Shapes[] = {1, 16, 2, 2, 24, 2, 4, 32, 4};

// crystal.c
const u8  Crystal_ShardStarts[] = {32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99, 54, 69, 34, 82,
	45, 71, 31, 91, 32, 69, 26, 80, 40, 68, 41, 60, 46, 60, 36, 63, 0, 0};
const u8  Crystal_Counts[] = {1, 110, 4, 134, 12, 166};
const u8  Crystal_ShardWidths[] = {5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22};
const u8  Crystal_ShardHeights[] = {14, 10, 7, 13, 14, 14, 32, 30, 24, 38, 20, 20};
const s32 Crystal_ShardOffsets[] = {0, 70, 170, 247, 429, 597, 751, 1359, 1899, 2451, 2907, 3347};

// lightning.c
const u16 LightningPillar_Sparks[] = {384, 2, 256, 3, 192, 4};
const u8  LightningPillar_Columns[] = {32, 64, 16, 48, 72, 96, 64, 88, 56, 104, 72, 48, 24, 56};
const u8  LightningPillar_Counts[] = {1, 3, 7};

// sparks.c
const u16 SparkGroups_Shapes[] = {
	128, 1, 16, 0, 0, 255, 1, 16, 0, 0, 192, 3, 0, 24, 48, 340, 1, 16, 24, 48, 32, 1, 16, 0, 0};
const u8 SparkGroups_FlashCells[] = {5, 5, 4, 3, 2, 1};

// pillars.c
const u8 FirePillars_Counts[] = {1, 2, 4};
const s8 FirePillars_Depths[][4] = {{0, 0, 0, 0}, {24, -24, 0, 0}, {-20, 20, -60, 60}};
const u8 FirePillars_StartFrames[] = {8, 18, 24, 30, 34};

// render_mode.c
const u8 RenderMode_GlintFlips[] = {0, 4, 8, 12};

// dispatch.c
// 効果の種類 - 1 ごとの処理
const BattleFxHandler BattleFx_ModeHandlers[] = {
	BattleFx_RunRockWall,							// 1
	BattleFx_RunShatteringRocks,					// 2
	BattlePres_RunRisingWall,						// 3
	BattleFx_RunDualTableMode0,						// 4
	BattleFx_RaisePillars,							// 5
	BattleFx_RunRisingMotes,						// 6
	BattleEffect_RunFallingParticles,				// 7
	BattleFx_RunFallingCrystals,					// 8
	BattleEffect_RunFallingBolts,					// 9
	BattleFx_RunVortexMotes,						// 10
	BattleFx_RunPuffArc,							// 11
	BattleFx_RunHomingEmbers,						// 12
	BattleFx_RunFirePillars,						// 13
	BattleFx_RunParticleFieldMode0,					// 14
	BattleFx_RunTwoModeBMode0,						// 15
	BattleFx_RunLightningBolts,						// 16
	BattleFx_RunClosingBolts,						// 17
	BattleFx_RunLightningPillars,					// 18
	BattleFx_RunMode0,								// 19
	BattleFx_RunTornado,							// 20
	BattleFx_RunSeriesAMode2,						// 21
	BattleFx_RunSeriesAMode0,						// 22
	BattleFx_RunSeriesAMode1,						// 23
	BattleFx_RunSeriesAMode3Or4,					// 24
	BattleFx_RunTwoModeAMode0,						// 25
	BattleFx_RunSeriesBMode0,						// 26
	BattleFx_RunPaletteRampMode0,					// 27
	BattleFx_RunSeriesBMode1,						// 28
	BattleFx_RunPaletteRampMode1,					// 29
	BattleFx_RunSeriesBMode2,						// 30
	BattleFx_RunPaletteRampMode2,					// 31
	BattleFx_RenderMode0Or1,						// 32
	BattleFx_RenderMode2,							// 33
	BattleFx_RunMemberOrbit,						// 34
	BattleFx_RenderMode4,							// 35
	BattleFx_RenderMode3,							// 36
	BattleFx_RunBouncingSheep,						// 37
	BattleFx_RunSpinningTriangle,					// 38
	BattleFx_RunClosingSkulls,						// 39
	BattleFx_RunCounterReveal,						// 40
	BattleFx_RunTargetMarkers,						// 41
	BattleFx_RunGatheringMotes,						// 42
	BattleFx_RunIceShardBursts,						// 43
	BattleFx_RunSeriesAMode1,						// 44
	BattleFx_RunSeriesAMode1,						// 45
	BattleFx_RunSeriesBMode0,						// 46
	BattleFx_RunPaletteRampMode1,					// 47
	NULL,											// 48
	BattleEffect_RunDualParticleStream,				// 49
	BattleFx_RunMemberBurstMode0,					// 50
	BattleFx_RunFireSwirlCharge,					// 51
	BattleFx_RunRevealColumnMode1,					// 52
	BattleFx_RunFlippingBurst,						// 53
	BattleFx_RunMode1,								// 54
	BattleFx_RunFireBurstShards,					// 55
	BattleFx_RunDualTableMode1,						// 56
	BattleEffect_RunMode5WithAction,				// 57
	BattleFx_RunFlameBlade,							// 58
	BattleFx_RunMode6WithAction,					// 59
	BattleFx_RunLeapingStrike,						// 60
	BattleFx_RunFallingSword,						// 61
	BattleFx_RunSpinningStars,						// 62
	BattleFx_RunMode9WithAction,					// 63
	BattleFx_RenderMode7,							// 64
	BattleFx_RunRevealColumnMode2,					// 65
	BattleFx_RenderMode6,							// 66
	BattleFx_RunTriangleStrike,						// 67
	BattleFx_RunRingBolts,							// 68
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 69-76
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 77-84
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 85-92
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 93-100
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 101-108
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 109-116
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 117-124
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 125-132
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 133-140
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 141-148
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 149-156
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 157-164
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 165-172
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 173-180
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 181-188
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 189-196
	NULL, NULL, NULL,								// 197-199
	BattleFx_RunSeriesBMode3,						// 200
	BattleFx_RunTwoModeAMode0,						// 201
	BattleEffect_RunRisingColumns,					// 202
	BattleFx_RunVenusDjinnRockToss,					// 203
	BattleFx_RunSeriesAMode0,						// 204
	BattleFx_RunMode7,								// 205
	BattleFx_RunSeriesAMode1,						// 206
	BattleFx_RunParticleReveal,						// 207
	BattleFx_RunSeriesAMode5,						// 208
	BattleFx_RunTwoModeAMode1,						// 209
	BattleFx_RunSeriesBMode0,						// 210
	BattleFx_RunSeriesBMode1,						// 211
	BattleFx_RunSeriesAMode6,						// 212
	BattleFx_RunSeriesBMode5,						// 213
	BattleFx_RunRipplingDisc,						// 214
	BattleFx_RunSeriesBMode2,						// 215
	BattleFx_RunSeriesBMode4,						// 216
	ObjectGroup_StoreObjectAndRunStep7,				// 217
	BattleFx_RunSpinningTriangle,					// 218
	EffectStep_RunAnimationMode6Or7Or8,				// 219
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 220-227
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 228-235
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 236-243
	NULL, NULL, NULL, NULL, NULL, NULL,				// 244-249
	BattleFx_RunVenusDjinnColumns,					// 250
	BattleFx_RunMercuryDjinnFlames,					// 251
	BattleFx_RunMarsDjinnPuffs,						// 252
	BattleFx_RunJupiterDjinnFall,					// 253
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 254-261
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 262-269
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 270-277
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 278-285
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 286-293
	NULL, NULL, NULL, NULL, NULL, NULL,				// 294-299
	BattleFx_RunFiveModeMode0,						// 300
	BattleFx_RunFiveModeMode1,						// 301
	BattleFx_RunFiveModeMode2,						// 302
	BattleFx_RunMode2,								// 303
	BattleFx_RunFiveModeMode4,						// 304
	BattleFx_RunFiveModeMode3,						// 305
	BattleFx_RunMode4,								// 306
	BattleFx_RunMode3,								// 307
	BattleFx_RunMemberBurstMode1,					// 308
	BattleFx_RunSevenModeMode0,						// 309
	EffectStep_RunAnimationMode6Or7Or8,				// 310
	BattleFx_RunParticleFieldVariant0,				// 311
	BattleFx_RenderMode5,							// 312
	BattleFx_RunTwelveModeMode6,					// 313
	BattleFx_RunParticleFieldVariant1,				// 314
	BattleFx_RunSevenModeMode1,						// 315
	BattleFx_RunTwelveModeMode3,					// 316
	EffectStep_RunAnimationMode0,					// 317
	BattleFx_RunTwelveModeMode1,					// 318
	EffectStep_RunAnimationMode3Or4Or5,				// 319
	BattleFx_RunRisingBurst,						// 320
	EffectStep_RunAnimationMode0Alternate,			// 321
	BattleFx_RunSevenModeMode2,						// 322
	BattleFx_RunTwelveModeMode1,					// 323
	BattleFx_RunTwelveModeMode5,					// 324
	BattleFx_RunTwelveModeMode9,					// 325
	BattleFx_RunParticleFieldVariant2,				// 326
	BattleFx_RunTwelveModeMode4,					// 327
	EffectStep_RunAnimationMode1,					// 328
	EffectStep_RunAnimationMode3Or4Or5,				// 329
	BattleFx_RunSevenModeMode3,						// 330
	NULL,											// 331
	BattleFx_RunTwelveModeMode0,					// 332
	BattleFx_RunTwelveModeMode8,					// 333
	EffectStep_RunAnimationMode2,					// 334
	BattleFx_RunTwelveModeMode7,					// 335
	BattleFx_RunTwelveModeMode10,					// 336
	BattleFx_RunTwelveModeMode2,					// 337
	BattleFx_RunTwoModeBMode1,						// 338
	BattleFx_RunParticleFieldVariant3,				// 339
	BattleFx_RunSevenModeMode6,						// 340
	NULL,											// 341
	EffectStep_RunAnimationMode0,					// 342
	EffectStep_RunAnimationMode0Alternate,			// 343
	BattleFx_RunParticlePoolMode0,					// 344
	BattleFx_RunSpiderWeb,							// 345
	BattleFx_RenderMode4,							// 346
	BattleFx_RunSevenModeMode4,						// 347
	BattleFx_RunFortyEightFrameMode0,				// 348
	BattleFx_RunParticlePoolMode1,					// 349
	BattleFx_RunSevenModeMode5,						// 350
	BattleFx_RunMemberBurstMode2,					// 351
	BattleFx_RunSeriesAMode2,						// 352
	BattleFx_RunFortyEightFrameMode1,				// 353
	BattleFx_RunTargetMarkers,						// 354
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 355-362
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 363-370
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 371-378
	NULL,											// 379
	BattleFx_RunParticleFieldMode1,					// 380
	BattleFx_RunMode11,								// 381
	BattleFx_RunTwoResourceMode0,					// 382
	BattleFx_RunTwoResourceMode1,					// 383
	BattleFx_InitializeMode1,						// 384
	BattleFx_RenderAnimationMode1,					// 385
	BattleFx_RunTwelveModeMode11,					// 386
	BattleFx_RunSparkApproachVariant3,				// 387
	BattleFx_RunSparkApproachVariant4,				// 388
	BattleFx_RunFortyEightFrameMode2,				// 389
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // 390-397
	NULL, NULL,										// 398-399
	BattleFx_RunSeriesIMode0,						// 400
	BattleFx_RunSeriesIMode2,						// 401
	BattleFx_RunMode10,								// 402
	BattleFx_RunSeriesIMode6,						// 403
	BattleFx_RunSeriesIMode3,						// 404
	BattleFx_RunSeriesIMode5,						// 405
	BattleFx_RunSeriesIMode7,						// 406
	BattleFx_RunSeriesIMode4						// 407
};

// initialize_mode10.c
const u8  ThornVines_ObjectPoses[] = {8, 10, 9, 11, 3, 4};
const u16 ThornVines_ThornOffsets[] = {3920, 3932, 3947, 3955, 3970};
const u8  ThornVines_ThornWidths[] = {2, 5, 2, 5, 3};
const u8  ThornVines_ThornHeights[] = {6, 3, 4, 3, 3};
const u16 ThornVines_LeafOffsets[] = {3456, 3712, 3856};
const u8  ThornVines_LeafSizes[] = {16, 12, 8};
const u16 ThornVines_ChipOffsets[] = {3920, 3932, 3947, 3955, 3970};
const u8  ThornVines_ChipWidths[] = {2, 5, 2, 5, 3};
const u8  ThornVines_ChipHeights[] = {6, 3, 4, 3, 3};
const u16 ThornVines_SparkOffsets[] = {3979, 4043, 4268, 4389, 4470};
const u8  ThornVines_SparkSizes[] = {8, 15, 11, 9, 5};
const u16 ThornVines_EmberOffsets[] = {5760, 6016, 7040, 9344, 11648, 13952, 16256};
const u16 ThornVines_EmberSizes[] = {16, 32, 48, 48, 48, 48, 48};
const u8 BattleFx10_Points[][2] = {{57, 130}, {75, 128}, {71, 104}, {101, 130}, {86, 126}, {85, 98},
	{64, 98}, {98, 96}, {77, 87}, {84, 88}, {68, 67}, {81, 49}, {94, 70}, {70, 51}, {93, 36},
	{81, 50}};
const s8  BattleFx10_ShakeOffsets[] = {-4, -3, -2, 0};
const u16 BattleFx10_RockCells[] = {0, 1152, 2064};
const u8  BattleFx10_RockWidths[] = {24, 24, 20};
const u8  BattleFx10_RockHeights[] = {48, 38, 26};
const u8  BattleFx10_Animations[] = {2, 9, 10, 11};
const u8  BattleFx10_DebrisWidths[] = {12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7};
const u8  BattleFx10_DebrisHeights[] = {10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10};
const u16 BattleFx10_DebrisCells[] = {
	2584, 2704, 2880, 2992, 5062, 5094, 5142, 5157, 5185, 5227, 5299};
const u8  BattleFx10_SprayWidths[] = {16, 16, 16};
const u8  BattleFx10_SprayHeights[] = {16, 16, 16};
const u16 BattleFx10_SprayCells[] = {4294, 4550, 4806};
const u16 BattleFx10_BoulderCells[] = {0, 1152, 2064};
const u8  BattleFx10_BoulderWidths[] = {24, 24, 20};
const u8  BattleFx10_BoulderHeights[] = {48, 38, 26};
const u8  BattleFx10_FallWidths[] = {24, 24, 20};
const u8  BattleFx10_FallHeights[] = {48, 38, 26};
const u16 BattleFx10_FallCells[] = {0, 1152, 2064};
const s32 IceShardBursts_Gravities[] = {0x4000, 0x2000, 0x4000, 0x8000};

// palette_ramp.c
const u16 PaletteRamp_ShardCells[] = {0, 120, 180, 258, 298, 352, 408, 471, 567, 607, 631, 661};
const u8  PaletteRamp_ShardWidths[] = {10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7};
const u8  PaletteRamp_ShardHeights[] = {12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5};

// rockwall.c
const u8 RockWall_Timings[][3] = {{80, 32, 2}, {112, 64, 4}, {144, 96, 8}};

// homing.c
const u8  HomingEmbers_Counts[] = {16, 32, 64};
const u8  HomingEmbers_FlareWidths[] = {24, 29, 30, 27, 24, 16};
const u8  HomingEmbers_FlareHeights[] = {23, 32, 37, 49, 53, 52};
const u8  HomingEmbers_FlareBiasY[] = {41, 32, 26, 11, 4, 2};
const u16 HomingEmbers_FlareCells[] = {0, 552, 1480, 2590, 3913, 5185};

// motes.c
const u8  RisingMotes_ColumnSpots[] = {53, 108, 73, 110, 29, 110, 73, 108, 33, 104, 96, 106, 8, 109,
	62, 98, 33, 100, 87, 100, 21, 95, 53, 90, 33, 90, 72, 91, 82, 91, 77, 86, 52, 84, 18, 92, 56,
	81};
const u8  RisingMotes_Timings[] = {32, 8, 85, 48, 12, 97, 64, 19, 109};
const u8  RisingMotes_MoteWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
const u8  RisingMotes_MoteHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
const u16 RisingMotes_MoteCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};
const u16 RisingMotes_ColumnCells[] = {0, 768, 1280};
const u8  RisingMotes_ColumnHeights[] = {24, 16, 16};
const u8  RisingMotes_EmberWidths[] = {12, 10, 7, 10, 8, 8, 8, 8};
const u8  RisingMotes_EmberHeights[] = {9, 16, 14, 15, 8, 8, 8, 8};
const u16 RisingMotes_EmberCells[] = {1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500};

// bolts.c
const u8 LightningBolts_Sparks[][2] = {{2, 4}, {2, 6}, {4, 8}};

// particle_field.c
const u8  ParticleField_Counts[][3] = {{16, 8, 72}, {32, 16, 80}, {128, 32, 88}};
const u16 ParticleField_PuffCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
const u16 ParticleField_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// 参照されないセル配置（6x6 から 20x20 まで）
const u16 Unused_SheetCells6[] = {0, 36, 100, 200, 344, 540};
const u16 Unused_SheetCells[] = {0, 36, 100, 200, 344, 540, 796, 1120, 1520, 1900, 2300};
const u8  Unused_SheetWidths[] = {6, 8, 10, 12, 14, 16, 18, 20, 19, 20};
const u8  Unused_SheetHeights[] = {6, 8, 10, 12, 14, 16, 18, 20, 20, 20};

// staged_particles.c
const Scale StagedParticles_UnitScale = {0x10000, 0x10000};

// columns.c
const u8  DualTable_VineWidths[] = {21, 28, 19};
const u8  DualTable_HeightLimits[] = {119, 119, 102};
const u16 DualTable_VineCells[] = {0, 2499, 5831};
const u8  DualTable_PillarWidths[] = {10, 8, 9};
const u16 DualTable_PillarCells[] = {0, 1080, 1888};
const u8  DualTable_ColumnCounts[] = {4, 8, 12};
const u8  DualTable_ColumnX[] = {64, 72, 56, 80, 48, 88, 40, 96, 32, 104, 24, 72, 56, 80, 48, 88};
const s8  DualTable_PillarHeights[] = {100, 60, 88, 96, 64, 104, 82, 98};
const s8  DualTable_PuffWidths[] = {5, 7, 11, 16, 23, 27, 16};
const u8  DualTable_PuffHeights[] = {10, 14, 21, 32, 45, 50, 32};
const u16 DualTable_PuffCells[] = {0, 50, 148, 379, 891, 1926, 3276};
const s8  RisingColumns_ColumnOffsets[] = {
	0, 14, -32, 8, 32, -48, 4, 12, -24, 24, -12, -40, -4, -16, -32, 12};
const u8  BattleFxPillar_Kinds[] = {0, 3, 2, 1, 2, 3, 2, 1};
const s8  BattleFxPillar_X[] = {24, 80, 24, 48, 88, 40, 8, 96};
const u8  BattleFxPillar_Counts[] = {3, 5, 8};
const s8  BattleFxPillar_PuffWidths[] = {5, 7, 11, 17, 32, 32, 19};
const u8  BattleFxPillar_PuffHeights[] = {10, 14, 21, 34, 59, 60, 38};
const u16 BattleFxPillar_PuffCells[] = {5888, 5938, 6036, 6267, 6845, 8733, 10653};

// lightning_bolts.c
const u8 LightningBolts_Counts[] = {16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3};
const u8 LightningBolts_GlintPalettes[] = {0, 4, 8, 12};
const u8 LightningBolts_GlintModes[] = {2, 2, 3};

// approach.c
const u8  Volley_CometHeights[] = {24, 36, 48};
const u8  Volley_Modes[][5] = {{6, 10, 3, 32, 4}, {4, 10, 5, 32, 8}, {10, 32, 3, 32, 4},
	{5, 10, 5, 32, 4}, {10, 10, 1, 32, 4}, {1, 5, 0, 32, 80}, {7, 8, 2, 32, 40}, {30, 24, 1, 32, 4},
	{6, 2, 3, 32, 4}, {60, 10, 1, 32, 4}, {60, 10, 1, 32, 4}, {10, 10, 1, 32, 4}};
const u8  Volley_FlashWidths[] = {12, 18, 24, 30, 36, 39, 40};
const u8  Volley_FlashHeights[] = {10, 15, 18, 22, 27, 28, 30};
const u8  Volley_FlashSteps[] = {2, 3, 4, 4, 5, 6, 6};
const u8  Volley_FlashRises[] = {0, 3, 5, 7, 10, 12, 12};
const u16 Volley_FlashOffsets[] = {8192, 8312, 8582, 9014, 9674, 10646, 11738};
const u16 Volley_PelletOffsets[] = {2308, 2372, 2436, 2500};

// djinn_bursts.c
const u8 MercuryDjinnFlames_LaunchFrames[] = {4, 7, 10, 21, 34};

// particle_reveal.c
const u8  ParticleReveal_CellWidths[] = {8, 8, 8, 8};
const u8  ParticleReveal_CellHeights[] = {8, 8, 8, 8};
const u16 ParticleReveal_CellSourceOffsets[] = {2308, 2372, 2436, 2500};
const u8  RockToss_ChipFlips[] = {3, 7, 11, 15};
const u8  RockToss_ChipWidths[] = {24, 7, 7, 8, 6, 13, 10, 11, 7};
const u8  RockToss_ChipHeights[] = {48, 13, 14, 24, 24, 17, 20, 14, 20};
const u16 RockToss_ChipCells[] = {0, 1152, 1243, 1341, 1533, 1677, 1898, 2098, 2252};
const u8  RockToss_ChipX[] = {0, 9, 9, 11, 10, 0, 3, 13, 7};
const u8  RockToss_ChipY[] = {0, 18, 18, 0, 24, 20, 5, 22, 26};

// shatter_rocks.c
const u8  Mode12_Animations[] = {7, 14, 10, 4};
const u8  Mode12_SmokeFlips[] = {0, 4, 8, 12};
const u8  ShatterRocks_ShardOffsets[32 * 2] = {65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56,
	88, 51, 80, 61, 75, 72, 81, 69, 71, 66, 70, 71, 64, 49, 71, 59, 69, 53, 68, 78, 73, 55, 67, 75,
	62, 61, 65, 53, 61};
const s8  ShatterRocks_RockX[] = {0, -12, 12, -8, 16};
const u8  ShatterRocks_DropFrames[] = {8, 16, 20, 24, 32};
const u8  ShatterRocks_RockCounts[] = {1, 3, 5};
const u8  ShatterRocks_ShardWidths[] = {5, 11, 10, 7, 7, 12, 13, 9, 19, 12, 17, 22, 15, 16, 14};
const u8  ShatterRocks_ShardHeights[] = {11, 6, 10, 11, 10, 7, 17, 21, 12, 21, 17, 13, 16, 16, 15};
const u16 ShatterRocks_ShardCells[] = {
	0, 55, 121, 221, 298, 368, 452, 673, 862, 1090, 1342, 1631, 1917, 2157, 2413, 2623};

// burst_scene.c
const u8  BurstScene_Records[] = {1, 0, 5, 16, 12, 80, 0, 1, 1, 7, 16, 12, 104, 0, 1, 2, 9, 16, 12,
	128, 0, 1, 3, 1, 16, 16, 36, 0, 1, 3, 2, 20, 16, 52, 0, 1, 3, 3, 24, 16, 68, 0, 0, 2, 1, 32, 12,
	28, 1, 0, 2, 2, 32, 12, 40, 1, 0, 2, 3, 32, 12, 52, 1};
const u16 RisingWall_RowSheetOffsets[] = {0, 160, 352, 640, 1024, 1504, 2080, 2752, 3776};
const u8  RisingWall_RowWidths[] = {5, 6, 9, 12, 15, 18, 21, 32, 40};
const u16 RisingWall_MoundSheetOffsets[] = {10672, 11505, 12550};
const u8  RisingWall_MoundWidths[] = {17, 19, 20};
const u8  RisingWall_MoundHeights[] = {49, 55, 64};
const s8  RisingWall_Bg2Shifts[][3] = {{-8, 0, 12}, {-72, -40, -16}};
const u8  RisingWall_LateRockColumns[] = {136, 40, 120, 56, 104, 72};

// fxstep.c
const u8  BeamSequence_CellWidths[] = {38, 54, 71, 74, 77, 65};
const u8  BeamSequence_CellHeights[] = {7, 9, 11, 13, 13, 11};
const u16 BeamSequence_CellSheetOffsets[] = {0, 266, 752, 1533, 2495, 3496};
const u8  BeamSequence_CellReaches[] = {58, 42, 25, 13, 5, 0};

// casting.c
const u8  CastingImpact_GlintDrawFlags[] = {0, 4, 8, 12};
const u8  CastingImpact_ImageX[] = {10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0};
const u8  CastingImpact_ImageY[] = {0, 20, 2, 2, 34, 64, 68};
const u16 CastingImpact_OrbitCells[] = {2308, 2372, 2436, 2500, 0};

// ripple.c
const u8  FireSwirl_CellWidths[] = {26, 46, 63, 77, 60, 39, 32};
const u8  FireSwirl_CellHeights[] = {11, 20, 37, 65, 65, 54, 39};
const u16 FireSwirl_CellSourceOffsets[] = {0, 286, 1206, 3537, 8542, 12442, 14548};
const s8  FireSwirl_CellBiasX[] = {53, 36, 16, 2, 2, 2, 2};
const s8  FireSwirl_CellBiasY[] = {2, 3, 3, 3, 4, 14, 29};

// place_formation_objects.c
const u8  Formation_TwelveX[] = {52, 25, 57, 89, 7, 39, 71, 103, 21, 53, 85, 117};
const u8  Formation_TwelveZ[] = {0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96};
const u8  Formation_RightEightX[] = {0, 32, 64, 16, 48, 0, 32, 64};
const u8  Formation_RightEightZ[] = {0, 0, 0, 32, 32, 64, 64, 64};
const u8  Formation_SmallEightX[] = {0, 40, 80, 0, 40, 80, 40, 80};
const u8  Formation_SmallEightZ[] = {13, 0, 0, 53, 40, 40, 80, 80};
const u8  ImpactBurst_CellWidths[] = {48, 48, 46, 36, 36, 32, 26, 6};
const u8  ImpactBurst_CellHeights[] = {96, 96, 90, 74, 74, 62, 52, 12};
const u16 ImpactBurst_CellSourceOffsets[] = {0, 4608, 9216, 13356, 16020, 18684, 20668, 22020};

// scenefx.c
const s16 ParticleStreams_OrbitPoints[][3] = {{-64, -96, 24}, {-48, -120, 32}, {-32, -80, -48},
	{-16, -72, -24}, {16, -80, 16}, {32, -112, -32}, {48, -96, 56}};
const s16 ParticleStreams_DropPoints[][2] = {
	{32, 32}, {48, 96}, {80, 80}, {112, 100}, {176, 80}, {196, 64}, {204, 96}};
const u16 ParticleStreams_FlashSheetOffsets[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
const u16 ParticleStreams_FlashSizes[] = {16, 32, 48, 48, 48, 48, 48};
const u8  CirclingScene_ObjectColumns[] = {12, 44, 28, 60, 0, 32, 64, 0, 32};
const u8  CirclingScene_ObjectRows[] = {0, 0, 32, 32, 64, 64, 64, 32, 32};
const u16 CirclingScene_PuffSheetOffsets[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
const u16 CirclingScene_PuffSizes[] = {16, 32, 48, 48, 48, 48, 48};

// flame_blade.c
const u8 FlameBlade_StrikeColumns[] = {32, 24, 0, 96, 104, 128};
const u8 FlameBlade_FlashCells[] = {5, 5, 4, 3, 2, 1};

// falling_sword.c
const u8  FallingSword_FlashCells[] = {5, 5, 4, 3, 2, 1};
const s32 FallingSword_DustGravity[] = {0x8000, 0x4000, 0x4000, 0x2000};
const u8  ShockwaveScene_BurstHeights[] = {2, 3, 4, 6, 10, 11, 12};
const u16 ShockwaveScene_BurstCells[] = {0, 8, 26, 58, 130, 330, 572};
const u16 ShockwaveScene_ShardCells[] = {2396, 2985, 3698, 4411, 4810, 5146};
const u8  ShockwaveScene_ShardWidths[] = {19, 23, 23, 19, 16, 17};
const u8  ShockwaveScene_ShardHeights[] = {31, 31, 31, 21, 21, 24};

// object_row.c
const u8 ObjectRow_Columns[] = {8, 40, 72, 104, 8, 40, 72, 104, 0};
const u8 ObjectRow_Rows[] = {0, 0, 0, 24, 32, 32, 32, 56, 64};

// swoop.c
const u8  BattleFx6_ObjectX[] = {0, 32, 64, 0, 32, 64, 96, 96};
const u8  BattleFx6_ObjectY[] = {0, 0, 0, 32, 32, 32, 0, 32};
const s32 BattleFx6_Gravity[] = {0x8000, 0x4000, 0x4000, 0x2000};

// colburst.c
const u16 RisingBurst_SparkCells[] = {0, 256, 1280, 3584, 5888, 8192, 10496};
const u16 RisingBurst_SparkSizes[] = {16, 32, 48, 48, 48, 48, 48};
