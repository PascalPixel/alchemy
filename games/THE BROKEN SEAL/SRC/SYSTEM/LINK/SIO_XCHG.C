#include "SERIAL_RUNTIME.H"
#include "DMA.H"

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

void SerialRuntime_PrepareSendPacket(void *payload)
{
    volatile struct SerialRuntime *state;
    u32 index;
    u32 checksum;

    state = SERIAL_RUNTIME;
    {
        u8 *packet;
        u8 current_mask;
        u8 received_mask;

        packet = (u8 *)state->send_buffer[0];
        packet[0] = state->sequence;
        current_mask = state->current_mask;
        received_mask = state->received_mask;
        checksum = 0;
        packet[1] = received_mask ^ current_mask;
        *(u16 *)(packet + 2) = checksum;
        Dma_Set(payload, packet + 4, 0x84000006, (volatile u32 *)0x040000d4);
    }
    {
        u16 *packet;
        u32 value;

        packet = state->send_buffer[0];
        for (index = 0; index <= 13; index++) {
            value = *packet;
            packet++;
            checksum += value;
        }
    }
    {
        u32 inverse;
        struct SerialPacket *packet;

        inverse = ~checksum;
        packet = (struct SerialPacket *)state->send_buffer[0];
        packet->checksum = inverse;
    }
    if (state->mode != 0)
        REG_TM3CNT_H = 0;
    state->send_index = -1;
    if (state->mode != 0 && state->transfer_enabled != 0)
        REG_TM3CNT_H = (u32)0x000000c0;
}
