#include "types.h"

#define Runtime_IdleForever Func_08002ee4

extern volatile u32 Data_03001c94;
void WaitFrames(u32);

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)Data_03001c94;
        WaitFrames(1);
    }
}
