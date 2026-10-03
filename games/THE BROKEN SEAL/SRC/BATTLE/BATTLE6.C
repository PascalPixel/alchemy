#include "BATTLE_RUNTIME.H"
#include "FIELDRUN.H"
#include "SCENE.H"
#include "EFFECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "ITEM.H"
#include "GAME_STATE.H"
#include "BATTLE_UNIT.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"

extern u32 gFrameCount;
extern u8 Animation_ChildPaletteCycle[];

extern char MsgNothingHappens;
extern char MsgActorUsesItem;
extern char MsgUseItemQuestion;
s32 BattleFx_ExecutePackedAbilityEffect(s32);
void *ObjectTable_Get(u32);
s32 BattleEffect_SelectNearbyObject(u32 object_id);
s32 GameFlag_IsConditionActive(s32 condition);
/* The actual producer returns u8; these legacy callers transport its r0 word. */
s32 GetFocusedObjectCollision(void);

extern struct BattleRuntime *gEventWork;

struct ItemCommandRuntime {
    u8 unknown_000[0x170];
    s16 result_code;
    u8 unknown_172[0xb54];
    u8 resolving_action;
};

typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

struct BattleUnit *Owner_GetStateFar(s32 actor);
s32 Party_CountActiveOwnersFar(void);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
s32 GameFlag_TestFar(s32 flag);
void UiWork_PushValueSlotFar(s32 value, s32 mode);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
s32 Object_CallSpawnRoutineAtOrigin(s32 mode);
void UiWork_FinalizePendingCoreFar(void);
void Battle_Reset(void);
void Event_SetValue1d8(s32 effect_id);

void BattleEv_RunWait(s32 value, s32 flag);
void BattleFx_FinishAction(void);

s32 BattleFx_LoadActionEffectResources(s32 action_id, s32 mode);
void BattleFx_Run(void);
void BattleEffect_CleanupSceneObjects(void);
u8 Inventory_RemoveFar(s32 actor, s32 slot);

struct BattleAction *Ability_GetData(s32);
void BattleFx_SetupObjectPair(s32, s32);
s32 BattleFx_RunEventAction(void *, s32, s32);
void FieldEvent_RunTypeHandler(void);
void EffectRuntime_StopCurrentObject(void);

void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette)
{
    struct AnimationObject *state;
    struct AnimationEntry **p;
    struct AnimationEntry *current;
    s32 cnt;
    s32 replacement;
    u8 n;

    if ((obj->record_storage_kind & 15) == 1) {
        state = obj->records;
        replacement = palette - 1;
        if (palette == 0)
            replacement = Animation_ChildPaletteCycle[(gFrameCount >> 1) & 7];

        n = state->count;
        if (n != 0) {
            p = state->entries;
            cnt = n;
            do {
                current = *p++;
                if (current != 0 && current->script != 0 &&
                    current->param != 15)
                    current->param = replacement;
                cnt--;
            } while (cnt != 0);
        }
        state->dirty = 1;
    }
}

void Battle_ResetEffectCounter(void)
{
  void *runtime;
  void **cell;
  u8 *counter;
  int zero;
  cell = (void **)((u32)&Data_03001ebc);
  runtime = *cell;
  counter = (u8 *)&((struct BattleRuntime *)runtime)->unknown_cb6;
  zero = 0;
  *((s16 *)counter) = zero;
  if ((*((s16 *)((struct BattleRuntime *)runtime)->unknown_cb8)) != 0)
  {
    BattleFx_ExecutePackedAbilityEffect(0x2090);
  }
}

