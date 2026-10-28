// Draft of overlay 6ab's Func_02001df0, the twin of overlay 6aa's
// Func_020022e4: its draft (resource_6aa_c_020022e4.c) with this overlay's
// symbols: differ 141 of 145. Left: what that draft's header records.
#include "types.h"
#include "io_reg.h"
#include "object_runtime.h"

/// One object the beam reaches, as Func_02001df0 records it.
struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct ObjectRuntime *object;
  s32 height; ///< 16.16 pixels
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  u16 stopCount;
  u16 stops[7];
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Task_Add(void (*callback)(void), s32 priority);
void Func_02001bf8(void);

void Func_02001df0(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (GameFlag_Test(0x109) == 0) {
    id = *targets++;
    i = 0;
    if (id > 0) {
    nextTarget:
      {
        s32 flags = *targets;
        s32 height = flags & 15;
        struct ObjectRuntime *object;

        flags &= 0xf0;
        object = Object_GetById(id);
        targets++;
        if (object != NULL) {
          ObjectDispatch_SetSingleChildFlags(object, 0);
          ((u8 *)object)[98] = 1;
          if ((flags & 0x80) == 0)
            object->collisionFlags = 0;
          target->active = 1;
          target->flags = flags;
          target->object = Object_GetById(id);
          target->height = height << 16;
          target->index = i;
          target++;
        }
      }
      i++;
      if (i <= 3) {
        id = *targets++;
        if (id > 0)
          goto nextTarget;
      }
    }
    if (stops != NULL) {
      u16 *stop;

      i = 0;
      beam->stopCount = *stops++;
      if ((s16)beam->stopCount > 0) {
        stop = &beam->stopCount;
      nextStop:
        stop[1] = *stops;
        i++;
        stops++;
        stop += 2;
        if (i <= 3) {
          *stop = *stops++;
          if ((s16)*stop > 0)
            goto nextStop;
        }
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    REG_BLDALPHA = 0x608;
    REG_BLDCNT = 0x3f10;
  }
  Task_Add(Func_02001bf8, 0x480);
}
