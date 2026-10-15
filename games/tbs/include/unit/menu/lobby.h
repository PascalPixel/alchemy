#ifndef ALCHEMY_UNIT_MENU_LOBBY_H
#define ALCHEMY_UNIT_MENU_LOBBY_H

#include "types.h"
#include "field_scene.h"

extern const s32 gLinkLobby_SlotValues[];
u8 *LinkLobby_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *LinkLobby_GetExits(void);
s32 LinkLobby_SelectPlacements(void);
extern u32 gLinkLobbyCallFrames;
u8 *LinkLobby_GetEvents(void);
s32 LinkLobby_RunRoundResult(void);
extern u8 gLinkLobby_SlotColumns[];
extern struct SceneEntrance gLinkLobbyEntrances[];
extern u32 gLinkLobbyExits[];
extern struct ScenePlacement gLinkLobbyPlacements[];
extern struct ScenePlacement gLinkLobbyBattlePlacements[];
extern struct SceneEvent gLinkLobbyEvents[];

#endif
