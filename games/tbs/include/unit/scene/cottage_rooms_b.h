#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_B_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *CottageRoomsB_GetEntrances(void);
const struct SceneExitRegion *CottageRoomsB_GetRegions(void);
const u32 *CottageRoomsB_GetExits(void);
const struct ActorTemplate *CottageRoomsB_GetActorTemplates(void);
const struct Trigger *CottageRoomsB_GetTriggers(void);
s32 CottageRoomsB_Initialize(void);
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
extern struct ActorTemplate gCottageRoomsB_ActorTemplates[];
extern struct ActorTemplate gCottageRoomsB_ActorTemplatesAfterLeaving[];
extern struct ActorTemplate gCottageRoomsB_ActorTemplatesAfterKalay[];
extern struct ActorTemplate gCottageRoomsB_BabiScholarsActorTemplates[];
extern struct Trigger gCottageRoomsB_StormTriggers[];
extern struct Trigger gCottageRoomsB_Triggers[];
extern struct Trigger gCottageRoomsB_TriggersAfterLeaving[];
extern struct Trigger gCottageRoomsB_TriggersAfterKalay[];
extern struct Trigger gCottageRoomsB_BabiScholarsTriggers[];
extern struct Trigger gCottageRoomsB_BabiScholarsLaterTriggers[];

#endif
