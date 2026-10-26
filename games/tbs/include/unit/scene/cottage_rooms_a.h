#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_A_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *CottageRoomsA_GetEntrances(void);
const struct SceneExitRegion *CottageRoomsA_GetRegions(void);
const u32 *CottageRoomsA_GetExits(void);
const struct ObjectEventTemplate *CottageRoomsA_GetObjectEventTemplates(void);
const struct MapEvent *CottageRoomsA_GetEvents(void);
s32 CottageRoomsA_Initialize(void);
extern s32 gCottageRoomsA_FaceTargetScript[];
extern s32 gCottageRoomsA_FatherPacingScript[];
extern s32 gCottageRoomsA_PartyLeavesScript[];
extern s32 gCottageRoomsA_SleepZScript[];
extern struct SceneEntrance gCottageRoomsA_Entrances[];
extern u32 gCottageRoomsA_Exits[];
extern u32 gCottageRoomsA_StormExits[];
extern struct ObjectEventTemplate gCottageRoomsA_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCottageRoomsA_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gCottageRoomsA_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gCottageRoomsA_StormEvents[];
extern struct MapEvent gCottageRoomsA_Events[];
extern struct MapEvent gCottageRoomsA_EventsAfterLeaving[];
extern struct MapEvent gCottageRoomsA_EventsAfterKalay[];

#endif
