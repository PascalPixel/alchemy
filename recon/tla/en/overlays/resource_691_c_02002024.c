/* Draft, differ 55 of 76: four random effect 403 spawns around (x, z), then
 * flag 0x8dc. The instructions are the native's but the registers are not:
 * GCSE hoists the position array's address out of the goto loop (the native
 * recomputes add r5, sp, #16 on every pass), so that address takes r7, the
 * config pointer r8, and the mask 63 r6 where the native keeps it in r8. As a
 * for or do loop (differ 45), loop.c hoists 63 and 0xffe00000 and counts
 * down; the native counts up to 4 and hoists neither. */
#include "types.h"

struct MagmaRockEffectConfig {
    s32 kind;
    s32 variant;
    s32 unknown_08;
    s32 unknown_0c;
    u8 unknown_10[8];
    u16 id;
    u8 unknown_1a[2];
    const void *data;
    u8 unknown_20[8];
};

extern u32 Data_02003d50[];

void GameFlag_SetBit(s32 flag);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, struct MagmaRockEffectConfig *config);

void Func_02002024(s32 x, s32 z)
{
    struct MagmaRockEffectConfig config;
    struct MagmaRockEffectConfig *cfg = &config;
    s32 position[3];
    s32 i;

    cfg->unknown_08 = 0x20000;
    cfg->unknown_0c = 0x20000;
    cfg->id = 403;
    cfg->kind = 1;
    cfg->data = Data_02003d50;
    i = 0;
loop:
    position[0] = x + ((Random16Far() & 63) << 16) - 0x200000;
    position[1] = (Random16Far() & 15) << 12;
    position[2] = z + ((Random16Far() & 63) << 16) - 0x200000;
    Func_0200015c(position[0], 0, position[2], 0, position[1], 0, 0x3a0001, cfg);
    i++;
    if (i != 4)
        goto loop;
    GameFlag_SetBit(0x8dc);
}
