#include "types.h"
#include "call.h"
#include "fixed_math.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[8];
};

extern u32 gData_02003d50[];

void GameFlag_SetBit(s32 flag);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);

void Func_02002024(s32 x, s32 z) {
  struct MagmaRockEffectConfig config;
  s32 position[3];
  s32 i;
  s32 kind;

  config.unk08 = Q16_ONE * 2;
  config.unk0c = Q16_ONE * 2;
  config.id = 403;
  kind = 1; // Holds one constant: the native loads 1 before the data address.
  config.data = gData_02003d50;
  config.kind = kind;
  i = 0;
loop:
  position[0] = x + ((Random16Far() & 63) << 16) - Q16_ONE * 32;
  position[1] = (Random16Far() & 15) << 12;
  position[2] = z + ((Random16Far() & 63) << 16) - Q16_ONE * 32;
  Func_0200015c(position[0], 0, position[2], 0, position[1], 0, 0x3a0001,
                &config);
  i++;
  if (i != 4)
    goto loop;
  GameFlag_SetBit(0x8dc);
}
