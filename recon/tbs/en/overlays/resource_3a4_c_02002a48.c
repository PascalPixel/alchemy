/* NONMATCHING: 264 of 272 bytes, 34 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 87 halfwords: control flow matches (two switches on the drift direction); the reference puts the 0x8000 compare and branch constants in r1, so its branch tails differ and are not cross-jumped as ours are */
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define TIMER(object) (*(s16 *)&(object)->unknown_64)

extern s32 ArutinYama_DriftDirection;

void Local_02002a48(struct FieldActor *object)
{
    switch (ArutinYama_DriftDirection) {
    case 0:
        object->x.fixed += (TIMER(object) << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
        break;
    case 0x8000:
        object->x.fixed -= (TIMER(object) << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
        break;
    }
    if (TIMER(object) <= 3) {
        switch (ArutinYama_DriftDirection) {
        case 0:
            object->x.fixed += 0x8000;
            break;
        case 0x8000:
            object->x.fixed -= 0x8000;
            break;
        }
        object->scale_x += 0x1999;
        object->scale_y += -0x2590cc9;
    } else {
        object->z.fixed += 0x13333;
        object->scale_x += 0x7ae;
        object->scale_y += 0x7ae;
    }
    if ((u32)(TIMER(object) * Engine_RandomNext()) >> 16 == 0)
        Engine_ObjectSetPalette(object, 7);
    if (object->unknown_64 != 0)
        TIMER(object) -= 2;
    else
        TIMER(object) = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
    if (--*(s32 *)object->unknown_68 == 0)
        Engine_ObjectDispatchRelease(object);
}
