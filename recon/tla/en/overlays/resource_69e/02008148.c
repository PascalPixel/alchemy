/* Draft of overlay 69E's Func_02000148 (92 bytes), the twin of The Broken Seal's
 * SceneEffect_UpdateOrbitAroundActor. differ 2 of 40, one reordered pair at the end: the
 * native loads the step -0x800 from the pool (ldr r1) before it reads the angle again
 * (ldrh r3, [r6]); here the read comes first. The two are equal in priority, so the native's
 * RTL had the constant ahead of the read.
 * Measured:
 *  - the coordinates as plain s32 fields: the saved_x store sinks below the angle read
 *    (differ 6); as the fixed/pixel union the stores alias the u16 read and stay in order;
 *  - as plain statements with the unions (z = center z + sine * 2; saved_x = x;
 *    saved_z = z; angle -= 0x800): differ 4, the x read comes after the shift of the sine
 *    where the native has it before; the forced temporaries and the loop note below put
 *    it there;
 *  - against the last pair: the step as a forced temporary with a loop note after it (no
 *    change, the constant is propagated back); the step behind an "l" asm barrier after
 *    the saves (the order is the native's, but the step takes r2 and saved_x is stored
 *    before z, differ 6), and before the saves (differ 9);
 *  - the angle through the field and not a pointer: the same code without a loop note,
 *    the address formed twice with one.
 * The devices below need their FAKEMATCH tags when this is adopted. */
#include "types.h"
#include "object_runtime.h"

// 16.16 固定小数点の座標
union FieldCoordinate {
    s32 fixed;
    struct {
        u16 fraction;
        s16 pixel;
    } part;
};

struct OrbitEffect {
    u8 unknown_00[8];
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[28];
    s32 radius;
    u8 unknown_34[4];
    union FieldCoordinate saved_x;
    u8 unknown_3c[4];
    union FieldCoordinate saved_z;
    u8 unknown_44[32];
    u16 angle;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

// アクター 23 のまわりを回る
void Func_02000148(struct OrbitEffect *effect)
{
    struct ObjectRuntime *center = Object_GetById(23);
    u16 *angle = &effect->angle;
    s32 turn = *angle;
    s32 sine;
    s32 x;
    s32 z;

    effect->x.fixed = center->x + Math_Cosine(turn) * (effect->radius + 3);
    sine = Math_Sine(turn);
    z = center->z;
    x = effect->x.fixed;
    do { } while (0);
    effect->z.fixed = z + (sine << 1);
    effect->saved_x.fixed = x;
    effect->saved_z.fixed = effect->z.fixed;
    *angle -= 0x800;
}
