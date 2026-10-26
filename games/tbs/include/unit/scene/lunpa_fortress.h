#ifndef ALCHEMY_UNIT_MAP_LUNPA_FORTRESS_H
#define ALCHEMY_UNIT_MAP_LUNPA_FORTRESS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneExitRegion;
struct MapEvent;
struct ObjectEventTemplate;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
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
extern struct ObjectEventTemplate gLunpaFortressObjectEventTemplatesOther[];
extern struct ObjectEventTemplate
    gLunpaFortressUnusedWestLunpaCaveObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLunpaFortressGroundFloorObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLunpaFortressUpperFloorObjectEventTemplates[];
extern struct ObjectEventTemplate gLunpaFortressCaveObjectEventTemplates[];
extern struct ObjectEventTemplate gLunpaFortressToadonpaObjectEventTemplates[];
extern struct MapEvent gLunpaFortressNoEvents[];
extern struct MapEvent gLunpaFortressGroundFloorEvents[];
extern struct MapEvent gLunpaFortressUpperFloorEvents[];
extern struct MapEvent gLunpaFortressCaveEvents[];
extern struct MapEvent gLunpaFortressEventsOther[];
extern u32 gLunpaFortressToadonpaDoorCells[];
extern u32 gLunpaFortressCaveDoorCells[];
extern u32 gLunpaFortressUpperFloorDoorCells[];
extern u32 gLunpaFortressGroundFloorDoorCells[];

#endif
