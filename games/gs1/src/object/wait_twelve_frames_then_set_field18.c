#include "types.h"

#define Object_WaitTwelveFramesThenSetField18 Func_080eceac

s32 WaitFrames(s32);
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

void Object_WaitTwelveFramesThenSetField18(struct Object080eceac *obj)
{
  Func_080b5078(obj->field_08, obj->field_24, 0x18, 0x73333);
 do { WaitFrames(0xC); } while (0);
  obj->field_18 = 3;
  Func_080d4604(obj, 2);
}