s32 Event_FindFacingTrigger(s32 source)
{
    /* FAKEMATCH: retain the existing word-result collision call boundary;
       the true u8 declaration adds two narrowing shifts, four bytes here. */
    struct FieldStepWork *runtime = (struct FieldStepWork *)Data_03001ebc;
    const struct SceneEvent *trigger = runtime->events;
    s32 facing = ((struct MotionObject *)ObjectTable_Get(
        gGameState.selected_actor))->angle;
    s32 selected = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
    s32 collision;

    source &= 0x1ff;
    collision = GetFocusedObjectCollision();

    while (trigger->control != -1) {
        s32 high_value = (s16)(u16)trigger->trigger & 0xf000;
        s16 has_facing = (u16)trigger->trigger & 0x0800;
        s32 low_value = (u16)trigger->trigger & 0xff;

        if ((trigger->control & 0x0f) == 4 &&
            GameFlag_IsConditionActive(trigger->condition) != 0 &&
            (has_facing == 0 ||
             (u16)(high_value - facing + 0x17ff) <= 0x2ffe)) {
            u32 flags = trigger->control;
            s32 owner = ((const u8 *)&trigger->control)[1];

            if (source == 0 || owner == source) {
                if ((flags & 0x10) != 0) {
                    if (low_value == selected)
                        return (s32)trigger;
                } else if (low_value == collision) {
                    return (s32)trigger;
                }
            }
        }
        trigger++;
    }
    return 0;
}

