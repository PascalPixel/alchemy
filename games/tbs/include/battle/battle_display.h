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
  s32 pos[3];     ///< the focus, 16.16 fixed-point pixels
  s32 unk18;      ///< cleared beside followPos; not read by name in TBS
  s32 *followPos; ///< when set, the focus followed instead of pos
  s32 distance;   ///< eye to focus, 16.16 fixed-point pixels
  u8 unk24[16];   ///< not named by TBS code
  u16 pitch;      ///< 1/65536 turns
  u16 yaw;        ///< 1/65536 turns
  u8 unk38[20];   ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct BattleCamera) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleCamera, pos) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleCamera, followPos) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleCamera, pitch) == 0x34);
STATIC_ASSERT(offsetof(struct BattleCamera, yaw) == 0x36);

/// Work slot 12 (HEAP_SLOT_CAMERA) during a battle: the battle camera.
extern struct BattleCamera *gCameraWork;

/// The battle background's work slot as the camera reads it: the yaw the
/// camera eases toward and the frames left to ease it.
struct BattlePresentationTransition {
  s32 targetYaw; ///< 1/65536 turns
  s32 frames;    ///< frames left to ease the yaw, a sixteenth a frame
  u8 unk08[8];   ///< BattleBackgroundView's mode and secondMode
  s32 active;    ///< BattleBackgroundView's busy, at the same offset
  /// 0 lets the camera update set the background's range each frame; 1 for
  /// a critical hit's zoom and once a round's view is set.
  s32 flag;
  s32 unk18; ///< cleared when the battle starts; not read by name in TBS
  s32 unk1c; ///< cleared when the battle starts; not read by name in TBS
};

/// A presentation step's stack work: the effect command it builds and runs,
/// and the targets' sprite colour modes saved beside it. The BattlePres_Run
/// and BattlePresentation_Run functions of staging.c and auto_battle.c,
/// BattlePres_RunEntryTransition and BattleFx_PlayUnitElementEffect keep one
/// as a local.
struct BattlePresentationWork {
  struct BattleEffectArgument effect;
  /// Each target's layers' SpriteColorMode (AnimationEntry.param), all but its
  /// last layer, saved before the effect; no code reads them back by name.
  u8 savedAnimationParameters[8][4];
};

STATIC_ASSERT(sizeof(struct BattlePresentationTransition) == 0x20);
STATIC_ASSERT(offsetof(struct BattlePresentationTransition, active) == 0x10);
STATIC_ASSERT(sizeof(struct BattlePresentationWork) == 0x54);
STATIC_ASSERT(offsetof(struct BattlePresentationWork, effect.actors) == 0x24);

/// The battle background's work slot as the background code reads it: its
/// display mode (2 draws it as an affine perspective plane).
struct BattleBackgroundView {
  u8 unk00[8];    ///< BattlePresentationTransition's targetYaw and frames
  s32 mode;       ///< 2 draws the affine plane; 1 sets BG1's scroll too
  s32 secondMode; ///< 1 also sets BG1's scroll; one effect sets it
  s32 busy;       ///< while set, BattleCamera_SetRange leaves BG1's scroll
};

/// The battle background's affine parameters and reference point, copied
/// whole to the BG2PA registers.
struct BattleAffine {
  /// BG2PA to BG2PD, 8.8 fixed point; BattleCamera_SetRange writes a plain
  /// zoom (pb and pc 0, pa and pd one over the scale).
  u16 pa, pb, pc, pd;
  s32 x, y; ///< BG2X and BG2Y, 8.8 fixed-point pixels
};

/// The battle background's scanline work in heap slot 10: two pages of
/// per-line BG2CNT values, swapped each frame, and the affine parameters.
struct BattleAffineHdma {
  u32 page;     ///< the page of lines shown; the other one is built
  u8 unk04[12]; ///< not named by TBS code
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

struct BattlePlan;
struct BattleEffectArgument;

#endif
