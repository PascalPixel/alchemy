/* Near miss, differ 4 of 18 (one reordered pair and the pool offset it
 * moves): the native loads the fill routine's address, then the zero, then
 * copies the size into r1; this copies the size first. From the sched2 dump:
 * the three argument loads tie on priority (65) and on dependents (3 each),
 * so they keep RTL order, and the size's copy is emitted first because the
 * constant 0x2a0 is precomputed before the routine's address is loaded.
 * Measured: a local routine pointer, an inline wrapper (either parameter
 * order) and the size as a named local all give this order; a "+l" barrier
 * on the pointer moves the load first but leaves the copy ahead of the zero
 * (differ 2), and holding the zero behind a second barrier puts it ahead of
 * the load (differ 2). Its neighbours 08126218 (pallevel.c) and 081263f0
 * (transwrk.c) are C. */
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct BattlePresentationTransition {
    u8 unknown_00[8];
    s32 mode;
};

void *Runtime_AllocateBlock(s32 slot, s32 size);

void Func_081263c4(void)
{
    void *block = Runtime_AllocateBlock(40, 0x2a0);
    struct BattlePresentationTransition *work = Ram_HeapSlots->transition_work;

    Iwram_FillWords(block, 0x2a0, 0);
    work->mode = 0;
}
