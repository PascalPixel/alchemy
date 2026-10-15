#include "battle_effect_runtime.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "objcmd.h"
#include "types.h"

//--------------------------------------------------------------------------
// 演出の表
// Effect tables: object scripts, curves and lookup tables of the OBJECT bank.
//--------------------------------------------------------------------------

// 読む側は battle.h, field.h, graphics.h, scroll.h と object_effect_scripts.h、
// 色の曲線は object.c が宣言する。次の表は読む側が const なしで宣言する:
// battle.h の BattleFx_TargetRangeByMode, Animation_ChildPaletteCycle,
// BattleFx_DefinitionTable, FieldFx_GroundParticleFrames, BattleFx_PulseScales,
// BattleFx_CyclePatternWords, BattleFx_RandomChildValues、graphics.h の
// DisplayTransition_NoiseWidths, DisplayTransition_EdgeOffsets、field.h の
// ObjectGroup_BlinkChildValues、object.c の BattlePalette_ChannelCurve 3 つ。
// それでも定義はすべて const: rom_main_04 では .data が scripts.o の後に
// 置かれ、この表が動くから。だからこのファイルはそれらのヘッダを読まない。

// 調べた床の文章番号の差 (MsgTileChecked, MsgTileResult に足す)
const u8 Field_TileMessageOffsets[6] =
{
	2,	// ツボ
	3,	// タル
	13, // 木ばこ
	8,	// おはか
	30, // ほん
	31, // いど
};

// {方式, 探す範囲 (ドット)}, -1 で終わり。ない方式は 16
const s16 BattleFx_TargetRangeByMode[25] =
{
	3, 16,
	6, 16,
	7, 16,
	11, 16,
	12, 16,
	2, 32,
	1, 32,
	14, 32,
	5, 32,
	9, 32,
	4, 48,
	13, 48,
	-1,
};

// 子のパレット番号 ((gFrameCount >> 1) & 7)
const u8 Animation_ChildPaletteCycle[8] = {1, 2, 3, 4, 3, 2, 1, 0};

