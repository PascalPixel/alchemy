#ifndef ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H
#define ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;
struct LightWheel;

const struct SceneEntrance *AltmillerCave_GetEntrances(void);
const struct SceneRegion *AltmillerCave_GetRegions(void);
const u32 *AltmillerCave_GetExits(void);
const struct ScenePlacement *AltmillerCave_GetPlacements(void);
const struct SceneEvent *AltmillerCave_GetEvents(void);
s32 AltmillerCave_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct LightWheel *gAltmillerCave_LightWheel;
extern struct SceneEntrance gAltmillerCave_Entrances[];
extern u32 gAltmillerCave_Exits[];
extern struct ScenePlacement gAltmillerCave_PlacementsOther[];
extern struct ScenePlacement gAltmillerCave_BabiPlacements[];
extern struct ScenePlacement gAltmillerCave_SquallPlacements[];
extern struct ScenePlacement gAltmillerCave_DraughtPlacements[];
extern u32 gAltmillerCave_PulseScales[];
extern s32 gAltmillerCave_GuardsArrivePath[];
extern s32 gAltmillerCave_SecondGuardLeavePath[];
extern s32 gAltmillerCave_FirstGuardLeavePath[];
extern s32 gAltmillerCave_BabiLeavePath[];
extern struct SceneEvent gAltmillerCave_EventsOther[];
extern struct SceneEvent gAltmillerCave_BabiEvents[];
extern struct SceneEvent gAltmillerCave_VialChestEvents[];
extern struct SceneEvent gAltmillerCave_SquallEvents[];
extern struct SceneEvent gAltmillerCave_CookieChestEvents[];
extern struct SceneEvent gAltmillerCave_DraughtEvents[];

#endif
