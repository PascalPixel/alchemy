#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
extern u8 MsgEnemyLabel[];
extern u8 gLinkStatus[];

s32 Runtime_BumpAllocateAlternatePool(u32 size);
void Runtime_BumpFree(s32 heap);
s32 SerialRuntime_BeginTransferB(void);
s32 SerialRuntime_GetActiveTransfers(void);
u8 *Owner_GetState(s32 owner);
void Engine_TaskWait(s32 frames);
void Ui_AdjustValueWithoutLimit(s32 id, u16 *buf);
void Trade_GetOfferState(s32 mode);

static __inline__ void Call2(void (*f)(s32, u16 *), s32 a0, u16 *a1)
{
    f(a0, a1);
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
    heap = Runtime_BumpAllocateAlternatePool(size);
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
    Call2(Ui_AdjustValueWithoutLimit, (s32)MsgEnemyLabel, buf);
    i = 0;
    if (buf[i] != 0) {
        do {
            i++;
            if (i > 4) {
                break;
            }
        } while (buf[i] != 0);
    }
    n = i;
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
    heap = Runtime_BumpAllocateAlternatePool(size);
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
