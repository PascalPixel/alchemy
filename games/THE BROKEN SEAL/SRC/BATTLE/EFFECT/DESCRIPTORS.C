#include "TYPES.H"

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
    u8 unused_14[0x15c];
    s16 queued_result;
    u8 unused_172[0x2a];
    s16 limit;
};

struct EffectObject {
    u8 unused_00[6];
    u16 reference;
};

extern u32 BattleFx_CheckDescriptorKind3Result(s32 descriptor, s32 value);
extern s32 GameFlag_IsConditionActive(s32 condition);
extern struct EffectObject *ObjectTable_Get(s32 object);
extern u8 gGameState;
#define PARTY_STATE gGameState
extern void *gEventWork;

struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value)
{
    struct EffectDescriptorRuntime *runtime =
        (struct EffectDescriptorRuntime *)gEventWork;
    struct EffectDescriptor *descriptor = runtime->descriptors;
    s32 state_index = 250;
    s32 flags;
    u32 reference =
        ObjectTable_Get(*(u32 *)((s16 *)&PARTY_STATE + state_index))->reference;

    flags = descriptor->flags;
    while (flags != -1) {
        if ((flags & 0xf) == kind && descriptor->value == value &&
            (BattleFx_CheckDescriptorKind3Result(flags, descriptor->result) != 0 ||
             (GameFlag_IsConditionActive(descriptor->condition) != 0 &&
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


typedef s32 (*WorkEntryFn)(void);

void Runtime_CallWorkBlockEntry(void)
{
    s32 base = 0x02008000;
    ((WorkEntryFn)*(s32 *)(base + 4))();
}


struct EffectSelectionWork {
    u8 unk_00[0x24a];
    s16 value;
};

extern u8 gGameState;
/* The shared runtime descriptor lookup; distinct from the local BattleFx_FindDescriptor. */
struct EffectDescriptor *BattleFx_FindDescriptor(s32, s32);

s32 BattleFx_FindDescriptorWithOverride(s32 arg0)
{
    struct EffectDescriptor *result = BattleFx_FindDescriptor(0, arg0);
    s32 value = ((struct EffectSelectionWork *)&PARTY_STATE)->value;

    if (value == arg0) {
        struct EffectDescriptor *next = BattleFx_FindDescriptor(7, value);

        if (next != 0) {
            return (s32)next;
        }
    }
    return (s32)result;
}


struct BattleEffectAction {
    s16 id;
    u8 unused_02[20];
    s8 mode;
    u8 unused_17;
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

extern struct ActionDescriptor *BattleAction_FindDescriptor(s32);
extern struct BattleActionObject *Object_GetById(s32);
extern s32 BattleFx_GetFlags(s32);
extern u32 Random16(void);
extern void Battle_Reset(void);
extern void Event_SetValue1d8(s32);
extern void BattleEv_RunWait(s32, s32);
extern void BattleFx_FinishAction(void);
extern void ObjectDispatch_ApplyValueToChildrenFar(struct BattleActionObject *, s32);
extern void Object_LinkPair(s32, s32, s32);
extern void UiWork_SetBusyFlagsFar(s32);
extern void Battle_InitializeRenderObject(void);
extern void ObjectMotion_SetActionCallback(struct BattleActionObject *, void *);
extern void ObjectDispatch_InitializeFar(struct BattleActionObject *, void *);
extern void BattleFx_ResumeObject(s32);
extern u8 gGameState;

s32 BattleFx_RunDescriptorAction(s32 id)
{
    struct BattleEffectAction *action =
        (struct BattleEffectAction *)BattleAction_FindDescriptor(id);
    s32 result = -1;
    struct BattleActionObject *object = Object_GetById(id);
    struct EffectDescriptor *descriptor;
    s32 special = 0;
    u32 saved_value;
    u32 cleared_selection;
    u32 shifted_mode;
    s32 used_fallback = 0;
    s32 selected_offset = 0x24a;

    if (*(s16 *)((u8 *)&PARTY_STATE + selected_offset) == id) {
        special = 1;
        descriptor = (struct EffectDescriptor *)BattleFx_FindDescriptor(7, id);
        if (descriptor == 0) {
            descriptor = (struct EffectDescriptor *)BattleFx_FindDescriptor(0, id);
            used_fallback = 1;
            if (descriptor == 0) {
                return -1;
            }
            if (descriptor->result >= 0x10000) {
                s32 index = BattleFx_GetFlags(id);
                u32 random = Random16();
                s32 message =
                    0x0e0b + index * 2 + (random * 2 >> 16);
                Battle_Reset();
                Event_SetValue1d8(message);
                BattleEv_RunWait(id, 0);
                BattleFx_FinishAction();
                goto finish;
            }
        }
    } else {
        descriptor = (struct EffectDescriptor *)BattleFx_FindDescriptor(0, id);
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
        ObjectDispatch_ApplyValueToChildrenFar(object, 0);
        saved_value = object->value;
        shifted_mode = (u8)action->mode << 24;
        if (shifted_mode <= (1 << 24) || shifted_mode == (3 << 24)) {
            s32 object_index = 250;
            s32 *object_slot =
                (s32 *)((s16 *)&PARTY_STATE + object_index);
            struct BattleActionObject *linked = Object_GetById(*object_slot);
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
        UiWork_SetBusyFlagsFar(used_fallback);
        Battle_Reset();
        Event_SetValue1d8(descriptor->result);
        BattleEv_RunWait(id, 0);
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
                    *(s32 *)((s16 *)&PARTY_STATE + object_index);
                object->linked_object = Object_GetById(object_id);
                object->flags_5a |= 1;
                ObjectMotion_SetActionCallback(object, (void *)0x0809ff40);
            } else if (action->mode == 1) {
                object->saved_value = saved_value;
                ObjectDispatch_InitializeFar(object, (void *)0x0809fc1c);
            }
        }
        object->busy_5b = 0;
        ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    }
    result = 0;
finish:
    if (special) {
        s32 finish_selected_offset = 0x24a;
        s16 *selected =
            (s16 *)((u8 *)&PARTY_STATE + finish_selected_offset);
        BattleFx_ResumeObject(*selected);
        cleared_selection = 0xffff;
        *selected = cleared_selection;
    }
    return result;
}



s32 BattleFx_RunKind6DescriptorAction(s32 arg0)
{
    s32 val;
    s32 ret;
    void *p;

    p = BattleFx_FindDescriptor(6, arg0);
    ret = -1;
    if (p != NULL) {
        val = *(s32 **)((u8 *)p + 8);
        if (val != 0) {
            if (val < 0x10000) {
                Battle_Reset();
                Event_SetValue1d8(*(s32 **)((u8 *)p + 8));
                BattleEv_RunWait(-1, 0);
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


typedef struct {
    s32 flags;
    u8 unk_04[4];
    s32 result;
    u8 unk_0c[0x170 - 0xC];
    s16 queued_result;
    u8 unk_172[0x19C - 0x172];
    s16 limit;
} EffectDescriptorWorkView;

void Audio_PlayCue(s32);
void ObjectEffect_BeginContextEffect26(void);
void ObjectEffect_BeginContextEffect25(void);

s32 BattleAction_RunDescriptor(s32 arg0)
{
    EffectDescriptorWorkView *desc;
    EffectDescriptorWorkView *desc2;
    s32 kind;
    s32 ret;
    EffectDescriptorWorkView *work;

    desc = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(2, arg0);
    ret = -1;
    work = *(EffectDescriptorWorkView **)0x03001ebc;
    if ((desc != 0) && (desc->result != 0)) {
        if (desc->flags & 0x200) {
            work->limit = 0;
        }
        if (desc->result < 0x10000) {
            Battle_Reset();
            Event_SetValue1d8(desc->result);
            BattleEv_RunWait(-1, 0);
            ret = 0;
            BattleFx_FinishAction();
        } else {
            ((void (*)(s32))desc->result)(arg0);
            goto block_17;
        }
    } else {
        desc2 = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(1, arg0);
        if (desc2 != 0) {
            kind = desc2->flags & 0x30;
            switch (kind) {
            case 0: Audio_PlayCue(0x7B); break;
            case 32: Audio_PlayCue(0x80); ObjectEffect_BeginContextEffect26(); break;
            case 48: Audio_PlayCue(0x81); ObjectEffect_BeginContextEffect25(); break;
            }
            work->queued_result = (s16)desc2->result;
block_17:
            ret = 0;
        }
    }
    return ret;
}