s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot)
{
    /* FAKEMATCH: retain the caller's r1=0; the callee's flag parameter is unused. */
    /* FAKEMATCH: the ROM caller treats this call as setting r0 (an implicit-int
       style declaration); the callee itself returns void. */

    s32 result;
    s32 item_id;
    s32 actor;
    struct BattleUnit *obj;
    const struct SceneEvent *event;
    u16 *p;
    s32 j;
    s32 i;
    s32 best;
    s32 matches;
    s32 count;
    struct ItemCommandRuntime *runtime;

    result = -1;
    item_id = arg & 0x3ff;
    actor = (arg >> 10) & 0xf;
    {
        best = 0;
        count = Party_CountActiveOwnersFar();

        if (actor == 15) {
            actor = 0;
            i = 0;
            if (actor < count) {
                struct GameState *party = &gGameState;
                do {
                    obj = (struct BattleUnit *)Owner_GetStateFar(party->active_owners[i]);
                    matches = 0;
                    p = obj->inventory;
                    j = 14;
                    do {
                        if ((*p++ & 0x1ff) == item_id)
                            matches++;
                        j--;
                    } while (j >= 0);

                    if (best < matches) {
                        best = matches;
                        actor = party->active_owners[i];
                    }
                    i++;
                } while (i < count);
            }
        } else {
            obj = (struct BattleUnit *)Owner_GetStateFar(actor);
            p = obj->inventory;
            j = 14;
            do {
                if ((*p++ & 0x1ff) == item_id)
                    best++;
                j--;
            } while (j >= 0);
        }

        if (best == 0) {
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
            return -1;
        }
    }

    event = (const struct SceneEvent *)Event_FindFacingTrigger((u16)item_id);
    if (event != 0 && (s32)event->value != 0) {
        GameFlag_ClearBitFar(0x143);
        GameFlag_ClearBitFar(0x142);
        if (!((u16)event->trigger & 0x400)) {
            UiWork_PushValueSlotFar(actor, 1);
            UiWork_PushValueSlotFar(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
        }
        if ((s32)event->value < 0x10000) {
            s32 objref = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
            Battle_Reset();
            Event_SetValue1d8((s32)event->value);
            BattleEv_RunWait(objref, 0);
            BattleFx_FinishAction();
        } else {
            ((BattleItemCallback)event->value)(item_id, actor, slot);
        }
        result = 0;
    } else {
        s32 action_id;
        s32 flag;

        flag = 0x142;
        GameFlag_ClearBitFar(0x143);
        GameFlag_SetBitFar(flag);
        action_id = Item_Get(item_id)->action_id;
        runtime = (struct ItemCommandRuntime *)gEventWork;

        if (action_id != 0) {
            GameFlag_SetBitFar(0x145);
            GameFlag_ClearBitFar(flag);

            if (action_id == 149 && !GameFlag_TestFar(0x144)) {
                UiWork_PushValueSlotFar(item_id, 2);
                UiText_ShowPositionedMessageAndWaitFar((s32)&MsgUseItemQuestion, 13);
                /* FAKEMATCH: the owner index doubles as the declined flag, which
                   ranks it above best in global allocation (r6, best r7). */
                i = Object_CallSpawnRoutineAtOrigin(1);
                UiWork_FinalizePendingCoreFar();
                if (i != 0)
                    return 0;

                {
                    struct GameState *work = &gGameState;
                    s32 a, b;
                    a = (u16)work->retreat_scene;
                    work->scene = a;
                    b = (u16)work->retreat_entrance;
                    work->entrance = b;
                }
                runtime->result_code = 999;
            }

            UiWork_PushValueSlotFar(actor, 1);
            UiWork_PushValueSlotFar(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
            BattleFx_LoadActionEffectResources(action_id, 0);
            runtime->resolving_action = 1;
            BattleFx_Run();
            runtime->resolving_action = 0;
            BattleEffect_CleanupSceneObjects();

            if (Item_Get(item_id)->use_type & 1)
                GameFlag_SetBitFar(0x143);
        }
    }

    if (GameFlag_TestFar(0x142))
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
    if (GameFlag_TestFar(0x143))
        Inventory_RemoveFar(actor, slot);
    return result;
}

s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result)
{
    /* FAKEMATCH: retain the existing word-result collision call boundary;
       the true u8 declaration adds two narrowing shifts, four bytes here. */
    struct FieldStepWork *runtime = (struct FieldStepWork *)Data_03001ebc;
    const struct SceneEvent *event = runtime->events;
    s32 reference = ((struct MotionObject *)ObjectTable_Get(
        gGameState.selected_actor))->angle;
    s32 selected = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, group);
    s32 alternate;
    s32 ignore_flags = 0;

    *(s32 *)result = selected;
    alternate = GetFocusedObjectCollision();
    if (requested_flags == 0x70000005)
        ignore_flags = 1;

    while (event->control != -1) {
        s32 high_value = (s16)(u16)event->trigger & 0xf000;
        s16 has_reference = (u16)event->trigger & 0x0800;
        s32 low_value = (u16)event->trigger & 0xff;

        if ((event->control & 0x0f) == 5 &&
            GameFlag_IsConditionActive(event->condition) != 0 &&
            (has_reference == 0 ||
             (u16)(high_value - reference + 0x17ff) <= 0x2ffe) &&
            BattleAction_Get(((const u8 *)&event->control)[1])->type_0c == group &&
            (ignore_flags ||
             (event->control & 0x7000000f) == requested_flags)) {
            if ((event->control & 0x80) != 0)
                return (s32)event;
            if ((event->control & 0x10) != 0) {
                if (low_value == selected)
                    return (s32)event;
            } else if (low_value == alternate) {
                return (s32)event;
            }
        }
        event++;
    }
    return 0;
}

s32 BattleFx_ExecutePackedAbilityEffect(s32 packed)
{
    /* FAKEMATCH: the ROM caller treats this call as setting r0 (an implicit-int
       style declaration); the callee itself returns void. */

    s32 output;
    s32 index;
    s32 mode;
    u8 object;
    void *first;
    void *second;

    index = packed & 0x3FF;
    mode = ((u32)packed >> 10) & 0xF;
    object = Ability_GetData(index)->type_0c;
    ObjectTable_Get(gGameState.selected_actor);
    first = (void *)BattleFx_FindMatchingEvent(0x30000005, object, &output);
    second = (void *)BattleFx_FindMatchingEvent(0x20000005, object, &output);
    BattleFx_LoadActionEffectResources(index, 0);
    BattleFx_SetupObjectPair(gGameState.selected_actor, output);
    BattleFx_RunEventAction(first, mode, output);
    FieldEvent_RunTypeHandler();
    EffectRuntime_StopCurrentObject();
    BattleFx_RunEventAction(second, mode, output);
    BattleEffect_CleanupSceneObjects();
    return 0;
}
