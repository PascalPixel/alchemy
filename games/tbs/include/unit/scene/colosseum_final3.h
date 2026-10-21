#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_FINAL_3_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_FINAL_3_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ColosseumFinal3_GetEntrances(void);
const struct SceneRegion *ColosseumFinal3_GetRegions(void);
const u32 *ColosseumFinal3_GetExits(void);
const struct ScenePlacement *ColosseumFinal3_GetPlacements(void);
const struct SceneEvent *ColosseumFinal3_GetEvents(void);
s32 ColosseumFinal3_Initialize(void);
extern const s32 gColosseumFinal3_SidewalkBlockOffBelt[];
extern const s8 gColosseumFinal3_WallWedgePattern[][6];
extern const u16 gColossoEarlySequenceData[];
extern const s32 gColossoSceneEventEffect[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColossoModeScript2[];
extern const s16 gColossoModeScript3B[];
extern char gColossoHexChars[20];
extern s32 gColossoRandomEffect[];
extern s32 gColossoLinkedEffect[];
extern u32 gStagedActor_DirectionSteps[];
extern struct SceneEntrance gColosseumFinal3Entrances[];
extern u32 gColosseumFinal3Exits[];
extern struct ScenePlacement gColosseumFinal3Placements[];
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;
extern struct SceneEvent gColosseumFinal3Events[];
extern s32 gColosseumFinal3_OpponentAfterBattle[];
extern s32 gColosseumFinal3_LeaderActionA[];
extern s32 gColosseumFinal3_LeaderActionB[];
extern s32 gColosseumFinal3_LeaderActionC[];
extern s32 gColossoMultiPhaseData[];
extern s32 gColossoParticleKinds[];
extern s16 gColosso_LetteringSlot;
extern s16 gColossoModeScriptDefault[];
extern s16 gColossoModeScript4[];
extern s16 gColossoModeScript3A[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColosseumFinal3_PlaceScript[];

#endif
