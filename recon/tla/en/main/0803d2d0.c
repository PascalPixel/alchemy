/* differ 4: the native loads the window work before building the resource-id offset; only ARM9TDMI load scheduling differs. Measured: an empty asm barrier after the window work swaps the registers of the work and of 999 (differ 12); a one-pass block around the load gives the registers but loads 999 before the offset is started and moves the literal pool ahead of the return (differ 3). */
#include "types.h"
#include "graphics.h"
#include "window.h"
#include "ram_buffer.h"

extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 CreateSideObject(s32 id, s32 kind, s32 slot, struct UiWindow *window, s32 x, s32 y);

void UiWork_SetTwoEntriesTo999(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    s32 index = 0;

    do {
        work->resourceIds[index] = 999;
        index++;
    } while (index != 2);
}
