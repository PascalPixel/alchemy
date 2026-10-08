#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "system.h"

struct ContextSprite {
    u8 unknown_00[0x1a];
    u8 flags;
    u8 count;
    u8 unknown_1c[0x10];
    void *entry;
};

struct ContextEffectEntry {
    u8 unknown_00[5];
    u8 param;
};

void ObjectEffect_PrepareContextEffect(s32 effect);
void GameFlag_SetBit(s32 flag);
struct ContextEffectEntry *ResourceMetadata_RegisterFar(struct ContextSprite *sprite, s32 id);
void ResourceMetadata_ClearRecordFar(void *record);

void ObjectEffect_EndContextEffect(s32 mode)
{
    s32 zero;
    s32 mask;
    struct ObjectRuntime *obj = ObjectTable_Get(gPartyState.current_owner);
    struct ContextSprite *ctx = obj->animation;
    struct ContextEffectEntry *eff = ResourceMetadata_RegisterFar(ctx, 0x10b);

    zero = 0;
    mask = 0xfff00000;
    ctx->flags = zero;
    eff->param = 15;
    obj->x = (obj->x & mask) + 0x80000;
    obj->z &= mask;
    Object_SetMode(obj, mode);
    WaitFrames(30);
    ctx->count = 1;
    ResourceMetadata_ClearRecordFar(ctx->entry);
    ctx->entry = (void *)zero;
    ctx->flags = 1;
    obj->acceleration = 0x10000;
    obj->speed_limit = 0x10000;
    Object_SetPosition(obj, obj->x, obj->y, obj->z + 0x80000);
    Object_CommitPosition(obj);
}
