#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_runtime.h"
#include "battle_types.h"

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
            Battle_unk5(arg0);
            break;
        case 5:
            Battle_unk6(arg0);
            break;
        case 6:
            Battle_unk7(arg0);
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
            Battle_unk10(arg0);
            break;
        case 11:
            Battle_Do(arg0);
            break;
        case 12:
            Battle_unk11(arg0);
            break;
        }
    }

    Battle_unk12(40);
    Battle_unk12(39);
    Battle_unk12(41);
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
