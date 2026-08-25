#include "types.h"

#define BattlePresentation_AppendLinkedActions Func_080b9724

struct BattleLinkedAction {
    s16 unit_id;
    u16 source_id;
    u16 flags;
    u16 unknown_06;
    u16 unknown_08;
    u16 kind;
    u8 unknown_0c[4];
};

struct BattleLinkedActionState {
    s32 count;
    s32 marker;
    s32 display_table;
    u8 unknown_0c[28];
};

s32 BattlePresentation_AppendLinkedActions(
    struct BattleLinkedAction *actions, s32 count)
{
    u8 *battle = *(u8 **)0x03001e74;
    s32 result = 0;
    s32 allocation_size = Func_080022f4(count * 16 + 19, 20) * 20;
    struct BattleLinkedActionState *state = Func_08004970(40);
    s32 index;

    for (index = 0; index < count; index++) {
        struct BattleLinkedAction *action = &actions[index];

        action->source_id = battle[action->unit_id + 72];
        if (battle[80] == 0) {
            if (action->flags & 1) {
                action->flags++;
            }
        } else {
            action->flags |= 1;
        }
    }

    if (battle[82] == 0) {
        if (battle[80] == 0) {
            u16 interrupt_enable;

            state->count = count;
            state->marker = Func_080771a0();
            interrupt_enable = *(u16 *)0x04000208;
            *(u16 *)0x04000208 = 0x0208;
            state->display_table = *(s32 *)0x03001cb4;
            *(s32 *)0x020023a8 = state->display_table;
            *(u16 *)0x04000208 = interrupt_enable;
            if (Func_080b9554() < 0 || Func_080b9604() < 0) {
                goto fail;
            }
            result = state->count;
        } else {
            if (Func_080b9604() < 0) {
                goto fail;
            }
            result = state->count;
            state->count = count;
            if (Func_080b9554() < 0) {
                goto fail;
            }
            if (Func_080771a0() != state->marker) {
                goto fail;
            }
            *(s32 *)0x020023a8 = state->display_table;
        }

        for (index = 0; index < result; index++) {
            struct BattleLinkedAction *action = &actions[count + index];

            action->unit_id = action->source_id;
            action->kind ^= 0x80;
        }

        Func_08002df0(state);
        return result;
    }

fail:
    Func_0800651c();
    Func_08006358();
    Func_08002df0(state);
    return -1;
}
