#ifndef ALCHEMY_UNIT_DEBUG_LEVEL_H
#define ALCHEMY_UNIT_DEBUG_LEVEL_H

#include "types.h"
#include "map_tables.h"

u8 *ItemLevel_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *ItemLevel_GetWarps(void);
u8 *ItemLevel_GetObjectEventTemplates(void);
u8 *ItemLevel_GetEvents(void);
s32 Map_RunLevelRoomSetup(void);
extern char gItemLevelLvLabel[4];
extern char gItemLevelItemPrompt[12];
extern char gItemLevelItemHelp[24];
extern char gItemLevelItemFull[20];
extern char gItemLevelPsyPrompt[8];
extern char gItemLevelPsyHelp[12];
extern char gItemLevelGlyphsUpper[28];
extern char gItemLevelGlyphsLower[28];
extern char gItemLevelGlyphsMarks[12];
extern struct MapWarpDest gItemLevelWarpDests[];
extern u32 gItemLevelWarps[];
extern struct ObjectEventTemplate gItemLevelObjectEventTemplates[];
extern struct MapEvent gItemLevelEvents[];

#endif
