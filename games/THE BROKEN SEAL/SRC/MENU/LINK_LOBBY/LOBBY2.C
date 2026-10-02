/* The link lobby: the attendant's call into the circle. */
#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "FIELD_EVENT.H"
#include "LOBBY.H"
#include "SERIAL_RUNTIME.H"
#include "CALL.H"
#include "TBS_EDITION.H"

extern u8 MsgLobbyWantParticipatePlease[];

/* Frames since the attendant last called out; it follows the overlay's
 * image, after the peers' wait. */
u32 gLinkLobbyCallFrames;

extern u8 MsgLobbyLinkDisconnected[];

extern u8 MsgEnemyLabel[];
extern u8 gLinkStatus[];
void Runtime_BumpFree(s32 heap);
s32 SerialRuntime_BeginTransferB(void);
s32 SerialRuntime_GetActiveTransfers(void);
u8 *Owner_GetState(s32 owner);
void Ui_AdjustValueWithoutLimit(s32 id, u16 *buf);
void Trade_GetOfferState(s32 mode);

/* Unless flag 0x203 is set, has the attendant call "please step into the
 * circle!" (MsgLobbyWantParticipatePlease) once every 300 frames: flag 0x200 marks a call
 * already made in the current period. */
s32 LinkLobby_CallIntoCircle(void)
{
    s32 set;

    set = Engine_GameFlagIsSet(0x203);
    if (set != 0) {
        return set;
    }
    if (++gLinkLobbyCallFrames == 300) {
        gLinkLobbyCallFrames = 0;
        Engine_GameFlagClear(0x200);
    }
    set = Value1(Engine_GameFlagIsSet, 0x200);
    if (set != 0) {
        return set;
    }
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgLobbyWantParticipatePlease);
    Engine_EventOpenMessage(8, 0);
    Engine_TaskWait(5);
    Engine_GameFlagSet(0x200);
    Engine_EventEnd();
}

/*
 * The link lobby's scene teardown, which clears the lobby flags: reset one
 * workspace field, clear three flags, play cue
 * 0x2927 and return the last call's result -- the epilogue pops the return
 * address into r1, so r0 survives and is the result. The 88-byte owner
 * includes its alignment bytes and four pool words, one of which is
 * 0x03001ebc -- the IWRAM workspace-pointer cell, not an in-image address.
 */
s32 Scene_ClearFlagsAndPlayCue2927(void)
{
    u16 *work = *(u16 **)&gEventWork;

    LinkLobby_WriteSlotValue(4);
    Engine_GameFlagClear(512);
    Engine_GameFlagClear(0x203);
    Engine_EventBegin();

    {
        /*
         * The halfword store goes through a pointer local and then a value
         * local, in that order. Storing the literal straight into the
         * halfword builds the constant in HImode and fetches it from the
         * literal pool, costing a pool word the reference does not have;
         * splitting the address out first also fixes which register holds
         * the address.
         */
        u16 *p = (u16 *)((u32)work + 386);
        s32 val = 0;
        *p = (u16)val;
    }

    Engine_EventSetMessage((s32)MsgLobbyLinkDisconnected);
    Engine_EventOpenMessage(8, 0);
    Engine_GameFlagClear(0x205);
    Engine_EventEnd();
}

/* Receives the linked player's three party records (0x154-byte transfers)
 * and then one 0x140-byte block, waiting on each transfer for at most 900
 * frames in total and 24 unready polls. Each party name gets the peer's
 * prefix (up to five characters) shifted in. Returns how many received
 * records have the byte at 0x12a set, or -1 when the link fails. */
s32 LinkLobby_ReceivePartyRecords(void)
{
    u16 buf[24];
    u32 size;
    s32 heap;
    s32 result;
    s32 slot;
    s32 timeout;
    s32 tries;
    u8 *rec;
    s32 ret;
    s32 n;
    s32 i;

    size = 0x154;
    heap = (s32)Runtime_BumpAllocateAlternatePool((s32)size);
    result = 0;
    timeout = 900;
    slot = 0;
    goto next;
wait1:
    if (SERIAL_VALUE_B > size) {
        result = -1;
        goto done;
    }
    Engine_TaskWait(1);
    if (--timeout < 0 || (*(volatile u16 *)gLinkStatus & 3) != 3) {
        if (++tries > 24) {
                result = -1;
                goto done;
        }
    }
test1:
    if (SerialRuntime_GetActiveTransfers() != 0) {
        goto wait1;
    }
    if (SERIAL_VALUE_B != size) {
        result = -1;
        goto done;
    }
    if (rec[0x12a] != 0) {
        result++;
    }
    Engine_TaskWait(2);
    Call2(Ui_AdjustValueWithoutLimit, (s32)MsgEnemyLabel, (s32)(buf));
    i = 0;
    if (buf[i] != 0) {
        do {
            i++;
            if (i > LOBBY_PREFIX_LIMIT) {
                break;
            }
        } while (buf[i] != 0);
    }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    buf[i] = ' ';
    n = i + 1;
#else
    n = i;
#endif
    for (i = 14; i >= n; i--) {
        rec[i] = rec[i - n];
    }
    for (i = 0; i < n; i++) {
        rec[i] = buf[i];
    }
    rec[14] = 0;
    slot++;
next:
    if (slot > 2) {
        goto second;
    }
    rec = Owner_GetState(slot + 128);
    tries = 0;
    if ((ret = SerialRuntime_BeginTransferB()) == -1) {
        goto failed;
    }
    goto test1;
wait2:
    ret = SERIAL_VALUE_B;
    if (ret > 0x140) {
        result = -1;
        goto done;
    }
    Engine_TaskWait(1);
    if (--timeout < 0 || (*(volatile u16 *)gLinkStatus & 3) != 3) {
        if (++tries > 24) {
            result = -1;
        goto done;
        }
    }
test2:
    if (SerialRuntime_GetActiveTransfers() != 0) {
        goto wait2;
    }
    if (SERIAL_VALUE_B != 0x140) {
        result = -1;
    } else {
        Engine_TaskWait(2);
    }
    goto done;
second:
    Runtime_BumpFree(heap);
    size = 0x140;
    heap = (s32)Runtime_BumpAllocateAlternatePool((s32)size);
    Trade_GetOfferState(1);
    tries = 0;
    if ((ret = SerialRuntime_BeginTransferB()) != -1) {
        goto test2;
    }
failed:
    result = ret;
done:
    Runtime_BumpFree(heap);
    return result;
}

/* The link lobby: the party members copied for the exchange. */
s32 SceneData_CopyUpToThreeEntries(u16 *dest)
{
    s32 cnt = Party_CountActiveOwners();
    if (cnt > 3) cnt = 3;
    if (cnt > 0) {
        s16 *p = (s16 *)&gGameState;
        const u8 *src;
        s32 n;
        p += 252;
        src = (const u8 *)p;
        n = cnt;
        do {
            u8 c = *src++;
            if (dest != 0) { *dest = (u16)c; dest++; }
            n--;
        } while (n != 0);
    }
    if (dest != 0) *dest = 0x00ff;
    return cnt;
}
