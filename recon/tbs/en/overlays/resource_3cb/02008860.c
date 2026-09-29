/* Draft of LinkLobby_RunConnectionSequence, resource_3cb at 0x02008860 (was
 * MENU/LINK_LOBBY/CONNECT.C).
 * Remaining difference: the ROM loads 0x43, 0x45, 0x58 and 0xbe from its
 * literal pool as link-time values.
 * The listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SYSTEM.H"

/* Run the link-lobby connection sequence and copy the received party data.
 * Reconstructed from the complete own-ROM 820-byte owner, including pools. */

s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
s32 LinkLobby_ExchangePartyRecords(void);
s32 UiText_OpenMessageWindow(s32 message, s32 x, s32 y, s32 mode);
void UiWork_Finalize(s32 window, s32 mode);
void Scheduler_SetCallbackMask(void *buf, s32 mode);
void Map_ClearLayerEntryFlag(s32 mode);
void Map_SetLayerEntryFlag(s32 mode);
s32 Party_SetFields1ceAnd1d0(s32 value, s32 index);
void Event_SetPair1d4(s32 value, s32 index);
void BattleFx_SetWeightedResult(s32 first, s32 second);

extern u8 LinkLobby_PollPeerReady[];
extern u8 Data_02018000[];
extern s32 Data_02002224[];

/* FAKEMATCH: the aggregate keeps the shared packet value in halfword mode. */
struct PacketHalf {
    u16 value;
};

s32 LinkLobby_RunConnectionSequence(void)
{
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
            window = UiText_OpenMessageWindow(0x2928, 5, 4, 1);
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
        buf = Engine_HeapAllocate(54, 0x7c8);
        Engine_TaskRemoveCallback(LinkLobby_PollPeerReady);
        Map_ClearLayerEntryFlag(5);
        Engine_TaskWait(8);
        Map_SetLayerEntryFlag(5);
        if (Engine_GameFlagIsSet(0x173)) {
            Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
            Engine_EventSetMessage(0x293b);
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
                Engine_HeapRelease(54);
                LinkLobby_WriteSlotValue(0);
                LinkLobby_WriteSlotValue(4);
                Engine_TaskAddCallback(LinkLobby_PollPeerReady, 0xc80);
                Scheduler_SetCallbackMask(LinkLobby_PollPeerReady, 1);
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
            Party_SetFields1ceAnd1d0(0xbe, 8);
            Event_SetPair1d4(0xbe, 9);
        } else {
            Party_SetFields1ceAnd1d0(0xbe, 10);
            Event_SetPair1d4(0xbe, 11);
        }
        gGameState.unknown_1f8[0x33] = 4;
        BattleFx_SetWeightedResult(1, 1);
        tbl = (u16 *)Data_02002224;
        value.value = 0x45;
        tbl[1] = 0x58;
        tbl[0] = value.value;
        tbl[2] = value.value;
        tbl[3] = 0x43;
        i = 0;
        src = buf;
        dst = Data_02018000;
        do {
            i++;
            *dst++ = *src++;
        } while (i <= 0x7c7);
        Engine_HeapRelease(54);
    }
done:
    return Engine_EventEnd();
}
