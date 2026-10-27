#ifndef ALCHEMY_BATTLE_FX_WORK_H
#define ALCHEMY_BATTLE_FX_WORK_H

#include "types.h"
#include "static_assert.h"
#include "effect_step.h"

struct BattleEffectArgument;

/// The battle effect scenes' work (heap slot 23), as The Lost Age lays it out.
// The Broken Seal's BattleEffectWork with the particles moved ahead of the
// sheet and 0xc4 more bytes ahead of the actor timers.
struct BattleFxWork {
  struct EffectStep particles[64];
  u8 sheet[0x6980]; ///< the effect's decoded pictures, at fixed offsets
  /// BattleEffectWork's bg2xLines and unk7300; not named through this view.
  u8 unk7080[0x700];
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
  void *objects[16]; ///< the effect's animation objects
  /// BattleEffectWork's objects 16 to 63; not named through this view.
  u8 unk781c[0xc0];
  /// Each target's flash timer in frames, for effect->actors[n]: set by
  /// ObjectGroup_UpdateMembers, counted down by ObjectGroup_CountDownMemberTimers.
  u8 actorTimers[8];
  s32 framesSinceTransfer; ///< 1 on a canvas copy's frame, then counts up
  /// 1 once the effect has drawn a frame; the canvas transfer callback copies
  /// the canvas and clears it.
  s32 transferPending;
  s32 unk78ec; ///< one draft sets 1 for a frame range; no reader found
  struct BattleEffectArgument *effect; ///< the command being run
};

STATIC_ASSERT(offsetof(struct BattleFxWork, sheet) == 0x700);
STATIC_ASSERT(offsetof(struct BattleFxWork, transferMode) == 0x7780);
STATIC_ASSERT(offsetof(struct BattleFxWork, shakeFrames) == 0x77a8);
STATIC_ASSERT(offsetof(struct BattleFxWork, objects) == 0x77dc);
STATIC_ASSERT(offsetof(struct BattleFxWork, actorTimers) == 0x78dc);
STATIC_ASSERT(offsetof(struct BattleFxWork, transferPending) == 0x78e8);
STATIC_ASSERT(offsetof(struct BattleFxWork, effect) == 0x78f0);

#endif
