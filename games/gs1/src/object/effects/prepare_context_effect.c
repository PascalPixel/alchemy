#include "types.h"
#include "scene.h"
#include "abi/object/effects/prepare_context_effect.h"

union Slot36_080942e0 {
    u32 word;
    struct {
        u8 padding0[2];
        u8 active;
        u8 padding3;
    } bytes;
};

struct Entity_080942e0 {
    u8 padding0[5];
    u8 kind;
    u8 padding6[2];
    u32 x;
    u8 paddingC[4];
    u32 y;
    u8 padding14[16];
    union Slot36_080942e0 slot24;
    u8 padding28[4];
    u32 field2C;
    u8 padding30[8];
    u32 field38;
    u8 padding3C[4];
    u32 field40;
    u8 padding44[12];
    struct Entity_080942e0 *context;
};

struct Effect_080942e0 {
    u8 padding0[5];
    u8 kind;
};

extern u32 gCell[];

struct Entity_080942e0 *ObjectTable_Get(u32);
/* Object table: 192 pointers at gWork + 0x14 (object/table/get.c). */
struct Effect_080942e0 *Obj_Run(struct Entity_080942e0 *, s32);
void Object_SetMode(struct Entity_080942e0 *, s32);
void WaitFrames(s32);

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct Entity_080942e0 *object;
    struct Entity_080942e0 *context;
    struct Effect_080942e0 *effect;

    object = ObjectTable_Get(gCell[125]);
    context = object->context;
    effect = Obj_Run(context, 27);
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
