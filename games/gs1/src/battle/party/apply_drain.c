#include "types.h"
#include "scene.h"

/* battle/party_apply_drain.c */
/* battle/party/apply_drain.c */
struct BattlePartyTargetState {
    u8 padding[0x1f8];
    u8 target_ids[1];
};

extern struct BattlePartyTargetState gCell;

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Battle_Check(amount);

    if (target_count > 0) {
        u8 *base = (u8 *)&gCell;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Battle_Apply(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}

/* battle/party/apply_status_damage.c */
extern u8 gCell[];

u8 *Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);

void BattleFx_ApplyColorToSourceBuffer(void *, s32);
void BattleFx_StartBufferInterpolation(s32);
void Audio_PlayCue(s32);

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Battle_Check();

    if (result < count) {
        s32 offset = 252;
        u8 *entry;
        s32 remaining;

        offset <<= 1;
        entry = gCell + offset;
        remaining = count;

        do {
            u8 *object = Runtime_GetObject(*entry);
            s32 amount;

            switch ((s8)object[0x131]) {
            case 1:
                amount = -FixedPoint_Ratio(*(s16 *)(object + 0x34) + 10, 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -FixedPoint_Ratio(*(s16 *)(object + 0x34) + 5, 10);
                if (amount == 0)
                    amount = -1;
                if (result <= 1)
                    result = 2;
                break;
            default:
                amount = 0;
                break;
            }

            remaining--;
            Battle_Apply(*entry, amount);
            entry++;
        } while (remaining != 0);
    }

    if (result != 0) {
        BattleFx_ApplyColorToSourceBuffer((void *)0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        Audio_PlayCue(133);
    }

    return result;
}

/* battle/set_object_flag5b_when_mode3.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Battle_SetObjectFlag5bWhenMode3(void)
{
    void *work;
    void *blk;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        blk = Runtime_AllocateBlock(0x1F, 0x540);
        if ((blk != NULL) && (FIELD_AT_OFFSET(blk, s8 *, 0x53D) != 0)) {
            FIELD_AT_OFFSET(blk, s8 *, 0x53A) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53B) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53C) = 1;
            FIELD_AT_OFFSET(blk, s8 *, 0x53D) = 0;
        }
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 1;
        Battle_Check();
    }
}

/* battle/clear_object_flag5b_when_mode3.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Battle_ClearObjectFlag5bWhenMode3(void)
{
    void *work;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        Battle_Check();
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 0;
    }
}

/* object/get_trigger_tile_ahead_of_current.c */
struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Object {
    u8 pad0[6];
    u16 angle;
    struct Vec pos;
    u8 pad1[14];
    u8 kind;
};

extern s32 gCell[];

struct Object *ObjectTable_Get(s32);

s32 Object_GetTriggerTileAheadOfCurrent(void)
{
    u8 *state;
    u8 *map;
    struct Object *obj;
    struct Vec pos;
    u8 *cell;
    u8 *base;
    s32 kind;
    s32 height;
    s32 result;

    result = 0;
    obj = ObjectTable_Get(gCell[125]);
    state = *(u8 **)0x03001ebc;
    map = *(u8 **)0x03001e70;
    if (obj != 0) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        RotateVectorByMagnitude(0x100000, obj->angle, &pos);
        if (*(s16 *)(state + 0x19e) == 3) {
            cell = (u8 *)0x02020000 + ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
        } else {
            base = *(u8 **)(map + 0x130);
            cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
        }
        kind = cell[2];
        if ((u32)(kind - 242) <= 5) {
            height = Obj_unk2(obj->kind, pos.x, pos.z);
            if (height >= obj->pos.y && height <= obj->pos.y + 0x400000) {
                result = kind;
            }
        } else if (Obj_Apply(3, kind) != 0) {
            result = kind;
        }
    }
    return result;
}

/* battle/effects/descriptors/check_kind3_result.c */
/* battle/effects/descriptors/check_kind3_result.c */
u32 BattleFx_CheckDescriptorKind3Result(s32 descriptor, s32 value)
{
  u32 tmp;
  s32 kind;
  u32 result;
  if ((0xF & descriptor) != 3)
  {
    return 0U;
  }
  kind = 0x1FF & descriptor;
  result = 0;
  if (kind != 3)
  {
    tmp = ((u32)value & 0xFFF00000) ^ 0x500000;
    result = tmp | (0U - tmp);
    result = result >> 0x1F;
  }
  return result;
}

