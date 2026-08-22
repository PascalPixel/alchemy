#include "b5_context.h"
#include "types.h"

void Func_08009140(void *);
void Func_08009150(void *, s32, s32, s32);
void Func_08009080(void *, s32);

struct Object080df8b8 {
    u8 padding_00[0x28];
    s32 field_28;
    u8 padding_2c[4];
    s32 field_30;
    s32 field_34;
    u8 padding_38[0xc];
    s32 field_44;
    s32 field_48;
    u8 padding_4c[0xc];
    u8 field_58;
    u8 padding_59;
    u8 field_5a;
};

void Func_080df8b8(s32 arg0) {
    struct B5Context *context = Func_080b5098(arg0);
    struct Object080df8b8 *object = context->object;

    object->field_34 = 0x20000;
    object->field_30 = 0x80000;
    object->field_28 = 0x40000;
    object->field_48 = 0xAB85;
    object->field_44 = 0;
    object->field_5a = 0;
    object->field_58 = 1;
    Func_08009140(object);
    Func_08009150(object, context->word_0c, 0, context->word_10);
    Func_08009080(object, 1);
}
