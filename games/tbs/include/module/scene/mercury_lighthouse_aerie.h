#ifndef ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gMercuryLighthouseAerieEntrances[];
extern u32 gMercuryLighthouseAerieExits[];
extern struct ActorPlacement gMercuryLighthouseAerieActorPlacements[];
extern struct Trigger gMercuryLighthouseAerieTriggers[];
extern s32 gMercuryLighthouseAerie_PlatformUnderfoot;
extern s32 gMercuryLighthouseAerie_SparkScript[];

#endif
