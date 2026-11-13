#ifndef ALCHEMY_MODULE_SCENE_TITLE_H
#define ALCHEMY_MODULE_SCENE_TITLE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Title_GetEntrances(void);
const u32 *Title_GetExits(void);
const struct ActorPlacement *Title_GetActorPlacements(void);
const struct Trigger *Title_GetTriggers(void);
s32 Title_Run(void);
extern struct SceneEntrance gTitleEntrances[];
extern u32 gTitleExits[];
extern struct ActorPlacement gTitleActorPlacements[];
extern struct Trigger gTitleTriggers[];
extern s16 gTitleVramBlock;

#endif
