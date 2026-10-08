/* Near miss: listing 76 instructions, draft 73, common 36, differ 77; the rest differs in instruction order. */
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

void Func_080e446c(u8 *source, u8 *destination, s32 phase)
{
    s32 width = 32;
    s32 height = 32;
    s32 column;
    s32 row;

    Iwram_ClearWords(destination, 0x400);
    for (column = 0; column < width; column++) {
        s32 wave = (Trig_Sin(column << 13) << 3) + 0x80000;

        for (row = 0; row < height; row++) {
            s32 shift = (phase * 0x14ccc - 0x7c0000 + row * 0x40000 - wave) >> 16;

            if (shift < 0) {
                destination[row * width + column] = source[row * width + column];
            } else if (shift + row < height) {
                destination[((shift + row) << 5) + column] = source[row * width + column];
            }
        }
    }
}
