#ifndef ALCHEMY_UNIT_MAP_FUCHIN_TEMPLE_H
#define ALCHEMY_UNIT_MAP_FUCHIN_TEMPLE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gFuchinTemple_JumpingStudentScript[];
extern s32 gFuchinTemple_HiddenSpriteScript[];
extern u16 gFuchinTemple_TempleDoorOpens[];
extern u16 gFuchinTemple_FehsDoorOpens[];
extern u16 gFuchinTemple_FehsDoorCloses[];
extern struct SceneEntrance gFuchinTempleGroundsEntrances[];
extern struct SceneEntrance gFuchinTempleEntrancesOther[];
extern u32 gFuchinTempleExits[];
extern struct ObjectEventTemplate gFuchinTempleGroundsObjectEventTemplates[];
extern struct ObjectEventTemplate gFuchinTempleObjectEventTemplates[];
extern struct ObjectEventTemplate
    gFuchinTempleObjectEventTemplatesFromLamaTemple[];
extern struct MapEvent gFuchinTempleGroundsEvents[];
extern struct MapEvent gFuchinTempleEvents[];
extern struct MapEvent gFuchinTempleEventsFromLamaTemple[];
extern s32 gFuchinTemple_ShotScript[];

#endif
