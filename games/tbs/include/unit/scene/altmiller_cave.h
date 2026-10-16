#ifndef ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H
#define ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;
struct ActorWheel;

struct SceneEntrance *AltmillerCave_GetEntrances(void);
struct SceneRegion *AltmillerCave_GetRegions(void);
u32 *AltmillerCave_GetExits(void);
const struct ScenePlacement *AltmillerCave_GetPlacements(void);
const struct SceneEvent *AltmillerCave_GetEvents(void);
s32 AltmillerCave_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern struct ActorWheel *gAltmillerCaveWheel;
extern struct SceneEntrance gAltmillerCave_Entrances[];
extern u32 gAltmillerCave_Exits[];
extern struct ScenePlacement gAltmillerCavePlacementsOther[];
extern struct ScenePlacement gAltmillerCavePlacements2[];
extern struct ScenePlacement gAltmillerCavePlacements4[];
extern struct ScenePlacement gAltmillerCavePlacements6[];
extern u32 gAltmillerCave_PulseScales[];
extern s32 gAltmillerActionA[];
extern s32 gAltmillerActionB[];
extern s32 gAltmillerActionC[];
extern s32 gAltmillerActionD[];
extern struct SceneEvent gAltmillerCaveEventsOther[];
extern struct SceneEvent gAltmillerCaveEvents2[];
extern struct SceneEvent gAltmillerCaveEvents3[];
extern struct SceneEvent gAltmillerCaveEvents4[];
extern struct SceneEvent gAltmillerCaveEvents5[];
extern struct SceneEvent gAltmillerCaveEvents6[];

#endif
