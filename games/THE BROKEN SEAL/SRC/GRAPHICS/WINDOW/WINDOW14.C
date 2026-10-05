#include "EDITION.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "RUNTIME_INTERFACES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "FIELD_SPRITE.H"
#include "WORKSPACE_OPTIONS.H"
#include "BATTLE_RUNTIME.H"

extern u8 Resource_FixedBlockBTiles[];

/* ui/window/copy_tilemap_region.c */
extern u8 RomBytes_080310a4[];

/* resource/copy_fixed_block_a.c */
extern const u8 RomBytes_080317e4[];

/* ui/text/draw/draw_padded_label.c */
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, 0x80, Resource_FixedBlockBTiles);
    return slot;
}

#if EDITION_INTERNATIONAL
/* A routine that only reports success, after the fixed resource block
   loader; nothing in the image calls it by name. The Japanese edition checks
   the name entry's voiced marks here instead. */
s32 Resource_ReturnTrue(void)
{
    return 1;
}
#else
/* The letters that take the voiced and the semi-voiced mark, as pairs of
   first and last codes ending at zero. */
extern const u8 NameEntry_DakutenRanges[];
extern const u8 NameEntry_HandakutenRanges[];

/* Whether the name entry may put a mark after a letter: a voiced mark (0xde)
   or semi-voiced mark (0xdf) only follows the letters in its ranges; any
   other code follows anything. */
s32 NameEntry_AcceptsMark(s32 mark, s32 letter)
{
    const u8 *range;

    if ((u32)(mark - 0xde) > 1)
        return 1;
    if (mark == 0xde)
        range = NameEntry_DakutenRanges;
    else
        range = NameEntry_HandakutenRanges;
    for (; *range != 0; range += 2) {
        if (letter >= range[0] && letter <= range[1])
            return 1;
    }
    return 0;
}

/* The letters of a name, not counting its voiced and semi-voiced marks. */
s32 NameEntry_CountLetters(const u8 *name)
{
    s32 count;

    count = 0;
    for (; *name != 0; name++) {
        if ((u8)(*name - 0xde) > 1)
            count++;
    }
    return count;
}
#endif

/* ui/window/window_copy_tilemap_region.c */
void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s16 *mirror = (s16 *)work->tilemap;
    s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
    s16 *input = buffer;
    u32 cell;
    s16 *vram;
    s32 row;

    Resource_DecodeType01(source, buffer);
    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;

    for (row = 0; row < window->height; row++) {
        s32 column;

        for (column = 0; column < window->width; column++) {
            s16 value = *input++;

            *vram++ = value;
            *mirror++ = value;
        }
        vram += 32 - window->width;
        mirror += 32 - window->width;
    }
    Runtime_BumpFree(buffer);
}

/* ui/window/set_tile_attribute_rect.c */
void UiWindow_SetTileAttributeRect(const struct RenderInput *window,
    s32 x, s32 y, s32 width, s32 height, u32 field) {
    u8 *base = gWindowWork[0];

    x += window->x + 1;
    y += window->y + 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *cell = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        ((struct UiRenderWork *)base)->dirty = 1;
    }
}

/* resource/copy_fixed_block_b.c */
void Resource_CopyFixedBlockB(s32 arg0)
{
    VramBlock_LoadCached(arg0, 0x80, RomBytes_080310a4);
}

void Resource_CopyFixedBlockA(s32 arg0)
{
    VramBlock_LoadCached(arg0, 0x80, (s32)RomBytes_080317e4);
}

