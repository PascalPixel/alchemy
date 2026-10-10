/* Pin-free NameEntry_EditOwnerName trial: EN/IT 506 native/built, 501 common;
 * JA 663 native/built, 658 common; DE/ES/FR 516 native/built, 511 common;
 * differ 10 in all six editions. A one-pass block
 * around the byte assignment keeps that CSE region inside the copy loop,
 * improving the ordinary pointer trial from 14 differences. Const or
 * restrict pointers, an indexed copy and an explicit byte temporary retain
 * 14. Unsigned counters and a do/while copy differ 16; reusing either outer
 * length variable differs 41. An inline source-first copy helper differs
 * 12, while count parameters restore 14; builtin memcpy differs 34. A
 * two-minute seed-409 prefix search compiled 4909 candidates and found
 * this single block rewrite, without a zero. Declaring src before dst
 * further reduces 12 to 10; counter-first declarations do not improve it.
 * An unsigned-byte counter differs 40; comparing against 15 with !=
 * restores 14. Linked source retains the
 * edition-specific bindings pending whole-image identity. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "mem.h"
#include "types.h"
#include "window.h"
#include "scene.h"
#include "resource.h"
#include "render_input.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "runtime_interfaces.h"
#include "io_reg.h"
#include "dma.h"
#include "field_sprite.h"
#include "workspace_options.h"
#include "btlsys.h"
void AudioCommand_PlayFar(s32 cue);

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define gKeyState (*(u32 *)&gKeyState)

#define ShopCursor_SetPositionImmediateFar ((void (*)(union MenuCursor*,s32,s32))ShopCursor_SetPositionImmediateFar)
#define Shop_SetCursorFar ((void (*)(union MenuCursor*,s32,s32,s32))Shop_SetCursorFar)
#define ShopCursor_AdvanceFar ((void (*)(union MenuCursor*))ShopCursor_AdvanceFar)
#define ShopCursor_MoveTowardTargetFar ((void (*)(union MenuCursor*))ShopCursor_MoveTowardTargetFar)
#define UiWork_FinalizeEntityMatchingLocalizedId ((void (*)(s32))UiWork_FinalizeEntityMatchingLocalizedId)

s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 Resource_ReturnTrue(void);
s32 NameEntry_AcceptsMark(s32 mark, s32 letter);
s32 NameEntry_CountLetters(const u8 *name);
void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source);
void Resource_CopyFixedBlockB(s32 arg0);
void Resource_CopyFixedBlockA(s32 arg0);
s32 UiText_SetRenderString(const u8 *str);
void UiText_DrawPaddedLabel(s32 output, u8 *input);
extern u32 gFrameTick;

s32 UiWindow_CreateWithSideObject(s32 entry, s32 side, s32 x, s32 y);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x0, s32 y0, s32 x1, s32 y1);
void RenderOutput_PrepareForRedraw(struct RenderInput *window);

#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

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

#if NAME_KEY_LABELS
void UiText_DrawStringInWindow(u8 *str, struct RenderInput *window, s32 x, s32 y);
#endif

s32 NameEntry_EditOwnerName(s32 entry)
{
    struct RenderInput *label;
    s32 result = 0;
    s32 original_length = 0;
    s32 length = 0;
    u8 text[16];
    u8 *input = text + 1;
    u8 *saved = (u8 *)UnitGetFar(entry);
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
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockATiles);
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

            /* FAKEMATCH: needed to match: slot carries the caret sprite here too; a pointer of its own takes r4 instead of r5. */
            slot = (s32)caret.output;
            ((struct FieldSprite *)((struct RenderOutput *)slot)->unknown_10)->x = ((struct RenderOutput *)slot)->x + NameEntry_CaretWave[frame];
            ((struct FieldSprite *)((struct RenderOutput *)slot)->unknown_10)->y = ((struct RenderOutput *)slot)->y + NameEntry_CaretWave[(frame + 5) & 7];
        }
        if (gKeysRepeat & KEY_UP) {
            AudioCommand_PlayFar(111);
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
            AudioCommand_PlayFar(111);
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
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            column--;
            if (column == -1) {
                column = 18;
                if (row < 4 || row > 5)
                    column = 16;
            } else if (column == 5 || column == 11 || column == 17) {
                column--;
            }
        }
        if (gKeysRepeat & KEY_RIGHT) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            column++;
            if (column == 19)
                column = 0;
            else if (column == 5 || column == 11 || column == 17)
                column++;
            if (column == 18 && (row < 4 || row > 5))
                column = 0;
        }
        if (gKeyState & KEY_START) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            column = 18;
            row = 5;
        }
        if (gKeysRepeat & KEY_B) {
            AudioCommand_PlayFar(113);
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
        AudioCommand_PlayFar(112);
        if (column == 18) {
            if (row == 5) {
                if (length == 0) {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawPaddedLabel((s32)label, saved);
                    WaitFrames(10);
                } else {
                    u8 *src = input;
                    u8 *dst = saved;
                    s32 i = 0;

                    while (i < 15) {
                        /* FAKEMATCH: keep the byte copy inside its own loop.c region; ordinary copying differs 14; this block and source-first declarations differ 10. */
                        do { *dst++ = *src++; } while (0);
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
#else

#undef gKeyState
#define gKeyState (*(volatile u32 *)&gKeyState)

s32 NameEntry_EditOwnerName(s32 entry)
{
    struct RenderInput *label;
    s32 result = 0;
    s32 keyboard = 0;
    s32 bytes = 0;
    s32 length = 0;
    u8 text[16];
    u8 *input = text + 1;
    u8 *saved = (u8 *)UnitGetFar(entry);
    u8 *work = gWindowWork[0];
    s32 text_dirty = 1;
    s32 cursor_dirty = 1;
    struct RenderInput *window;
    union MenuCursor cursor;
    union MenuCursor caret;
    s32 column, row, slot, x;
    u32 character;
    u16 *pal;

    Ui_LoadWindowGraphics();
    window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 13, 2);
    label = (struct RenderInput *)UiWindow_Create(8, 3, 8, 3, 2);
    UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
    UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
    UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
    ((struct UiRenderWork *)work)->dirty = text_dirty;
    text[0] = result;
    {
        s32 i;

        for (i = 0; i < 14; i++) {
            character = saved[i];

            input[i] = character;
            if (character != 0) {
                bytes++;
                length++;
                if ((u8)(character - 0xde) <= 1)
                    length--;
            }
        }
    }
    input[14] = 0;
    UiText_DrawString(saved, (s32)label, 0, 0);
    column = 18;
    slot = Resource_FindFreeEntry();
    row = 10;
    if (slot <= 95) {
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
        slot = (s32)RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        cursor.output = (struct RenderOutput *)slot;
        ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 + 140, window->y * 8 + 92);
    }
    slot = Resource_FindFreeEntry();
    if (slot <= 95) {
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockATiles);
        slot = (s32)RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        caret.output = (struct RenderOutput *)slot;
        ((struct RenderOutput *)slot)->sentinel = 255;
        ((struct FieldSprite *)((struct RenderOutput *)slot)->unknown_10)->priority = 0;
        x = 72 + NameEntry_CountLetters(input) * 10;
        ShopCursor_SetPositionImmediateFar(&caret, x, 22);
    }
    pal = (u16 *)0x050001c0;
    Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
    pal[4] = 0x6318;
    for (;;) {
        s32 width = 1;

        if (column == 18) {
            if (row == 0)
                width = 4;
            if (row == 8)
                width = 3;
            if (row == 10)
                width = 3;
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
            x = 72 + NameEntry_CountLetters(input) * 10;
            Shop_SetCursorFar(&caret, x, 22, 3);
        }
        ShopCursor_AdvanceFar(&cursor);
        ShopCursor_MoveTowardTargetFar(&caret);
        {
            u32 frame = (gFrameTick >> 1) & 7;

            /* FAKEMATCH: needed to match: slot carries the caret sprite here too; a pointer of its own takes r4 instead of r5. */
            slot = (s32)caret.output;
            ((struct FieldSprite *)((struct RenderOutput *)slot)->unknown_10)->x = ((struct RenderOutput *)slot)->x + NameEntry_CaretWave[frame];
            ((struct FieldSprite *)((struct RenderOutput *)slot)->unknown_10)->y = ((struct RenderOutput *)slot)->y + NameEntry_CaretWave[(frame + 5) & 7];
        }
        if (gKeysRepeat & KEY_UP) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            row--;
            if (column != 18) {
                if (row == -1)
                    row = 10;
                else if (row == 5)
                    row = 4;
            } else {
                if (row == -1) {
                    row = 10;
                } else {
                    row = (row != 7);
                    row *= 8;
                }
            }
        }
        if (gKeysRepeat & KEY_DOWN) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            row++;
            if (column != 18) {
                if (row == 11)
                    row = 0;
                if (row == 5)
                    row = 6;
            } else {
                if (row == 1)
                    row = 8;
                else if (row == 9)
                    row = 10;
                else
                    row = 0;
            }
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            column--;
            if (column == -1) {
                if (row == 0 || row == 8 || row == 10)
                    column = 18;
                else
                    column = 16;
            } else if (column == 5 || column == 11 || column == 17) {
                column--;
            }
        }
        if (gKeysRepeat & KEY_RIGHT) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            column++;
            if (column == 19)
                column = 0;
            else if (column == 5 || column == 11 || column == 17)
                column++;
            if (column == 18) {
                if (row == 0 || row == 8 || row == 10)
                    column = 18;
                else
                    column = 0;
            }
        }
        if (gKeyState & KEY_START) {
            AudioCommand_PlayFar(111);
            cursor_dirty = 1;
            column = 18;
            row = 10;
        }
        if (gKeyState & KEY_SELECT) {
            AudioCommand_PlayFar(111);
change_keyboard:
            keyboard ^= 1;
            if (keyboard)
                UiWindow_CopyTilemapRegion(window, NameEntry_AlternateKeyboardTilemap);
            else
                UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
            UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
            ((struct UiRenderWork *)work)->dirty = 1;
            continue;
        }
        if (gKeysRepeat & KEY_B) {
            AudioCommand_PlayFar(113);
remove_character:
            if (length != 0) {
                length--;
                bytes--;
                character = input[bytes];
                input[bytes] = 0;
                if ((u8)(character - 0xde) <= 1) {
                    bytes--;
                    input[bytes] = 0;
                }
            } else {
                result = -1;
                goto finish;
            }
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawString(input, (s32)label, 0, 0);
            text_dirty = 1;
            continue;
        }
        if (!(gKeysRepeat & KEY_A))
            continue;
        AudioCommand_PlayFar(112);
        if (column == 18) {
            if (row == 10) {
                if (length == 0) {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawString(saved, (s32)label, 0, 0);
                    WaitFrames(10);
                } else {
                    u8 *src = input;
                    u8 *dst = saved;
                    s32 i = 0;

                    while (i < 15) {
                        /* FAKEMATCH: keep the byte copy inside its own loop.c region; ordinary copying differs 14; this block and source-first declarations differ 10. */
                        do { *dst++ = *src++; } while (0);
                        i++;
                    }
                }
                goto finish;
            }
            if (row == 8)
                goto remove_character;
            if (row == 0)
                goto change_keyboard;
            continue;
        }
        {
            u32 tx = window->x + column + 1;
            u32 cell;
            u8 *dst;

            cell = ((window->y + row + 1) << 5) + tx;
            character = work[cell * 2];
            if (character == 0xfe)
                character = 0xde;
            if (character == 0xff)
                character = 0xdf;
            if (length == 5 && (u8)(character - 0xde) > 1)
                continue;
            dst = input + bytes;
            if (!NameEntry_AcceptsMark(character, input[bytes - 1])) {
                AudioCommand_PlayFar(113);
                continue;
            }
            *dst = character;
            bytes++;
            input[bytes] = 0;
            length++;
            if ((u8)(character - 0xde) <= 1)
                length--;
            if (length == 5) {
                column = 18;
                row = 10;
                cursor_dirty = 1;
            }
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawString(input, (s32)label, 0, 0);
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
