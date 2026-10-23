#ifndef ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "map_tables.h"

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
extern s32 gMercuryLighthouse_RippleScript[];
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
extern struct SceneEntrance gMercuryLighthouseMiddleEntrances[];
extern struct SceneEntrance gMercuryLighthouseUpperEntrances[];
extern struct SceneEntrance gMercuryLighthouseExteriorEntrances[];
extern u32 gMercuryLighthouse_Exits[];
extern struct ScenePlacement gMercuryLighthousePlacementsOther[];
extern struct ScenePlacement gMercuryLighthouseLowerPlacements[];
extern struct ScenePlacement gMercuryLighthouseMiddlePlacements[];
extern struct ScenePlacement gMercuryLighthouseUpperPlacements[];
extern struct ScenePlacement gMercuryLighthouseExteriorPlacements[];
extern struct SceneEvent gMercuryLighthouseLowerEvents[];
extern struct SceneEvent gMercuryLighthouseMiddleEvents[];
extern struct SceneEvent gMercuryLighthouseUpperEvents[];
extern struct SceneEvent gMercuryLighthouseEventsOther[];

#endif
