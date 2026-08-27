#include "types.h"
#include "global_cells.h"
#include "effect_step.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

volatile int Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080e0524(s32, s32, s32, s32);
void Func_080041d8(s32, s32);
void Func_08004278(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_080b50e8(s32);
void Func_080072f4(s32, s32, s32, s32, s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080cd52c(void);
void EffectPosition_ApplyStepAndYOffset(s32, struct EffectPosition *);

void Func_080ccebc(void *arg0)
{
    u32 *cell = (u32 *)0x03001ef0;
    u32 base = *(cell - 1);
    u32 second = *cell;
    struct EffectPosition local1;
    struct EffectPosition local2;
    s32 mid;
    s32 frame;
    s32 flash;
    s32 shade;
    s32 i;
    s32 count;

    M2C_FIELD(base, void **, 0x7828) = arg0;
    Func_080cd594(2);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000052 = 0x1000;

    EffectPosition_ApplyStepAndYOffset(*(s16 *)((u8 *)arg0 + 36), &local1);
    EffectPosition_ApplyStepAndYOffset(
        *(s16 *)((u8 *)arg0 + M2C_FIELD(arg0, s32 *, 20) * 2 + 34), &local2);
    mid = local1.x + (local2.x - local1.x) / 2;
    local1.x = mid;

    *(u16 *)0x04000028 = (64 - mid) << 8;
    Func_080e0524(0x59, base, 1, 1);

    M2C_FIELD(base, s32 *, 0x7780) = 1;
    M2C_FIELD(base, s32 *, 0x7784) = 0;
    Func_080041d8(0x080cd261, 0x480);

    Func_080f9010(0x8f);

    frame = 0;
    flash = 1;
    shade = 32;
    for (;;) {
        if (frame <= 8)
            *(u16 *)0x04000028 |= (frame << 1) | 0x1000;
        if (frame > 53)
            *(u16 *)0x04000028 |= (0x7c - (frame << 1)) | 0x1000;

        Func_080ed408(0x2e, 7, 7, 3, flash);
        Func_080072f4(second, shade, shade, 33, 41);
        Func_08002dd8(0x2e);

        Func_080ed408(0x2e, 7, 7, 7, flash);
        Func_080072f4(second, shade, shade, 64, 41);
        Func_08002dd8(0x2e);

        Func_080ed408(0x2e, 7, 7, 11, flash);
        Func_080072f4(second, shade, shade, 33, 72);
        Func_08002dd8(0x2e);

        Func_080ed408(0x2e, 7, 7, 15, flash);
        Func_080072f4(second, shade, shade, 64, 72);
        Func_08002dd8(0x2e);

        if (frame == 32)
            Func_080b50e8(0x8f);

        count = M2C_FIELD(base, s32 *, 0x7828);
        count = M2C_FIELD(count, s32 *, 20);
        if (count != 0) {
            for (i = 0; i < count; i++) {
                if (frame == 10) {
                    void *p = M2C_FIELD(base, void **, 0x7828);
                    Func_080d6888(i, 7, -1, 8, *(s16 *)((u8 *)p + 36 + i * 2));
                }
            }
        }

        Func_080cd52c();
        M2C_FIELD(base, s32 *, 0x77a4) = flash;
        Func_080030f8(1);

        frame++;
        if (frame == 63)
            break;
    }

    Func_08004278(0x080cd261);
    Func_080cdbc0();
}
