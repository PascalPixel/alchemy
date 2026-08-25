#include "types.h"

#define BattlePresentation_DispatchAction Func_080b9b30

struct BattlePresentationTransition {
    s32 blend;
    s32 timer;
    u8 reserved08[12];
    s32 active;
};

s32 BattlePresentation_DispatchAction(s16 *action, s32 delay)
{
    u16 unit_stack[16];
    struct BattlePresentationTransition *transition;
    u8 *battle;
    u8 *actor;
    u8 *render_state;
    u8 **transition_slot;
    s32 result;
    s32 visible_count;
    s32 index;
    s32 preserve_action;

    preserve_action = 0;
    if (action[0] == 0xff)
        return 0;
    actor = Func_08077008(action[0]);
    if (*(s16 *)(actor + 0x38) == 0)
        return -1;
    if (actor[0x129] == 0)
        Func_080bd424(action, 1);

    transition_slot = (u8 **)0x03001f00;
    transition = *(struct BattlePresentationTransition **)transition_slot;
    transition->timer = 60;
    battle = *(transition_slot - 35);
    transition->active = 0;
    render_state = *(transition_slot - 32);
    *(s32 *)(battle + 0x644) = 0x10000;
    Func_080049ac();
    Func_080051d8(render_state, render_state + 12);
    Func_08005258(0x01fe0000, Func_080072f0(0x01fe0000, 0xc000), 0x7fff0000);

    if (delay != 0) {
        transition->blend = 0x2000;
        Func_080030f8(delay);
    }
    unit_stack[14] = action[0];
    unit_stack[15] = 0xff;
    Func_080c10e8(unit_stack + 14, 1);

    result = Func_080be378(action, battle + 0x654);
    if (result == 0) {
        switch (*(s32 *)(battle + 0x6a8)) {
        case 1: Func_080ba27c(battle + 0x654, 0); break;
        case 2: Func_080ba2c0(battle + 0x654, 0); break;
        case 3: Func_080b9ec0(battle + 0x654, 1); break;
        case 4: Func_080b9ec0(battle + 0x654, 0); break;
        case 5: Func_080ba978(battle + 0x654, 0); break;
        case 6: Func_080ba978(battle + 0x654, 1); break;
        case 7: Func_080ba978(battle + 0x654, 2); break;
        case 8: Func_080ba6ac(battle + 0x654, 0, action); break;
        case 9:
            if (Func_080b9dc4(battle + 0x654) != 0)
                preserve_action = 1;
            break;
        }
        if (preserve_action != 0)
            goto finish;
    } else {
        if (result == -1) {
            Func_080bb65c();
            Func_080030f8(3);
        }
        Func_080c10e8(0, 0);
    }

    Func_080b7e7c();
    Func_080bfba4(battle + 0x654);
    Func_080b6c90();
    visible_count = Func_080b6c08(3, unit_stack);
    for (index = 0; index < visible_count; index++)
        Func_080b8000(unit_stack[index]);
    action[0] = 0xff;

finish:
    Func_080c0774(2, *(u16 *)(battle + 0x648), 0);
    return preserve_action;
}
