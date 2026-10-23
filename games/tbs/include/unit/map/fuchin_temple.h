#ifndef ALCHEMY_UNIT_MAP_FUCHIN_TEMPLE_H
#define ALCHEMY_UNIT_MAP_FUCHIN_TEMPLE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gFuchinTemple_JumpingStudentScript[];
extern s32 gFuchinTemple_HiddenSpriteScript[];
extern u16 gFuchinTemple_TempleDoorOpens[];
extern u16 gFuchinTemple_FehsDoorOpens[];
extern u16 gFuchinTemple_FehsDoorCloses[];
extern struct MapWarpDest gFuchinTempleGroundsWarpDests[];
extern struct MapWarpDest gFuchinTempleWarpDestsOther[];
extern u32 gFuchinTempleWarps[];
extern struct ObjectEventTemplate gFuchinTempleGroundsObjectEventTemplates[];
extern struct ObjectEventTemplate gFuchinTempleObjectEventTemplates[];
extern struct ObjectEventTemplate
    gFuchinTempleObjectEventTemplatesFromLamaTemple[];
extern struct MapEvent gFuchinTempleGroundsEvents[];
extern struct MapEvent gFuchinTempleEvents[];
extern struct MapEvent gFuchinTempleEventsFromLamaTemple[];
extern s32 gFuchinTemple_ShotScript[];

#endif
