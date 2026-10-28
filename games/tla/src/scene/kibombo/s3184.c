#include "kibombo.h"
#include "io_reg.h"

extern struct KibomboObject *gData_02007950;

struct KibomboEffectWork {
  u8 unk000[0x7c4];
  s16 field7c4;
};

void Func_02000624(void);
void Func_02000668(void);
void Func_020006ac(void);
void Func_020006f0(void);
void Func_0200057c(void);
void Func_02002c34(void);
void Func_02002ccc(void);
void Func_02002d64(void);
void Func_02003504(void);
void Func_020031f0(s32 id);
void Func_02003220(void);
void Func_02003898(void);
void Func_020038ec(void);

struct KibomboObject *Func_02005a34(s32 type, s32 x, s32 y, s32 z);
void Func_02005a24(struct KibomboObject *object, s32 mode);
void Func_02005a3c(struct KibomboObject *object);
void Actor_SetSingleChildFlags(struct KibomboObject *object, u32 value);
void Animation_SetStateFlags(struct KibomboObject *object, s32 flags);
void Actor_SetModeById(s32 actor, s32 mode);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005bb4(void);
void Func_02005bc4(s32 countdown);
void Func_02005c14(s32 a, s32 b);
s32 Func_02005c1c(s32 a, s32 b);
void Func_02005c24(void);
void Func_02005c2c(void);
void Func_02005c34(void);

static inline void Kibombo_Spawn(s32 x, s32 y, s32 z) {
  Func_02003104(x, y, z);
}

void Func_02003184(void) {
  s32 x = 0x1b80000;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Kibombo_Spawn(x, 0x280000, 0x1c20000);
  Kibombo_Spawn(x, 0x2e0000, 0x1b60000);
  Kibombo_Spawn(x, 0x340000, 0x1aa0000);
  Kibombo_Spawn(x, 0x3a0000, 0x19e0000);
  /* FAKEMATCH(barrier): an empty asm before the last spawn, which shifts x's
   * shift after the second register store as the native has it; without it,
   * 2 of 43 instructions differ in tla-en. */
  asm("");
  Func_02003104(x, 0x400000, 0x1920000);
}
