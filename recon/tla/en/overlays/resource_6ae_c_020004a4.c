// Draft, differ 47 (tla-en, 2026-11-05). Func_020004a4 of overlay 6ae.
#include "types.h"
#include "call.h"
#include "actor.h"

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

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
struct Actor *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct Actor *object, const u8 *script);
void Func_020005a0(void);

#define Func_02005574(a0, a1, a2, a3) Value4((s32 (*)())Func_02005574, (s32)(a0), (s32)(a1), (s32)(a2), (s32)(a3))

void Func_020004a4(void) {
  struct Actor *anchor = Actor_GetById(8);
  s32 offset = (Random16Far() * 48 >> 16) << 16;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;

  if (anchor != NULL && (phase = Data_0200eb60 & 15) == 0) {
    object = Func_02005574(0x2a0, anchor->x + 0x80000,
                           anchor->y + offset + 0x80000, anchor->z);
    offset = (offset / 0x60000) << 16;
    if (object != NULL) {
      u8 zero;

      sprite = object->animation;
      Func_0200556c(object, gData_02006ad0);
      Actor_SetPartAttribute(object, 10);
      object->motionFlags = phase;
      object->action = Random16Far() & 0x0ffff000;
      object->unk66 = phase;
      object->update = Func_020005a0;
      zero = 0;
      object->watchedObject = anchor;
      object->speedLimit = Math_Sine((offset & 0xfffff) >> 4) * 24 >> 16;
      sprite->unk1a = zero;
      sprite->layer =
          ((struct MarsLighthouseAerieSprite *)anchor->animation)->layer;
    }
  }
  Data_0200eb60++;
}
