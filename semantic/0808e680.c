#include "layout_guard.h"
#include "types.h"

typedef struct ActionDefinition_0808e680 {
    u8 padding00[9];
    u8 cost;
    u8 padding0a[2];
    u8 type;
} ActionDefinition_0808e680;

typedef struct ActorState_0808e680 {
    u8 padding00[0x3a];
    s16 resource;
} ActorState_0808e680;

typedef struct ActionResource_0808e680 {
    u8 padding00[4];
    u16 flags;
} ActionResource_0808e680;

typedef struct ActionRuntime_0808e680 {
    u8 padding000[0x170];
    s16 special_anchor;
    u8 padding172[0x2c];
    s16 scene_mode;
    u8 padding1a0[0xb26];
    u8 action_running;
} ActionRuntime_0808e680;

typedef struct FieldState_0808e680 {
    u16 current_map;
    u16 current_variant;
    u8 padding004[0x1f0];
    u32 current_object;
    u8 padding1f8[0x48];
    u16 saved_map;
    u16 saved_variant;
} FieldState_0808e680;

LAYOUT_OFFSET_GUARD(
    ActionDefinition0808e680_Cost,
    ActionDefinition_0808e680,
    cost,
    9);
LAYOUT_OFFSET_GUARD(
    ActionDefinition0808e680_Type,
    ActionDefinition_0808e680,
    type,
    0x0c);
LAYOUT_OFFSET_GUARD(
    ActorState0808e680_Resource,
    ActorState_0808e680,
    resource,
    0x3a);
LAYOUT_OFFSET_GUARD(
    ActionRuntime0808e680_SpecialAnchor,
    ActionRuntime_0808e680,
    special_anchor,
    0x170);
LAYOUT_OFFSET_GUARD(
    ActionRuntime0808e680_SceneMode,
    ActionRuntime_0808e680,
    scene_mode,
    0x19e);
LAYOUT_OFFSET_GUARD(
    ActionRuntime0808e680_ActionRunning,
    ActionRuntime_0808e680,
    action_running,
    0xcc6);
LAYOUT_OFFSET_GUARD(
    FieldState0808e680_CurrentObject,
    FieldState_0808e680,
    current_object,
    0x1f4);
LAYOUT_OFFSET_GUARD(
    FieldState0808e680_SavedMap,
    FieldState_0808e680,
    saved_map,
    0x240);

extern ActionRuntime_0808e680 *Data_03001ebc;
extern FieldState_0808e680 Data_02000240;

void Func_08015040(s32, s32);
void Func_08015120(s32, s32);
void Func_08015140(void);
ActorState_0808e680 *Func_08077008(s32);
ActionDefinition_0808e680 *Func_08077080(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);
void Func_080770d0(s32);
void Func_08077120(s32, s32);
void Func_0808b8e8(void);
void Func_0808b98c(void);
void *Func_0808ba1c(s32);
u32 Func_0808df1c(u32, u8);
ActionResource_0808e680 *Func_0808e4b4(u32, u16, s32 *);
s32 Func_0808e5d8(u32);
void Func_08091660(void);
s32 Func_08091d84(s32);
void Func_08096810(void);
void Func_08096960(void);
void Func_08096ab0(void);
void Func_08096b28(ActionResource_0808e680 *, s32, s32);
void Func_08096fb0(s32, s32);
void Func_080970f8(u32, s32);
void Func_08097174(void);
void Func_08097194(void);
void Func_0809728c(void);

static void ShowUnavailable_0808e680(s32 actor, s32 action, s32 text)
{
    Func_08015120(actor, 1);
    Func_08015120(action, 4);
    Func_08015040(text, 1);
}

/* Resolve and present one packed field-action request. */
s32 Func_0808e680(u32 packed_action)
{
    ActionRuntime_0808e680 *runtime = Data_03001ebc;
    s32 action = packed_action & 0x03ff;
    s32 actor = (packed_action >> 10) & 0x0f;
    u8 action_type = Func_08077080(action)->type;
    s32 selected_object = 0;
    s32 restored_special_state = 0;
    ActionResource_0808e680 *primary;
    ActionResource_0808e680 *middle;
    ActionResource_0808e680 *secondary;

    Func_0808ba1c(Data_02000240.current_object);
    Func_08091660();
    Func_080770d0(0x145);
    if (actor == 0x0f)
        actor = 0;

    if (Func_080770c0(0x17e) != 0) {
        ShowUnavailable_0808e680(actor, action, 0x91f);
        return 0;
    }
    if (runtime->scene_mode == 3 && action == 0x90) {
        ShowUnavailable_0808e680(actor, 0x90, 0x91f);
        return 0;
    }

    if (action == 0x95) {
        if (Func_080770c0(0x144) != 0) {
            ShowUnavailable_0808e680(actor, 0x95, 0x921);
            return 0;
        }

        Func_08015120(0x95, 4);
        Func_08015040(0x920, 0x0d);
        restored_special_state = Func_08091d84(1);
        Func_08015140();
        if (restored_special_state != 0)
            return 0;

        Data_02000240.current_map = Data_02000240.saved_map;
        Data_02000240.current_variant = Data_02000240.saved_variant;
        runtime->special_anchor = 0x3e7;
        restored_special_state = 1;
    }

    if ((packed_action & 0x2000) != 0)
        return Func_0808e5d8(packed_action);

    if (actor <= 7) {
        u8 cost = Func_08077080(action)->cost;

        if (Func_08077008(actor)->resource < cost) {
            ShowUnavailable_0808e680(actor, action, 0x91e);
            if (restored_special_state != 0)
                runtime->special_anchor = 0;
            return 0;
        }
        Func_08077120(actor, -(s32)cost);
    }

    primary = Func_0808e4b4(0x10000005, action_type, &selected_object);
    middle = Func_0808e4b4(5, action_type, &selected_object);
    secondary = Func_0808e4b4(0x50000005, action_type, &selected_object);

    selected_object = -1;
    Func_080770c8(0x140);
    Func_080770c8(0x141);
    if (primary != 0 || middle != 0 || secondary != 0) {
        selected_object = (s32)Func_0808df1c(
            Data_02000240.current_object, action_type);
        if (middle != 0 && (middle->flags & 0x0400) != 0) {
            Func_080770d0(0x140);
            Func_080770d0(0x141);
        }
    } else {
        Func_080770d0(0x141);
    }

    if (runtime->scene_mode == 3)
        Func_0808b8e8();

    Func_08096fb0(action, 0);
    runtime->action_running = 1;
    Func_080970f8(Data_02000240.current_object, selected_object);
    Func_0809728c();

    Func_08096b28(primary, actor, selected_object);
    if (Func_080770c0(0x140) != 0) {
        if (Func_080770c0(0x141) != 0)
            Func_08096960();
        else
            Func_08096810();
    }
    Func_08097174();

    Func_08096b28(middle, actor, selected_object);
    if (Func_080770c0(0x140) != 0)
        Func_08096ab0();

    Func_080770d0(0x140);
    Func_080770d0(0x141);
    runtime->action_running = 0;
    Func_08097194();

    if (runtime->scene_mode == 3)
        Func_0808b98c();
    return 0;
}
