#ifndef ALCHEMY_UNIT_MAP_IMIL_H
#define ALCHEMY_UNIT_MAP_IMIL_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *Imil_GetWarpDests(void);
const struct MapWarpRegion *Imil_GetRegions(void);
const u32 *Imil_GetWarps(void);
const struct ObjectEventTemplate *Imil_GetObjectEventTemplates(void);
const struct MapEvent *Imil_GetEvents(void);
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
extern struct MapWarpDest gImil_TownWarpDests[];
extern struct MapWarpDest gImil_InteriorWarpDests[];
extern u32 gImil_Warps[];
extern struct ObjectEventTemplate gImil_TownObjectEventTemplates[];
extern struct ObjectEventTemplate gImil_TownObjectEventTemplatesCured[];
extern struct ObjectEventTemplate gImil_InteriorObjectEventTemplates[];
extern struct MapEvent gImil_TownEvents[];
extern struct MapEvent gImil_InteriorEvents[];
extern u16 gImil_DoorCellSteps5[];
extern u16 gImil_DoorCellSteps4[];
extern u16 gImil_DoorCellSteps1[];
extern u16 gImil_DoorCellSteps0[];
extern u16 gImil_DoorCellSteps2[];
extern u16 gImil_DoorCellSteps3[];
extern u16 *gImil_DoorCellSteps[];
extern s16 gImil_DoorCellPoints[][2];
extern u16 gImil_CellStepsA[];
extern u16 gImil_CellStepsB[];

#endif
