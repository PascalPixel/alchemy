#ifndef ALCHEMY_UNIT_MAP_IMIL_H
#define ALCHEMY_UNIT_MAP_IMIL_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *Imil_GetEntrances(void);
const struct SceneExitRegion *Imil_GetRegions(void);
const u32 *Imil_GetExits(void);
const struct ActorTemplate *Imil_GetActorTemplates(void);
const struct Trigger *Imil_GetTriggers(void);
s32 Imil_Initialize(void);
extern s16 gImil_KeyHeadings[];
extern s16 gImil_SwayHeadings[];
extern s32 gImil_FreezingYoungManScript[];
extern s32 gImil_ShiverScript[];
extern s32 gImil_OldWomanHurryScript[];
extern s32 gImil_LeaderStepAsideScript[];
extern s32 gImil_BobScript[];
extern s32 gImil_SparkScript[];
extern s32 gImil_MiaCheckRoomScript[];
extern s32 gImil_MiaRunOutScript[];
extern s32 gImil_FaceTargetScript[];
extern s32 gImil_FollowMiaOutScript[];
extern struct SceneEntrance gImil_TownEntrances[];
extern struct SceneEntrance gImil_InteriorEntrances[];
extern u32 gImil_Exits[];
extern struct ActorTemplate gImil_TownActorTemplates[];
extern struct ActorTemplate gImil_TownActorTemplatesCured[];
extern struct ActorTemplate gImil_InteriorActorTemplates[];
extern struct Trigger gImil_TownTriggers[];
extern struct Trigger gImil_InteriorTriggers[];
extern u16 gImil_SanctumDoorSteps[];
extern u16 gImil_InteriorsEastDoorSteps[];
extern u16 gImil_InnDoorSteps[];
extern u16 gImil_HouseSouthwestDoorSteps[];
extern u16 gImil_HouseWestDoorSteps[];
extern u16 gImil_HouseNortheastDoorSteps[];
extern u16 *gImil_DoorCellSteps[];
extern s16 gImil_DoorCellPoints[][2];
extern u16 gImil_CellStepsA[];
extern u16 gImil_CellStepsB[];

#endif
