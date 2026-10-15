#include "local.h"

void Func_02000714(struct ObjectRuntime *object)
{
    s32 step;

    object->x += object->action << 8;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads y before it builds 0x8000 (2 differ without it). */
    object->y += 0x8000;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native stores y before it starts the step, and loads scaleX between the step's first byte and its shift (4 differ without it; the step as a local alone changes nothing). */
    step = 0x51e;
    object->scaleX += step;
    object->scaleY += step;
    object->action += 2;
    if (--*(s32 *)&object->linkedObject == 0)
        Func_02002000(object);
}

void Func_02000758(s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object = Func_02001ff8(0x11d, x, y, z);

    if (object != NULL) {
        s8 *animation;

        object->unk5c[0] = 2;
        object->motionFlags = 0;
        object->action = 0;
        object->priorityFlags &= 0xfe;
        Object_SetPartAttribute(object, 7);
        animation = object->animation;
        animation[9] = (animation[9] & -13) | 4;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        object->scaleX = 0x8000;
        object->scaleY = 0x8000;
        *(s32 *)&object->linkedObject = 60;
        object->update = (void (*)(void))Func_02000714;
        Func_02001fe0(object, 5);
        Animation_SetStateFlags(object, 1);
    }
}

void Func_020007d8(void)
{
    if ((gFrameCount & 63) == 0)
        Func_02000758(0xf80000, 0x300000, 0x1000000);
}
