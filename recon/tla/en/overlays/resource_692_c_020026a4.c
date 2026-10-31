// Near miss, differ 4 of 122 (tla-en, 2026-10-31): scheduler order only. The
// native builds the display control's address after the window store before it
// and after the blend store at the end; sched2 ranks the address's movs
// (priority 39, two insns ahead of the ldrh) above the store (38, one ahead),
// with the same dependences and registers as the native, so no C form moves it.
// An empty asm or one-pass block there: 8 or more (a one-pass after the window
// store ends sched2's first block and reorders the prologue's shifts and
// Func_02000694's arguments; after the blend store, those arguments: 4).
// Unchanged at 4: REG_DISPCNT = REG_DISPCNT | DISPCNT_OBJWIN_ON, 0x8000u, the
// WINOUT_* and ~DISPCNT_OBJWIN_ON spellings; a u16 or u32 temporary for the
// display control: 8; a temporary for the window value: 35. The permuter (4
// minutes, 21204 candidates) found nothing better.
// A glow at cell (x, z): creates a blended object (type 288) there with sound
// 106, fades its blend in over 8 frames and out over 16 inside the object
// window, then copies the 1 by 2 cells from 64 columns across (Func_02004cd8),
// clears the cell's high byte (Func_02000694) and removes the object. The copy
// goes through Call6, which keeps the stack arguments 1 and 2 in their own
// registers (called directly: 14).
#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "actor.h"
#include "field_sprite.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e90(s32 value);
void Func_02004f30(s32 sound);
void wait(s32 frames);
void Func_02004cd8(s32 a, s32 b, s32 x, s32 z, s32 width, s32 height);
void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);
void Func_02004cb0(struct Actor *object);

void Func_020026a4(s32 x, s32 z) {
  struct Actor *object;
  s32 i;
  s32 cellZ;

  object = Import_ActorCreate(288, x << 16, 0, z << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(object, 0);
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->blendMode = 2;
  Func_02004d40();
  Func_02004e90(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Func_02004f30(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  wait(1);
  if (++i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  wait(1);
  if (++i != 16)
    goto fadeOut;
  x >>= 4;
  cellZ = (z >> 4) - 1;
  Call6((void (*)())Func_02004cd8, 65, 1, x + 64, cellZ, 1, 2);
  Func_02000694(0, x, cellZ, 0xff00);
  Func_02004cb0(object);
  Func_02004d48();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}
