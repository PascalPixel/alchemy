#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void Func_081434d8(void)
{
    Iwram_ClearWords(Ram_HeapSlots->battle_fx_work[0] + 0x78dc, 8);
}
