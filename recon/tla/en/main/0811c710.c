#include "TYPES.H"
#include "SYSTEM.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PRESENTATION.H"
#include "MOTION_OBJECT.H"

extern u8 gTransitionWork[];

struct TransitionContext {
    s32 kind;
    s32 side;
    s32 actor;
    u8 unknown_0c[8];
    s32 count;
    u8 unknown_18[12];
    s16 actors[14];
    u8 unknown_40[20];
};

void Object_SetMode(struct MotionObject *object, s32 mode);

s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action)
{
    u16 visible_units[14];
    struct TransitionContext context;
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
            *(struct BattlePresentationTransition **)gTransitionWork;
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
