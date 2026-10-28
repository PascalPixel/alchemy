#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
s32 GameFlag_Test(s32 flag);
void Func_02003130(void *handle);
void Func_020024e4(void);
void Func_02002708(void);
void Func_02002e6c(struct Actor *object);
#include "sequence_parent_work.h"

void Func_02003118(struct Actor *object, s32 value);
void Func_020032b8(s32 value);
void Func_02003238(s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void FieldEvent_Wait(s32 frames);
void Func_0200166c(void);
void Task_RemoveFar(void (*func)(void));
void Func_0200055c(void);
void Func_0200170c(void);

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

void Func_02000058(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  object->y = 0xe0000;
  Actor_SetSingleChildFlags(object, 0);
  Func_02003118(object, value);
}
