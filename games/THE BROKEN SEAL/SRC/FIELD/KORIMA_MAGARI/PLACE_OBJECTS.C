#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_080091a8(s32 layer, s32 x, s32 z);

/* An object placed at a cell, turned along one axis or the other. */
struct IcePlacement {
    s16 type;
    s16 x;
    s16 z;
    s16 turned;
    struct FieldActor *actor;
};

/* Create each listed object centred on its cell and standing on the terrain;
 * the list ends at type -1. */
void KorimaMagari_PlaceObjects(struct IcePlacement *entry)
{
    struct FieldActor *actor;
    s32 height;
    s32 x;
    s32 z;

    for (; entry->type != -1; entry++) {
        if (entry->turned == 0) {
            x = (entry->x << 20) + 0x200000;
            z = (entry->z << 20) + 0x80000;
        } else {
            x = (entry->x << 20) + 0x80000;
            z = (entry->z << 20) + 0x200000;
        }
        actor = Engine_ObjectCreate(entry->type, x, 0, z);
        if (actor == NULL)
            return;
        entry->actor = actor;
        Engine_ObjectSetAnimation(actor, 1);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->collision_flags = 0;
        actor->radius = 32;
        height = Main_080091a8(0, actor->x.part.pixel, actor->z.part.pixel) << 16;
        /* FAKEMATCH: y is updated through an s32 lvalue; the coordinate
         * union has an s16 member, which would order the next entry load
         * after this store. */
        *(s32 *)&actor->y += height;
        *(s32 *)actor->unknown_14 = height;
    }
}
