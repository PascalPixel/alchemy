#ifndef ALCHEMY_UNIT_SCENE_VALE_HOME_H
#define ALCHEMY_UNIT_SCENE_VALE_HOME_H

#include "types.h"
#include "field_scene.h"

u8 *ValeHome_GetEntrances(void);
s32 ValeHome_GetDefaultResult(void);
void *SceneData_SelectTableByFlag834(void);
void *SceneData_SelectTableByFlags834And87a(void);
void *SceneData_SelectTableByFlags87a_815_834(void);
s32 ValeHome_RestoreEntryState(void);
extern u32 ValeHome_Actor22ActionsHead[];
extern s32 ValeHome_Actor22Actions[];
extern s32 ValeHome_Actor24Actions[];
extern s32 ValeHome_Actor23Actions[];
extern s32 ValeHome_LeaderActions[];
extern s32 ValeHome_Actor25Actions[];
extern s32 ValeHome_Actor23BounceActions[];
extern s32 ValeHome_Actor24BounceActions[];
extern s32 ValeHome_FaceTargetActions[];
extern s32 ValeHome_Actor30Actions[];
extern s32 ValeHome_Actor31Actions[];
extern s32 ValeHome_Actor11Actions[];
extern s32 ValeHome_Actor12Actions[];
extern s32 ValeHome_Actor0And1Actions[];
extern s32 ValeHome_Actor21Actions[];
extern s32 ValeHome_Actor17Actions[];
extern s32 ValeHome_LeaderStepActions[];
extern s32 ValeHome_Actor25StepActions[];
extern struct SceneEntrance gValeHomeEntrances[];
extern u32 gValeHomeExits[];
extern u32 gValeHomeExitsFlag834[];
extern struct ScenePlacement gValeHomePlacements[];
extern struct ScenePlacement gValeHomePlacementsFlag834[];
extern struct ScenePlacement gValeHomePlacementsEntrance12[];
extern struct ScenePlacement gValeHomePlacementsFlag87a[];
extern struct SceneEvent gValeHomeEvents[];
extern struct SceneEvent gValeHomeEventsFlag834[];
extern struct SceneEvent gValeHomeEventsEntrance12[];
extern struct SceneEvent gValeHomeEventsAfterLeaving[];
extern struct SceneEvent gValeHomeEventsFlag87a[];
extern u16 ValeHome_CellSteps[];

#endif
