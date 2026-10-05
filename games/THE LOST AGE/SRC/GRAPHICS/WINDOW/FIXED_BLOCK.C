#include "GLOBAL.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "RAM_BUFFER.H"
#include "WINDOW.H"


void VramBlock_LoadResourceFar(s32 entry_no, s32 size, s32 resource_id);
s32 UiText_MeasureEntryDimensions(s32 start, u32 *width, u32 *height, s32 mode);
void UiText_DrawString(u8 *text, s32 output, s32 x, s32 y);

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
    return slot;
}

void Resource_CopyFixedBlockB(s32 slot)
{
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
}

void Resource_CopyFixedBlockA(s32 slot)
{
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiFixedBlockATiles);
}

#if EDITION_INTERNATIONAL
s32 UiText_SetRenderString(const u8 *str)
{
    struct UiRenderWork *work;
    u16 *dst;
    s32 count;
    u32 width;
    u32 height;

    work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    count = 0;
    if (*str != 0) {
        dst = work->entries;
        do {
            *dst = *str;
            str++;
            /* FAKEMATCH: the ARM9TDMI load schedule moves the count step after the next character's load and the entry step; native counts right after the store. */
            __asm__ volatile("");
            count++;
            /* FAKEMATCH: this boundary keeps the next character's load ahead of the entry step, as native. */
            __asm__ volatile("");
            dst++;
        } while (*str != 0);
    }
    work->entries[count] = 0;
    UiText_MeasureEntryDimensions(0, &width, &height, 0);
    return width;
}

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
    u8 text[20];
    s32 length = 0;

    if (*input != 0) {
        do {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6) {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    UiText_DrawString(text, output, 0, -2);
}
#endif
