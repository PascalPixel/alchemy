/* Near miss, differ 2 of 47 (ported from The Broken Seal's
   ObjectEffect_PrepareContextEffect, with one tagged one-pass block): the
   native loads the party state's address before it copies the argument to
   r8 (mov r8, r0); this draft copies first. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ContextAnimation {
    u8 unk00[0x1a];
    u8 flags;
};

struct ContextEffect {
    u8 unk00[5];
    u8 param;
};

struct ContextEffect *ResourceMetadata_RegisterFar(void *context, s32 id);
void WaitFrames(s32 frames);

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct ObjectRuntime *object;
    struct ContextAnimation *context;
    struct ContextEffect *effect;

    object = ObjectTable_Get(gPartyState.currentOwner);
    context = object->animation;
    effect = ResourceMetadata_RegisterFar(context, 267);
    zero = 0;
    context->flags = zero;
    /* FAKEMATCH: the one-pass block stores the cleared flags before the kind is loaded (differ 6 without it). */
    do {
    } while (0);
    kind = 15;
    effect->param = kind;
    object->x = (object->x & 0xFFF00000) + 0x80000;
    object->z = (object->z & 0xFFF00000) + 0x100000;
    object->velocityX = zero;
    object->velocityZ = zero;
    object->targetX = 0x80000000;
    object->targetZ = 0x80000000;
    Object_SetMode(object, value);
    WaitFrames(18);
}
