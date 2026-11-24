#ifndef ALCHEMY_BATTLE_CAMERA_H
#define ALCHEMY_BATTLE_CAMERA_H

/// @file
/// The battle camera, the battle background's modes and scanline state, and the
/// presentation state an action's staging hands its battle effect.
// The Broken Seal's presentation state holds eight targets; this one nine.

#include "battle/battle_action.h"

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "system/projection.h"

/// The battle camera: its eye, the focus it turns about, and its pitch, yaw
/// and distance.
struct BattleCamera {
  /// The eye, 16.16 fixed-point pixels: written distance behind the focus
  /// along the turned view.
  FIXED eye[3];
  FIXED focus[3];      ///< the focus, 16.16 fixed-point pixels
  s32 followEye;       ///< cleared beside followPosition; nothing reads it
  s32 *followPosition; ///< when set, followed instead of focus

  FIXED distance;             ///< eye to focus, 16.16 fixed-point pixels
  u8 unusedAfterDistance[16]; ///< no code names it
  u16 pitch;                  ///< 1/65536 turns
  u16 yaw;                    ///< 1/65536 turns
  u8 unusedAfterYaw[20];      ///< no code names it
};

STATIC_ASSERT(sizeof(struct BattleCamera) == 76);
STATIC_ASSERT(offsetof(struct BattleCamera, focus) == 12);
STATIC_ASSERT(offsetof(struct BattleCamera, followPosition) == 28);
STATIC_ASSERT(offsetof(struct BattleCamera, pitch) == 52);
STATIC_ASSERT(offsetof(struct BattleCamera, yaw) == 54);

/// The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the yaw the battle camera eases
/// toward and the frames left to ease it, and how the battle background is
/// shown.
struct BattleBackgroundState {
  s32 targetYaw; ///< 1/65536 turns
  s32 frames;    ///< frames left to ease the yaw, a sixteenth a frame
  s32 mode;      ///< enum BattleBackgroundMode
  /// BATTLE_BACKGROUND_FIXED also sets BG1's scroll whatever the mode; the
  /// battle effects' canvas layers set it.
  s32 bg1ScrollMode;
  s32 busy; ///< while set, an effect owns BG1's scroll and the camera leaves it
  /// 0 lets the camera update set the background's range each frame.
  s32 keepRange;
  s32 unusedAfterKeepRange[2]; ///< cleared when a battle starts; nothing reads
                               ///< them
};

/// A presentation's effect command, laid out as BattleEffectParams with a
/// ninth actor, and the animation state it keeps.
struct BattlePresentationState {
  s32 kind;         ///< the effect's index in the handler table
  s32 side;         ///< 1 when the targets are the party, 0 for enemies
  s32 actor;        ///< the user's unit id
  s32 firstTarget;  ///< the action's first target
  s32 initialValue; ///< BattleEffectParams's range: the action's, or 1
  s32 count;        ///< targets in actors
  s32 variant;      ///< the ability's size, from 0
  s32 flags;        ///< BattleEffectParams's prelude, at the same offset
  u8 unused[4];     ///< no code names it
  s16 actors[9];    ///< the targets' unit ids, count of them
  u8 params[8][4];  ///< each target's animation entries' param, kept
};

STATIC_ASSERT(sizeof(struct BattleBackgroundState) == 32);
STATIC_ASSERT(offsetof(struct BattleBackgroundState, mode) == 8);
STATIC_ASSERT(offsetof(struct BattleBackgroundState, busy) == 16);
STATIC_ASSERT(sizeof(struct BattlePresentationState) == 88);
STATIC_ASSERT(offsetof(struct BattlePresentationState, actors) == 36);
STATIC_ASSERT(offsetof(struct BattlePresentationState, params) == 54);

/// BattleBackgroundState.mode, how the battle background is shown, and its
/// bg1ScrollMode.
enum BattleBackgroundMode {
  /// Not loaded yet.
  BATTLE_BACKGROUND_NONE,
  /// The picture at a fixed size on BG1, its scroll following the camera, BG2
  /// left to a battle effect.
  BATTLE_BACKGROUND_FIXED,
  /// The picture zoomed and panned on BG2, an affine layer; HBlank DMA sets
  /// each line's BG2CNT to the half of the picture it reads.
  BATTLE_BACKGROUND_ZOOMED
};

/// BattleBackground_SetMode's fade that restores the saved palette instead of
/// taking from each colour channel.
#define BATTLE_BACKGROUND_FADE_RESTORE 128

/// The battle background's affine parameters and reference point, copied
/// whole to the BG2PA registers.
struct BattleAffine {
  u16 pa, pb, pc, pd;
  s32 x, y;
};

/// The battle background's scanline state: two pages of per-line values,
/// swapped each frame, and the affine parameters.
struct BattleAffineHdma {
  u32 page;      ///< the page of lines shown
  u8 unused[12]; ///< no code names it
  struct BattleAffine affine;
  u16 lines[2][160];
};

STATIC_ASSERT(sizeof(struct BattleAffineHdma) == 672);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, affine) == 16);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, lines) == 32);

struct BattlePlan;
UNREAD_RESULT BattleStaging_RunMessages(struct BattlePlan *plan, s32 unused);

void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                   struct BattlePresentationState *state);

void BattleCamera_SetRange(FIXED cx, FIXED cy, s32 ox, s32 oy, FIXED scale);

s32 BattleObject_IsValidId(u32 objectId);
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom);
void BattleHighlight_SetSpriteMode(s32 actor, s32 mode);
void BattleObject_Put(void);
/// Runs one of the large battle effects, the summons and the scenes of a few
/// abilities.
void BattleEffect_RunSummonFar(s32 *context);
/// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_TransferCanvas(void);
void BattleEffect_ArmBg2AffineHBlankDma(void);
/// Steps the background palette fade.
void BattleEffect_DarkenBackgroundStep(void);
struct BattleEffectParams;

/// The battle camera's distance from its focus in its default view, and the
/// unzoomed projection's focal length, in 16.16 pixels (BattleCamera_SetView).
#define BATTLE_CAMERA_DISTANCE toFIXED(510)

#endif
