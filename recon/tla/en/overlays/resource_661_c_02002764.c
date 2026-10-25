// Draft, differ 2 of 364: a third patrol in thirty-two states, turned onto
// its inner loops by flags 0x201 and 0x202, or ended (starting a 202-frame
// count). Left: as in Func_02002208, the native loads gData_02004808's
// address before it zeroes the update hook.
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

/// As WALK, after Func_020021d4.
#define WALK_ON(state_, x, z)                                                  \
  case state_:                                                                 \
    Func_020021d4();                                                           \
    Call4((void (*)())Func_02003fe0, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (Func_020021f0(object))                                                 \
      *state = next;                                                           \
    break;

/// Waits for the object to arrive, then turns off by a flag.
#define ARRIVE_OR(state_, flag, turn, next)                                    \
  case state_:                                                                 \
    if (Func_020021f0(object)) {                                               \
      if (GameFlag_Test(flag))                                                 \
        *state = turn;                                                         \
      else                                                                     \
        *state = next;                                                         \
    }                                                                          \
    break;

void Func_02002764(struct ObjectRuntime *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 132 << 17)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 172 << 17)
    ARRIVE(3, 4)
    WALK(4, 148 << 17, 172 << 17)
    ARRIVE_OR(5, 0x202, 32, 6)
    WALK(6, 168 << 16, 172 << 17)
    ARRIVE(7, 8)
    WALK(8, 168 << 16, 148 << 17)
    ARRIVE(9, 10)
    WALK(10, 184 << 16, 148 << 17)
    ARRIVE_OR(11, 0x201, 48, 12)
    WALK(12, 140 << 17, 148 << 17)
    ARRIVE(13, 14)
    WALK(14, 140 << 17, 164 << 17)
  case 15:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x202)) {
        object->update = NULL;
        gData_02004808 = 202;
      } else {
        *state = 16;
      }
    }
    break;
    WALK(16, 140 << 17, 180 << 17)
    ARRIVE(17, 18)
    WALK(18, 200 << 16, 180 << 17)
    ARRIVE(19, 20)
    WALK(20, 200 << 16, 156 << 17)
    ARRIVE_OR(21, 0x201, 34, 22)
    WALK(22, 200 << 16, 132 << 17)
    ARRIVE(23, 0)
    WALK_ON(32, 148 << 17, 156 << 17)
    ARRIVE(33, 34)
    WALK_ON(34, 164 << 17, 156 << 17)
    ARRIVE(35, 36)
    WALK(36, 164 << 17, 188 << 17)
    ARRIVE(37, 38)
    WALK(38, 148 << 17, 188 << 17)
    ARRIVE(39, 32)
    WALK_ON(48, 184 << 16, 172 << 17)
    ARRIVE(49, 50)
    WALK(50, 168 << 16, 172 << 17)
    ARRIVE(51, 52)
    WALK(52, 168 << 16, 148 << 17)
    ARRIVE(53, 54)
    WALK(54, 184 << 16, 148 << 17)
    ARRIVE(55, 48)
  }
}
