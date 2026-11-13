#ifndef ALCHEMY_MODULE_SCENE_FUCHIN_TEMPLE_H
#define ALCHEMY_MODULE_SCENE_FUCHIN_TEMPLE_H

#include "types.h"
#include "scene_tables.h"

extern s32 gFuchinTemple_JumpingStudentScript[];
extern s32 gFuchinTemple_HiddenSpriteScript[];
extern u16 gFuchinTemple_TempleDoorOpens[];
extern u16 gFuchinTemple_FehsDoorOpens[];
extern u16 gFuchinTemple_FehsDoorCloses[];
extern struct SceneEntrance gFuchinTempleGroundsEntrances[];
extern struct SceneEntrance gFuchinTempleEntrancesOther[];
extern u32 gFuchinTempleExits[];
extern struct ActorPlacement gFuchinTempleGroundsActorPlacements[];
extern struct ActorPlacement gFuchinTempleActorPlacements[];
extern struct ActorPlacement gFuchinTempleActorPlacementsFromLamaTemple[];
extern struct Trigger gFuchinTempleGroundsTriggers[];
extern struct Trigger gFuchinTempleTriggers[];
extern struct Trigger gFuchinTempleTriggersFromLamaTemple[];
extern s32 gFuchinTemple_ShotScript[];

#endif
