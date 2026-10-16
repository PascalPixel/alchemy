#ifndef ALCHEMY_UNIT_SCENE_VALE_HOME_H
#define ALCHEMY_UNIT_SCENE_VALE_HOME_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *ValeHome_GetEntrances(void);
s32 ValeHome_GetDefaultResult(void);
u32 *ValeHome_SelectExits(void);
struct ScenePlacement *ValeHome_SelectPlacements(void);
struct SceneEvent *ValeHome_SelectEvents(void);
s32 ValeHome_RestoreEntryState(void);
extern u32 gValeHome_Actor22ActionsHead[];
extern s32 gValeHome_Actor22Actions[];
extern s32 gValeHome_Actor24Actions[];
extern s32 gValeHome_Actor23Actions[];
extern s32 gValeHome_LeaderActions[];
extern s32 gValeHome_Actor25Actions[];
extern s32 gValeHome_Actor23BounceActions[];
extern s32 gValeHome_Actor24BounceActions[];
extern s32 gValeHome_FaceTargetActions[];
extern s32 gValeHome_Actor30Actions[];
extern s32 gValeHome_Actor31Actions[];
extern s32 gValeHome_Actor11Actions[];
extern s32 gValeHome_Actor12Actions[];
extern s32 gValeHome_Actor0And1Actions[];
extern s32 gValeHome_Actor21Actions[];
extern s32 gValeHome_Actor17Actions[];
extern s32 gValeHome_LeaderStepActions[];
extern s32 gValeHome_Actor25StepActions[];
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
extern u16 gValeHome_CellSteps[];

#endif
