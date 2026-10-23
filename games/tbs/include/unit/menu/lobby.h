#ifndef ALCHEMY_UNIT_MENU_LOBBY_H
#define ALCHEMY_UNIT_MENU_LOBBY_H

#include "types.h"
#include "map_tables.h"

extern const s32 gLinkLobby_SlotValues[];
u8 *LinkLobby_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *LinkLobby_GetWarps(void);
s32 LinkLobby_SelectObjectEventTemplates(void);
extern u32 gLinkLobbyCallFrames;
u8 *LinkLobby_GetEvents(void);
s32 LinkLobby_RunRoundResult(void);
extern u8 gLinkLobby_SlotColumns[];
extern struct MapWarpDest gLinkLobbyWarpDests[];
extern u32 gLinkLobbyWarps[];
extern struct ObjectEventTemplate gLinkLobbyObjectEventTemplates[];
extern struct ObjectEventTemplate gLinkLobbyBattleObjectEventTemplates[];
extern struct MapEvent gLinkLobbyEvents[];

#endif
