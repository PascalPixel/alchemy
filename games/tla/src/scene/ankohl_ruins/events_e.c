#include "ankohl_ruins.h"

struct AnkohlRuinsEventWork {
    u8 unk000[0x16a];
    s16 alarm;
};

extern s16 Data_02003782;
extern s16 Data_02003784;
extern s32 Data_0200378c;

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void Func_020015ac(void)
{
    struct AnkohlRuinsEventWork *work = (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;

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
