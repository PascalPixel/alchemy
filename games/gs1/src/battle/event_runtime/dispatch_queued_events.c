#include "types.h"
#include "scene.h"
#include "abi/battle/event_runtime/dispatch_queued_events.h"
#include "battle_event.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct BattleEventRuntime {
    struct BattleEventQueue queue;
    u8 reserved_148[0x20];
    s32 pending_sound_id;
    u32 actor_auxiliary;
};

extern u8 *gBattleWork;
extern u8 *gIw;

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventRuntime *runtime = (void *)(gBattleWork + 0x6b8);
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < FIELD(queue, s32, 0x144); i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Battle_Apply(queue, queue->operands[i]); break;
        case 12: Battle_Do(queue->operands[i]); break;
        case 0: Battle_Apply2(queue->operands[i], 1); break;
        case 1: Battle_Apply2(queue->operands[i], 5); break;
        case 2: Battle_Apply2(queue->operands[i] & 0x1ff, 2); break;
        case 3: Battle_Apply2(queue->operands[i] & 0x3fff, 4); break;
        case 6: FIELD(gIw, s32, 8) = 1; break;
        case 7: Battle_Run(); break;
        case 4:
            if ((s32)queue->operands[i] >= 0) Battle_Do2(queue->operands[i]);
            Battle_Run2();
            Battle_Run();
            break;
        case 5:
            if ((s32)queue->operands[i] >= 0) Battle_Do2(queue->operands[i]);
            Battle_Run();
            break;
        case 8:
            if (runtime->pending_sound_id > 0) Battle_Do3(runtime->pending_sound_id);
            Battle_Place(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            u32 operand_offset = i * 4 + 64;
            u32 auxiliary = runtime->actor_auxiliary;
            Battle_Apply3(FIELD(queue, u32, operand_offset), auxiliary);
            Battle_Do4(FIELD(queue, u32, operand_offset));
            Battle_Do5(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: Battle_Do6(gBattleWork[65]); break;
        case 11:
            Battle_Apply4(queue->operands[i], Battle_Do7(queue->operands[i]));
            Battle_Do8(queue->operands[i]);
            break;
        }
    }
    return Battle_Run3();
}
