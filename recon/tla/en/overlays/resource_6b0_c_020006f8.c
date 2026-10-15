/* Near miss, differ 4 of 73 (21 before, 2026-10-12): rewritten with a struct; the function returns 0 when the radius is under the step and 1 after two steps. Two rows are left in another order, both in the first step only: the native loads the angle (ldrh r3) before it starts the spin's sign extension (lsls r2), and loads the radius into r0 before the angle for the first polar call; the second step matches. Tried: the polar helper declared as returning a value (no change), a radius temporary behind an empty asm barrier (differ 10), spin and angle in locals (differ 19). */
#include "types.h"

struct Mover {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 fall;
    u8 unk18[24];
    s32 step;
    s32 height;
    u8 unk38[12];
    s32 originX;
    s32 originZ;
    s32 radius;
    u8 unk50[20];
    s16 angle;
    s16 spin;
};

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
void Func_020006a4(struct Mover *mover);

s32 Func_020006f8(struct Mover *mover)
{
    if (mover->radius < mover->step)
        return 0;
    mover->angle += mover->spin >> 1;
    mover->x = mover->originX;
    mover->y = mover->height;
    mover->z = mover->originZ;
    Vector_AddPolarOffsetFar(mover->radius, mover->angle, &mover->x);
    Func_020006a4(mover);
    mover->radius -= mover->step >> 1;
    mover->angle += mover->spin >> 1;
    mover->x = mover->originX;
    mover->y = mover->height;
    mover->z = mover->originZ;
    Vector_AddPolarOffsetFar(mover->radius, mover->angle, &mover->x);
    Func_020006a4(mover);
    mover->radius -= mover->step >> 1;
    mover->height -= mover->fall;
    return 1;
}
