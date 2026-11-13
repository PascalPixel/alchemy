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

s32 Func_02002784(s32 limit) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);

  if (!Event_GetFlag(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

void Func_020027ac(s32 index, s32 value) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}

void Func_020027cc(s32 index, s16 value) {
  ((struct RideWork *)(gData_020023c4 + 0x188))->entries[index].c = value;
}

s32 Func_020027dc(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->b;
}

void Func_020027ec(u32 index, s32 value) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);

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

void Func_02002800(void) {
  struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;
  s32 leader = FieldParty_GetLeaderActor();
  s32 count;
  s32 i;
  s16 hp[8];

  (void *)Actor_Get(leader);
  count = Party_CountMembers();
  for (i = 0; i < count; i++)
    hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;
  BattleParty_ApplyHealthDelta(-10, 0);
  work->downCount = 0;
  work->standingCount = 0;
  Actor_SetAttachedEffect(leader, 258);
  for (i = 0; i < count; i++) {
    struct MainRoutineWork *main = &gMainRoutineWork;
    struct BattleUnit *unit = Unit_Get(main->activeOwners[i]);

    if (unit->hp > 0) {
      work->standingCount++;
    } else if (hp[i] != 0) {
      work->downOwners[(s16)work->downCount++] = main->activeOwners[i];
      work->unk16a = 0xffff;
      ((u8 *)unit)[0x131] = 0;
    }
  }
}

#include "../../field/field_effect/advance_spin.c"
