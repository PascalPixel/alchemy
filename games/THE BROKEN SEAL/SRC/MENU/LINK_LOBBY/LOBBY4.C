#include "RUNTIME_MEM.H"
/* The party-record exchange with the other console: the parent sends
   first and the child receives first; the received count is kept in flag
   byte 0x3f0, and any failure resets the serial transfer. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "SERIAL_RUNTIME.H"
#include "SCENE_IDS.H"
#include "CALL.H"

s32 LinkLobby_SendPartyRecords(void);
s32 LinkLobby_ReceivePartyRecords(void);
void GameFlag_SetByte(s32 flag, s32 value);

extern u8 MsgLobbyAwaitingOpponent[];
extern u8 MsgLobbyGoodLuck[];
extern u8 gMapCellBuffer[];
s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
s32 LinkLobby_ExchangePartyRecords(void);
void LinkLobby_PollPeerReady(void);
s32 UiText_OpenMessageWindow(s32 message, s32 x, s32 y, s32 flags);
void UiWork_Finalize(s32 window, s32 flags);
void Map_ClearLayerEntryFlag(s32 layer);
void Map_SetLayerEntryFlag(s32 layer);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

struct PacketHalf {
    u16 value;
};

extern u8 MsgLobbyOpponentArrived[];
extern u8 MsgLobbyPleaseSpeakWhen[];
s32 LinkLobby_InitializeSerial(void);

s32 LinkLobby_ExchangePartyRecords(void)
{
    s32 result;
    s32 first;
    s32 flag;

    /* FAKEMATCH: separate initialization from the following flag query. */
    do {
        result = 0;
    } while (0);
    flag = Engine_GameFlagIsSet(0x302);
    gLinkExchangeState = result;
    if (!flag) {
        Engine_TaskWait(5);
        result = LinkLobby_SendPartyRecords();
        if (result < 0)
            goto fail;
        Engine_TaskWait(5);
        first = result = LinkLobby_ReceivePartyRecords();
        if (result < 0)
            goto check;
    } else {
        first = result = LinkLobby_ReceivePartyRecords();
        if (result < 0)
            goto fail;
        Engine_TaskWait(10);
        result = LinkLobby_SendPartyRecords();
        if (result < 0)
            goto fail;
    }
    GameFlag_SetByte(0x3f0, first);
    result = first;
check:
    if (first < 0) {
fail:
        SerialRuntime_ResetTransferState();
    }
    return result;
}

/* The link lobby's connection: wait for the other console to answer (or
   give up), walk the leader into the battle room, exchange the party
   records and copy what arrived into the map cell buffer's second half. */
s32 LinkLobby_RunConnectionSequence(void)
{
    /* FAKEMATCH: the aggregate keeps the shared packet value in halfword mode. */

    struct EventWork *work;
    s32 failed;
    s32 window;
    s32 cnt;
    s32 stop;
    u8 *buf;
    u8 *src;
    u8 *dst;
    u16 *tbl;
    struct PacketHalf value;
    u32 i;

    failed = 0;
    window = 0;
    /* FAKEMATCH: this initialization order resolves the scheduler's tied moves. */
    work = gEventWork;
    cnt = 0;
    if (Engine_GameFlagIsSet(0x173)) {
        Engine_EventBegin();
    } else {
        /* FAKEMATCH: these event callback exits have no defined result. */
        if (!Engine_GameFlagIsSet(0x200))
            return;
        if (Engine_GameFlagIsSet(0x205))
            return;
        Engine_EventBegin();
        Engine_GameFlagSet(0x203);
        LinkLobby_WriteSlotValue(2);
        if (!LinkLobby_PeerSlotMatches(2))
            window = UiText_OpenMessageWindow((s32)MsgLobbyAwaitingOpponent, 5, 4, 1);
        while (!LinkLobby_PeerSlotMatches(2)) {
            Engine_TaskWait(1);
            stop = 0;
            if (!Engine_GameFlagIsSet(0x201))
                stop = 1;
            if (Engine_GameFlagIsSet(0x205))
                stop = 1;
            if (!LinkLobby_PeerSlotMatches(2) && !LinkLobby_PeerSlotMatches(1)) {
                if (++cnt > 25)
                    stop = 1;
            } else {
                cnt = 0;
            }
            if (stop) {
                work->raised_trigger = 2;
                Engine_GameFlagSet(0x205);
                Engine_GameFlagClear(0x201);
                Engine_GameFlagClear(0x202);
                LinkLobby_WriteSlotValue(4);
                failed = 1;
                Engine_GameFlagClear(0x200);
                break;
            }
        }
        if (window)
            UiWork_Finalize(window, 1);
        Engine_TaskWait(5);
    }
    if (!failed) {
        buf = Runtime_AllocateBlock(54, 0x7c8);
        Engine_TaskRemoveCallback(LinkLobby_PollPeerReady);
        Map_ClearLayerEntryFlag(5);
        Engine_TaskWait(8);
        Map_SetLayerEntryFlag(5);
        if (Engine_GameFlagIsSet(0x173)) {
            Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
            Engine_EventSetMessage((s32)MsgLobbyGoodLuck);
            Engine_EventOpenMessage(8, 0);
            Engine_TaskWait(45);
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Engine_ActorWalkTo(0, 216, 184);
            Engine_ActorWaitForMove(0);
            Engine_ActorWalkTo(0, 216, 168);
            Engine_ActorWaitForMove(0);
        } else {
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Engine_ActorWalkTo(0, 216, 200);
            Engine_ActorWaitForMove(0);
            Actor_SetSpeed(0, 0x1999, 0xccc);
            Engine_ActorWalkTo(0, 216, 168);
            if (LinkLobby_ExchangePartyRecords() < 0) {
                Actor_SetSpeed(0, 0x10000, 0x8000);
                Engine_ActorWalkTo(0, 216, 200);
                Map_ClearLayerEntryFlag(5);
                Engine_TaskWait(8);
                Map_SetLayerEntryFlag(5);
                Engine_ActorWaitForMove(0);
                Runtime_ReleaseHeapBlock(54);
                LinkLobby_WriteSlotValue(0);
                LinkLobby_WriteSlotValue(4);
                Engine_TaskAddCallback(LinkLobby_PollPeerReady, 0xc80);
                Scheduler_SetCallbackMask((u32)(LinkLobby_PollPeerReady), 1);
                Engine_GameFlagClear(0x201);
                Engine_GameFlagClear(0x202);
                Engine_GameFlagClear(0x303);
                Engine_GameFlagClear(0x203);
                Engine_GameFlagClear(0x200);
                work->raised_trigger = 2;
                goto done;
            }
            Actor_SetSpeed(0, 0x8000, 0x4000);
            Engine_ActorWaitForMove(0);
        }
        if (Engine_GameFlagIsSet(0x173)) {
            Party_SetFields1ceAnd1d0((s32)&SceneId_LinkLobby, 8);
            Event_SetPair1d4((s32)&SceneId_LinkLobby, 9);
        } else {
            Party_SetFields1ceAnd1d0((s32)&SceneId_LinkLobby, 10);
            Event_SetPair1d4((s32)&SceneId_LinkLobby, 11);
        }
        gGameState.unknown_200[0x22b - 0x200] = 4;
        BattleFx_SetWeightedResult(1, 1);
        tbl = (u16 *)gSerialTransfer.payload;
        value.value = 0x45;
        tbl[1] = 0x58;
        tbl[0] = value.value;
        tbl[2] = value.value;
        tbl[3] = 0x43;
        i = 0;
        src = buf;
        dst = gMapCellBuffer + 0x8000;
        do {
            i++;
            *dst++ = *src++;
        } while (i <= 0x7c7);
        Runtime_ReleaseHeapBlock(54);
    }
done:
    /* FAKEMATCH: the scene returns whatever the event end leaves in r0. */
    return ((s32 (*)(void))Engine_EventEnd)();
}

