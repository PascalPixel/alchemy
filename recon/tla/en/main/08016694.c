/* Draft, differ 2 of 121: at the end the native loads the halfword constant
 * 0x80 from the pool before the volatile read of SIOCNT (ldr r2, =0x80; ldrh
 * r3, [r1]), this reads SIOCNT first. Measured for that: the operands either
 * way round, a cast, a u16 or u32 temporary (differ 16, a movs or an extra
 * extension), barriers before or after the timer store, the register through
 * a pinned pointer (all differ 2 or worse); three minutes of permuter found
 * nothing. Fixed so far: sio_control set before the data read (the permuter's
 * form, differ 6 to 4) and the send index read into a temporary before a
 * barrier, so it loads before the isParent store (differ 4 to 2). */
#include "serial_runtime.h"
#include "io_reg.h"

void SerialRuntime_HandleTransferInterrupt(void)
{
    struct SerialRuntime *sendState;
    struct SerialRuntime *receiveState;
    struct SerialRuntime *tailState;
    u16 recv[4];
    volatile u32 *sioControl;
    s32 channel;

    sioControl = &REG_SIOCNT;
    *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
    sendState = &gSerialRuntime;
    {
        s32 index;
        u8 isParent = (*sioControl << 25) >> 31;

        index = sendState->sendIndex;
        asm volatile("");
        sendState->isParent = isParent;
        if (index == -1) {
            u32 idleWord = 0xfefe;
            u16 *swap;

            ((volatile u16 *)sioControl)[1] = idleWord;
            swap = sendState->sendBuffer[1];
            sendState->sendBuffer[1] = sendState->sendBuffer[0];
            sendState->sendBuffer[0] = swap;
        } else if (index >= 0) {
            ((volatile u16 *)sioControl)[1] = sendState->sendBuffer[1][index];
        }
    }
    receiveState = &gSerialRuntime;
    if (receiveState->sendIndex <= 14)
        receiveState->sendIndex++;

    channel = 0;
receive_loop:
    {
        s32 receiveIndex;
        u16 *incoming;

        if (recv[channel] == 0xfefe && receiveState->receiveIndex[channel] > 13) {
            receiveState->receiveIndex[channel] = -1;
        } else {
            receiveIndex = receiveState->receiveIndex[channel];
            incoming = receiveState->incomingBuffer[channel];
            incoming[receiveIndex] = recv[channel];
            if (receiveIndex == 13) {
                u16 *swap = receiveState->readyBuffer[channel];

                receiveState->readyBuffer[channel] = incoming;
                receiveState->incomingBuffer[channel] = swap;
                receiveState->channelFlags[channel] |= 1;
            }
        }
    }
    tailState = &gSerialRuntime;
    if (tailState->isParent != 0)
        tailState->channelFlags[channel] |= 2;
    if (tailState->receiveIndex[channel] <= 14)
        tailState->receiveIndex[channel]++;
    channel++;
    receiveState = tailState;
    if (channel <= 1)
        goto receive_loop;

    if (tailState->mode == 8) {
        REG_TM3CNT_H = 0;
        asm volatile("");
        REG_SIOCNT16 |= 0x80;
        REG_TM3CNT_H = 0xc0;
    }
}
