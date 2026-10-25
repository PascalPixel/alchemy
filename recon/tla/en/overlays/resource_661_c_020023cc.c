// Draft, differ 2 of 247: a second patrol in twenty states, which flag
// 0x200 either ends (starting a 201-frame count) or turns onto a shorter
// loop from state 32. Left: as in Func_02002208, the native loads
// gData_02004808's address before it zeroes the update hook.
#include "types.h"
#include "object_runtime.h"
#include "call.h"

extern s32 gData_02004808;

s32 GameFlag_Test(s32 flag);
void Func_02003fe0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 Func_020021f0(struct ObjectRuntime *object);
void Func_020021d4(void);

/// Sends the object toward a point and moves to the next state.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    Call4((void (*)())Func_02003fe0, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (Func_020021f0(object))                                                 \
      *state = next;                                                           \
    break;

void Func_020023cc(struct ObjectRuntime *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 166 << 18)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 182 << 18)
    ARRIVE(3, 4)
    WALK(4, 246 << 16, 182 << 18)
  case 5:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x200)) {
        object->update = NULL;
        gData_02004808 = 201;
      } else {
        *state = 6;
      }
    }
    break;
    WALK(6, 184 << 16, 182 << 18)
    ARRIVE(7, 8)
    WALK(8, 184 << 16, 178 << 18)
    ARRIVE(9, 10)
    WALK(10, 140 << 17, 178 << 18)
    ARRIVE(11, 12)
    WALK(12, 140 << 17, 190 << 18)
    ARRIVE(13, 14)
    WALK(14, 232 << 16, 190 << 18)
    ARRIVE(15, 16)
    WALK(16, 232 << 16, 186 << 18)
  case 17:
    if (Func_020021f0(object)) {
      if (!GameFlag_Test(0x200))
        *state = 18;
      else
        *state = 32;
    }
    break;
    WALK(18, 232 << 16, 166 << 18)
    ARRIVE(19, 0)
  case 32:
    Func_020021d4();
    Call4((void (*)())Func_02003fe0, (s32)object, 148 << 17, 0, 186 << 18);
    *state = 33;
    break;
    ARRIVE(33, 34)
    WALK(34, 148 << 17, 190 << 18)
    ARRIVE(35, 36)
    WALK(36, 232 << 16, 190 << 18)
    ARRIVE(37, 38)
    WALK(38, 232 << 16, 186 << 18)
    ARRIVE(39, 32)
  }
}
