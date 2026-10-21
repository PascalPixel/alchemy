#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_NAVAMPA_MATCH_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_NAVAMPA_MATCH_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ColosseumNavampaMatch_GetEntrances(void);
const struct SceneRegion *ColosseumNavampaMatch_GetRegions(void);
const u32 *ColosseumNavampaMatch_GetExits(void);
const struct ScenePlacement *ColosseumNavampaMatch_GetPlacements(void);
const struct SceneEvent *ColosseumNavampaMatch_GetEvents(void);
s32 ColosseumNavampaMatch_Initialize(void);
extern const s32 gColosseumNavampaMatch_SidewalkBlockOffBelt[];
extern const s8 gColosseumNavampaMatch_WallWedgePattern[][6];
extern const u16 gColossoEarlySequenceData[];
extern const s32 gColossoSceneEventEffect[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColossoModeScript2[];
extern const s16 gColossoModeScript3B[];
extern char gColossoHexChars[20];
extern s32 gColossoRandomEffect[];
extern s32 gColossoLinkedEffect[];
extern u32 gStagedActor_DirectionSteps[];
extern struct SceneEntrance gColosseumNavampaMatchEntrances[];
extern u32 gColosseumNavampaMatchExits[];
extern struct ScenePlacement gColosseumNavampaMatchPlacements[];
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;
extern struct SceneEvent gColosseumNavampaMatchEvents[];
extern s32 gColosseumNavampaMatch_OpponentAfterBattle[];
extern s32 gColosseumNavampaMatch_LeaderActionA[];
extern s32 gColosseumNavampaMatch_LeaderActionB[];
extern s32 gColosseumNavampaMatch_LeaderActionC[];
extern s32 gColossoMultiPhaseData[];
extern s32 gColossoParticleKinds[];
extern s16 gColosso_LetteringSlot;
extern s16 gColossoModeScriptDefault[];
extern s16 gColossoModeScript4[];
extern s16 gColossoModeScript3A[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColosseumNavampaMatch_PlaceScript[];

#endif
