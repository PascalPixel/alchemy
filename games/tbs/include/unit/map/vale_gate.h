#ifndef ALCHEMY_UNIT_SCENE_VALE_GATE_H
#define ALCHEMY_UNIT_SCENE_VALE_GATE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *ValeGate_GetEntrances(void);
const struct SceneRegion *ValeGate_GetRegions(void);
const u32 *ValeGate_GetExits(void);
const struct ScenePlacement *ValeGate_GetPlacements(void);
const struct SceneEvent *ValeGate_GetEvents(void);
s32 ValeGate_Initialize(void);
extern u8 gValeGate_UnusedPaletteSteps[];
extern s32 gValeGate_ChargedUpStrongmanActions[];
extern s32 gValeGate_SecondTravelerLeapActions[];
extern s32 gValeGate_FirstTravelerLeapActions[];
extern s32 gValeGate_LeaderActions[];
extern s32 gValeGate_StormGaretLeapActions[];
extern s32 gValeGate_FirstTravelerBounceActions[];
extern s32 gValeGate_SecondTravelerBounceActions[];
extern s32 gValeGate_FaceTargetActions[];
extern s32 gValeGate_FarewellDogActions[];
extern s32 gValeGate_FarewellChaseActions[];
extern s32 gValeGate_FarewellFirstChildActions[];
extern s32 gValeGate_FarewellSecondChildActions[];
extern s32 gValeGate_PartyDepartureActions[];
extern s32 gValeGate_HutOwnerSearchActions[];
extern s32 gValeGate_AideRunActions[];
extern s32 gValeGate_LeaderStepActions[];
extern s32 gValeGate_StormGaretStepActions[];
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
