#ifndef ALCHEMY_UNIT_SCENE_HAIDIA_IE_H
#define ALCHEMY_UNIT_SCENE_HAIDIA_IE_H

#include "types.h"
#include "field_scene.h"

u8 *HaidiaIe_GetEntrances(void);
s32 HaidiaIe_GetDefaultResult(void);
void *SceneData_SelectTableByFlag834(void);
void *SceneData_SelectTableByFlags834And87a(void);
void *SceneData_SelectTableByFlags87a_815_834(void);
s32 HaidiaIe_RestoreEntryState(void);
extern u32 HaidiaIe_Actor22ActionsHead[];
extern s32 HaidiaIe_Actor22Actions[];
extern s32 HaidiaIe_Actor24Actions[];
extern s32 HaidiaIe_Actor23Actions[];
extern s32 HaidiaIe_LeaderActions[];
extern s32 HaidiaIe_Actor25Actions[];
extern s32 Data_0200aa48[];
extern s32 Data_0200ab2c[];
extern s32 HaidiaIe_FaceTargetActions[];
extern s32 Data_0200ac14[];
extern s32 Data_0200ac90[];
extern s32 Data_0200acf8[];
extern s32 Data_0200ad74[];
extern s32 Data_0200adf0[];
extern s32 Data_0200ae34[];
extern s32 HaidiaIe_Actor17Actions[];
extern s32 HaidiaIe_LeaderStepActions[];
extern s32 HaidiaIe_Actor25StepActions[];
extern struct SceneEntrance Data_0200afa0[];
extern u32 Data_0200b108[];
extern u32 Data_0200b144[];
extern struct ScenePlacement Data_0200b170[];
extern struct ScenePlacement Data_0200b380[];
extern struct ScenePlacement Data_0200b560[];
extern struct ScenePlacement Data_0200b7d0[];
extern struct SceneEvent Data_0200b938[];
extern struct SceneEvent Data_0200ba64[];
extern struct SceneEvent Data_0200bb30[];
extern struct SceneEvent Data_0200bb3c[];
extern struct SceneEvent Data_0200bcec[];
extern u16 Value_0200beb4[];

#endif
