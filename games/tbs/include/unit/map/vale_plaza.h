#ifndef ALCHEMY_UNIT_MAP_VALE_PLAZA_H
#define ALCHEMY_UNIT_MAP_VALE_PLAZA_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValePlaza_GetWarpDests(void);
const struct MapWarpRegion *ValePlaza_GetRegions(void);
const u32 *ValePlaza_GetWarps(void);
const struct ObjectEventTemplate *ValePlaza_GetObjectEventTemplates(void);
const struct MapEvent *ValePlaza_GetEvents(void);
s32 ValePlaza_Initialize(void);
extern u8 gValePlaza_UnusedPaletteSteps[];
extern s32 gValePlaza_ChargedUpStrongmanActions[];
extern s32 gValePlaza_SecondTravelerLeapActions[];
extern s32 gValePlaza_FirstTravelerLeapActions[];
extern s32 gValePlaza_LeaderActions[];
extern s32 gValePlaza_StormGaretLeapActions[];
extern s32 gValePlaza_FirstTravelerBounceActions[];
extern s32 gValePlaza_SecondTravelerBounceActions[];
extern s32 gValePlaza_FaceTargetActions[];
extern s32 gValePlaza_FarewellDogActions[];
extern s32 gValePlaza_FarewellChaseActions[];
extern s32 gValePlaza_FarewellFirstChildActions[];
extern s32 gValePlaza_FarewellSecondChildActions[];
extern s32 gValePlaza_PartyDepartureActions[];
extern s32 gValePlaza_HutOwnerSearchActions[];
extern s32 gValePlaza_AideRunActions[];
extern s32 gValePlaza_LeaderStepActions[];
extern s32 gValePlaza_StormGaretStepActions[];
extern struct MapWarpDest gValePlaza_WarpDests[];
extern u32 gValePlaza_Warps[];
extern u32 gValePlaza_StormWarps[];
extern struct ObjectEventTemplate gValePlaza_ObjectEventTemplates[];
extern struct ObjectEventTemplate gValePlaza_StormObjectEventTemplates[];
extern struct ObjectEventTemplate gValePlaza_DepartureObjectEventTemplates[];
extern struct ObjectEventTemplate gValePlaza_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gValePlaza_Events[];
extern struct MapEvent gValePlaza_StormEvents[];
extern struct MapEvent gValePlaza_DepartureEvents[];
extern struct MapEvent gValePlaza_EventsAfterLeaving[];
extern struct MapEvent gValePlaza_EventsAfterKalay[];
extern u16 gValePlaza_DoorOpens[];

#endif
