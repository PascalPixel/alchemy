#include "SERIAL_RUNTIME.H"
#include "DMA.H"

#define SerialRuntime_PrepareSendPacket Func_080060e8

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
