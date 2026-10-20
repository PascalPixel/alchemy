#ifndef ALCHEMY_UNIT_SCENE_VALE_HOME_H
#define ALCHEMY_UNIT_SCENE_VALE_HOME_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeHome_GetEntrances(void);
const struct SceneRegion *ValeHome_GetRegions(void);
const u32 *ValeHome_GetExits(void);
const struct ScenePlacement *ValeHome_GetPlacements(void);
const struct SceneEvent *ValeHome_GetEvents(void);
s32 ValeHome_Initialize(void);
extern u32 gValeHome_Actor22ActionsHead[];
extern s32 gValeHome_Actor22Actions[];
extern s32 gValeHome_Actor24Actions[];
extern s32 gValeHome_Actor23Actions[];
extern s32 gValeHome_LeaderActions[];
extern s32 gValeHome_Actor25Actions[];
extern s32 gValeHome_Actor23BounceActions[];
extern s32 gValeHome_Actor24BounceActions[];
extern s32 gValeHome_FaceTargetActions[];
extern s32 gValeHome_Actor30Actions[];
extern s32 gValeHome_Actor31Actions[];
extern s32 gValeHome_Actor11Actions[];
extern s32 gValeHome_Actor12Actions[];
extern s32 gValeHome_Actor0And1Actions[];
extern s32 gValeHome_Actor21Actions[];
extern s32 gValeHome_Actor17Actions[];
extern s32 gValeHome_LeaderStepActions[];
extern s32 gValeHome_Actor25StepActions[];
extern struct SceneEntrance gValeHome_Entrances[];
extern u32 gValeHome_Exits[];
extern u32 gValeHome_StormExits[];
extern struct ScenePlacement gValeHome_Placements[];
extern struct ScenePlacement gValeHome_StormPlacements[];
extern struct ScenePlacement gValeHome_DeparturePlacements[];
extern struct ScenePlacement gValeHome_PlacementsAfterKalay[];
extern struct SceneEvent gValeHome_Events[];
extern struct SceneEvent gValeHome_StormEvents[];
extern struct SceneEvent gValeHome_DepartureEvents[];
extern struct SceneEvent gValeHome_EventsAfterLeaving[];
extern struct SceneEvent gValeHome_EventsAfterKalay[];
extern u16 gValeHome_DoorOpens[];

#endif
