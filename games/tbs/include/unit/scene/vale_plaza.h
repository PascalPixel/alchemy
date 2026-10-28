#ifndef ALCHEMY_UNIT_MAP_VALE_PLAZA_H
#define ALCHEMY_UNIT_MAP_VALE_PLAZA_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *ValePlaza_GetEntrances(void);
const struct SceneExitRegion *ValePlaza_GetRegions(void);
const u32 *ValePlaza_GetExits(void);
const struct ActorPlacement *ValePlaza_GetActorPlacements(void);
const struct Trigger *ValePlaza_GetTriggers(void);
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
extern struct SceneEntrance gValePlaza_Entrances[];
extern u32 gValePlaza_Exits[];
extern u32 gValePlaza_StormExits[];
extern struct ActorPlacement gValePlaza_ActorPlacements[];
extern struct ActorPlacement gValePlaza_StormActorPlacements[];
extern struct ActorPlacement gValePlaza_DepartureActorPlacements[];
extern struct ActorPlacement gValePlaza_ActorPlacementsAfterKalay[];
extern struct Trigger gValePlaza_Triggers[];
extern struct Trigger gValePlaza_StormTriggers[];
extern struct Trigger gValePlaza_DepartureTriggers[];
extern struct Trigger gValePlaza_TriggersAfterLeaving[];
extern struct Trigger gValePlaza_TriggersAfterKalay[];
extern u16 gValePlaza_DoorOpens[];

#endif
