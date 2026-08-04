#include "layout_guard.h"
#include "types.h"

struct Placement_0808d5dc {
    u8 unknown_00[0x16];
    s8 transition_kind;
};

struct Action_0808d5dc {
    u8 unknown_00[8];
    u32 callback;
};

struct ObjectRuntime_0808d5dc {
    u32 command_table;
    s16 command_index;
    u16 saved_value;
    u8 unknown_08[0x52];
    u8 flags_5a;
    u8 transition_active;
    u8 unknown_5c[8];
    u16 restored_value;
    u8 unknown_66[2];
    struct ObjectRuntime_0808d5dc *reference;
};

LAYOUT_OFFSET_GUARD(
    Placement0808d5dc_TransitionKind,
    struct Placement_0808d5dc,
    transition_kind,
    0x16);
LAYOUT_OFFSET_GUARD(
    Action0808d5dc_Callback,
    struct Action_0808d5dc,
    callback,
    8);
LAYOUT_OFFSET_GUARD(
    ObjectRuntime0808d5dc_CommandIndex,
    struct ObjectRuntime_0808d5dc,
    command_index,
    4);
LAYOUT_OFFSET_GUARD(
    ObjectRuntime0808d5dc_SavedValue,
    struct ObjectRuntime_0808d5dc,
    saved_value,
    6);
LAYOUT_OFFSET_GUARD(
    ObjectRuntime0808d5dc_Flags5a,
    struct ObjectRuntime_0808d5dc,
    flags_5a,
    0x5a);
LAYOUT_OFFSET_GUARD(
    ObjectRuntime0808d5dc_TransitionActive,
    struct ObjectRuntime_0808d5dc,
    transition_active,
    0x5b);
LAYOUT_OFFSET_GUARD(
    ObjectRuntime0808d5dc_RestoredValue,
    struct ObjectRuntime_0808d5dc,
    restored_value,
    0x64);
LAYOUT_OFFSET_GUARD(
    ObjectRuntime0808d5dc_Reference,
    struct ObjectRuntime_0808d5dc,
    reference,
    0x68);

struct Position_0808d5dc {
    s32 x;
    s32 y;
    s32 z;
};

struct ReferenceRuntime_0808d5dc {
    u8 unknown_00[8];
    struct Position_0808d5dc position;
    u8 unknown_14[0x10];
    s32 previous_x;
    s32 previous_y;
    s32 previous_z;
    u8 unknown_30[8];
    struct Position_0808d5dc saved_position;
};

LAYOUT_OFFSET_GUARD(
    ReferenceRuntime0808d5dc_Position,
    struct ReferenceRuntime_0808d5dc,
    position,
    8);
LAYOUT_OFFSET_GUARD(
    ReferenceRuntime0808d5dc_PreviousPosition,
    struct ReferenceRuntime_0808d5dc,
    previous_x,
    0x24);
LAYOUT_OFFSET_GUARD(
    ReferenceRuntime0808d5dc_SavedPosition,
    struct ReferenceRuntime_0808d5dc,
    saved_position,
    0x38);

struct Placement_0808d5dc *Func_0808d394(s32 object_id);
struct ObjectRuntime_0808d5dc *Func_08092054(s32 object_id);
struct Action_0808d5dc *Func_0808d48c(s32 action_kind, s32 object_id);
s32 Func_080915dc(s32 object_id);
u32 Func_08004458(void);
void Func_08091660(void);
void Func_080916b0(void);
void Func_08091750(void);
void Func_08092b94(s32 effect_id);
void Func_08092f84(s32 object_id, s32 unused);
void Func_08009088(struct ObjectRuntime_0808d5dc *object, s32 mode);
void Func_08009098(struct ObjectRuntime_0808d5dc *object, const void *script);
void Func_08092848(s32 object_id, s32 reference_id, s32 unused);
void Func_08093a6c(
    struct ObjectRuntime_0808d5dc *object,
    const void *script);
void Func_08015058(s32 mode);
void Func_0809ade8(s32 object_id);

typedef void (*ActionCallback_0808d5dc)(s32 object_id);

#define CURRENT_OBJECT_ID (*(volatile s16 *)0x0200048a)
#define REFERENCE_OBJECT_ID (*(const s32 *)0x080a0134)
#define TRANSITION_SCRIPT ((const void *)0x0809fc1c)

/*
 * Execute the map action selected for an object.
 *
 * The current object may use a class-7 action; every other object, and the
 * current object's fallback, use class 0.  Small action values select the
 * ordinary effect pipeline, while larger values are one-argument callbacks.
 * Non-current objects temporarily enter transition mode and preserve the
 * runtime state needed by the transition script.
 */
s32 Func_0808d5dc(s32 object_id)
{
    struct Placement_0808d5dc *placement;
    struct ObjectRuntime_0808d5dc *object;
    struct ReferenceRuntime_0808d5dc *reference;
    struct Action_0808d5dc *action;
    s32 is_current;
    s32 used_fallback;
    s32 result;
    u16 saved_value;

    placement = Func_0808d394(object_id);
    object = Func_08092054(object_id);
    result = -1;
    used_fallback = 0;
    is_current = CURRENT_OBJECT_ID == object_id;

    if (is_current) {
        action = Func_0808d48c(7, object_id);
        if (action == 0) {
            action = Func_0808d48c(0, object_id);
            used_fallback = 1;
            if (action == 0)
                return result;
        }

        if (action->callback >= 0x10000) {
            s32 effect_id = 0x0e0b
                + (Func_080915dc(object_id) * 2)
                + ((Func_08004458() >> 16) * 2);

            Func_080916b0();
            Func_08092b94(effect_id);
            Func_08092f84(object_id, 0);
            Func_08091750();
            goto finish;
        }
    } else {
        action = Func_0808d48c(0, object_id);
    }

    if (action == 0 || action->callback == 0)
        goto finish;

    if (!is_current) {
        object->transition_active = 1;
        Func_08009088(object, 0);
        saved_value = object->saved_value;

        if (placement->transition_kind <= 1 ||
            placement->transition_kind == 3) {
            reference = Func_08092054(REFERENCE_OBJECT_ID);
            reference->saved_position = reference->position;
            reference->previous_x = 0;
            reference->previous_y = 0;
            reference->previous_z = 0;
            Func_08092848(object_id, REFERENCE_OBJECT_ID, 0);
        }
    }

    if (action->callback < 0x10000) {
        Func_08015058(used_fallback);
        Func_080916b0();
        Func_08092b94(action->callback);
        Func_08092f84(object_id, 0);
        Func_08091750();
    } else {
        Func_08091660();
        ((ActionCallback_0808d5dc)action->callback)(object_id);
    }

    if (!is_current) {
        s32 command =
            *(s32 *)(object->command_table + ((s32)object->command_index * 4));

        if (command == 16) {
            if (placement->transition_kind == 3) {
                object->reference = Func_08092054(REFERENCE_OBJECT_ID);
                object->flags_5a |= 1;
                Func_08093a6c(object, TRANSITION_SCRIPT);
            } else if (placement->transition_kind == 1) {
                object->restored_value = saved_value;
                Func_08009098(object, TRANSITION_SCRIPT);
            }
        }

        object->transition_active = 0;
        Func_08009088(object, 16);
    }

    result = 0;

finish:
    if (is_current) {
        Func_0809ade8(CURRENT_OBJECT_ID);
        CURRENT_OBJECT_ID = -1;
    }

    return result;
}
