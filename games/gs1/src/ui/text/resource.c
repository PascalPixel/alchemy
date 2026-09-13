#include "types.h"

void UiTextResource_NoOpCallback(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback0(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback1(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback2(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback3(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallback4(void)
{
}

struct TextResourceSetup {
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

s32 Func_08004080(void);
s32 Resource_CopyData(s32, s32, const void *);
extern const u8 Data_080342f8[];

void TextResource_Initialize(struct TextResourceSetup *object, s32 *slot)
{
    const void *data = Data_080342f8;
    s32 value = Func_08004080();

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

struct TextResourcePosition {
    u8 padding[4];
    s8 kind;
    u8 padding2;
    u16 index : 9;
    u16 rest : 7;
};
void Runtime_PushSlotEntry(s32 *arg0, s32 arg1);
void TextResource_SetPosition(struct TextResourcePosition *obj, s32 arg1, s32 arg2)
{
    obj->index = arg1;
    obj->kind = arg2;
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

s32 Resource_ResetEntry(s32);

/* 受け取った値を呼出し先へ渡す。 */
void TextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
