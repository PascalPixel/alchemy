#include "types.h"
#include "scene.h"
#include "abi/runtime/idle_forever.h"

extern volatile u32 gIw;
void WaitFrames(u32);

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)gIw;
        WaitFrames(1);
    }
}
