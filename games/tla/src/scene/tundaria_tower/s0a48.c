#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_effect.h"

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
void GameFlag_SetBit(s32 flag);
void Func_02000338(struct Actor *object);
void Func_02002fc0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

void Func_02000a48(s32 arg0, s32 arg1) {
  struct EffectOptions options;
  s16 *effectWork = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(effectWork[13]);
  s32 velocityX;

  if (arg1 < 8) {
    options.type = 279;
    options.startScaleY = 0x10000;
    options.startScaleX = 0x10000;
    options.priority = 0;
    options.targetScaleY = 0x8000;
    options.targetScaleX = 0x8000;
    options.rotation = (Random16Far() & 15) << 12;
    velocityX = ((Random16Far() & 15) - 8) << 13;
    Func_020000b8(object->x, object->y - ((arg1 * 3) << 16) + 0x100000,
                  object->z, velocityX, 0, 0, 0x5a0000, &options);
    object->scaleY -= 7936;
  }
  if (arg0 == 1) {
    object->scaleY = 0x10000;
    GameFlag_SetBit(object->action);
    Func_02000338(Actor_GetById(effectWork[13]));
    Func_02002fc0(67, 23, (object->x >> 20) + 64, object->z >> 20, 1, 1);
    object->z = 0;
    object->y = 0;
    object->x = 0;
  }
}
