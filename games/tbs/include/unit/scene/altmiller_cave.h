#ifndef ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H
#define ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8                          *AltmillerCave_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
u8                          *AltmillerCave_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          AltmillerCave_ApplyEntryState(void);
extern s32                   gStagedActor_DirectionSteps[];
extern u32                   gStagedActor_FootprintKinds[];
extern s32                   gStagedActor_FootprintBounds[];
extern u16                  *gAltmillerCave_ClearTarget;
extern struct SceneEntrance  gAltmillerCave_SceneTableA[];
extern u32                   gAltmillerCave_SceneTableB[];
extern struct ScenePlacement gAltmillerCavePlacementsOther[];
extern struct ScenePlacement gAltmillerCavePlacements2[];
extern struct ScenePlacement gAltmillerCavePlacements4[];
extern struct ScenePlacement gAltmillerCavePlacements6[];
extern u32                   gAltmillerCave_PulseScales[];
extern s32                   gAltmillerActionA[];
extern s32                   gAltmillerActionB[];
extern s32                   gAltmillerActionC[];
extern s32                   gAltmillerActionD[];
extern struct SceneEvent     gAltmillerCaveEventsOther[];
extern struct SceneEvent     gAltmillerCaveEvents2[];
extern struct SceneEvent     gAltmillerCaveEvents3[];
extern struct SceneEvent     gAltmillerCaveEvents4[];
extern struct SceneEvent     gAltmillerCaveEvents5[];
extern struct SceneEvent     gAltmillerCaveEvents6[];

#endif
