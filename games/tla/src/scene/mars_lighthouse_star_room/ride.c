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
void Func_020041ec(void *handle);
void Func_02002148(void);
void Func_0200236c(void);
void Func_02002ad0(struct Actor *object);

s32 Func_02001f24(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a;
}

void Func_02001f34(s32 index, s16 value) {
  ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a = value;
}

s32 Func_02001f44(void) {
  return ((struct RideWork *)(gData_020023c4 + 0x188))->counter;
}

s32 Func_02001f50(s32 limit) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);

  if (!GameFlag_Test(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

void Func_02001f78(s32 index, s32 value) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}

void Func_02001f98(s32 index, s16 value) {
  ((struct RideWork *)(gData_020023c4 + 0x188))->entries[index].c = value;
}

s32 Func_02001fa8(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->b;
}

void Func_02001fb8(u32 index, s32 value) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);

  if (index <= 3)
    work->slots[index].value = value;
}
