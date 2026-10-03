#include "CALLBACK_SCHEDULER.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "MENU_LIST.H"

void UiTextResource_NoOpCallback(void)
{
}

/* Empty callback slots retained by the text-resource dispatcher. */
void UiTextResource_NoOpCallbackBank(void)
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

extern const u8 Menu_CursorObjectTiles[];

void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot)
{
    const void *data = Menu_CursorObjectTiles;
    s32 value = Resource_FindFreeEntry();

    /* ビットフィールドは生成時の設定順を保持する。 */
    *slot = value;
    object->oam.f.tile = VramBlock_LoadCached(value, 0x80, data);
    object->oam.f.mode = 0;
    object->oam.f.mosaic = 0;
    object->oam.f.colors = 1;
    object->oam.f.affine = 0;
    object->oam.f.affine_index = 0;
    object->oam.f.size = 0;
    object->oam.f.shape = 2;
    object->oam.f.priority = 0;
}

void UiTextResource_SetPosition(struct MenuSprite *obj, s32 x, s32 y)
{
    obj->oam.f.x = x;
    obj->oam.f.y = y;
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}


/* 受け取った値を呼出し先へ渡す。 */
void UiTextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
