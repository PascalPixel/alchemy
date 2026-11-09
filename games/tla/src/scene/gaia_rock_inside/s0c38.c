#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02002cb4(s32 a);
void Func_02004f74(s32 a, s32 b);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_0200500c(s32 a, s32 b, s32 c);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Func_0200500c_(s32 a, s32 b, s32 c) {
  Func_0200500c(a, b, c);
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

static __inline__ void Func_02004ff4_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02004ff4(a, b, c, d, e, f, g);
}

void Func_02000c38(void) {
  Func_02004ff4_(16, 25, 0, 5, 5, 40, 8);
  if (GameFlag_Test_(0xa63) == 0) {
    Event_SetFlag(0xa63);
    Func_02002cb4(2);
  }
}

void Func_02000c74(void) {
  Func_02004ff4_(18, 17, 0, 3, 3, 31, 0);
  if (Event_GetFlag(0x9b7) == 0) {
    Func_0200500c_(249, 328, 344);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b7;
    gMainRoutineWork.battleRule24b = 2;
    Func_02004f74(106, 2);
  }
}
