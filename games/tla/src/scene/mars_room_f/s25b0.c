#include "room.h"
#include "call.h"
#include "iwram_call.h"

struct MarsRoomFAnimation {
  u8 unk00[0x10];
  u8 value10;
  u8 flag11 : 1;
};

struct MarsRoomFRideWork {
  u8 unk00[0x0c];
  u16 value0c;
  u8 unk0e[0x62];
  struct ObjectRuntime *handle;
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);

void Func_020025b0(void) {
  struct MarsRoomFRideWork *work =
      (struct MarsRoomFRideWork *)(gData_020023c4 + 0x188);
  struct ObjectRuntime *object;
  struct MarsRoomFAnimation *animation;

  if (!GameFlag_Test(0x109))
    Iwram_ClearWords(work, 116);
  object =
      (struct ObjectRuntime *)Value4((s32 (*)())Func_02005498, 365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02005480(object, 1);
  animation = object->animation;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}
