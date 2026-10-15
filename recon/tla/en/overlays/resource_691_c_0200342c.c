#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"

struct SpriteBits {
    u8 unk0 : 2;
    u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
    u8 unk00[9];
    struct SpriteBits bits;
};

struct ObjectRuntime *Object_GetById(s32 id);

void Func_0200342c(struct ObjectRuntime *object)
{
    s32 vx = object->bounce;
    s32 vz;
    struct PartyState *party;
    struct FieldSprite *sprite;
    struct ObjectRuntime *owner;
    struct SpriteAttributes *to;
    struct SpriteAttributes *from;
    struct SpriteBits source;
    struct SpriteBits bits;

    object->x += vx;
    object->y += object->gravity;
    vz = object->unk4c;
    object->z += vz;
    object->bounce = vx - vx / 18;
    object->unk4c = vz - vz / 16;
    object->scaleX += object->speedLimit;
    object->scaleY += object->acceleration;
    sprite = object->animation;
    sprite->rotation += object->action;

    party = &gPartyState;
    do {
    } while (0);
    owner = Object_GetById(party->currentOwner);
    to = (struct SpriteAttributes *)object->animation;
    from = (struct SpriteAttributes *)owner->animation;
    source = from->bits;
    do {
        bits = to->bits;
        bits.priority = source.priority;
        to->bits = bits;
    } while (0);
}
