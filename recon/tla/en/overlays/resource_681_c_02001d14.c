// Draft, differ 88 of 173: before flags 0x9ee and 0x9ba, the screen blends out,
// person 12 sinks into the floor over forty frames with two rings of effects,
// a boulder drops in its place (flag 0x9ee) and the screen blends back.
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "system/io_reg.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004e5c(void);
void Func_02004fcc(s32 mode);
void Func_02004e24(s32 x, s32 y, s32 z);
void Func_02005014(s32 cue);
void Func_02004ddc(void);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *OverlayObject_PrepareObject(s32 x, s32 y, s32 z,
                                                  s32 type);
void Func_02004ebc(s32 id, s32 x, s32 z);
void Func_02001c9c(void);
void Func_02004dbc(struct ObjectRuntime *object);
void Func_02004dd4(void);
s32 Func_02004e64(void);

void Func_02001d14(void) {
  struct ObjectRuntime *person;
  struct ObjectRuntime *boulder;
  volatile u16 level;
  s32 i;

  if (GameFlag_Test(0x9ee) || GameFlag_Test(0x9ba))
    return;
  person = Object_GetById(12);
  level = REG_BLDALPHA;
  Func_02004e5c();
  Func_02004fcc(0);
  Call3((void (*)())Func_02004e24, 0xa0000, 0xa0000, 0x10000);
  Func_02005014(107);
  Func_02004ddc();
  level &= 0xff;
  if (level != 0) {
  fadeOut:
    REG_BLDALPHA = level | 0x1000;
    level--;
    WaitFrames(1);
    if (level != 0)
      goto fadeOut;
  }
  REG_DISPCNT &= ~0x200;
  FieldEvent_WaitFrames(30);
  boulder = OverlayObject_PrepareObject(232 << 17, -0x200000, 174 << 17, 0x143);
  Func_02004ebc(12, 232 << 17, 174 << 17);
  Func_02001c9c();
  i = 0;
sink:
  if (i == 10)
    Func_02001c9c();
  person->z -= 0x10000;
  i++;
  FieldEvent_WaitFrames(2);
  if (i <= 19)
    goto sink;
  GameFlag_SetBit(0x9ee);
  Call3((void (*)())Func_02004e24, -1, -1, 0xe666);
  boulder->x = 0;
  boulder->z = 0;
  FieldEvent_WaitFrames(1);
  Func_02004dbc(boulder);
  Func_02005014(0x186);
  FieldEvent_WaitFrames(30);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  if (level != 8) {
  fadeIn:
    REG_BLDALPHA = level | 0x1000;
    level++;
    WaitFrames(1);
    if (level != 8)
      goto fadeIn;
  }
  Func_02004dd4();
  Func_02004e64();
}
