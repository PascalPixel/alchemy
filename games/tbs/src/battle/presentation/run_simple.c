#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "btlev.h"
#include "scene.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "battle_command.h"
#include "btlsys.h"
#include "system.h"
#include "btlmes.h"
#include "btldisp.h"
#include "motobj.h"
#include "animspr.h"
#include "btlwork.h"
#include "objdisp.h"
#include "inventory.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "btlobj.h"
#include "field_event.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct MotionObject*,s32))ObjectDispatch_ApplyValueToChildrenFar)
#define BattleFx_DispatchByIdRangeFar ((void (*)(struct BattlePresentationWork*))BattleFx_DispatchByIdRangeFar)
#define BattleFx_DispatchModeFar ((void (*)(struct BattlePresentationWork*))BattleFx_DispatchModeFar)
#define BattleActor_SpawnObjectsForList ((s32 (*)(void*,s32))BattleActor_SpawnObjectsForList)

s32 ArcTan2(s32 first, s32 second);

s32 RunBattlePresentation(struct BtlPlan *plan, s32 mode)
{
    struct BattlePresentationWork work;
    struct MotionObject *object;
    struct AnimationObject *animation;
    struct BtlUnit *unit;
    s16 position[3];
    s32 scripted;
    struct BattlePresentationTransition *transition;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 x;
    s32 z;
    s32 divisor;
    s32 direct;
    s32 index;
    s32 phase;

    /* FAKEMATCH: r0 forgets the plan, so the first read reloads it */
    asm volatile("" : : : "r0");
    transition = gTransitionWork;
    object = BtlObjGet(plan->aid)->object;
    z = object->z;
    x = object->x;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - 0x2000;
    if (plan->aid > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (transition->target_yaw == facing_angle) {
        transition->target_yaw = facing_angle;
        WaitFrames(5);
    } else {
        transition->target_yaw = facing_angle;
        WaitFrames(10);
    }

    BattlePres_SetActorModes(0, 0);
    BattlePres_BuildTargetList(plan, &work);
    if (work.kind == 0x87)
        PtyStDrawFar(BtlWk->pty_st & ~1);

    unit = UnitGetFar(work.actor);
    UnitGetFar(work.actors[0]);
    scripted = plan->tgt_crit[0];
    direct = plan->tgt_adj[0] == 0;

    animation = GetMotionRecord(
        BtlObjGet(plan->aid)->object, 0);
    divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
    BattleMotion_ApproachTarget(
        work.actor,
        work.actors[0],
        divisor,
        Battle_GetSpriteFlagHighBits(unit->class_id) << 16);
    ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(work.actor)->object, 16);
    BtlObjGet(work.actors[0]);

    if ((u16)work.actors[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    REG_WIN0H = 0x00f0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0x00f0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    REG_DISPCNT |= 0x6000;

    if (direct != 0) {
        WaitFrames(10);
        BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        BtlEvPush(0, plan->tgt_no[0]);
        BtlEvPush(4, (s32)MsgNimblyDodgesBlow);
        BattleEv_DispatchQueued();
        ObjMotRst(work.actors[0]);
    } else {
        phase = 0;
        work.flags = 0;
        if (plan->presentation_flags != 0)
            work.flags = 1;

        if (scripted != 0) {
            work.kind += 200;
            phase = 1;
            transition->flag = 1;
            position[0] = work.actor;
            position[1] = work.first_target;
            position[2] = 0xff;
            BattleActor_SpawnObjectsForList(position, 0);
        }

        divisor -= 8;
        if (divisor <= 0)
            divisor = 1;
        {
            s32 first;
            s32 second;

            for (index = 0; index != divisor; index++) {
                if (phase != 0) {
                    first = work.actor;
                    second = work.first_target;
                    BattlePres_SetupTransitionAtPairMidpoint(
                        first,
                        second,
                        index * 30 / divisor + 100);
                }
                WaitFrames(1);
            }
        }

        Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
        if (work.kind != 0) {
            if (plan->presentation_flags & 0x4000)
                BattleFx_DispatchByIdRangeFar(&work);
            else
                BattleFx_DispatchModeFar(&work);
        }
        BattleEventRuntime_WaitForReady();
        if (scripted != 0) {
            transition->flag = 0;
            BattleParty_ListAllUnitsAndSubmit();
            BattlePres_SetupTransitionScene(0, 0, 0, 100);
        }
        ObjMotRst(work.actors[0]);
    }
    ObjMotRst(work.actor);
    return 0;
}

s32 BattlePres_RunSimple(struct BtlPlan *input, s32 flags)
{
    struct BattlePresentationWork work;
    struct BtlPlan *saved_input;
    struct MotionObject *object;
    struct AnimationObject *animation;
    struct BattlePresentationTransition *transition;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 x;
    s32 z;
    s32 divisor;
    s32 scripted;

    transition = gTransitionWork;
    saved_input = input;
    object = BtlObjGet(saved_input->aid)->object;
    z = object->z;
    x = object->x;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - 0x2000;
    if (saved_input->aid > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (transition->target_yaw == facing_angle) {
        transition->target_yaw = facing_angle;
        WaitFrames(5);
    } else {
        transition->target_yaw = facing_angle;
        WaitFrames(20);
    }

    BattlePres_SetActorModes(0, 0);
    BattlePres_BuildTargetList(saved_input, &work);
    UnitGetFar(work.actor);
    UnitGetFar(saved_input->tgt_no[0]);

    scripted = flags & 2;
    animation = GetMotionRecord(
        BtlObjGet(saved_input->aid)->object, 0);
    divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
    BattleMotion_ApproachTarget(
        work.actor,
        saved_input->tgt_no[0],
        divisor,
        0);
    ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(work.actor)->object, 16);
    BtlObjGet(saved_input->tgt_no[0]);

    if (saved_input->tgt_no[0] <= 7)
        work.side = 1;
    else
        work.side = 0;
    if (scripted != 0) {
        WaitFrames(10);
        BattleMotion_ResetObjectAtScaledAnchor(saved_input->tgt_no[0]);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        ObjMotRst(saved_input->tgt_no[0]);
    } else {
        BattleFx_DispatchByIdRangeFar(&work);
        BattleEv_DispatchQueued();
        ObjMotRst(saved_input->tgt_no[0]);
    }
    ObjMotRst(work.actor);
    return 0;
}

s32 BattlePres_RunItem(struct BtlPlan *plan, s32 unused,
                        struct BattleActionRecord *selection)
{
    struct BattlePresentationWork work;
    struct MotionObject *object;
    s32 i;
    s32 item;
    struct BtlUnit *unit;
    struct ItemDefinition *def;
    struct BattlePresentationTransition *transition = gTransitionWork;
    s32 facing;

    if (plan->aid > 4)
        facing = -0x2000;
    else
        facing = 0x2000;
    if (transition->target_yaw != facing)
        transition->target_yaw = facing;
    BattlePres_BuildTargetList(plan, &work);
    BattlePres_SetActorModes(0, 0);
    object = BtlObjGet(work.actor)->object;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    if (plan->tgt_no[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    for (i = 0; i != work.count; i++) {
        struct AnimationObject *animation = GetMotionRecord(
            BtlObjGet(work.actors[i])->object, 0);
        s32 j;

        for (j = 0; j != animation->count - 1; j++)
            work.params[i][j] = animation->entries[j]->param;
    }

    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (work.kind != 0) {
        s32 fade;

        i = 0;
        fade = 0;
        do {
            struct BtlWork *battle = BtlWk;

            if (i < 20) {
                u16 *palette = battle->palette;
                s32 value = 0x10000 - fade;

                battle->brightness = value;
                Graphics_ScaleRgb555Clamped(palette, (u16 *)(BG_PLTT + 6 * 16), value, 128);
            }
            WaitFrames(1);
            i++;
            fade += 0x444;
        } while (i < 20);
        if (plan->presentation_flags & 0x4000)
            BattleFx_DispatchByIdRangeFar(&work);
        else
            BattleFx_DispatchModeFar(&work);
    } else {
        WaitFrames(60);
    }
    BattleEventRuntime_WaitForReady();
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    for (i = 0; i != work.count; i++)
        ObjMotRst(work.actors[i]);

    unit = UnitGetFar(selection->unit_id);
    item = unit->inventory[selection->parameter];
    def = Item_GetDirectFar(item);
    if (def->use_type == 1) {
        s32 result = Inventory_RemoveFar(selection->unit_id, selection->parameter);
        s32 index = selection->parameter;
        if (result == 2) {
            struct BtlWork *battle = BtlWk;
            u32 row;

            for (row = 0; row < 20; row++) {
                if (battle->actions[row].command == 2
                    && battle->actions[row].unit_id == selection->unit_id) {
                    if (battle->actions[row].parameter == index)
                        battle->actions[row].parameter = -1;
                    else if (battle->actions[row].parameter > index)
                        battle->actions[row].parameter--;
                }
            }
        }
    } else if (def->use_type == 2) {
        if ((BtlRndFar() & 7) == 0) {
            BtlEvPush(BEV_ITEM, unit->inventory[selection->parameter]);
            BtlEvPush(BEV_MES, (s32)MsgUsedItemBroke);
            Inventory_BreakFar(selection->unit_id, selection->parameter);
            BattleEv_DispatchQueued();
        }
    } else if (def->use_type == 4) {
        if ((item & ITEM_ID_MASK) == 0xb8)
            item = 0xb9;
        unit->inventory[selection->parameter] = item;
    }
    return 0;
}
