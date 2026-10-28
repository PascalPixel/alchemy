#include "kibombo.h"

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
      Actor_SetSingleChildFlags(object, 0);
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
