/* Draft of Func_02000918 (overlay 64B, 51 instructions), differ 2: the exit back to
 * the map, saving actor 8's x and z in the party state. Written from the listing; a
 * one-pass block after Func_02004db8(0) keeps gPartyState's load ahead of 530 as in
 * the native (without it, differ 4). Left: after storing x the native starts 636
 * (lsls r1) before it loads z; here z is loaded first (sched2: the z load's store
 * dependence has latency 1, and its priority is higher).
 * Measured: the saved position as struct fields (no change). */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_ClearBit(s32 flag);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200050c(void);
void Func_02004c20(void);
void Func_02004d40(s32 value);
void Func_02004db8(s32 value);
void Func_02004e30(s32 cue);

void Func_02000918(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *leader = Object_GetById(8);

    Func_02004c20();
    Func_02004db8(0);
    do {
    } while (0);
    if (gPartyState.render_mode == 8)
        Func_0200050c();
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    *(s32 *)((u8 *)&gPartyState + 632) = leader->x;
    *(s32 *)((u8 *)&gPartyState + 636) = leader->z;
    *(s32 *)((u8 *)&gPartyState + 640) = 0;
    gPartyState.owner_override = 0;
    GameFlag_ClearBit(0x176);
    Func_02004e30(123);
    Func_02004d40(*(s16 *)(work + 340));
}
