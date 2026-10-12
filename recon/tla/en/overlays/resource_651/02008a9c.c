/* Near miss of Func_02000a9c (19 instructions, 4 differ): the native holds the constant 1 in r3
 * and the loaded flags in r2 (orrs r3, r2), the draft the reverse. Tried: |= then &=, one
 * expression with 1 on the left, two bit-fields (worse: ~2 becomes movs 3, negs), an inline
 * set-and-clear helper. */
#include "types.h"
#include "ram_buffer.h"

extern s16 Data_02000d80[];

void Func_02000d58(s16 *data);

void Func_02000a9c(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    u8 *state;

    Func_02000d58(Data_02000d80);
    state = *(u8 **)(work + 0xcbc);
    state[35] |= 1;
    state[35] &= ~2;
}
