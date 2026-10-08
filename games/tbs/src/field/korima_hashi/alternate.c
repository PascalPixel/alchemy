#include "types.h"
#include "field_event.h"

void KorimaHashi_AlternateBridgePostHeights(void)
{
    struct FieldActor *p;

    p = (void *)Object_GetById(13);
    if (p != 0) {
        p->motion_flags = 0;
        if ((gFrameCount & 1) == 0) {
            p->y.fixed = 0;
        } else {
            p->y.fixed = 0x1f40000;
        }
    }
    p = (void *)Object_GetById(14);
    if (p != 0) {
        p->motion_flags = 0;
        if ((gFrameCount & 1) != 0) {
            p->y.fixed = 0;
        } else {
            p->y.fixed = 0x1f40000;
        }
    }
    p = (void *)Object_GetById(15);
    if (p != 0) {
        p->motion_flags = 0;
        if ((gFrameCount & 1) == 0) {
            p->y.fixed = 0;
        } else {
            p->y.fixed = 0x1f40000;
        }
    }
    p = (void *)Object_GetById(16);
    if (p != 0) {
        p->motion_flags = 0;
        if ((gFrameCount & 1) != 0) {
            p->y.fixed = 0;
        } else {
            p->y.fixed = 0x1f40000;
        }
    }
}
