#include "types.h"
#include "io_reg.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);
void Func_02004cb0(struct Actor *object);

void Func_020026a4(s32 x, s32 z) {
  struct Actor *object;
  s32 i;
  s32 cellZ;

  object = FieldActor_Create(288, x << 16, 0, z << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(object, 0);
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->blendMode = 2;
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Event_PlaySound(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  System_Wait(1);
  if (++i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  System_Wait(1);
  if (++i != 16)
    goto fadeOut;
  x >>= 4;
  cellZ = (z >> 4) - 1;
  MapGrid_CopyCellsTo(65, 1, x + 64, cellZ, 1, 2);
  Func_02000694(0, x, cellZ, 0xff00);
  Func_02004cb0(object);
  Func_02004d48();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}
