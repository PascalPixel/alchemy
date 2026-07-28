#include "types.h"

s32 Func_080030f8(s32);
s32 Func_080b5078(s32, s16, s32, s32);
s32 Func_080d4604(void *, s32);

struct Object080eceac {
  u8 padding_00[8];
  s32 field_08;
  u8 padding_0c[0xc];
  s32 field_18;
  u8 padding_1c[8];
  s16 field_24;
};

void Func_080eceac(struct Object080eceac *object)
{
  Func_080b5078(object->field_08, object->field_24, 0x18, 0x73333);
 do { Func_080030f8(0xC); } while (0);
  object->field_18 = 3;
  Func_080d4604(object, 2);
}
