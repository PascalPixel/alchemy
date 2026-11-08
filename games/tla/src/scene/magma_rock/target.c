#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "magma_rock.h"
#include "macros.h"

void Actor_SetPartAttribute(struct Actor *object, s32 value);
void FieldEvent_Wait(s32 frames);
void Func_02000ff4(void);
void Func_020011e4(struct Actor *object);
void Func_02003820(s32 x, s32 y, s32 z, s32 kind);
void Func_020038dc(void);
void Func_02003998(void);
void Func_02003a54(struct Actor *object, s32 a);
void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003bcc(s32 value);

void Func_02001444(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Func_02003aec();
  Func_02003bcc(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  Actor_SetPartAttribute(object, 7);
  FieldEvent_Wait(3);
  Actor_SetPartAttribute(work->target, 0);
  FieldEvent_Wait(1);
  Func_020011e4(work->target);
  Func_02003af4();
}

void Func_0200148c(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Func_02003aec();
  Func_02003bcc(0);
  object = work->target;
  object->action = 0x400;
  object->update = Func_02000ff4;
  Func_02003af4();
}

void Func_020014bc(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    Func_020038dc();
  if (mode == 2) {
    struct Actor *object = work->target;

    Func_02003820(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    Func_02003820(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    Func_02003998();
  if (cue == 178)
    Func_02003a54(work->target, 3);
}
