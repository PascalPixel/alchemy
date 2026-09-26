#include "TYPES.H"

/* NONMATCHING: resource_3cb [02000860,02000b94), complete 820-byte owner.
 * Fresh unit score: 808 bytes, 155 differing halfwords, 144 aligned edits.
 * ROM handshake/exchange, not a dialogue switch. Flag-return, cancellation,
 * message-window, exchange-failure and success tails are all represented.
 * Reference pools split at +0x274 and +0x304; this candidate has one +0x2d4
 * pool. Shared result exit also moves the initial flag-check block; copy-loop
 * counter/destination registers and work initialization order still differ.
 * Three structural tests: direct named calls 363 halfwords/195 edits;
 * inline speed boundary + shared result exit 155/144 (retained); numeric
 * halfword stores 386/178, becoming immediates instead of splitting pools.
 * No allocation spelling sweep. Imported return-valued EventEnd/party-set
 * ABI spellings mirror adjacent drafts; the setters' bodies themselves are
 * void. Local_020007b0 is the still-unregistered exchange coordinator.
 * Engine bindings are in link-lobby-exchange-candidate, runtime +0x8000;
 * no in-file address aliases. This draft contributes no matching-C bytes.
 */
struct LinkLobbyWork {
    u8 unknown_000[0x182];
    s16 raised_trigger;
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

void Engine_EventBegin(void);
s32 Engine_EventEnd(void);
s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_TaskWait(s32 frames);
s32 Engine_TaskAddCallback(s32 (*callback)(void), s32 priority);
s32 Engine_TaskRemoveCallback(s32 (*callback)(void));
void *Engine_HeapAllocate(s32 slot, s32 size);
void Engine_HeapRelease(s32 slot);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 y);
void Engine_ActorWaitForMove(s32 actor);
s32 Engine_UiTextOpenMessageWindow(s32 message, s32 style, s32 mode, s32 wait);
void Engine_DebugFinalizeWindow(s32 window, s32 mode);
s32 Engine_TaskSetCallbackMask(s32 (*callback)(void), s32 mode);
void Engine_MapClearLayerEntryFlag(s32 layer);
void Engine_MapSetLayerEntryFlag(s32 layer);
void Engine_BattleFxSetWeightedResult(s32 first, s32 second);
s32 Engine_PartySetFields1ceAnd1d0(s32 first, s32 second);
void Engine_EventSetPair1d4(s32 first, s32 second);
s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
s32 LinkLobby_PollPeerReady(void);
s32 Local_020007b0(void);

extern struct LinkLobbyWork *gEventWork;
extern union GameStateRows gGameStateRows;
extern u16 LinkLobby_LocalValues[];
extern u8 LinkLobby_PartyBuffer[];
extern u8 Value_00000043[], Value_00000045[], Value_00000058[];
extern u8 Value_000000be[];

static __inline__ void LinkLobby_SetActorSpeed(s32 actor, s32 speed, s32 acceleration)
{
    Engine_ActorSetSpeed(actor, speed, acceleration);
}

