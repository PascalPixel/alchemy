#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_event.h"
#include "../../../include/types.h"
#include "../../../include/battle_actor.h"
#include "../../../include/battle_calc.h"
#include "../../../include/battle_command.h"
#include "../../../include/battle_efx.h"
#include "../../../include/battle_event.h"
#include "../../../include/battle_msg.h"
#include "../../../include/battle_runtime.h"
#include "../../../include/battle_summon.h"
#include "../../../include/battle_types.h"
#include "../../../include/battle_work.h"
#include "../../../include/runtime_1e74.h"
#include "../../../include/runtime_mem.h"

/* battle/ev_set_runtime_field8.c */
/* battle/event_runtime/set_runtime_field8.c */
void BattleEv_SetRuntimeField8(void)
{
    FIELD_AT_OFFSET(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}

/* battle/actor/destroy_temporary_object.c */
struct Creature_080bb8e8 {
    u8 padding_000[0x12a];
    u8 field_12a;
};

struct Runtime_080bb8e8 {
    s32 field_00;
    u8 padding_004[0x24];
    s16 field_28;
};

s32 Object_Destroy(s32);
struct Creature_080bb8e8 *Runtime_GetObject();

s32 ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct Creature_080bb8e8 *creature;
    struct Runtime_080bb8e8 *runtime;

    creature = Runtime_GetObject();
    if (creature->field_12a == 1) {
        Actor_Apply(creature, 0);
        Actor_Check(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = (struct Runtime_080bb8e8 *)Actor_GetObject(arg0);
        result = Object_Destroy(runtime->field_00);
        runtime->field_00 = 0;
        runtime->field_28 = 0;
        return result;
    }
    return (s32)creature;
}

/* battle/event_runtime/set_runtime_flag_bit0.c */
struct Runtime_080bb928 {
    u8 padding_000[0x16c];
    s32 flags;
};

void Battle_SetRuntimeFlagBit0(struct Runtime_080bb928 *runtime, s32 unused)
{
    runtime->flags |= 1;
}

/* battle/event_runtime/dispatch_queued_events.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct BattleEventRuntime {
    struct BattleEventQueue queue;
    u8 reserved_148[0x20];
    s32 pending_sound_id;
    u32 actor_auxiliary;
};

extern u8 *gIw;

extern s32 HitFalloff[];
extern s32 PpLossFalloff[];
extern s32 HpHealFalloff[];
extern s32 PpDmgFalloff[];
extern s32 HpDmgFalloff5[];
extern s32 HpDmgFalloff8[];
extern s32 HpDmgFalloff6[];
extern s32 PpHealFalloff[];
extern s32 HpDmgFalloff[];

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventRuntime *runtime = (void *)((u8 *)gBattleWork + 0x6b8);
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < FIELD(queue, s32, 0x144); i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Battle_Apply(queue, queue->operands[i]); break;
        case 12: BattleActor_DestroyTemporaryObject(queue->operands[i]); break;
        case 0: UiText_DrawQuantity(queue->operands[i], 1); break;
        case 1: UiText_DrawQuantity(queue->operands[i], 5); break;
        case 2: UiText_DrawQuantity(queue->operands[i] & 0x1ff, 2); break;
        case 3: UiText_DrawQuantity(queue->operands[i] & 0x3fff, 4); break;
        case 6: FIELD(gIw, s32, 8) = 1; break;
        case 7: Battle_Run(); break;
        case 4:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWait(queue->operands[i]);
            BattlePres_WaitForAdvance();
            Battle_Run();
            break;
        case 5:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWait(queue->operands[i]);
            Battle_Run();
            break;
        case 8:
            if (runtime->pending_sound_id > 0) Audio_PlayCue(runtime->pending_sound_id);
            BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            u32 operand_offset = i * 4 + 64;
            u32 auxiliary = runtime->actor_auxiliary;
            Battle_Apply3(FIELD(queue, u32, operand_offset), auxiliary);
            BattleActor_ResetRuntimeFields(FIELD(queue, u32, operand_offset));
            BattleMotion_InitializeActorRecords(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: Sys_SetMode(((u8 *)gBattleWork)[65]); break;
        case 11:
            Battle_Apply4(queue->operands[i], Actor_GetObject(queue->operands[i]));
            BattlePres_SetActorModeAndAction(queue->operands[i]);
            break;
        }
    }
    return BattleEventRuntime_Reset();
}

/* battle/event/push.c */
u32 BattleEv_Push(u32 opcode, u32 operand)
{
    u8 *base = *(u8 **)ADDR_03001E74;
    struct BattleEventQueue *queue =
        (struct BattleEventQueue *)(base + 1720);
    u32 *count = (u32 *)&queue->count;
    u32 index = *count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    *count = index + 1;
    return opcode;
}
