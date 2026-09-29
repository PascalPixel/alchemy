#ifndef LINK_LOBBY_LOBBY_H
#define LINK_LOBBY_LOBBY_H

/* The link lobby (resource_3cb): the declarations its scene sources share. */

#include "TYPES.H"
#include "SCENE.H"
extern struct EventWork *gEventWork;

extern s16 gGameState[];
extern u8 gLinkLobbyEntrances[];
extern u8 gLinkLobbyExits[];
extern u8 gLinkLobbyPlacements[];
extern u8 gLinkLobbyBattlePlacements[];
extern u8 gLinkLobbyEvents[];

u8 *LinkLobby_GetEntrances(void);

s32 SceneData_ReturnZero(void);

u32 State_RunQueryWithInterruptMasterSaved(void);

u8 *LinkLobby_GetExits(void);

s32 LinkLobby_SelectPlacements(void);

s32 Scene_ClearFlagsAndPlayCue2927(void);

s32 SceneData_CopyUpToThreeEntries(u16 *dest);

s32 State_ApplyValueAndGetResult(s32 arg0);

void State_NoOp(void);

u8 *LinkLobby_GetEvents(void);

s32 LinkLobby_SaveBattleResults(void);

s32 LinkLobby_SaveMonsterBattleResults(void);

s32 LinkLobby_DrawThreeDigitValue(s32 value);

#endif
