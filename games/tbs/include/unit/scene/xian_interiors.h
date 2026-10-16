#ifndef ALCHEMY_UNIT_SCENE_XIAN_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_XIAN_INTERIORS_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *XianInteriors_GetEntrances(void);
const struct SceneRegion *XianInteriors_GetRegions(void);
const u32 *XianInteriors_GetExits(void);
const struct ScenePlacement *XianInteriors_GetPlacements(void);
const struct SceneEvent *XianInteriors_GetEvents(void);
s32 XianInteriors_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gXianInteriors_Entrances[];
extern u32 gXianInteriors_Exits[];
extern struct ScenePlacement gXianInteriors_Placements[];
extern struct ScenePlacement gXianInteriors_PlacementsEntrance8[];
extern struct SceneEvent gXianInteriors_Events[];
extern struct SceneEvent gXianInteriors_EventsEntrance8[];

#endif
