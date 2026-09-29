#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

#define FrameCounter (*(u32 *)&gFrameCount)

void Vector_AddPolarOffset(s32 radius, s32 angle, union FieldCoordinate *pos);

/* Blink the actor's palette with the frame counter and, while it is not held,
 * bob it on its sine path and advance the angle. */
void WorldMap_UpdateBobbingMarker(struct FieldActor *actor)
{
    s16 *angle;

    if (FrameCounter & 2)
        Engine_ObjectSetPartPalettes(actor, 10);
    else
        Engine_ObjectSetPartPalettes(actor, 7);
    if ((s16)actor->unknown_66 == 0) {
        angle = (s16 *)&actor->unknown_64;
        actor->x.fixed = 0x15d00000;
        actor->y.fixed = Iwram_MulQ16(Engine_MathSin(*angle << 3), 0x40000) + 0x100000;
        actor->z.fixed = 0x5300000;
        Vector_AddPolarOffset(0x100000, *angle, &actor->x);
        actor->facing = *angle + 0x4000;
        *angle += 0x400;
    }
}
