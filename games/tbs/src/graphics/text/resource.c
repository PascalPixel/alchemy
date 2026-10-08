#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "types.h"
#include "resource.h"
#include "menu_list.h"

void UiTextResource_NoOpCallback(void)
{
}

void UiTextResource_NoOpCallbackBank(void)
{
}

void UiTextResource_NoOpCallback1(void)
{
}

void UiTextResource_NoOpCallback2(void)
{
}

void UiTextResource_NoOpCallback3(void)
{
}

void UiTextResource_NoOpCallback4(void)
{
}


void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot)
{
    const void *data = Menu_CursorObjectTiles;
    s32 value = Resource_FindFreeEntry();

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

void UiTextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
