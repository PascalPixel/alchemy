#include "types.h"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Actor {
    u8 pad0[4];
    s32 screenX;
    s32 screenZ;
    u8 pad1[8];
    s32 x;
    s32 z;
    u8 pad2[28];
    s16 timer;
    u8 pad3[2];
    s16 yaw;
    s16 pitch;
    s8 phase;
};

struct Target {
    u8 pad0[8];
    struct Vec pos;
};

extern s32 Data_02000240[];

void Func_0800447c(s32, s32, struct Vec *);
struct Target *Func_08092054(s32);
void Func_080974d8(struct Vec *);
void Func_0809bb34(struct Actor *);

void Func_08095938(struct Actor *actor)
{
    struct Target *target;
    struct Vec pos;

    target = Func_08092054(Data_02000240[125]);
    if (actor->phase == 0) {
        actor->yaw += 1;
        actor->pitch += 1;
        if (actor->timer == 60) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 1) {
        actor->pitch += 1;
        if (actor->timer == 40) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 2) {
        actor->pitch += 1;
        pos.x = target->pos.x;
        pos.y = target->pos.y + 0x140000;
        pos.z = target->pos.z;
        Func_080974d8(&pos);
        actor->x += (pos.x - actor->x) / 8;
        actor->z += (pos.z - actor->z) / 8;
        if (actor->timer == 40) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 3) {
        actor->yaw -= 1;
        actor->pitch += 1;
        if (actor->timer == 60) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 4) {
        Func_0809bb34(actor);
    }
    pos.x = actor->x;
    pos.z = actor->z;
    Func_0800447c(actor->yaw << 16, actor->pitch << 11, &pos);
    actor->screenX = pos.x;
    actor->screenZ = pos.z;
}
