#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

/// @file
/// BattleEffectWork, the running battle effect's work, and its animation
/// objects.

#include "effect_step.h"
#include "battle_effect_argument.h"
#include "static_assert.h"
#include "types.h"

struct AnimationObject;

/// Two adjacent affine background coordinates, queued together for BG2X/BG2Y.
struct BattleCanvasOrigin {
  s32 x; ///< BG2X, 8 fractional bits
  s32 y; ///< BG2Y, 8 fractional bits
  /// 1 once the coordinates are queued, 0 before the first; nothing in the
  /// game reads it, and the queue copies only x and y.
  s32 queued;
};

STATIC_ASSERT(sizeof(struct BattleCanvasOrigin) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleCanvasOrigin, y) == 0x04);

/// How BattleEffect_ProcessPendingGraphicsTransfer moves the effect's
/// 128x128 canvas into VRAM once a frame (work->transferMode); the canvas
/// pixels are indices into a palette ramp, so scaling them fades a trail.
/// For the clear, darken and brighten modes transferValue is a word, a fill or
/// a step repeated in each of its four bytes (one per pixel); the fade mode
/// only compares it with 50.
enum CanvasTransferMode {
  CANVAS_COPY,             ///< copy as drawn
  CANVAS_COPY_AND_CLEAR,   ///< copy, then fill the canvas with transferValue (0
                           ///< clears it; 0x10101010 is level 16 everywhere)
  CANVAS_COPY_AND_FADE,    ///< copy, keeping half of each pixel's level for the
                           ///< next frame when transferValue is 50, and three
                           ///< quarters for any other value (75 by convention)
  CANVAS_COPY_AND_DARKEN,  ///< copy, then lower each level by a byte of
                           ///< transferValue, stopping at 0
  CANVAS_COPY_AND_BRIGHTEN ///< copy, then raise each level by a byte of
                           ///< transferValue, stopping at 63
};

/// The running battle effect's work: its canvas image, 64 particles, the
/// display registers it stages and its animation objects. It lives in the
/// HEAP_SLOT_BATTLE_ANIM work slot.
struct BattleEffectWork {
  /// Images and scanline tables reuse the storage before the particles.
  /// The tile-animation mask extends beyond sheet into the table area.
  union {
    struct {
      u8 sheet[0x6980];   ///< the effect's decoded pictures, at fixed offsets
      s32 bg2xLines[160]; ///< BG2X for each scanline, for an HBlank DMA
      /// The rest of imageStorage, which only effects that skip the scanline
      /// table reach through it.
      u8 imageTail[0x480];
    };
    u8 imageStorage[0x7080];
    /// The tracked elemental scene stages its origin inside the image area.
    struct {
      u8 image[0x13c4];
      struct BattleCanvasOrigin origin;
    } trackedCanvas;
  };
  struct EffectStep particles[64]; ///< free while variant is -1
  /// How the canvas transfer callbacks treat the canvas after each copy
  /// (enum CanvasTransferMode in effect_internal.h).
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
  s32 shakeFrames;    ///< shake frames left; Camera_ApplyShake counts down
  s32 cameraDelta;    ///< camera yaw turned per effect frame
  /// The effect sets 1 after a frame; Camera_ApplyPhasedDelta then turns the
  /// whole cameraDelta and sets 0, else turns half and toggles 0 and 2.
  s32 cameraPhase;
  s32 fadeFrames; ///< backdrop fade frames left (Palette_StepFadeTransfer)
  s32 fadeStep;   ///< CANVAS_FADE_STEPs the backdrop has darkened so far
  /// Window, display and blend registers, which
  /// Graphics_ApplyWindowBlendRegisters writes all at once.
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
  struct AnimationObject *animations[16]; ///< the effect's animation objects
  /// Each target's flash timer in frames, for effect->actors[n]: set by
  /// ObjectGroup_UpdateMembers, counted down by ObjectGroup_TickMemberTimers.
  u8 actorTimers[8];
  s32 framesSinceTransfer; ///< 1 on a canvas copy's frame, then counts up
  /// 1 once the effect has drawn a frame (EFFECT_PRESENT_FRAME); the canvas
  /// transfer callback copies the canvas and clears it.
  s32 transferPending;
  struct BattleEffectArgument *effect; ///< the command being run
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
STATIC_ASSERT(offsetof(struct BattleEffectWork, animations) == 0x77d8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, actorTimers) == 0x7818);
STATIC_ASSERT(offsetof(struct BattleEffectWork, framesSinceTransfer) == 0x7820);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferPending) == 0x7824);
STATIC_ASSERT(offsetof(struct BattleEffectWork, effect) == 0x7828);
STATIC_ASSERT(sizeof(struct BattleEffectWork) == 0x782c);

#endif
