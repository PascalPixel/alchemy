#include "types.h"

void Func_080040e8(void);
void Func_080041d8(s32 message, s32 code);
void Func_080f037c(u32 *destination);
void Func_080f0254(s32 layer);
void Func_080f0678(void);
void Func_080030f8(u32 frames);
void Func_080f02b0(s32 scene, s32 direction);
void Func_0800479c(void);
void Func_08004760(void);

s32 Func_080f03f0(void)
{
    const s32 *scenes = (const s32 *)0x080f0a5c;
    volatile u16 *display_control = (volatile u16 *)0x04000000;
    volatile u16 *blend_control = (volatile u16 *)0x04000050;
    volatile u16 *blend_alpha = (volatile u16 *)0x04000052;
    s32 scene_index;

    *(u8 *)0x03001d18 = 0;
    *(u8 *)0x03001f58 = 0;
    *(u8 *)0x03001ac4 = 0;
    *(u8 *)0x03001d08 = 0;
    Func_080040e8();
    Func_080041d8(0x080f03c1, 0x480);

    *display_control = 0x40;
    Func_080f037c((u32 *)0x06007800);
    Func_080f037c((u32 *)0x0600f800);
    Func_080f0254(0);
    Func_080f0254(1);
    *(volatile u16 *)0x0400000c = 0x1f8a;
    *(volatile u16 *)0x0400000e = 0x0f83;
    *display_control = 0x1c40;
    *blend_control = 0x2844;
    Func_080f0678();
    Func_080030f8(300);

    for (scene_index = 0; scene_index <= 32; scene_index++) {
        s32 step;
        s32 odd = scene_index & 1;

        Func_080f02b0(scenes[scene_index], odd ^ 1);
        for (step = 1; step <= 16; step++) {
            if (odd)
                *blend_alpha = (step << 8) | (16 - step);
            else
                *blend_alpha = ((16 - step) << 8) | step;
            Func_080030f8(4);
        }
        Func_080030f8(267);
    }

    *blend_control = 0;
    *display_control = 0x1040;
    Func_0800479c();
    Func_08004760();
    *(u8 *)0x03001d18 = 1;
    return 0;
}
