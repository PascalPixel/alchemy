#include "SERIAL_RUNTIME.H"

/* Exchange one packet pair, then report the same status as the poll paths. */

s32 SerialRuntime_ExchangePayloads(void *send, void *receive)
{
    volatile u32 *siocnt;
    u32 control;
    s32 status;
    s32 result;

    siocnt = &REG_SIOCNT;
    control = *siocnt;
    if (gSerialRuntime.phase == 1) {
        SerialRuntime_CollectReceivedPayloads(receive);
        SerialRuntime_PrepareSendPacket(send);
        gSerialRuntime.sequence++;
    }
    status = gSerialRuntime.current_mask | (gSerialRuntime.received_mask << 8);
    if (gSerialRuntime.mode == 8)
        status |= 0x80;
    result = SerialRuntime_AddParentFlag(status);
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
