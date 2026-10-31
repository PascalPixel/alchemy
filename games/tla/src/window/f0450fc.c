#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "system/affine.h"

struct Effect {
  unsigned x : 16;
  unsigned y : 16;
  unsigned z : 16;
  unsigned unk : 16;
};

struct Object {
  u8 filler0[6];
  u16 src6;
  u8 src8;
  u8 filler9[6];
  u8 out15;
  u8 filler16[4];
  u8 out20;
  u8 mode21 : 2;
  u8 rest21 : 6;
  u16 pos22 : 9;
  u16 affine22 : 5;
  u16 rest22 : 2;
};

extern u32 gVBlankCount;
extern s32 gWindowSprite_PulseScales[];

void Func_080450fc(struct Object *obj) {
  s32 v;
  struct Effect effect;

  v = gWindowSprite_PulseScales[(gVBlankCount >> 1) & 7] / 256;

  if (obj != 0) {
    effect.x = v;
    effect.y = v;
    effect.z = 0;
    obj->affine22 =
        AffineMatrix_BuildForEffect((struct AffineTransform *)&effect);
    obj->mode21 = 3;
    obj->pos22 = obj->src6 + 0xfff0;
    obj->out20 = obj->src8 + 0xf0;
    obj->out15 = 0xfc;
  }
}
