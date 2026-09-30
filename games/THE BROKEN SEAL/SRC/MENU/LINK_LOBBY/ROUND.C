/* Apply a link round's result: reset the lobby's frame counters, mark "TALK"
 * in the outgoing payload, record which party members stay, then by the
 * round's outcome update the win counts and records and reopen the lobby's
 * dialogue, and restart the peer poll. */
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "CALL.H"
extern u8 MsgLobbyNotBadNextMonster[];
extern u8 MsgLobbyNoteCantUse[];
extern u8 MsgLobbyWonNumberBattle[];
extern u8 MsgLobbyWonNumberBattleBroke[];
extern u8 MsgLobbyWonNumberBattleTold[];
extern u8 gOptionMirror[];

struct EventWork {
    u8 unknown_000[0x182];
    s16 raised_trigger;
    u8 unknown_184[0x3c];
    s32 start_transition;
};

void Engine_TaskWait(s32 frames);
void Engine_TaskAddCallback(void (*callback)(void), s32 priority);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                  s32 dest_y);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_EventBegin(void);
void Engine_EventOpenScreen(void);
void Engine_EventWaitForScreen(void);
s32 Engine_EventEnd(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
void Engine_EventShowMessage(s32 actor, s32 mode);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
extern struct EventWork *gEventWork;
extern u16 gGameState[][1];

u32 State_RunQueryWithInterruptMasterSaved(void);
s32 LinkLobby_DrawThreeDigitValue(s32 value);
s32 LinkLobby_SaveBattleResults(void);
s32 LinkLobby_SaveMonsterBattleResults(void);
void Sound_LoadPresetParameters(s32 value);
void LinkLobby_WriteSlotValue(s32 mode);
void Map_SetLayerEntryFlag(s32 value);
s32 LinkLobby_PartyContains(s32 index);
s32 GameFlag_GetByte(s32 counter);
void GameFlag_SetByte(s32 counter, s32 value);
void UiText_DrawQuantity(s32 value, s32 digits);
void SerialRuntime_Initialize(void);
void Scheduler_SetCallbackMask(void (*callback)(void), s32 value);
void Owner_RefreshActiveRatios(s32 value);
void BattlePlacement_UpdateTimedEntriesTwentyTimes(void);

extern u32 gLinkLobbyCallFrames;
extern s32 gLinkLobbyWaitFrames;

union GameStateRows {
    u8 bytes[512][2];
    u16 halves[512][1];
    s32 words[256];
};

#define ROW(n) gGameState[n][0]

void LinkLobby_PollPeerReady(void);

s32 LinkLobby_RunRoundResult(void)
{
    s32 i;
    u32 score;
    struct EventWork *ew;
    void (*callback)(void);

    gLinkLobbyCallFrames = 0;
    gLinkLobbyWaitFrames = 0;
    gEventWork->start_transition = 0x201;
    Sound_LoadPresetParameters(2);
    LinkLobby_DrawThreeDigitValue(ROW(344));
    {
        s32 a = 13, b = 10;
        ((void (*)())Engine_MapCopyCellAttributes)(11, 11, 1, 1, a, b);
    }
    LinkLobby_WriteSlotValue(4);
    Engine_TaskWait(1);
    Map_SetLayerEntryFlag(5);
    /* Through a pointer the halfword letters load from the literal pool. */
    {
        u16 *name = (u16 *)gSerialTransfer.reserved;

        name[4] = 'T';
        name[5] = 'A';
        name[6] = 'L';
        name[7] = 'K';
    }
    for (i = 0; i < 8; i++) {
        Engine_GameFlagClear(0x2f0 + i);
        if (LinkLobby_PartyContains(i)) {
            Engine_GameFlagSet(0x2f0 + i);
        }
    }
    if ((s16)ROW(225) == 8) {
        s32 wins;
        s32 msg;
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(5);
        {
            union GameStateRows *state = (union GameStateRows *)gGameState;

            state->halves[338][0]++;
            state->halves[341][0]++;
        }
        wins = (s8)GameFlag_GetByte(0x3f8);
        msg = wins * 2 + 2;
        if (msg > 14) {
            msg = 14;
        }
        v = Value1(GameFlag_GetByte, 1000);
        if (v == 2) {
            GameFlag_SetByte(1000, 0);
            wins++;
            msg++;
        } else {
            Call2(GameFlag_SetByte, 1000, v + 1);
        }
        {
        union GameStateRows *state = (union GameStateRows *)gGameState;

        Engine_ActorFaceActor(8, state->words[125], 0);
        Engine_EventSetMessage((s32)MsgLobbyNotBadNextMonster + msg);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            if (wins > 90) {
                wins = 90;
            }
            GameFlag_SetByte(0x3f8, wins);
        } else {

            Engine_GameFlagClear(0x173);
            GameFlag_SetByte(0x3f8, -1);
            /* FAKEMATCH: one scalar holds the row address, then its score. */
            score = (u32)state->halves[341];
            UiText_DrawQuantity(*(u16 *)score, 5);
            score = *(u16 *)score;
            if (state->halves[340][0] < score) {
                state->halves[340][0] = score;
                Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
                Engine_EventOpenMessage(8, 0);
                LinkLobby_SaveMonsterBattleResults();
            } else {
                Engine_EventSetMessage((s32)MsgLobbyWonNumberBattle);
                Engine_EventOpenMessage(8, 0);
            }
            LinkLobby_WriteSlotValue(0);
        }
        }
        Engine_EventEnd();
    } else if ((s16)ROW(225) == 9) {

        ROW(339)++;
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(5);
        Engine_ActorFaceActor(8, *(s32 *)gGameState[250], 0);
        /* FAKEMATCH: one scalar holds the row address, then its score. */
        score = (u32)&ROW(341);
        UiText_DrawQuantity(*(u16 *)score, 5);
        score = *(u16 *)score;
        if (ROW(340) < score) {
            ROW(340) = score;
            Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
            Engine_EventOpenMessage(8, 0);
            LinkLobby_SaveMonsterBattleResults();
        } else {
            Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleTold);
            Engine_EventOpenMessage(8, 0);
        }
        ROW(341) = 0;
        Engine_GameFlagClear(0x173);
        GameFlag_SetByte(0x3f8, -1);
        LinkLobby_WriteSlotValue(0);
        Engine_EventEnd();
    } else if ((s16)ROW(225) == 10) {
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        if (Value1(Engine_GameFlagIsSet, 1000)) {
            ew = gEventWork;
            Engine_GameFlagClear(1000);
            ew->raised_trigger = 2;
            Engine_GameFlagClear(0x304);
            Engine_TaskWait(20);
            State_RunQueryWithInterruptMasterSaved();
            LinkLobby_WriteSlotValue(0);
            LinkLobby_WriteSlotValue(4);
        } else {
            ROW(342)++;
            v = ROW(345) + 1;
            ROW(345) = v;
            /* FAKEMATCH: reuse the other score branches' address slot. */
            score = (u32)&ROW(344);
            if (*(u16 *)score < (u16)v) {
                *(u16 *)score = v;
            }
            LinkLobby_DrawThreeDigitValue(*(u16 *)score);
            LinkLobby_SaveBattleResults();
            Engine_GameFlagSet(0x304);
            Engine_GameFlagSet(0x305);
        }
        Engine_EventEnd();
    } else if ((s16)ROW(225) == 11) {
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        if ((v = Engine_GameFlagIsSet(0x173)) == 0) {
            ROW(343)++;
            ROW(345) = v;
            LinkLobby_SaveBattleResults();
        }
        Engine_GameFlagSet(0x304);
        Engine_GameFlagClear(0x305);
        Engine_EventEnd();
    } else {
        SerialRuntime_Initialize();
        Engine_GameFlagClear(0x172);
        GameFlag_SetByte(0x3f8, -1);
        if (*(u8 *)gGameState[277] != 0) {
            Engine_EventBegin();
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_ActorFaceActor(8, *(s32 *)gGameState[250], 0);
            Engine_EventSetMessage((s32)MsgLobbyNoteCantUse);
            Engine_EventShowMessage(8, 0);
            Engine_EventEnd();
        }
        *(u8 *)gGameState[277] = 0;
        *(u8 *)gOptionMirror = 0;
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
    }
    callback = LinkLobby_PollPeerReady;
    Engine_TaskAddCallback(callback, 0xc80);
    Scheduler_SetCallbackMask(callback, 1);
    if ((s16)ROW(225) != 8 || !Engine_GameFlagIsSet(0x173)) {
        Owner_RefreshActiveRatios(1);
        BattlePlacement_UpdateTimedEntriesTwentyTimes();
    }
    return 0;
}