/* battle/effects/descriptors/find_matching.c */
struct EffectDescriptor {
    s32 flags;
    u8 value;
    u8 unused_05;
    s16 condition;
    s32 result;
};

struct EffectDescriptorRuntime {
    u8 unused_00[16];
    struct EffectDescriptor *descriptors;
    u8 unused_14[0x188];
    s16 limit;
};

struct EffectObject {
    u8 unused_00[6];
    u16 reference;
};

extern struct EffectObject *Battle_RunCheckKind3Result(s32 object);
extern u8 gCell;
extern void *gWork;

struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value)
{
    struct EffectDescriptorRuntime *runtime =
        (struct EffectDescriptorRuntime *)gWork;
    struct EffectDescriptor *descriptor = runtime->descriptors;
    s32 state_index = 250;
    s32 flags;
    u32 reference =
        Battle_RunCheckKind3Result(*(u32 *)((s16 *)&gCell + state_index))->reference;

    flags = descriptor->flags;
    while (flags != -1) {
        if ((flags & 0xf) == kind && descriptor->value == value &&
            (Battle_ApplyCheckKind3Result(flags, descriptor->result) != 0 ||
             (Battle_CheckCheckKind3Result(descriptor->condition) != 0 &&
              (flags = descriptor->flags, 1)))) {
            s32 accepted = 0;
            s32 threshold = 12;
            s32 range;

            if ((flags & 0x800) != 0)
                threshold = 2;
            range = flags & 0x600;

            switch (range) {
            case 0:
                accepted = 1;
                break;
            case 0x200:
                if (runtime->limit > threshold) {
                    accepted = 1;
                }
                break;
            case 0x400:
                flags &= 0xf000;
                accepted = (u16)((flags - reference) + 0x1fff) <= 0x3ffe;
                break;
            case 0x600:
                if (runtime->limit > threshold) {
                    flags &= 0xf000;
                    accepted =
                        (u16)((flags - reference) + 0x1fff) <= 0x3ffe;
                }
                break;
            }
            if (accepted) {
                return descriptor;
            }
        }
        descriptor++;
        if (descriptor->flags == -1)
            break;
        flags = descriptor->flags;
    }
    return 0;
}

/* battle/effects/descriptors/find_with_override.c */
/* battle/effects/descriptors/find_with_override.c */
struct State_0808d5a4 {
    u8 padding[0x24a];
    s16 value;
};

extern u8 gCell;
struct EffectDescriptor *Battle_RunFindWithOverride(s32, s32);

s32 BattleFx_FindDescriptorWithOverride(s32 arg0)
{
    s32 result = Battle_RunFindWithOverride(0, arg0);
    s32 value = ((struct State_0808d5a4 *)&gCell)->value;

    if (value == arg0) {
        s32 next = Battle_RunFindWithOverride(7, value);

        if (next != 0) {
            return next;
        }
    }
    return result;
}

/* battle/effects/descriptors/run_action.c */
struct ActionDescriptor_0808d5dc {
    s16 id;
    u8 unused_02[20];
    s8 mode;
    u8 unused_17;
};

struct EffectDescriptor_0808d5dc {
    s32 flags;
    u8 unused_04[4];
    s32 result;
};

struct BattleActionObject {
    void **states;
    s16 state_index;
    u16 value;
    u8 unused_08[82];
    u8 flags_5a;
    u8 busy_5b;
    u8 unused_5c[8];
    u16 saved_value;
    u8 unused_66[2];
    void *linked_object;
};

extern struct ActionDescriptor *Battle_RunAction(s32);
extern struct EffectDescriptor *BattleFx_FindDescriptor(s32, s32);
extern struct BattleActionObject *get_by_id(s32);

extern u8 gCell;

