#include "types.h"

enum MessageId_080b3284 {
    MESSAGE_INTRO_080b3284 = 0xd1c,
    MESSAGE_READY_080b3284,
    MESSAGE_LIMIT_080b3284,
    MESSAGE_FORCED_080b3284,
    MESSAGE_COMPLETE_080b3284,
};

struct RuntimeState_080b3284 {
    u8 padding_000[0x0c];
    s32 window;
    u8 padding_010[0x394];
    u16 resource_id;
    u8 padding_3a6[3];
    u8 active;
    u8 padding_3aa[2];
    u8 special_active;
};

struct GlobalState_080b3284 {
    u8 padding_00[0x10];
    u32 limit;
};

struct ObjectComponent_080b3284 {
    u8 padding_00[0x28];
    u16 *resource_id;
};

struct Object_080b3284 {
    u8 padding_00[0x50];
    struct ObjectComponent_080b3284 *component;
};

extern struct RuntimeState_080b3284 *Data_03001f2c;
extern struct GlobalState_080b3284 Data_02000240;

void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b04dc(s32 message_id);
s32 Func_080b0634(s32);
void Func_080b10cc(void);
s32 Func_080b3210(s32);
void Func_080b3398(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
s32 Func_080150f8(u16, s32, s32, s32);
void Func_08015120(s32, s32);
struct Object_080b3284 *Func_0808a080(s32);

s32 Func_080b3284(s32 mode, s32 object_id)
{
    struct RuntimeState_080b3284 *state;
    struct Object_080b3284 *object;
    s32 resource_window;
    s32 amount;
    s32 message_base;

    Func_080b010c();
    state = Data_03001f2c;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Func_0808a080(object_id);
    state->resource_id = *object->component->resource_id;
    resource_window = Func_080150f8(state->resource_id, 0, 0, 0);

    amount = Func_080b3210(mode);
    Func_08015120(amount, 5);
    message_base = MESSAGE_INTRO_080b3284;
    Func_080b04dc(message_base);
    state->window = Func_08015010(0, 16, 12, 4, 2);
    Func_080b10cc();

    if (Func_080b0634(0) != 0) {
        Func_080b04dc(message_base
            + (MESSAGE_FORCED_080b3284 - MESSAGE_INTRO_080b3284));
        Func_08015018(state->window, 2);
    } else if ((u32)amount > Data_02000240.limit) {
        Func_080b04dc(message_base
            + (MESSAGE_LIMIT_080b3284 - MESSAGE_INTRO_080b3284));
        Func_08015018(state->window, 2);
    } else {
        Func_08015018(state->window, 2);
        Func_080b04dc(message_base
            + (MESSAGE_READY_080b3284 - MESSAGE_INTRO_080b3284));
        Func_08015018(resource_window, 2);
        Func_080b3398(amount);

        object = Func_0808a080(object_id);
        state->resource_id = *object->component->resource_id;
        resource_window = Func_080150f8(state->resource_id, 0, 0, 0);
        Func_080b04dc(message_base
            + (MESSAGE_COMPLETE_080b3284 - MESSAGE_INTRO_080b3284));
    }

    Func_08015018(resource_window, 2);
    Func_080b0204();
    return 0;
}
