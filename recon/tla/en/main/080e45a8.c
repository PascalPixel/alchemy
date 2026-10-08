/* Near miss: listing 119 instructions, draft 119, common 116, differ 6; the rest differs in instruction order. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "iwram_call.h"

struct EffectSprite {
    u8 unknown_00[4];
    u16 attr0_0 : 13;
    u16 color : 1;
    u16 attr0_14 : 2;
    u16 attr1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[2];
    s32 x;
    s32 y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_1c[2];
    u16 depth;
    u8 unknown_20[8];
};

struct EffectScene {
    u16 mode;
    u8 unknown_02[14];
    struct ObjectRuntime *main_object;
};

struct EffectParticle {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
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

void Func_080e45a8(struct EffectSprite *sprite, struct EffectParticle *particle, s32 unused, s32 tiles, s32 high)
{
    struct ObjectRuntime *main_object = ((struct EffectScene *)Ram_HeapSlots->effect_work)->main_object;
    s32 position[3];
    s32 y;

    position[0] = main_object->x;
    position[1] = y = main_object->y;
    position[2] = main_object->z;
    if (high) {
        position[1] = y + 0x20000;
        Vector_AddPolarOffset(0x20000, Random16(), position);
    } else {
        Vector_AddPolarOffset(0x40000, Random16(), position);
    }
    particle->x = position[0];
    particle->y = position[1];
    particle->z = position[2];
    position[2] = 0;
    position[0] = 0;
    if (high)
        Vector_AddPolarOffset(0x3333, Random16(), position);
    else
        Vector_AddPolarOffset(0xcccc, Random16(), position);
    particle->velocity_x = position[0];
    particle->velocity_y = 0;
    particle->velocity_z = position[2];
    Func_080eaf98(sprite, 8, 8, 0x40000000, tiles);
    sprite->scale_x = 0x3333;
    sprite->scale_y = 0x3333;
    sprite->priority = Func_080db9c0(main_object);
    sprite->depth = Func_080db9cc(main_object) + 2;
    sprite->color = 1;
    sprite->palette = 0;
}
