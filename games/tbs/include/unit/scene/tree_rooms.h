#ifndef ALCHEMY_UNIT_MAP_TREE_ROOMS_H
#define ALCHEMY_UNIT_MAP_TREE_ROOMS_H

#include "types.h"
#include "scene_tables.h"
#include "scene_header.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 TreeRooms_EnterRoom(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s16 gTreeRooms_MapPatches[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gTreeRooms_LeaderGathersActions[];
extern s32 gTreeRooms_GaretGathersActions[];
extern s32 gTreeRooms_IvanGathersActions[];
extern s32 gTreeRooms_MiaGathersActions[];
extern s32 gTreeRooms_FaceSinksActions[];
extern s32 gTreeRooms_RejoinLeaderActions[];
extern u32 gTreeRooms_TintSteps[];
extern struct SceneEntrance gTreeRoomsEntrances[];
extern struct SceneExitRegion gTreeRoomsRegions[];
extern u32 gTreeRoomsExits[];
extern struct ActorTemplate gTreeRoomsActorTemplates[];
extern struct Trigger gTreeRoomsTriggers[];
extern s32 gTreeRooms_TintIndex;

#endif
