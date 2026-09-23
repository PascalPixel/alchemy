#include "SERIAL_RUNTIME.H"

#define SerialRuntime_PollAndTransfer Func_08005ee0

s32 SerialRuntime_PollAndTransfer(void *send, void *receive)
{
    volatile u32 *siocnt;
    u32 control;
    s32 status;
    s32 result;

    siocnt = &REG_SIOCNT;
    control = *siocnt;
    switch (Data_02002240.phase) {
    case 0: {
        u8 mask30 = control & 0x30;
        u8 mode;

        if (mask30 == 0 && (mode = control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && Data_02002240.send_index == -1) {
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
                Data_02002240.mode = mode;
            }
        } else if (mask30 == 0) {
            break;
        }
        Data_02002240.phase = 1;
    }
    case 1:
        Func_0800615c(receive);
        Func_080060e8(send);
        break;
    }

    Data_02002240.sequence++;
    status = Data_02002240.current_mask | (Data_02002240.received_mask << 8);
    if (Data_02002240.mode == 8)
        status |= 0x80;
    result = SerialRuntime_AddParentFlag(status);
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
