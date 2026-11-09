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
  {
    /* FAKEMATCH(pin): the sprite copied into r1 for its two stores, leaving
     * r3 to the zero's copy from sl; as ctx in both stores, 8 of 69
     * instructions differ in tla-en. */
    register struct ContextSprite *pinnedCtx asm("r1") = ctx;

    pinnedCtx->flags = 1;
    pinnedCtx->entries[1] = (void *)zero;
  }
  obj->acceleration = 0x10000;
  obj->speedLimit = 0x10000;
  Actor_SetTargetPosition(obj, obj->x, obj->y, obj->z + 0x80000);
  Actor_CommitPosition(obj);
}
