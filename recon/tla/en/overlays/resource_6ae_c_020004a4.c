// Draft, differ 70 of 103: rewritten from the listing. Every 16th call, while
// object 8 exists, spawns an effect of type 0x2a0 above it at a random height
// that follows it on its sprite's layer. An empty asm with a memory clobber
// before the closing increment stops GCSE sharing the counter's address with
// its first read (87 without). Left: the native tests and reads object 8
// through a copy in r0 where this uses r2 and r4, and the callee-saved
// registers and the pool order differ from there.
#include "types.h"
#include "object_runtime.h"

struct MarsLighthouseAerieSprite {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 layer : 2;
  u8 unk09_4 : 4;
  u8 unk0a[0x10];
  u8 unk1a;
};

extern u32 Data_0200eb60;
extern const u8 gData_02006ad0[];

struct ObjectRuntime *Object_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
struct ObjectRuntime *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct ObjectRuntime *object, const u8 *script);
void Func_020005a0(void);

void Func_020004a4(void) {
  struct ObjectRuntime *anchor = Object_GetById(8);
  s32 offset = (Random16Far() * 48 >> 16) << 16;
  struct ObjectRuntime *object;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;

  if (anchor != NULL && (phase = Data_0200eb60 & 15) == 0) {
    object = Func_02005574(0x2a0, anchor->x + 0x80000,
                           anchor->y + offset + 0x80000, anchor->z);
    offset = (offset / 0x60000) << 16;
    if (object != NULL) {
      sprite = object->animation;
      Func_0200556c(object, gData_02006ad0);
      Object_SetPartAttribute(object, 10);
      object->motionFlags = phase;
      object->action = Random16Far() & 0x0ffff000;
      object->unk66 = phase;
      object->update = Func_020005a0;
      object->linkedObject = anchor;
      object->speedLimit = Math_Sine((offset & 0xfffff) >> 4) * 24 >> 16;
      sprite->unk1a = 0;
      sprite->layer =
          ((struct MarsLighthouseAerieSprite *)anchor->animation)->layer;
    }
  }
  __asm__("" : : : "memory");
  Data_0200eb60++;
}
