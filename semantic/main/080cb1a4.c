#include "types.h"

typedef void (*Renderer_080cb1a4)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

#define S32_AT_080CB1A4(base, offset) \
    (*(s32 *)((u8 *)(base) + (offset)))

#define MUL_SHIFT_080CB1A4(left, right, shift) \
    ((s32)((u32)(left) * (u32)(right)) >> (shift))

s32 Func_080022ec(s32 dividend, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *address);
void Func_080049ac(void);
void Func_080051d8(void *arg0, void *arg1);
void Func_08009080(void *object, s32 arg1);
void Func_08009140(void *object);
void Func_08009150(void *object, s32 x, s32 y, s32 z);
s32 Func_080b5070(s32 arg0);
void *Func_080b5098(s32 arg0);
void Func_080b50e8(s32 arg0);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 arg0, void *runtime, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3944(const s32 *input, s32 *output);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080cb1a4(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080cb1a4 renderer;
    void *primary;
    void *secondary;
    s32 primary_id;
    s32 secondary_id;
    s32 primary_height;
    s32 secondary_height;
    s32 target_x;
    s32 target_z;
    s32 frame;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    Func_080e0524(0x7d, runtime, 1, 1);
    Func_080ed408(46, 7, 7, 3, 2);
    renderer = (Renderer_080cb1a4)runtime_header[7];
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    primary_id = *(s32 *)((u8 *)argument + 8);
    secondary_id = *(s16 *)((u8 *)argument + 36);
    primary = *(void **)Func_080b5098(primary_id);
    secondary = *(void **)Func_080b5098(secondary_id);

    target_x = S32_AT_080CB1A4(primary, 8) +
        Func_080022ec(
            (s32)(
                (u32)90 *
                ((u32)S32_AT_080CB1A4(secondary, 8) -
                 (u32)S32_AT_080CB1A4(primary, 8))),
            100);
    target_z = S32_AT_080CB1A4(primary, 16) +
        Func_080022ec(
            (s32)(
                (u32)90 *
                ((u32)S32_AT_080CB1A4(secondary, 16) -
                 (u32)S32_AT_080CB1A4(primary, 16))),
            100);
    primary_height = Func_080b5070(primary_id);
    secondary_height = Func_080b5070(secondary_id);

    Func_08009140(primary);
    Func_08009150(primary, target_x, 0, target_z);
    Func_08009080(primary, 2);
    *((u8 *)primary + 88) = 1;
    *((u8 *)primary + 90) = 1;
    S32_AT_080CB1A4(primary, 52) = 0x20000;
    S32_AT_080CB1A4(primary, 48) = 0x80000;
    Func_080030f8(20);

    for (frame = 0; frame < 96; frame++) {
        void *effect_context = *(void **)0x03001e80;
        s32 source_point[3];
        s32 projected[3];

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        if (frame == 0) {
            S32_AT_080CB1A4(secondary, 40) = 0xf00000;
            S32_AT_080CB1A4(secondary, 72) = 0x91eb;
            S32_AT_080CB1A4(primary, 40) = 0xf00000;
            S32_AT_080CB1A4(primary, 72) = 0x91eb;
        }

        if (frame == 11) {
            S32_AT_080CB1A4(secondary, 28) =
                -S32_AT_080CB1A4(secondary, 28);
            S32_AT_080CB1A4(primary, 28) =
                -S32_AT_080CB1A4(primary, 28);
            S32_AT_080CB1A4(primary, 12) += primary_height;
            S32_AT_080CB1A4(secondary, 12) += secondary_height;
        }

        if (frame == 54) {
            Func_080d6888(secondary_id, 7, 5, 0, 10);
            S32_AT_080CB1A4(secondary, 40) = 0x80000;
            S32_AT_080CB1A4(secondary, 72) = 0xab85;
            S32_AT_080CB1A4(primary, 40) = 0x50000;
            S32_AT_080CB1A4(primary, 72) = 0x7851;
            S32_AT_080CB1A4(primary, 52) = 0x10000;
            S32_AT_080CB1A4(primary, 48) = 0x20000;
            *((u8 *)primary + 90) = 0;
            Func_08009140(primary);
            Func_08009150(primary, 0, 0, S32_AT_080CB1A4(primary, 16));
        }

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        source_point[0] = S32_AT_080CB1A4(primary, 8);
        source_point[1] = S32_AT_080CB1A4(primary, 12);
        source_point[2] = S32_AT_080CB1A4(primary, 16);
        Func_080e3944(source_point, projected);
        projected[0] >>= 1;

        if ((u32)(frame - 54) <= 1) {
            renderer(
                render_context,
                runtime,
                projected[0] - 16,
                projected[1] - 16,
                32,
                64);
        }

        if ((u32)(frame - 56) <= 11) {
            s32 base_offset = ((frame - 56) / 2) << 11;
            s32 radius = frame - 46;
            s32 i;

            for (i = 0; i < 16; i++) {
                s32 angle = i << 12;
                s32 x = projected[0] +
                    MUL_SHIFT_080CB1A4(
                        radius,
                        Func_08002322(angle),
                        16) -
                    16;
                s32 y = MUL_SHIFT_080CB1A4(
                    radius,
                    Func_0800231c(angle),
                    16) -
                    frame +
                    100;

                renderer(
                    render_context,
                    runtime + base_offset,
                    x,
                    y,
                    32,
                    64);
            }
        }

        if (frame == 64) {
            S32_AT_080CB1A4(secondary, 28) =
                -S32_AT_080CB1A4(secondary, 28);
            S32_AT_080CB1A4(primary, 28) =
                -S32_AT_080CB1A4(primary, 28);
            S32_AT_080CB1A4(primary, 12) -= primary_height;
            S32_AT_080CB1A4(secondary, 12) -= secondary_height;
            Func_08009080(primary, 0);
        }

        if (frame == 54)
            Func_080b50e8(134);

        if (frame == 0) {
            Func_080f9010(136);
            *(s32 *)(runtime + 0x77a8) = 6;
        }
        if (frame == 53)
            *(s32 *)(runtime + 0x77a8) = 6;

        Func_080e155c(16, 16);
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
