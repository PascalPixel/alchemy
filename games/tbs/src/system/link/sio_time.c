#include "serial_runtime.h"
#include "dma.h"
#include "io_reg.h"

void SerialRuntime_Initialize(void)
{
    u32 interrupt_enable;
    /* FAKEMATCH: keep the fill word's address before its store. */
    volatile u32 zero;
    s32 index;
    volatile u16 *ime_reg;
    struct SerialRuntime *state;

    interrupt_enable = REG_IME;
    /* FAKEMATCH: retain the reference's IME pointer lifetime. */
    do { ime_reg = &REG_IME; } while (0);
    *ime_reg = (u32)ime_reg;
    Runtime_SetIrqHandler(7, 0, SerialRuntime_HandleTransferInterrupt);
    Runtime_SetIrqHandler(6, 0, SerialRuntime_HandleTransferInterrupt);
    REG_IME = 0;
    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;
    REG_RCNT = 0x8000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x1000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
    state = &gSerialRuntime;
    REG_IME = 1;
    zero = 0;
    Dma_Set(&zero, state, 0x85000058, REG_DMA3);
    state->send_index = -1;
    state->send_buffer[0] = (u16 *)(state->storage + 0);
    state->send_buffer[1] = (u16 *)(state->storage + 32);
    for (index = 0; index <= 1; index++) {
        state->incoming_buffer[index] = (u16 *)(state->storage + 64 + index * 96);
        state->ready_buffer[index] = (u16 *)(state->storage + 96 + index * 96);
        state->pending_buffer[index] = (u16 *)(state->storage + 128 + index * 96);
    }
    REG_IME = 0;
    REG_IE |= 0x80;
    REG_IME = 1;
    gSerialExchangeActive = 1;
    gLinkExchangeState = 0;
    gSerialSendSource = 0;
    gSerialSendSize = 0;
    gSerialReceiveDest = 0;
    gSerialReceivedSize = 0;
    BattleLink_ResetTransferState();
    REG_IME = interrupt_enable;
}

void SerialRuntime_EnableTransferTimer(void)
{
    struct SerialRuntime *state = &gSerialRuntime;

    if (state->mode != 0)
        state->transfer_enabled = 1;
}

void SerialRuntime_DisableTransfer(void)
{
    struct SerialRuntime *state;

    REG_IME = 0;
    REG_IE = REG_IE & 0xff3f;
    REG_IME = 1;
    REG_SIOCNT16 = 0x2003;
    REG_TM3CNT = 0x0000c963;
    REG_IF = 0xc0;
    state = &gSerialRuntime;
    state->transfer_enabled = 0;
}

s32 SerialRuntime_PollAndTransfer(void *send, void *receive)
{
    volatile u32 *siocnt;
    u32 control;
    s32 status;
    s32 result;

    siocnt = &REG_SIOCNT;
    control = *siocnt;
    switch (gSerialRuntime.phase) {
    case 0: {
        u8 mask30 = control & 0x30;
        u8 mode;

        if (mask30 == 0 && (mode = control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && gSerialRuntime.send_index == -1) {
                u32 ie;

                /* FAKEMATCH: the do/while blocks and the separate enable variable are meaningless, as in SerialRuntime_PollStatus. */
                do {
                    REG_IME = serial_error;
                } while (0);
                ie = REG_IE;
                do {
                    ie &= ~0x80;
                    ie |= 0x40;
                    REG_IE = ie;
                    {
                        u8 enable = 1;
                        REG_IME = enable;
                    }
                    *((volatile s8 *)siocnt + 1) &= ~0x40;
                    REG_IF = 0xc0;
                    REG_TM3CNT = 0xc963;
                } while (0);
                gSerialRuntime.mode = mode;
            }
        } else if (mask30 == 0) {
            break;
        }
        gSerialRuntime.phase = 1;
    }
    case 1:
        SerialRuntime_CollectReceivedPayloads(receive);
        SerialRuntime_PrepareSendPacket(send);
        break;
    }

    gSerialRuntime.sequence++;
    status = gSerialRuntime.current_mask | (gSerialRuntime.received_mask << 8);
    if (gSerialRuntime.mode == 8)
        status |= 0x80;
    result = SerialRuntime_AddParentFlag(status);
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}

s32 SerialRuntime_PollStatus(void)
{
    volatile u32 *siocnt;
    u32 control;
    s32 status;
    s32 result;

    siocnt = &REG_SIOCNT;
    control = *siocnt;
    if (gSerialRuntime.phase == 0) {
        u8 mode = control & 0x88;

        if (mode == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && gSerialRuntime.send_index == -1) {
                u32 ie;

                /* FAKEMATCH: one-pass scopes place IE after the IME write and the mask after the IE read; fresh enable reuses r3 for ~0x40. */
                do {
                    REG_IME = serial_error;
                } while (0);
                ie = REG_IE;
                do {
                    ie &= ~0x80;
                    ie |= 0x40;
                    REG_IE = ie;
                    {
                        u8 enable = 1;
                        REG_IME = enable;
                    }
                    *((volatile s8 *)siocnt + 1) &= ~0x40;
                    REG_IF = 0xc0;
                    REG_TM3CNT = 0xc963;
                } while (0);
                gSerialRuntime.mode = mode;
            }
            gSerialRuntime.phase = 1;
        }
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
