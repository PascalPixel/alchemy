#ifndef ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *MercuryLighthouse_GetEntrances(void);
const struct SceneRegion *MercuryLighthouse_GetRegions(void);
const u32 *MercuryLighthouse_GetExits(void);
const struct ScenePlacement *MercuryLighthouse_GetPlacements(void);
const struct SceneEvent *MercuryLighthouse_GetEvents(void);
s32 MercuryLighthouse_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gMercuryLighthouse_RiseScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 gMercuryLighthouse_SparkBurstScript[];
extern s32 gMercuryLighthouse_PushScriptA[];
extern s32 gMercuryLighthouse_PushScriptB[];
extern s32 gMercuryLighthouse_PushScriptD[];
extern s32 gMercuryLighthouse_PushScriptC[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern u16 gMercuryLighthouse_GateCells[];
extern u16 gMercuryLighthouse_GateCloseCells[];
extern u16 gMercuryLighthouse_FloorSwitchCells[];
extern u16 gMercuryLighthouse_FloorSwitchCloseCells[];
extern u16 gMercuryLighthouse_ColumnCells[];
extern struct SceneEntrance gMercuryLighthouseEntrancesOther[];
extern struct SceneEntrance gMercuryLighthouseEntrances2[];
extern struct SceneEntrance gMercuryLighthouseEntrances3[];
extern struct SceneEntrance gMercuryLighthouseEntrances4[];
extern u32 gMercuryLighthouse_Exits[];
extern struct ScenePlacement gMercuryLighthousePlacementsOther[];
extern struct ScenePlacement gMercuryLighthousePlacements1[];
extern struct ScenePlacement gMercuryLighthousePlacements2[];
extern struct ScenePlacement gMercuryLighthousePlacements3[];
extern struct ScenePlacement gMercuryLighthousePlacements4[];
extern struct SceneEvent gMercuryLighthouseEvents1[];
extern struct SceneEvent gMercuryLighthouseEvents2[];
extern struct SceneEvent gMercuryLighthouseEvents3[];
extern struct SceneEvent gMercuryLighthouseEventsOther[];

#endif
