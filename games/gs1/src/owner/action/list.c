#include "owner_state.h"

void *Owner_GetState(s32);
void Func_08078bf0(s32);
s32 Func_080792c4(s32, s32);

s32 OwnerAction_Add(s32 state_index, s32 value)
{
    struct OwnerActionState *state = (struct OwnerActionState *)Owner_GetState(state_index);
    s32 key = value & 0x3fff;
    s32 found = -1;
    s32 index;

    for (index = 0; index <= 30; index++) {
        s32 masked = state->action_slots[index].encoded_action & 0x3fff;

        if ((masked ^ key) == 0) {
            state->action_slots[index].encoded_action = masked;
            found = index;
            break;
        }
    }

    if (found < 0) {
        for (index = 0; index <= 30; index++) {
            s32 offset = (index * 4) + 0x58;
            if (*(u16 *)((u8 *)state + offset) == 0) {
                *(u16 *)((u8 *)state + offset) = key;
                found = index;
                break;
            }
        }
        if (found < 0) {
            return -1;
        }
    }

    Func_08078bf0(state_index);
    for (index = 0; index <= 31; index++) {
        if (state->action_slots[index].encoded_action == key) {
            break;
        }
    }
    return index;
}

s32 OwnerAction_CheckLevelThreshold(s32 owner, s32 value)
{
    return Func_080792c4(owner, value);
}
