#include "mem.h"
#include "entrance.h"

void OverlayObject_ReleasePublishedAttachment(void)
{
    u8 **pub = Runtime_AllocateBlock(35, 4);
    u8 *state;
    u8 *obj;

    if (pub == 0)
        return;
    state = *pub;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;
    Engine_ObjectDispatchRelease(obj);
    *(u8 **)(state + 20) = 0;
}
