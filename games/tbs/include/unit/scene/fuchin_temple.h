#ifndef ALCHEMY_UNIT_MAP_FUCHIN_TEMPLE_H
#define ALCHEMY_UNIT_MAP_FUCHIN_TEMPLE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
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
