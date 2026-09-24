#include "SERIAL_RUNTIME.H"


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

                /* FAKEMATCH: the do/while blocks and the separate enable
                 * variable are meaningless, as in SerialRuntime_PollStatus. */
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
