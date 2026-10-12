#ifndef ALCHEMY_UNIT_FIELD_SORU_STAR_STAR_H
#define ALCHEMY_UNIT_FIELD_SORU_STAR_STAR_H

#include "types.h"
#include "field_scene.h"

struct OverlayObject;

s32 UpdateOverlayObjectAngle(struct OverlayObject *object);
s32 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
s32 *SceneData_GetMessageTable(void);
s32 *SceneData_GetActorTable(void);
s32 *SceneData_GetEffectTable(void);
void Scene_BagVenusStar(void);
void Scene_BagMercuryStar(void);
void Scene_HandOverStars(void);
void Scene_BagJupiterStar(void);
void FieldScene_StagePairedActors(void);
void FieldScene_RunElementalStarDemand(void);
void Scene_OfferGuarantee(void);
void Scene_UnmaskGarcia(void);
void Scene_AlexTakesStars(void);
void SoruStar_LineUpFollowers(void);
void Scene_BagMarsStar(void);
void Sukureta_Talk(void);
void Jasmine_Talk(void);
void Saturos_Talk(void);
void Menardi_Talk(void);
void Garcia_Talk(void);
void Alex_Talk(void);
void SceneDialogue_RunLine1072WithPair9And10(void);
void SoruStar_CheckLeaderAhead(void);
void Scene_EnterStarRoom(void);
s32 SoruStar_ApplyEntryState(void);
void Event_SayThenWait(s32 speaker, s32 frames);
s32 Scene_PresentItem(s32 item, s32 x, s32 y, s32 z);
void Scene_GiveMythrilBags(void);
void SoruStar_RiseActorFourteenSparks(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 SoruStar_AngleScript[];
extern s32 SoruStar_PresentItemScript[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern u16 SoruStar_StarCells[];
extern u8 Soru_RingDrift[][3];
extern u8 Soru_RingSwing[][3];
extern s8 Soru_RingDirection[][3];
extern s32 Soru_RingOffsetX[];
extern s32 Soru_RingOffsetZ[];

#endif
