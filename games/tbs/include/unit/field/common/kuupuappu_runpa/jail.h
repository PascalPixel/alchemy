#ifndef ALCHEMY_UNIT_FIELD_COMMON_KUUPUAPPU_RUNPA_JAIL_H
#define ALCHEMY_UNIT_FIELD_COMMON_KUUPUAPPU_RUNPA_JAIL_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void ThiefLeader_Talk(void);
void LeftThief_Talk(void);
void RightThief_Talk(void);
void ThiefLeader_ReadMind(void);
void LeftThief_ReadMind(void);
void RightThief_ReadMind(void);
void VaultDoor_Leave(void);
s32 Scene_Initialize(void);

#endif
