/* Near miss of Func_02002d90 (21 instructions, 2 differ): the native loads the event work before
 * it starts the three-instruction offset 0xca2, the draft after its first instruction; the same
 * early load keeps 694 Func_02001250 a draft. */
#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_020043f4(void);
void Func_02004444(void);

void Func_02002d90(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    if (work->action_counter != 0) {
        Call1((void (*)())GameFlag_SetBit, 0x106);
        Func_020043f4();
        Func_02004444();
        Call1((void (*)())GameFlag_ClearBit, 0x106);
    }
}
