#include "local.h"

void Func_02001ca0(s32 a, s32 b) {
  struct Actor **work = Ram_HeapSlots->effectWork;

  work[5] = Actor_GetById(9);
  Func_02001190(a, b);
  GameFlag_SetBit(0x350);
}

void Func_02001cd0(s32 a, s32 b) {
  struct Actor **work = Ram_HeapSlots->effectWork;

  work[5] = Actor_GetById(11);
  Func_02001190(a, b);
  GameFlag_SetBit(0x351);
}

struct EffectConfig {
  s32 kind;
  s32 variant;
  u8 unk08[16];
  u16 id;
  u8 unk1a[2];
  const u8 *data;
  u8 unk20[8];
};

extern const u8 gData_0200502c[];

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   struct EffectConfig *config);

void Func_02001d04(struct Actor *object) {
  if (++object->action > 64)
    object->update = 0;
  if ((object->action & 1) == 0) {
    struct EffectConfig config;

    config.id = 403;
    config.kind = 2;
    config.variant = 10;
    config.data = gData_0200502c;
    Func_0200015c(object->x, object->y, object->z, 0, 0x1000, 0, 0x330000,
                  &config);
  }
}
