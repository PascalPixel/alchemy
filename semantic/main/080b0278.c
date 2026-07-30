#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct Runtime_080b0278 {
    u8 padding0[0x26c];
    u8 selectionData[0x114];
    void *displayObject;
    u8 padding384[0x0c];
    u16 resource;
    u8 padding392[0x12];
    u16 actorResource;
    u8 result;
    u8 padding3a7[2];
    u8 selectionValue;
    u8 state;
    u8 padding3ab;
    u8 specialSelection;
};

struct Resource_080b0278 {
    u16 value;
};

struct ActorPart_080b0278 {
    u8 padding0[0x28];
    struct Resource_080b0278 *resource;
};

struct Actor_080b0278 {
    u8 padding0[0x50];
    struct ActorPart_080b0278 *part;
};

struct DisplayObject_080b0278 {
    u8 padding0[4];
    u8 type;
    u8 active;
};

typedef char Assert_080b0278_selection_data[
    OFFSET_OF(struct Runtime_080b0278, selectionData) == 0x26c ? 1 : -1];
typedef char Assert_080b0278_display_object[
    OFFSET_OF(struct Runtime_080b0278, displayObject) == 0x380 ? 1 : -1];
typedef char Assert_080b0278_resource[
    OFFSET_OF(struct Runtime_080b0278, resource) == 0x390 ? 1 : -1];
typedef char Assert_080b0278_actor_resource[
    OFFSET_OF(struct Runtime_080b0278, actorResource) == 0x3a4 ? 1 : -1];
typedef char Assert_080b0278_result[
    OFFSET_OF(struct Runtime_080b0278, result) == 0x3a6 ? 1 : -1];
typedef char Assert_080b0278_selection_value[
    OFFSET_OF(struct Runtime_080b0278, selectionValue) == 0x3a9 ? 1 : -1];
typedef char Assert_080b0278_state[
    OFFSET_OF(struct Runtime_080b0278, state) == 0x3aa ? 1 : -1];
typedef char Assert_080b0278_special_selection[
    OFFSET_OF(struct Runtime_080b0278, specialSelection) == 0x3ac ? 1 : -1];
typedef char Assert_080b0278_actor_part[
    OFFSET_OF(struct Actor_080b0278, part) == 0x50 ? 1 : -1];
typedef char Assert_080b0278_part_resource[
    OFFSET_OF(struct ActorPart_080b0278, resource) == 0x28 ? 1 : -1];

s32 Func_080b26c8(s32 selection, s32 mode);
void Func_080b26cc(s32 selection);
void Func_080b010c(void);
u8 Func_080b2764(s32 selection);
struct Actor_080b0278 *Func_0808a080(s32 objectId);
void *Func_080150f8(s32 resource, s32 x, s32 y, s32 mode);
void *Func_08015010(s32 x, s32 y, s32 width, s32 height, s32 mode);
struct DisplayObject_080b0278 *Func_080150c8(
    s32 resource, u32 flags, void *window, s32 x, s32 y);
void Func_080b0a20(void *object, s32 x, s32 y);
void Func_080b04dc(s32 message);
s32 Func_08015380(s32 previousState);
u8 Func_080b2720(s32 selection, void *data);
void Func_080b0aac(void);
void Func_080b1a14(void);
s32 Func_080b0070(void);
void Func_080030f8(s32 frames);
void Func_080b2110(void);
void Func_08015018(void *window, s32 mode);
void Func_080b0204(void);

s32 Func_080b0278(s32 selection, s32 objectId)
{
    struct Runtime_080b0278 *runtime =
        *(struct Runtime_080b0278 **)0x03001f2c;
    struct Actor_080b0278 *actor;
    struct DisplayObject_080b0278 *object;
    void *window;
    s32 state = 0;
    s32 count;

    count = Func_080b26c8(selection, 0);
    if (selection >= count || selection < 0)
        selection = 0;

    Func_080b26cc(selection);
    Func_080b010c();
    runtime->selectionValue = Func_080b2764(selection);

    if (selection == 16)
        runtime->specialSelection = 1;
    if (selection == 17)
        runtime->specialSelection = 1;
    if (selection == 18)
        runtime->specialSelection = 1;

    actor = Func_0808a080(objectId);
    runtime->actorResource = actor->part->resource->value;

    window = Func_080150f8(runtime->actorResource, 0, 0, 0);
    if (window == 0)
        window = Func_08015010(-5, 0, 5, 5, 2);

    object = Func_080150c8(runtime->resource, 0x40000000, window, 0, 0);
    object->active = 1;
    object->type = 0x9b;

    Func_080b0a20(&runtime->displayObject, -32, 112);
    runtime->displayObject = object;
    Func_080b04dc(0xc9b);

    for (;;) {
        s32 commonMessage = 0;

        state = Func_08015380(state);
        runtime->state = state;

        if (state == 0) {
            runtime->result =
                Func_080b2720(selection, runtime->selectionData);
            commonMessage = 0xca7;
        } else if (state == 1) {
            Func_080b04dc(0xca9);
            Func_080b1a14();
        } else if (state == 2) {
            if (Func_080b0070() != 0) {
                commonMessage = 0xcb8;
            } else {
                Func_080b04dc(0xcb7);
                Func_080030f8(1);
            }
        } else if (state == 3) {
            Func_080b04dc(0xcb9);
            Func_080b2110();
        } else {
            break;
        }

        if (commonMessage != 0) {
            Func_080b04dc(commonMessage);
            Func_080b0aac();
        }
        Func_080b0a20(&runtime->displayObject, -32, 112);
        Func_080b04dc(0xca4);
    }

    Func_080b04dc(0xca5);
    Func_08015018(window, 2);
    Func_080b0204();
    return 0;
}
