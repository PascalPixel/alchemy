#include "types.h"
#include "actor.h"
#include "system/io_reg.h"
#include "fixed_math.h"
#include "event_inline.h"

void Func_02004e5c(void);
void Func_02004ddc(void);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02001c9c(void);
void Func_02004dbc(struct Actor *object);
void Func_02004dd4(void);
s32 Func_02004e64(void);

/// Unless flag 0x9ee or 0x9ba is set: fades the blend out, lowers person 12 by
/// one unit every 2 frames for 20 steps, with Func_02001c9c's ring at the start
/// and halfway, places object 323, sets flag 0x9ee, and fades the blend back.
void Func_02001d14(void) {
  struct Actor *person;
  struct Actor *object;
  /* FAKEMATCH(volatile): the form kept; as u16 level, 146 of 173 instructions
   * differ in tla-en. */
  volatile u16 level;
  s32 i;

  if (GameFlag_Test(0x9ee) || GameFlag_Test(0x9ba))
    return;
  person = Actor_GetById(12);
  level = REG_BLDALPHA;
  Func_02004e5c();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): the form kept; as a direct call, 13 of 173
   * instructions differ in tla-en. */
  MapGrid_SetShake(Q16_ONE * 10, Q16_ONE * 10, Q16_ONE);
  Sound_Play(107);
  Func_02004ddc();
  level &= 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    wait(1);
  }
  REG_DISPCNT &= ~0x200;
  Event_Wait(30);
  object =
      OverlayObject_PrepareObject(232 << 17, -Q16_ONE * 32, 174 << 17, 323);
  Actor_SetHorizontalPositionWithTerrain(12, 232 << 17, 174 << 17);
  Func_02001c9c();
  for (i = 0; i <= 19; i++) {
    if (i == 10)
      Func_02001c9c();
    person->z -= Q16_ONE;
    Event_Wait(2);
  }
  GameFlag_SetBit(0x9ee);
  /* FAKEMATCH(wrapper): the form kept; as a direct call, 25 of 173
   * instructions differ in tla-en. */
  MapGrid_SetShake(-1, -1, 0xe666);
  object->x = 0;
  object->z = 0;
  Event_Wait(1);
  Func_02004dbc(object);
  Sound_Play(390);
  Event_Wait(30);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  while (level != 8) {
    REG_BLDALPHA = level | 0x1000;
    level++;
    wait(1);
  }
  Func_02004dd4();
  Func_02004e64();
}
