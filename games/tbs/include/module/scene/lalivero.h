#ifndef ALCHEMY_MODULE_SCENE_LALIVERO_H
#define ALCHEMY_MODULE_SCENE_LALIVERO_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gLalivero_CryingScript[];
extern struct SceneEntrance gLaliveroEntrances[];
extern u32 gLaliveroExits[];
extern struct ActorPlacement gLaliveroActorPlacements[];
extern struct ActorPlacement gLaliveroActorPlacementsAfterLighthouse[];
extern s16 gLaliveroDoorCells[][2];
extern u16 gLaliveroDoorOpenSteps[];
extern u16 gLaliveroGateOpenSteps[];
extern struct Trigger gLaliveroTriggers[];
extern struct Trigger gLaliveroTriggersAfterLighthouse[];

#endif
