#ifndef ALCHEMY_UNIT_MAP_VALE_GATE_H
#define ALCHEMY_UNIT_MAP_VALE_GATE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeGate_GetWarpDests(void);
const struct MapWarpRegion *ValeGate_GetRegions(void);
const u32 *ValeGate_GetWarps(void);
const struct ObjectEventTemplate *ValeGate_GetObjectEventTemplates(void);
const struct MapEvent *ValeGate_GetEvents(void);
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
extern struct MapWarpDest gValeGate_WarpDests[];
extern u32 gValeGate_Warps[];
extern u32 gValeGate_StormWarps[];
extern struct ObjectEventTemplate gValeGate_ObjectEventTemplates[];
extern struct ObjectEventTemplate gValeGate_StormObjectEventTemplates[];
extern struct ObjectEventTemplate gValeGate_DepartureObjectEventTemplates[];
extern struct ObjectEventTemplate gValeGate_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gValeGate_Events[];
extern struct MapEvent gValeGate_StormEvents[];
extern struct MapEvent gValeGate_DepartureEvents[];
extern struct MapEvent gValeGate_EventsAfterLeaving[];
extern struct MapEvent gValeGate_EventsAfterKalay[];
extern u16 gValeGate_DoorOpens[];

#endif
