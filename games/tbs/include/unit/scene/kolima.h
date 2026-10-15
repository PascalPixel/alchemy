#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 Kolima_ActionTable1[];
extern s32 Kolima_ActionTable2[];
extern s32 Kolima_ActionTable3[];
extern s32 Kolima_ActionTable4[];
extern s32 Kolima_ActionTable5[];
extern s32 Kolima_ActionTable6[];
extern s32 Kolima_ActionTable7[];
extern s32 Kolima_EntryActions[];
extern s32 Kolima_DebrisScript[];
extern s32 Kolima_Actor8Path[];
extern u16 Kolima_SwitchCells[];
extern struct SceneEntrance gKolimaEntrances[];
extern struct SceneEntrance gKolimaEntrances3[];
extern struct SceneEntrance gKolimaEntrances2[];
extern s16 gKolimaRegions2[];
extern u32 gKolimaExits[];
extern struct ScenePlacement gKolimaPlacements[];
extern struct ScenePlacement gKolimaPlacements1[];
extern struct ScenePlacement Kolima_EntryScript[];
extern struct ScenePlacement gKolimaPlacements3[];
extern struct SceneEvent gKolimaEvents[];
extern struct SceneEvent gKolimaEvents3[];
extern s32 Kolima_Object26Script[];
extern s32 Kolima_RegionScript[];
extern s32 gFallingEffectScript[];

#endif
