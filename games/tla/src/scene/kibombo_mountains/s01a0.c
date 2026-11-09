#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "flag_call.h"
#include "call.h"
#include "event_inline.h"

extern const u8 gData_02001404[];

void Actor_ResetTargetsAndVelocity(s32 actor);
void Func_0200015c(s32 id);
void Func_02001044(struct Actor *object, const u8 *script);
void Func_0200104c(struct Actor *object);

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
      MapGrid_CopyCellAttributes(49, 28, 1, 1, x, z);
      Flag_Set(0x223);
      Actor_SetSpritePriority(14, 2);
      other = Actor_GetById(10);
      cell = other->z >> 20;
      if (cell == 27) {
        Sound_Play(257);
        other->z = 220 << 17;
      } else if (cell == 28 || cell == 29) {
        Sound_Play(258);
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
        FieldActor_SetMoveTarget(other, block->x, block->y, block->z);
        Actor_WaitForMove(other);
        Func_02001044(block, gData_02001404);
        Func_0200104c(other);
        other->x = 0;
        other->z = 0;
        Flag_Set(0x200);
      } else {
        Call3((void (*)())Iwram_CopyWords, (s32)&saved, (s32)other, 128);
        Actor_ResetTargetsAndVelocity(9);
        FieldActor_Jump(9, 4, 0);
        FieldEvent_Wait(30);
        Actor_ShowEmote(9, 257, 0);
        Actor_FaceActor(9, 15, 0);
        FieldEvent_Wait(60);
        Actor_SetAnimationAndWait(9, 4);
        FieldEvent_Wait(20);
        { void *from = &saved; Iwram_CopyWords(other, from, 128); }
        MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
        Flag_Set(0x224);
      }
    }
  }
}
