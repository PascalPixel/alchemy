#include "types.h"
#include "call.h"
#include "actor.h"
#include "frame.h"

struct MarsLighthouseAerieSprite {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 layer : 2;
  u8 unk09_4 : 4;
  u8 unk0a[0x10];
  u8 unk1a;
};

extern u32 Data_02006ccc;
extern u32 Data_02006b68;
extern const u8 gData_02005b54[];
extern const u8 gData_02006180[];

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetMovementTypeById(s32 id, const void *script);
void SceneActor_ParkRecord(struct Actor *object);
void Task_RemoveFar(void (*func)(void));
struct Actor *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct Actor *object, const u8 *script);
void Func_020055c4(s32 a, s32 b, s32 c);
void Func_0200575c(s32 a, s32 b);
void Func_0200576c(s32 a);
void Func_020057dc(s32 sound);
void Func_020000e4(void);

static __inline__ void SetScale(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 41 of 207
   * instructions differ in tla-en. */
  Func_020055c4(a, b, c);
}

void Func_02003370(void) {
  struct Actor *anchor = Actor_GetById(8);
  s32 spawn = 0;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;

  switch (Data_02006ccc) {
  case 0:
    Func_020057dc(220);
    SetScale(0x60000, 0x60000, 0x10000);
    Func_0200575c(0x2063ff, 1);
    Func_0200576c(8);
    break;
  case 8:
    Func_0200575c(0x10000, 1);
    Func_0200576c(8);
    break;
  case 16:
    SetScale(0x10000, 0x10000, 0x10000);
    break;
  case 24:
    anchor->x = 0x2000000;
    anchor->y = -0x1680000;
    anchor->z = 0xcc0000;
    anchor->scaleX = 0x10000;
    anchor->scaleY = 0x10000;
    SceneActor_ParkRecord(anchor);
    Actor_SetMovementTypeById(8, gData_02005b54);
    break;
  case 25:
    Data_02006ccc--;
    if (anchor->y > 0) {
      Func_0200575c(0x203210, 0);
      Func_0200576c(16);
      Data_02006ccc++;
    } else {
      if ((gFrameCount & 7) == 0)
        Func_020057dc(246);
      spawn = 1;
      anchor->y += 0x24000;
    }
    break;
  case 26:
    Data_02006ccc--;
    if (anchor->y > 0x140000) {
      Data_02006ccc = 999;
      Task_RemoveFar(Func_02003370);
    } else {
      if ((gFrameCount & 7) == 0)
        Func_020057dc(246);
      anchor->y += 0x24000;
      spawn = 1;
    }
    break;
  }
  if (spawn) {
    /* FAKEMATCH(wrapper): the spawn forwarded through Value4; as a direct call,
     * 2 of 207 instructions differ in tla-en. */
    object = (struct Actor *)Value4(
        (s32 (*)())Func_02005574, 0x2a0, anchor->x,
        anchor->y - ((Random16Far() * 80 >> 16) << 16) - 0x80000, anchor->z);
    if (object != NULL) {
      u8 zero;
      s32 mask;

      sprite = object->animation;
      Func_0200556c(object, gData_02006180);
      Actor_SetPartAttribute(object, 1);
      object->motionFlags = 0;
      {
        /* FAKEMATCH(temporary): the masked value held in a local; as
         * object->action = Random16Far() & 0x0ffff000, 36 of 207 instructions
         * differ in tla-en. */
        s32 action = Random16Far() & 0x0ffff000;

        object->action = action;
      }
      zero = 0;
      object->unk66 = 0;
      {
        /* FAKEMATCH(temporary): the byte held in a local; as a direct store, 2
         * of 207 instructions differ in tla-en. */
        u8 phase = Random16Far() >> 13;

        ((u8 *)object)[98] = phase;
      }
      object->update = Func_020000e4;
      object->speedLimit = Math_Sine(Random16Far() * 0xffff >> 20) * 24;
      object->speedLimit = anchor->speedLimit >> 16;
      sprite->unk1a = zero;
      mask = -13;
      ((u8 *)sprite)[9] = (((u8 *)sprite)[9] & mask) | 4;
    }
  }
  if (++Data_02006ccc > Data_02006b68)
    Task_RemoveFar(Func_02003370);
}
