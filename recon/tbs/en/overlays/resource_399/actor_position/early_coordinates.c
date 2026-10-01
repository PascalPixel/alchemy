/* NONMATCHING: early Imil cell coordinates, 2026-10-01.
 * The complete actor-eleven update compiles with approved TBS flags but
 * assigning its cell coordinates before the flag and priority calls adds
 * four bytes and keeps them in saved registers. Production sets them after
 * those calls in the editions without the live-position guard.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    struct FieldActor *actor = Object_GetById(11);

    a = actor->x.fixed >> 20;
    if (a != 15)
        return;
    b = actor->z.fixed >> 20;
    if (b != 7)
        return;
    GameFlag_Set(0x203);
    Engine_ActorSetSpritePriority(11, 3);
#else
    a = 15;
    b = 7;
    GameFlag_Set(0x203);
    Engine_ActorSetSpritePriority(11, 3);
#endif
    Map_CopyCellAttributes(15, 6, 1, 1, a, b);
}
