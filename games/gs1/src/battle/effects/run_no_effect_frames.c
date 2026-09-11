#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_runtime.h"
#include "battle_types.h"
#include "b5_context.h"

/* battle/effects/fx_run_no_effect_frames.c */
/* battle/effects/common/run_no_effect_frames.c */
void BattleFx_RunNoEffectFrames(s32 frame_count)
{
    s32 frame;

    frame = 0;
    if (frame_count != 0) {
        do {
            frame += 1;
            Battle_Run();
        } while (frame != frame_count);
    }
}

/* battle/effects/runtime/init/initialize_mode.c */
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

void BattleFx_RenderAnimationMode0(s32 arg0);

void BattleFx_InitializeMode(s32 *arg0)
{
    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    if (*arg0 == 0) {
        Battle_Do(arg0);
    } else {
        switch (*arg0) {
        case 1:
            Battle_unk2_2(arg0);
            break;
        case 2:
            Battle_unk3_2(arg0);
            break;
        case 3:
            Battle_unk4_2(arg0);
            break;
        case 4:
            BattleFx_RunStagedParticles(arg0);
            break;
        case 5:
            Battle_unk6(arg0);
            break;
        case 6:
            FunctionHead(arg0);
            break;
        case 7:
            BattleFx_RenderAnimationMode0(arg0);
            break;
        case 8:
            Battle_unk8(arg0);
            break;
        case 9:
            Battle_unk9(arg0);
            break;
        case 10:
            FunctionHead(arg0);
            break;
        case 11:
            Battle_Do(arg0);
            break;
        case 12:
            FunctionHead(arg0);
            break;
        }
    }

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}

/* battle/effects/runtime/misc/dispatch_mode.c */
typedef void (*Callback)(s32 *);

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern Callback BattleFx_CallbackTable[];

void BattleFx_DispatchMode(s32 *state)
{
    s32 index;
    s32 **destination;

    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    destination = (s32 **)(*(s32 *)ADDR_03001EEC + 0x7828);
    index = state[0];
    *destination = state;
    if (index == 0)
        state[6] = 0;
    else
        BattleFx_CallbackTable[index - 1](state);

    Battle_Do(40);
    Battle_Do(39);
    Battle_Do(41);
}

/* battle/effects/calc/select_living_targets.c */
struct BattleEffectTargetArgument {
    u8 reserved_00[36];
    s16 unit_id;
};

void BattleFx_SelectLivingTargets(struct BattleEffectTargetArgument *argument)
{
    s16 targets[14];
    s32 count;
    s32 i;

    count = 0;
    if (argument->unit_id > 127) {
        for (i = 0; i != 6; i++) {
            s32 unit = i + 128;

            if (BattleUnit_Get(unit)->hp > 0)
                targets[count++] = unit;
        }
    } else {
        for (i = 0; i != 8; i++) {
            if (BattleUnit_Get(i)->hp > 0)
                targets[count++] = i;
        }
    }
    targets[count] = 0xff;
    Battle_Apply(targets, 0);
}

/* object/group/update_members.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define SET_OWNER(set) FIELD(set, void *, 0x00)
#define SET_EXCLUDED_20(set) FIELD(set, void *, 0x20)
#define SET_EXCLUDED_24(set) FIELD(set, void *, 0x24)
#define SET_SUPPRESS_UPDATES(set) FIELD(set, s16, 0x2a)
#define GROUP_OBJECT_COUNT(group) FIELD(group, u8, 0x27)
#define OBJECT_VALUE(object) FIELD(object, u8, 0x05)
#define OBJECT_REFRESH(object) FIELD(object, u8, 0x16)

void *Obj_Run(void *, s32);

void Object_InitializeMode(void *, s32);
extern u32 gIw;

void ObjectGroup_UpdateMembers(
    s32 set_id,
    s32 object_value,
    s32 group_value,
    s32 state_slot,
    s32 state_value)
{
    struct B5Context *set;
    void *group;
    u8 *state;
    s32 group_index;

    set = Obj_unk2_2(set_id);
    state = (u8 *)gIw;
    group_index = 0;

    while ((group = Obj_Run(SET_OWNER(set), group_index)) != NULL) {
        if (state_slot != -1) {
            s32 state_offset = state_slot + 0x7818;

            state[state_offset] = ((u8 *)&state_value)[0];
        }

        if (SET_SUPPRESS_UPDATES(set) == 0) {
            if (object_value != -1) {
                s32 object_index;

                object_index = 0;
                if (GROUP_OBJECT_COUNT(group)!= 0) {
                    void **objects;

                    objects = (void **)((u8 *)group + 0x28);
                    do {
                        void *object;

                        object = *objects++;
                        if (object != NULL
                            && object != SET_EXCLUDED_24(set)
                            && object != SET_EXCLUDED_20(set)) {
                            if (object_value == 0)
                                OBJECT_VALUE(object) = Obj_Check(set_id);
                            else
                                OBJECT_VALUE(object) = object_value;
                            OBJECT_REFRESH(object) = 0xff;
                        }
                        object_index++;
                    } while (object_index != GROUP_OBJECT_COUNT(group));
                }
            }

            if (group_value != -1)
                Object_InitializeMode(group, group_value);
        }
        group_index++;
    }
}

/* battle/effects/misc/init_tilemap_and_flush_queue.c */
void BattleFx_InitTilemapAndFlushQueue(void)
{
    Battle_Check(1);
    Battle_unk2();
}
