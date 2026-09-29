/* The nine radial effects. */
#include "BABI.H"

void SceneEffect_SpawnNineRadialEffects(s32 actor)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = (struct SceneObject *)Object_GetById(actor);
    params.unk00 = 1;
    params.mode = 7;
    params.callback = (s32)Effect_AdvanceMotion;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Math_Cos(v);
        vec.y = 0;
        z = Math_Sin(v);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        ((void (*)())Effect_Spawn)(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}
