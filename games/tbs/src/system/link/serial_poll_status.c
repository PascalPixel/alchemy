#include "serial_runtime.h"

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
