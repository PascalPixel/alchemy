#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/io_reg.h"
#include "fixed_math.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_02004e5c(void);
void Func_02004fcc(s32 mode);
void Func_02004e24(s32 x, s32 y, s32 z);
void Func_02005014(s32 cue);
void Func_02004ddc(void);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004ebc(s32 id, s32 x, s32 z);
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
  Func_02004fcc(0);
  /* FAKEMATCH(wrapper): the form kept; as a direct call, 13 of 173
   * instructions differ in tla-en. */
  Call3((void (*)())Func_02004e24, Q16_ONE * 10, Q16_ONE * 10, Q16_ONE);
  Func_02005014(107);
  Func_02004ddc();
  level &= 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    wait(1);
  }
  REG_DISPCNT &= ~0x200;
  FieldEvent_Wait(30);
  object =
      OverlayObject_PrepareObject(232 << 17, -Q16_ONE * 32, 174 << 17, 323);
  Func_02004ebc(12, 232 << 17, 174 << 17);
  Func_02001c9c();
  for (i = 0; i <= 19; i++) {
    if (i == 10)
      Func_02001c9c();
    person->z -= Q16_ONE;
    FieldEvent_Wait(2);
  }
  GameFlag_SetBit(0x9ee);
  /* FAKEMATCH(wrapper): the form kept; as a direct call, 25 of 173
   * instructions differ in tla-en. */
  Call3((void (*)())Func_02004e24, -1, -1, 0xe666);
  object->x = 0;
  object->z = 0;
  FieldEvent_Wait(1);
  Func_02004dbc(object);
  Func_02005014(390);
  FieldEvent_Wait(30);
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
