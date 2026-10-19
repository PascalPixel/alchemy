// Draft, differ 16 of 73: two things are left. The native loads the halfword
// zero for the motion flags right after the random offset is added, before it
// takes the counter's address; and its literal pool is dumped one instruction
// earlier (after str r7, [r6, #48]), so its length estimate is 2 bytes longer
// than this form's. An empty asm after the sum, as here, moves the pool from
// two instructions late to one; a one-pass block there fixes neither (differ
// 12, pool two late). The sum goes through an int variable: stored directly it
// is narrowed to a halfword add of 0x8000 (differ 13).
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

s32 Math_Cosine(s32 angle);
void Func_02005a2c(struct KibomboObject *object, const void *script);
void Effect_AdvanceMotion(void);
extern u32 gData_02005ca4[];

void Func_02002f34(void) {
  s32 phase = gFrameCount & 3;

  if (phase == 0) {
    struct KibomboObject *object =
        Func_02005a34(0x16e, 0x1080000, 0x200000, 0x13c0000);
    s32 value;

    if (object != 0) {
      object->field44 = Math_Cosine(((Random16Far() << 13) >> 16) + 0x3000);
      object->field48 = 0x10000;
      object->field4c = 0x40000 - (u16)Random16Far();
      value = (u16)Random16Far() + 0xffff8000;
      /* FAKEMATCH: an empty asm after the sum moves the literal pool one
       * instruction toward the native's place (still one late). */
      asm volatile("");
      object->field64 = value;
      object->motionFlags = 0;
      ObjectDispatch_SetSingleChildFlags(object, 0);
      Func_02005a24(object, 2);
      Func_02005a2c(object, gData_02005ca4);
      object->sprite->priority = 1;
      object->field64 = 0;
      *(s32 *)&object->unk2c[4] = 0;
      *(s32 *)&object->unk2c[8] = 0;
      object->update = Effect_AdvanceMotion;
    }
  }
}
