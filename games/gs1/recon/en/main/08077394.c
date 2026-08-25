#include "types.h"

#define OWNER_STATE_SIZE 0x14c
#define MAIN_OWNER_COUNT 8
#define REMOTE_OWNER_FIRST 0x80
#define REMOTE_OWNER_COUNT 6

void *Func_08077394(u32 owner)
{
    u8 *remote_states;

    if (owner < MAIN_OWNER_COUNT) {
        return (void *)(0x02000500 + OWNER_STATE_SIZE * owner);
    }
    if (owner - REMOTE_OWNER_FIRST < REMOTE_OWNER_COUNT) {
        remote_states = *(u8 **)0x03001f28;
        if (remote_states != 0) {
            return remote_states + OWNER_STATE_SIZE * owner -
                OWNER_STATE_SIZE * REMOTE_OWNER_FIRST;
        }
    }
    return 0;
}
