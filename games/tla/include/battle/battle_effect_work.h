#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

/// @file
/// The Lost Age's copy of the battle effect work.
// The Broken Seal's block with the 64 particles in front of the sprite sheet,
// the two affine scales after the BG2 reference point, 64 object slots and one
// more word before the effect.

#include "field/particle.h"
#include "battle/battle_effect_params.h"
#include "static_assert.h"
#include "system/io_reg.h"
#include "types.h"

/// How the canvas transfer treats the canvas after each copy (a battle
/// effect's BattleEffectWork.transferMode, Lucky Wheels' ReelEffectWork's);
/// transferValue is its argument.
enum CanvasTransferMode {
  CANVAS_COPY,             ///< copy as drawn
  CANVAS_COPY_AND_CLEAR,   ///< copy, then fill the canvas with transferValue (0
                           ///< clears it)
  CANVAS_COPY_AND_FADE,    ///< copy, keeping half of each pixel's level for the
                           ///< next frame when transferValue is
                           ///< CANVAS_FADE_HALF, and three quarters for any
                           ///< other value
  CANVAS_COPY_AND_DARKEN,  ///< copy, then lower each level by a byte of
                           ///< transferValue, stopping at 0
  CANVAS_COPY_AND_BRIGHTEN ///< copy, then raise each level by a byte of
                           ///< transferValue
};

/// CANVAS_COPY_AND_FADE's transferValue: the percentage of each pixel's level
/// kept for the next frame.
enum CanvasFadeShare { CANVAS_FADE_HALF = 50, CANVAS_FADE_THREE_QUARTERS = 75 };

/// The running battle effect's work: its canvas image, 64 particles, the
/// display registers it stages and its sprites. It lives in the
/// HEAP_SLOT_BATTLE_EFFECT heap slot.
struct BattleEffectWork {
  struct Particle particles[64];
  u8 sheet[27008];    ///< the effect's decoded pictures, at fixed offsets
  s32 bg2xLines[160]; ///< BG2X for each scanline, for an HBlank DMA
  /// The rest of the pictures' storage, which no code names.
  u8 imageTail[832];
  /// The backdrop's 160 colours, one a line, that
  /// BattleEffect_ArmPaletteHBlankDma streams into colour 0.
  u16 backdropLines[160];
  /// How the canvas transfer callback treats the canvas after each copy, an
  /// enum CanvasTransferMode.
  s32 transferMode;
  /// The transfer mode's argument: the fill value, the share kept (an enum
  /// CanvasFadeShare) or the darken or brighten amount.
  s32 transferValue;
  s32 unused;         ///< never read or written in either game
  s32 frame;          ///< frame counter some effects keep, from 0
  s32 scrollTimer;    ///< frames since the last interval scroll step
  s32 scrollInterval; ///< frames between interval scroll steps
  s32 scrollStepX;    ///< added to BG1 or BG2 x at each step
  s32 scrollStepY;    ///< added to BG1 or BG2 y at each step
  s32 savedBg1X;      ///< BG1 scroll restored when a shake ends
  s32 savedBg1Y;      ///< BG1 scroll restored when a shake ends
  s32 shakeFrames;    ///< shake frames left; Camera_ApplyShake counts down
  s32 cameraDelta;    ///< camera yaw turned per effect frame
  /// The effect sets 1 after a frame; BattleCamera_ApplyPhasedDelta then turns
  /// the whole cameraDelta and sets 0, else turns half and toggles 0 and 2.
  s32 cameraPhase;
  s32 fadeFrames; ///< backdrop fade frames left (Palette_StepFadeTransfer)
  s32 fadeStep;   ///< steps the backdrop has darkened so far
  /// Window, display and blend registers, which
  /// BattleEffect_ApplyWindowBlendRegisters writes all at once.
  ///@{
  u16 win0h;
  u16 win0v;
  u16 win1h;
  u16 win1v;
  u16 winin;
  u16 winout;
  u16 dispcnt;
  u16 bldcnt;
  u16 bldalpha;
  ///@}
  u16 padding; ///< aligns bg2x
  /// BG2's reference point, written to BG2X and BG2Y by
  /// Display_ApplyBg2Reference and Display_ApplyBg2Affine. Volatile, as the
  /// scale below.
  ///@{
  volatile s32 bg2x;
  volatile s32 bg2y;
  ///@}
  /// BG2's scale, written to BG2PA and BG2PD by Display_ApplyBg2Affine.
  /// Volatile: Display_ApplyBg2Affine copies them out from the VBlank handler
  /// (TASK_PASS_VBLANK).
  ///@{
  volatile u16 bg2pa;
  volatile u16 bg2pd;
  ///@}
  void *sprites[64]; ///< the effect's sprites
  /// Each target's flash timer in frames, for args->targets[n]: set by
  /// BattleEffect_TintBattler, counted down by
  /// BattleEffect_CountDownBattlerTints.
  u8 actorTimers[8];
  /// 1 on a canvas copy's VBlank, then counts up. Volatile and unsigned: the
  /// VBlank handler counts it while an effect waits on it, each wait reads it
  /// afresh and compares it unsigned.
  volatile u32 vblanksSinceTransfer;
  /// 1 once the effect has drawn a frame; the canvas transfer callback copies
  /// the canvas and clears it.
  s32 transferPending;
  s32 stampPending; ///< one draft sets 1 for a frame range; no reader found
  struct BattleEffectParams *args; ///< the command being run
};

STATIC_ASSERT(offsetof(struct BattleEffectWork, sheet) == 1792);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2xLines) == 28800);
STATIC_ASSERT(offsetof(struct BattleEffectWork, backdropLines) == 30272);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferMode) == 30592);
STATIC_ASSERT(offsetof(struct BattleEffectWork, shakeFrames) == 30632);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeStep) == 30648);
STATIC_ASSERT(offsetof(struct BattleEffectWork, win0h) == 30652);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2x) == 30672);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2pa) == 30680);
STATIC_ASSERT(offsetof(struct BattleEffectWork, sprites) == 30684);
STATIC_ASSERT(offsetof(struct BattleEffectWork, actorTimers) == 30940);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferPending) == 30952);
STATIC_ASSERT(offsetof(struct BattleEffectWork, args) == 30960);

/// The battle canvas the effects draw into: 128 by 128 pixels, a byte each.
#define CANVAS_BYTES (128 * 128)
/// A word of four canvas pixels at one level, for filling the canvas.
#define CANVAS_FILL_WORD(level) PIXELS_8BPP(level)

#endif
