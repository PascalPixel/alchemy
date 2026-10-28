#ifndef ALCHEMY_UNIT_MAP_LALIVERO_H
#define ALCHEMY_UNIT_MAP_LALIVERO_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Lalivero_GetEntrances(void);
const struct SceneExitRegion *Lalivero_GetRegions(void);
const u32 *Lalivero_GetExits(void);
const struct ActorPlacement *Lalivero_GetActorPlacements(void);
const struct Trigger *Lalivero_GetTriggers(void);
s32 Lalivero_Initialize(void);
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
