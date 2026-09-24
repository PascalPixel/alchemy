#include "TYPES.H"
#include "FIELD_EVENT.H"

/* When actor 12 stands on cell (30, 20), settles it there, copies the cell's
 * attributes to (32, 20) and sets flag 0x212. */
void ArutamiraDou_Func02000cf8(void)
{
    struct FieldActor *actor = Engine_ActorGet(12);

    if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
        actor->motion_flags = 2;
        *(s32 *)actor->unknown_14 = 0;
        actor->priority_flags = 2;
        Engine_MapCopyCellAttributes(30, 20, 1, 1, 32, 20);
        Engine_GameFlagSet(0x212);
    }
}
