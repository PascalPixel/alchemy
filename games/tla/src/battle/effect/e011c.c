#include "types.h"
#include "ram_buffer.h"

void Func_080e0134();

void Func_080e011c(void)
{
    (*(u8 **)((u8 *)Ram_HeapSlots->effectWork + 20))[91] = 1;
    Func_080e0134();
}
