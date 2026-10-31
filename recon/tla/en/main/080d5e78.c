// Near miss, differ 2 of 69 (tla-en, 2026-10-31): after the record is
// cleared, the flags store (strb r6, [r1, #26]) is scheduled after the zero's
// reload (mov r3, sl) instead of before it. Registers and dependences match;
// sched2 ranks the reload at priority 72 and the store at 71, which no
// statement order moves (the permuter, 4 minutes, found nothing better).
// ☀️'s twin is Entrance_EndArrivalAnimation (games/tbs/src/field/
// event_commands/exit_animations.c). The sprite's s32 scale word (the
// LayeredSprite layout of 08022010.c) lets its byte stores conflict with the
// actor's words, and the mask as a constant loads it after the first flags
// store: together they took the draft from differ 6 to 2.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system.h"

struct ContextSprite {
  u8 unk00[0xc];
  s32 scale;
  u8 unk10[0xa];
  u8 flags;
  u8 count;
  u8 unk1c[0xc];
  void *entries[4];
};

struct ContextEffectEntry {
  u8 unk00[5];
  u8 param;
};

void ObjectEffect_PrepareContextEffect(s32 effect);
void GameFlag_SetBit(s32 flag);
struct ContextEffectEntry *
ResourceMetadata_RegisterFar(struct ContextSprite *sprite, s32 id);
void ResourceMetadata_ClearRecordFar(void *record);

void ObjectEffect_EndContextEffect(s32 mode) {
  s32 zero;
  struct Actor *obj = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct ContextSprite *ctx = obj->animation;
  struct ContextEffectEntry *eff = ResourceMetadata_RegisterFar(ctx, 0x10b);

  zero = 0;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & 0xfff00000) + 0x80000;
  obj->z &= 0xfff00000;
  Actor_SetMode(obj, mode);
  wait(30);
  ctx->count = 1;
  ResourceMetadata_ClearRecordFar(ctx->entries[1]);
  ctx->entries[1] = (void *)zero;
  ctx->flags = 1;
  obj->acceleration = 0x10000;
  obj->speedLimit = 0x10000;
  Actor_SetTargetPosition(obj, obj->x, obj->y, obj->z + 0x80000);
  Actor_CommitPosition(obj);
}
