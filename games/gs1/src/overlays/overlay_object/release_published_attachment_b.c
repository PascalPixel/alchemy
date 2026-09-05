#include "types.h"

#define OverlayObject_ReleasePublishedAttachmentB Func_02005948

extern u8 **Func_0200b304(s32 group, s32 slot);
extern void Func_0200b33c(u8 *obj);

/* Release the scene object's optional attachment and clear its slot. */
void OverlayObject_ReleasePublishedAttachmentB(void)
{
    u8 **slot = Func_0200b304(35, 4);
    u8 *state;
    u8 *obj;

    if (slot == 0)
        return;

    state = *slot;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;

    Func_0200b33c(obj);
    *(u8 **)(state + 20) = 0;
}
