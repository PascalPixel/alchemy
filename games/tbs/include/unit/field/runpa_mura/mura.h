#ifndef ALCHEMY_UNIT_FIELD_RUNPA_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_RUNPA_MURA_MURA_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);
extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern const s32 *gEffectScripts[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FloatingNut_Catch(void);
void HiddenPuddle_Freeze(void);
void IcePillar_UpdateDrawOrder(void);
void Reveal_ShowSecrets(void);
void Reveal_HideSecrets(void);
void Well_Search(void);
const struct SceneEvent *Scene_GetEvents(void);
void WestGuard_Talk(void);
void EastGuard_Talk(void);
void EastGuard_MindRead(void);
void VillagerA_Talk(void);
void VillagerC_Talk(void);
void VillagerG_Talk(void);
void VillagerB_Shivers(void);
void VillagerD_Talk(void);
void LeftGuard_Talk(void);
void RightGuard_Talk(void);
void LeftGuard_MindRead(void);
void RightGuard_MindRead(void);
void Party_WatchForFortress(void);
void Gerald_RefusesToReturn(void);
void Door_Enter(void);
void HiddenPassage_Enter(void);
void FortressGate_Enter(void);
void Party_CheckAhead(void);
void Reveal_PlayTreasureCue(void);
void HiddenSwitch_Pull(void);
void Guards_BlockGate(void);
void Scene_DoNothing(void);
void Cloak_Begin(void);
void Guards_CatchParty(void);
void Gateway_Reopen(void);
void Cloak_End(void);
void Guards_Watch(void);
void Leader_KickUpDust(void);
void Party_ThrownOut(void);
void Leader_SneaksOut(void);
s32 Scene_Initialize(void);
s32 SceneEffect_UpdateOrbitPosition(s32 *p);
void InitializeStagedActorSceneOrbitingEffect(s32 id);
extern struct SceneEntrance gLunpaEntrances[];
extern struct SceneEntrance gGateEntrances[];
extern u32 gLunpaExits[];
extern u32 gGateExits[];
extern struct ScenePlacement gLunpaPlacements[];
extern struct ScenePlacement gGatePlacements[];
extern struct SceneEvent gLunpaSealedEvents[];
extern struct SceneEvent gLunpaReopenedEvents[];
extern struct SceneEvent gGateEvents[];
extern u32 gLunpaUnusedWord;

#endif
