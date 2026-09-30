#include "TYPES.H"
#include "RESOURCE.H"

void UiTextResource_Initialize(struct TextResourceSetup *object, s32 *slot)
{
    const void *data = Menu_CursorObjectTiles;
    s32 value = Resource_FindFreeEntry();

    /* ビットフィールドは生成時の設定順を保持する。 */
    *slot = value;
    object->field_80 = VramBlock_LoadCached(value, 0x80, data);
    object->field_52 = 0;
    object->field_54 = 0;
    object->field_55 = 1;
    object->field_50 = 0;
    object->field_71 = 0;
    object->field_76 = 0;
    object->field_56 = 2;
    object->field_8a = 0;
}
