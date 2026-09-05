#include "types.h"
#include "battle_event.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct BattleEventRuntime {
    struct BattleEventQueue queue;
    u8 reserved_148[0x20];
    s32 pending_sound_id;
    u32 actor_auxiliary;
};

extern u8 *Data_03001e74;
extern u8 *Data_03001ee4;

void Func_080bb928(void *, u32);
void Func_080bb8e8(u32);
void Func_08015120(u32, u32);
void Func_080151c8(u32);
void Func_080bb65c(void);
void Func_08015118(void);
void Func_080f9010(u32);
void Func_080babdc(u32, u32, u32);
void Func_080c24f0(u32, u32);
void Func_080bb588(u32);
void Func_080bace8(u32);
void Func_08015130(u32);
u32 Func_080b7dd0(u32);
void Func_080b78e4(u32, u32);
void Func_080b7aac(u32);
u32 Func_080bdfec(void);

u32 BattleEvent_DispatchQueued(void)
{
    struct BattleEventRuntime *runtime = (void *)(Data_03001e74 + 0x6b8);
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < FIELD(queue, s32, 0x144); i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Func_080bb928(queue, queue->operands[i]); break;
        case 12: Func_080bb8e8(queue->operands[i]); break;
        case 0: Func_08015120(queue->operands[i], 1); break;
        case 1: Func_08015120(queue->operands[i], 5); break;
        case 2: Func_08015120(queue->operands[i] & 0x1ff, 2); break;
        case 3: Func_08015120(queue->operands[i] & 0x3fff, 4); break;
        case 6: FIELD(Data_03001ee4, s32, 8) = 1; break;
        case 7: Func_08015118(); break;
        case 4:
            if ((s32)queue->operands[i] >= 0) Func_080151c8(queue->operands[i]);
            Func_080bb65c();
            Func_08015118();
            break;
        case 5:
            if ((s32)queue->operands[i] >= 0) Func_080151c8(queue->operands[i]);
            Func_08015118();
            break;
        case 8:
            if (runtime->pending_sound_id > 0) Func_080f9010(runtime->pending_sound_id);
            Func_080babdc(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            u32 operand_offset = i * 4 + 64;
            u32 auxiliary = runtime->actor_auxiliary;
            Func_080c24f0(FIELD(queue, u32, operand_offset), auxiliary);
            Func_080bb588(FIELD(queue, u32, operand_offset));
            Func_080bace8(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: Func_08015130(Data_03001e74[65]); break;
        case 11:
            Func_080b78e4(queue->operands[i], Func_080b7dd0(queue->operands[i]));
            Func_080b7aac(queue->operands[i]);
            break;
        }
    }
    return Func_080bdfec();
}
