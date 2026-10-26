#ifndef ALCHEMY_UNIT_MAP_BABI_PALACE_H
#define ALCHEMY_UNIT_MAP_BABI_PALACE_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *BabiPalace_GetRegions(void);
const u32 *BabiPalace_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 BabiPalace_ApplyEntryState(void);
extern u16 gBabiPalace_DoorCells[];
extern s32 gBabiPalace_IndigestionScript[];
extern struct SceneEntrance gBabiPalace_Entrances[];
extern struct SceneEntrance gBabiPalace_OutsideEntrances[];
extern struct SceneExitRegion gBabiPalace_Regions[];
extern u32 gBabiPalace_Exits[];
extern struct ObjectEventTemplate gBabiPalace_OutsideObjectEventTemplates[];
extern struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesBabiMissing[];
extern struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesFinals[];
extern struct ObjectEventTemplate
    gBabiPalace_ObjectEventTemplatesAfterColosso[];
extern struct MapEvent gBabiPalace_EventsBabiMissing[];
extern struct MapEvent gBabiPalace_EventsFinals[];
extern struct MapEvent gBabiPalace_EventsAfterColosso[];
extern struct MapEvent gBabiPalace_OutsideEventsBabiMissing[];
extern struct MapEvent gBabiPalace_OutsideEventsFinals[];
extern struct MapEvent gBabiPalace_OutsideEventsAfterColosso[];

#endif
