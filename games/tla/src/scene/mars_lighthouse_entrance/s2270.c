#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_inline.h"

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

void Func_02004bc0(struct Actor *object, s32 value);

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
  animation = object->animation;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

#include "ride.h"

void Func_02004be0(void *handle);

void Func_020022c4(void) {
  struct RideWork *work = RIDE_WORK;

  Func_02004be0(work->handle);
  work->handle = NULL;
}

void Func_020022d8(s16 value) { RIDE_WORK->value0e = value; }

#include "io_reg.h"
#include "field.h"

/// One object the beam reaches, as Func_020022e4 records it.
struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  struct {
    u16 unk00;
    u16 unk02;
  } stops[4];
};

void Func_020020ec(void);

void Func_020022e4(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;
  s32 value;
  s32 one;
  s32 flags;
  s32 height;
  struct Actor *object;
  s32 alpha;
  s32 control;

  if (Event_GetFlag(0x109) == 0) {
    for (i = 0; i < 4 && (id = *targets++) > 0; i++) {
      flags = *targets;
      height = flags & 15;
      flags &= 0xf0;
      object = Actor_Get(id);
      targets++;
      if (object != NULL) {
        Actor_SetSingleChildFlags(object, 0);
        ((u8 *)object)[98] = one = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = one;
        target->flags = flags;
        target->object = Actor_Get(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i < 4; i++) {
        value = *stops++;
        beam->stops[i].unk00 = value;
        if (value <= 0)
          break;
        beam->stops[i].unk02 = *stops++;
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    alpha = 0xc08;
    REG_BLDALPHA = alpha;
    control = 0x3f10;
    REG_BLDCNT = control;
  }
  Task_AddFrame(Func_020020ec, 0x480);
}

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

void Func_02004da8(s32 a, s32 b);
struct BattleUnit *Unit_Get(s32 owner);

void Func_020024c0(void) {
  struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;
  s32 leader = FieldParty_GetLeaderActor();
  s32 count;
  s32 i;
  s16 hp[8];

  (void *)Actor_Get(leader);
  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;
  Func_02004da8(-10, 0);
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

void Func_02002614(struct Actor *object) {
  switch (gFrameCount & 7) {
  case 0:
    Animation_ApplyChildValues(object, 10);
    break;
  case 4:
    Animation_ApplyChildValues(object, 0);
    break;
  }
}
