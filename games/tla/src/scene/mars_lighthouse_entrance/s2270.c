#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "call.h"
#include "iwram_call.h"

struct MarsLighthouseEntranceAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[6];
  u8 value10;
  u8 flag11 : 1;
  u16 angle;
};

struct MarsLighthouseEntranceRideHead {
  s32 counter;
  s16 active;
  u8 unk06[6];
  u16 value0c;
  s16 value0e;
  u8 unk10[0x60];
  struct Actor *handle;
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
void Func_02004bc0(struct Actor *object, s32 value);
struct Actor *Func_02004bd8(s32 kind, s32 x, s32 y, s32 z);

void Func_02002270(void) {
  struct MarsLighthouseEntranceRideHead *work =
      (struct MarsLighthouseEntranceRideHead *)(gData_020023c4 + 0x188);
  struct Actor *object;
  struct MarsLighthouseEntranceAnimation *animation;

  if (!GameFlag_Test(0x109))
    Iwram_ClearWords(work, 116);
  object =
      (struct Actor *)Value4((s32 (*)())Func_02004bd8, 365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02004bc0(object, 1);
  animation = object->animation;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}
