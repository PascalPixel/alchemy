#ifndef ALCHEMY_MODULE_SCENE_DAILA_H
#define ALCHEMY_MODULE_SCENE_DAILA_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct SceneEntrance gDailaEntrances[];
extern s32 gData_02001018[];
extern s32 gData_02001068[];
extern struct ActorPlacement gDaila1ActorPlacements[];
extern struct ActorPlacement gDaila2ActorPlacements[];
extern struct Trigger gDaila1Triggers[];
extern struct Trigger gDaila2Triggers[];
extern s16 gData_02000d80[];

void Func_02000d18(void);

void Func_02000d68(s32 a, s32 b);

extern s32 gData_02000d9c[];

extern u32 gData_02000f44[];

void Func_02000d58(s16 *data);

#endif