s32 Scene_RunScene3cbSequenceB(void)
{
    struct LinkLobbyWork *work = gEventWork;
    s32 aborted = 0;
    s32 window = 0;
    s32 misses = 0;
    s32 cancel;
    s32 result;
    s32 (*callback)(void);
    u8 *buf;
    u8 *src;
    u8 *dst;
    u32 i;

    result = Engine_GameFlagIsSet(0x173);
    if (result) {
        Engine_EventBegin();
        goto exchange;
    }
    result = Engine_GameFlagIsSet(0x200);
    if (!result)
        goto done;
    result = Engine_GameFlagIsSet(0x205);
    if (result)
        goto done;
    Engine_EventBegin();
    Engine_GameFlagSet(0x203);
    LinkLobby_WriteSlotValue(2);
    if (!LinkLobby_PeerSlotMatches(2))
        window = Engine_UiTextOpenMessageWindow(0x2928, 5, 4, 1);
    while (!LinkLobby_PeerSlotMatches(2)) {
        Engine_TaskWait(1);
        cancel = 0;
        if (!Engine_GameFlagIsSet(0x201))
            cancel = 1;
        if (Engine_GameFlagIsSet(0x205))
            cancel = 1;
        if (!LinkLobby_PeerSlotMatches(2) && !LinkLobby_PeerSlotMatches(1)) {
            if (++misses > 25)
                cancel = 1;
        } else {
            misses = 0;
        }
        if (cancel) {
            work->raised_trigger = 2;
            Engine_GameFlagSet(0x205);
            Engine_GameFlagClear(0x201);
            Engine_GameFlagClear(0x202);
            LinkLobby_WriteSlotValue(4);
            aborted = 1;
            Engine_GameFlagClear(0x200);
            break;
        }
    }
    if (window)
        Engine_DebugFinalizeWindow(window, 1);
    Engine_TaskWait(5);
exchange:
    if (aborted)
        goto finish;
    buf = Engine_HeapAllocate(54, 1992);
    callback = LinkLobby_PollPeerReady;
    Engine_TaskRemoveCallback(callback);
    Engine_MapClearLayerEntryFlag(5);
    Engine_TaskWait(8);
    Engine_MapSetLayerEntryFlag(5);
    if (Engine_GameFlagIsSet(0x173)) {
        Engine_ActorFaceActor(8, gGameStateRows.words[125], 0);
        Engine_EventSetMessage(0x293b);
        Engine_EventOpenMessage(8, 0);
        Engine_TaskWait(45);
        LinkLobby_SetActorSpeed(0, 0x10000, 0x8000);
        Engine_ActorSetDestination(0, 216, 184);
        Engine_ActorWaitForMove(0);
        Engine_ActorSetDestination(0, 216, 168);
        Engine_ActorWaitForMove(0);
    } else {
        LinkLobby_SetActorSpeed(0, 0x10000, 0x8000);
        Engine_ActorSetDestination(0, 216, 200);
        Engine_ActorWaitForMove(0);
        LinkLobby_SetActorSpeed(0, 0x1999, 0xccc);
        Engine_ActorSetDestination(0, 216, 168);
        if (Local_020007b0() < 0) {
            LinkLobby_SetActorSpeed(0, 0x10000, 0x8000);
            Engine_ActorSetDestination(0, 216, 200);
            Engine_MapClearLayerEntryFlag(5);
            Engine_TaskWait(8);
            Engine_MapSetLayerEntryFlag(5);
            Engine_ActorWaitForMove(0);
            Engine_HeapRelease(54);
            LinkLobby_WriteSlotValue(0);
            LinkLobby_WriteSlotValue(4);
            Engine_TaskAddCallback(callback, 0xc80);
            Engine_TaskSetCallbackMask(callback, 1);
            Engine_GameFlagClear(0x201);
            Engine_GameFlagClear(0x202);
            Engine_GameFlagClear(0x303);
            Engine_GameFlagClear(0x203);
            Engine_GameFlagClear(0x200);
            work->raised_trigger = 2;
            goto finish;
        }
        LinkLobby_SetActorSpeed(0, 0x8000, 0x4000);
        Engine_ActorWaitForMove(0);
    }
    if (Engine_GameFlagIsSet(0x173)) {
        Engine_PartySetFields1ceAnd1d0((s32)Value_000000be, 8);
        Engine_EventSetPair1d4((s32)Value_000000be, 9);
    } else {
        Engine_PartySetFields1ceAnd1d0((s32)Value_000000be, 10);
        Engine_EventSetPair1d4((s32)Value_000000be, 11);
    }
    gGameStateRows.bytes[277][1] = 4;
    Engine_BattleFxSetWeightedResult(1, 1);
    LinkLobby_LocalValues[1] = (u16)(s32)Value_00000058;
    LinkLobby_LocalValues[0] = (u16)(s32)Value_00000045;
    LinkLobby_LocalValues[2] = (u16)(s32)Value_00000045;
    LinkLobby_LocalValues[3] = (u16)(s32)Value_00000043;
    src = buf;
    dst = LinkLobby_PartyBuffer;
    i = 0;
    do {
        *dst++ = *src++;
        ++i;
    } while (i <= 1991);
    Engine_HeapRelease(54);
finish:
    result = Engine_EventEnd();
done:
    return result;
}
