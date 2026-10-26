#ifndef ALCHEMY_UNIT_MAP_LUNPA_FORTRESS_H
#define ALCHEMY_UNIT_MAP_LUNPA_FORTRESS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneExitRegion;
struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gLunpaFortressRandomPick;
extern s32 gSecondPatrolRoute[];
extern s32 gFirstPatrolRoute[];
extern s32 gDonpaSleeps[];
extern s32 gGroundFloorPatrolRoute[];
extern s32 gHallPatrolRoute[];
extern struct SceneEntrance gLunpaFortressGroundFloorEntrances[];
extern struct SceneEntrance gLunpaFortressUpperFloorEntrances[];
extern struct SceneEntrance gLunpaFortressCaveEntrances[];
extern struct SceneEntrance gLunpaFortressEntrancesOther[];
extern u32 gLunpaFortressExitsOther[];
extern u32 gLunpaFortressUpperFloorExits[];
extern u32 gLunpaFortressCaveAndToadonpaExits[];
extern struct ActorTemplate gLunpaFortressActorTemplatesOther[];
extern struct ActorTemplate gLunpaFortressUnusedWestLunpaCaveActorTemplates[];
extern struct ActorTemplate gLunpaFortressGroundFloorActorTemplates[];
extern struct ActorTemplate gLunpaFortressUpperFloorActorTemplates[];
extern struct ActorTemplate gLunpaFortressCaveActorTemplates[];
extern struct ActorTemplate gLunpaFortressToadonpaActorTemplates[];
extern struct Trigger gLunpaFortressNoTriggers[];
extern struct Trigger gLunpaFortressGroundFloorTriggers[];
extern struct Trigger gLunpaFortressUpperFloorTriggers[];
extern struct Trigger gLunpaFortressCaveTriggers[];
extern struct Trigger gLunpaFortressTriggersOther[];
extern u32 gLunpaFortressToadonpaDoorCells[];
extern u32 gLunpaFortressCaveDoorCells[];
extern u32 gLunpaFortressUpperFloorDoorCells[];
extern u32 gLunpaFortressGroundFloorDoorCells[];

#endif
