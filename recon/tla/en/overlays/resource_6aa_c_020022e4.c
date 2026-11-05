// Draft, differ 28: overlay 6aa's copy of the function drafted in
// resource_6ac_c_0200218c.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "actor.h"

struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

/// One pair of the beam's second list.
struct MarsLighthouseBeamStop {
  u16 unk00;
  u16 unk02;
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  struct MarsLighthouseBeamStop stops[4];
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Task_Add(void (*func)(void), s32 priority);
void Func_020020ec(void);

void Func_020022e4(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (GameFlag_Test(0x109) == 0) {
    i = 0;
    while ((id = *targets++) > 0) {
      s32 flags = *targets;
      s32 height = flags & 15;
      struct Actor *object;

      flags &= 0xf0;
      object = Actor_GetById(id);
      targets++;
      if (object != NULL) {
        Actor_SetSingleChildFlags(object, 0);
        ((u8 *)object)[98] = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = 1;
        target->flags = flags;
        target->object = Actor_GetById(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
      i++;
      if (i > 3)
        break;
    }
    if (stops != NULL) {
      s32 count;
      s32 value;

      i = 0;
      count = *stops++;
      beam->stops[i].unk00 = count;
      if (count > 0) {
        do {
          beam->stops[i].unk02 = *stops++;
          i++;
          if (i > 3)
            break;
          value = *stops++;
          beam->stops[i].unk00 = value;
        } while (value > 0);
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
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  ((s32 (*)())Task_Add)(Func_020020ec, 0x480);
}
