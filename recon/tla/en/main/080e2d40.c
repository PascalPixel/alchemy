/* Near miss, differ 2: the native loads the effect work before the
   callback's address; this draft loads the address first. Ends the effect
   whose update is Func_080e2c38, releasing its resource entry. */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "CALLBACK_SCHEDULER.H"

void Func_080e2c38(void);

void BattleFx_EndX(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    Scheduler_RemoveCallback((u32)Func_080e2c38);
    Resource_ResetEntry(*(s16 *)(work + 0x1b00));
}
