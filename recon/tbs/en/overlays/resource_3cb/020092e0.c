/* Draft of LinkLobby_RunRoundResult, resource_3cb at 0x020092e0 (was
 * MENU/LINK_LOBBY/ROUND.C).
 * Remaining difference: the ROM loads 0x4b and 0x4c from its literal pool as
 * link-time values, and it reads words just past the loaded image.
 * The listing keeps these rows. */
/* Apply the link round's result, update records, and reopen lobby dialogue.
 * Reconstructed from this owner's complete own-ROM listing and registered
 * draft; exact 1060-byte extent, including literal pools (2026-09-26). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgLobbyNotBadNextMonster[];
extern u8 MsgLobbyNoteCantUse[];
extern u8 MsgLobbyWonNumberBattle[];
extern u8 MsgLobbyWonNumberBattleBroke[];
extern u8 MsgLobbyWonNumberBattleTold[];
extern u8 gOptionMirror[];

void Sound_LoadPresetParameters(s32 value);
void Scene_DrawThreeDigitValue(s32 value);
void LinkLobby_WriteSlotValue(s32 mode);
void Map_SetLayerEntryFlag(s32 value);
s32 LinkLobby_PartyContains(s32 index);
s32 GameFlag_GetByte(s32 counter);
void Engine_GameFlagWriteValue(s32 counter, s32 value);
void UiText_DrawQuantity(s32 value, s32 digits);
void Scene_ShowDialoguePair292c(void);
void Scene_ShowDialoguePair292a(void);
void State_RunQueryWithInterruptMasterSaved(void);
void SerialRuntime_Initialize(void);
void Scheduler_SetCallbackMask(void (*callback)(void), s32 value);
void Owner_RefreshActiveRatios(s32 value);
void BattlePlacement_UpdateTimedEntriesTwentyTimes(void);

extern u16 gGameState[][1];
extern u16 Data_02002224[];
extern s32 Data_02009f50;
extern s32 Data_02009f4c;

union GameStateRows {
    u8 bytes[512][2];
    u16 halves[512][1];
    s32 words[256];
};

#define ROW(n) gGameState[n][0]

static __inline__ s32 Value1(s32 (*fn)(), s32 value)
{
    return fn(value);
}

static __inline__ void Call1(void (*fn)(), s32 value)
{
    fn(value);
}

static __inline__ void Call2(void (*fn)(), s32 left, s32 right)
{
    fn(left, right);
}

static __inline__ void Name_StoreLetter(s32 letter, u16 *dst, s32 index)
{
    dst[index] = letter;
}

s32 LinkLobby_RunRoundResult(void)
{
    s32 i;
    u32 score;
    struct EventWork *ew;
    void (*callback)(void);

    Data_02009f50 = 0;
    Data_02009f4c = 0;
    gEventWork->start_transition = 0x201;
    Sound_LoadPresetParameters(2);
    Scene_DrawThreeDigitValue(ROW(344));
    {
        s32 a = 13, b = 10;
        ((void (*)())Engine_MapCopyCellAttributes)(11, 11, 1, 1, a, b);
    }
    LinkLobby_WriteSlotValue(4);
    Engine_TaskWait(1);
    Map_SetLayerEntryFlag(5);
    /* FAKEMATCH: word-sized locals retain halfword-mode link constants,
     * giving the short literal-pool reach used by the original name stores. */
    Name_StoreLetter((u16)0x54, Data_02002224, 4);
    Name_StoreLetter((u16)0x41, Data_02002224, 5);
    Name_StoreLetter((u16)0x4c, Data_02002224, 6);
    Name_StoreLetter((u16)0x4b, Data_02002224, 7);
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
        wins = (s8)Value1(GameFlag_GetByte, 0x3f8);
        msg = wins * 2 + 2;
        if (msg > 14) {
            msg = 14;
        }
        v = Value1(GameFlag_GetByte, 1000);
        if (v == 2) {
            Call2(Engine_GameFlagWriteValue, 1000, 0);
            wins++;
            msg++;
        } else {
            Call2(Engine_GameFlagWriteValue, 1000, v + 1);
        }
        {
        union GameStateRows *state = (union GameStateRows *)gGameState;

        Engine_ActorFaceActor(8, state->words[125], 0);
        Call1(Engine_EventSetMessage, (s32)MsgLobbyNotBadNextMonster + msg);
        Engine_EventOpenMessage(8, 0);
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
            if (wins > 90) {
                wins = 90;
            }
            Call2(Engine_GameFlagWriteValue, 0x3f8, wins);
        } else {

            Engine_GameFlagClear(0x173);
            Call2(Engine_GameFlagWriteValue, 0x3f8, -1);
            /* FAKEMATCH: one scalar holds the row address, then its score. */
            score = (u32)state->halves[341];
            Call2(UiText_DrawQuantity, *(u16 *)score, 5);
            score = *(u16 *)score;
            if (state->halves[340][0] < score) {
                state->halves[340][0] = score;
                Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
                Engine_EventOpenMessage(8, 0);
                Scene_ShowDialoguePair292c();
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
        Call2(UiText_DrawQuantity, *(u16 *)score, 5);
        score = *(u16 *)score;
        if (ROW(340) < score) {
            ROW(340) = score;
            Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
            Engine_EventOpenMessage(8, 0);
            Scene_ShowDialoguePair292c();
        } else {
            Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleTold);
            Engine_EventOpenMessage(8, 0);
        }
        ROW(341) = 0;
        Engine_GameFlagClear(0x173);
        Call2(Engine_GameFlagWriteValue, 0x3f8, -1);
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
            Call1(Engine_GameFlagClear, 1000);
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
            Scene_DrawThreeDigitValue(*(u16 *)score);
            Scene_ShowDialoguePair292a();
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
            Scene_ShowDialoguePair292a();
        }
        Engine_GameFlagSet(0x304);
        Engine_GameFlagClear(0x305);
        Engine_EventEnd();
    } else {
        SerialRuntime_Initialize();
        Engine_GameFlagClear(0x172);
        Call2(Engine_GameFlagWriteValue, 0x3f8, -1);
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
    callback = (void (*)(void))0x2008149;
    Engine_TaskAddCallback(callback, 0xc80);
    Scheduler_SetCallbackMask(callback, 1);
    if ((s16)ROW(225) != 8 || !Engine_GameFlagIsSet(0x173)) {
        Owner_RefreshActiveRatios(1);
        BattlePlacement_UpdateTimedEntriesTwentyTimes();
    }
    return 0;
}