// 半分の大きさから 2 フレームごとに広がる
const s32 BattleFx_ParticleEmitterScript[] =
{
	SC_SET(SW_SCALE_X, 0x8000),
	SC_SET(SW_SCALE_Y, 0x8000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xc000),
	SC_SET(SW_SCALE_Y, 0xc000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xe000),
	SC_SET(SW_SCALE_Y, 0xe000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xf000),
	SC_SET(SW_SCALE_Y, 0xf000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_END,
};

// 2 度脈打って止まる
const s32 BattleFx_RisingPulseScript[] =
{
	SC_WAIT(10),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_WAIT(50),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

// 60 フレーム上がって消える
const s32 BattleFx_ParticleScript[] =
{
	SC_ADD(SW_Y, -0x4000),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_FREE,
};

const s32 BattleFx_MarkerParticleScript[] =
{
	SC_CALL(EffectRuntime_IsActive),
	SC_FREE,
};

// 画面切り替えの雑音の幅
const u8 DisplayTransition_NoiseWidths[34] =
{
	0, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 11, 13,
	16, 18, 22, 26, 30, 36, 43, 51, 60, 72, 85, 101, 120, 143, 170, 202, 240,
};

// 円の縁の揺れ
const s8 DisplayTransition_EdgeOffsets[32] =
{
	-4, -3, -3, -3, -2, -2, -1, 0, 0, 0, 1, 2, 2, 3, 3, 3,
	4, 3, 3, 3, 2, 2, 1, 0, 0, 0, -1, -2, -2, -3, -3, -3,
};

// 0 から 63 の並べ替え: 色を変える順
const u8 DisplayTransition_DitherTable[64] =
{
	0, 12, 35, 3, 29, 39, 6, 7, 50, 59, 4, 11, 18, 40, 46, 31,
	16, 42, 47, 51, 20, 23, 43, 32, 8, 52, 17, 41, 54, 15, 63, 24,
	58, 5, 2, 26, 36, 61, 9, 34, 56, 27, 10, 48, 53, 30, 28, 22,
	62, 45, 19, 60, 57, 49, 25, 55, 38, 14, 44, 13, 33, 37, 1, 21,
};

// 色成分の曲線 (0 から 31 を 0x7c00 までに)
const u16 BattlePalette_ChannelCurve[32] =
{
	0x0000, 0x020c, 0x042f, 0x066b, 0x08c0, 0x0b2f, 0x0db9, 0x105f,
	0x1322, 0x1603, 0x1903, 0x1c22, 0x1f63, 0x22c5, 0x264b, 0x29f4,
	0x2dc3, 0x31b9, 0x35d7, 0x3a1d, 0x3e8e, 0x432b, 0x47f6, 0x4cee,
	0x5217, 0x5772, 0x5d00, 0x62c3, 0x68bc, 0x6eed, 0x7559, 0x7c00,
};

const u16 BattlePalette_ChannelCurveLight[32] =
{
	0x0000, 0x0265, 0x04df, 0x076e, 0x0a14, 0x0cd1, 0x0fa5, 0x1291,
	0x1596, 0x18b4, 0x1bec, 0x1f3f, 0x22ac, 0x2636, 0x29dc, 0x2da0,
	0x3182, 0x3583, 0x39a3, 0x3de3, 0x4245, 0x46c9, 0x4b70, 0x503a,
	0x5529, 0x5a3d, 0x5f78, 0x64da, 0x6a65, 0x7019, 0x75f7, 0x7c00,
};

const u16 BattlePalette_ChannelCurveContrast[32] =
{
	0x0000, 0x0168, 0x02f7, 0x04b1, 0x0699, 0x08b2, 0x0b01, 0x0d8a,
	0x1052, 0x135d, 0x16b0, 0x1a52, 0x1e48, 0x2298, 0x274a, 0x2c65,
	0x31f0, 0x37f5, 0x3e7c, 0x458e, 0x4d37, 0x5580, 0x5e77, 0x6826,
	0x729c, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
};

// {番号, 値 (0xff はなし), フラグ}: BattleFx_FindDefinition は 131 番目
// までを探し、最後の組は見つからないときのもの
const struct BattleEffectEntry BattleFx_DefinitionTable[131] =
{
	{0, 16, 0x00},
	{1, 16, 0x00},
	{2, 16, 0x00},
	{3, 12, 0x00},
	{4, 28, 0x00},
	{5, 12, 0x00},
	{6, 12, 0x00},
	{18, 8, 0x00},
	{19, 8, 0x00},
	{30, 24, 0x00},
	{31, 20, 0x00},
	{32, 28, 0x00},
	{33, 20, 0x00},
	{34, 28, 0x00},
	{35, 28, 0x00},
	{36, 16, 0x00},
	{37, 8, 0x00},
	{38, 20, 0x00},
	{39, 12, 0x00},
	{40, 28, 0x00},
	{41, 16, 0x00},
	{42, 12, 0x00},
	{43, 28, 0x00},
	{44, 28, 0x00},
	{45, 20, 0x00},
	{46, 28, 0x00},
	{47, 28, 0x00},
	{48, 4, 0x00},
	{49, 24, 0x00},
	{50, 28, 0x00},
	{51, 24, 0x00},
	{52, 28, 0x00},
	{53, 28, 0x00},
	{54, 28, 0x00},
	{55, 24, 0x00},
	{71, 28, 0x00},
	{56, 20, 0x00},
	{57, 28, 0x00},
	{58, 28, 0x00},
	{59, 28, 0x00},
	{60, 20, 0x00},
	{61, 20, 0x00},
	{62, 28, 0x00},
	{63, 28, 0x00},
	{64, 28, 0x00},
	{65, 24, 0x00},
	{66, 16, 0x00},
	{67, 28, 0x00},
	{68, 24, 0x00},
	{69, 28, 0x00},
	{93, 16, 0x00},
	{90, 16, 0x00},
	{92, 16, 0x00},
	{91, 16, 0x00},
	{101, 28, 0x00},
	{102, 4, 0x00},
	{103, 24, 0x00},
	{104, 0, 0x00},
	{106, 12, 0x00},
	{107, 8, 0x00},
	{108, 16, 0x00},
	{109, 28, 0x00},
	{111, 20, 0x00},
	{114, 28, 0x00},
	{115, 20, 0x00},
	{116, 28, 0x00},
	{117, 24, 0x00},
	{118, 12, 0x00},
	{119, 28, 0x00},
	{120, 20, 0x00},
	{121, 24, 0x00},
	{122, 28, 0x00},
	{123, 28, 0x00},
	{124, 28, 0x00},
	{125, 28, 0x00},
	{128, 20, 0x00},
	{129, 16, 0x00},
	{130, 16, 0x00},
	{131, 12, 0x00},
	{132, 28, 0x00},
	{133, 16, 0x00},
	{134, 28, 0x00},
	{135, 28, 0x00},
	{136, 0, 0x00},
	{144, 28, 0x00},
	{145, 28, 0x00},
	{146, 28, 0x00},
	{147, 28, 0x00},
	{148, 12, 0x00},
	{149, 16, 0x00},
	{150, 28, 0x00},
	{151, 8, 0x00},
	{152, 28, 0x00},
	{153, 28, 0x00},
	{155, 0, 0x00},
	{156, 16, 0x00},
	{157, 28, 0x00},
	{158, 28, 0x00},
	{159, 16, 0x00},
	{160, 28, 0x00},
	{162, 20, 0x00},
	{163, 28, 0x00},
	{164, 24, 0x00},
	{165, 16, 0x00},
	{166, 20, 0x00},
	{167, 24, 0x00},
	{168, 8, 0x00},
	{169, 4, 0x00},
	{170, 28, 0x00},
	{171, 28, 0x00},
	{187, 28, 0xff},
	{189, 20, 0xff},
	{190, 28, 0xff},
	{191, 28, 0xff},
	{192, 16, 0xff},
	{193, 16, 0xff},
	{194, 24, 0xff},
	{195, 28, 0xff},
	{196, 0, 0xff},
	{197, 12, 0xff},
	{198, 28, 0xff},
	{199, 24, 0xff},
	{20, 255, 0xff},
	{200, 255, 0xff},
	{105, 255, 0xff},
	{110, 255, 0xff},
	{274, 255, 0xff},
	{231, 255, 0xff},
	{293, 255, 0xff},
	{22, 255, 0xff},
	{0, 16, 0x00},
};

// 方式 (1 から 3) の回数だけ揺れる: (3 - 方式) * 32 語目から始めて終わりまで
// field.h は const u8[] と宣言し、psynergy_effects.c はバイトで (3 - 方式) << 7 を足して s32 * で読む
const s32 ObjectMotion_VariantScripts[97] =
{
	// 方式 3
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x800),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x800),
	SC_WAIT(4),
	// 方式 2
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x800),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x800),
	SC_WAIT(4),
	// 方式 1
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x800),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x800),
	SC_WAIT(4),
	SC_END,
};

