#include "types.h"

union EffectMotionSlot {
    u32 word;
    struct {
        u8 padding0[2];
        u8 active;
        u8 padding3;
    } bytes;
};

struct EffectMotionObject {
    u8 padding0[5];
    u8 kind;
    u8 padding6[2];
    u32 x;
    u8 paddingC[4];
    u32 y;
    u8 padding14[16];
    union EffectMotionSlot slot24;
    u8 padding28[4];
    u32 field2C;
    u8 padding30[8];
    u32 field38;
    u8 padding3C[4];
    u32 field40;
    u8 padding44[12];
    struct EffectMotionObject *context;
};

struct EffectKindObject {
    u8 padding0[5];
    u8 kind;
};

extern u32 Data_02000240[];

struct EffectMotionObject *Func_0808ba1c(u32);
/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
#define ObjectTable_Get Func_0808ba1c
struct EffectKindObject *Func_08009048(struct EffectMotionObject *, s32);
void Object_SetMode(struct EffectMotionObject *, s32);
void WaitFrames(s32);

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct EffectMotionObject *object;
    struct EffectMotionObject *context;
    struct EffectKindObject *effect;

    object = ObjectTable_Get(Data_02000240[125]);
    context = object->context;
    effect = Func_08009048(context, 27);
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
