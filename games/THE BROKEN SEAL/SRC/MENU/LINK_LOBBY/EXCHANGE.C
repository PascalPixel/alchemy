/* The party-record exchange with the other console: the parent sends
   first and the child receives first; the received count is kept in flag
   byte 0x3f0, and any failure resets the serial transfer. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SERIAL_RUNTIME.H"

s32 LinkLobby_SendPartyRecords(void);
s32 LinkLobby_ReceivePartyRecords(void);
void GameFlag_SetByte(s32 flag, s32 value);

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
