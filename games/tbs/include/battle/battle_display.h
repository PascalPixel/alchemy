#ifndef ALCHEMY_BATTLE_DISPLAY_H
#define ALCHEMY_BATTLE_DISPLAY_H

/// @file
/// The battle camera, its view work and the battle background's affine
/// work.

#include "battle_session.h"
#include "battle_effect_params.h"

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

/// The battle camera: its eye, the focus it turns about, and its pitch, yaw
/// and distance. It lives in the HEAP_SLOT_CAMERA heap slot.
struct BattleCamera {
  /// The eye, 16.16 fixed-point pixels: written each frame distance behind
  /// the focus along the turned view.
  s32 eye[3];
  s32 focus[3]; ///< the focus, 16.16 fixed-point pixels
  /// When set, ActorSystem_ProjectSprites faces the sprites from it instead
  /// of eye; every camera setup clears it and nothing sets it.
  s32 *followEye;
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

/// The HEAP_SLOT_CAMERA heap slot during a battle: the battle camera.
extern struct BattleCamera *gBattleCamera;

/// The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the yaw the battle camera eases
/// toward and the frames left to ease it, and how the battle background is
/// shown.
struct BattleBackgroundWork {
  s32 targetYaw; ///< 1/65536 turns
  s32 frames;    ///< frames left to ease the yaw, a sixteenth a frame
  s32 mode;      ///< enum BattleBackgroundMode
  /// BATTLE_BACKGROUND_FIXED also sets BG1's scroll whatever the mode;
  /// the battle effects' canvas layers set it.
  s32 secondMode;
  s32 busy; ///< while set, BattleCamera_SetRange leaves BG1's scroll
  /// 0 lets the camera update set the background's range each frame; 1 for
  /// a critical hit's zoom and once a turn's view is set.
  s32 flag;
  s32 unusedAfterFlag[2]; ///< cleared when a battle starts; nothing reads them
};

/// A presentation step's stack work: the battle effect command it builds and
/// runs, and the targets' sprite colour modes saved beside it. The
/// BattleStaging_Run and AutoBattle_Run functions of staging.c and
/// auto_battle.c, BattleTransition_Run and ElementGlow_Play keep one as a
/// local.
struct BattleStagingWork {
  struct BattleEffectParams effect;
  /// Each target's sprite animations' SpriteColorMode
  /// (SpriteAnimation.colorMode), all but its last, saved before the effect;
  /// no code reads them back by name.
  u8 savedAnimationParameters[8][4];
};

STATIC_ASSERT(sizeof(struct BattleBackgroundWork) == 0x20);
STATIC_ASSERT(offsetof(struct BattleBackgroundWork, mode) == 0x08);
STATIC_ASSERT(offsetof(struct BattleBackgroundWork, busy) == 0x10);
STATIC_ASSERT(sizeof(struct BattleStagingWork) == 0x54);
STATIC_ASSERT(offsetof(struct BattleStagingWork, effect.targets) == 0x24);

/// BattleBackgroundWork.mode and secondMode: how the battle background is
/// shown.
enum BattleBackgroundMode {
  /// Not loaded yet: the next load adds BattleBackground_ShowZoomedPage.
  BATTLE_BACKGROUND_NONE,
  /// The picture at a fixed size on BG1 (BATTLE_BG1CNT), its scroll following
  /// the camera, BG2 left to a battle effect.
  BATTLE_BACKGROUND_FIXED,
  /// The picture zoomed and panned on BG2, an affine layer; HBlank DMA sets
  /// each line's BG2CNT to the half of the picture it reads. The depth is drawn
  /// into the picture; the hardware only zooms it.
  BATTLE_BACKGROUND_ZOOMED
};

/// BattleBackground_SetMode's fade that restores the saved palette instead of
/// taking from each colour channel.
#define BATTLE_BACKGROUND_FADE_RESTORE 0x80

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

STATIC_ASSERT(sizeof(struct BattleAffineHdma) == 0x2a0);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, affine) == 0x10);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, lines) == 0x20);

struct BattleActionRun;
struct BattleEffectParams;

#endif
