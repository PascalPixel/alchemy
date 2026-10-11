/* Draft, differ 2 of 21: the native loads event_work before `movs r2, #192`
 * of the 0xca2 offset; the draft builds the constant's first byte before
 * the load. A struct field, an s16 index and a local pointer give the same. */
#include "types.h"
#include "ram_buffer.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02002118(void);
void Func_02002148(void);

static __inline__ void Flag_Set(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

void Func_0200173c(void)
{
    if (*(s16 *)((u8 *)Ram_HeapSlots->event_work + 0xca2) != 0) {
        Flag_Set(0x106);
        Func_02002118();
        Func_02002148();
        Flag_Clear(0x106);
    }
}
