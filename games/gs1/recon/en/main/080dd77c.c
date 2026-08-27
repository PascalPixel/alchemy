#include "types.h"
#include "global_cells.h"
#include "effect_step.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

volatile int Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080e0524(s32, s32, s32, s32);
void Func_080de2f8(void *, s32, s32, s32, void *, void *);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_080b50e8(s32);
void Func_08004278(s32);
void Func_080072f4(s32, s32, s32, s32, s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void EffectPosition_ApplyStepAndYOffset(s32, struct EffectPosition *);

extern s8 Data_080cd260[16];

void Func_080dd77c(void *arg0)
{
    u32 *cell = (u32 *)ADDR_03001EEC;
    u32 base = *cell;
    u32 second = *(cell + 1);
    struct EffectPosition local_a;
    struct EffectPosition local_b;
    s32 mid;
    s32 frame;
    s32 col;
    s32 count;
    s32 i;
    s32 j;

    M2C_FIELD(base, void **, 0x7828) = arg0;
    Func_080de2f8(arg0, 4, M2C_FIELD(arg0, s32 *, 4), 4, &local_a, &local_b);

    Func_080cd594(1);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000050 = 0;
    Func_080e0524(0xa6, base, 1, 1);

    EffectPosition_ApplyStepAndYOffset(*(s16 *)((u8 *)arg0 + 36), &local_a);
    EffectPosition_ApplyStepAndYOffset(
        *(s16 *)((u8 *)arg0 + M2C_FIELD(arg0, s32 *, 20) * 2 + 34), &local_b);
    mid = local_a.x + (local_b.x - local_a.x) / 2;
    local_a.x = mid;
    *(u16 *)0x04000028 = (64 - mid) << 8;

    Func_080ed408(0x2e, 7, 7, 3, 1);
    Func_080ed408(0x2f, 7, 7, 7, 1);

    for (i = 0; i < 16; i++)
        M2C_FIELD(base, s32 *, 0x7080 + i * 28) = (s32)Data_080cd260[i] + 64;

    M2C_FIELD(base, s32 *, 0x7780) = 1;
    M2C_FIELD(base, s32 *, 0x7784) = 0;
    Func_08004278(0x080cd261);

    for (frame = 0; frame < 70; frame++) {
        if (frame == 32) {
            Func_080f9010(0x8f);
            count = M2C_FIELD(M2C_FIELD(base, s32 *, 0x7828), s32 *, 20);
            for (j = 0; j < count; j++)
                Func_080d6888(j, 7, 16, j, *(s16 *)((u8 *)arg0 + 36 + j * 2));
        }

        for (col = 0; col < 16; col++) {
            if (frame == col * 4 + 5)
                M2C_FIELD(base, s32 *, 0x7080 + col * 28 + 8) = 2;

            if (frame > col * 2 + 4) {
                s32 width;
                if (frame >= col * 2 + 32) {
                    width = 160 - (frame - col * 2) * 4;
                } else {
                    width = (frame - col * 2) * 4 - 16;
                    if (width > 32)
                        width = 32;
                }
                if (width > 0)
                    Func_080072f4(second, 32, 7, width, col * 28 + 104);
            }
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        M2C_FIELD(base, s32 *, 0x7828) = 1;
        Func_080030f8(1);
    }

    Func_08004278(0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}
