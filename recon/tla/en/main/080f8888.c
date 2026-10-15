/* Near miss, differ 2 with the barrier (6 without): the native loads attribute byte 21 before it stores byte 23 (ldrb r2, [r0, #21]; strb r3, [r0, #23]); sched2 ranks the store first here, as it depends on the last instruction scheduled. */
#include "m7_interfaces.h"

void UiIcon_PrepareObject(struct RenderOutput *object)
{
    if (object != NULL) {
        object->active = 1;
        /* FAKEMATCH: draft barrier; the header gives the measured difference. */
        __asm__("");
        object->oam.x = object->x;
        object->oam.y = object->y;
        object->oam.matrix = 0;
        object->oam.affineMode = 0;
    }
}
