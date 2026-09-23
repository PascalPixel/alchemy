/* NONMATCHING: 16 halfwords. The reference opens with a dead "mov r3, lr" and
 * loads the 0x02000500 base before the first test; the single-return form
 * below keeps its register order closer than early returns (27 halfwords).
 */
#include "TYPES.H"

#define OWNER_STATE_SIZE 0x14c
#define MAIN_OWNER_COUNT 8
#define REMOTE_OWNER_FIRST 0x80
#define REMOTE_OWNER_COUNT 6

void *Owner_GetState(u32 owner)
{
    void *result = 0;
    u8 *remote_states;

    if (owner < MAIN_OWNER_COUNT) {
        return (void *)(0x02000500 + OWNER_STATE_SIZE * owner);
    }
    if (owner - REMOTE_OWNER_FIRST < REMOTE_OWNER_COUNT) {
        remote_states = *(u8 **)0x03001f28;
        if (remote_states != 0) {
            result = remote_states + OWNER_STATE_SIZE * owner -
                OWNER_STATE_SIZE * REMOTE_OWNER_FIRST;
        }
    }
    return result;
}
