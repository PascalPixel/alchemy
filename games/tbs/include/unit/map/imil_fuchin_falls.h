#ifndef ALCHEMY_UNIT_MAP_IMIL_FUCHIN_FALLS_H
#define ALCHEMY_UNIT_MAP_IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "map_tables.h"
#include "field_effect.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

extern s32 gImilFuchinFalls_TrackLeader;
const struct MapWarpDest *ImilFuchinFalls_GetWarpDests(void);
const struct MapWarpRegion *ImilFuchinFalls_GetRegions(void);
const u32 *ImilFuchinFalls_GetWarps(void);
const struct ObjectEventTemplate *ImilFuchinFalls_GetObjectEventTemplates(void);
const struct MapEvent *ImilFuchinFalls_GetEvents(void);
s32 ImilFuchinFalls_Initialize(void);
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra);
extern s16 gImilFuchinFalls_KeyHeadings[];
extern s16 gImilFuchinFalls_SlideHeadings[];
extern struct ObjectEvent *gImilFuchinFalls_DragonsEye;
extern struct MapWarpDest gImilFuchinFalls_NoWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_ImilFallsCaveWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_FirstRoomWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_SecondRoomWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_ThirdRoomWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_FourthRoomWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_KiRoomWarpDests[];
extern struct MapWarpDest gImilFuchinFalls_DragonRoomWarpDests[];
extern u32 gImilFuchinFalls_Warps[];
extern struct ObjectEventTemplate gImilFuchinFalls_NoObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_ImilFallsCaveObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_FirstRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_SecondRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_ThirdRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_FourthRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gImilFuchinFalls_DragonRoomObjectEventTemplates[];
extern struct MapEvent gImilFuchinFalls_NoEvents[];
extern struct MapEvent gImilFuchinFalls_ImilFallsCaveEvents[];
extern struct MapEvent gImilFuchinFalls_FirstRoomEvents[];
extern struct MapEvent gImilFuchinFalls_SecondRoomEvents[];
extern struct MapEvent gImilFuchinFalls_ThirdRoomEvents[];
extern struct MapEvent gImilFuchinFalls_FourthRoomEvents[];
extern struct MapEvent gImilFuchinFalls_KiRoomEvents[];
extern struct MapEvent gImilFuchinFalls_DragonRoomEvents[];
extern s32 gImilFuchinFalls_ShortDustScript[];
extern s32 gImilFuchinFalls_MediumDustScript[];
extern s32 gImilFuchinFalls_LongDustScript[];

#endif