// 子の値 ((gFrameCount >> 1) & 3)
const u8 ObjectGroup_BlinkChildValues[4] = {7, 0, 0, 0};

// 正弦 (4096 倍, 256 で一周)
const s16 DisplayScroll_WaveSine[256] =
{
	0, 100, 200, 301, 401, 501, 601, 700,
	799, 897, 995, 1092, 1189, 1284, 1379, 1474,
	1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191,
	2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824,
	2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348,
	3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744,
	3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996,
	4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094,
	4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035,
	4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821,
	3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460,
	3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966,
	2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358,
	2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659,
	1567, 1474, 1379, 1284, 1189, 1092, 995, 897,
	799, 700, 601, 501, 401, 301, 200, 100,
	0, -100, -200, -301, -401, -501, -601, -700,
	-799, -897, -995, -1092, -1189, -1284, -1379, -1474,
	-1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191,
	-2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824,
	-2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348,
	-3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744,
	-3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996,
	-4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094,
	-4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035,
	-4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821,
	-3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460,
	-3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966,
	-2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358,
	-2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659,
	-1567, -1474, -1379, -1284, -1189, -1092, -995, -897,
	-799, -700, -601, -501, -401, -301, -200, -100,
};

// {dx, dy, タイル, 形, 大きさ} x 16
const s16 FieldFx_GroundParticleFrames[80] =
{
	-8, -12, 20, 0, 1,
	-8, -12, 20, 0, 1,
	-8, -12, 20, 0, 1,
	-8, -12, 16, 0, 1,
	-8, -12, 16, 0, 1,
	-8, -12, 12, 0, 1,
	-8, -12, 8, 0, 1,
	0, -32, 0, 2, 2,
	8, -48, 0, 2, 2,
	16, -64, 0, 2, 2,
	24, -80, 0, 2, 2,
	32, -96, 0, 2, 2,
	40, -112, 0, 2, 2,
	48, -128, 0, 2, 2,
	56, -144, 0, 2, 2,
	64, -160, 0, 2, 2,
};

