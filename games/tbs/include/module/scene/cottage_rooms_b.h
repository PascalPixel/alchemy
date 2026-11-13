#ifndef ALCHEMY_MODULE_SCENE_COTTAGE_ROOMS_B_H
#define ALCHEMY_MODULE_SCENE_COTTAGE_ROOMS_B_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gCottageRoomsB_LeaveHouseScript[];
extern s32 gCottageRoomsB_DoraLeavesScript[];
extern s32 gCottageRoomsB_IsaacLeavesScript[];
extern s32 gCottageRoomsB_DoraRunsAheadScript[];
extern s32 gCottageRoomsB_IsaacRunsAheadScript[];
extern s32 gCottageRoomsB_DoraRunsBackScript[];
extern s32 gCottageRoomsB_IsaacRunsBackScript[];
extern s32 gCottageRoomsB_RunToPlazaScript[];
extern s32 gCottageRoomsB_FlameShakesScript[];
extern s32 gCottageRoomsB_FlameRisesScript[];
extern s32 gCottageRoomsB_DoraFacesIsaacScript[];
extern s32 gCottageRoomsB_MythrilBagRisesScript[];
extern s32 gCottageRoomsB_MythrilBagWobblesScript[];
extern struct SceneEntrance gCottageRoomsB_Entrances[];
extern u32 gCottageRoomsB_Exits[];
extern u32 gCottageRoomsB_StormExits[];
extern struct ActorPlacement gCottageRoomsB_ActorPlacements[];
extern struct ActorPlacement gCottageRoomsB_ActorPlacementsAfterLeaving[];
extern struct ActorPlacement gCottageRoomsB_ActorPlacementsAfterKalay[];
extern struct ActorPlacement gCottageRoomsB_BabiScholarsActorPlacements[];
extern struct Trigger gCottageRoomsB_StormTriggers[];
extern struct Trigger gCottageRoomsB_Triggers[];
extern struct Trigger gCottageRoomsB_TriggersAfterLeaving[];
extern struct Trigger gCottageRoomsB_TriggersAfterKalay[];
extern struct Trigger gCottageRoomsB_BabiScholarsTriggers[];
extern struct Trigger gCottageRoomsB_BabiScholarsLaterTriggers[];

#endif
