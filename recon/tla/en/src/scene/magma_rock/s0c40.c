#include "types.h"
#include "call.h"
#include "actor.h"

struct MagmaRockEffectParams {
  s32 unk00;
  u8 unk04[0x14];
  u16 kind;
  u8 unk1a[2];
  const void *script;
  u8 unk20[8];
};

extern u32 gFrameCount;
extern const u8 gData_02003d74[];

u32 Random16Far(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_0200015c(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct MagmaRockEffectParams *params);
void Func_02000dc0(void);

void Func_02000c40(struct Actor *object) {
  struct Actor *linked = object->watchedObject;
  s16 *timer = &object->action;

  if (*timer < 50) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct MagmaRockEffectParams params;
      s32 dx;
      s32 dy;

      params.kind = 403;
      params.unk00 = 1;
      params.script = gData_02003d74;
      {
        s32 mask = 15;

        dx = mask & Random16Far();
      }
      dy = Random16Far() & 31;
      Func_0200015c(object->x + ((dx - 8) << 16),
                    object->y + (dy << 16) - 0x80000, object->z, 0, phase,
                    phase, SPLIT_CONSTANT(0x320001), &params);
    }
    if (gFrameCount & 4)
      Actor_SetPartAttribute(object, 10);
    else
      Actor_SetPartAttribute(object, 0);
  } else if (*timer < 100) {
    if (gFrameCount & 4)
      Actor_SetPartAttribute(object, 10);
    else
      Actor_SetPartAttribute(object, 0);
  } else if (*timer < 140) {
    if (gFrameCount & 2)
      Actor_SetPartAttribute(object, 10);
    else
      Actor_SetPartAttribute(object, 0);
  } else if (*timer < 144) {
    Actor_SetPartAttribute(object, 7);
  } else {
    Actor_SetPartAttribute(object, 0);
    object->update = NULL;
    linked->x = object->x;
    linked->y = object->y;
    linked->z = object->z;
    linked->update = Func_02000dc0;
  }
  (*timer)++;
}
