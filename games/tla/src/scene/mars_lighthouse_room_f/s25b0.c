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
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"

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

extern u16 gData_02004cf0[];
void Func_020054a0(void *handle);
void Func_02002148(void);
void Func_0200236c(struct Actor *object, s32 unused);
void Func_02002ad0(struct Actor *object);

void Func_02002604(void) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);

  Func_020054a0(work->handle);
  work->handle = NULL;
}

void Func_02002618(s16 value) {
  ((struct RideWork *)(gData_020023c4 + 0x188))->value0e = value;
}

#include "io_reg.h"
#include "field.h"

/// One object the beam reaches, as Func_02002624 records it.
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

void Func_0200242c(void);

void Func_02002624(u16 *targets, u16 *stops, s32 a2, s32 a3) {
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
  Task_AddFrame(Func_0200242c, 0x480);
}
