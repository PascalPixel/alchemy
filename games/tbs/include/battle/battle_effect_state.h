#ifndef ALCHEMY_BATTLE_EFFECT_STATE_H
#define ALCHEMY_BATTLE_EFFECT_STATE_H

/// @file
/// BattleEffectState, the running battle effect's state, and its sprites.

#include "static_assert.h"
#include "types.h"
#include "battle/battle_effect_params.h"
#include "field/particle.h"
#include "system/io_reg.h"

struct Sprite;

/// Two adjacent affine background coordinates, queued together for BG2X/BG2Y.
struct BattleBg2Origin {
  s32 x; ///< BG2X, 8 fractional bits
  s32 y; ///< BG2Y, 8 fractional bits
  /// 1 once the coordinates are queued, 0 before the first; nothing in the
  /// game reads it, and the queue copies only x and y.
  s32 queued;
};

STATIC_ASSERT(sizeof(struct BattleBg2Origin) == 12);
STATIC_ASSERT(offsetof(struct BattleBg2Origin, y) == 4);

/// How BattleEffect_ProcessPendingGraphicsTransfer moves the effect's
/// 128x128 canvas into VRAM once a frame (state->transferMode); the canvas
/// pixels are indices into a palette ramp, so scaling them fades a trail.
/// For the clear, darken and brighten modes transferValue is a word, a fill or
/// a step repeated in each of its four bytes (one per pixel,
/// CANVAS_FILL_WORD); the fade mode only compares it with CANVAS_FADE_HALF.
enum CanvasTransferMode {
  CANVAS_COPY,             ///< copy as drawn
  CANVAS_COPY_AND_CLEAR,   ///< copy, then fill the canvas with transferValue (0
                           ///< clears it)
  CANVAS_COPY_AND_FADE,    ///< copy, keeping half of each pixel's level for the
                           ///< next frame when transferValue is
                           ///< CANVAS_FADE_HALF, and three quarters for any
                           ///< other value (CANVAS_FADE_THREE_QUARTERS by
                           ///< convention)
  CANVAS_COPY_AND_DARKEN,  ///< copy, then lower each level by a byte of
                           ///< transferValue, stopping at 0
  CANVAS_COPY_AND_BRIGHTEN ///< copy, then raise each level by a byte of
                           ///< transferValue, stopping at
                           ///< CANVAS_LEVEL_BRIGHTEST; no effect uses it
};

/// CANVAS_COPY_AND_FADE's transferValue: the percentage of each pixel's level
/// kept for the next frame.
enum CanvasFadeShare { CANVAS_FADE_HALF = 50, CANVAS_FADE_THREE_QUARTERS = 75 };

/// A canvas word holding `level` in each of its four bytes: a fill for
/// CANVAS_COPY_AND_CLEAR or a step for CANVAS_COPY_AND_DARKEN, one byte per
/// pixel.
#define CANVAS_FILL_WORD(level) PIXELS_8BPP(level)

/// The brightest canvas level: the additive blitters and
/// CANVAS_COPY_AND_BRIGHTEN stop at it.
#define CANVAS_LEVEL_BRIGHTEST 63

/// The running battle effect's state: its canvas image, 64 particles, the
/// display registers it stages and its sprites. It lives in the
/// HEAP_SLOT_BATTLE_EFFECT heap slot.
struct BattleEffectState {
  /// Images and scanline tables reuse the storage before the particles.
  /// The tile-effect mask extends beyond sprite sheet into the table area.
  union {
    struct {
      u8 sheet[27008];    ///< the effect's decoded pictures, at fixed offsets
      s32 bg2xLines[160]; ///< BG2X for each scanline, for an HBlank DMA
      /// The rest of imageStorage, which only effects that skip the scanline
      /// table reach through it.
      u8 imageTail[1152];
    };
    u8 imageStorage[28800];
    /// The tracked elemental effect stages its origin inside the image area.
    struct {
      u8 image[5060];
      struct BattleBg2Origin origin;
    } trackedCanvas;
  };
  struct Particle particles[64]; ///< free while variant is -1
  /// How the canvas transfer tasks treat the canvas after each copy
  /// (enum CanvasTransferMode in battle_effect_internal.h).
  s32 transferMode; ///< an enum CanvasTransferMode
  /// The transfer mode's argument: the fill value, the share kept (50 or 75
  /// percent) or the darken or brighten amount.
  s32 transferValue;
  s32 unused;         ///< never read or written in either game
  s32 frame;          ///< frame counter some effects keep, from 0
  s32 scrollTimer;    ///< frames since the last interval scroll step
  s32 scrollInterval; ///< frames between interval scroll steps
  s32 scrollStepX;    ///< added to BG1 or BG2 x at each step
  s32 scrollStepY;    ///< added to BG1 or BG2 y at each step
  s32 savedBg1X;      ///< BG1 scroll restored when a shake ends
  s32 savedBg1Y;      ///< BG1 scroll restored when a shake ends
  s32 shakeFrames;    ///< shake frames left; BattleCamera_Shake counts down
  s32 cameraDelta;    ///< camera yaw turned per frame of the effect
  /// The effect sets 1 after a frame; BattleCamera_ApplyPhasedDelta then
  /// turns the whole cameraDelta and sets 0, else turns half and toggles 0
  /// and 2.
  s32 cameraPhase;
  s32 fadeFrames; ///< battle background fade frames left
                  ///< (Palette_StepFadeTransfer)
  s32 fadeStep;   ///< CANVAS_FADE_STEPs the background has darkened so far
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
  u16 padding; ///< aligns bg2x; never read or written
  /// BG2's reference point, written to BG2X and BG2Y by
  /// Display_ApplyBg2Reference and BattleEffect_AdvanceBg2OnInterval.
  ///@{
  s32 bg2x;
  s32 bg2y;
  ///@}
  struct Sprite *sprites[16]; ///< the effect's sprites
  /// Each target's flash timer in frames, for args->targets[n]: set by
  /// BattleEffect_TintBattler, counted down by
  /// BattleEffect_CountDownBattlerTints.
  u8 actorTimers[8];
  s32 vblanksSinceTransfer; ///< 1 on a canvas copy's VBlank, then counts up
  /// 1 once the effect has presented its canvas (EFFECT_PRESENT_CANVAS); the
  /// canvas transfer task copies the canvas and clears it.
  s32 transferPending;
  struct BattleEffectParams *args; ///< the command being run
};