/* The Japanese edition has neither of the two text helpers that follow. */
#if EDITION_INTERNATIONAL
/* ui/text/text_set_render_string.c */
/* ui/text/misc/set_render_string.c */
s32 UiText_SetRenderString(const u8 *str)
{
    struct UiRenderWork *work;
    u16 *dst;
    s32 count;
    u32 width;
    u32 height;

    work = (struct UiRenderWork *)gWindowWork[0];
    count = 0;
    if (*str != 0) {
        dst = work->entries;
        do {
            *dst = *str;
            str++;
            dst++;
            count++;
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

/* The Japanese edition has its own kana editor, still on its scaffold. */
#if EDITION_INTERNATIONAL
/* Spanish and Italian names take one more column in the label window. */
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

/* German, Spanish and French write the delete and end keys as text over the
   keyboard; English and Italian draw them in the keyboard's tilemap. Both
   keys are three columns wide, except the German ENDE key, which takes four. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if defined(TBS_EDITION_DE)
#define NAME_END_KEY_WIDTH 4
#else
#define NAME_END_KEY_WIDTH 3
#endif

/* The cursor's sideways sway, eight frames long. */
extern s8 NameEntry_CaretWave[];
extern const u8 NameEntry_KeyboardTilemap[];
#if NAME_KEY_LABELS
extern u8 NameEntry_DeleteKeyLabel[];
extern u8 NameEntry_EndKeyLabel[];
void UiText_DrawStringInWindow(u8 *str, struct RenderInput *window, s32 x, s32 y);
#endif
extern u32 gFrameTick;
/* Read again for every key test. */
extern volatile u32 gKeysRepeat;
extern u32 gKeyState;

void Ui_LoadWindowGraphics(void);
s32 Localization_LookupEntryId(s32 entry);
s32 UiWindow_CreateWithSideObject(s32 entry, s32 side, s32 x, s32 y);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x0, s32 y0, s32 x1, s32 y1);
void ShopCursor_SetPositionImmediateFar(union MenuCursor *cursor, s32 x, s32 y);
void RenderOutput_PrepareForRedraw(struct RenderInput *window);
void Shop_SetCursorFar(union MenuCursor *cursor, s32 x, s32 y, s32 speed);
void ShopCursor_AdvanceFar(union MenuCursor *cursor);
void ShopCursor_MoveTowardTargetFar(union MenuCursor *cursor);
void UiWork_FinalizeEntityMatchingLocalizedId(s32 entry);

/* Edits an owner's name in place, five characters long, with the cursor
   state the shop menus use. Returns -1 when cancelled. */
s32 NameEntry_EditOwnerName(s32 entry)
{
    struct RenderInput *label;
    s32 result = 0;
    s32 original_length = 0;
    s32 length = 0;
    u8 text[16];
    u8 *input = text + 1;
    u8 *saved = (u8 *)Owner_GetStateFar(entry);
    u8 *work = gWindowWork[0];
    s32 text_dirty = 1;
    s32 cursor_dirty = 1;
    struct RenderInput *window;
    union MenuCursor cursor;
    union MenuCursor caret;
    s32 column, row, slot;
    u8 *end;
    u16 *pal;

    Ui_LoadWindowGraphics();
    window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 9, 2);
    label = (struct RenderInput *)UiWindow_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
    UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
    UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
    UiText_DrawStringInWindow(NameEntry_DeleteKeyLabel, window, 144, 32);
    UiText_DrawStringInWindow(NameEntry_EndKeyLabel, window, 144, 40);
#endif
    UiWindow_DrawDividerLine(window, 18, 0, 18, 7);
    ((struct UiRenderWork *)work)->dirty = text_dirty;
    text[0] = result;
    {
        s32 i;

        for (i = 0; i < 14; i++) {
            u32 c = saved[i];

            input[i] = c;
            if (c != 0) {
                original_length++;
                length++;
            }
        }
    }
    input[14] = 0;
    UiText_DrawPaddedLabel((s32)label, saved);
    slot = Resource_FindFreeEntry();
    column = 18;
    row = 5;
    if (slot <= 95) {
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
        slot = (s32)RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        cursor.output = (struct RenderOutput *)slot;
        ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 + 140, window->y * 8 + 52);
    }
    slot = Resource_FindFreeEntry();
    if (slot <= 95) {
        VramBlock_LoadCached(slot, 128, RomBytes_080317e4);
        slot = (s32)RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        caret.output = (struct RenderOutput *)slot;
        ((struct RenderOutput *)slot)->sentinel = 255;
        ((struct FieldSprite *)((struct RenderOutput *)slot)->unknown_10)->priority = 0;
        ShopCursor_SetPositionImmediateFar(&caret, UiText_SetRenderString(input) + 70, 22);
    }
    pal = (u16 *)0x050001c0;
    Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
    pal[4] = 0x6318;
    end = (u8 *)(original_length + (s32)input);
    for (;;) {
        s32 width = 1;

        if (column == 18) {
            if (row == 4)
                width = 3;
            if (row == 5)
                width = NAME_END_KEY_WIDTH;
        }
        UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
        if (cursor_dirty) {
            cursor_dirty = 0;
            Shop_SetCursorFar(&cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
        }
        if (text_dirty) {
            text_dirty = 0;
            Shop_SetCursorFar(&caret, UiText_SetRenderString(input) + 70, 22, 3);
        }
        ShopCursor_AdvanceFar(&cursor);
        ShopCursor_MoveTowardTargetFar(&caret);
        {
            u32 frame = (gFrameTick >> 1) & 7;
            /* FAKEMATCH: the caret sprite lives in r5, as the native allocates it. */
            register struct RenderOutput *o asm("r5") = caret.output;

            ((struct FieldSprite *)o->unknown_10)->x = o->x + NameEntry_CaretWave[frame];
            ((struct FieldSprite *)o->unknown_10)->y = o->y + NameEntry_CaretWave[(frame + 5) & 7];
        }
        if (gKeysRepeat & KEY_UP) {
            Audio_PlayCue(111);
            cursor_dirty = 1;
            row--;
            if (column != 18) {
                if (row == -1)
                    row = 5;
            } else {
                row = (row != 3);
                row = 5 - row;
            }
        }
        if (gKeysRepeat & KEY_DOWN) {
            Audio_PlayCue(111);
            cursor_dirty = 1;
            row++;
            if (column != 18) {
                if (row == 6)
                    row = 0;
            } else {
                row = (row != 6);
                row += 4;
            }
        }
        if (gKeysRepeat & KEY_LEFT) {
            Audio_PlayCue(111);
            cursor_dirty = 1;
            column--;
            if (column == -1) {
                column = 18;
                if ((u32)(row - 4) > 1)
                    column = 16;
            } else if (column == 5 || column == 11 || column == 17) {
                column--;
            }
        }
        if (gKeysRepeat & KEY_RIGHT) {
            Audio_PlayCue(111);
            cursor_dirty = 1;
            column++;
            if (column == 19)
                column = 0;
            else if (column == 5 || column == 11 || column == 17)
                column++;
            if (column == 18 && (u32)(row - 4) > 1)
                column = 0;
        }
        if (gKeyState & KEY_START) {
            Audio_PlayCue(111);
            cursor_dirty = 1;
            column = 18;
            row = 5;
        }
        if (gKeysRepeat & KEY_B) {
            Audio_PlayCue(113);
remove_character:
            if (length != 0) {
                length--;
                *--end = 0;
                RenderOutput_PrepareForRedraw(label);
                UiText_DrawPaddedLabel((s32)label, input);
                text_dirty = 1;
                continue;
            }
            result = -1;
            goto finish;
        }
        if (!(gKeysRepeat & KEY_A))
            continue;
        Audio_PlayCue(112);
        if (column == 18) {
            if (row == 5) {
                if (length == 0) {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawPaddedLabel((s32)label, saved);
                    WaitFrames(10);
                } else {
                    /* FAKEMATCH: the destination lives in r2, as the native allocates it. */
                    register u8 *dst asm("r2") = saved;
                    /* FAKEMATCH: the source lives in r1, as the native allocates it. */
                    register u8 *src asm("r1") = input;
                    s32 i = 0;

                    while (i < 15) {
                        *dst++ = *src++;
                        i++;
                    }
                }
                goto finish;
            }
            if (row == 4)
                goto remove_character;
            continue;
        }
        {
            u32 tx = window->x + column + 1;
            u32 cell;
            u32 character;

            /* FAKEMATCH: keeps the column term whole, as the native adds it. */
            __asm__("" : "+r"(tx));
            cell = ((window->y + row + 1) << 5) + tx;
            character = work[cell * 2];
            if (length == 5)
                continue;
            *end++ = character;
            *end = 0;
            length++;
            if (length == 5) {
                column = 18;
                row = 5;
                cursor_dirty = 1;
            }
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawPaddedLabel((s32)label, input);
            text_dirty = 1;
        }
    }
finish:
    UiWork_Finalize((struct UiWindow *)window, 2);
    UiWork_Finalize((struct UiWindow *)label, 2);
    UiWork_FinalizeEntityMatchingLocalizedId(entry);
    WaitFrames(1);
    return result;
}
#endif
