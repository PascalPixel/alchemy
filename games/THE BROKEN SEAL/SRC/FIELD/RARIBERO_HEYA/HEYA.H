#ifndef HEYA_H
#define HEYA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* The scene tables of the Lalivero house and sanctum, in the overlay's data.
 * The sanctum and the house share one overlay; flag 0x9a7 selects the later
 * placements and events. */
extern u8 gRariberoEntrances[];
extern u8 gRariberoSanctumRegions[];
extern u8 gRariberoRegions[];
extern u8 gRariberoExits[];
extern u8 gRariberoPlacements[];
extern u8 gRariberoHeyaPlacements[];
extern u8 gRariberoHeyaPlacements9a7[];
extern u8 gRariberoEvents[];
extern u8 gRariberoEvents9a7[];
extern u8 gRariberoHeyaEvents[];
extern u8 gRariberoHeyaEvents9a7[];

/* Main-image services reached through the overlay's import veneers, declared
 * old-style where call sites pass differing arguments. */
void Engine_ResetSceneEffectCounter(void);
void Owner_RefreshActiveRatios();
s32 Motion_LaunchFromFocusedObject();
void FieldScene_RunSecondaryScript();

/* SCRIPT.C: the report the house plays when the party arrives to give it. */
void FieldScene_RunPrimaryScript(void);

/* The pose is the attached effect the actor carries. */

#endif
