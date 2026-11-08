// Matches: differ 0 (tla-en, 2026-11-08)
// One tagged device, FAKEMATCH(dead): the sum's local is also assigned the
// burst's flags inside the effect call's arguments. That makes it a global
// pseudo, which avoids r0 to r3 and takes the native r4. As a block-local pseudo,
// local-alloc ties it to the shifted value (r3, 6 differ). Through value it took
// value's r0 (4 differ). The flags assignment works only inside the call's
// arguments, where its life is short: as a statement before the call, 136
// differ. Also from this round: the zero variable was not needed (CSE carries the bounce's 0 into the branch, same
// code), and a direct call to OverlayObject_PrepareObject matches too.
// Tried without gain on 8 November: the sum in angle (4, r5), i (68), height
// (11), spawned (18) or the zero (72); s16, u16, s8 or u8 sum (6, 13); a u8
// prototype for the collision call (extends, 10 to 29); * 0x100000 (27).
// Older notes, before value took the sum:
// From 16: the burst loop is a goto loop (no loop notes, so GCSE no longer
// hoists the array's address), and one array serves as the velocity and
// then the position, giving the native frame of 68 with options at sp+16 and
// the array at sp+56. Left: kind + 2 is formed after the blend store, in r3,
// where the native forms it before, in r4; a named sum, kind += 2 (before or
// after the blend), the z in a local and * 0x100000 all widen the gap; the
// sum first, s32 or u32 kind and the blend stores reordered leave it at 6.
#include "types.h"
#include "call.h"
#include "actor.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
struct EffectOptions {
  s32 palette;
  s32 kind;
  s32 startScaleX;
  s32 startScaleY;
  u8 unk10[0x14];
  void (*update)(void);
};
void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags, struct EffectOptions *options);
s32 Func_020003b4();
s32 Func_020003f0();
s32 Func_0200053c();
s32 Func_0200108c();
s32 Func_020010a4();
s32 Func_020010cc();
s32 Func_0200115c();
s32 Func_02001164();
s32 GameFlag_SetBit();
s32 Map_GetTerrainHeight();
s32 Math_Cosine();
s32 Math_Sine();
s32 OverlayObject_PrepareObject();
s32 OverlayObject_WaitUntilIdle();
s32 Random16Far();
s32 wait();
void Func_020007ec(void);
void Func_02000848(void);

void Func_0200087c(void) {
  s32 velocity[3];
  struct EffectOptions options;
  s32 work;
  s32 current;
  struct Actor *object;
  s32 *other;
  s32 kind;
  s32 height;
  s32 value;
  s32 angle;
  u32 i;
  s32 spawned;
  u16 *blendControl;
  s32 sum;

  work = FIELD_AT(0x3000000, s32, 108);
  current = *(s32 *)0x30000e0;
  object = FIELD_AT(work, struct Actor *, 3260);
  if (current != 0 && FIELD_AT(work, s16, 358) != 0) {
    if (FIELD_AT(current, s16, 28) != 147)
      Call1((void (*)())Func_0200053c, -1);
  } else {
    Call1((void (*)())Func_0200053c, -1);
  }
  if (Value3((s32 (*)())Map_GetTerrainHeight, 0, object->x, object->z) !=
      object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    object->bounce = 0;
    Call1((void (*)())wait, 1);
    kind = Value3((s32 (*)())Func_020010a4, 2, object->x, object->z);
    if (kind != 0) {
      object->priorityFlags |= 2;
      *(u16 *)0x4000052 = 0x1000;
      blendControl = (u16 *)0x4000050;
      *blendControl = 0x3f10;
      sum = kind + 2;
      spawned = Value4((s32 (*)())OverlayObject_PrepareObject, object->x, 0,
                       object->z + (sum << 20), 312);
      Call1((void (*)())Func_02001164, 204);
      Call1((void (*)())OverlayObject_WaitUntilIdle, (s32)object);
      Call1((void (*)())OverlayObject_WaitUntilIdle, (s32)object);
      object->x = object->y = object->z = 0;
      Call1((void (*)())Func_0200108c, spawned);
      Call1((void (*)())wait, 1);
      *blendControl = 0;
    } else {
      Call1((void (*)())OverlayObject_WaitUntilIdle, (s32)object);
      Call1((void (*)())OverlayObject_WaitUntilIdle, (s32)object);
      Call1((void (*)())Func_02001164, 188);
      options.kind = 7;
      options.palette = 2;
      options.update = Func_020007ec;
      options.startScaleX = 0xcccc;
      options.startScaleY = 0xcccc;
      i = 0;
      // A goto loop carries no loop notes: as a do loop, GCSE moves the
      // array's address out of the loop.
    loop:
      {
        angle = i << 12;
        velocity[0] = Value1((s32 (*)())Math_Cosine, angle) * 2;
        velocity[1] = 0;
        value = Value1((s32 (*)())Math_Sine, angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[2] = value;
        velocity[0] +=
            ((u32)(Value0((s32 (*)())Random16Far) * 3 << 14) >> 16) - 0x6000;
        velocity[2] +=
            ((u32)(Value0((s32 (*)())Random16Far) * 3 << 13) >> 16) - 0x3000;
        /* FAKEMATCH(dead): the form kept; as 0x10b0000 passed directly, 6 of
         * 255 instructions differ in tla-en. */
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
      velocity[1] = object->y - 0x100000;
      velocity[2] = object->z;
      other = (s32 *)Value2((s32 (*)())Func_0200115c, (s32)velocity, 1);
      if (other != 0) {
      object->update = Func_02000848;
      FIELD_AT(object, u8, 98) = 1;
      FIELD_AT(other, u8, 98) = 1;
      Call1((void (*)())GameFlag_SetBit, 0x212);
      }
    }
  }
  height =
      (Value3((s32 (*)())Map_GetTerrainHeight, 0, object->x, object->z) >> 19) +
      2;
  Call4((void (*)())Func_020010cc, object->x, object->z, 0, height);
  Call4((void (*)())Func_020010cc, object->x, object->z, 2, height);
  value = Value3((s32 (*)())Func_020003b4, 0, object->x >> 20, object->z >> 20);
  object->unk4c = value;
  Call4((void (*)())Func_020003f0, 0, object->x >> 20, object->z >> 20,
        value | 128);
}
