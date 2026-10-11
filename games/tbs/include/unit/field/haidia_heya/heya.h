#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_HEYA_HEYA_H

#include "types.h"

struct FacingObject;

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj);
void *HaidiaHeya_GetEntrances(void);
int SceneData_ReturnZero(void);
void *HaidiaHeya_SelectExits(void);
void *SceneData_SelectFlaggedTable(void);
void *HaidiaHeya_SelectEvents(void);
void Villager_AskWhySukuretaCame(void);
void Villager_PlanToScareVisitors(void);
void Scene_GiveFarewellHerb(void);
void Villager_AskAboutStrangePowers(void);
void Villager_AskAboutDora(void);
void HaidiaHeya_TalkHopeDidntGetSick(void);
void HaidiaHeya_RequestWindowExit(s32 value);
void FieldScene_RunIndexedStep1(void);
void FieldScene_RunIndexedStep2(void);
void FieldScene_RunIndexedStep3(void);
void FieldScene_RunIndexedStep4(void);
void FieldScene_RunIndexedStep5(void);
void FieldScene_RunIndexedStep6(void);
void FieldScene_RunIndexedStep7(void);
void FieldScene_RunIndexedStep8(void);
s32 Scene_RunSupplementalSequenceOne(void);
void FieldScene_RunByActorDirectionAndFlags(void);
void HaidiaHeya_OpenArmorShop(void);
void HaidiaHeya_OpenItemShop(void);
void FieldScene_RunLongPresentationSequence(void);
void Scene_UpdateTimedActor(void);

#endif
