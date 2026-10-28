#ifndef ALCHEMY_BATTLE_DISPLAY_H
#define ALCHEMY_BATTLE_DISPLAY_H

/// @file
/// The Lost Age's presentation work, which holds nine actors.
// The Broken Seal's (games/tbs/include/battle/battle_camera.h) holds eight.

#include "battle_session.h"

#include "types.h"
#include "static_assert.h"
#include "edition.h"

/// The battle camera: its eye, the focus it turns about, and its pitch, yaw
/// and distance.
struct BattleCamera {
  /// The eye, 16.16 fixed-point pixels: written distance behind the focus
  /// along the turned view.
  s32 eye[3];
  s32 pos[3];     ///< the focus, 16.16 fixed-point pixels
  s32 unk18;      ///< cleared beside followPos; not read by name in TLA
  s32 *followPos; ///< when set, the focus followed instead of pos
  s32 distance;   ///< eye to focus, 16.16 fixed-point pixels
  u8 unk24[16];   ///< not named by TLA code
  u16 pitch;      ///< 1/65536 turns
  u16 yaw;        ///< 1/65536 turns
  u8 unk38[20];   ///< not named by TLA code
};

STATIC_ASSERT(sizeof(struct BattleCamera) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleCamera, pos) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleCamera, followPos) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleCamera, pitch) == 0x34);
STATIC_ASSERT(offsetof(struct BattleCamera, yaw) == 0x36);

extern struct BattleCamera *gCameraWork;

/// The battle background's heap slot as the camera reads it: the yaw the
/// camera eases toward and the frames left to ease it.
struct BattlePresentationTransition {
  s32 targetYaw; ///< 1/65536 turns
  s32 frames;    ///< frames left to ease the yaw, a sixteenth a frame
  u8 unk08[8];   ///< BattleBackgroundView's mode and secondMode
  s32 active;    ///< BattleBackgroundView's busy, at the same offset
  s32 flag;      ///< 0 lets the camera update set the background's range
  s32 unk18;     ///< cleared at the battle's start; not read by name in TLA
  s32 unk1c;     ///< cleared at the battle's start; not read by name in TLA
};

/// A presentation's effect command, laid out as BattleEffectArgument with a
/// ninth actor, and the animation state it keeps.
struct BattlePresentationWork {
  s32 kind;         ///< the effect's index in the handler table
  s32 side;         ///< 1 when the targets are the party, 0 for enemies
  s32 actor;        ///< the user's unit id
  s32 firstTarget;  ///< the action's first target
  s32 initialValue; ///< BattleEffectArgument's range: the action's, or 1
  s32 count;        ///< targets in actors
  s32 variant;      ///< the ability's size, from 0
  s32 flags;        ///< BattleEffectArgument's prelude, at the same offset
  u8 unk20[4];      ///< not named by TLA code
  s16 actors[9];    ///< the targets' unit ids, count of them
  u8 params[8][4];  ///< each target's animation entries' param, kept
};

STATIC_ASSERT(sizeof(struct BattlePresentationTransition) == 0x20);
STATIC_ASSERT(offsetof(struct BattlePresentationTransition, active) == 0x10);
STATIC_ASSERT(sizeof(struct BattlePresentationWork) == 0x58);
STATIC_ASSERT(offsetof(struct BattlePresentationWork, actors) == 0x24);
STATIC_ASSERT(offsetof(struct BattlePresentationWork, params) == 0x36);

/// The battle background's heap slot as the background code reads it: its
/// display mode.
struct BattleBackgroundView {
  u8 unk00[8];    ///< BattlePresentationTransition's targetYaw and frames
  s32 mode;       ///< 2 draws the affine plane through the HBlank DMA
  s32 secondMode; ///< set by an effect while its canvas is open
  s32 busy;
};

/// The battle background's affine parameters and reference point, copied
/// whole to the BG2PA registers.
struct BattleAffine {
  u16 pa, pb, pc, pd;
  s32 x, y;
};

/// The battle background's scanline work: two pages of per-line values,
/// swapped each frame, and the affine parameters.
struct BattleAffineHdma {
  u32 page;     ///< the page of lines shown
  u8 unk04[12]; ///< not named by TLA code
  struct BattleAffine affine;
  u16 lines[2][160];
};

STATIC_ASSERT(sizeof(struct BattleBackgroundView) == 0x14);
STATIC_ASSERT(offsetof(struct BattleBackgroundView, mode) == 0x08);
STATIC_ASSERT(sizeof(struct BattleAffineHdma) == 0x2a0);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, affine) == 0x10);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, lines) == 0x20);

struct BattlePlan;
s32 BattleStaging_RunActorEntries(struct BattlePlan *plan, s32 unused);

void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                struct BattlePresentationWork *work);

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32 BattleStaging_RunPairedUnitTransition(
    struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 objectId);
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 mode);
void BattleHighlight_SetSpriteMode(s32 actor, s32 mode);
void BattleObject_Put(void);
void BattleEffect_RunSceneEffectFar(s32 *context);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_ArmBg2AffineHBlankDma(void);
void Palette_StepFadeTransfer(void);
struct BattleEffectArgument;
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

#endif
