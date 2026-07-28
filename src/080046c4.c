#include "low_runtime.h"

void Func_080046c4(const u8 *source)
{
    if (*(u8 *)0x03001ac4 != 0) {
        u32 cursor_address = 0x03001cbc;
        u32 value = *source;
        u16 *destination = *(u16 **)cursor_address;
        u32 count = 0;
        source++;

        if (value != 0) {
            u32 mask = 0xf000;
            cursor_address = 0x06002500;
            do {
                *destination++ = value | mask;
                if (destination == (u16 *)cursor_address)
                    destination = (u16 *)0x06002000;
                count++;
                if (count > 31)
                    break;
                value = *source++;
            } while (value != 0);
            cursor_address = 0x03001cbc;
        }
        *(u16 **)cursor_address = destination;
    }
}
