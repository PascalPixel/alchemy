#ifndef ALCHEMY_UNIT_MAP_TOLBI_GAMES_H
#define ALCHEMY_UNIT_MAP_TOLBI_GAMES_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *TolbiSpring_GetWarpDests(void);
const struct MapWarpRegion *TolbiSpring_GetRegions(void);
const u32 *TolbiSpring_GetWarps(void);
const struct ObjectEventTemplate *TolbiSpring_GetObjectEventTemplates(void);
const struct MapEvent *TolbiSpring_GetEvents(void);
s32 TolbiSpring_Initialize(void);
extern const struct MapWarpDest gTolbiSpring_WarpDests[];
extern const u32 gTolbiSpring_Warps[];
extern const struct ObjectEventTemplate gTolbiSpringObjectEventTemplates[];
extern const struct ObjectEventTemplate gTolbiSpringObjectEventTemplatesOther[];
extern const struct MapEvent gTolbiSpringEventsOther[];
extern const struct MapEvent gTolbiSpringEvents[];
extern const u16 gTolbiSpring_AlphaSteps[];
extern const u32 gTolbiSpring_PrizeItems[];
extern const u16 gTolbiSpring_CoinPrizes[];
extern const u32 gTolbiSpring_LuckyWheelsPrizeItems[];
extern const u8 gTolbiSpring_SliderFrames[];
extern const u8 gTolbiSpring_CircleFrames[];
extern const u8 gTolbiSpring_ObjectEventPixelX[];
extern const u8 gTolbiSpring_ObjectEventPixelZ[];
extern const u16 gTolbiSpring_ObjectEventHeadings[];

#endif
