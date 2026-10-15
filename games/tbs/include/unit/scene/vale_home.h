#ifndef ALCHEMY_UNIT_SCENE_VALE_HOME_H
#define ALCHEMY_UNIT_SCENE_VALE_HOME_H

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
extern s32 HaidiaIe_Actor23BounceActions[];
extern s32 HaidiaIe_Actor24BounceActions[];
extern s32 HaidiaIe_FaceTargetActions[];
extern s32 HaidiaIe_Actor30Actions[];
extern s32 HaidiaIe_Actor31Actions[];
extern s32 HaidiaIe_Actor11Actions[];
extern s32 HaidiaIe_Actor12Actions[];
extern s32 HaidiaIe_Actor0And1Actions[];
extern s32 HaidiaIe_Actor21Actions[];
extern s32 HaidiaIe_Actor17Actions[];
extern s32 HaidiaIe_LeaderStepActions[];
extern s32 HaidiaIe_Actor25StepActions[];
extern struct SceneEntrance gHaidiaIeEntrances[];
extern u32 gHaidiaIeExits[];
extern u32 gHaidiaIeExitsFlag834[];
extern struct ScenePlacement gHaidiaIePlacements[];
extern struct ScenePlacement gHaidiaIePlacementsFlag834[];
extern struct ScenePlacement gHaidiaIePlacementsEntrance12[];
extern struct ScenePlacement gHaidiaIePlacementsFlag87a[];
extern struct SceneEvent gHaidiaIeEvents[];
extern struct SceneEvent gHaidiaIeEventsFlag834[];
extern struct SceneEvent gHaidiaIeEventsEntrance12[];
extern struct SceneEvent gHaidiaIeEventsAfterLeaving[];
extern struct SceneEvent gHaidiaIeEventsFlag87a[];
extern u16 HaidiaIe_CellSteps[];

#endif
