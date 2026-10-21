#ifndef ALCHEMY_UNIT_SCENE_VALE_GATE_H
#define ALCHEMY_UNIT_SCENE_VALE_GATE_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeGate_GetEntrances(void);
const struct SceneRegion *ValeGate_GetRegions(void);
const u32 *ValeGate_GetExits(void);
const struct ScenePlacement *ValeGate_GetPlacements(void);
const struct SceneEvent *ValeGate_GetEvents(void);
s32 ValeGate_Initialize(void);
extern u32 gValeGate_Actor22ActionsHead[];
extern s32 gValeGate_Actor22Actions[];
extern s32 gValeGate_Actor24Actions[];
extern s32 gValeGate_Actor23Actions[];
extern s32 gValeGate_LeaderActions[];
extern s32 gValeGate_Actor25Actions[];
extern s32 gValeGate_Actor23BounceActions[];
extern s32 gValeGate_Actor24BounceActions[];
extern s32 gValeGate_FaceTargetActions[];
extern s32 gValeGate_Actor30Actions[];
extern s32 gValeGate_Actor31Actions[];
extern s32 gValeGate_Actor11Actions[];
extern s32 gValeGate_Actor12Actions[];
extern s32 gValeGate_Actor0And1Actions[];
extern s32 gValeGate_Actor21Actions[];
extern s32 gValeGate_Actor17Actions[];
extern s32 gValeGate_LeaderStepActions[];
extern s32 gValeGate_Actor25StepActions[];
extern struct SceneEntrance gValeGate_Entrances[];
extern u32 gValeGate_Exits[];
extern u32 gValeGate_StormExits[];
extern struct ScenePlacement gValeGate_Placements[];
extern struct ScenePlacement gValeGate_StormPlacements[];
extern struct ScenePlacement gValeGate_DeparturePlacements[];
extern struct ScenePlacement gValeGate_PlacementsAfterKalay[];
extern struct SceneEvent gValeGate_Events[];
extern struct SceneEvent gValeGate_StormEvents[];
extern struct SceneEvent gValeGate_DepartureEvents[];
extern struct SceneEvent gValeGate_EventsAfterLeaving[];
extern struct SceneEvent gValeGate_EventsAfterKalay[];
extern u16 gValeGate_DoorOpens[];

#endif
