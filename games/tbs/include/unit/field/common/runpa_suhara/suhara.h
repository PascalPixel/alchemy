#ifndef ALCHEMY_UNIT_FIELD_COMMON_RUNPA_SUHARA_SUHARA_H
#define ALCHEMY_UNIT_FIELD_COMMON_RUNPA_SUHARA_SUHARA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

extern const struct SceneEntrance gInteriorEntrances[];
extern const u32 gInteriorExits[];
extern const struct ScenePlacement gInteriorPlacements[];
extern const struct SceneEvent gInteriorSealedEvents[];
extern const struct SceneEvent gInteriorReopenedEvents[];
extern const struct SceneEvent gSuharaGateHouseEvents[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void Innkeeper_Talk(void);
void Chef_Talk(void);
void Chef_ReadMind(void);
void InnClerk_Talk(void);
void TemplePriest_Talk(void);
void ItemMerchant_ReadMind(void);
void ItemMerchant_Talk(void);
void WeaponMerchant_ReadMind(void);
void WeaponMerchant_Talk(void);
void ArmorMerchant_ReadMind(void);
void ArmorMerchant_Talk(void);
s32 Scene_Initialize(void);
void TravelingPriest_Talk(void);

#endif
