#include "types.h"
#include "ram_buffer.h"

struct AirsRockEventWork {
    u8 unk000[0xcbc];
    s32 valueCbc;
};

extern s16 Data_02003d94[];

void Func_02000468(s32 value);
void Func_02003cb8(const void *data);

void Func_02001ca4(void)
{
    struct AirsRockEventWork *work = Ram_HeapSlots->eventWork;

    Func_02003cb8(Data_02003d94);
    Func_02000468(work->valueCbc);
}
