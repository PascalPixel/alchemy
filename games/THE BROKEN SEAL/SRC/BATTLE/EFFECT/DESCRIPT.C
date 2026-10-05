#include "TYPES.H"
#include "IO_REG.H"
#include "OBJDISP.H"
#include "GAME_STATE.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "text/MSG_IDS.H"

s32 GameFlag_TestFar();

extern u8 gOverlayArea[];
extern const u8 ObjectMotion_LinkedActionScript[];
extern const s32 ObjectMotion_StepAngleScript[];

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
extern void *gEventWork;
typedef s32 (*WorkEntryFn)(void);

struct EffectDescriptor *BattleFx_FindDescriptor(s32, s32);

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
    s32 x;
    s32 y;
    s32 z;
    u8 unused_14[0x10];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    u8 unused_30[8];
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unused_44[0x16];
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
extern void Battle_Reset(void);
extern void Event_SetMessageId(s32);
extern void BattleEv_RunWait(s32, s32);
extern void BattleFx_FinishAction(void);
extern void ObjectDispatch_ApplyValueToChildrenFar(struct BattleActionObject *, s32);
extern void Object_LinkPair(s32, s32, s32);
extern void UiWork_SetBusyFlagsFar(s32);
extern void Battle_InitializeRenderObject(void);
extern void ObjectMotion_SetActionCallback(struct BattleActionObject *, void *);
extern void BattleFx_ResumeObject(s32);

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

struct RuntimeState_0808d8f0 {
    u8 padding0[0x19C];
    s16 delay;
};

struct GlobalData_0808d8f0 {
    u8 padding0[0x1F4];
    s32 selected_object;
};

extern volatile u32 gKeysHeld;
void BattleFx_RunRisingObjectSequence(s32, s32, s32);
void Field_TryJumpForward(void);
void FieldEffect_UpdateGridPlacement(void);
void battle_owner_69(void);

s32 GameFlag_IsConditionActive(s32 condition)
{
    s32 flag_state;

    if (condition == -1) {
        return 1;
    }
    if (0x1000 & condition) {
        return GameFlag_TestFar();
    }
    flag_state = GameFlag_TestFar();
    return 1 - ((u32)((0 - flag_state) | flag_state) >> 0x1F);
}

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
    tmp = (value & 0xFFF00000) ^ 0x500000;
    result = tmp | (0U - tmp);
    result = result >> 0x1F;
  }
  return result;
}

struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value)
{
    struct EffectDescriptorRuntime *runtime =
        (struct EffectDescriptorRuntime *)gEventWork;
    struct EffectDescriptor *descriptor = runtime->descriptors;
    s32 state_index = 250;
    s32 flags;
    u32 reference =
        ObjectTable_Get(*(u32 *)((s16 *)&gGameState + state_index))->reference;

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

void Runtime_CallWorkBlockEntry(void)
{
    s32 base = (u32)gOverlayArea;
    ((WorkEntryFn)*(s32 *)(base + 4))();
}

s32 BattleFx_FindDescriptorWithOverride(s32 arg0)
{
    struct EffectDescriptor *result = BattleFx_FindDescriptor(0, arg0);
    s32 value = gGameState.paused_object;

    if (value == arg0) {
        struct EffectDescriptor *next = BattleFx_FindDescriptor(7, value);

        if (next != 0) {
            return (s32)next;
        }
    }
    return (s32)result;
}

s32 BattleFx_RunDescriptorAction(s32 id)
{
    struct BattleEffectAction *action =
        (struct BattleEffectAction *)BattleAction_FindDescriptor(id);
    s32 result = -1;
    struct BattleActionObject *object = Object_GetById(id);
    struct EffectDescriptor *descriptor;
    s32 special = 0;
    u32 saved_value;
    u32 shifted_mode;
    s32 used_fallback = 0;

    if (gGameState.paused_object == id) {
        special = 1;
        descriptor = BattleFx_FindDescriptor(7, id);
        if (descriptor == 0) {
            descriptor = BattleFx_FindDescriptor(0, id);
            used_fallback = 1;
            if (descriptor == 0) {
                return -1;
            }
            if (descriptor->result >= 0x10000) {
                s32 index = BattleFx_GetFlags(id);
                u32 random = Random16();
                s32 message =
                    (s32)MsgPonderEllipsis + index * 2 + (random * 2 >> 16);
                Battle_Reset();
                Event_SetMessageId(message);
                BattleEv_RunWait(id, 0);
                BattleFx_FinishAction();
                goto finish;
            }
        }
    } else {
        descriptor = BattleFx_FindDescriptor(0, id);
    }

    if (descriptor != 0 && descriptor->result != 0) {
        if (!special) {
            u8 *busy = &object->busy_5b;

            *busy = 1;
            ObjectDispatch_ApplyValueToChildrenFar(object, 0);
            saved_value = object->value;
            shifted_mode = (u8)action->mode << 24;
            if (shifted_mode <= (1 << 24) || shifted_mode == (3 << 24)) {
                struct BattleActionObject *linked = Object_GetById(gGameState.selected_actor);

                linked->target_x = linked->x;
                linked->target_y = linked->y;
                linked->target_z = linked->z;
                linked->velocity_x = 0;
                linked->velocity_y = 0;
                linked->velocity_z = 0;
                Object_LinkPair(id, gGameState.selected_actor, 0);
            }
        }
        if (descriptor->result < 0x10000) {
            UiWork_SetBusyFlagsFar(used_fallback);
            Battle_Reset();
            Event_SetMessageId(descriptor->result);
            BattleEv_RunWait(id, 0);
            BattleFx_FinishAction();
        } else {
            Battle_InitializeRenderObject();
            ((void (*)(s32))descriptor->result)(id);
        }
        if (!special) {
            if (object->states[object->state_index] == (void *)16) {
                if (action->mode == 3) {
                    object->linked_object = Object_GetById(gGameState.selected_actor);
                    object->flags_5a |= 1;
                    ObjectMotion_SetActionCallback(object, (void *)ObjectMotion_LinkedActionScript);
                } else if (action->mode == 1) {
                    object->saved_value = saved_value;
                    ObjectDispatch_InitializeFar((struct DispatchObject *)object, ObjectMotion_StepAngleScript);
                }
            }
            object->busy_5b = 0;
            ObjectDispatch_ApplyValueToChildrenFar(object, 16);
        }
        result = 0;
    }
finish:
    if (special) {
        BattleFx_ResumeObject(gGameState.paused_object);
        gGameState.paused_object = -1;
    }
    return result;
}

s32 BattleFx_RunKind6DescriptorAction(s32 arg0)
{
    s32 val;
    s32 ret;
    struct EffectDescriptor *p;

    p = BattleFx_FindDescriptor(6, arg0);
    ret = -1;
    if (p != NULL) {
        val = p->result;
        if (val != 0) {
            if (val < 0x10000) {
                Battle_Reset();
                Event_SetMessageId(p->result);
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

s32 BattleAction_RunDescriptor(s32 arg0)
{
    EffectDescriptorWorkView *desc;
    EffectDescriptorWorkView *desc2;
    s32 kind;
    s32 ret;
    EffectDescriptorWorkView *work;

    desc = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(2, arg0);
    ret = -1;
    work = *(EffectDescriptorWorkView **)&gEventWork;
    if ((desc != 0) && (desc->result != 0)) {
        if (desc->flags & 0x200) {
            work->limit = 0;
        }
        if (desc->result < 0x10000) {
            Battle_Reset();
            Event_SetMessageId(desc->result);
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
            work->queued_result = desc2->result;
block_17:
            ret = 0;
        }
    }
    return ret;
}

s32 Battle_DispatchInputEvent(s32 event)
{
    struct RuntimeState_0808d8f0 *state = (struct RuntimeState_0808d8f0 *)gEventWork;
    s32 selected_object = ((struct GlobalData_0808d8f0 *)&gGameState)->selected_object;

    switch (event) {
    case 0xFC:
        if (state->delay > 12 && (gKeysHeld & KEY_DOWN)) {
            BattleFx_RunRisingObjectSequence(selected_object, 6, 0);
            state->delay = 0;
        }
        break;
    case 0xF9:
    case 0xFE:
        if (state->delay > 12) {
            Field_TryJumpForward();
            state->delay = 0;
        }
        break;
    case 0xFD:
        if (state->delay > 12) {
            if (gKeysHeld & KEY_DOWN) {
                FieldEffect_UpdateGridPlacement();
            } else if (gKeysHeld & KEY_UP) {
                battle_owner_69();
            }
            state->delay = 0;
        }
        break;
    }
    return 0;
}