s32 BattleFx_RunDescriptorAction(s32 id)
{
    struct ActionDescriptor_0808d5dc *action =
        (struct ActionDescriptor_0808d5dc *)Battle_RunAction(id);
    s32 result = -1;
    struct BattleActionObject *object = get_by_id(id);
    struct EffectDescriptor_0808d5dc *descriptor;
    s32 special = 0;
    u32 saved_value;
    u32 cleared_selection;
    u32 shifted_mode;
    s32 used_fallback = 0;
    s32 selected_offset = 0x24a;

    if (*(s16 *)((u8 *)&gCell + selected_offset) == id) {
        special = 1;
        descriptor = (struct EffectDescriptor_0808d5dc *)BattleFx_FindDescriptor(7, id);
        if (descriptor == 0) {
            descriptor = (struct EffectDescriptor_0808d5dc *)BattleFx_FindDescriptor(0, id);
            used_fallback = 1;
            if (descriptor == 0) {
                return -1;
            }
            if (descriptor->result >= 0x10000) {
                s32 index = Battle_CheckFindWithOverride(id);
                u32 random = random_16();
                s32 message =
                    0x0e0b + index * 2 + (random * 2 >> 16);
                Battle_Reset();
                Battle_Do(message);
                Battle_ApplyFindWithOverride(id, 0);
                BattleFx_FinishAction();
                goto finish;
            }
        }
    } else {
        descriptor = (struct EffectDescriptor_0808d5dc *)BattleFx_FindDescriptor(0, id);
    }

    if (descriptor == 0)
        goto finish;
    if (descriptor->result != 0)
        goto run_descriptor;
    goto finish;
run_descriptor:
    if (!special) {
        u8 *busy = &object->busy_5b;
        *busy = 1;
        Object_SetAction(object, 0);
        saved_value = object->value;
        shifted_mode = (u8)action->mode << 24;
        if (shifted_mode <= (1 << 24) || shifted_mode == (3 << 24)) {
            s32 object_index = 250;
            s32 *object_slot =
                (s32 *)((s16 *)&gCell + object_index);
            struct BattleActionObject *linked = get_by_id(*object_slot);
            *(void **)((u8 *)linked + 56) = *(void **)((u8 *)linked + 8);
            *(void **)((u8 *)linked + 60) = *(void **)((u8 *)linked + 12);
            *(void **)((u8 *)linked + 64) = *(void **)((u8 *)linked + 16);
            *(s32 *)((u8 *)linked + 36) = 0;
            *(s32 *)((u8 *)linked + 40) = 0;
            *(s32 *)((u8 *)linked + 44) = 0;
            Object_LinkPair(id, *object_slot, 0);
        }
    }
    if (descriptor->result < 0x10000) {
        Battle_unk2_2(used_fallback);
        Battle_Reset();
        Battle_Do(descriptor->result);
        Battle_ApplyFindWithOverride(id, 0);
        BattleFx_FinishAction();
    } else {
        typedef void (*EffectRunner)(s32);
        Battle_InitializeRenderObject();
        ((EffectRunner)descriptor->result)(id);
    }
    if (!special) {
        if (object->states[object->state_index] == (void *)16) {
            if (action->mode == 3) {
                s32 object_index = 250;
                s32 object_id =
                    *(s32 *)((s16 *)&gCell + object_index);
                object->linked_object = get_by_id(object_id);
                object->flags_5a |= 1;
                Motion_SetActionCallback(object, (void *)0x0809ff40);
            } else if (action->mode == 1) {
                object->saved_value = saved_value;
                ObjectDispatch_InitializeFar(object, (void *)0x0809fc1c);
            }
        }
        object->busy_5b = 0;
        Object_SetAction(object, 16);
    }
    result = 0;
finish:
    if (special) {
        s32 finish_selected_offset = 0x24a;
        s16 *selected =
            (s16 *)((u8 *)&gCell + finish_selected_offset);
        BattleFx_ResumeObject(*selected);
        cleared_selection = 0xffff;
        *selected = cleared_selection;
    }
    return result;
}

