#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H

#include "types.h"
#include "field_scene.h"

/// A field object seen as one of the reunion particles after the battle: its
/// position, scale and drifting velocity.
typedef struct SceneParticle {
  u8 filler00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 filler14[4];
  s32 scaleX; ///< 16.16 fixed point, where Q16_ONE is full size
  s32 scaleY; ///< 16.16 fixed point, where Q16_ONE is full size
  u8 filler20[0x1c];
  s32 anchorY; ///< 16.16 fixed-point pixels
  u8 filler40[0x15];
  u8 state;
  u8 filler56[0xe];
  s16 velocityX; ///< 1/256 pixels per step
  s16 velocityY; ///< 1/256 pixels per step
} SceneParticle;

/// A field object seen as the moving source that scatters random effects
/// around itself.
typedef struct StageEffect {
  s32 reserved00;
  s32 reserved04;
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 reserved14[20];
  s32 verticalMotion;
  u8 reserved2c[41];
  u8 state;
} StageEffect;

/// A field object linked to the item actor it raises, by that actor's slot.
typedef struct LinkedStageEffect {
  s32 reserved00;
  s32 reserved04;
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 reserved14[65];
  u8 state;
  u8 reserved56[14];
  s16 linkedEffectSlot;
} LinkedStageEffect;

struct SceneEntrance *ColossoLog_GetEntrances(void);
const struct SceneRegion *ColossoLog_GetRegions(void);
u32 *ColossoLog_GetExits(void);
struct ScenePlacement *ColossoLog_GetPlacements(void);
struct SceneEvent *ColossoLog_GetEvents(void);
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
