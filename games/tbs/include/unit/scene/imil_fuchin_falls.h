#ifndef ALCHEMY_UNIT_MAP_IMIL_FUCHIN_FALLS_H
#define ALCHEMY_UNIT_MAP_IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "map_tables.h"
#include "field_effect.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

extern s32 gImilFuchinFalls_TrackLeader;
const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void);
const struct SceneExitRegion *ImilFuchinFalls_GetRegions(void);
const u32 *ImilFuchinFalls_GetExits(void);
const struct ObjectEventTemplate *ImilFuchinFalls_GetObjectEventTemplates(void);
const struct MapEvent *ImilFuchinFalls_GetEvents(void);
s32 ImilFuchinFalls_Initialize(void);
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra);
extern s16 gImilFuchinFalls_KeyHeadings[];
extern s16 gImilFuchinFalls_SlideHeadings[];
extern struct ObjectEvent *gImilFuchinFalls_DragonsEye;
extern struct SceneEntrance gImilFuchinFalls_NoEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[];
extern struct SceneEntrance gImilFuchinFalls_EntranceEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ZephyrRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonsEyeRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ArcticBladeRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[];
extern u32 gImilFuchinFalls_Exits[];
extern struct ObjectEventTemplate gImilFuchinFalls_NoObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_ImilFallsCaveObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_EntranceObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_ZephyrRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_DragonsEyeRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_ArcticBladeRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_DragonRoomObjectEventTemplates[];
extern struct MapEvent gImilFuchinFalls_NoEvents[];
extern struct MapEvent gImilFuchinFalls_ImilFallsCaveEvents[];
extern struct MapEvent gImilFuchinFalls_EntranceEvents[];
extern struct MapEvent gImilFuchinFalls_ZephyrRoomEvents[];
extern struct MapEvent gImilFuchinFalls_DragonsEyeRoomEvents[];
extern struct MapEvent gImilFuchinFalls_ArcticBladeRoomEvents[];
extern struct MapEvent gImilFuchinFalls_KiRoomEvents[];
extern struct MapEvent gImilFuchinFalls_DragonRoomEvents[];
extern s32 gImilFuchinFalls_ShortDustScript[];
extern s32 gImilFuchinFalls_MediumDustScript[];
extern s32 gImilFuchinFalls_LongDustScript[];

#endif
