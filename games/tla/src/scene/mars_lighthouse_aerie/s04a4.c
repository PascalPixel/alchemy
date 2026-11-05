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

void Func_020004a4(void) {
  struct Actor *anchor = Actor_GetById(8);
  s32 offset = (Random16Far() * 48 >> 16) << 16;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;

  if (anchor != NULL && (phase = Data_0200eb60 & 15) == 0) {
    /* FAKEMATCH(wrapper): the spawn forwarded through Value4; as a direct call,
     * 35 of 103 instructions differ in tla-en. */
    object = (struct Actor *)Value4((s32 (*)())Func_02005574, 0x2a0,
                                    anchor->x + 0x80000,
                                    anchor->y + offset + 0x80000, anchor->z);
    offset = (offset / 0x60000) << 16;
    if (object != NULL) {
      u8 zero;

      sprite = object->animation;
      Func_0200556c(object, gData_02006ad0);
      Actor_SetPartAttribute(object, 10);
      object->motionFlags = phase;
      {
        /* FAKEMATCH(temporary): the masked value held in a local; as
         * object->action = Random16Far() & 0x0ffff000, 45 of 103 instructions
         * differ in tla-en. */
        s32 action = Random16Far() & 0x0ffff000;

        object->action = action;
      }
      object->unk66 = phase;
      object->update = Func_020005a0;
      zero = 0;
      object->watchedObject = anchor;
      object->speedLimit = Math_Sine((offset & 0xfffff) >> 4) * 24 >> 16;
      ((u8 *)sprite)[26] = zero;
      sprite->layer =
          ((struct MarsLighthouseAerieSprite *)anchor->animation)->layer;
    }
  }
  Data_0200eb60++;
}
