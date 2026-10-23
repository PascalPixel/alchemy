#ifndef ALCHEMY_UNIT_MENU_TITLE_H
#define ALCHEMY_UNIT_MENU_TITLE_H

#include "types.h"
#include "map_tables.h"

u8 *Title_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *Title_GetWarps(void);
u8 *Title_GetObjectEventTemplates(void);
u8 *Title_GetEvents(void);
s32 Title_Run(void);
extern struct MapWarpDest gTitleWarpDests[];
extern u32 gTitleWarps[];
extern struct ObjectEventTemplate gTitleObjectEventTemplates[];
extern struct MapEvent gTitleEvents[];
extern s16 gTitleVramBlock;

#endif
