#include "TYPES.H"
#include "FIELD_EVENT.H"


/* The way the wind blows the drifting leaves: 0 or 0x8000. */
extern s32 ArutinYama_LeafMode;

/* A drifting leaf: blown along the wind with a random wobble while its
 * timer runs, shrinking and settling once it is low, flickering to palette
 * 7 at random; released when its lifetime ends. */
void ArutinYama_UpdateDriftingLeaf(struct FieldActor *object)
{
    switch (ArutinYama_LeafMode) {
    case 0:
        object->x.fixed += ((*(s16 *)&(object)->unknown_64) << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
        break;
    case 0x8000:
        object->x.fixed -= ((*(s16 *)&(object)->unknown_64) << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
        break;
    }
    if ((*(s16 *)&(object)->unknown_64) <= 3) {
        switch (ArutinYama_LeafMode) {
        case 0:
            object->x.fixed += 0x8000;
            break;
        case 0x8000:
            object->x.fixed -= 0x8000;
            break;
        }
        object->scale_x += 0x1999;
        object->scale_y -= 0xccc;
    } else {
        object->z.fixed += 0x13333;
        object->scale_x += 0x7ae;
        object->scale_y += 0x7ae;
    }
    if ((u32)((*(s16 *)&(object)->unknown_64) * Engine_RandomNext()) >> 16 == 0)
        Engine_ObjectSetPalette(object, 7);
    if ((*(s16 *)&(object)->unknown_64) != 0)
        (*(s16 *)&(object)->unknown_64) -= 2;
    else
        (*(s16 *)&(object)->unknown_64) = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
    if (--*(s32 *)object->unknown_68 == 0)
        Engine_ObjectDispatchRelease(object);
}
