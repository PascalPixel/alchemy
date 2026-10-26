#ifndef ALCHEMY_UNIT_DEBUG_LEVEL_H
#define ALCHEMY_UNIT_DEBUG_LEVEL_H

#include "types.h"
#include "map_tables.h"

u8 *IncreaseItemAndLevel_GetWarpDests(void);
s32 Scene_GetWarpRegions(void);
u8 *IncreaseItemAndLevel_GetWarps(void);
u8 *IncreaseItemAndLevel_GetObjectEventTemplates(void);
u8 *IncreaseItemAndLevel_GetEvents(void);
s32 IncreaseItemAndLevel_Initialize(void);
extern char gIncreaseItemAndLevel_LvLabel[4];
extern char gIncreaseItemAndLevel_ItemPrompt[12];
extern char gIncreaseItemAndLevel_ItemHelp[24];
extern char gIncreaseItemAndLevel_ItemFull[20];
extern char gIncreaseItemAndLevel_PsyPrompt[8];
extern char gIncreaseItemAndLevel_PsyHelp[12];
extern char gIncreaseItemAndLevel_GlyphsUpper[28];
extern char gIncreaseItemAndLevel_GlyphsLower[28];
extern char gIncreaseItemAndLevel_GlyphsMarks[12];
extern struct MapWarpDest gIncreaseItemAndLevel_WarpDests[];
extern u32 gIncreaseItemAndLevel_Warps[];
extern struct ObjectEventTemplate gIncreaseItemAndLevel_ObjectEventTemplates[];
extern struct MapEvent gIncreaseItemAndLevel_Events[];

#endif
