#ifndef ALCHEMY_MODULE_SCENE_MADRA_H
#define ALCHEMY_MODULE_SCENE_MADRA_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern struct ActorPlacement gMadra_Placements[];
extern struct ActorPlacement gMadra_PlacementsOnReturn[];

void Shop_Run(s32 a, s32 b);

void Madra_GrantPassageToOsenia(void);

void Madra_MeetKarst(void);
extern struct Trigger gMadra_TriggersBeforeRaid[];
extern struct Trigger gMadra_TriggersAfterRaid[];
extern struct Trigger gMadra_TriggersOnReturn[];

void Madra_AskComingThroughHole(void);

void Madra_AskComingThroughSealedHole(void);
void Madra_MindReadKarst(void);

void Madra_KarstCallsForMenardi(void);

void Madra_WorkerBlocksSealedHole(void);

void Madra_AskAboutBriggs(void);

void Madra_MindReadBriggsFan(void);

void Madra_InspectPartyAtGate(void);
extern s16 gMadra_DoorOpening[];
extern s16 gMadra_CavernHomeDoorOpening[];

void FieldCamera_WaitForArrival(void);

#endif
