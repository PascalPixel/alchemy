#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "LOBBY.H"
#include "CALL.H"
#include "SCENE_IDS.H"
#include "SERIAL_RUNTIME.H"

extern u8 MsgLobbyChooseParameters[];
extern u8 MsgLobbyNoParameterAsk[];
extern u8 MsgLobbySettingsMadeRest[];
extern u8 MsgLobbyTease[];
extern u8 gKeyState[];
extern u8 gKeysRepeat[];
s32 Engine_DebugCreateWindow();
void RenderOutput_PrepareForRedraw();
void UiText_DrawNumber();
void UiWork_Finalize();
s32 DebugParty_LoadPreset();

s32 Party_CountActiveOwners(void);

extern u8 MsgLobbyTryingGetAway[];
extern u8 MsgLobbyCantWaitLets[];
extern u8 MsgLobbyGoingFightAlone[];
extern u8 MsgLobbyKnewLostBecause[];
extern u8 MsgLobbyNotFaultLost[];
extern u8 MsgLobbyWonWithoutWell[];
extern u8 MsgLobbyWonToldCountOn[];
s32 LinkLobby_PartyContains(s32 actor);
void SerialRuntime_RemoveIrqHandlers(void);
void Sound_LoadPresetParameters(s32 preset);
s32 Event_SetPairWork1c0(s32 scene, s32 entrance);
extern u8 MsgLobbyBattleArenaOld[];
extern u8 MsgLobbyChangeOrderParty[];
extern u8 MsgLobbyThreeAlliesFight[];
extern u8 MsgLobbyThreeAlliesFightLinked[];
extern u8 MsgLobbyThreeAlliesFightLinkedFinals[];
extern u8 MsgLobbyTravelingWarrior[];
extern u8 MsgLobbyCantWaitSee[];
extern u8 MsgLobbyWonNumberConsecutive[];
void UiText_DrawQuantity();
extern u8 MsgLobbySavingBattleResults[];
extern u8 MsgLobbyBattleResultsSaved[];
extern u8 MsgLobbySavingMonsterBattle[];
extern u8 MsgLobbyMonsterBattleResults[];
s32 UiText_OpenMessageWindow(s32 message, s32 x, s32 y, s32 flags);
s32 UiWork_IsComplete(void);
void SaveState_ProcessSelectedSlot(void);

extern u8 MsgLobbyNotBadNextMonster[];
extern u8 MsgLobbyNoteCantUse[];
extern u8 MsgLobbyWonNumberBattle[];
extern u8 MsgLobbyWonNumberBattleBroke[];
extern u8 MsgLobbyWonNumberBattleTold[];
extern u8 gOptionMirror[];
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
void LinkLobby_PollPeerReady(void);

/* Link lobby: ask for a choice in a small window, then answer with one of
 * three messages depending on the choice and its result. */
s32 LinkLobby_RunChoicePrompt(s32 id)
{
    s32 window;
    s32 choice;
    s32 shown;

    Engine_EventBegin();
    Engine_ActorFaceActor(id, gGameState.selected_actor, 0);
    Engine_EventSetMessage((s32)MsgLobbyChooseParameters);
    Engine_EventShowMessage(id, 0);
    window = Engine_DebugCreateWindow(0, 0, 6, 4, 2);
    choice = 0;
    shown = -1;
    for (;;) {
        if (choice != shown) {
            RenderOutput_PrepareForRedraw(window);
            UiText_DrawNumber(choice, 3, window, 0, 0);
            shown = choice;
        }
        if (*(volatile u32 *)gKeysRepeat & 32) {
            choice--;
        }
        if (*(volatile u32 *)gKeysRepeat & 16) {
            choice++;
        }
        if (choice < 0) {
            choice = 0;
        }
        if (*(volatile u32 *)gKeyState & 1) {
            break;
        }
        if (*(volatile u32 *)gKeyState & 2) {
            choice = -1;
            break;
        }
        Engine_TaskWait(1);
    }
    UiWork_Finalize(window, 1);
    {
        s32 ok;
        s32 message;

        if (choice >= 0) {
            ok = DebugParty_LoadPreset(choice);
        } else {
            message = (s32)MsgLobbyTease;
            goto show;
        }
        /* FAKEMATCH: the refusal jumps into the 0x98b branch so the two share
         * one message call, as the reference lays them out. */
        if (ok != 0) {
            message = (s32)MsgLobbyNoParameterAsk;
        show:
            Engine_EventSetMessage(message);
            Engine_EventShowMessage(9, 0);
        } else {
            Engine_EventSetMessage((s32)MsgLobbySettingsMadeRest);
            Engine_EventShowMessage(9, 0);
        }
    }
    Engine_TaskWait(10);
    Engine_EventEnd();
}

/* The link lobby: a value applied through the shop confirmation. */
s32 State_ApplyValueAndGetResult(s32 arg0)
{
    Engine_EventBegin();
    Engine_SanctumOpen(arg0);
    Engine_EventEnd();
}

