#include "types.h"
#include "ram_buffer.h"
#include "input.h"

struct EventPrompt {
    u8 unknown_000[0x166];
    u16 choice[2];
    u16 result;
    u8 unknown_16c[30];
    s8 mode;
};

s32 GameFlag_Test(s32 flag);

s32 Func_08026dd0(u32 value)
{
    struct EventPrompt *event = Ram_HeapSlots->event_work;
    u32 kind = value >> 14;
    s32 code = value & 0x3fff;

    if (GameFlag_Test(0x107) != 0) {
        event->result = 250;
    } else if (event->mode == 3) {
        /* FAKEMATCH: the native code reads the held keys again for the second test. */
        volatile u32 *held = &gInput.held;

        if (*held & 0x100)
            event->result = 0xfc88;
        else if (*held & 0x200)
            event->result = 0xfc87;
    } else {
        switch (kind) {
        case 0:
            event->choice[0] = code;
            break;
        case 1:
            event->choice[1] = code;
            break;
        }
    }
    return code;
}
