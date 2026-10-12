#ifndef ALCHEMY_UNIT_FIELD_KORIMA_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_KORIMA_MURA_MURA_H

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
extern s32 KorimaMura_ActionTable1[];
extern s32 KorimaMura_ActionTable2[];
extern s32 KorimaMura_ActionTable3[];
extern s32 KorimaMura_ActionTable4[];
extern s32 KorimaMura_ActionTable5[];
extern s32 KorimaMura_ActionTable6[];
extern s32 KorimaMura_ActionTable7[];
extern s32 KorimaMura_EntryActions[];
extern s32 KorimaMura_DebrisScript[];
extern s32 KorimaMura_Actor8Path[];
extern u16 KorimaMura_SwitchCells[];
extern struct SceneEntrance gKorimaMuraEntrances[];
extern struct SceneEntrance gKorimaMuraEntrances3[];
extern struct SceneEntrance gKorimaMuraEntrances2[];
extern s16 gKorimaMuraRegions2[];
extern u32 gKorimaMuraExits[];
extern struct ScenePlacement gKorimaMuraPlacements[];
extern struct ScenePlacement gKorimaMuraPlacements1[];
extern struct ScenePlacement KorimaMura_EntryScript[];
extern struct ScenePlacement gKorimaMuraPlacements3[];
extern struct SceneEvent gKorimaMuraEvents[];
extern struct SceneEvent gKorimaMuraEvents3[];
extern s32 KorimaMura_Object26Script[];
extern s32 KorimaMura_RegionScript[];
extern s32 gFallingEffectScript[];

#endif
