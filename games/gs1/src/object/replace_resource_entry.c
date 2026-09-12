#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "object_lookup.h"
#include "object_efx.h"
#include "effect_0809b11c.h"

/* object/replace_resource_entry.c */
/* object/replace_resource_entry.c */
s32 Resource_ResetEntry(u32 index);

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
    void *ret;
    void *obj;

    obj = src;
    ret = NULL;
    if (obj != NULL) {
        if (alt == NULL) {
            FIELD_AT_OFFSET(obj, u8 *, 0x1D) = (u8)(FIELD_AT_OFFSET(obj, u8 *, 0x1D) | 1);
        } else {
            Resource_ResetEntry(FIELD_AT_OFFSET(obj, u8 *, 0x1C));
            FIELD_AT_OFFSET(obj, u8 *, 0x1C) = (u8)FIELD_AT_OFFSET(alt, u8 *, 0x1C);
            FIELD_AT_OFFSET(obj, u8 *, 0x1D) = (u8)(FIELD_AT_OFFSET(obj, u8 *, 0x1D) | 1);
            obj = alt;
        }
        ret = obj;
    }
    return ret;
}

/* object/spawn.c */
u8 *Obj_Run(s32, s32, s32, s32);
void Object_Destroy(void);

void Object_SetMode(u8 *, s32);

u8 *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    u8 *base = *(u8 **)ADDR_03001F30;
    u8 *object;
    u8 *child;
    u8 flag;

    object = Obj_Run(kind, x, y, z);
    if (object != 0) {
        if (object[84] == 0) {
            Object_Destroy();
            return 0;
        }
        *(u32 *)(object + 20) = *(u32 *)(*(u8 **)(base + 16) + 20);
        flag = 4;
        object[85] = flag;
        object[35] = flag;
        child = *(u8 **)(object + 80);
        child[9] &= ~(flag + 8);
        Obj_Apply(object, 0);
        Object_SetMode(object, 1);
    }
    return object;
}

/* object/group/set_action_for_others.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Object_SetAction(s32, s32);
extern u8 gOv[];

void ObjectGroup_SetActionForOthers(s32 excluded_object, s32 group_mode, s32 action)
{
    s16 *active_object_id;
    s32 object;
    s32 object_id;

    object_id = 0;
    active_object_id = (s16 *)gOv;
    do {
        object = ObjectTable_Get(object_id);
        if ((object_id != *active_object_id) && (object != 0) && (object != excluded_object)) {
            FIELD_AT_OFFSET(object, s8, 0x5B) = group_mode;
            Object_SetAction(object, action);
        }
        object_id += 1;
    } while (object_id <= 0x42);
}

/* battle/effects/fx_update_scaled_arc_object_a.c */
/* battle/effects/obj/update_scaled_arc_object_a.c */
void BattleFx_UpdateScaledArcObjectA(void *obj)
{
    s32 v;
    s32 step;
    void *link;

    step = FIELD_AT_OFFSET(obj, u16 *, 0x64) + 1;
    link = *(void *volatile *)((u8 *)obj + 0x68);
    *(volatile s16 *)((u8 *)obj + 0x64) = step;
    v = (s16)step;
    if (v > 0x1F) {
        Object_SetCallback((s32)obj, gRom);
        return;
    }
    v = Battle_Run(v << 10);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = v;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = v;
    FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
    FIELD_AT_OFFSET(obj, s32 *, 0xC) += 0x10000;
    FIELD_AT_OFFSET(obj, s32 *, 0x10) =
        FIELD_AT_OFFSET(link, s32 *, 0x10) + ((0x10000 - v) * 5) + 0x90000;
}

/* battle/effects/obj/update_scaled_arc_object_b.c */
void BattleFx_UpdateScaledArcObjectB(void *obj)
{
    s32 v;
    s32 step;
    void *link;

    step = FIELD_AT_OFFSET(obj, u16 *, 0x64) + 1;
    link = *(void *volatile *)((u8 *)obj + 0x68);
    *(volatile u16 *)((u8 *)obj + 0x64) = step;
    v = (s16)step;
    if (v > 0x1F) {
        Object_SetCallback((s32)obj, gRom);
        return;
    }
    v = Battle_Run(v << 10);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = v;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = -v;
    FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
    FIELD_AT_OFFSET(obj, s32 *, 0xC) += 0x10000;
    FIELD_AT_OFFSET(obj, s32 *, 0x10) =
        FIELD_AT_OFFSET(link, s32 *, 0x10) - ((0x10000 - v) * 5) + 0x100000;
}

