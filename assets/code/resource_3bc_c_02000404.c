#include "types.h"

typedef struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x1c];
    s32 field30;
    s32 field34;
    u8 filler38[0x1d];
    u8 field55;
} Object;

extern s16 Data_02000240[];

void Func_02004e8a();
Object *Func_02004e38();
void Func_02004d50();
Object *Func_02004e5e();
void Func_02004d6e();
void Func_02004ec4();
void Func_02004db8();
void Func_02004cbe();
void Func_02004e44();

void Func_02000404(void)
{
    s16 *table;
    Object *object;

    table = Data_02000240;

    Func_02004e8a(*(s32 *)&table[250], 1);

    object = Func_02004e38(11);
    object->field55 = 0;
    object->field34 = 0x6666;
    object->field30 = 0xcccc;
    Func_02004d50(object, object->x, 0x40000, object->z);

    object = Func_02004e5e(10);
    object->field55 = 0;
    object->field34 = 0x6666;
    object->field30 = 0xcccc;
    Func_02004d6e(object, object->x, 0x200000, object->z);

    Func_02004ec4(10);
    {
        s32 stack_first = 9;
        s32 stack_second = 12;
        Func_02004db8(0, 25, 1, 1, stack_first, stack_second);
    }
    Func_02004cbe(2);
    Func_02004e44(0x367);
}
