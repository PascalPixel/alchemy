#include "types.h"
#include "scene.h"

/* ui/text/resource/no_op_callback.c */
void UiTextResource_NoOpCallback(void)
{
}

/* ui/text/resource/no_op_callback_bank.c */
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

/* ui/text/resource/initialize.c */
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
extern const u8 gRom[];

void TextResource_Initialize(struct Object_0801c0dc *object, s32 *slot)
{
    const void *data = gRom;
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

/* ui/text/resource/set_position.c */
struct Obj {
    u8 padding[4];
    s8 kind;
    u8 padding2;
    u16 index : 9;
    u16 rest : 7;
};
void Runtime_PushSlotEntry(s32 *arg0, s32 arg1);
void TextResource_SetPosition(struct Obj *obj, s32 arg1, s32 arg2)
{
    obj->index = arg1;
    obj->kind = arg2;
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

/* ui/text/resource/release.c */
s32 Resource_ResetEntry(s32);

/* 受け取った値を呼出し先へ渡す。 */
void TextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
