#ifndef ALCHEMY_BATTLE_DISPLAY_H
#define ALCHEMY_BATTLE_DISPLAY_H

/// @file
/// The battle camera and the presentation's transition, background and affine
/// work.

#include "battle_session.h"
#include "battle_effect_argument.h"

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

/// The battle camera: its eye, the focus it turns about, and its pitch, yaw
/// and distance. It lives in the HEAP_SLOT_CAMERA work slot.
struct BattleCamera {
  /// The eye, 16.16 fixed-point pixels: written each frame distance behind
  /// the focus along the turned view.
  s32 eye[3];
  s32 focus[3]; ///< the focus, 16.16 fixed-point pixels
  /// XXX: cleared beside followPosition and never read (see the battle
  /// README).
  s32 unk18;
  s32 *followPosition;        ///< when set, followed instead of focus
  s32 distance;               ///< eye to focus, 16.16 fixed-point pixels
  u8 unusedAfterDistance[16]; ///< no code names it
  u16 pitch;                  ///< 1/65536 turns
  u16 yaw;                    ///< 1/65536 turns
  u8 unusedAfterYaw[20];      ///< no code names it
};

STATIC_ASSERT(sizeof(struct BattleCamera) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleCamera, focus) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleCamera, followPosition) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleCamera, pitch) == 0x34);
STATIC_ASSERT(offsetof(struct BattleCamera, yaw) == 0x36);

/// The HEAP_SLOT_CAMERA work slot during a battle: the battle camera.
extern struct BattleCamera *gCameraWork;

/// The battle background's work slot as the camera reads it: the yaw the
/// camera eases toward and the frames left to ease it.
struct BattlePresentationTransition {
  s32 targetYaw;  ///< 1/65536 turns
  s32 frames;     ///< frames left to ease the yaw, a sixteenth a frame
  s32 mode;       ///< BattleBackgroundView's; not named here
  s32 secondMode; ///< BattleBackgroundView's; not named here
  s32 active;     ///< BattleBackgroundView's busy, at the same offset
  /// 0 lets the camera update set the background's range each frame; 1 for
  /// a critical hit's zoom and once a round's view is set.
  s32 flag;
  s32 unk18; ///< XXX: cleared when the battle starts and never read
  s32 unk1c; ///< XXX: cleared when the battle starts and never read
};

/// A presentation step's stack work: the effect command it builds and runs,
/// and the targets' sprite colour modes saved beside it. The
/// BattlePresentation_Run functions of staging.c and auto_battle.c,
/// BattleTransition_Run and ElementGlow_Play
/// keep one as a local.
struct BattleStagingWork {
  struct BattleAnimArgs effect;
  /// Each target's layers' SpriteColorMode (AnimationEntry.colorMode), all but
  /// its last layer, saved before the effect; no code reads them back by name.
  u8 savedAnimationParameters[8][4];
};

STATIC_ASSERT(sizeof(struct BattlePresentationTransition) == 0x20);
STATIC_ASSERT(offsetof(struct BattlePresentationTransition, active) == 0x10);
STATIC_ASSERT(sizeof(struct BattleStagingWork) == 0x54);
STATIC_ASSERT(offsetof(struct BattleStagingWork, effect.targets) == 0x24);

/// BattleBackgroundView.mode and secondMode: how the battle background is
/// shown.
enum BattleBackgroundMode {
  /// Not loaded yet: the next load adds BattlePresentation_UpdateHBlankScroll.
  BATTLE_BACKGROUND_NONE,
  /// The flat picture on BG1 (BATTLE_BG1CNT), its scroll following the
  /// camera, BG2 left to an effect.
  BATTLE_BACKGROUND_FIXED,
  /// The picture zoomed and panned on BG2, an affine layer; HBlank DMA sets
  /// each line's BG2CNT to the half of the picture it reads. The code's
  /// "perspective plane" is this plain zoom: the depth is in the picture.
  BATTLE_BACKGROUND_ZOOMED
};

/// The battle background's work slot as the background code reads it: its
/// display mode.
struct BattleBackgroundView {
  s32 targetYaw; ///< BattlePresentationTransition's; not named here
  s32 frames;    ///< BattlePresentationTransition's; not named here
  s32 mode;      ///< enum BattleBackgroundMode
  /// BATTLE_BACKGROUND_FIXED also sets BG1's scroll whatever the mode;
  /// the effects' canvas layers set it.
  s32 secondMode;
  s32 busy; ///< while set, BattleCamera_SetRange leaves BG1's scroll
};

/// The battle background's affine parameters and reference point, copied
/// whole to the BG2PA registers.
struct BattleAffine {
  /// BG2PA to BG2PD, 8.8 fixed point; BattleCamera_SetRange writes a plain
  /// zoom (pb and pc 0, pa and pd one over the scale).
  u16 pa, pb, pc, pd;
  s32 x, y; ///< BG2X and BG2Y, 8.8 fixed-point pixels
};

/// The battle background's scanline work, in HEAP_SLOT_BATTLE_SCANLINES: two
/// pages of per-line BG2CNT values, swapped each frame, and the affine
/// parameters.
struct BattleAffineHdma {
  u32 page;      ///< the page of lines shown; the other one is built
  u8 unused[12]; ///< no code names it
  struct BattleAffine affine;
  /// Each scanline's BG2CNT, one page per frame: which half of the picture a
  /// line shows, or the plane off, with BGCNT_WRAP at a zoom of 1 or more.
  u16 lines[2][160];
};

STATIC_ASSERT(sizeof(struct BattleBackgroundView) == 0x14);
STATIC_ASSERT(offsetof(struct BattleBackgroundView, mode) == 0x08);
STATIC_ASSERT(sizeof(struct BattleAffineHdma) == 0x2a0);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, affine) == 0x10);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, lines) == 0x20);

struct BattleTurn;
struct BattleAnimArgs;

#endif