/* battle/effects/objects/flicker_and_tick.c */
extern s32 gIw;

void BattleFx_FlickerObjectAndTick(s32 arg0)
{
    if ((gIw & 2) != 0) {
        Battle_Apply(arg0, 7);
    } else {
        Battle_Apply(arg0, 0);
    }
    if ((gIw & 15) == 0) {
        Battle_Do(arg0);
    }
}

/* battle/effects/obj/cycle_object_value_by_counter.c */
extern volatile s32 gIw;

void BattleFx_CycleObjectValueByCounter(s32 arg0)
{
    if ((gIw & 1) != 0) {
        s32 value = Battle_Apply((s32)((unsigned int)gIw >> 1), 6);

        Battle_Apply2(arg0, value);
    }
    if ((gIw & 15) == 0) {
        Battle_Do(arg0);
    }
}

/* battle/effects/misc/update_all_effect_slots.c */
void BattleFx_UpdateAllEffectSlots(void)
{
    s32 p;
    s32 cnt;

    p = *(s32 *)ADDR_03001F30 + 0x58;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_Update((struct EffectSlot *)p);
        p += 0x48;
    } while (cnt >= 0);
}

/* battle/effects/obj/setup_object_pair.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void RotateVectorByMagnitude(s32, s32, void *);
void BattleFx_SetupObjectPair(s32 first_object_id, s32 second_object_id)
{
    void *first_object; void *second_object; s32 facing_quadrant; void *state;
    state = *(void **)ADDR_03001F30;
    FIELD_AT_OFFSET(state, s16, 0x18) = first_object_id;
    first_object = ObjectTable_Get((s16)first_object_id);
    FIELD_AT_OFFSET(state, s16, 0x1A) = second_object_id;
    FIELD_AT_OFFSET(state, s32 *, 0x10) = (s32)first_object;
    second_object = ObjectTable_Get((s16)second_object_id);
    facing_quadrant = (FIELD_AT_OFFSET(first_object, u16, 6) + 0x2000) & 0xC000;
    FIELD_AT_OFFSET(state, s32 *, 0x14) = (s32)second_object;
    FIELD_AT_OFFSET(state, s32 *, 0) = facing_quadrant;
    if (second_object != 0) {
        FIELD_AT_OFFSET(state, s32 *, 0x38) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0x6C);
        FIELD_AT_OFFSET(state, s32 *, 0x3C) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0);
        {
            u8 object_variant = (u8)FIELD_AT_OFFSET(FIELD_AT_OFFSET(FIELD_AT_OFFSET(second_object, void **, 0x50), void **, 0x28), u8, 5);
            FIELD_AT_OFFSET(state, u8, 0x44) = object_variant;
        }
        FIELD_AT_OFFSET(state, s32 *, 4) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 8);
        FIELD_AT_OFFSET(state, s32 *, 0xC) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0x10);
        FIELD_AT_OFFSET(state, s32 *, 8) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0xC);
        return;
    }
    FIELD_AT_OFFSET(state, s32 *, 4) = (s32)FIELD_AT_OFFSET(first_object, s32 *, 8);
    FIELD_AT_OFFSET(state, s32 *, 0xC) = (s32)FIELD_AT_OFFSET(first_object, s32 *, 0x10);
    FIELD_AT_OFFSET(state, s32 *, 8) = (s32)FIELD_AT_OFFSET(first_object, s32 *, 0xC);
    RotateVectorByMagnitude(0x100000, facing_quadrant, (u8 *)state + 4);
}

/* effect_runtime/stop_current_object.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void WaitFrames(s32);

struct State_08097174 {
    u8 unknown_00[0x10];
    u8 *object;
};

void EffectRuntime_StopCurrentObject(void)
{
    struct State_08097174 *state = *(void **)ADDR_03001F30;
    u8 *object = state->object;

    FIELD_AT_OFFSET(object, s32 *, 0x6C) = 0;
    Sys_Apply(object, 0);
    WaitFrames(1);
}