/* Handle battle applications and reopen the lobby attendant's dialogue. */

s32 LinkLobby_RunBattleApplication(void)
{
    /* FAKEMATCH: The unused callback result keeps the last callee's r0 at return. */

    s32 answer;
    s32 msg;
    s32 line;

    msg = (s32)MsgLobbyPleaseSpeakWhen;
    Engine_EventBegin();
    Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
    if (LinkLobby_PeerSlotMatches(0) == 0) {
        Engine_TaskWait(1);
    }
    if (LinkLobby_PeerSlotMatches(0) == 0) {
        LinkLobby_WriteSlotValue(5);
        LinkLobby_InitializeSerial();
        if (!Value1(Engine_GameFlagIsSet, 0x173)) {
            Engine_EventSetMessage(msg + 5);
            Engine_EventOpenMessage(8, 0);
            answer = Engine_EventChooseYesNo(0, 0);
            if (answer == 0) {
                GameFlag_SetByte(1000, 0);
                Engine_GameFlagSet(0x173);
                Engine_GameFlagClear(0x172);
                Engine_GameFlagClear(0x16c);
                Engine_GameFlagSet(0x202);
                /* FAKEMATCH: Select the shared-tail message before its answer store. */
                line = msg + 7;
                gGameState.link_tallies[3] = answer;
                goto message;
            } else {
                Engine_GameFlagClear(0x173);
                Engine_GameFlagSet(0x16c);
                LinkLobby_WriteSlotValue(0);
                line = msg + 6;
                goto message;
            }
        } else {
            line = msg + 3;
            goto message;
        }
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x173)) {
            LinkLobby_WriteSlotValue(0);
            Engine_EventSetMessage((s32)MsgLobbyOpponentArrived);
            Engine_EventOpenMessage(8, 0);
            Engine_GameFlagClear(0x202);
            Engine_GameFlagClear(0x173);
        }
        if (Engine_GameFlagIsSet(0x202)) {
            line = msg + 3;
            /* FAKEMATCH: the first branch's answers jump into this block's
             * message tail, and an accepted challenge back into its opening,
             * which keeps the reference's single copy of each. */
message:
            Engine_EventSetMessage(line);
open_message:
            Engine_EventOpenMessage(8, 0);
            goto done;
        } else if (!Engine_GameFlagIsSet(0x201) && !Value1(Engine_GameFlagIsSet, 0x300)) {
            Engine_EventSetMessage(msg);
            Engine_EventOpenMessage(8, 0);
            Engine_GameFlagSet(0x300);
        } else {
            Engine_GameFlagSet(0x300);
            if (Engine_GameFlagIsSet(0x201)) {
                Engine_EventSetMessage(msg + 2);
            } else {
                Engine_EventSetMessage(msg + 1);
            }
            Engine_EventOpenMessage(8, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                if (LinkLobby_PeerSlotMatches(0)) {
                    Engine_GameFlagSet(0x16c);
                    Engine_GameFlagSet(0x172);
                    if (Engine_GameFlagIsSet(0x201)) {
                        Engine_EventSetMessage(msg + 3);
                    } else {
                        Engine_EventSetMessage(msg + 4);
                    }
                    LinkLobby_WriteSlotValue(1);
                    Engine_GameFlagSet(0x202);
                    goto open_message;
                } else {
                    Engine_GameFlagSet(0x205);
                }
            } else {
                Engine_EventSetMessage(msg);
                Engine_EventOpenMessage(8, 0);
            }
        }
    }
done:
    Engine_EventEnd();
}
