/* Japanese NameEntry_EditOwnerName draft; native extent 0x5e0.
   alchemy diff --edition tbs-ja: native663/draft672/common641/differ53.
   The108-byte local frame matches; this draft is not linked.
   2026-10-06 session 2 diagnosis: the last-letter pointer is not a user
   variable but loop.c's reduced giv of input+bytes-1 (its spill slot
   [sp,#12] lies below &cursor's [sp,#16], so its pseudo is newer than
   gcse's &cursor), hence the indexed input[bytes] source. Global
   priorities floor_log2(refs)*refs/live were &caret3040, cursor flag
   2954, giv3086: the cursor flag and &caret refs sit in blocks of loop
   depth 6 (one natural loop per latch). A seventh latch (the remove path
   redraws and continues itself, as the international source does) makes
   them depth 7: &caret r9, cursor flag fp, giv spilled, as native.
   Steps kept: indexed bytes216 (identical code), right-key wrap as
   0/8/10 -> 18 else 0 212, duplicated remove-path redraw107, row != 7 then
   *= 8 in two statements103, volatile key-state view for START and
   SELECT91, copy pointers pinned r1/r2 as international79 (natural
   orders: count first93), caret x held in a function-scope x (global
   pseudo, so the multiply chain keeps r3 and reload copies to r1)53;
   slot for x61, x = count*10 with x+72 in the call71, a block-local x79.
   Remaining: the remove-path redraw tail is not cross-jumped into the add
   path's (its text_dirty constant reloads into r4 where the add tail
   uses r1, and its two argument loads swap), so 9 extra instructions and
   every later branch offset differ. A single goto redraw tail gives168
   (the allocation falls back). Both tails reload their constant 1 into
   r3 first; choose_reload_regs' round-robin then gives the remove tail r4
   and the result path r1, where the native result path has r4. That
   rotation suggests the native has no separate remove-path reload there,
   so its seventh latch may come from elsewhere: still open. Trials,
   reverted: keyboard toggle duplicated in the A path with the duplicated
   tail178, with a goto redraw tail279. Permuter (5 min, seed 1): length++
   before bytes++ in the add path gives71 (draft662): jump2 now merges the
   two redraw tails, but on the remove tail's r4 constant, and the add
   path's increments reorder; the native merged tail keeps r1, so both
   tails must reload into r1 while the result path takes r4 after them.
   The empty-name exit tested first (result -1 before the removal)96.
   Natural trials: pointer copy loop323; indexed loop320; one shared
   character local254; count order and side-key navigation228. The existing
   international caret-r5 device gives218. Pinning cursor flag alone333,
   caret control430 or both343 was reverted; integer last-pointer218 and
   volatile last-pointer383 were also reverted. All scores are normalized
   instruction differences, not complete-byte proof.
   2026-10-06 allocation map: native row r7, column r8, caret control r9,
   window sl, cursor flag fp, last-letter pointer [sp,#12], cursor control
   [sp,#16]; the draft gives the last-letter pointer r9, caret control fp
   and spills the cursor flag to [sp,#16], so the pointer outranks three
   loop-wide values it falls below natively. Trials, all reverted: dirty
   flags swapped226; last pointer declared first or beside cursor flag218;
   input+bytes-1 and &input[bytes-1]222; count*10+72218; row shifted or
   multiplied in a second statement224, ternary or 8*(...)218, row-=7
   first225; read-then-post-decrement removal211 (both halves213, second
   half alone259) but it moves the decrement after the read where the
   native subtracts first, so it is not kept; indexed save copy222;
   register keyword on cursor flag218; cursor flag pinned to fp219 (the
   pointer still takes r9 and caret control spills), and with the caret r5
   device removed227; while (1) loop218; label-and-goto loop506; caret
   x as 72+10*count, count*10+70+2218, an s16 cast407.
   Kept: the save copy declares its source before its destination
   (differ216, source now r2 as native; destination r0 and count r1 still
   swap the native r1 and r0). The count declared first or between,
   counted before the copy, or a for loop also216; destination first218;
   do-while copy218; split increments or a byte temporary216.
   UiText_DrawPaddedLabel, UiText_SetRenderString and Resource_ReturnTrue
   are not in the Japanese map: Japanese NAME.C links NameEntry_AcceptsMark
   and NameEntry_CountLetters, and this function draws with
   UiText_DrawString and measures with NameEntry_CountLetters.
   JA physical labels for both keyboard tilemaps and the caret wave are
   still needed before an adoption; the source uses semantic symbols. */
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
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

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define gKeyState (*(volatile u32 *)&gKeyState)

#define ShopCursor_SetPositionImmediateFar ((void (*)(union MenuCursor*,s32,s32))ShopCursor_SetPositionImmediateFar)
#define Shop_SetCursorFar ((void (*)(union MenuCursor*,s32,s32,s32))Shop_SetCursorFar)
#define ShopCursor_AdvanceFar ((void (*)(union MenuCursor*))ShopCursor_AdvanceFar)
#define ShopCursor_MoveTowardTargetFar ((void (*)(union MenuCursor*))ShopCursor_MoveTowardTargetFar)
#define UiWork_FinalizeEntityMatchingLocalizedId ((void (*)(s32))UiWork_FinalizeEntityMatchingLocalizedId)

extern u8 Resource_FixedBlockBTiles[];



void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

extern u32 gFrameTick;
extern const u8 NameEntry_AlternateKeyboardTilemap[];
s32 Localization_LookupEntryId(s32 entry);
s32 UiWindow_CreateWithSideObject(s32 entry, s32 side, s32 x, s32 y);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x0, s32 y0, s32 x1, s32 y1);
void RenderOutput_PrepareForRedraw(struct RenderInput *window);
s32 NameEntry_CountLetters(const u8 *name);
s32 NameEntry_AcceptsMark(s32 mark, s32 letter);

s32 NameEntry_EditOwnerName(s32 entry)
{
    struct RenderInput *label;
    s32 result = 0;
    s32 keyboard = 0;
    s32 bytes = 0;
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
        VramBlock_LoadCached(slot, 128, RomBytes_080317e4);
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
            /* FAKEMATCH: native/international caret r5 gives differ216; natural r4 gives226. */
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
            Audio_PlayCue(111);
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
            Audio_PlayCue(111);
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
            Audio_PlayCue(111);
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
            Audio_PlayCue(111);
            cursor_dirty = 1;
            column = 18;
            row = 10;
        }
        if (gKeyState & KEY_SELECT) {
            Audio_PlayCue(111);
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
            Audio_PlayCue(113);
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
                RenderOutput_PrepareForRedraw(label);
                UiText_DrawString(input, (s32)label, 0, 0);
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
            if (row == 10) {
                if (length == 0) {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawString(saved, (s32)label, 0, 0);
                    WaitFrames(10);
                } else {
                    /* FAKEMATCH: the destination lives in r1, as the native allocates it. */
                    register u8 *dst asm("r1") = saved;
                    /* FAKEMATCH: the source lives in r2, as the native allocates it. */
                    register u8 *src asm("r2") = input;
                    s32 i = 0;

                    while (i < 15) {
                        *dst++ = *src++;
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
                Audio_PlayCue(113);
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
