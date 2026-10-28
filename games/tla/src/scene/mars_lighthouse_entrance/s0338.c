#include "room.h"
#include "field_effect.h"

struct EffectHolder {
  u8 unk00[0x1a];
  s16 objectId;
};

void GameFlag_SetBit(s32 flag);
void Func_020001f8(struct Actor *object);
void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

/// While index is at most 7, spawns effect type 0x117 index steps below the
/// object the effect work names and shrinks the object a little; mode 1
/// restores its size, sets its flag, hands it to Func_020001f8 and clears its
/// position.
void Func_02000338(s32 mode, s32 index) {
  struct EffectHolder *holder = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(holder->objectId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = 0x117;
    options.startScaleX = options.startScaleY = 0x10000;
    options.targetScaleX = options.targetScaleY = 0x8000;
    options.rotation = (Random16Far() & 15) << 12;
    Func_02003b30(object->x, object->y - index * 3 * 0x10000 + 0x100000,
                  object->z, ((Random16Far() & 15) - 8) << 13, 0, 0, 0x5a0000,
                  &options);
    object->scaleY -= 0x1f00;
  }
  if (mode == 1) {
    object->scaleY = 0x10000;
    GameFlag_SetBit(object->action);
    Func_020001f8(Actor_GetById(holder->objectId));
    object->x = object->y = object->z = 0;
  }
}
