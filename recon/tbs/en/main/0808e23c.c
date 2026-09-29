/* Draft, not exact (2026-09-26): 628 of 632 bytes, 283 differing halfwords.
   Normalized edit distance: 81 halfwords. The callback is ordinary C; typed
   views and indexed halfwords recover the confirmation offsets and pool.
   Count/runtime share r9, not r8; best/index exchange r6/r7. Extending the
   word through zero regresses to 106 edits. Reusing best as the no-effect
   flag changes 82 to 81 edits but does not recover the predicted roles.
   Remaining: phase-value allocation and runtime flag-pointer scheduling.
   2026-09-29: alchemy permute (8 minutes) 465 -> 270 with three natural
   reorders: best is declared before matches, best = 0 precedes the owner
   count, and i = 0 precedes actor = 0. BattleEv_RunWait's name and the
   plain 999 result code (not a Value_ symbol) bring it to 150: 18
   register-only and one moved movs r1, #0. The register-only rows rotate
   r5/r6/r7 among the ability-scan locals and the action id: global
   allocation ranks best (13 references over 130 insns) above the owner
   index i (7 over 56), so best takes r6 where the reference gives i r6 and
   best r7. A separate no-effect flag local, counting the single-actor scan
   into matches, a one-argument BattleEv_RunWait and testing i in the loop
   guard do not reorder them (150 to 370). A second 8-minute search from 150
   found nothing lower. */
#include "TYPES.H"

#include "ITEM.H"
#include "BATTLE_EFFECT_RUNTIME.H"

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
s32 BattleEffect_SelectNearbyObject(s32 object_id);
void Battle_Reset(void);
void Event_SetValue1d8(s32 effect_id);
/* FAKEMATCH: retain the ROM caller's r1=0; the registered callee has one argument. */
void BattleEv_RunWait(s32 value, s32 flag);
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
    /* FAKEMATCH: reuse a scalar for the inventory count and no-effect flag. */
    s32 best;
    s32 matches;
    /* FAKEMATCH: reuse one word across the count and runtime phases. */
    union ItemCommandWork work;

    result = -1;
    item_id = arg & 0x3ff;
    actor = (arg >> 10) & 0xf;
    {
        best = 0;
        work.count = Party_CountActiveOwnersFar();

        if (actor == 15) {
            s32 i;

            i = 0;
            actor = 0;
            if (actor < work.count) {
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

        best = 0x142;
        GameFlag_ClearBitFar(0x143);
        GameFlag_SetBitFar(best);
        action_id = Item_Get(item_id)->action_id;
        work.runtime = (struct ItemCommandRuntime *)gEventWork;

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
                    u16 *work = (u16 *)&gGameState;
                    s32 a, b;
                    a = work[288];
                    work[224] = a;
                    b = work[289];
                    work[225] = b;
                }
                work.runtime->result_code = 999;
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
