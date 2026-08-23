#include "types.h"

extern volatile u32 Data_03001c94;
void WaitFrames(u32);

void Func_08002ee4(void)
{
    for (;;) {
        (void)Data_03001c94;
        WaitFrames(1);
    }
}
