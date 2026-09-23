#include "TYPES.H"
#include "FIELD_EVENT.H"

struct SceneEffectOrigin {
    s32 x;
    s32 y;
    s32 z;
};
struct SceneEffectWork {
    struct SceneEffectOrigin *origin;
};
extern struct SceneEffectWork *gSceneEffectWork;
extern u32 gFallingEffectState;
extern s32 gFallingEffectWidth;
extern s32 gFallingEffectOffset;
extern const s32 gFallingEffectScript[];

void FieldScene_UpdateFallingEffect(void)
{
    /* FAKEMATCH: object starts as a meaningless null; the reference keeps that
       zero in r5 from the prologue and stores it when state 3 ends. */
    struct FieldActor *object = 0;
    struct SceneEffectOrigin *origin;
    struct FieldSprite *sprite;
    s32 x, z;

    switch (gFallingEffectState) {
    case 1:
        if (gFallingEffectWidth < 15000)
            gFallingEffectWidth += 50;
        if (gFallingEffectOffset > 0x3c0000)
            gFallingEffectOffset -= 0x4000;
        break;
    case 2:
        if (gFallingEffectWidth < 30000)
            gFallingEffectWidth += 50;
        if (gFallingEffectOffset > 0x180000)
            gFallingEffectOffset -= 0x4000;
        break;
    case 3:
        if (gFallingEffectOffset < -0x800000) {
            gFallingEffectState = 0;
        } else {
            gFallingEffectWidth += 50;
            gFallingEffectOffset -= 0x4000;
        }
        break;
    }
    if ((gFrameCount & 7) == 0 && (object = Engine_ObjectCreate(285, 0, 0, 0)) != 0) {
        origin = gSceneEffectWork->origin;
        if ((gFrameCount & 63) == 0)
            Engine_AudioPlayCue(246);
        if (gFallingEffectState != 0)
            x = origin->x + (((u32)Engine_RandomNext() * gFallingEffectWidth >> 16) << 8)
                + gFallingEffectOffset;
        else
            x = origin->x + (Engine_RandomNext() << 8) - 0x800000;
        z = origin->z + (Engine_RandomNext() << 8) - 0x800000;
        object->motion_flags = 0;
        object->y.fixed = 0xa00000;
        sprite = object->sprite;
        object->scale_x = 0xe666;
        object->scale_y = 0xe666;
        object->x.fixed = x;
        object->z.fixed = z;
        sprite->flags = 0;
        object->priority_flags &= ~1;
        sprite->priority = 1;
        Engine_ObjectSetAnimation(object, 1);
        Engine_ObjectSetScript(object, gFallingEffectScript);
    }
}
