#include "TYPES.H"
#include "FIELD_EVENT.H"

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);

/* Tile 97: swing the rolling object a quarter turn about the cell corner
 * beside it, sixteen steps of 0x400 from the grid-snapped pivot. */
void ArutinYama_TurnRollingObjectA(struct FieldActor *object)
{
    union FieldCoordinate *p;
    union FieldCoordinate pos[3];
    s32 angle;
    s32 x;
    s32 z;
    s32 n;

    angle = (object->facing + 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 5);
    n = 0;
    Engine_AudioPlayCue(184);
    while (n < 16) {
        angle += 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Vector_AddPolarOffset(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle + 0x4000;
        Engine_TaskWait(1);
        n++;
    }
    Engine_AudioPlayCue(233);
}

/* Tile 98: the same swing the other way. */
void ArutinYama_TurnRollingObjectB(struct FieldActor *object)
{
    union FieldCoordinate *p;
    union FieldCoordinate pos[3];
    s32 angle;
    s32 x;
    s32 z;
    s32 n;

    angle = (object->facing - 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 6);
    n = 0;
    Engine_AudioPlayCue(184);
    while (n < 16) {
        angle -= 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Vector_AddPolarOffset(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle - 0x4000;
        Engine_TaskWait(1);
        n++;
    }
    Engine_AudioPlayCue(233);
}