STATIC_ASSERT(offsetof(struct BattleEffectState, bg2xLines) == 27008);
STATIC_ASSERT(offsetof(struct BattleEffectState, imageStorage) == 0);
STATIC_ASSERT(offsetof(struct BattleEffectState, trackedCanvas.origin) == 5060);
STATIC_ASSERT(offsetof(struct BattleEffectState, particles) == 28800);
STATIC_ASSERT(offsetof(struct BattleEffectState, transferMode) == 30592);
STATIC_ASSERT(offsetof(struct BattleEffectState, transferValue) == 30596);
STATIC_ASSERT(offsetof(struct BattleEffectState, frame) == 30604);
STATIC_ASSERT(offsetof(struct BattleEffectState, scrollTimer) == 30608);
STATIC_ASSERT(offsetof(struct BattleEffectState, scrollStepX) == 30616);
STATIC_ASSERT(offsetof(struct BattleEffectState, savedBg1X) == 30624);
STATIC_ASSERT(offsetof(struct BattleEffectState, shakeFrames) == 30632);
STATIC_ASSERT(offsetof(struct BattleEffectState, cameraDelta) == 30636);
STATIC_ASSERT(offsetof(struct BattleEffectState, fadeFrames) == 30644);
STATIC_ASSERT(offsetof(struct BattleEffectState, fadeStep) == 30648);
STATIC_ASSERT(offsetof(struct BattleEffectState, win0h) == 30652);
STATIC_ASSERT(offsetof(struct BattleEffectState, bg2x) == 30672);
STATIC_ASSERT(offsetof(struct BattleEffectState, sprites) == 30680);
STATIC_ASSERT(offsetof(struct BattleEffectState, actorTimers) == 30744);
STATIC_ASSERT(offsetof(struct BattleEffectState, vblanksSinceTransfer) ==
              30752);
STATIC_ASSERT(offsetof(struct BattleEffectState, transferPending) == 30756);
STATIC_ASSERT(offsetof(struct BattleEffectState, args) == 30760);
STATIC_ASSERT(sizeof(struct BattleEffectState) == 30764);

// The pixel fragments BattleEffect_BuildBlitter copies into its blitters, from
// blitter_fragments.s: Forward reads the picture left to right and
// Reverse right to left. None of them runs where it is.
/// Max, four pixels (one canvas word), right to left; a full cell's row takes
/// it twice.
extern const u32 BattleEffect_Max4Reverse[];
/// Max, four pixels (one canvas word), left to right; a full cell's row takes
/// it twice.
extern const u32 BattleEffect_Max4Forward[];
/// Max, one pixel, right to left.
extern const u32 BattleEffect_MaxReverse[];
/// Max, one pixel, left to right.
extern const u32 BattleEffect_MaxForward[];
/// Additive, four pixels added a word at a time, right to left; a full cell's
/// row takes it twice.
extern const u32 BattleEffect_ColorReverse[];
/// Additive, four pixels added a word at a time, left to right; a full cell's
/// row takes it twice.
extern const u32 BattleEffect_ColorForward[];
/// Additive, one pixel, right to left.
extern const u32 BattleEffect_AdditiveReverse[];
/// Additive, one pixel, left to right.
extern const u32 BattleEffect_AdditiveForward[];
/// Builds the mask word, 64 in every byte, with which an additive word's
/// pixels are capped at CANVAS_LEVEL_BRIGHTEST.
extern const u32 BattleEffect_Mask[];
/// Transparent, a full cell's eight pixels, right to left.
extern const u32 BattleEffect_Transparent8Reverse[];
/// Transparent, a full cell's eight pixels, left to right.
extern const u32 BattleEffect_Transparent8Forward[];
/// Transparent, one pixel, right to left.
extern const u32 BattleEffect_TransparentReverse[];
/// Transparent, one pixel, left to right.
extern const u32 BattleEffect_TransparentForward[];
/// Copy, one pixel, right to left.
extern const u32 BattleEffect_CopyReverse[];
/// Copy, one pixel, left to right.
extern const u32 BattleEffect_CopyForward[];

#endif
