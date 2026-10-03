#include "FIELDOBJ.H"
#include "HEYA.H"

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *obj;
    struct FieldSprite *rec;

    obj = Engine_ObjectCreate(kind, x, y, z);
    if (obj != NULL) {
        rec = obj->sprite;
        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 0xE);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

/* Creates an object of the given kind at (x, y, z) and configures it: the
 * sprite's priority becomes 1, its flags clear, palette 15, and the
 * object's draw bits select the second layer. */
void *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *effect = Engine_ObjectCreate(kind, x, y, z);

    if (effect != NULL) {
        struct FieldSprite *sprite = effect->sprite;

        sprite->priority = 1;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Engine_ActorSetSpriteFlags(effect, 0);
        ObjectGroup_SetChildValue(effect, 15);
        effect->priority_flags = (effect->priority_flags & 0xfe) | 2;
        return effect;
    }
    return NULL;
}