s32 LinkLobby_PartyContains(s32 id)
{
    s32 count;
    s32 max;
    s32 i;

    count = Party_CountActiveOwners();
    max = 3;
    if (Engine_GameFlagIsSet(0x172) == 0) {
        max = 4;
    }
    if (count > max) {
        count = max;
    }
    for (i = 0; i < count; i++) {
        if (gGameState.active_owners[i] == 0xff) {
            return 0;
        }
        if (gGameState.active_owners[i] == id) {
            return 1;
        }
    }
    return 0;
}

/* The link lobby: the callback that does nothing. */
/* Deliberate no-op callback. */
void State_NoOp(void) {}

/* A lobby actor's line, one per actor from each base: with flag 0x304 set, by flag 0x305 and the actor's own flag 0x2f0 + actor; otherwise by what LinkLobby_PartyContains reports for actor 0 and for this actor. */
s32 LinkLobby_TalkByProgress(s32 actor)
{
    s32 base = (s32)MsgLobbyCantWaitLets;
    s32 lead = LinkLobby_PartyContains(0);
    s32 own = LinkLobby_PartyContains(actor);

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
    if (Engine_GameFlagIsSet(0x304)) {

        Value1(Engine_GameFlagIsSet, 0x2f0);
        base = Engine_GameFlagIsSet(actor + 0x2f0);
        if (Engine_GameFlagIsSet(0x305)) {
            if (base)
                base = (s32)MsgLobbyWonToldCountOn;
            else
                base = (s32)MsgLobbyWonWithoutWell;
        } else {
            if (base)
                base = (s32)MsgLobbyNotFaultLost;
            else
                base = (s32)MsgLobbyKnewLostBecause;
        }
    } else if (lead != 0) {
        if (own == 0)
            base = (s32)MsgLobbyGoingFightAlone;
    } else {
        base = (s32)MsgLobbyTryingGetAway;
    }
    Engine_EventSetMessage(base + actor - 1);
    Engine_EventShowMessage(actor, 0);
    Engine_EventEnd();
}

/* Leave the link lobby for the exchange: stop the serial interrupts, load
 * the sound preset and go to scene row 1's first entrance. */
s32 LinkLobby_StartExchange(void)
{
    SerialRuntime_RemoveIrqHandlers();
    Sound_LoadPresetParameters(2);
    /* FAKEMATCH: the do/while loads the scene number before the 1. */
    do {
        return Event_SetPairWork1c0((s32)&SceneId_Clear, 1);
    } while (0);
}

/* FAKEMATCH: Engine_EventEnd is declared returning s32 so each handler
 * returns the last callee's r0, as the battle application does, and
 * the game state is read through a word view for the selected actor. */

/* The three lobby regulars face the leader and speak a line that moves on
 * with the lobby's progress flags. */
s32 LinkLobby_TalkToAttendant(s32 actor)
{
    s32 message;
    s32 step = 0;

    Engine_EventBegin();
    switch (actor) {
    case 12:
        message = (s32)MsgLobbyTravelingWarrior;
        break;
    case 13:
        message = (s32)MsgLobbyThreeAlliesFight;
        break;
    case 14:
    default:
        message = (s32)MsgLobbyBattleArenaOld;
        break;
    }
    Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
    if (Engine_GameFlagIsSet(0x304))
        step = 2 - (Engine_GameFlagIsSet(0x305) != 0);
    Engine_EventSetMessage(message + step);
    Engine_EventOpenMessage(actor, 0);
    Engine_EventEnd();
}

/* The attendant alternates between explaining linked finals and the party
 * order. */
s32 LinkLobby_TalkAlternating(s32 actor)
{
    s32 message;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
    if (!Value1(Engine_GameFlagIsSet, 0x204)) {
        if (Party_CountActiveOwners() <= 3)
            message = (s32)MsgLobbyThreeAlliesFightLinkedFinals;
        else
            message = (s32)MsgLobbyThreeAlliesFightLinked;
        Engine_GameFlagSet(0x204);
    } else {
        message = (s32)MsgLobbyChangeOrderParty;
        Engine_GameFlagClear(0x204);
    }
    Engine_EventSetMessage(message);
    Engine_EventOpenMessage(actor, 0);
    Engine_EventEnd();
}

/* Link lobby: face the speaker and report the stored count, or the empty
 * message when there is none. */
s32 LinkLobby_ShowCountMessage(s32 id)
{
    u8 *gs;

    Engine_EventBegin();
    gs = (u8 *)&gGameState;
    Engine_ActorFaceActor(id, *(s32 *)(gs + 500), 0);
    if (*(u16 *)(gs + 680) != 0) {
        UiText_DrawQuantity(*(u16 *)(gs + 680), 5);
        Engine_EventSetMessage((s32)MsgLobbyWonNumberConsecutive);
    } else {
        Engine_EventSetMessage((s32)MsgLobbyCantWaitSee);
    }
    Engine_EventOpenMessage(id, 0);
    Engine_EventEnd();
}

/* The link lobby: its event table. */
u8 *LinkLobby_GetEvents(void) { return gLinkLobbyEvents; }

/* Saving the battle results between two message windows, and the
   three-digit counter drawn into the map. */
