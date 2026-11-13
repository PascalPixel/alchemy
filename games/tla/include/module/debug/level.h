#ifndef ALCHEMY_MODULE_DEBUG_LEVEL_H
#define ALCHEMY_MODULE_DEBUG_LEVEL_H

/// @file
/// What games/tla/src/debug/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct ActorPlacement gIncreaseItemAndLevel_ActorPlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_FirstNamePlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_SecondNamePlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_ThirdNamePlacements[];
extern struct ActorPlacement gIncreaseItemAndLevel_FourthNamePlacements[];
extern struct Trigger gIncreaseItemAndLevel_NameTriggers[];
extern struct Trigger gIncreaseItemAndLevel_Triggers[];
extern char gIncreaseItemAndLevel_LvLabel[];
extern char gIncreaseItemAndLevel_ItemPrompt[];
extern char gIncreaseItemAndLevel_ItemHelp[];
extern char gIncreaseItemAndLevel_ItemFull[];
void IncreaseItemAndLevel_RunMotionTest(void);
void IncreaseItemAndLevel_EnableEffectChildMode(void);
void IncreaseItemAndLevel_OpenIconBrowser(void);
void IncreaseItemAndLevel_OpenPortraitBrowser(void);

#endif
