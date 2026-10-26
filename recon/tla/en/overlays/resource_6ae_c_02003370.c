// Draft, differ 60 of 207: rewritten from the listing. A scheduled step of the
// aerie's closing: by its frame counter it sets the blend and scale (frames 0,
// 8, 16), parks object 8 below the floor (24), then raises it, spawning effects
// of type 0x2a0 behind it (25, 26) until it stands high enough, and removes
// itself after Data_02006b68 frames. Func_020055c4 goes through a forwarding
// inline, and empty asms with memory clobbers keep GCSE from carrying object
// 8's y into the raising branches. Left: register choices in the raising
// branches (the step 0x24000 in r1, the sprite in r1), some loads one
// instruction off, and the literal pool's order.
#include "types.h"
#include "object_runtime.h"
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

struct ObjectRuntime *Object_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void SceneActor_ParkRecord(struct ObjectRuntime *object);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
struct ObjectRuntime *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct ObjectRuntime *object, const u8 *script);
void Func_020055c4(s32 a, s32 b, s32 c);
void Func_0200575c(s32 a, s32 b);
void Func_0200576c(s32 a);
void Func_020057dc(s32 sound);
void Func_020000e4(void);

static __inline__ void SetScale(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h). */
  Func_020055c4(a, b, c);
}

void Func_02003370(void) {
  struct ObjectRuntime *anchor = Object_GetById(8);
  s32 spawn = 0;
  struct ObjectRuntime *object;
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
    ObjectMotion_EnableActionAndSetCallback(8, gData_02005b54);
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
      __asm__("" : : : "memory");
      anchor->y += 0x24000;
    }
    break;
  case 26:
    Data_02006ccc--;
    if (anchor->y > 0x140000) {
      Data_02006ccc = 999;
      Scheduler_RemoveCallbackFar(Func_02003370);
    } else {
      if ((gFrameCount & 7) == 0)
        Func_020057dc(246);
      __asm__("" : : : "memory");
      anchor->y += 0x24000;
      spawn = 1;
    }
    break;
  }
  if (spawn) {
    object = Func_02005574(
        0x2a0, anchor->x,
        anchor->y - ((Random16Far() * 80 >> 16) << 16) - 0x80000, anchor->z);
    if (object != NULL) {
      sprite = object->animation;
      Func_0200556c(object, gData_02006180);
      Object_SetPartAttribute(object, 1);
      object->motionFlags = 0;
      object->action = Random16Far() & 0x0ffff000;
      object->unk66 = 0;
      ((u8 *)object)[98] = Random16Far() >> 13;
      object->update = Func_020000e4;
      object->speedLimit = Math_Sine(Random16Far() * 0xffff >> 20) * 24;
      object->speedLimit = anchor->speedLimit >> 16;
      sprite->unk1a = 0;
      sprite->layer = 1;
    }
  }
  if (++Data_02006ccc > Data_02006b68)
    Scheduler_RemoveCallbackFar(Func_02003370);
}
