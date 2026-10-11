#include "ov686.h"

struct Ov686EventWork {
    u8 unknown_000[0x16a];
    s16 alarm;
};

extern s16 Data_02003782;
extern s16 Data_02003784;
extern s32 Data_0200378c;

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void Func_020015ac(void)
{
    struct Ov686EventWork *work = (struct Ov686EventWork *)Ram_HeapSlots->event_work;

    if (GameFlag_Test(0x104) == 0) {
        if (++Data_02003782 >= Data_02003784) {
            Scheduler_RemoveCallbackFar(Func_020015ac);
            if (Data_0200378c == 1)
                work->alarm = 68;
            else
                work->alarm = 69;
        }
    }
}
