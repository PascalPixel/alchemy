#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

/// @file
/// BattleEffectWork, the running battle effect's work, and its sprites.

#include "particle.h"
#include "battle_effect_params.h"
#include "static_assert.h"
#include "types.h"

struct Sprite;

/// Two adjacent affine background coordinates, queued together for BG2X/BG2Y.
struct BattleBg2Origin {
  s32 x; ///< BG2X, 8 fractional bits
  s32 y; ///< BG2Y, 8 fractional bits
  /// 1 once the coordinates are queued, 0 before the first; nothing in the
  /// game reads it, and the queue copies only x and y.
  s32 queued;
};

STATIC_ASSERT(sizeof(struct BattleBg2Origin) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleBg2Origin, y) == 0x04);

/// How BattleEffect_ProcessPendingGraphicsTransfer moves the effect's
/// 128x128 canvas into VRAM once a frame (work->transferMode); the canvas
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
                           ///< transferValue, stopping at 63; no effect
                           ///< uses it
};

/// CANVAS_COPY_AND_FADE's transferValue: the percentage of each pixel's level
/// kept for the next frame.
enum CanvasFadeShare { CANVAS_FADE_HALF = 50, CANVAS_FADE_THREE_QUARTERS = 75 };

/// A canvas word holding `level` in each of its four bytes: a fill for
/// CANVAS_COPY_AND_CLEAR or a step for CANVAS_COPY_AND_DARKEN, one byte per
/// pixel.
#define CANVAS_FILL_WORD(level) ((level) * 0x01010101)

/// The running battle effect's work: its canvas image, 64 particles, the
/// display registers it stages and its sprites. It lives in the
/// HEAP_SLOT_BATTLE_EFFECT heap slot.
struct BattleEffectWork {
  /// Images and scanline tables reuse the storage before the particles.
  /// The tile-effect mask extends beyond sheet into the table area.
  union {
    struct {
      u8 sheet[0x6980];   ///< the effect's decoded pictures, at fixed offsets
      s32 bg2xLines[160]; ///< BG2X for each scanline, for an HBlank DMA
      /// The rest of imageStorage, which only effects that skip the scanline
      /// table reach through it.
      u8 imageTail[0x480];
    };
    u8 imageStorage[0x7080];
    /// The tracked elemental effect stages its origin inside the image area.
    struct {
      u8 image[0x13c4];
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
  s32 unused7788;     ///< never read or written in either game
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

STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2xLines) == 0x6980);
STATIC_ASSERT(offsetof(struct BattleEffectWork, imageStorage) == 0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, trackedCanvas.origin) ==
              0x13c4);
STATIC_ASSERT(offsetof(struct BattleEffectWork, particles) == 0x7080);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferMode) == 0x7780);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferValue) == 0x7784);
STATIC_ASSERT(offsetof(struct BattleEffectWork, frame) == 0x778c);
STATIC_ASSERT(offsetof(struct BattleEffectWork, scrollTimer) == 0x7790);
STATIC_ASSERT(offsetof(struct BattleEffectWork, scrollStepX) == 0x7798);
STATIC_ASSERT(offsetof(struct BattleEffectWork, savedBg1X) == 0x77a0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, shakeFrames) == 0x77a8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, cameraDelta) == 0x77ac);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeFrames) == 0x77b4);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeStep) == 0x77b8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, win0h) == 0x77bc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2x) == 0x77d0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, sprites) == 0x77d8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, actorTimers) == 0x7818);
STATIC_ASSERT(offsetof(struct BattleEffectWork, vblanksSinceTransfer) ==
              0x7820);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferPending) == 0x7824);
STATIC_ASSERT(offsetof(struct BattleEffectWork, args) == 0x7828);
STATIC_ASSERT(sizeof(struct BattleEffectWork) == 0x782c);

#endif
