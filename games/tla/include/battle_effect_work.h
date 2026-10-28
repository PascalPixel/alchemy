#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

/// @file
/// The Lost Age's copy of the battle effect work.
// The Broken Seal's block with the 64 particles in front of the sheet, the two
// affine scales after the BG2 reference point, 64 object slots and one more
// word before the effect.

#include "effect_step.h"
#include "battle_effect_argument.h"
#include "static_assert.h"
#include "types.h"

struct BattleEffectWork {
  struct EffectStep particles[64];
  u8 sheet[0x6980];   ///< the effect's decoded pictures, at fixed offsets
  s32 bg2xLines[160]; ///< BG2X for each scanline, for an HBlank DMA
  u8 unk7300[0x480];  ///< some drafts keep a colour table at 0x340
  /// How the canvas transfer callback treats the canvas after each copy: 0
  /// copies, 1 also fills it with transferValue, 2 keeps 50 or 75 percent, 3
  /// darkens and 4 brightens it.
  s32 transferMode;
  /// The transfer mode's argument: the fill value, the share kept (50 or 75
  /// percent) or the darken or brighten amount.
  s32 transferValue;
  s32 unk_7788;       ///< not named by TLA code
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
  s32 fadeStep;   ///< steps the backdrop has darkened so far
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
  u16 unk_77ce; ///< not named by TLA code
  /// BG2's reference point, written to BG2X and BG2Y by
  /// Display_ApplyBg2Reference and Display_ApplyBg2Affine.
  ///@{
  s32 bg2x;
  s32 bg2y;
  ///@}
  /// BG2's scale, written to BG2PA and BG2PD by Display_ApplyBg2Affine.
  ///@{
  u16 bg2pa;
  u16 bg2pd;
  ///@}
  void *objects[64]; ///< the effect's animation objects
  /// Each target's flash timer in frames, for effect->actors[n]: set by
  /// Actor_UpdateMembers, counted down by BattleEffect_CountDownBattlerTints.
  u8 actorTimers[8];
  s32 vblanksSinceTransfer; ///< 1 on a canvas copy's VBlank, then counts up
  /// 1 once the effect has drawn a frame; the canvas transfer callback copies
  /// the canvas and clears it.
  s32 transferPending;
  s32 unk78ec; ///< one draft sets 1 for a frame range; no reader found
  struct BattleEffectArgument *effect; ///< the command being run
};

STATIC_ASSERT(offsetof(struct BattleEffectWork, sheet) == 0x700);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2xLines) == 0x7080);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferMode) == 0x7780);
STATIC_ASSERT(offsetof(struct BattleEffectWork, shakeFrames) == 0x77a8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeStep) == 0x77b8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, win0h) == 0x77bc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2x) == 0x77d0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2pa) == 0x77d8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, objects) == 0x77dc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, actorTimers) == 0x78dc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferPending) == 0x78e8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, effect) == 0x78f0);

#endif
