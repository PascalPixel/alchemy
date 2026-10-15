/* Draft, differ 2 of 46: the native loads the state pointer (ldr r3, =Data)
 * right after the direction call, before movs r1, #192; this draft loads it two
 * instructions later. Tried: the state assigned before or after the call (6),
 * a u16 direction (9), the mode as a local (8), asm barriers and a one-pass
 * block after the state (6 to 16), a minute of alchemy permute; the state
 * through an asm operand alone or with the direction (16), pinned to r8 (2).
 * sched2 ranks the mask's movs (priority 9) above the state's load (8, its
 * chain ends in an anti dependence of cost 0). Also measured: the state
 * pinned to r3 (2; with a one-pass block or empty asm before it, the load
 * moves ahead of movs but the direction's copy too: 4), the call's result
 * pinned to r0 (4), four minutes of alchemy permute on the r3 form (5). */
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/object_runtime.h"

extern u8 Data_020023c4[];
struct ObjectRuntime *Object_GetById(u32 id);
s32 Func_02004904(void);
void Func_02004d9c(struct ObjectRuntime *object, s32 mode);
void Func_02000a60(struct ObjectRuntime *object);
void Func_02000aac(struct ObjectRuntime *object);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);

void Func_02000af8(s32 unused, u32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    u32 direction = Func_02004904();
    s32 *state = (s32 *)(Data_020023c4 + 0x288);

    object->collision_flags = 0;
    Func_02004d9c(object, ((direction & 0xc000) >> 14) + 2);
    if (direction & 0x4000)
        object->update = (void (*)(void))Func_02000aac;
    else
        object->update = (void (*)(void))Func_02000a60;
    object->collision_flags = 1;
    GameFlag_SetBit(*state + 0xa50);
    GameFlag_SetBit(0x200);
    WaitFrames(20);
}
