#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_A_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *CottageRoomsA_GetEntrances(void);
const struct SceneExitRegion *CottageRoomsA_GetRegions(void);
const u32 *CottageRoomsA_GetExits(void);
const struct ActorPlacement *CottageRoomsA_GetActorPlacements(void);
const struct Trigger *CottageRoomsA_GetTriggers(void);
s32 CottageRoomsA_Initialize(void);
extern s32 gCottageRoomsA_FaceTargetScript[];
extern s32 gCottageRoomsA_FatherPacingScript[];
extern s32 gCottageRoomsA_PartyLeavesScript[];
extern s32 gCottageRoomsA_SleepZScript[];
extern struct SceneEntrance gCottageRoomsA_Entrances[];
extern u32 gCottageRoomsA_Exits[];
extern u32 gCottageRoomsA_StormExits[];
extern struct ActorPlacement gCottageRoomsA_ActorPlacements[];
extern struct ActorPlacement gCottageRoomsA_ActorPlacementsAfterLeaving[];
extern struct ActorPlacement gCottageRoomsA_ActorPlacementsAfterKalay[];
extern struct Trigger gCottageRoomsA_StormTriggers[];
extern struct Trigger gCottageRoomsA_Triggers[];
extern struct Trigger gCottageRoomsA_TriggersAfterLeaving[];
extern struct Trigger gCottageRoomsA_TriggersAfterKalay[];

#endif
