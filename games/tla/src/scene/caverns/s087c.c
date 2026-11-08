#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_effect.h"
#include "fixed_math.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct FieldActor;

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
s32 Func_020003b4(u32 layer, s32 x, s32 y);
s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value);
void Func_0200053c(s32 offset);
void Func_0200108c(struct Actor *object);
s32 Func_020010a4(s32 layer, s32 x, s32 z);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
struct Actor *Func_0200115c(s32 *position, s32 mode);
void Func_02001164(s32 cue);
void GameFlag_SetBit(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void OverlayObject_WaitUntilIdle(struct Actor *object);
u32 Random16Far(void);
void wait(s32 frames);
void Func_020007ec(struct Actor *object);
void Func_02000848(struct FieldActor *object);

void Func_0200087c(void) {
  s32 velocity[3];
  struct EffectOptions options;
  u8 *work;
  u8 *current;
  struct Actor *object;
  struct Actor *other;
  s32 kind;
  s32 height;
  s32 value;
  s32 angle;
  u32 i;
  struct Actor *spawned;
  u16 *blendControl;
  s32 sum;

  work = Ram_HeapSlots->eventWork;
  current = Ram_HeapSlots->effectWork;
  object = FIELD_AT(work, struct Actor *, 3260);
  if (current != NULL && FIELD_AT(work, s16, 358) != 0) {
    if (FIELD_AT(current, s16, 28) != 147)
      Func_0200053c(-1);
  } else {
    Func_0200053c(-1);
  }
  if (Map_GetTerrainHeight(0, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    object->bounce = 0;
    wait(1);
    kind = Func_020010a4(2, object->x, object->z);
    if (kind != 0) {
      object->priorityFlags |= 2;
      *(u16 *)0x4000052 = 0x1000;
      blendControl = (u16 *)0x4000050;
      *blendControl = 0x3f10;
      sum = kind + 2;
      spawned = OverlayObject_PrepareObject(object->x, 0,
                                            object->z + (sum << 20), 312);
      Func_02001164(204);
      OverlayObject_WaitUntilIdle(object);
      OverlayObject_WaitUntilIdle(object);
      object->x = object->y = object->z = 0;
      Func_0200108c(spawned);
      wait(1);
      *blendControl = 0;
    } else {
      OverlayObject_WaitUntilIdle(object);
      OverlayObject_WaitUntilIdle(object);
      Func_02001164(188);
      options.palette = 7;
      options.priority = 2;
      options.update = (void (*)(union FieldObject *))Func_020007ec;
      options.startScaleX = 0xcccc;
      options.startScaleY = 0xcccc;
      i = 0;
      // A goto loop carries no loop notes: as a do loop, GCSE moves the
      // array's address out of the loop.
    loop:
      {
        angle = i << 12;
        velocity[0] = Math_Cosine(angle) * 2;
        velocity[1] = 0;
        velocity[2] = Math_Sine(angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[0] += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
        velocity[2] += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
        /* FAKEMATCH(dead): sum, never read again, assigned the flags inside
         * the call's arguments; as 0x10b0000 passed directly, 6 of 255
         * instructions differ in tla-en. */
        Func_0200015c(object->x, object->y, object->z, velocity[0], velocity[1],
                      velocity[2], sum = 0x10b0000, &options);
        i++;
      }
      if (i <= 16)
        goto loop;
    }
    object->motionFlags = 0;
    object->terrainId = 0;
    // The velocity array is reused for the position under the object.
    {
      velocity[0] = object->x;
      velocity[1] = object->y - (16 << 16);
      velocity[2] = object->z;
      other = Func_0200115c(velocity, 1);
      if (other != NULL) {
        object->update = (void (*)(void))Func_02000848;
        FIELD_AT(object, u8, 98) = 1;
        FIELD_AT(other, u8, 98) = 1;
        GameFlag_SetBit(0x212);
      }
    }
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  Func_020010cc(object->x, object->z, 0, height);
  Func_020010cc(object->x, object->z, 2, height);
  value = Func_020003b4(0, object->x >> 20, object->z >> 20);
  object->unk4c = value;
  Func_020003f0(0, object->x >> 20, object->z >> 20, value | 128);
}
