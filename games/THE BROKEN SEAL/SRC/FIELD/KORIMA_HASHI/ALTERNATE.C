#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The four bridge posts, raised and lowered on alternate frames. */
struct BridgePost {
    u8 pad00[0x0c];
    s32 field0c;
    u8 pad10[0x55 - 0x10];
    u8 field55;
};


void SceneActor_AlternateSlots13To16Field0c(void)
{
    struct BridgePost *p;

    p = (void *)Object_GetById(13);
    if (p != 0) {
        p->field55 = 0;
        if ((gFrameCount & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = (void *)Object_GetById(14);
    if (p != 0) {
        p->field55 = 0;
        if ((gFrameCount & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = (void *)Object_GetById(15);
    if (p != 0) {
        p->field55 = 0;
        if ((gFrameCount & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = (void *)Object_GetById(16);
    if (p != 0) {
        p->field55 = 0;
        if ((gFrameCount & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
}
