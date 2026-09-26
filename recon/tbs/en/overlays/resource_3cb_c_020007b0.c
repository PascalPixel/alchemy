/* NONMATCHING: 180 of 176 bytes, 40 differing halfwords, 22 aligned edits.
 * Own-ROM owner 020007b0..02000860 includes the eight-word final pool.
 * CONNECT tests this party-handshake result; send/receive return -1 on failure.
 * 2026-09-26 three structural trials: shared serial globals plus word-width IME
 * snapshot removed the early pool (188/72/47 to 180/37/26); explicit transfer
 * ownership and one reset value retained 180/40/22, restoring pool order.
 * A volatile transfer aggregate added byte reads and regressed to 192/75/48.
 * Retain the second trial. The decoder found no unique source repair.
 * Remaining: initial zero after the flag call, an extra pooled byte zero,
 * and transfer/IME register lifetimes. No exact-byte credit. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SERIAL_RUNTIME.H"

s32 Local_02000580(void);
s32 LinkLobby_ReceivePartyRecords(void);
void Engine_GameFlagWriteValue(s32 flag, s32 value);

extern u8 Data_020023a0;

s32 Func_020007b0(void)
{
    s32 result;
    s32 first;
    u32 ime;
    s32 flag;
    volatile u16 *ime_reg;
    struct SerialTransferState *transfer;
    s32 clear;

    result = 0;
    flag = Engine_GameFlagIsSet(0x302);
    Data_020023a0 = result;
    if (!flag) {
        Engine_TaskWait(5);
        result = Local_02000580();
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
        result = Local_02000580();
        if (result < 0)
            goto fail;
    }
    Engine_GameFlagWriteValue(0x3f0, first);
    result = first;
check:
    if (first < 0) {
fail:
        transfer = &gSerialTransfer;
        ime_reg = (volatile u16 *)0x04000208;
        ime = *ime_reg;
        /* FAKEMATCH: the reference writes the IME address's low halfword. */
        *ime_reg = (u32)ime_reg;
        clear = 0;
        transfer->status = 0x80;
        gSerialSendSource = clear;
        gSerialSendSize = clear;
        gSerialReceiveDest = clear;
        transfer->peer_flags = clear;
        transfer->flags = clear;
        gSerialReceivedSize = clear;
        *ime_reg = ime;
    }
    return result;
}
