#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *MercuryLighthouse_GetEntrances(void);
const struct SceneExitRegion *MercuryLighthouse_GetRegions(void);
const u32 *MercuryLighthouse_GetExits(void);
const struct ActorTemplate *MercuryLighthouse_GetActorTemplates(void);
const struct Trigger *MercuryLighthouse_GetTriggers(void);
s32 MercuryLighthouse_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gMercuryLighthouse_RippleScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern const s32 *gFieldEffect_DurationScripts[];
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
extern struct ActorTemplate gMercuryLighthouseActorTemplatesOther[];
extern struct ActorTemplate gMercuryLighthouseLowerActorTemplates[];
extern struct ActorTemplate gMercuryLighthouseMiddleActorTemplates[];
extern struct ActorTemplate gMercuryLighthouseUpperActorTemplates[];
extern struct ActorTemplate gMercuryLighthouseExteriorActorTemplates[];
extern struct Trigger gMercuryLighthouseLowerTriggers[];
extern struct Trigger gMercuryLighthouseMiddleTriggers[];
extern struct Trigger gMercuryLighthouseUpperTriggers[];
extern struct Trigger gMercuryLighthouseTriggersOther[];

#endif
