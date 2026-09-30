#include "EFFECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "ITEM.H"
extern char MsgNothingHappens;
extern char MsgActorUsesItem;
extern char MsgUseItemQuestion;

s32 BattleFx_ExecutePackedAbilityEffect(s32);
void Battle_ResetEffectCounter(void)
{
  void *runtime;
  void **cell;
  u8 *counter;
  int zero;
  cell = (void **)((u32)&Data_03001ebc);
  runtime = *cell;
  counter = ((u8 *)runtime) + 0xCB6;
  zero = 0;
  *((s16 *)counter) = zero;
  if ((*((s16 *)(((u8 *)runtime) + 0xCB8))) != 0)
  {
    BattleFx_ExecutePackedAbilityEffect(0x2090);
  }
}

void *ObjectTable_Get(u32);
s32 BattleEffect_SelectNearbyObject(u32 object_id);
s32 GameFlag_IsConditionActive(s32 condition);
s32 GetFocusedObjectCollision(void);

struct FacingTrigger {
    s32 flags;
    u16 metadata;
    s16 condition;
    u8 unknown_08[4];
};

struct FacingTriggerRuntime {
    u8 unknown_00[16];
    struct FacingTrigger *triggers;
};

struct FacingObject {
    u8 unknown_00[6];
    u16 facing;
};

s32 Event_FindFacingTrigger(s32 source)
{
    struct FacingTriggerRuntime *runtime =
        (struct FacingTriggerRuntime *)Data_03001ebc;
    struct FacingTrigger *trigger = runtime->triggers;
    s32 facing = ((struct FacingObject *)ObjectTable_Get(
        Data_02000240.object_id))->facing;
    s32 selected = BattleEffect_SelectNearbyObject(Data_02000240.object_id);
    s32 collision;

    source &= 0x1ff;
    collision = GetFocusedObjectCollision();

    while (trigger->flags != -1) {
        s32 high_value = (s16)trigger->metadata & 0xf000;
        s16 has_facing = trigger->metadata & 0x0800;
        s32 low_value = trigger->metadata & 0xff;

        if ((trigger->flags & 0x0f) == 4 &&
            GameFlag_IsConditionActive(trigger->condition) != 0 &&
            (has_facing == 0 ||
             (u16)(high_value - facing + 0x17ff) <= 0x2ffe)) {
            s32 flags = trigger->flags;
            s32 owner = ((u8 *)&trigger->flags)[1];

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

extern struct BattleWork gGameState;
extern struct BattleRuntime *gEventWork;

struct BattleUnitObject {
    u8 unknown_000[0xd8];
    u16 abilities[15]; /* 0xd8: object+216, masked 0x1ff, matches
                           shop/sel/use.c's Ability_GetAvailability scan. */
};

struct ItemPartyView {
    u8 unknown_000[0x1f4];
    s32 object_id;
    u8 active_owners[8];
};

struct ItemCommandRuntime {
    u8 unknown_000[0x170];
    s16 result_code;
    u8 unknown_172[0xb54];
    u8 resolving_action;
};

typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

union BattleItemEffect {
    s32 id;
    BattleItemCallback callback;
};

struct BattleItemEventRecord {
    s32 flags;
    u16 metadata;
    s16 unknown_06;
    union BattleItemEffect effect; /* The ID/pointer threshold is signed. */
};

void *Owner_GetStateFar(s32 actor);
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
/* FAKEMATCH: retain the ROM caller's r1=0; the registered callee has one argument. */
void BattleEv_RunWait(s32 value, s32 flag);
void BattleFx_FinishAction(void);
/* FAKEMATCH: the ROM caller treats this call as setting r0 (an implicit-int
   style declaration); the callee itself returns void. */
s32 BattleFx_LoadActionEffectResources(s32 action_id, s32 mode);
void BattleFx_Run(void);
void BattleEffect_CleanupSceneObjects(void);
u8 Inventory_RemoveFar(s32 actor, s32 slot);

s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot)
{
    s32 result;
    s32 item_id;
    s32 actor;
    struct BattleUnitObject *obj;
    struct BattleItemEventRecord *event;
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
                struct ItemPartyView *party =
                    (struct ItemPartyView *)&gGameState;
                do {
                    obj = (struct BattleUnitObject *)Owner_GetStateFar(party->active_owners[i]);
                    matches = 0;
                    p = obj->abilities;
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
            obj = (struct BattleUnitObject *)Owner_GetStateFar(actor);
            p = obj->abilities;
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

    event = (struct BattleItemEventRecord *)Event_FindFacingTrigger((u16)item_id);
    if (event != 0 && event->effect.id != 0) {
        GameFlag_ClearBitFar(0x143);
        GameFlag_ClearBitFar(0x142);
        if (!(event->metadata & 0x400)) {
            UiWork_PushValueSlotFar(actor, 1);
            UiWork_PushValueSlotFar(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
        }
        if (event->effect.id < 0x10000) {
            s32 objref = BattleEffect_SelectNearbyObject(gGameState.object_id);
            Battle_Reset();
            Event_SetValue1d8(event->effect.id);
            BattleEv_RunWait(objref, 0);
            BattleFx_FinishAction();
        } else {
            event->effect.callback(item_id, actor, slot);
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
                    u16 *work = (u16 *)&gGameState;
                    s32 a, b;
                    a = work[288];
                    work[224] = a;
                    b = work[289];
                    work[225] = b;
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
