// Matches: differ 0 (tla-en, 2026-11-01)
// Overlay 6a5's per-frame update for a moving object, close kin of Airs Rock's
// Func_02000550: it idles while four event words are set, otherwise it plays a
// sound every 32 frames, copies the leader's sprite priority, and moves along
// its angle. Off the walkable cells or outside -32 to 272 pixels on screen it
// shuts itself down. Within 12 of the leader and 12 pixels of its height, with
// flag 0x104 clear, the leader starts watching it.
// What the match rests on: the sprite byte is s8, because a char store conflicts
// with the x and z loads after it, which puts the 2 first; a bitfield or u8 field
// view does not. The named priority sets orrs' operands, as Airs Rock's matched
// copies (s1f7c, s2cf0, s3654) write it. The flags pointer gives the store alias
// set 0, which puts it ahead of the work address; the member store does not. Both
// byte stores of 1 are member stores, so reload shares the 1. renderMode's zero
// is its field store's pooled zero. No device of any kind remains.
// Names are placeholders where the code alone suggests them (H3).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

/// The object this update drives: an Actor with its own bytes over the callback
/// word.
struct Object6a5 {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  u8 unk34[0x1c];
  s8 *sprite;
  u8 unk54[0x0e];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(void);
};

/// The scene's event work, as far as this update reads it.
struct EventWork6a5 {
  u8 unk000[346];
  s16 unk15a;
  u8 unk15c[2];
  s16 unk15e;
  u8 unk160[6];
  s16 unk166;
  s16 unk168;
  s16 unk16a;
};

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

s32 Func_020042e0(s32 *a, s32 *b);
void Func_02005764(struct Object6a5 *object, const void *script);
void Func_02005994(struct Vector3 *position);
s32 Func_020059e4();
void Func_020059f4(s32 sound);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Actor_SetAnimationSpeed(struct Object6a5 *object, s32 value);
struct Actor *Actor_GetById(s32 id);
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));
extern u8 gData_020069fc[];
extern s32 Data_02006a28;

void Func_0200430c(struct Object6a5 *object) {
  struct EventWork6a5 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Vector3 position;
  s32 distance;
  s32 near;
  s32 timer;
  u8 *flags;

  if (work->unk166 != 0 || work->unk15a != 0 || work->unk15e != 0 ||
      work->unk168 != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  timer = object->unk64 + 1;
  object->unk64 = timer;
  if ((timer & 31) == 31)
    Func_020059f4(231);
  {
    s8 *from = leader->animation;
    s8 *to = object->sprite;
    s32 priority = from[9] & 12;
    to[9] = (to[9] & ~12) | priority;
  }
  if (Func_020059e4(object->x, object->z, 2) != 255) {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Func_02005994(&position);
    if (position.x <= 0x1100000) {
      if (position.x >= -0x200000) {
      if (object->unk62 == 0) {
        distance = leader->y - object->y;
        if (distance < 0)
          distance = object->y - leader->y;
        near = 0;
        if (Func_020042e0(&leader->x, &object->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near != 0 && GameFlag_Test(0x104) == 0) {
          leader->watchedObject = (struct Actor *)object;
          leader->angle = object->angle;
          flags = &leader->priorityFlags;
          *flags &= ~1;
          work->unk16a = 200;
          gMainRoutineWork.renderMode = 0;
          object->unk62 = 1;
          object->unk63 = 1;
        }
      }
      object->x += IwramMulQ16(Math_Cosine(object->angle), object->speed);
      object->z += IwramMulQ16(Math_Sine(object->angle), object->speed);
      return;
      }
    }
  }
  object->unk63 = 0;
  object->update = 0;
  Func_02005764(object, gData_020069fc);
  Func_020059f4(228);
  Data_02006a28 = 0;
}
