#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H

#include "types.h"
#include "field_scene.h"

/// A field object seen as one of the reunion particles after the battle: its
/// position, scale and drifting velocity.
typedef struct SceneParticle {
  u8 filler00[8];    ///< FieldActor's script pointer, cursor and facing
  s32 x;             ///< 16.16 fixed-point pixels
  s32 y;             ///< 16.16 fixed-point pixels
  s32 z;             ///< 16.16 fixed-point pixels
  u8 filler14[4];    ///< FieldActor's terrainHeight; not named here
  s32 scaleX;        ///< 16.16 fixed point, where Q16_ONE is full size
  s32 scaleY;        ///< 16.16 fixed point, where Q16_ONE is full size
  u8 filler20[0x1c]; ///< FieldActor's radius to targetX; not named here
  /// FieldActor's targetY, in 16.16 fixed-point pixels: set to the height
  /// the particle starts at.
  s32 anchorY;
  /// FieldActor's targetZ to spriteStorageKind; not named here.
  u8 filler40[0x15];
  u8 state; ///< FieldActor's motionFlags; zeroed when the particle is shown
  u8 filler56[0xe]; ///< FieldActor's unk56 to riseEnabled; not named here
  /// FieldActor's timer: 1/256 pixels per step, 25 at first and 5 more each
  /// step.
  s16 velocityX;
  /// FieldActor's work: 1/256 pixels per step, 128 at first and 1 less each
  /// step.
  s16 velocityY;
} SceneParticle;

/// A field object seen as the moving source that scatters random effects
/// around itself.
typedef struct StageEffect {
  s32 reserved00; ///< FieldActor's script pointer; not named here
  s32 reserved04; ///< FieldActor's cursor and facing; not named here
  s32 x;          ///< 16.16 fixed-point pixels
  s32 y;          ///< 16.16 fixed-point pixels
  s32 z;          ///< 16.16 fixed-point pixels
  /// FieldActor's terrainHeight to velocityX; not named here.
  u8 reserved14[20];
  /// FieldActor's velocityY, in 16.16 fixed-point pixels per frame: within
  /// 255 either way, state is cleared.
  s32 verticalMotion;
  /// FieldActor's velocityZ to spriteStorageKind; not named here.
  u8 reserved2c[41];
  u8 state; ///< FieldActor's motionFlags; zeroed on the source and effects
} StageEffect;

/// A field object linked to the item actor it raises, by that actor's slot.
typedef struct LinkedStageEffect {
  s32 reserved00; ///< FieldActor's script pointer; not named here
  s32 reserved04; ///< FieldActor's cursor and facing; not named here
  s32 x;          ///< 16.16 fixed-point pixels
  s32 y;          ///< 16.16 fixed-point pixels
  s32 z;          ///< 16.16 fixed-point pixels
  /// FieldActor's terrainHeight to spriteStorageKind; not named here.
  u8 reserved14[65];
  u8 state;          ///< FieldActor's motionFlags; zeroed on the item raised
  u8 reserved56[14]; ///< FieldActor's unk56 to riseEnabled; not named here
  /// FieldActor's timer: the item actor's number, cleared once raised.
  s16 linkedEffectSlot;
} LinkedStageEffect;

const struct SceneEntrance *ColossoLog_GetEntrances(void);
const struct SceneRegion *ColossoLog_GetRegions(void);
const u32 *ColossoLog_GetExits(void);
const struct ScenePlacement *ColossoLog_GetPlacements(void);
const struct SceneEvent *ColossoLog_GetEvents(void);
s32 ColossoLog_Initialize(void);
extern const s32 gColossoLog_Record30Script[];
extern const s8 gColossoLog_LogPattern[][6];
extern const u16 gColossoEarlySequenceData[];
extern const s32 gColossoSceneEventEffect[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColossoModeScript2[];
extern const s16 gColossoModeScript3B[];
extern char gColossoHexChars[20];
extern s32 gColossoRandomEffect[];
extern s32 gColossoLinkedEffect[];
extern u32 gStagedActor_DirectionSteps[];
extern struct SceneEntrance gColossoLogEntrances[];
extern u32 gColossoLogExits[];
extern struct ScenePlacement gColossoLogPlacements[];
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;
extern struct SceneEvent gColossoLogEvents[];
extern s32 gColossoLog_Actor8Action[];
extern s32 gColossoLog_LeaderActionA[];
extern s32 gColossoLog_LeaderActionB[];
extern s32 gColossoLog_LeaderActionC[];
extern s32 gColossoMultiPhaseData[];
extern s32 gColossoParticleKinds[];
extern s16 gColosso_LetteringSlot;
extern s16 gColossoModeScriptDefault[];
extern s16 gColossoModeScript4[];
extern s16 gColossoModeScript3A[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColossoLog_PlaceScript[];

#endif
