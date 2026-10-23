#ifndef ALCHEMY_UNIT_MENU_LOBBY_H
#define ALCHEMY_UNIT_MENU_LOBBY_H

#include "types.h"
#include "map_tables.h"

extern const s32 gBattleArena_SlotValues[];
u8 *BattleArena_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *BattleArena_GetWarps(void);
s32 BattleArena_SelectObjectEventTemplates(void);
extern u32 gBattleArenaCallFrames;
u8 *BattleArena_GetEvents(void);
s32 BattleArena_RunRoundResult(void);
extern u8 gBattleArena_SlotColumns[];
extern struct MapWarpDest gBattleArenaWarpDests[];
extern u32 gBattleArenaWarps[];
extern struct ObjectEventTemplate gBattleArenaObjectEventTemplates[];
extern struct ObjectEventTemplate gBattleArenaBattleObjectEventTemplates[];
extern struct MapEvent gBattleArenaEvents[];

#endif
