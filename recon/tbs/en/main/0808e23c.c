/* Draft, not exact (2026-09-26): 628 of 632 bytes, 283 differing halfwords.
   Normalized edit distance: 81 halfwords. The callback is ordinary C; typed
   views and indexed halfwords recover the confirmation offsets and pool.
   Count/runtime share r9, not r8; best/index exchange r6/r7. Extending the
   word through zero regresses to 106 edits. Reusing best as the no-effect
   flag changes 82 to 81 edits but does not recover the predicted roles.
   Remaining: phase-value allocation and runtime flag-pointer scheduling. */
#include "TYPES.H"
extern u8 Value_000003e7;
#include "ITEM.H"
#include "BATTLE_EFFECT_RUNTIME.H"

extern struct BattleWork Data_02000240;
extern struct BattleRuntime *Data_03001ebc;

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

union ItemCommandWork {
    s32 count;
    struct ItemCommandRuntime *runtime;
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
s32 Event_FindFacingTrigger(u16 item_id);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
s32 GameFlag_TestFar(s32 flag);
void UiWork_PushValueSlotFar(s32 value, s32 mode);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
s32 Object_CallSpawnRoutineAtOrigin(s32 mode);
void UiWork_FinalizePendingCoreFar(void);
s32 Func_0808ddec(s32 object_id);
void Battle_Reset(void);
void Event_SetValue1d8(s32 effect_id);
/* FAKEMATCH: retain the ROM caller's r1=0; the registered callee has one argument. */
void Func_08092f84(s32 value, s32 flag);
void BattleFx_FinishAction(void);
void BattleFx_LoadActionEffectResources(s32 action_id, s32 mode);
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
    s32 matches;
    /* FAKEMATCH: reuse a scalar for the inventory count and no-effect flag. */
    s32 best;
    /* FAKEMATCH: reuse one word across the count and runtime phases. */
    union ItemCommandWork work;

    result = -1;
    item_id = arg & 0x3ff;
    actor = (arg >> 10) & 0xf;
    {
        work.count = Party_CountActiveOwnersFar();
        best = 0;

        if (actor == 15) {
            s32 i;

            actor = 0;
            i = 0;
            if (actor < work.count) {
                struct ItemPartyView *party =
                    (struct ItemPartyView *)&Data_02000240;
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
                } while (i < work.count);
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
            UiText_ShowPositionedMessageAndWaitFar(0x927, 1);
            return -1;
        }
    }

    event = (struct BattleItemEventRecord *)Event_FindFacingTrigger(item_id);
    if (event != 0 && event->effect.id != 0) {
        GameFlag_ClearBitFar(0x143);
        GameFlag_ClearBitFar(0x142);
        if (!(event->metadata & 0x400)) {
            UiWork_PushValueSlotFar(actor, 1);
            UiWork_PushValueSlotFar(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar(0x91c, 1);
        }
        if (event->effect.id < 0x10000) {
            s32 objref = Func_0808ddec(Data_02000240.object_id);
            Battle_Reset();
            Event_SetValue1d8(event->effect.id);
            Func_08092f84(objref, 0);
            BattleFx_FinishAction();
        } else {
            event->effect.callback(item_id, actor, slot);
        }
        result = 0;
    } else {
        s32 action_id;

        best = 0x142;
        GameFlag_ClearBitFar(0x143);
        GameFlag_SetBitFar(best);
        action_id = Item_Get(item_id)->action_id;
        work.runtime = (struct ItemCommandRuntime *)Data_03001ebc;

        if (action_id != 0) {
            GameFlag_SetBitFar(0x145);
            GameFlag_ClearBitFar(best);

            if (action_id == 149 && !GameFlag_TestFar(0x144)) {
                s32 declined;

                UiWork_PushValueSlotFar(item_id, 2);
                UiText_ShowPositionedMessageAndWaitFar(0x924, 13);
                declined = Object_CallSpawnRoutineAtOrigin(1);
                UiWork_FinalizePendingCoreFar();
                if (declined != 0)
                    return 0;

                {
                    u16 *work = (u16 *)&Data_02000240;
                    s32 a, b;
                    a = work[288];
                    work[224] = a;
                    b = work[289];
                    work[225] = b;
                }
                work.runtime->result_code = (s32)&Value_000003e7;
            }

            UiWork_PushValueSlotFar(actor, 1);
            UiWork_PushValueSlotFar(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar(0x91c, 1);
            BattleFx_LoadActionEffectResources(action_id, 0);
            work.runtime->resolving_action = 1;
            BattleFx_Run();
            work.runtime->resolving_action = 0;
            BattleEffect_CleanupSceneObjects();

            if (Item_Get(item_id)->use_type & 1)
                GameFlag_SetBitFar(0x143);
        }
    }

    if (GameFlag_TestFar(0x142))
        UiText_ShowPositionedMessageAndWaitFar(0x927, 1);
    if (GameFlag_TestFar(0x143))
        Inventory_RemoveFar(actor, slot);
    return result;
}
