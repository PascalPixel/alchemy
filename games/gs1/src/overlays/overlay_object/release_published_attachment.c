#include "types.h"

#define OverlayObject_ReleasePublishedAttachment Func_020023b8

extern u8 **Func_020047ec(s32, s32);
extern void Func_02004824(u8 *);

/* Release the optional published attachment; complete owner, no pool. */
void OverlayObject_ReleasePublishedAttachment(void)
{
    u8 **pub = Func_020047ec(35, 4);
    u8 *state;
    u8 *obj;

    if (pub == 0)
        return;
    state = *pub;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;
    Func_02004824(obj);
    *(u8 **)(state + 20) = 0;
}
