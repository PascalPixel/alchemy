#ifndef ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gMercuryLighthouse_RippleScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern s32 gGlint_TwinkleScript[];
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
extern struct ActorPlacement gMercuryLighthouseActorPlacementsOther[];
extern struct ActorPlacement gMercuryLighthouseLowerActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseMiddleActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseUpperActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseExteriorActorPlacements[];
extern struct Trigger gMercuryLighthouseLowerTriggers[];
extern struct Trigger gMercuryLighthouseMiddleTriggers[];
extern struct Trigger gMercuryLighthouseUpperTriggers[];
extern struct Trigger gMercuryLighthouseTriggersOther[];

#endif
