#include "../shared/routines.h"
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Func_02002270(void) {
  struct MarsLighthouseEntranceRideHead *work =
      (struct MarsLighthouseEntranceRideHead *)(gData_020023c4 + 0x188);
  struct Actor *object;
  struct MarsLighthouseEntranceAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWords(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02004bc0(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

#include "ride.h"

void Func_020022c4(void) {
  struct RideWork *work = RIDE_WORK;

  Func_02004be0(work->handle);
  work->handle = NULL;
}

void Func_020022d8(s16 value) { RIDE_WORK->value0e = value; }

#include "io_reg.h"
#include "field.h"

void Func_020020ec(void);

DEFINE_FUNC_0200218C(Func_020022e4, Func_020020ec)

s32 Func_02002418(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a;
}

void Func_02002428(s32 index, s16 value) {
  ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a = value;
}

s32 Func_02002438(void) { return RIDE_WORK->counter; }

s32 Func_02002444(s32 limit) {
  struct RideWork *work = RIDE_WORK;

  if (!Event_GetFlag(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

void Func_0200246c(s32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}

void Func_0200248c(s32 index, s16 value) {
  RIDE_WORK->entries[index].c = value;
}

s32 Func_0200249c(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->b;
}

void Func_020024ac(u32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].value = value;
}

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

DEFINE_FUNC_02001FCC(Func_020024c0)

#include "../../field/field_effect/advance_spin.c"

DEFINE_FUNC_020012BC(Func_02002614, 10, 4, 0)
