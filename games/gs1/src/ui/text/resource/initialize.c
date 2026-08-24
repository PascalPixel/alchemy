#include "types.h"

struct Object_0801c0dc {
    u8 filler0[5];
    u8 field_50 : 2;
    u8 field_52 : 2;
    u8 field_54 : 1;
    u8 field_55 : 1;
    u8 field_56 : 2;
    u8 filler6;
    u8 field_70 : 1;
    u8 field_71 : 5;
    u8 field_76 : 2;
    u16 field_80 : 10;
    u16 field_8a : 2;
    u16 field_8c : 4;
};

s32 Resource_FindFreeSlot(void);
s32 Resource_CopyData(s32, s32, const void *);
extern const u8 Data_080342f8[];

void TextResource_Initialize(struct Object_0801c0dc *object, s32 *slot)
{
    const void *data = Data_080342f8;
    s32 value = Resource_FindFreeSlot();

    /* ビットフィールドは生成時の設定順を保持する。 */
    *slot = value;
    object->field_80 = Resource_CopyData(value, 0x80, data);
    object->field_52 = 0;
    object->field_54 = 0;
    object->field_55 = 1;
    object->field_50 = 0;
    object->field_71 = 0;
    object->field_76 = 0;
    object->field_56 = 2;
    object->field_8a = 0;
}
