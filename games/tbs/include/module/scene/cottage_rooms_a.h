#ifndef ALCHEMY_MODULE_SCENE_COTTAGE_ROOMS_A_H
#define ALCHEMY_MODULE_SCENE_COTTAGE_ROOMS_A_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
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
