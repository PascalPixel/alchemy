#include "TYPES.H"

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

extern s32 gGameState[];
#define PARTY_STATE gGameState

void Math_AdvanceVectorByAngle(s32, s32, struct Vec *);
struct Target *Object_GetById(s32);
void Camera_WorldToScreen(struct Vec *);
void BattleFx_ClearOwnedSlot(struct Actor *);

void BattleFx_RunAngledApproachPhases(struct Actor *actor)
{
    struct Target *target;
    struct Vec pos;

    target = Object_GetById(PARTY_STATE[125]);
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
        Camera_WorldToScreen(&pos);
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
        BattleFx_ClearOwnedSlot(actor);
    }
    pos.x = actor->x;
    pos.z = actor->z;
    Math_AdvanceVectorByAngle(actor->yaw << 16, actor->pitch << 11, &pos);
    actor->screenX = pos.x;
    actor->screenZ = pos.z;
}
