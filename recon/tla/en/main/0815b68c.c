/* Near miss of Func_0815b68c, decompiled from the listing (101 instructions,
 * draft 101, differ 12): the native forms the position's address (add r1,
 * sp, #8) before the copied scale is stored and starts the 0x6e4 offset
 * after the two zero stores, and it stores x and y (str r3, [r5] and
 * [r6]) one row after the next offset is started; here those rows come
 * one step earlier or later. Matched on the way: the particle's fields read
 * through work at each use (a pointer to the particle shares one base) and
 * position[3] and position[1] cleared first. */
#include "types.h"
#include "battle.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "ram_buffer.h"

struct PlacementScale {
    s32 x;
    s32 y;
};

extern const struct PlacementScale gData_08196e4c;

s32 Animation_ApplyChildArgumentFar(void *object, s32 argument);

// Moves the last particle by its velocity, draws effect object 7 there and
// steps its animation.
void Func_0815b68c(void)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    struct PlacementScale scale = gData_08196e4c;
    s32 position[4];

    position[3] = 0;
    position[1] = 0;
    position[0] = work->particles[63].x;
    position[2] = work->particles[63].y;
    Render_ApplyProjectedPlacementFar(work->objects[7], position, (s32 *)&scale, 0);
    work->particles[63].x += work->particles[63].velocityX;
    work->particles[63].y += work->particles[63].velocityY;
    if (work->frame <= 19)
        work->particles[63].velocityY += 0x1000;
    else
        work->particles[63].velocityY -= 0x1000;
    if (work->frame > 11)
        work->particles[63].velocityX += 0x1000;
    Animation_ApplyChildArgumentFar(work->objects[7], work->frame / 7 % 8 + 7);
    work->frame++;
}
