#ifndef ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MercuryLighthouse_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gMercuryLighthouse_RiseScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercury_RampScript[];
extern s32 gMercury_ScaleCounterScript[];
extern s32 gMercury_PillarScript[];
extern u16 gMercury_MoveAngles[];
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
extern u32 gMercuryLighthouse_SceneTable[];
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
