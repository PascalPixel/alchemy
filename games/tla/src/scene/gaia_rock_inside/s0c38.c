#include "types.h"
#include "sequence_parent_work.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02002cb4(s32 a);
void Func_02004f74(s32 a, s32 b);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_0200500c(s32 a, s32 b, s32 c);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Func_0200500c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200500c(a, b, c);
}

struct SequenceParentWork254 {
  u8 unk000[0x254];
  u16 value;
};

static __inline__ void Func_02004ff4_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004ff4(a, b, c, d, e, f, g);
}

void Func_02000c38(void) {
  Func_02004ff4_(16, 25, 0, 5, 5, 40, 8);
  if (GameFlag_Test_(0xa63) == 0) {
    GameFlag_SetBit_(0xa63);
    Func_02002cb4(2);
  }
}

void Func_02000c74(void) {
  Func_02004ff4_(18, 17, 0, 3, 3, 31, 0);
  if (GameFlag_Test_(0x9b7) == 0) {
    Func_0200500c_(249, 328, 344);
    ((struct SequenceParentWork254 *)&gSequenceParentWork)->value = 0x19b7;
    gSequenceParentWork.battleRule24b = 2;
    Func_02004f74(106, 2);
  }
}
