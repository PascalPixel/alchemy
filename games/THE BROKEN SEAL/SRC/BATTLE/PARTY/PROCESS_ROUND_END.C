#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OWNERVAL.H"
#include "BATTLE_STATUS_ICON.H"
#include "BTLMES.H"
#include "BTLPTY.H"
#include "BTLTYPE.H"
#include "BTLWORK.H"

#include "MOTOBJ.H"
#include "MEM.H"
#include "BTLEV.H"
#include "BTLSYS.H"
#include "OBJDISP.H"
#include "IWRAM_CALL.H"
#include "BTLCALC.H"
void UnitCalcFar(s32 unit_id);

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct MotionObject*,s32))ObjectDispatch_ApplyValueToChildrenFar)
#define BattlePres_SetActorModeAndAction ((void (*)(s32))BattlePres_SetActorModeAndAction)
#define BattlePresentation_ConfigurePaletteFade ((void (*)(s32,s32,s32))BattlePresentation_ConfigurePaletteFade)
#define BattleFx_PlayUnitElementEffect ((s32 (*)(s32,s32,s32,s32))BattleFx_PlayUnitElementEffect)

#define MES_PUSH(m) BtlEvPush(BEV_MES, (u32)(m))

struct BattleGroupOrder {
    s32 group[2];
};


struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
s32 Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
s32 Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct MotionObject *object, s32 animation);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void AudioCommand_PlayFar(s32 cue);
void BattlePresentation_WaitForAdvance(void);

