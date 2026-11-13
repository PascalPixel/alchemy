/// @file
/// Scene code of overlay 0x6ad, in ROM order.

#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"

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

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020041ec(void *handle);
void Func_0200236c(struct Actor *object, s32 unused);

s32 Func_02002758(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a;
}

void Func_02002768(s32 index, s16 value) {
  ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a = value;
}

s32 Func_02002778(void) {
  return ((struct RideWork *)(gData_020023c4 + 0x188))->counter;
}

DEFINE_FUNC_02001F50(Func_02002784)

DEFINE_FUNC_02001F78(Func_020027ac)

void Func_020027cc(s32 index, s16 value) {
  ((struct RideWork *)(gData_020023c4 + 0x188))->entries[index].c = value;
}

s32 Func_020027dc(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->b;
}

DEFINE_FUNC_02001FB8(Func_020027ec)

#include "main_routine_work.h"
#include "battle_unit.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

struct BattleUnit *Unit_Get(s32 owner);

DEFINE_FUNC_02001FCC(Func_02002800)

#include "../../field/field_effect/advance_spin.c"
