#include "types.h"
#include "ram_buffer.h"

struct ShakeWork {
    u8 unk0000[54];
    u16 offset;
    u8 unk0038[0x7774];
    s32 amount;
    s32 phase;
};

void Func_0814c928(void)
{
    struct ShakeWork *work = (struct ShakeWork *)Ram_HeapSlots->battleFxWork[0];
    struct ShakeWork *camera = Ram_HeapSlots->cameraWork;

    if (work->phase == 1) {
        camera->offset += work->amount;
        work->phase = 0;
    } else {
        camera->offset += work->amount / 2;
        if (work->phase == 2)
            work->phase = 0;
        else
            work->phase = 2;
    }
}
