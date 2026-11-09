#include "room.h"
#include "iwram_call.h"

struct MarsLighthouseRoomFAnimation {
  u8 unk00[0x10];
  u8 value10;
  u8 flag11 : 1;
};

struct MarsLighthouseRoomFRideWork {
  u8 unk00[0x0c];
  u16 value0c;
  u8 unk0e[0x62];
  struct Actor *handle;
};

extern u8 gData_020023c4[];


void Func_020025b0(void) {
  struct MarsLighthouseRoomFRideWork *work =
      (struct MarsLighthouseRoomFRideWork *)(gData_020023c4 + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomFAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWords(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02005480(object, 1);
  animation = object->animation;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}
