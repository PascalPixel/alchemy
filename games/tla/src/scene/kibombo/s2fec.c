#include "kibombo.h"

void Func_02002fec(struct KibomboObject *object)
{
    s32 step;

    object->x += object->field_64 << 8;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores x before it loads y (6 differ without the two blocks). */
    object->y += 0x8000;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores y before it builds the scale step. */
    step = 0x51e; /* FAKEMATCH: the temporary builds the scale step before scale_x is loaded (one reordered row). */
    object->scale_x += step;
    object->scale_y += step;
    object->field_64 += 2;
    if (--object->field_68 == 0)
        Func_02005a3c(object);
}

void Func_02003030(void)
{
    if ((gFrameCount & 63) == 0) {
        struct KibomboObject *object = Func_02005a34(0x11d, 0x580000, 0x100000, 0x13a0000);
        u8 *part;
        s32 mask;

        if (object != 0) {
            object->field_5c = 2;
            object->motion_flags = 0;
            object->field_64 = 0;
            do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native stores the counter before it takes the flags' address (one reordered pair). */
            object->priority_flags &= ~1;
            mask = 13;
            /* FAKEMATCH: ⚓️ builds the priority mask before loading the sprite, as in ObjectGroup_Spawn; the bit-field store loads the sprite first (differ 4). */
            asm volatile("" : "+l"(mask) : : "memory");
            part = (u8 *)object->sprite;
            part[9] = (part[9] & -mask) | 8;
            ObjectDispatch_SetSingleChildFlags(object, 0);
            object->scale_x = 0x8000;
            object->scale_y = 0x8000;
            object->field_68 = 60;
            object->update = Func_02002fec;
            Func_02005a24(object, 5);
            Animation_SetStateFlags(object, 1);
        }
    }
}
