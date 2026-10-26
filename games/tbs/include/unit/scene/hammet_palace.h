#ifndef ALCHEMY_UNIT_MAP_HAMMET_PALACE_H
#define ALCHEMY_UNIT_MAP_HAMMET_PALACE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gHammetPalace_PartyActions[];
extern struct SceneEntrance gHammetPalaceEntrancesOther[];
extern struct SceneEntrance gHammetPalaceEntrances[];
extern u32 gHammetPalaceExits[];
extern struct ActorTemplate gHammetPalaceActorTemplatesOther[];
extern struct ActorTemplate gHammetPalaceActorTemplates[];
extern struct Trigger gHammetPalaceTriggersOther[];
extern struct Trigger gHammetPalaceTriggers[];

#endif