// {dy, タイル}
const struct SparkleFrame FieldFx_SparkleFrames[32] =
{
	{-2, 12},
	{-4, 12},
	{-6, 12},
	{-8, 8},
	{-10, 8},
	{-12, 8},
	{-14, 8},
	{-16, 8},
	{-19, 4},
	{-21, 4},
	{-24, 4},
	{-27, 4},
	{-30, 4},
	{-33, 4},
	{-36, 4},
	{-40, 4},
	{-44, 4},
	{-48, 0},
	{-52, 0},
	{-56, 0},
	{-60, 0},
	{-64, 0},
	{-68, 0},
	{-72, 0},
	{-76, 0},
	{-80, 0},
	{-85, 0},
	{-90, 0},
	{-95, 0},
	{-100, 0},
	{-110, 0},
	{-120, 0},
};

// 大きさを 2 つ交互に ((gFrameTick >> 2) & 1)。3 語目は読まれない
const u32 BattleFx_PulseScales[3] = {0x10000, 0xf851, 16};

// [0] は解放だけ、+1 は到着を待って解放。+3 は次の BattleFx_SourceHoldScript
const s32 BattleFx_CommonParticleScript[] =
{
	SC_FREE,
	SC_SYNC,
	SC_FREE,
};

const s32 BattleFx_SourceHoldScript[] =
{
	SC_FACE_LINK,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 BattleFx_FragmentScript[] =
{
	SC_SYNC,
	SC_SET(SW_SPEED, 0x1000),
	SC_ARG(0),
	SC_WAIT(10),
	SC_FREE,
};

// 十字キーの向き: 右・左・上・下の 4 ビットごと、0xffff は向きなし
const u16 BattleFx_CyclePatternWords[16] =
{
	0xffff, 0x0000, 0x8000, 0xffff,
	0xc000, 0xffff, 0xffff, 0xc000,
	0x4000, 0xffff, 0xffff, 0x4000,
	0xffff, 0x0000, 0x8000, 0xffff,
};

const u8 BattleFx_QuadrantModes[4] = {3, 1, 2, 1};

// +4 語は BattleFx_HeavyImpactScript
const s32 BattleFx_BurstParticleObjectScript[] =
{
	SC_WAIT(60),
	SC_FREE,
};

// BattleFx_QuadrantModes と同じ 4 バイト。読む所はない
const u8 Unused_BattleFxBytes[4] = {3, 1, 2, 1};

const s32 BattleFx_HeavyImpactScript[] =
{
	SC_SYNC,
	SC_CALL(SpawnHeavyImpactEffect),
	SC_FREE,
};

// gEffectScripts の 3 つ
const s32 BattleFx_ShortEffectScript[] =
{
	SC_WAIT(24),
	SC_FREE,
};

const s32 BattleFx_MediumEffectScript[] =
{
	SC_WAIT(46),
	SC_FREE,
};

const s32 BattleFx_LongEffectScript[] =
{
	SC_WAIT(128),
	SC_FREE,
};

const s8 BattleFx_RandomChildValues[8] = {0, 0, 1, 2, 3, 4, 5, 6};

// 目印の明るさ ((gFrameCount >> 1) & 31)
const u8 WorldMap_MarkerBlendCycle[32] =
{
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 15, 15, 14, 12, 10, 9, 9, 8, 9, 9, 10, 12, 14, 14, 15,
};

// 十字キーの向き (8 方向)、0xffff は向きなし
const u16 WorldMap_CursorDirectionAngles[16] =
{
	0xffff, 0x0000, 0x8000, 0xffff,
	0xc000, 0xe000, 0xa000, 0xc000,
	0x4000, 0x2000, 0x6000, 0x4000,
	0xffff, 0x0000, 0x8000, 0xffff,
};
