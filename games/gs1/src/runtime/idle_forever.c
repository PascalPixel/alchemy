#include "types.h"
#include "scene.h"
#include "resource.h"

/* runtime/idle_forever.c */
/* runtime/idle_forever.c */
extern volatile u32 gIw;
void WaitFrames(s32);

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)gIw;
        WaitFrames(1);
    }
}
