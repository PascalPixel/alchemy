#include "types.h"
#include "ram_buffer.h"

struct Ov663EventWork {
    u8 unknown_000[0xcbc];
    s32 value_cbc;
};

extern s16 Data_02003d94[];

void Func_02000468(s32 value);
void Func_02003cb8(const void *data);

void Func_02001ca4(void)
{
    struct Ov663EventWork *work = Ram_HeapSlots->event_work;

    Func_02003cb8(Data_02003d94);
    Func_02000468(work->value_cbc);
}
