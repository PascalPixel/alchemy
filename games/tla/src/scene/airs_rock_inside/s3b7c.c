#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/io_reg.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void wait(s32 frames);
void Func_02005d9c(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
void Func_02005d7c(struct Actor *object);

void Func_02003b7c(s32 x, s32 y) {
  struct Actor *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Import_ActorCreate(288, x << 16, 0, y << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(marker, 0);
  Actor_SetSingleChildFlags(marker, 0);
  sprite = marker->animation;
  sprite->blendMode = 2;
  Func_02005e14();
  Func_02005f2c(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Func_02005f94(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  wait(1);
  i++;
  if (i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  wait(1);
  i++;
  if (i != 16)
    goto fadeOut;
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  Call6((void (*)())Func_02005d9c, 65, 1, cellX + 64, cellY, 1, 2);
  Func_0200062c(0, cellX, cellY, 0xff00);
  Func_02005d7c(marker);
  Func_02005e1c();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}
