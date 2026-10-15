#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct LohoPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const struct LohoPicture gData_02001f14;

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020012a0(const void *data, s32 a, s32 b);
void Func_020012a8(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020012e0(void);
void Func_020012e8(void);
void Func_02001348(s32 id, s32 x, s32 z);
void Func_020013d0(s32 value);
void Func_020013f8(s32 value);
void Func_02001448(s32 cue);

static __inline__ void Func_020012a0_(const void *data, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020012a0(data, a, b);
}

static __inline__ void Func_020012a8_(s32 srcX, s32 srcY, s32 width, s32 height,
                                      s32 destX, s32 destY) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020012a8(srcX, srcY, width, height, destX, destY);
}

static __inline__ void Func_02001348_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001348(id, x, z);
}

static __inline__ void Offset_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000e54(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Func_020012e0();
  Func_020013f8(0);
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the zero on every pass, and a for loop hoists it and
   * moves the work pointer (differ 4). */
  i = 8;
loop:
  {
    struct ObjectRuntime *object = Object_GetById(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  work = (u8 *)&((struct EventWork154 *)work)->value;
  Func_02001448(158);
  Func_020012a0_(gData_02001f14.data, gData_02001f14.a, gData_02001f14.b);
  Object_SetModeById(gPartyState.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  Offset_(gPartyState.currentOwner, 0, -4);
  Func_02001448(123);
  Battle_WaitMode0(6);
  Func_020013d0(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020012e8();
}
