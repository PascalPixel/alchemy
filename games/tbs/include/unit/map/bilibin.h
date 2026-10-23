#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Bilibin_GetEntrances(void);
const struct SceneRegion *Bilibin_GetRegions(void);
const u32 *Bilibin_GetExits(void);
const struct ScenePlacement *Bilibin_GetPlacements(void);
const struct SceneEvent *Bilibin_GetEvents(void);
s32 Bilibin_Initialize(void);
extern s32 gBilibin_FaceLeaderScript[];
extern u16 gBilibinEntrancesOther[];
extern struct SceneEntrance gBilibinEntrances[];
extern struct SceneEntrance gBilibinBarricadeEntrances[];
extern struct SceneEntrance gBilibinTreasuryEntrances[];
extern s16 gBilibinTreasuryRegions[];
extern u32 gBilibinExits[];
extern struct ScenePlacement gBilibinPlacementsOther[];
extern struct ScenePlacement gBilibinPlacements[];
extern struct ScenePlacement gBilibinBarricadePlacements[];
extern struct ScenePlacement gBilibinTreasuryPlacements[];
extern struct SceneEvent gBilibinEventsOther[];
extern struct SceneEvent gBilibinEvents[];
extern struct SceneEvent gBilibinBarricadeEvents[];
extern struct SceneEvent gBilibinTreasuryEvents[];
extern u16 gBilibin_SanctumDoorSteps[];
extern u16 gBilibin_ItemShopDoorSteps[];
extern u16 gBilibin_WeaponAndArmorShopDoorSteps[];
extern u16 gBilibin_HouseEastDoorSteps[];
extern u16 gBilibin_InnDoorSteps[];
extern u16 gBilibin_HouseWestDoorSteps[];
extern u16 gBilibin_HouseNorthDoorSteps[];
extern u16 *gBilibin_DoorSteps[];
extern u32 gBilibin_DoorCells[];
extern u32 gBilibin_CratesLaneCells[];
extern s32 gBilibin_FacingCellSteps[];
extern s32 gBilibin_RippleScript[];

#endif
