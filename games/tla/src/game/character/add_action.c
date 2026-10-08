#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"


s32 OwnerAction_Add(s32 state_index, s32 value)
{
    struct OwnerActionState *state = (struct OwnerActionState *)UnitGet(state_index);
    s32 key = value & OWNER_ACTION_ID_MASK;
    s32 found = -1;
    s32 index;

    for (index = 0; index <= 30; index++) {
        s32 masked = state->action_slots[index].encoded_action & OWNER_ACTION_ID_MASK;

        if ((masked ^ key) == 0) {
            state->action_slots[index].encoded_action = masked;
            found = index;
            break;
        }
    }

    if (found < 0) {
        /* FAKEMATCH: the scalar member-address walk preserves halfword operands and the four-byte-longer free-slot loop. */
        for (index = 0; index <= 30; index++) {
            s32 offset = index * (s32)sizeof(state->action_slots[0]) +
                (s32)&((struct OwnerActionState *)0)->action_slots;

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

    Owner_RefreshClassActions(state_index);
    for (index = 0; index <= 31; index++) {
        if (state->action_slots[index].encoded_action == key) {
            break;
        }
    }
    return index;
}
