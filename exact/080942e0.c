#include "types.h"

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

extern u32 Data_02000240[];

struct Entity_080942e0 *Func_0808ba1c(u32);
struct Effect_080942e0 *Func_08009048(struct Entity_080942e0 *, s32);
void Func_08009080(struct Entity_080942e0 *, s32);
void Func_080030f8(s32);

void Func_080942e0(s32 value)
{
    u32 zero;
    u8 kind;
    struct Entity_080942e0 *object;
    struct Entity_080942e0 *context;
    struct Effect_080942e0 *effect;

    object = Func_0808ba1c(Data_02000240[125]);
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
    Func_08009080(object, value);
    Func_080030f8(18);
}
