/* Draft, differ 50 of 58: ☀️'s SpawnRadialEffectBurst (scene/takara_shima.c)
 * for overlay 65F. Left: the native keeps the object in r7 and the angle step
 * in r10, and forms the vector's address (add r5, sp, #56) after the cosine
 * call on every pass; this C, like ☀️'s native, keeps the object in r10 and
 * the address in r7 for the whole loop. Tried: a goto loop, a vector pointer
 * or an inner-scope vector. Also measured: loop.c moves the vector's
 * address out of a for loop (savings 1, life 19 against 37 insns) and in
 * this goto form GCSE's PRE hoists it to the entry edge; a vector pointer
 * assigned after the cosine call (43), the address through an empty asm
 * (its &vec input is hoisted instead: 63), one-pass blocks inside the loop
 * (50 to 54), the object pinned to r7, four minutes of alchemy permute
 * (best 38). */
#include "types.h"
#include "field_effect.h"
#include "object_runtime.h"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
    const struct EffectOptions *options);

void Func_020012a8(s32 object_id)
{
    struct ObjectRuntime *object;
    struct Vec vec;
    struct EffectOptions params;
    u32 angle_step;
    s32 angle;
    s32 x;
    s32 z;

    object = Object_GetById(object_id);
    params.update = Effect_AdvanceMotion;
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes form the vector's address after the cosine call on every pass, and a for loop hoists it. */
    angle_step = 0;
burst:
        angle = angle_step << 12;
        vec.x = Math_Cosine(angle);
        vec.y = 0;
        z = Math_Sine(angle);
        x = vec.x;
        vec.z = z;
        x += x / 3;
        vec.x = x;
        Func_020000b8(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
    angle_step += 2;
    if (angle_step <= 16)
        goto burst;
}
