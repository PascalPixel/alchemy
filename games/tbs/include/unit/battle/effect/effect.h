#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

typedef struct
{
	s32 reserved_00[3];
	s32 current_value;
	s32 index_radius;
	s32 entry_limit;
	s32 reserved_18[3];
	s16 values[1];
} BattleEffectIndexWindow;

void BattlePres_ConfigureEffectDisplay(void);
void BattleFx_AdvanceScrollOnInterval(void);
void Camera_AdvanceBg2Reference(void);
void BattleFx_ArmWin0HBlankDma(void);
void BattleEffect_RunFallingParticles(struct BattleEffectArgument *effect);
void BattleEffect_RunFallingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode1B(struct BattleEffectArgument *effect);
void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunParticlePoolMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticlePoolMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticlePool(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunTwelveModeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode10(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode5(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode9(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode8(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode7(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode11(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect);
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect);
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect);
void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect);
void BattlePresentation_DrawStreaks(void);
void BattlePresentation_PrepareScene(s32 kind);
void BattleFx_ScheduleCallbacksAndReleaseBlocks(void);
void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx);
void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx);
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode);
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx);
void BattleEffect_WipeCanvas(s32 mode, s32 value);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_FlushPendingGraphicsTransfer(void);
void Graphics_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleFx_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void BattleFx_OpenCanvasLayer(s32 bg_control);
void BattleFx_BeginTiledCanvas(s32 bg_control);
void BattleFx_EndCanvasLayer(void);
void BattleFx_SetTransitionFlagAndDisplay(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void BattleFx_RunLeapingStrike(struct BattleEffectArgument *effect);
void BattleFx_RunClosingSkulls(struct BattleEffectArgument *effect);
void BattleFx_RunMemberOrbit(void *effect);
void BattleFx_RunMemberBurstMode0(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode1(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode2(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_FetchRectangleBlitters(s32 flip, BattleEffectDrawRectangle *out);
void BattleFx_RunFortyEightFrameMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode0(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode2(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunTwoModeAMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeAMode1(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSevenModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunCounterReveal(struct BattleEffectArgument *object);
void Graphics_ConvertBackgroundToBlueRamp(void);
void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect);
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect);
void BattleFx_RunRingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect);
void BattleFx_RunWhirlwindComets(struct BattleEffectArgument *argument);
void BattleFx_RenderAnimationMode1(s32 effect);
void BattleFx_RenderAnimationMode0(s32 effect);
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect);
void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect);
void BattleFx_RunTornado(struct BattleEffectArgument *effect);
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeBMode0(s32 effect);
void BattleFx_RunTwoModeBMode1(s32 effect);
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind);
void BattleFx_RunFirePillars(struct BattleEffectArgument *effect);
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect);
void BattleFx_RenderMode1(struct BattleEffectArgument *effect);
void BattleFx_RenderMode2(struct BattleEffectArgument *effect);
void BattleFx_RenderMode3(struct BattleEffectArgument *effect);
void BattleFx_RenderMode4(struct BattleEffectArgument *effect);
void BattleFx_RenderMode5(struct BattleEffectArgument *effect);
void BattleFx_RenderMode7(struct BattleEffectArgument *effect);
void BattleFx_RenderMode6(struct BattleEffectArgument *effect);
void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunBouncingSheep(void *object);
void BattleFx_RunTargetMarkers(struct BattleEffectArgument *efx);
void BattleFx_RunSparkTravel(struct BattleEffectArgument *effect);

// 粒の置き場：Ram_MapCellBuffer を struct EffectStep の並びとして使う
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y) // 上下左右ではみ出しを切る

#define HI(v) (((s16 *)&(v))[1])

struct Placement
{
	s32 x;
	s32 y;
	s32 z;
	s32 unknown_0c;
};

// 2 次元の組 {x, y}（位置や速さ）
struct Point2D
{
	s32 x;
	s32 y;
};

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

#define gBlitters ((DrawRectangle *)gWorkSlot)

#endif
