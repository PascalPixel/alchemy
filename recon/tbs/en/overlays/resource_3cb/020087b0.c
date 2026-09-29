/* Draft of LinkLobby_ExchangePartyRecords, resource_3cb at 0x020087b0 (was
 * MENU/LINK_LOBBY/EXCHANGE_PARTY.C).
 * Remaining difference: it stores to 0x020023a0, twelve bytes before
 * gSerialReceiveDest, which the main image does not name.
 * The listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SERIAL_RUNTIME.H"

s32 LinkLobby_SendPartyRecords(void);
s32 LinkLobby_ReceivePartyRecords(void);
void Engine_GameFlagWriteValue(s32 flag, s32 value);
extern u8 Data_020023a0;

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
    Data_020023a0 = result;
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
    Engine_GameFlagWriteValue(0x3f0, first);
    result = first;
check:
    if (first < 0) {
fail:
        SerialRuntime_ResetTransferState();
    }
    return result;
}
