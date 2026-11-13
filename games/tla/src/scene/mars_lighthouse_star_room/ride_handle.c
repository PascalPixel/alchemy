/// @file
/// Scene code of overlay 0x6ab, in ROM order.

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
void Actor_Release(struct Actor *object);
void MarsLighthouseStarRoom_SlideOnIce(void);

DEFINE_FUNC_02001DD0(Func_02001dd0, Actor_Release)

void Func_02001de4(s16 value) {
  ((struct RideWork *)(gData_020023c4 + 0x188))->value0e = value;
}

#include "io_reg.h"
#include "field.h"

/* VIEW(ride_handle): tag struct MarsLighthouseBeam as struct MarsLighthouseBeam__ride_handle; module/scene/scene.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room/ride_handle.c:135: structure has no member named `stopCount'. */
struct MarsLighthouseBeam__ride_handle {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  u16 stopCount;
  u16 stops[7];
};

void Func_02001df0(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam__ride_handle *beam =
      (struct MarsLighthouseBeam__ride_handle *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (Event_GetFlag(0x109) == 0) {
    i = 0;
    id = *targets++;
    {
      /* FAKEMATCH(dead): a test combine folds, keeping the first loop's refs
       * unweighted; as no test, 90 of 145 instructions differ in tla-en. */
      s32 never = id & 0x40;
      if (never < 0) {
        id = 0;
        goto afterTarget;
      }
    }
    if (id > 0) {
    nextTarget:
      {
        s32 flags = *targets;
        s32 height = flags & 15;
        struct Actor *object;
        s32 one;

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
    afterTarget:
      i++;
      if (i <= 3) {
        if ((id = *targets++) > 0)
          goto nextTarget;
      }
    }
    if (stops != NULL) {
      s32 count;
      s32 stopOffset;
      u16 *p;

      i = 0;
      count = *stops++;
      stopOffset = 84;
      *(u16 *)((u8 *)beam + stopOffset) = count;
      {
        /* FAKEMATCH(dead): a test combine folds, keeping the second loop's refs
         * unweighted; as no test, 133 of 145 instructions differ in tla-en. */
        s32 never = count & 0x40;
        if (never < 0) {
          p = stops;
          goto afterStop;
        }
      }
      if (count > 0) {
        p = &beam->stopCount;
      nextStop:
        p[1] = *stops;
      afterStop:
        stops++;
        i++;
        p += 2;
        if (i <= 3) {
          s32 stop = *stops++;

          *p = stop;
          if (stop > 0)
            goto nextStop;
        }
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 control = 0x3f10;
      REG_BLDCNT = control;
    }
  }
  Task_AddFrame(Func_02001bf8, 0x480);
}

s32 Func_02001f24(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a;
}

void Func_02001f34(s32 index, s16 value) {
  ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a = value;
}

s32 Func_02001f44(void) {
  return ((struct RideWork *)(gData_020023c4 + 0x188))->counter;
}

DEFINE_FUNC_02001F50(Func_02001f50)

DEFINE_FUNC_02001F78(Func_02001f78)

void Func_02001f98(s32 index, s16 value) {
  ((struct RideWork *)(gData_020023c4 + 0x188))->entries[index].c = value;
}

s32 Func_02001fa8(s32 index) {
  return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->b;
}

DEFINE_FUNC_02001FB8(Func_02001fb8)

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

DEFINE_FUNC_02001FCC(Func_02001fcc)

#include "../../field/field_effect/advance_spin.c"

DEFINE_FUNC_020012BC(Func_02002120, 10, 4, 0)
