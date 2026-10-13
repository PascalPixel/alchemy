/* Near miss, differ 2 of 47 (one reordered pair): the native loads the 0xfff00000 mask before
 * the object's x (ldr r2, =mask; ldr r3, [r5, #8]); this loads x first. Measured: without the
 * barrier after the sprite's store, differ 4 (the effect's 15 is also loaded before that
 * store); the 15 in a u8 local, before or after the barrier, the same 2; a second barrier
 * after the effect's store, the same 2; the mask in a local, alone or ahead of a one-pass
 * block, differ 22 (the zero and the mask trade r1 and r2). The earlier form of this draft,
 * with its own structs and gGameState[125] for the owner, measured differ 12. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"

u8 *ResourceMetadata_RegisterFar(void *sprite, s32 resource);
void WaitFrames(s32 frames);

void ObjectEffect_PrepareContextEffect(s32 mode)
{
    struct PartyState *party = &gPartyState;
    struct ObjectRuntime *object;
    struct FieldSprite *sprite;
    u8 *effect;
    u32 zero;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    object = ObjectTable_Get(party->current_owner);
    sprite = object->animation;
    effect = ResourceMetadata_RegisterFar(sprite, 267);
    zero = 0;

    sprite->next_frame = zero;
    /* FAKEMATCH: the barrier keeps the sprite's store ahead of the effect's 15, as in its sibling 080d5d70 (differ 2 more without it). */
    __asm__("");
    effect[5] = 15;
    object->x = (object->x & 0xfff00000) + 0x80000;
    object->z = (object->z & 0xfff00000) + 0x100000;
    object->velocity_x = zero;
    object->velocity_z = zero;
    object->target_x = 0x80000000;
    object->target_z = 0x80000000;
    Object_SetMode(object, mode);
    WaitFrames(18);
}
