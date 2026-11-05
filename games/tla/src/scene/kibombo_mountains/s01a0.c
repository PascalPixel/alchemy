#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "flag_call.h"
#include "call.h"

extern const u8 gData_02001404[];

struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_ResetTargetsAndVelocity(s32 actor);
void Actor_Jump(s32 actor, s32 height, s32 frames);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Func_0200015c(s32 id);
void Func_02001044(struct Actor *object, const u8 *script);
void Func_0200104c(struct Actor *object);
void Func_02001054(struct Actor *object, s32 x, s32 y, s32 z);
void Func_0200105c(struct Actor *object);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_0200111c(s32 actor, s32 a, s32 b);
void Func_0200119c(s32 value);

void Func_020001a0(s32 unused, s32 id) {
  struct Actor saved;

  if (id == 14) {
    struct Actor *block = Actor_GetById(14);
    struct Actor *other;
    s32 x;
    s32 z;
    s32 cell;


    if (!Flag_IsSet(0x223) && (x = block->x >> 20) == 47 &&
        (z = block->z >> 20) == 28) {
      Func_0200015c(14);
      Call6((void (*)())Func_0200106c, 49, 28, 1, 1, x, z);
      Flag_Set(0x223);
      Actor_SetSpritePriority(14, 2);
      other = Actor_GetById(10);
      cell = other->z >> 20;
      if (cell == 27) {
        Func_0200119c(257);
        other->z = 220 << 17;
      } else if (cell == 28 || cell == 29) {
        Func_0200119c(258);
        other->z = 236 << 17;
      }
      if (block->z < other->z)
        Flag_Set(0x201);
      else
        Flag_Set(0x203);
    } else {
      Actor_SetSpritePriority(id, 1);
    }
  }
  if (id == 15) {
    struct Actor *block = Actor_GetById(15);
    struct Actor *other;


    if (!Flag_IsSet(0x224) && block->x >> 20 == 53 && block->z >> 20 == 36) {
      s32 dx;

      Func_0200015c(15);
      Actor_SetSpritePriority(15, 2);
      other = Actor_GetById(9);
      dx = block->x - other->x;
      if (dx >= 0 ? dx < 0x80000 : other->x - block->x < 0x80000) {
        other->motionFlags = 0;
        other->speedLimit = 0x10000;
        Func_02001054(other, block->x, block->y, block->z);
        Func_0200105c(other);
        Func_02001044(block, gData_02001404);
        Func_0200104c(other);
        other->x = 0;
        other->z = 0;
        Flag_Set(0x200);
      } else {
        Call3((void (*)())Iwram_CopyWords, (s32)&saved, (s32)other, 128);
        Actor_ResetTargetsAndVelocity(9);
        Actor_Jump(9, 4, 0);
        FieldEvent_Wait(30);
        Value3((s32 (*)())Func_0200111c, 9, 257, 0);
        Actor_SetAngleToward(9, 15, 0);
        FieldEvent_Wait(60);
        Motion_SetModeAndWaitAnimation(9, 4);
        FieldEvent_Wait(20);
        { void *from = &saved; Iwram_CopyWords(other, from, 128); }
        Call6((void (*)())Func_0200106c, 53, 35, 1, 1, 53, 36);
        Flag_Set(0x224);
      }
    }
  }
}