/* battle/effects/descriptors/run_kind6_action.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct EffectDescriptor *Battle_RunFindWithOverride(s32, s32);
void Battle_Reset(void);

s32 BattleFx_RunKind6DescriptorAction(s32 arg0)
{
    s32 val;
    s32 ret;
    void *p;

    p = Battle_RunFindWithOverride(6, arg0);
    ret = -1;
    if (p != NULL) {
        val = FIELD_AT_OFFSET(p, s32 *, 8);
        if (val != 0) {
            if (val < 0x10000) {
                Battle_Reset();
                Battle_Do(FIELD_AT_OFFSET(p, s32 *, 8));
                Battle_ApplyFindWithOverride(-1, 0);
                ret = 0;
                BattleFx_FinishAction();
            } else {
                ((void (*)(s32))val)(arg0);
                ret = 0;
            }
        }
    }
    return ret;
}

/* battle/runtime/run_descriptor.c */
typedef struct {
    s32 flags;
    u8 pad4[4];
    s32 val8;
    u8 padA[0x170 - 0xC];
    s16 h170;
    u8 padB[0x19C - 0x172];
    s16 h19c;
} Obj;

struct EffectDescriptor *Battle_RunFindWithOverride(s32, s32);
void Battle_Reset(void);

void Audio_PlayCue(s32);

s32 BattleAction_RunDescriptor(s32 arg0)
{
    Obj *desc;
    Obj *desc2;
    s32 kind;
    s32 ret;
    Obj *work;

    desc = (Obj *)Battle_RunFindWithOverride(2, arg0);
    ret = -1;
    work = *(Obj **)0x03001ebc;
    if ((desc != 0) && (desc->val8 != 0)) {
        if (desc->flags & 0x200) {
            work->h19c = 0;
        }
        if (desc->val8 < 0x10000) {
            Battle_Reset();
            Battle_Do(desc->val8);
            Battle_ApplyFindWithOverride(-1, 0);
            ret = 0;
            BattleFx_FinishAction();
        } else {
            ((void (*)(s32))desc->val8)(arg0);
            goto block_17;
        }
    } else {
        desc2 = (Obj *)Battle_RunFindWithOverride(1, arg0);
        if (desc2 != 0) {
            kind = desc2->flags & 0x30;
            switch (kind) {
            case 0: Audio_PlayCue(0x7B); break;
            case 32: Audio_PlayCue(0x80); ObjectEffect_BeginContextEffect26(); break;
            case 48: Audio_PlayCue(0x81); ObjectEffect_BeginContextEffect25(); break;
            }
            work->h170 = (s16)desc2->val8;
block_17:
            ret = 0;
        }
    }
    return ret;
}

/* battle/event/dispatch_input_event.c */
struct RuntimeState_0808d8f0 {
    u8 padding0[0x19C];
    s16 delay;
};

struct GlobalData_0808d8f0 {
    u8 padding0[0x1F4];
    s32 selected_object;
};

extern void *gWork;
extern u8 gCell;
extern volatile u32 gIw;

void BattleFx_RunRisingObjectSequence(s32, s32, s32);

s32 Battle_DispatchInputEvent(s32 event)
{
    struct RuntimeState_0808d8f0 *state = (struct RuntimeState_0808d8f0 *)gWork;
    s32 selected_object = ((struct GlobalData_0808d8f0 *)&gCell)->selected_object;

    switch (event) {
    case 0xFC:
        if (state->delay > 12 && (gIw & 0x80)) {
            BattleFx_RunRisingObjectSequence(selected_object, 6, 0);
            state->delay = 0;
        }
        break;
    case 0xF9:
    case 0xFE:
        if (state->delay > 12) {
            Battle_RunDispatchInputEvent();
            state->delay = 0;
        }
        break;
    case 0xFD:
        if (state->delay > 12) {
            if (gIw & 0x80) {
                FieldEffect_UpdateGridPlacement();
            } else if (gIw & 0x40) {
                battle_owner_69();
            }
            state->delay = 0;
        }
        break;
    }
    return 0;
}

/* battle/effects/misc/map_key_through_table.c */
extern s16 gRom[];

s32 BattleFx_MapKeyThroughTable(s32 key)
{
    s16 *entry = gRom;
    s32 result = 16;
    s32 current = *entry;

    while (current != -1) {
        ++entry;
        if (key == current) {
            result = *entry;
            break;
        }
        ++entry;
        current = *entry;
    }
    return result;
}
