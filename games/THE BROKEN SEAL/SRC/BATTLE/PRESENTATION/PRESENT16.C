#include "TYPES.H"
#include "HEAP_STATE.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_MSG.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TARGET.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"
#include "BATTLE_RUNTIME.H"
#include "DMA.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_MOTION.H"
#include "BATTLE_COMMAND.H"

void UiWork_ClearValueNameTablesFar(void);
s32 BattlePres_ShowMessageWhenField38Positive(const struct BattleActionRecord *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

/* battle/presentation/act/run.c */
extern struct BattlePresentationTransition *gTransitionWork;
void UiText_ShowMessageAndWaitCoreFar(s32);
void UiWork_ResetFreeChannelFar(void);

/* battle/presentation/misc/msg_field38.c */
void UiWork_PushValueSlotFar(s32, s32);

void Object_SetMode(struct MotionObject *object, s32 mode);

s32 BattleObject_IsValidId(u32);

void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);
void Actor_ResetMotionAtAnchor(s32);
void BattleFx_DispatchByIdRangeFar(struct BattlePresentationWork *);

/* Build the action order from agility: party members start with target
   priority 0x80, enemies with a random party target. Sort by descending
   agility, moving the complete command records through DMA. */
s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleActionRecord *entries)
{
    struct BattleActionRecord swap;
    u16 unit_ids[14];
    s32 first_count;
    s32 count = 0;
    s32 second_count;
    s32 priority_range;
    s32 index;
    struct BattleActionRecord *entry;
    struct BattleUnit *unit;

    first_count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unit_ids);
    for (index = 0; index != 4; index++)
        Owner_GetStateFar(index);
    for (index = 0; index < first_count; index++) {
        s32 unit_id = unit_ids[index];

        unit = Owner_GetStateFar(unit_id);
        entry = &entries[index];
        entry->unit_id = unit_id;
        entry->priority = unit->agility;
        entry->command = 0;
        entry->parameter = 0;
        entry->target = 0x80;
        count++;
    }
    second_count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unit_ids);
    entry = &entries[first_count];
    priority_range = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
    for (index = 0; index < second_count; index++) {
        s32 unit_id = unit_ids[index];

        unit = Owner_GetStateFar(unit_id);
        entry->unit_id = unit_id;
        entry->priority = unit->agility >> 1;
        if (entry->priority != 0)
            entry->priority += (u32)(Random16() * unit->agility) >> 16;
        entry->command = 0;
        entry->parameter = 0;
        entry->target = (u32)(Random16() * priority_range) >> 16;
        count++;
        entry++;
    }
    for (index = count - 2; index > 0; index--) {
        s32 swaps = 0;
        s32 pos;

        for (pos = count - 1; pos > 0; pos--) {
            if ((s16)entries[pos].priority > (s16)entries[pos - 1].priority) {
                Dma_Set(&entries[pos], &swap, 0x84000004, (volatile u32 *)0x040000d4);
                Dma_Set(&entries[pos - 1], &entries[pos], 0x84000004, (volatile u32 *)0x040000d4);
                Dma_Set(&swap, &entries[pos - 1], 0x84000004, (volatile u32 *)0x040000d4);
                swaps++;
            }
        }
        if (swaps == 0)
            break;
    }
    return count;
}

void BattlePres_AdjustCameraByShoulderKeys(void)
{
    /* FAKEMATCH: separate cell loads change the literal pool and register
       order. The existing walk reaches slot 44 from camera slot 12. */
    void **slot = (void **)&gCameraWork;
    struct BattleCamera *cam = slot[0];
    struct BattlePresentationTransition *trans = slot[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
    volatile u32 *keys = (volatile u32 *)gKeysHeld;

    if ((*keys & 512) != 0) {
        cam->yaw += 512;
    }
    if ((*keys & 256) != 0) {
        cam->yaw -= 512;
    }
    if (trans->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

s32 BattlePres_RunAction(struct BattleActionRecord *action)
{
    struct BattlePresentationTransition *transition;
    s32 actor_id;
    s32 battle_mode;
    struct BattleUnit *actor;

    actor_id = action->unit_id;
    actor = Owner_GetStateFar(actor_id);
    if (actor->hp == 0)
        return -1;

    action->target = BattleTarget_ReplaceDefeated(action);
    transition = gTransitionWork;
    if (action->unit_id > 4)
        battle_mode = -0x2000;
    else
        battle_mode = 0x2000;
    transition->target_yaw = battle_mode;
    transition->frames = 60;
    UiWork_ClearValueNameTablesFar();

    switch (action->command) {
    case 99:
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartyFlees);
        if (BattleEscape_PlayRun(action)!= 0)
            return 1;
        break;
    case 3:
        WaitFrames(45);
        BattlePres_ShowMessageWhenField38Positive(action);
        break;
    case 2:
        WaitFrames(45);
        BattlePres_RunUnitAction((s16 *)action);
        break;
    case 0:
    default: {
        struct BattlePresentationTransition *tr = gTransitionWork;
        tr->flag = 0;
        BattlePres_RunUnitAction((s16 *)action);
        tr->flag = 0;
        break;
    }
    case 1:
        BattlePresentation_RunPairedUnitTransition(action);
        break;
    }

    UiWork_ResetFreeChannelFar();
    return 0;
}

/* battle/object/is_valid_id.c */
s32 BattleObject_IsValidId(u32 object_id)
{
    if (object_id <= 7) {
        return 0;
    }
    if (object_id - 128 <= 5) {
        return 0;
    }
    return -1;
}

/* battle/escape/play_run.c */
s32 BattleEscape_PlayRun(struct BattleActionRecord *action)
{
    s16 party_members[14];
    s32 party_size;
    s32 animated;
    s32 member_slot;

    (void)action;
    if (((u32)(Random16() << 4) >> 16) != 0) {
        party_size = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            party_members);
        animated = 0;
        if (party_size != 0) {
            member_slot = 0;
            do {
                BattleMotion_SetupEscapeObject(party_members[member_slot]);
                animated++;
                WaitFrames(8);
                member_slot++;
            } while (animated != party_size);
        }
        WaitFrames(22);
        return 1;
    }
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgCannotEscape);
    return 0;
}

