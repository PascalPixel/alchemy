#include "layout_guard.h"
#include "types.h"

typedef struct RuntimeState_080b29a8 {
    u8 padding_000[0x0c];
    void *window_00c;
    u8 padding_010[0x370];
    void *display_object_380;
    u8 padding_384[0x0c];
    u16 display_resource_390;
    u8 padding_392[0x12];
    u16 actor_resource_3a4;
    u8 padding_3a6[4];
    s8 selection_state_3aa;
} RuntimeState_080b29a8;

typedef struct ActorResource_080b29a8 {
    u16 resource_id_00;
} ActorResource_080b29a8;

typedef struct ActorComponent_080b29a8 {
    u8 padding_00[0x28];
    ActorResource_080b29a8 *resource_28;
} ActorComponent_080b29a8;

typedef struct Actor_080b29a8 {
    u8 padding_00[0x50];
    ActorComponent_080b29a8 *component_50;
} Actor_080b29a8;

typedef struct DisplayObject_080b29a8 {
    u8 padding_00[4];
    u8 mode_04;
    u8 active_05;
} DisplayObject_080b29a8;

LAYOUT_OFFSET_GUARD(
    RuntimeState080b29a8_Window,
    RuntimeState_080b29a8,
    window_00c,
    0x0c);
LAYOUT_OFFSET_GUARD(
    RuntimeState080b29a8_DisplayObject,
    RuntimeState_080b29a8,
    display_object_380,
    0x380);
LAYOUT_OFFSET_GUARD(
    RuntimeState080b29a8_DisplayResource,
    RuntimeState_080b29a8,
    display_resource_390,
    0x390);
LAYOUT_OFFSET_GUARD(
    RuntimeState080b29a8_ActorResource,
    RuntimeState_080b29a8,
    actor_resource_3a4,
    0x3a4);
LAYOUT_OFFSET_GUARD(
    RuntimeState080b29a8_SelectionState,
    RuntimeState_080b29a8,
    selection_state_3aa,
    0x3aa);
LAYOUT_OFFSET_GUARD(
    Actor080b29a8_Component,
    Actor_080b29a8,
    component_50,
    0x50);
LAYOUT_OFFSET_GUARD(
    ActorComponent080b29a8_Resource,
    ActorComponent_080b29a8,
    resource_28,
    0x28);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b29a8_Mode,
    DisplayObject_080b29a8,
    mode_04,
    4);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b29a8_Active,
    DisplayObject_080b29a8,
    active_05,
    5);

extern RuntimeState_080b29a8 *Data_03001f2c;

void *Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(void *, s32);
DisplayObject_080b29a8 *Func_080150c8(u16, u32, void *, s32, s32);
void Func_080150d8(s32, s32, s32, void *, s32, s32);
void *Func_080150f8(u16, s32, s32, s32);
s32 Func_08015388(s32);
Actor_080b29a8 *Func_0808a080(s32);
void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b0a20(void *, s32, s32);
void Func_080b10cc(void);
s32 Func_080b280c(void);
void Func_080b28d4(s32);
void Func_080b2b10(void);

/*
 * Run the actor-backed selection screen.  Its display resource is retained
 * across each selection feedback pass and both allocated windows are released
 * when the signed selection service returns its -1 sentinel.
 */
s32 Func_080b29a8(s32 object_id)
{
    RuntimeState_080b29a8 *state;
    Actor_080b29a8 *actor;
    DisplayObject_080b29a8 *display;
    void *resource_window;
    s8 selection_state = 0;

    Func_080b010c();
    state = Data_03001f2c;
    state->selection_state_3aa = 0;

    actor = Func_0808a080(object_id);
    state->actor_resource_3a4 =
        actor->component_50->resource_28->resource_id_00;

    resource_window =
        Func_080150f8(state->actor_resource_3a4, 0, 0, 0);
    if (resource_window == 0) {
        resource_window = Func_08015010(-5, 0, 5, 5, 2);
        if (resource_window == 0) {
            resource_window = Func_08015010(0, 0, 5, 5, 2);
            Func_080150d8(2, 0, 0, resource_window, -4, -4);
        }
    }

    display = Func_080150c8(
        state->display_resource_390,
        0x40000000,
        resource_window,
        0,
        0);
    display->active_05 = 1;
    display->mode_04 = 0;
    Func_080b0a20(&state->display_object_380, -32, 112);
    state->display_object_380 = display;

    Func_080b28d4(0x0d21);
    state->window_00c = Func_08015010(16, 11, 12, 4, 2);
    Func_080b10cc();

    for (;;) {
        selection_state = (s8)Func_08015388(selection_state);
        state->selection_state_3aa = selection_state;
        if (selection_state == -1)
            break;

        Func_080b28d4(0x0d24);
        if (Func_080b280c() == 0)
            Func_080b28d4(0x0d25);
        else
            Func_080b2b10();

        state->selection_state_3aa = 0;
        Func_080b0a20(&state->display_object_380, -32, 112);
        Func_080b28d4(0x0d22);
    }

    Func_080b28d4(0x0d23);
    Func_08015018(state->window_00c, 2);
    Func_08015018(resource_window, 2);
    Func_080b0204();
    return 0;
}
