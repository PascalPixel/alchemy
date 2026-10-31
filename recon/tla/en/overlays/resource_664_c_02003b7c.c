// Near miss, differ 4 of 122 (tla-en, 2026-10-31): sched2 builds
// REG_DISPCNT's address before the REG_WINOUT store and before the REG_BLDCNT
// store at the end, where the native builds it after each. A marker of type
// 288 at (x, y) blends in over 8 frames and out over 16 inside an event with
// cue 106, then its 1 by 2 cells are copied from (65, 1) to the marked place,
// the cell's kind is set to 0xff00 and the marker removed. The same listing as
// overlay 692's Func_020026a4. Measured: each fade steps its counter after the
// wait (sched2 lifts the step above the call, as the native has it; 18 to 14),
// and the cell copy forwarded through Call6 gives its two stack arguments their
// own registers (14 to 4; the same 4 as through a local inline). Why it stays:
// in sched2 the address's first instruction has priority 39 and the window
// store 38, both feeding the REG_DISPCNT load, so no plain C reorders them;
// non-volatile register accesses change nothing (4). Any barrier also adds a
// dependent to the last uses of the saved registers and flips argument ties:
// an empty one-pass block after the REG_WINOUT store fixes that pair but moves
// x << 16 and the marker copy (8 with the end pair); one after the REG_BLDCNT
// store fixes the end but moves cellX's argument (4); a one-pass block around
// Func_02005e1c and the REG_BLDCNT store moves 0xff00's shift (4); one around
// the REG_DISPCNT update 10. Tried before: a one-pass block around either
// store, as Anemos Inner Sanctum's Func_020019a8 has (the WINOUT one moves the
// literal pool, 16; the BLDCNT one 6), an empty asm after the WINOUT store
// (16), both statements in one block (20), 10 minutes of alchemy permute.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/io_reg.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
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
