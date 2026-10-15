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

s32 OwnerAction_Add(s32 stateIndex, s32 value)
{
    struct OwnerActionState *state = (struct OwnerActionState *)UnitGet(stateIndex);
    s32                      key = value & OWNER_ACTION_ID_MASK;
    s32                      found = -1;
    s32                      index;

    for (index = 0; index <= 30; index++)
    {
        s32 masked = state->actionSlots[index].encodedAction & OWNER_ACTION_ID_MASK;

        if ((masked ^ key) == 0)
        {
            state->actionSlots[index].encodedAction = masked;
            found = index;
            break;
        }
    }

    if (found < 0)
    {
        /* FAKEMATCH: the scalar member-address walk preserves halfword operands and the four-byte-longer free-slot loop. */
        for (index = 0; index <= 30; index++)
        {
            s32 offset = index * (s32)sizeof(state->actionSlots[0])
                         + (s32)&((struct OwnerActionState *)0)->actionSlots;

            if (*(u16 *)((u8 *)state + offset) == 0)
            {
                *(u16 *)((u8 *)state + offset) = key;
                found = index;
                break;
            }
        }
        if (found < 0)
        {
            return -1;
        }
    }

    Owner_RefreshClassActions(stateIndex);
    for (index = 0; index <= 31; index++)
    {
        if (state->actionSlots[index].encodedAction == key)
        {
            break;
        }
    }
    return index;
}

void Func_080afb80();

void Func_080af794(s32 owner)
{
    Func_080afb80(owner);
}