s32 Battle_ProcessRoundEnd(void)
{
    struct BtlWork *work;
    s32 side;
    s32 sides;
    s32 count;
    s32 i;
    struct DjinnRecoveryList *list;
    struct DjinnRecoveryEntry *timed_entry;
    struct DjinnRecoveryEntry *expired_entry;

    work = BtlWk;
    sides = (work->two_sided != 0) + 1;
    for (side = 0; side < sides; side++) {
        list = &Trade_GetOfferStateFar(side)->list;
        i = 0;
        if (i < list->count) {
            timed_entry = list->entries;
            do {
                if (timed_entry->turns > 0 && BtlObjGet(timed_entry->unit_id) != 0
                    && UnitGetFar(timed_entry->unit_id)->hp != 0) {
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

                    if (BtlObjGet(id) != 0) {
                        BattleEventRuntime_Reset();
                        BattleEventRuntime_SchedulePhase(30);
                        BtlEvPush(BEV_UNIT, id);
                        BtlEvPush(BEV_ACT, expired_entry->element * 20 + expired_entry->index + 300);
                        BtlEvPush(BEV_SE, 175);
                        BtlEvPush(BEV_REDRAW, 0);
                        MES_PUSH(MsgDjinnSet);
                        BtlEvPush(BEV_OBJ_END, id);
                        AudioCommand_PlayFar(212);
                        ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 3);
                        ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, 32);
                        element = expired_entry->element;
                        Djinn_ActivateFar(id, element, expired_entry->index);
                        Trade_RemoveOfferFar(id, expired_entry->element, expired_entry->index);
                        UnitCalcFar(id);
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
    BattlePresentation_ConfigurePaletteFade(2, BtlWk->background, 0);
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
            struct BtlUnit *unit = UnitGetFar(id);

            if (unit->sonae != 0) {
                unit->sonae--;
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
                        UiText_ShowMessageAndWaitCoreFar((s32)MsgHpFull);
                    } else {
                        UiText_ShowMessageAndWaitCoreFar((s32)MsgHpRecover);
                    }
                    AudioCommand_PlayFar(175);
                    BattlePresentation_WaitForAdvance();
                }
                if (unit->ep_regen != 0 && unit->ep != unit->max_ep) {
                    pp_amount = unit->ep_regen;
                    if (unit->ep + pp_amount > unit->max_ep) {
                        pp_amount = unit->max_ep - unit->ep;
                    }
                    UnitEpAddFar(id, pp_amount);
                    UiWork_PushValueSlotFar(id, 1);
                    UiWork_PushValueSlotFar(pp_amount, 5);
                    if (unit->ep == unit->max_ep) {
                        UiText_ShowMessageAndWaitCoreFar((s32)MsgEpFull);
                    } else {
                        UiText_ShowMessageAndWaitCoreFar((s32)MsgEpRecover);
                    }
                    AudioCommand_PlayFar(175);
                    BattlePresentation_WaitForAdvance();
                }
            }
            if (BattleUnit_TickAgilityModifier(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgAgiNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickAttackModifier(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgAtkNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickDefenseModifier(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgDefNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickResistanceModifier(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgResNormal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickDelusion(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgCureDelusion);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickConfusion(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgConscious);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickCharm(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgConscious2);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickStun(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                BattlePres_SetActorModeAndAction(id);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgCureStun);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickSleep(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                BattlePres_SetActorModeAndAction(id);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgWakes);
                BattlePresentation_WaitForAdvance();
            }
            if (Battle_AdvanceCounterAndCheckChance(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgCureSeal);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickRefrain(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgRefrainEnd);
                BattlePresentation_WaitForAdvance();
            }
            if (BattleUnit_TickReflect(id) != 0) {
                BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
                UiWork_PushValueSlotFar(id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgReflectEnd);
                BattlePresentation_WaitForAdvance();
            }
        }
    }
    }
}


s32 BattleUnit_ProcessTurnEnd(struct BtlPlan *plan)
{
    s32 id;
    struct BtlUnit *unit;
    s32 both_sides;
    s32 counts[4];
    struct DjinnRecoveryList *list;
    s32 best;
    s32 order;

    id = plan->aid;
    both_sides = 0;
    unit = UnitGetFar(id);
    list = &Trade_GetOfferStateFar((id < 0 || id > 7))->list;
    for (best = 0; best < list->count; best++) {
        if (list->entries[best].unit_id == id && list->entries[best].turns == -1) {
            Djinn_DeactivateFar(id, list->entries[best].element, list->entries[best].index);
        }
    }
    if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0 && BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0) {
        both_sides = 1;
    }
    {
        s32 i;
        s32 owner;
        struct DjinnRecoveryList *pending;
        pending = &Trade_GetOfferStateFar((id < 0 || id > 7))->list;
        for (i = 0; i < 4; i++) {
            counts[i] = 0;
        }
        for (;;) {
            s32 tmp;
            owner = -1;
            for (i = 0; i < pending->count; i++) {
                if (pending->entries[i].turns == -2) {
                    owner = pending->entries[i].unit_id;
                    break;
                }
            }
            /* FAKEMATCH: one temporary for the sentinel and the third scan's count puts the -1 in r3 and the order copy before the count load. */
            tmp = -1;
            if (owner == tmp) {
                break;
            }
            order = -1;
            for (i = 0; i < pending->count; i++) {
                if (pending->entries[i].unit_id == owner && pending->entries[i].turns > order) {
                    order = pending->entries[i].turns;
                }
            }
            order++;
            if (order <= 1) {
                order = 2;
            }
            i = 0;
            tmp = pending->count;
            if (i < tmp) {
                do {
                    if (pending->entries[i].unit_id == owner && pending->entries[i].turns == -2) {
                        pending->entries[i].turns = order;
                        counts[pending->entries[i].element]++;
                        order++;
                    }
                    i++;
                } while (i < pending->count);
            }
        }
    }
    if (both_sides != 0) {
        struct BtlUnit *before;
        s32 most;
        s32 i;
        most = 0;
        before = Runtime_BumpAllocateAlternatePool(sizeof(struct BtlUnit));
        Iwram_CopyWords(before, unit, sizeof(struct BtlUnit));
        best = -1;
        for (i = 0; i <= 3; i++) {
            if (counts[i] > most) {
                most = counts[i];
                best = i;
            }
        }
        if (best >= 0 && unit->elm_lv[best] < most) {
            unit->elm_lv[best] = most;
        }
        UnitCalcFar(id);
        for (i = 0; i <= 3; i++) {
            order = unit->elm[i].pow - before->elm[i].pow;
            if (order > 0) {
                BattleEventRuntime_Reset();
                BattleEventRuntime_SchedulePhase(25);
                BtlEvPush(BEV_UNIT, id);
                BtlEvPush(BEV_NUM, order);
                BtlEvPush(BEV_SE, 175);
                MES_PUSH(MsgEarthPowerUp + i);
                BtlEvPush(BEV_OBJ_END, id);
                AudioCommand_PlayFar(212);
                ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 3);
                ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, 32);
                BattleFx_PlayUnitElementEffect(id, i, 2, most - 1);
                BattleEventRuntime_WaitForReady();
            }
        }
        MemFree(before);
    }
    if (both_sides != 0) {
        s8 *poison;
        BattleEventRuntime_Reset();
        if (plan->akuryo_dmg != 0) {
            BtlEvPush(BEV_OBJ_BGN, id);
            BtlEvPush(BEV_UNIT, id);
            BtlEvPush(BEV_NUM, plan->akuryo_dmg);
            MES_PUSH(MsgCurseDamage);
            if (Owner_AdjustFirstValueFar(id, -plan->akuryo_dmg) == 0) {
                s32 text;
                BtlEvPush(BEV_OBJ_RES, id);
                BtlEvPush(BEV_UNIT, id);
                if (id >= 0 && id <= 7) {
                    text = (s32)MsgGoesDown;
                } else {
                    text = (s32)MsgStrengthExhausted;
                }
                MES_PUSH(text);
            } else {
                BtlEvPush(BEV_OBJ_END, id);
            }
        }
        BattleEv_DispatchQueued();
        BattleEventRuntime_Reset();
        poison = &unit->doku;
        if (*poison != 0) {
            s32 damage = unit->max_hp * *poison / 10;
            struct BtlWork *state = BtlWk;
            BtlEvPush(BEV_OBJ_BGN, id);
            BtlEvPush(BEV_UNIT, id);
            BtlEvPush(BEV_NUM, damage);
            MES_PUSH(MsgPoisonDamage);
            if (*poison != 0) {
                state->events.pending_cue = SOUND_HEAVY_IMPACT;
            } else {
                state->events.pending_cue = 133;
            }
            if (Owner_AdjustFirstValueFar(id, -damage) == 0) {
                s32 text;
                BtlEvPush(BEV_OBJ_RES, id);
                BtlEvPush(BEV_UNIT, id);
                if (id >= 0 && id <= 7) {
                    text = (s32)MsgGoesDown;
                } else {
                    text = (s32)MsgStrengthExhausted;
                }
                MES_PUSH(text);
            } else {
                BtlEvPush(BEV_OBJ_END, id);
            }
        }
        BattleEv_DispatchQueued();
        BattleEventRuntime_Reset();
        if (unit->shiryo != 0) {
            if (--unit->shiryo == 0 && Owner_AdjustFirstValueFar(id, 0xc0000000) == 0) {
                BtlEvPush(BEV_UNIT, id);
                MES_PUSH(MsgReaperCalls);
                BtlEvPush(BEV_OBJ_BGN, id);
                BtlEvPush(BEV_OBJ_RES, id);
                BtlEvPush(BEV_UNIT, id);
                if (id >= 0 && id <= 7) {
                    MES_PUSH(MsgReaperCalls - 3);
                } else {
                    MES_PUSH(MsgReaperCalls + 3);
                }
            }
        }
        BattleEv_DispatchQueued();
    }
    UnitCalcFar(id);
}
