#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "ITEM.H"

s32 BattleFx_ExecutePackedAbilityEffect(s32);

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
            UiText_ShowPositionedMessageAndWaitFar(0x927, 1);
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
                UiText_ShowPositionedMessageAndWaitFar(0x924, 13);
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
            UiText_ShowPositionedMessageAndWaitFar(0x91c, 1);
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
        UiText_ShowPositionedMessageAndWaitFar(0x927, 1);
    if (GameFlag_TestFar(0x143))
        Inventory_RemoveFar(actor, slot);
    return result;
}
