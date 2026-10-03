#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"

#include "MOTION_OBJECT.H"

/* The two party groups in the order their round-end effects resolve. */
struct BattleGroupOrder {
    s32 group[2];
};

extern const struct BattleGroupOrder BattleParty_RoundEndGroupOrder;

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void Owner_RecalculateStatsFar(s32 unit_id);
s32 Owner_AdjustFirstValueFar(s32 unit_id, s32 amount);
s32 Owner_AdjustSecondValueFar(s32 unit_id, s32 amount);
s32 Djinn_ActivateFar(u8 unit_id, u8 element, u8 index);
void Trade_RemoveOfferFar(u8 unit_id, u8 element, u8 index);
void Object_SetMode(void *object, s32 animation);
void ObjectDispatch_ApplyValueToChildrenFar(void *object, s32 flags);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void Audio_PlayCue(s32 cue);
s32 BattleParty_ListActorIds(s32 group, u16 *ids);
void BattlePres_SetActorModeAndAction(s32 unit_id);
void BattlePresentation_WaitForAdvance(void);
void BattleEventRuntime_SchedulePhase(s32 phase);
s32 BattleUnit_TickCounter132(s32 unit_id);
s32 BattleUnit_TickCounter134(s32 unit_id);
s32 BattleUnit_TickCounter136(s32 unit_id);
s32 BattleUnit_TickCounter138(s32 unit_id);
s32 BattleUnit_TickCounter139(s32 unit_id);
s32 BattleUnit_TickCounter13a(s32 unit_id);
s32 BattleUnit_TickCounter13b(s32 unit_id);
s32 BattleUnit_TickCounter13c(s32 unit_id);
s32 Battle_AdvanceCounterAndCheckChance(s32 unit_id);
s32 BattleUnit_TickCounter13e(s32 unit_id);
s32 BattleUnit_TickCounter13f(s32 unit_id);
s32 BattleUnit_TickCounter146(s32 unit_id);
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 value, s32 arg);
s32 BattleFx_PlayUnitElementEffect(s32 unit_id, s32 element, s32 mode, s32 arg);

/* Round-end processing. Each side's set-aside Djinn count down and return
   to their owners; then every listed unit ticks its counters, regenerates
   HP and PP and recovers from expiring conditions, each with its message.
   Declared int like its callers expect, though it returns nothing. */
s32 Battle_ProcessRoundEnd(void)
{
    struct BattleSession *work;
    s32 side;
    s32 sides;
    s32 count;
    s32 i;
    struct DjinnRecoveryList *list;
    struct DjinnRecoveryEntry *timed_entry;
    struct DjinnRecoveryEntry *expired_entry;

    work = gBattleWork;
    sides = (work->two_sided != 0) + 1;
    for (side = 0; side < sides; side++) {
        list = &Trade_GetOfferStateFar(side)->list;
        i = 0;
        if (i < list->count) {
            timed_entry = list->entries;
            do {
                if (timed_entry->turns > 0 && GetBattleObjectSlot(timed_entry->unit_id) != 0
                    && Owner_GetStateFar(timed_entry->unit_id)->hp != 0) {
                    timed_entry->turns--;
                }
                i++;
                timed_entry++;
            } while (i < list->count);
        }
        i = 0;
        if (i < list->count) {
            expired_entry = list->entries;
            do {
                if (expired_entry->turns == 0) {
                    u8 id = expired_entry->unit_id;
                    s32 element;

                    if (GetBattleObjectSlot(id) != 0) {
                        BattleEventRuntime_Reset();
                        BattleEventRuntime_SchedulePhase(30);
                        BattleEv_Push(0, id);
                        BattleEv_Push(3, expired_entry->element * 20 + expired_entry->index + 300);
                        BattleEv_Push(14, 175);
                        BattleEv_Push(10, 0);
                        BattleEv_Push(4, (s32)&MsgDjinnSet);
                        BattleEv_Push(11, id);
                        Audio_PlayCue(212);
                        Object_SetMode(GetBattleObjectSlot(id)->object, 3);
                        ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(id)->object, 32);
                        element = expired_entry->element;
                        Djinn_ActivateFar(id, element, expired_entry->index);
                        Trade_RemoveOfferFar(id, expired_entry->element, expired_entry->index);
                        Owner_RecalculateStatsFar(id);
                        BattleFx_PlayUnitElementEffect(id, element, 3, 0);
                        BattleEventRuntime_WaitForReady();
                    }
                } else {
                    expired_entry++;
                    i++;
                }
            } while (i < list->count);
        }
    }
    BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
    {
    s32 *group_list;
    struct BattleGroupOrder order = BattleParty_RoundEndGroupOrder;
    u16 ids[14];
    s32 group;
    s32 j;

    if (work->two_sided != 0 && work->link_side != 0) {
        order.group[0] = 2;
        order.group[1] = 1;
    }
    group_list = order.group;
    for (group = 0; group <= 1; group++) {
        count = BattleParty_ListActorIds(group_list[group], ids);
        for (j = 0; j < count; j++) {
            s32 hp_amount;
            s32 pp_amount;
            s32 id = ids[j];
            struct BattleUnit *unit = Owner_GetStateFar(id);

            if (unit->ready_pose != 0) {
                unit->ready_pose--;
            }
            if (unit->hp != 0) {
                if (unit->hp_regen != 0 && unit->hp != unit->max_hp) {
                    hp_amount = unit->hp_regen;
                    if (unit->hp + hp_amount > unit->max_hp) {
                        hp_amount = unit->max_hp - unit->hp;
                    }
                    Owner_AdjustFirstValueFar(id, hp_amount);
                    UiWork_PushValueSlotFar(id, 1);
                    UiWork_PushValueSlotFar(hp_amount, 5);
                    if (unit->hp == unit->max_hp) {
                        UiText_ShowMessageAndWaitCoreFar((s32)&MsgHpFull);
                    } else {
                        UiText_ShowMessageAndWaitCoreFar((s32)&MsgHpRecover);
                    }
                    Audio_PlayCue(175);
                    BattlePresentation_WaitForAdvance();
                }
                if (unit->pp_regen != 0 && unit->pp != unit->max_pp) {
                    pp_amount = unit->pp_regen;
                    if (unit->pp + pp_amount > unit->max_pp) {
                        pp_amount = unit->max_pp - unit->pp;
                    }
                    Owner_AdjustSecondValueFar(id, pp_amount);
                    UiWork_PushValueSlotFar(id, 1);
                    UiWork_PushValueSlotFar(pp_amount, 5);
                    if (unit->pp == unit->max_pp) {
                        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPpFull);
                    } else {
                        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPpRecover);
                    }
                    Audio_PlayCue(175);
                    BattlePresentation_WaitForAdvance();
                }
            }
            if (BattleUnit_TickCounter146(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgAgiNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter132(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgAtkNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter134(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgDefNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter136(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgResNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter138(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgCureDelusion);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter139(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgConscious);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter13a(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgConscious2);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter13b(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                BattlePres_SetActorModeAndAction(id);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgCureStun);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter13c(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                BattlePres_SetActorModeAndAction(id);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgWakes);
                BattlePresentation_WaitForAdvance();
            }
            if (Battle_AdvanceCounterAndCheckChance(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgCureSeal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter13e(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgRefrainEnd);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCounter13f(id) != 0) {
                BattleUnit_BuildStatusFlags(id, GetBattleObjectSlot(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgReflectEnd);
                BattlePresentation_WaitForAdvance();
            }
        }
    }
    }
}
