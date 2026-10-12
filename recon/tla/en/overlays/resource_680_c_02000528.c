/* Near miss: differ 82 of 88 as a goto loop (registers), differ 109 as a for loop.
 * The native keeps everything inside the loop: mov r5, sp for the position on every pass,
 * 0x30000 rebuilt (shared by the radius and the scale), the priority byte's zero loaded from
 * the pool into r5, and the timer a second counter in r10 stepped by -2 beside i in r8.
 * As a for loop, loop.c hoists all three and the timer is 20 - 2 * i. As the goto loop below
 * only gcse's hoisting of the position address (sfp - 12) remains, with the source in r8 and
 * the position in r7 where the native has r7 and r5; the native's frame holds no r11.
 * The priority byte's zero comes from the pool with a plain byte store. */
#include "types.h"

struct OrbitEffect {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[15];
    u8 priority_flags;
    u8 unknown_24[4];
    s32 scale;
    u8 unknown_2c[28];
    s32 speed;
    u8 unknown_4c[24];
    u16 timer;
    s16 pause;
    struct OrbitEffect *anchor;
    void (*update)(struct OrbitEffect *);
};

u32 Random16Far(void);
void Func_02000e5c(struct OrbitEffect *effect, s32 priority);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
struct OrbitEffect *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02000474(struct OrbitEffect *effect);

s32 Func_02000528(struct OrbitEffect *source)
{
    s32 i;
    s32 timer;

    i = 0;
    timer = 20;
loop:
    {
        struct OrbitEffect *effect;
        s32 radius;
        s32 position[3];

        position[0] = source->x;
        position[1] = source->y + i * 0xcccc + 0x40000;
        position[2] = source->z;
        radius = Random16Far();
        Vector_AddPolarOffsetFar(radius * 5 + 0x30000, Random16Far(), position);
        effect = Object_Spawn(308, position[0], position[1], position[2]);
        if (effect != NULL) {
            effect->update = Func_02000474;
            effect->timer = timer;
            effect->pause = 0;
            effect->speed = 0x1999;
            effect->scale = 0x30000;
            effect->angle = Random16Far();
            effect->priority_flags = 0;
            Func_02000e5c(effect, 2);
        }
    }
    i++;
    timer -= 2;
    if (i < 16)
        goto loop;
    return 0;
}
