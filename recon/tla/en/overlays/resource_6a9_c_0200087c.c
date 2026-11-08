// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. Every call is
// direct, with the caverns' prototypes; the options are field_effect.h's
// EffectOptions (priority 2, palette 7).
// One tagged device, FAKEMATCH(dead): the sum's local is also assigned the
// burst's flags inside the effect call's arguments. That makes it a global
// pseudo, which avoids r0 to r3 and takes the native r4. As a block-local
// pseudo, local-alloc ties it to the shifted value (r3, 6 differ). Through
// value it took value's r0 (4 differ). The flags assignment works only inside
// the call's arguments, where its life is short: as a statement before the
// call, 136 differ. Also from this round: the zero variable was not needed (CSE
// carries the bounce's 0 into the branch, same code), and a direct call to
// OverlayObject_PrepareObject matches too. Tried without gain on 8 November:
// the sum in angle (4, r5), i (68), height (11), spawned (18) or the zero (72);
// s16, u16, s8 or u8 sum (6, 13); a u8 prototype for the collision call
// (extends, 10 to 29); * 0x100000 (27). Older notes, before value took the sum:
// From 16: the burst loop is a goto loop (no loop notes, so GCSE no longer
// hoists the array's address), and one array serves as the velocity and
// then the position, giving the native frame of 68 with options at sp+16 and
// the array at sp+56. Left: kind + 2 is formed after the blend store, in r3,
// where the native forms it before, in r4; a named sum, kind += 2 (before or
// after the blend), the z in a local and * 0x100000 all widen the gap; the
// sum first, s32 or u32 kind and the blend stores reordered leave it at 6.
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
