#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ColossoLog_GetEntrances(void);
const struct SceneRegion *ColossoLog_GetRegions(void);
const u32 *ColossoLog_GetExits(void);
const struct ScenePlacement *ColossoLog_GetPlacements(void);
const struct SceneEvent *ColossoLog_GetEvents(void);
s32 ColossoLog_Initialize(void);
extern const s32 gColossoLog_Record30Script[];
extern const s8 gColossoLog_WallWedgePattern[][6];
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
