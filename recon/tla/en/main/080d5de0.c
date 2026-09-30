#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

union EffectMotionSlot {
    u32 word;
    struct {
        u8 unknown0[2];
        u8 active;
        u8 unknown3;
    } bytes;
};

struct EffectMotionObject {
    u8 unknown0[5];
    u8 kind;
    u8 unknown6[2];
    u32 x;
    u8 unknownC[4];
    u32 y;
    u8 unknown14[16];
    union EffectMotionSlot slot24;
    u8 unknown28[4];
    u32 field2C;
    u8 unknown30[8];
    u32 field38;
    u8 unknown3C[4];
    u32 field40;
    u8 unknown44[12];
    struct EffectMotionObject *context;
};

struct EffectKindObject {
    u8 unknown0[5];
    u8 kind;
};

extern u32 gGameState[];

/* Object table: 192 pointers at gEventWork + 0x14 (see ObjectTable_Get). */
void *ResourceMetadata_RegisterFar(void *, s32);
void Object_SetMode(void *, s32);

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct EffectMotionObject *object;
    struct EffectMotionObject *context;
    struct EffectKindObject *effect;

    object = ObjectTable_Get(gGameState[125]);
    context = object->context;
    effect = ResourceMetadata_RegisterFar(context, 27);
    zero = 0;
    kind = 15;

    context->slot24.bytes.active = zero;
    effect->kind = kind;
    object->x = (object->x & 0xFFF00000) + 0x80000;
    object->y = (object->y & 0xFFF00000) + 0x100000;
    object->slot24.word = zero;
    object->field2C = zero;
    object->field38 = 0x80000000;
    object->field40 = 0x80000000;
    Object_SetMode(object, value);
    WaitFrames(18);
}
