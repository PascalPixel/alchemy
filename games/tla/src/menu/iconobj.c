#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

struct MenuIconWork {
    u8 unknown_000[0x220];
    void *objects[4];
    s16 x[4];
    s16 y[4];
    s32 scale[4];
    s16 frame[4];
};

void ResourceObject_ReleaseFar(void *object);
void MenuIcon_UpdateTransforms(void);

void MenuIcon_ReleaseObjects(void)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (menu->objects[i] != NULL) {
            ResourceObject_ReleaseFar(menu->objects[i]);
            menu->objects[i] = NULL;
        }
    }
    Scheduler_RemoveCallback(MenuIcon_UpdateTransforms);
}

extern s32 Data_08105a40[];
void ResourceObject_ReleaseFar(void *object);
void *ResourceObject_CreateFar(s32 resource);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);

void MenuIcon_ReplaceObject(s32 index, s32 kind, s32 animation)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;
    s8 *object;

    if (menu->objects[index] != NULL) {
        ResourceObject_ReleaseFar(menu->objects[index]);
        menu->objects[index] = NULL;
    }
    object = ResourceObject_CreateFar(Data_08105a40[kind]);
    if (object != NULL) {
        Animation_ApplyChildArgumentFar(object, animation);
        object[9] &= -13;
    }
    menu->objects[index] = object;
}

void MenuIcon_SetPosition(s32 index, s32 x, s32 y)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;

    /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
    __asm__ volatile("" : "+l"(menu));

    if (menu->objects[index] != NULL) {
        menu->x[index] = x;
        menu->y[index] = y;
    }
}

void MenuIcon_SetFrame(s32 index, s32 frame)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;

    /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
    __asm__ volatile("" : "+l"(menu));

    if (menu->objects[index] != NULL)
        menu->frame[index] = frame;
}
