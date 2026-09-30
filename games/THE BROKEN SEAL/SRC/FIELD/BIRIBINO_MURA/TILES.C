#include "MURA.H"

/* Spawns the kind-24 effect where the actor stands, drawn translucent
   behind the background layers. */
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor)
{
    struct FieldActor *effect;
    struct FieldSprite *sprite;

    effect = Engine_ObjectCreate(24, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    if (effect == NULL)
        return;

    sprite = effect->sprite;
    Object_SetScript(effect, Mura_SpawnScript);
    effect->motion_flags = 0;
    effect->unknown_22 = 1;
    effect->priority_flags = 2;
    if (sprite == NULL)
        return;

    AnimationObjects_SelectAnimation(sprite, 2);
    sprite->flags = 0;
    sprite->blend_mode = 1;
    sprite->priority = 3;
}

/* Opens the two gate cells on row 14 the actor stands in (z 6 or 9). */
void FieldScene_DrawTilesByActor8Row(void)
{
    struct FieldActor *actor;

    actor = Actor_Get(8);
    if (actor == NULL)
        return;

    if (actor->z.fixed >> 20 == 6)
        Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
    else
        Map_CopyCellAttributes(0, 0, 1, 1, 14, 6);

    if (actor->z.fixed >> 20 == 9)
        Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
    else
        Map_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}
