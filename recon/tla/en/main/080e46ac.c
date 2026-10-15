/* Near miss, differ 2 with two barriers (6 without): the native loads the tile mask from the pool before it scales the frame (ldr r3; lsls r2, r2, #3); here the shift comes first, and no barrier can hoist a pool constant. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "iwram_call.h"

struct EffectSprite {
    u8 unk00[4];
    u16 attr0_0 : 13;
    u16 color : 1;
    u16 attr0_14 : 2;
    u16 attr1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unk0a[2];
    s32 x;
    s32 y;
    s32 scaleX;
    s32 scaleY;
    u8 unk1c[2];
    u16 depth;
    u8 unk20[8];
};

struct EffectScene {
    u16 mode;
    u8 unk02[14];
    struct ObjectRuntime *mainObject;
};

struct EffectParticle {
    s32 x;
    s32 y;
    s32 z;
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s32 timer;
};

void Func_080eaf98(struct EffectSprite *sprite, s32 width, s32 height, u32 shape, s32 tiles);
s32 Func_080db9cc(struct ObjectRuntime *object);
s32 Func_080db9c0(struct ObjectRuntime *object);
void Func_080eb01c(struct EffectSprite *sprite);
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void BattleFx_IntegrateVector2(struct EffectParticle *particle, s32 frames, s32 flags);
u32 Random16(void);
s32 Trig_Sin(s32 angle);

void Func_080e46ac(struct EffectSprite *sprite, struct EffectParticle *particle, s32 unused, s32 tiles)
{
    s32 position[3];
    s32 scale;
    s32 n;

    if ((u32)particle->timer <= 31) {
        n = (particle->timer >> 2) & 7;
        /* FAKEMATCH: draft barrier; the header gives the measured difference. */
        __asm__("");
        n <<= 3;
        sprite->tile = tiles + n;
        /* FAKEMATCH: draft barrier; the header gives the measured difference. */
        __asm__("");
        scale = sprite->scaleX + 0x1999;
        sprite->scaleX = scale;
        if (scale > 0x10000)
            scale = 0x10000;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        position[0] = particle->x;
        position[1] = particle->y;
        position[2] = particle->z;
        Camera_WorldToScreen(position);
        sprite->x = position[0];
        sprite->y = position[2];
        Func_080eb01c(sprite);
        BattleFx_IntegrateVector2(particle, 60, 0);
    }
    particle->timer++;
}
