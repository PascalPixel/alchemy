#include "types.h"
#include "scene.h"

/* battle/effects/wait_then_set_field.c */
void WaitFrames(s32);

void BattleFx_WaitThenSetField18To4(void *arg0)
{
  unsigned int ofs;
  Battle_SetMode(*((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
  WaitFrames(0x1D);
  ofs = 0x18;
  *((s32 *)(((u8 *)arg0) + ofs)) = 4;
  Battle_Apply(arg0, 2);
}

/* object/wait_twelve_frames_then_set_field18.c */
s32 Obj_SetMode(s32, s16, s32, s32);
s32 Obj_Apply(void *, s32);

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
  Obj_SetMode(obj->field_08, obj->field_24, 0x18, 0x73333);
 do { WaitFrames(0xC); } while (0);
  obj->field_18 = 3;
  Obj_Apply(obj, 2);
}
