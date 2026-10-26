#ifndef ALCHEMY_UNIT_MAP_BILIBIN_H
#define ALCHEMY_UNIT_MAP_BILIBIN_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Bilibin_GetEntrances(void);
const struct SceneExitRegion *Bilibin_GetRegions(void);
const u32 *Bilibin_GetExits(void);
const struct ObjectEventTemplate *Bilibin_GetObjectEventTemplates(void);
const struct MapEvent *Bilibin_GetEvents(void);
s32 Bilibin_Initialize(void);
extern s32 gBilibin_FaceLeaderScript[];
extern u16 gBilibinEntrancesOther[];
extern struct SceneEntrance gBilibinEntrances[];
extern struct SceneEntrance gBilibinBarricadeEntrances[];
extern struct SceneEntrance gBilibinTreasuryEntrances[];
extern s16 gBilibinTreasuryRegions[];
extern u32 gBilibinExits[];
extern struct ObjectEventTemplate gBilibinObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gBilibinObjectEventTemplates[];
extern struct ObjectEventTemplate gBilibinBarricadeObjectEventTemplates[];
extern struct ObjectEventTemplate gBilibinTreasuryObjectEventTemplates[];
extern struct MapEvent gBilibinEventsOther[];
extern struct MapEvent gBilibinEvents[];
extern struct MapEvent gBilibinBarricadeEvents[];
extern struct MapEvent gBilibinTreasuryEvents[];
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
