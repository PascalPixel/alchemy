#include "types.h"

typedef s32 (*ScaleKernel_080b9b30)(s32 value, s32 scale);

void Func_080030f8(s32 arg0);
void Func_080049ac(void);
void Func_080051d8(void *arg0, void *arg1);
void Func_08005258(s32 arg0, s32 arg1, s32 arg2);
void *Func_08077008(s32 arg0);
s32 Func_080b6c08(s32 arg0, u16 *output);
void Func_080b6c90(void);
void Func_080b7e7c(void);
void Func_080b8000(u16 arg0);
s32 Func_080b9dc4(void *arg0);
void Func_080b9ec0(void *arg0, s32 arg1);
void Func_080ba27c(void *arg0, s32 arg1);
void Func_080ba2c0(void *arg0, s32 arg1);
void Func_080ba6ac(void *arg0, s32 arg1, s16 *arg2);
void Func_080ba978(void *arg0, s32 arg1);
void Func_080bb65c(void);
void Func_080bd424(s16 *arg0, s32 arg1);
s32 Func_080be378(s16 *arg0, void *arg1);
void Func_080bfba4(void *arg0);
void Func_080c0774(s32 arg0, u16 arg1, s32 arg2);
void Func_080c10e8(void *arg0, s32 arg1);

s32 Func_080b9b30(s16 *selection, s32 delay)
{
    u8 *runtime;
    u8 *state;
    void *effect_context;
    s32 result = 0;
    s32 entity_id = selection[0];
    s32 action_result;
    u16 local_selection[2];
    u16 pending[16];

    if (entity_id == 255)
        return 0;

    {
        u8 *entity = (u8 *)Func_08077008(entity_id);

        if (*(s16 *)(entity + 56) == 0)
            return -1;
        if (entity[0x129] == 0)
            Func_080bd424(selection, 1);
    }

    state = *(u8 **)0x03001f00;
    runtime = *(u8 **)0x03001e74;
    effect_context = *(void **)0x03001e80;
    *(s32 *)(state + 4) = 60;
    *(s32 *)(state + 20) = 0;
    *(s32 *)(runtime + 0x644) = 0x10000;

    Func_080049ac();
    Func_080051d8(effect_context, (u8 *)effect_context + 12);

    {
        ScaleKernel_080b9b30 scale =
            (ScaleKernel_080b9b30)0x0300013c;
        s32 scaled = scale(0x01fe0000, 0x0000c000);

        Func_08005258(0x01fe0000, scaled, 0x7fff0000);
    }

    if (delay != 0) {
        *(s32 *)state = 0x4000;
        Func_080030f8(delay);
    }

    local_selection[0] = selection[0];
    local_selection[1] = 255;
    Func_080c10e8(local_selection, 1);

    action_result = Func_080be378(selection, runtime + 0x654);
    if (action_result == 0) {
        s32 shared_option = 0;
        s32 call_shared_ba978 = 0;

        switch (*(s32 *)(runtime + 0x6a8)) {
        case 1:
            Func_080ba27c(runtime + 0x654, 0);
            break;
        case 2:
            Func_080ba2c0(runtime + 0x654, 0);
            break;
        case 3:
            shared_option = 0;
            call_shared_ba978 = 1;
            break;
        case 4:
            Func_080ba6ac(runtime + 0x654, 0, selection);
            break;
        case 5:
            Func_080b9ec0(runtime + 0x654, 1);
            break;
        case 6:
            Func_080ba978(runtime + 0x654, 1);
            break;
        case 7:
            if (Func_080b9dc4(runtime + 0x654) != 0)
                result = 1;
            break;
        case 8:
            shared_option = 2;
            call_shared_ba978 = 1;
            break;
        case 9:
            Func_080b9ec0(runtime + 0x654, 0);
            break;
        }

        if (call_shared_ba978)
            Func_080ba978(runtime + 0x654, shared_option);

        if (result != 0)
            goto finish;
    } else {
        if (action_result == -1) {
            Func_080bb65c();
            Func_080030f8(3);
        }
        Func_080c10e8(0, 0);
    }

    Func_080b7e7c();
    Func_080bfba4(runtime + 0x654);
    Func_080b6c90();

    {
        s32 count = Func_080b6c08(3, pending);
        s32 index;

        for (index = 0; index < count; index++)
            Func_080b8000(pending[index]);
    }

    selection[0] = 255;

finish:
    runtime = *(u8 **)0x03001e74;
    Func_080c0774(2, *(u16 *)(runtime + 0x648), 0);
    return result;
}
