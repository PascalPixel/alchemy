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
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 MercuryLighthouse_RiseScript[];
extern s32 MercuryLighthouse_SparkScript[];
extern s32 Mercury_RampScript[];
extern s32 Mercury_ScaleCounterScript[];
extern s32 Mercury_PillarScript[];
extern u16 Mercury_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 MercuryLighthouse_SparkBurstScript[];
extern s32 MercuryLighthouse_PushScriptA[];
extern s32 MercuryLighthouse_PushScriptB[];
extern s32 MercuryLighthouse_PushScriptD[];
extern s32 MercuryLighthouse_PushScriptC[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern u16 MercuryLighthouse_GateCells[];
extern u16 MercuryLighthouse_GateCloseCells[];
extern u16 MercuryLighthouse_FloorSwitchCells[];
extern u16 MercuryLighthouse_FloorSwitchCloseCells[];
extern u16 MercuryLighthouse_ColumnCells[];
extern struct SceneEntrance gMercuryLighthouseEntrancesOther[];
extern struct SceneEntrance gMercuryLighthouseEntrances2[];
extern struct SceneEntrance gMercuryLighthouseEntrances3[];
extern struct SceneEntrance gMercuryLighthouseEntrances4[];
extern u32 MercuryLighthouse_SceneTable[];
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
