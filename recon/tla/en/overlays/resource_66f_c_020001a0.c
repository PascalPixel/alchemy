// Draft of Func_020001a0 (overlay 66F, 194 instructions), differ 53: the
// push-block callback for blocks 14 and 15, written from the listing (the twin
// map's ☀️ scene is a hint only). Three differences remain:
// - block 14 sits in r6 and its cell x in r7; the native has them the other
// way;
// - after snapping actor 10's z, the native reloads both z values for the
// compare; here gcse keeps actor 10's z from the first load and the stores
// (r2);
// - the copy buffer's address is kept in r5 and IwramCopyWords in r6; the
// native rebuilds sp + 8 at each copy and keeps the routine in r5. Measured:
// the snap as a switch (worse); a separate cell variable for the snap took it
// from differ 71 to 53.
#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "flag_call.h"
#include "call.h"

extern const u8 gData_02001404[];

struct Actor *Actor_GetById(s32 id);
void FieldEvent_WaitFrames(s32 frames);
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
  struct Actor *block;
  struct Actor *other;
  s32 x;
  s32 z;
  s32 cell;

  if (id == 14) {
    block = Actor_GetById(14);
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
    block = Actor_GetById(15);
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
        Iwram_CopyWords(&saved, other, 128);
        Actor_ResetTargetsAndVelocity(9);
        Actor_Jump(9, 4, 0);
        FieldEvent_WaitFrames(30);
        Value3((s32 (*)())Func_0200111c, 9, 257, 0);
        Actor_SetAngleToward(9, 15, 0);
        FieldEvent_WaitFrames(60);
        Motion_SetModeAndWaitAnimation(9, 4);
        FieldEvent_WaitFrames(20);
        Iwram_CopyWords(other, &saved, 128);
        Call6((void (*)())Func_0200106c, 53, 35, 1, 1, 53, 36);
        Flag_Set(0x224);
      }
    }
  }
}
