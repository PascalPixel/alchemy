#include "RUNTIME_MEM.H"
#include "PROBE.H"

void OverlayObject_ReleasePublishedAttachmentB(void)
{
    u8 **slot = Runtime_AllocateBlock(35, 4);
    u8 *state;
    u8 *obj;

    if (slot == 0)
        return;

    state = *slot;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;

    Engine_ObjectDispatchRelease(obj);
    *(u8 **)(state + 20) = 0;
}
