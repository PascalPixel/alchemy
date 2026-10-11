#ifndef ALCHEMY_UNIT_MENU_LINK_LOBBY_LOBBY_H
#define ALCHEMY_UNIT_MENU_LINK_LOBBY_LOBBY_H

#include "types.h"

u8 *LinkLobby_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 LinkLobby_InitializeSerial(void);
u8 *LinkLobby_GetExits(void);
s32 LinkLobby_SelectPlacements(void);
s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
s32 LinkLobby_PollPeerReady(void);
s32 LinkLobby_CallIntoCircle(void);
s32 Scene_ClearFlagsAndPlayCue2927(void);
s32 LinkLobby_ReceivePartyRecords(void);
s32 SceneData_CopyUpToThreeEntries(u16 *dest);
s32 LinkLobby_SendPartyRecords(void);
s32 LinkLobby_ExchangePartyRecords(void);
s32 LinkLobby_RunConnectionSequence(void);
s32 LinkLobby_RunBattleApplication(void);
s32 LinkLobby_TalkLinkedWins(s32 actor);
s32 LinkLobby_RunChoicePrompt(s32 id);
s32 State_ApplyValueAndGetResult(s32 arg0);
s32 LinkLobby_PartyContains(s32 id);
void State_NoOp(void);
s32 LinkLobby_TalkByProgress(s32 actor);
s32 LinkLobby_StartExchange(void);
s32 LinkLobby_TalkToAttendant(s32 actor);
s32 LinkLobby_TalkAlternating(s32 actor);
s32 LinkLobby_ShowCountMessage(s32 id);
u8 *LinkLobby_GetEvents(void);
s32 LinkLobby_SaveBattleResults(void);
s32 LinkLobby_SaveMonsterBattleResults(void);
s32 LinkLobby_DrawThreeDigitValue(s32 value);
s32 LinkLobby_RunRoundResult(void);

#endif
