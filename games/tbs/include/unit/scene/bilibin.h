#ifndef ALCHEMY_UNIT_MAP_BILIBIN_H
#define ALCHEMY_UNIT_MAP_BILIBIN_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Bilibin_GetEntrances(void);
const struct SceneExitRegion *Bilibin_GetRegions(void);
const u32 *Bilibin_GetExits(void);
const struct ActorTemplate *Bilibin_GetActorTemplates(void);
const struct Trigger *Bilibin_GetTriggers(void);
s32 Bilibin_Initialize(void);
extern s32 gBilibin_FaceLeaderScript[];
extern u16 gBilibinEntrancesOther[];
extern struct SceneEntrance gBilibinEntrances[];
extern struct SceneEntrance gBilibinBarricadeEntrances[];
extern struct SceneEntrance gBilibinTreasuryEntrances[];
extern s16 gBilibinTreasuryRegions[];
extern u32 gBilibinExits[];
extern struct ActorTemplate gBilibinActorTemplatesOther[];
extern struct ActorTemplate gBilibinActorTemplates[];
extern struct ActorTemplate gBilibinBarricadeActorTemplates[];
extern struct ActorTemplate gBilibinTreasuryActorTemplates[];
extern struct Trigger gBilibinTriggersOther[];
extern struct Trigger gBilibinTriggers[];
extern struct Trigger gBilibinBarricadeTriggers[];
extern struct Trigger gBilibinTreasuryTriggers[];
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
