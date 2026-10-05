/* Draft: ☀️'s ObjectEffect_EndContextEffect (OBJECT10.C) for ⚓️: the
   controlled owner from gPartyState, sprite entry 0x10b, the sprite's flag
   and count bytes at 0x1a and 0x1b. Differ 8 with entry 0 stored before
   the flag is raised (14 the other way round). What remains: the listing builds the
   first sprite base copy in r2 (mov r2, r8) where this build uses r3, loads
   the x coordinate before building 0x80000 (movs r1, #128), and after the
   record is cleared reloads the sprite base into r1 and zero into r3; the
   draft uses r3 and r1. A literal 0 instead of the zero local changes
   nothing. Goes after ObjectEffect_BeginContextEffect25 in
   BEGIN_CONTEXT_EFFECT_26.C. */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "SYSTEM.H"

/* The controlled character's sprite, as the context effects set it. */
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

/* Settles the controlled character back onto its tile in the given mode
   and drops the context effect's sprite entry. */
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
