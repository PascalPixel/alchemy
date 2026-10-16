// Near miss: differ 24 of 109 in English, the same in Japanese. The 0x921 block
// reads x through r2, orders two argument moves differently, and the last pair
// of calls stores its first stack argument early.
#include "types.h"
#include "object_runtime.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000e44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

#ifdef TLA_EDITION_JA
void Func_02000100(s32 id);
#define JUMP_A(id) Func_02000100(id)
#define JUMP_B(id) Func_02000100(id)
#else
void Func_02000100(s32 id, s32 cue);
#define JUMP_A(id) Call2((void (*)())Func_02000100, id, 181)
#define JUMP_B(id) Func_02000100(id, 181)
#endif

void Func_02000164(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  Call6((void (*)())Func_02000e44, 8, 90, 1, 1, 8, 91);
  if (id == 16 && Value1((s32 (*)())GameFlag_Test, 0x920) == 0 &&
      (object->x >> 19) <= 11) {
    JUMP_A(16);
    Call1((void (*)())GameFlag_SetBit, 0x920);
    Call6((void (*)())Func_02000e44, 6, 27, 1, 1, 5, 27);
  }
  if (id == 19 && GameFlag_Test(0x921) == 0) {
    if ((object->x >> 19) == 77)
      Call6((void (*)())Func_02000e44, 33, 102, 1, 2, 37, 102);
    if ((object->x >> 19) == 79 && (object->y >> 19) == 12) {
      JUMP_B(19);
      Call6((void (*)())Func_02000e44, 38, 46, 1, 1, 39, 46);
    }
    if ((object->x >> 19) == 81) {
      GameFlag_SetBit(0x921);
      Func_02000e44(41, 46, 1, 1, 39, 46);
      Func_02000e44(27, 44, 1, 1, 40, 46);
    }
  }
}
