/* Draft, differ 47 of 94: count random sparks around a position through the overlay's spawn
 * (Func_020000b8), written from the listing (its ☀️ twin by shape, mercury_lighthouse.c's
 * SceneEffect_SpawnRandomizedBurst, is a different function). Left: allocation. The native keeps
 * velocityX in r8, built in place (mov r8, r0; ands through r3; add r8, -8; lsls through r3), the
 * mask 15 in sl and the position in fp, and leaves count on the stack; the draft keeps count in fp
 * and builds velocityX in r4 and spills it. Measured: a for, while or do-while loop hoists the mask
 * out of the loop (differ 80 to 88); the goto loop keeps it inside. As a member store the rotation
 * goes through a bit-field insert whose zero CSE keeps in sl for the velocity_z argument (59); the
 * raw u16 store off the options pointer avoids it (47). velocityX written in place in four
 * statements compiles the same.
 */
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

void Func_020006d4(union FieldObject *object);

void Func_02000708(struct TundariaTowerPosition *position, s32 count, s32 scale)
{
    struct EffectOptions options;
    struct EffectOptions *p = &options;
    s32 i;
    s32 velocityX;
    s32 velocityY;
    s32 x;

    p->priority = 1;
    p->type = 279;
    p->target_scale_y = 0xcccc;
    p->target_scale_x = 0xcccc;
    p->start_scale_y = scale;
    p->start_scale_x = scale;
    p->update = Func_020006d4;
    i = 0;
    if (count == 0)
        return;
loop:
    *(u16 *)((u8 *)p + 32) = (Random16Far() & 15) << 12;
    velocityX = ((Random16Far() & 15) - 8) << 14;
    velocityY = Random16Far() & 15;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    Func_020000b8(x, position->y + ((Random16Far() & 15) << 16), position->z, velocityX,
                  (velocityY + 8) << 14, 0, 0x15e0000, (struct TundariaTowerEffectParams *)p);
    if (++i != count)
        goto loop;
}
