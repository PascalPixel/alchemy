#include "types.h"
#include "ram_buffer.h"

struct MenuEntryWork {
    u8 unknown_000[0xf8];
    void *objects[8];
    s16 x[8];
    s16 y[8];
    s32 scale[8];
    u8 unknown_158[0x509 - 0x158];
    s8 active;
};

void Func_08020018(void *object, s32 *transform, s32 *scale, s32 limit, s32 depth);

void Menu_UpdateEntryObjectTransforms(void)
{
    struct MenuEntryWork *menu = Ram_HeapSlots->menu_runtime;
    s32 scale[2];
    s32 transform[4];
    s32 i;

    if (menu->active != 0) {
        for (i = 0; i <= 7; i++) {
            s32 top = 0x728000 - (menu->y[i] << 15);

            if (menu->objects[i] != NULL) {
                scale[0] = menu->scale[i];
                scale[1] = menu->scale[i];
                transform[0] = menu->x[i] << 16;
                transform[1] = top;
                transform[2] = (menu->y[i] << 16) + top;
                transform[3] = 0;
                Func_08020018(menu->objects[i], transform, scale, 0x4000, 250);
            }
        }
    }
}