s32 BattlePres_ShowMessageWhenField38Positive(const struct BattleActionRecord *action)
{
    s32 object_id;
    s32 result;
    struct BattleUnit *unit;

    object_id = action->unit_id;
    unit = Owner_GetStateFar(object_id);
    if (BattleObject_IsValidId(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (unit->hp <= 0) {
        return result;
    }
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(object_id, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
    return 0;
}

s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action)
{
    u16 visible_units[14];
    struct BattlePresentationWork context;
    s32 actor_id;
    s32 target_id;
    u32 side_start;
    u32 side_count;
    u32 living_count;
    struct MotionObject *actor_record;
    u32 index;

    living_count = 0;
    actor_id = action->unit_id;
    if (BattleObject_IsValidId(actor_id) < 0) {
        return -1;
    }
    target_id = action->target;
    if (BattleObject_IsValidId(target_id) < 0) {
        return -1;
    }

    {
        struct BattlePresentationTransition *transition =
            gTransitionWork;
        transition->target_yaw = action->unit_id > 4 ? 0x5000 : 0x2000;
        transition->frames = 60;
    }
    WaitFrames(10);
    Random16();
    actor_record = GetBattleObjectSlot(actor_id)->object;

    if ((u32)target_id <= 7) {
        side_count = BattleParty_ListLivingUnits(2, visible_units);
        side_start = 0x80;
    } else {
        side_count = BattleParty_ListLivingUnits(1, visible_units);
        side_start = 0;
    }

    for (index = 0; index != side_count; index++) {
        if (index + side_start == actor_id) {
            Object_SetMode(actor_record, 3);
        }
    }

    WaitFrames(30);
    *(u16 *)0x04000050 = 0x3f40;
    for (index = 0; index != side_count; index++) {
        BattlePres_SetActorRecordMode(index + side_start, 1);
    }
    for (index = 0; index != 16; index++) {
        *(u16 *)0x04000052 = (16 - index) | 0x1000;
        WaitFrames(1);
    }
    UiWindow_DrawPartyStatusContentsFar(9);

    if (target_id > 0x7f) {
        u32 count = BattleParty_ListLivingUnits(2, visible_units);
        for (index = 0; living_count != count; index++) {
            u32 unit = index + 0x80;
            if (Owner_GetStateFar(unit)->hp > 0) {
                visible_units[living_count++] = unit;
            }
        }
    } else {
        u32 count = BattleParty_ListLivingUnits(1, visible_units);
        for (index = 0; living_count != count; index++) {
            if (Owner_GetStateFar(index)->hp > 0) {
                visible_units[living_count++] = index;
            }
        }
    }
    visible_units[living_count] = 0xff;
    BattleActor_SpawnObjectsForList(visible_units, 0);

    context.kind = action->parameter;
    context.actor = actor_id;
    for (index = 0; index != living_count; index++) {
        context.actors[index] = visible_units[index];
    }
    context.count = living_count;
    if ((u32)target_id <= 7) {
        context.side = 1;
    } else {
        context.side = 0;
    }
    BattleFx_InitializeModeFar((s32 *)&context);

    WaitFrames(10);
    BattleActor_CommitPlacement();
    *(u16 *)0x04000050 = 0x3f40;
    for (index = 0; index != side_count; index++) {
        BattlePres_SetActorRecordMode(index + side_start, 1);
    }
    for (index = 0; index != 16; index++) {
        *(u16 *)0x04000052 = index | 0x1000;
        WaitFrames(1);
    }
    for (index = 0; index != side_count; index++) {
        BattlePres_SetActorRecordMode(index + side_start, 0);
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
    WaitFrames(3);
    return 0;
}

s32 BattlePres_RunApproachAction(struct BattleActionRecord *input)
{
    struct BattlePresentationWork work;

    if (gTransitionWork->target_yaw == 0x2000) {
        gTransitionWork->target_yaw = 0x2000;
        WaitFrames(10);
    } else {
        gTransitionWork->target_yaw = 0x2000;
        WaitFrames(30);
    }

    work.actor = input->unit_id;
    if (BattleObject_IsValidId(work.actor) < 0)
        return -1;

    work.actors[0] = ((u16)input->target);
    if (BattleObject_IsValidId(work.actors[0]) < 0)
        return -1;

    Owner_GetStateFar(work.actor);
    Owner_GetStateFar(work.actors[0]);
    Random16();
    UiWork_PushValueSlotFar(work.actor, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorAttacks);
    BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
    ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(work.actor)->object, 16);
    GetBattleObjectSlot(work.actors[0]);

    work.count = 1;
    if ((u16)work.actors[0] <= 7)
        work.side = 1;
    else
        work.side = 0;
    work.flags = 0;

    WaitFrames(4);
    BattleFx_DispatchByIdRangeFar(&work);
    Actor_ResetMotionAtAnchor(work.actors[0]);
    Actor_ResetMotionAtAnchor(work.actor);
    return 0;
}
