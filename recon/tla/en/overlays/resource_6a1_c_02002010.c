/* Draft, differ 2 of 81: one prologue instruction in a different place; the native loads gPartyState's address before spilling effect (str r2, [sp]), where this spills it first. In sched2 the spill and the load tie at priority 105 and the spill wins on insn order. Storing x before z took it from differ 61 to 2 (permute, 4 minutes). Tried: effect as u32, work declared before leader (differ 38), declarations apart from their calls, an early return, writing the marker through work, the effect through work (differ 33); permute (4 more minutes) found nothing below 2. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct JupiterRoom8Puzzle {
    s16 unk00;
    s16 flag;
};

struct JupiterRoom8EventWork {
    u8 unk000[0x1ac];
    s32 screenEffect;
    u8 unk1b0[0x1c];
    struct ObjectRuntime *marker;
};

extern struct JupiterRoom8Puzzle Data_02002bf8;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002188(void);
void Func_02001dc8(void);

void Func_02002010(s32 x, s32 z, s32 effect)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct JupiterRoom8EventWork *work = Ram_HeapSlots->eventWork;
    s32 dx = GameFlag_GetByte(Data_02002bf8.flag);
    s32 dz = GameFlag_GetByte(Data_02002bf8.flag + 8);

    if (dx != 0 || dz != 0) {
        struct ObjectRuntime *marker;

        GameFlag_SetByte(Data_02002bf8.flag, 0);
        GameFlag_SetByte(Data_02002bf8.flag + 8, 0);
        leader->x = ((dx + x) << 20) + 0x80000;
        leader->z = ((dz + z) << 20) + 0x80000;
        marker = work->marker;
        marker->x = leader->x;
        marker->z = leader->z;
        Func_02002188();
        Func_02001dc8();
        ((struct JupiterRoom8EventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
    }
}
