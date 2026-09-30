/* The link lobby: the callback that does nothing. */
#include "LOBBY.H"
#include "TYPES.H"
#include "CALL.H"
#include "SCENE_IDS.H"

extern u8 MsgLobbyTryingGetAway[];
extern u8 MsgLobbyCantWaitLets[];
extern u8 MsgLobbyGoingFightAlone[];
extern u8 MsgLobbyKnewLostBecause[];
extern u8 MsgLobbyNotFaultLost[];
extern u8 MsgLobbyWonWithoutWell[];
extern u8 MsgLobbyWonToldCountOn[];

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

s32 LinkLobby_PartyContains(s32 actor);
void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 actor, s32 mode);
s32 Engine_EventEnd(void);

void SerialRuntime_RemoveIrqHandlers(void);
void Sound_LoadPresetParameters(s32 preset);
s32 Event_SetPairWork1c0(s32 scene, s32 entrance);

extern u8 MsgLobbyBattleArenaOld[];
extern u8 MsgLobbyChangeOrderParty[];
extern u8 MsgLobbyThreeAlliesFight[];
extern u8 MsgLobbyThreeAlliesFightLinked[];
extern u8 MsgLobbyThreeAlliesFightLinkedFinals[];
extern u8 MsgLobbyTravelingWarrior[];
void Engine_ActorFaceActor(s32 actor, s32 target, s32 mode);
void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
s32 Party_CountActiveOwners(void);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);

extern u8 MsgLobbyCantWaitSee[];
extern u8 MsgLobbyWonNumberConsecutive[];
void Engine_EventBegin();
void Engine_ActorFaceActor();
void UiText_DrawQuantity();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
s32 Engine_EventEnd();

extern u8 MsgLobbySavingBattleResults[];
extern u8 MsgLobbyBattleResultsSaved[];
extern u8 MsgLobbySavingMonsterBattle[];
extern u8 MsgLobbyMonsterBattleResults[];
void Engine_AudioPlayCue(s32 cue);
s32 UiText_OpenMessageWindow(s32 message, s32 x, s32 y, s32 flags);
s32 UiWork_IsComplete(void);
s32 UiWork_Finalize(s32 window, s32 flags);
void Engine_TaskWait(s32 frames);
void SaveState_ProcessSelectedSlot(void);
s32 Engine_MathDivide(s32 value, s32 divisor);
s32 Engine_MathRemainder(s32 value, s32 divisor);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                           s32 height);
s32 Engine_MapRedraw(void);

/* Deliberate no-op callback. */
void State_NoOp(void) {}

/* A lobby actor's line, one per actor from each base: with flag 0x304 set, by flag 0x305 and the actor's own flag 0x2f0 + actor; otherwise by what LinkLobby_PartyContains reports for actor 0 and for this actor. */
s32 LinkLobby_TalkByProgress(s32 actor)
{
    s32 base = (s32)MsgLobbyCantWaitLets;
    s32 lead = LinkLobby_PartyContains(0);
    s32 own = LinkLobby_PartyContains(actor);

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, (*(union GameStateRows *)gGameState).words[125], 0);
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
    return Engine_EventEnd();
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
    Engine_ActorFaceActor(actor, (*(union GameStateRows *)gGameState).words[125], 0);
    if (Engine_GameFlagIsSet(0x304))
        step = 2 - (Engine_GameFlagIsSet(0x305) != 0);
    Engine_EventSetMessage(message + step);
    Engine_EventOpenMessage(actor, 0);
    return Engine_EventEnd();
}

/* The attendant alternates between explaining linked finals and the party
 * order. */
s32 LinkLobby_TalkAlternating(s32 actor)
{
    s32 message;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, (*(union GameStateRows *)gGameState).words[125], 0);
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
    return Engine_EventEnd();
}

/* Link lobby: face the speaker and report the stored count, or the empty
 * message when there is none. */
s32 LinkLobby_ShowCountMessage(s32 id)
{
    u8 *gs;

    Engine_EventBegin();
    gs = ((u8 *)gGameState);
    Engine_ActorFaceActor(id, *(s32 *)(gs + 500), 0);
    if (*(u16 *)(gs + 680) != 0) {
        UiText_DrawQuantity(*(u16 *)(gs + 680), 5);
        Engine_EventSetMessage((s32)MsgLobbyWonNumberConsecutive);
    } else {
        Engine_EventSetMessage((s32)MsgLobbyCantWaitSee);
    }
    Engine_EventOpenMessage(id, 0);
    return Engine_EventEnd();
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
    /* FAKEMATCH: closing the first window through a void call sets the
     * window argument before the flags, as the game does. */
    ((void (*)(s32, s32))UiWork_Finalize)(window, 1);
    Engine_TaskWait(1);
    window = UiText_OpenMessageWindow((s32)MsgLobbyBattleResultsSaved, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    return UiWork_Finalize(window, 1);
}

s32 LinkLobby_SaveMonsterBattleResults(void)
{
    s32 window;

    Engine_AudioPlayCue(85);
    window = UiText_OpenMessageWindow((s32)MsgLobbySavingMonsterBattle, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    SaveState_ProcessSelectedSlot();
    /* FAKEMATCH: as above. */
    ((void (*)(s32, s32))UiWork_Finalize)(window, 1);
    Engine_TaskWait(1);
    window = UiText_OpenMessageWindow((s32)MsgLobbyMonsterBattleResults, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    return UiWork_Finalize(window, 1);
}

s32 LinkLobby_DrawThreeDigitValue(s32 value)
{
    s32 digit;

    if (value > 999)
        value = 999;
    for (digit = 0; digit <= 2; digit++) {
        Engine_MapCopyCellsTo(27, Engine_MathRemainder(value, 10), 16 - digit, 8, 1, 1);
        value = Engine_MathDivide(value, 10);
    }
    return Engine_MapRedraw();
}