s32 LinkLobby_SaveBattleResults(void)
{
    s32 window;

    Engine_AudioPlayCue(85);
    window = UiText_OpenMessageWindow((s32)MsgLobbySavingBattleResults, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    SaveState_ProcessSelectedSlot();
    UiWork_Finalize(window, 1);
    Engine_TaskWait(1);
    window = UiText_OpenMessageWindow((s32)MsgLobbyBattleResultsSaved, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    UiWork_Finalize(window, 1);
}

s32 LinkLobby_SaveMonsterBattleResults(void)
{
    s32 window;

    Engine_AudioPlayCue(85);
    window = UiText_OpenMessageWindow((s32)MsgLobbySavingMonsterBattle, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    SaveState_ProcessSelectedSlot();
    UiWork_Finalize(window, 1);
    Engine_TaskWait(1);
    window = UiText_OpenMessageWindow((s32)MsgLobbyMonsterBattleResults, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    UiWork_Finalize(window, 1);
}

s32 LinkLobby_DrawThreeDigitValue(s32 value)
{
    s32 digit;

    if (value > 999)
        value = 999;
    for (digit = 0; digit <= 2; digit++) {
        Engine_MapCopyCellsTo(27, value % 10, 16 - digit, 8, 1, 1);
        value = value / 10;
    }
    Engine_MapRedraw();
}

/* Apply a link round's result: reset the lobby's frame counters, mark "TALK"
 * in the outgoing payload, record which party members stay, then by the
 * round's outcome update the win counts and records and reopen the lobby's
 * dialogue, and restart the peer poll. */
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
    LinkLobby_DrawThreeDigitValue(gGameState.link_tallies[6]);
    {
        s32 a = 13, b = 10;
        Engine_MapCopyCellAttributes(11, 11, 1, 1, a, b);
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
    if (gGameState.entrance == 8) {
        s32 wins;
        s32 msg;
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(5);
        {
            struct GameState *state = &gGameState;

            state->link_tallies[0]++;
            state->link_tallies[3]++;
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
        struct GameState *state = &gGameState;

        Engine_ActorFaceActor(8, state->selected_actor, 0);
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
            score = (u32)&state->link_tallies[3];
            UiText_DrawQuantity(*(u16 *)score, 5);
            score = *(u16 *)score;
            if (state->link_tallies[2] < score) {
                state->link_tallies[2] = score;
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
    } else if (gGameState.entrance == 9) {

        gGameState.link_tallies[1]++;
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(5);
        Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
        /* FAKEMATCH: one scalar holds the row address, then its score. */
        score = (u32)&gGameState.link_tallies[3];
        UiText_DrawQuantity(*(u16 *)score, 5);
        score = *(u16 *)score;
        if (gGameState.link_tallies[2] < score) {
            gGameState.link_tallies[2] = score;
            Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
            Engine_EventOpenMessage(8, 0);
            LinkLobby_SaveMonsterBattleResults();
        } else {
            Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleTold);
            Engine_EventOpenMessage(8, 0);
        }
        gGameState.link_tallies[3] = 0;
        Engine_GameFlagClear(0x173);
        GameFlag_SetByte(0x3f8, -1);
        LinkLobby_WriteSlotValue(0);
        Engine_EventEnd();
    } else if (gGameState.entrance == 10) {
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
            gGameState.link_tallies[4]++;
            v = gGameState.link_tallies[7] + 1;
            gGameState.link_tallies[7] = v;
            /* FAKEMATCH: reuse the other score branches' address slot. */
            score = (u32)&gGameState.link_tallies[6];
            if (*(u16 *)score < (u16)v) {
                *(u16 *)score = v;
            }
            LinkLobby_DrawThreeDigitValue(*(u16 *)score);
            LinkLobby_SaveBattleResults();
            Engine_GameFlagSet(0x304);
            Engine_GameFlagSet(0x305);
        }
        Engine_EventEnd();
    } else if (gGameState.entrance == 11) {
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        if ((v = Engine_GameFlagIsSet(0x173)) == 0) {
            gGameState.link_tallies[5]++;
            gGameState.link_tallies[7] = v;
            LinkLobby_SaveBattleResults();
        }
        Engine_GameFlagSet(0x304);
        Engine_GameFlagClear(0x305);
        Engine_EventEnd();
    } else {
        SerialRuntime_Initialize();
        Engine_GameFlagClear(0x172);
        GameFlag_SetByte(0x3f8, -1);
        if (gGameState.unknown_200[0x2a] != 0) {
            Engine_EventBegin();
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
            Engine_EventSetMessage((s32)MsgLobbyNoteCantUse);
            Engine_EventShowMessage(8, 0);
            Engine_EventEnd();
        }
        gGameState.unknown_200[0x2a] = 0;
        *(u8 *)gOptionMirror = 0;
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
    }
    callback = LinkLobby_PollPeerReady;
    Engine_TaskAddCallback(callback, 0xc80);
    Scheduler_SetCallbackMask(callback, 1);
    if (gGameState.entrance != 8 || !Engine_GameFlagIsSet(0x173)) {
        Owner_RefreshActiveRatios(1);
        BattlePlacement_UpdateTimedEntriesTwentyTimes();
    }
    return 0;
}
